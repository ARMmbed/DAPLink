/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "RTL.h"
#include "rl_usb.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include "main.h"
#include "board.h"
#include "gpio.h"
#include "uart.h"
#include "serial.h"
#include "tasks.h"
#include "target_reset.h"
#include "swd_host.h"
#include "version.h"
#include "virtual_fs.h"
#include "config_settings.h"

// Event flags for main task
// Timers events
#define FLAGS_MAIN_90MS                 (1 << 0)
#define FLAGS_MAIN_30MS                 (1 << 1)
// Reset events
#define FLAGS_MAIN_RESET                (1 << 2)
// USB Events
#define FLAGS_MAIN_MSC_DISCONNECT       (1 << 3)
#define FLAGS_MAIN_FORCE_MSC_DISCONNECT (1 << 7)
// Other Events
#define FLAGS_MAIN_POWERDOWN            (1 << 4)
#define FLAGS_MAIN_DISABLEDEBUG         (1 << 5)
// Used by msd when flashing a new binary
#define FLAGS_LED_BLINK_30MS            (1 << 6)
// Timing constants (in 90mS ticks)
// USB busy time
#define USB_BUSY_TIME           (33)
// Delay before a USB device connect may occur
#define USB_CONNECT_DELAY       (11)
// Delay before target may be taken out of reset or reprogrammed after startup
#define STARTUP_DELAY           (1)
// Decrement to zero
#define DECZERO(x)              (x ? --x : 0)

// Reference to our main task
OS_TID main_task_id;
OS_TID serial_task_id;

// USB busy LED state; when TRUE the LED will flash once using 30mS clock tick
static uint8_t hid_led_usb_activity = 0;
static uint8_t cdc_led_usb_activity = 0;
static uint8_t msc_led_usb_activity = 0;
static main_led_state_t hid_led_state = MAIN_LED_FLASH;
static main_led_state_t cdc_led_state = MAIN_LED_FLASH;
static main_led_state_t msc_led_state = MAIN_LED_FLASH;

// Global state of usb
main_usb_connect_t usb_state;

static main_usb_busy_t usb_busy;
static uint32_t usb_busy_count;

static U64 stk_timer_30_task[TIMER_TASK_30_STACK/8];
static U64 stk_dap_task[DAP_TASK_STACK/8];
static U64 stk_serial_task[SERIAL_TASK_STACK/8];
static U64 stk_main_task[MAIN_TASK_STACK/8];

// Timer task, set flags every 30mS and 90mS
__task void timer_task_30mS(void)
{
    uint8_t i = 0;
    os_itv_set(3); // 30mS

    while(1) {
        os_itv_wait();
        os_evt_set(FLAGS_MAIN_30MS, main_task_id);
        if (!(i++ % 3)) {
            os_evt_set(FLAGS_MAIN_90MS, main_task_id);
        }
    }
}

// Forward reset from the user pressing the reset button
// Boards which tie the reset pin directly to the target
// should override this function with a stub that does nothing
__attribute__((weak))
void target_forward_reset(bool assert_reset)
{
    if (assert_reset) {
        target_set_state(RESET_HOLD);
    } else {
        target_set_state(RESET_RUN);
    }
}

// Functions called from other tasks to trigger events in the main task
// parameter should be reset type??
void main_reset_target(uint8_t send_unique_id)
{
    os_evt_set(FLAGS_MAIN_RESET, main_task_id);
    return;
}

// Flash HID LED using 30mS tick
void main_blink_hid_led(main_led_state_t permanent)
{
    hid_led_usb_activity = 1;
    hid_led_state = (permanent) ? MAIN_LED_FLASH_PERMANENT : MAIN_LED_FLASH;
    return;
}

// Flash CDC LED using 30mS tick
void main_blink_cdc_led(main_led_state_t permanent)
{
    cdc_led_usb_activity = 1;
    cdc_led_state = (permanent) ? MAIN_LED_FLASH_PERMANENT : MAIN_LED_FLASH;
    return;
}

// Flash MSC LED using 30mS tick
void main_blink_msc_led(main_led_state_t permanent)
{
    msc_led_usb_activity = 1;
    msc_led_state = (permanent) ? MAIN_LED_FLASH_PERMANENT : MAIN_LED_FLASH;
    return;
}

// MSC data transfer in progress
void main_usb_busy_event(void) {
    usb_busy_count = USB_BUSY_TIME;
    usb_busy = USB_ACTIVE;
    //os_evt_set(FLAGS_MAIN_USB_BUSY, main_task_id);
    return;
}

// A new program has been flashed in the target
void main_msc_disconnect_event(void)
{
    os_evt_set(FLAGS_MAIN_MSC_DISCONNECT, main_task_id);
    return;
}

// A failure has occured during msc programming
void main_force_msc_disconnect_event(void)
{
    os_evt_set(FLAGS_MAIN_FORCE_MSC_DISCONNECT, main_task_id);
    return;
}

// Power down the interface
void main_powerdown_event(void)
{
    os_evt_set(FLAGS_MAIN_POWERDOWN, main_task_id);
    return;
}

// Disable debug on target
void main_disable_debug_event(void)
{
    os_evt_set(FLAGS_MAIN_DISABLEDEBUG, main_task_id);
    return;
}

os_mbx_declare(serial_mailbox, 20);
#define SIZE_DATA (64)
static uint8_t data[SIZE_DATA];

#ifdef CDC_ENDPOINT
__task void serial_process()
{
    UART_Configuration config;
    int32_t len_data = 0;
    void *msg;

    while (1) {
        // Check our mailbox to see if we need to set anything up with the UART
        // before we do any sending or receiving
        if (os_mbx_wait(&serial_mailbox, &msg, 0) == OS_R_OK) {
            switch((SERIAL_MSG)(unsigned)msg) {
                case SERIAL_INITIALIZE:
                    uart_initialize();
                    break;

                case SERIAL_UNINITIALIZE:
                    uart_uninitialize();
                    break;

                case SERIAL_RESET:
                    uart_reset();
                    break;

                case SERIAL_SET_CONFIGURATION:
                    serial_get_configuration(&config);
                    uart_set_configuration(&config);
                    break;

                default:
                    break;
            }
        }

        len_data = USBD_CDC_ACM_DataFree();
        if (len_data > SIZE_DATA) {
            len_data = SIZE_DATA;
        }
        if (len_data) {
            len_data = uart_read_data(data, len_data);
        }
        if (len_data) {
            if(USBD_CDC_ACM_DataSend(data , len_data)) {
                main_blink_cdc_led(MAIN_LED_OFF);
            }
        }

        len_data = uart_write_free();
        if (len_data > SIZE_DATA) {
            len_data = SIZE_DATA;
        }
        if (len_data) {
            len_data = USBD_CDC_ACM_DataRead(data, len_data);
        }
        if (len_data) {
            if (uart_write_data(data, len_data)) {
                main_blink_cdc_led(MAIN_LED_OFF);
            }
        }
    }
}
#endif

extern __task void hid_process(void);
__attribute__((weak)) void prerun_target_config(void){}

__task void main_task(void)
{
    // State processing
    uint16_t flags = 0;
    // LED
    gpio_led_state_t hid_led_value = GPIO_LED_ON;
    gpio_led_state_t cdc_led_value = GPIO_LED_ON;
    gpio_led_state_t msc_led_value = GPIO_LED_ON;
    // USB
    uint32_t usb_state_count = USB_BUSY_TIME;
    // thread running after usb connected started
    uint8_t thread_started = 0;
    // button state
    main_reset_state_t main_reset_button_state = MAIN_RESET_RELEASED;

    // Initialize settings
    config_init();

    // Initialize our serial mailbox
    os_mbx_init(&serial_mailbox, sizeof(serial_mailbox));
    // Get a reference to this task
    main_task_id = os_tsk_self();

    // leds
    gpio_init();
    // Turn off LED
    gpio_set_hid_led(GPIO_LED_ON);
    gpio_set_cdc_led(GPIO_LED_ON);
    gpio_set_msc_led(GPIO_LED_ON);

    // do some init with the target before USB and files are configured
    prerun_target_config();

    // Update HTML version information file
    init_auth_config();

    // USB
    usbd_init();
    usbd_connect(0);
    usb_busy = USB_IDLE;
    usb_busy_count = 0;
    usb_state = USB_CONNECTING;
    usb_state_count = USB_CONNECT_DELAY;

    // Start timer tasks
    os_tsk_create_user(timer_task_30mS, TIMER_TASK_30_PRIORITY, (void *)stk_timer_30_task, TIMER_TASK_30_STACK);

    // Target running
    target_set_state(RESET_RUN);

    while(1) {
        os_evt_wait_or(   FLAGS_MAIN_RESET              // Put target in reset state
                        | FLAGS_MAIN_90MS               // 90mS tick
                        | FLAGS_MAIN_30MS               // 30mS tick
                        | FLAGS_MAIN_POWERDOWN          // Power down interface
                        | FLAGS_MAIN_DISABLEDEBUG       // Disable target debug
                        | FLAGS_MAIN_MSC_DISCONNECT    // programming complete - disconnect msc endpoint
                        | FLAGS_MAIN_FORCE_MSC_DISCONNECT,  // programming failure - force disconnect msc endpoint
                        NO_TIMEOUT);

        // Find out what event happened
        flags = os_evt_get();

        if (flags & FLAGS_MAIN_MSC_DISCONNECT) {
            // auto reset the board if configured to do so
            if (config_get_auto_rst()) {
                target_set_state(RESET_RUN);
            }
            usb_busy = USB_IDLE;                    // USB not busy
            usb_state_count = USB_CONNECT_DELAY;
            usb_state = USB_DISCONNECT_CONNECT;     // disconnect the usb
        }

        if (flags & FLAGS_MAIN_FORCE_MSC_DISCONNECT) {
            usb_busy = USB_IDLE;                    // USB not busy
            usb_state_count = 0;
            usb_state = USB_DISCONNECT_CONNECT;     // disconnect the usb
        }

        if (flags & FLAGS_MAIN_RESET) {
            target_set_state(RESET_RUN);
        }

        if (flags & FLAGS_MAIN_POWERDOWN) {
            // Disable debug
            target_set_state(NO_DEBUG);
            // Disconnect USB
            usbd_connect(0);
            // Turn off LED
            gpio_set_hid_led(GPIO_LED_OFF);
            gpio_set_cdc_led(GPIO_LED_OFF);
            gpio_set_msc_led(GPIO_LED_OFF);
            // TODO: put the interface chip in sleep mode
            while(1);
        }

        if (flags & FLAGS_MAIN_DISABLEDEBUG) {
            // Disable debug
            target_set_state(NO_DEBUG);
        }

        if (flags & FLAGS_MAIN_90MS) {
            // Update USB busy status
            switch (usb_busy) {
                case USB_ACTIVE:
                    if (DECZERO(usb_busy_count) == 0) {
                        usb_busy=USB_IDLE;
                    }
                    break;

                case USB_IDLE:
                default:
                    break;
            }
            // Update USB connect status
            switch (usb_state) {
                case USB_DISCONNECTING:
                    // Wait until USB is idle before disconnecting
                    if (usb_busy == USB_IDLE) {
                        usb_state = USB_DISCONNECTED;
                    }
                    break;

                case USB_DISCONNECT_CONNECT:
                    // Wait until USB is idle before disconnecting
                    if ((usb_busy == USB_IDLE) && (DECZERO(usb_state_count) == 0)) {
                        usb_state = USB_CONNECTING;
                        // Delay the connecting state before reconnecting to the host - improved usage with VMs
                        usb_state_count = USB_BUSY_TIME;
                        USBD_MSC_MediaReady = 0;
                    }
                    break;

                case USB_CONNECTING:
                    // Wait before connecting
                    if (DECZERO(usb_state_count) == 0) {
                        usbd_connect(1);
                        usb_state = USB_CHECK_CONNECTED;
                    }
                    break;

                case USB_CHECK_CONNECTED:
                    if(usbd_configured()) {
                        if (!thread_started) {
                            os_tsk_create_user(hid_process, DAP_TASK_PRIORITY, (void *)stk_dap_task, DAP_TASK_STACK);
#ifdef CDC_ENDPOINT
                            serial_task_id = os_tsk_create_user(serial_process, SERIAL_TASK_PRIORITY, (void *)stk_serial_task, SERIAL_TASK_STACK);
#endif
                            thread_started = 1;
                        }
                        usb_state = USB_CONNECTED;
                        reset_file_transfer_state();
                        USBD_MSC_MediaReady = 1;
                    }
                    break;

                case USB_CONNECTED:
                case USB_DISCONNECTED:
                default:
                    break;
            }
         }

        // 30mS tick used for flashing LED when USB is busy
        if (flags & FLAGS_MAIN_30MS) {
            // handle reset button without eventing
            switch (main_reset_button_state) {
                default:
                case MAIN_RESET_RELEASED:
                    if (0 == gpio_get_sw_reset()) {
                        main_reset_button_state = MAIN_RESET_PRESSED;
                        target_forward_reset(true);
                    }
                    break;

                case MAIN_RESET_PRESSED:
                    // ToDo: add a counter to do a mass erase or target recovery after xxx seconds of being held
                    if (1 == gpio_get_sw_reset()) {
                        main_reset_button_state = MAIN_RESET_TARGET;
                    }
                    break;

                case MAIN_RESET_TARGET:
                    target_forward_reset(false);
                    main_reset_button_state = MAIN_RESET_RELEASED;
                    break;
            }

            if (hid_led_usb_activity && ((hid_led_state == MAIN_LED_FLASH) || (hid_led_state == MAIN_LED_FLASH_PERMANENT))) {
                // Flash DAP LED ONCE
                if (hid_led_value) {
                    hid_led_value = GPIO_LED_OFF;
                } else {
                    hid_led_value = GPIO_LED_ON; // Turn on
                    if (hid_led_state == MAIN_LED_FLASH) {
                        hid_led_usb_activity = 0;
                    }
                }

                // Update hardware
                gpio_set_hid_led(hid_led_value);
            }

            if (msc_led_usb_activity && ((msc_led_state == MAIN_LED_FLASH) || (msc_led_state == MAIN_LED_FLASH_PERMANENT))) {
                // Flash MSD LED ONCE
                if (msc_led_value) {
                    msc_led_value = GPIO_LED_OFF;
                } else {
                    msc_led_value = GPIO_LED_ON; // Turn on
                    if (msc_led_state == MAIN_LED_FLASH) {
                        msc_led_usb_activity = 0;
                    }
                }

                // Update hardware
                gpio_set_msc_led(msc_led_value);
            }

            if (cdc_led_usb_activity && ((cdc_led_state == MAIN_LED_FLASH) || (cdc_led_state == MAIN_LED_FLASH_PERMANENT))) {
                // Flash CDC LED ONCE
                if (cdc_led_value) {
                    cdc_led_value = GPIO_LED_OFF;
                } else {
                    cdc_led_value = GPIO_LED_ON; // Turn on
                    if (cdc_led_state == MAIN_LED_FLASH) {
                        cdc_led_usb_activity = 0;
                    }
                }

                // Update hardware
                gpio_set_cdc_led(cdc_led_value);
            }

        }
    }
}

int main (void)
{
    os_sys_init_user(main_task, MAIN_TASK_PRIORITY, stk_main_task, MAIN_TASK_STACK);
}
