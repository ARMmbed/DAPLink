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
#include <RTL.h>
#include "rl_usb.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "main.h"
#include "board.h"
#include "gpio.h"
#include "uart.h"
#include "semihost.h"
#include "serial.h"
#include "tasks.h"

#include "target_reset.h"
#include "swd_host.h"
#include "version.h"
#ifdef BOARD_UBLOX_C027
#include <LPC11Uxx.h>
#include "DAP_config.h"
#include "read_uid.h"
#endif

#if defined(BOARD_LPC1549) || defined(BOARD_LPC11U68) || defined(BOARD_LPC4337)
    #define USE_USB_EJECT_INSERT
#endif

// Event flags for main task
// Timers events
#define FLAGS_MAIN_90MS           (1 << 0)
#define FLAGS_MAIN_30MS           (1 << 1)

// Reset events
#define FLAGS_MAIN_RESET          (1 << 2)

// USB Events
#define FLAGS_MAIN_USB_DISCONNECT (1 << 3)

// Other Events
#define FLAGS_MAIN_POWERDOWN      (1 << 4)
#define FLAGS_MAIN_DISABLEDEBUG   (1 << 5)

// Used by msd when flashing a new binary
#define FLAGS_LED_BLINK_30MS      (1 << 6)

// Used by msd to eject/insert the file system
#define FLAGS_MAIN_USB_MEDIA_EJECT   (1<<7)

// Timing constants (in 90mS ticks)
// USB busy time
#define USB_BUSY_TIME           (10)
// Delay before a USB device connect may occur
#define USB_CONNECT_DELAY       (2)
// Delay before target may be taken out of reset or reprogrammed after startup
#define STARTUP_DELAY           (0)
// Decrement to zero
#define DECZERO(x)              (x ? --x : 0)

// LED state
typedef enum {
    LED_OFF,
    LED_FLASH,
    LED_FLASH_PERMANENT
} LED_STATE;

// Reference to our main task
OS_TID main_task_id;
OS_TID serial_task_id;

// USB busy LED state; when TRUE the LED will flash once using 30mS clock tick
static uint8_t dap_led_usb_activity = 0;
static uint8_t cdc_led_usb_activity = 0;
static uint8_t msd_led_usb_activity = 0;

static LED_STATE dap_led_state = LED_FLASH;
static LED_STATE cdc_led_state = LED_FLASH;
static LED_STATE msd_led_state = LED_FLASH;

static uint8_t send_uID = 0;

#ifdef USE_USB_EJECT_INSERT
    typedef enum {
        EJECT_INSERT_INACTIVE,
        EJECT_INSERT_WAIT_TO_EJECT,
        EJECT_INSERT_WAIT_TO_INSERT,
    } EJECT_INSERT_MODE;

    // Delay of ~0.5 second using 90ms ticks
    #define EJECT_INSERT_DELAY_500MS  (5)

    // Variables to handle media eject/insert after a successful drag-n-drop
    extern BOOL USBD_MSC_MediaReadyEx;
    static EJECT_INSERT_MODE EjectInsertMediaMode = EJECT_INSERT_INACTIVE;
    static BOOL EjectInsertMediaCounter = 0;
#endif

// Global state of usb
USB_CONNECT usb_state;

static USB_BUSY usb_busy;
static uint32_t usb_busy_count;

static U64 stk_timer_30_task[TIMER_TASK_30_STACK/8];
static U64 stk_dap_task[DAP_TASK_STACK/8];
static U64 stk_serial_task[SERIAL_TASK_STACK/8];
static U64 stk_main_task[MAIN_TASK_STACK/8];

// Timer task, set flags every 30mS and 90mS
__task void timer_task_30mS(void) {
    uint8_t i = 0;
    os_itv_set(3); // 30mS

    while(1) {
        os_itv_wait();
        os_evt_set(FLAGS_MAIN_30MS, main_task_id);
        if (!(i++ % 3))
            os_evt_set(FLAGS_MAIN_90MS, main_task_id);
    }
}

// Functions called from other tasks to trigger events in the main task
void main_reset_target(uint8_t send_unique_id) {
    if (send_unique_id) {
        send_uID = 1;
    }
    os_evt_set(FLAGS_MAIN_RESET, main_task_id);
    return;
}

// Flash DAP LED using 30mS tick
void main_blink_dap_led(uint8_t permanent) {
    dap_led_usb_activity=1;
    dap_led_state = (permanent) ? LED_FLASH_PERMANENT : LED_FLASH;
    return;
}

// Flash Serial LED using 30mS tick
void main_blink_cdc_led(uint8_t permanent) {
    cdc_led_usb_activity=1;
    cdc_led_state = (permanent) ? LED_FLASH_PERMANENT : LED_FLASH;
    return;
}

// Flash MSD LED using 30mS tick
void main_blink_msd_led(uint8_t permanent) {
    msd_led_usb_activity=1;
    msd_led_state = (permanent) ? LED_FLASH_PERMANENT : LED_FLASH;
    return;
}

// MSC data transfer in progress
void main_usb_busy_event(void) {
    usb_busy_count = USB_BUSY_TIME;
    usb_busy = USB_ACTIVE;
    //os_evt_set(FLAGS_MAIN_USB_BUSY, main_task_id);
    return;
}

// A new binary has been flashed in the target
void main_usb_disconnect_event(void) {
#ifdef USE_USB_EJECT_INSERT
    // Instead of restarting the entire USB stack, just report the
    // media as missing and then as present again. This should force
    // the host to reload the file system.
    os_evt_set(FLAGS_MAIN_USB_MEDIA_EJECT, main_task_id);
#else
    os_evt_set(FLAGS_MAIN_USB_DISCONNECT, main_task_id);
#endif
    return;
}

// Power down the interface
void main_powerdown_event(void) {
    os_evt_set(FLAGS_MAIN_POWERDOWN, main_task_id);
    return;
}

// Disable debug on target
void main_disable_debug_event(void) {
    os_evt_set(FLAGS_MAIN_DISABLEDEBUG, main_task_id);
    return;
}

#define SIZE_DATA (64)
os_mbx_declare(serial_mailbox, 20);

__task void serial_process() {
    uint8_t data[SIZE_DATA];
    int32_t len_data = 0;
    void *msg;

    while (1) {

        // Check our mailbox to see if we need to set anything up with the UART
        // before we do any sending or receiving
        if (os_mbx_wait(&serial_mailbox, &msg, 0) == OS_R_OK)
        {
            switch((SERIAL_MSG)(unsigned)msg)
            {
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
                    {
                        UART_Configuration config;
                        serial_get_configuration(&config);
                        uart_set_configuration(&config);
                    }
                    break;
                default:
                    break;
            }
        }

        len_data = USBD_CDC_ACM_DataFree();
        if (len_data > SIZE_DATA)
            len_data = SIZE_DATA;
        if (len_data)
            len_data = uart_read_data(data, len_data);
        if (len_data) {
            if(USBD_CDC_ACM_DataSend(data , len_data))
                main_blink_cdc_led(0);
        }

        len_data = uart_write_free();
        if (len_data > SIZE_DATA)
            len_data = SIZE_DATA;
        if (len_data)
            len_data = USBD_CDC_ACM_DataRead(data, len_data);
        if (len_data) {
            if (uart_write_data(data, len_data))
                main_blink_cdc_led(0);
        }
    }
}

extern __task void hid_process(void);
__task void main_task(void) {
    // State processing
    uint16_t flags;

    // LED
    uint8_t dap_led_value = 1;
    uint8_t cdc_led_value = 1;
    uint8_t msd_led_value = 1;

    // USB
    uint32_t usb_state_count;

    // thread running after usb connected started
    uint8_t thread_started = 0;

    // button state
    char button_activated;

    // string containing unique ID
    uint8_t * id_str;

    // Initialize our serial mailbox
    os_mbx_init(&serial_mailbox, sizeof(serial_mailbox));

    // Get a reference to this task
    main_task_id = os_tsk_self();

    // leds
    gpio_init();
    // Turn off LED
    gpio_set_dap_led(1);
    gpio_set_cdc_led(1);
    gpio_set_msd_led(1);

#ifdef BOARD_UBLOX_C027
    PORT_SWD_SETUP();
    // wait until reset output to the target is pulled high
    while (!PIN_nRESET_IN()) {
        /* wait doing nothing */
    }
    os_dly_wait(4);
    // if the reset input from button is low then enter isp programming mode
    if (!(LPC_GPIO->B[19/*RESET_PIN*/ + (1/*RESET_PORT*/ << 5)] & 1)) {
        enter_isp();
    }
#endif 

    usbd_init();
    swd_init();

    // Setup reset button
    gpio_enable_button_flag(main_task_id, FLAGS_MAIN_RESET);
    button_activated = 1;

    // USB
    usbd_connect(0);
    usb_busy = USB_IDLE;
    usb_busy_count = 0;
    usb_state = USB_CONNECTING;
    usb_state_count = USB_CONNECT_DELAY;

    // Update HTML version information file
    update_html_file();

    // Start timer tasks
    os_tsk_create_user(timer_task_30mS, TIMER_TASK_30_PRIORITY, (void *)stk_timer_30_task, TIMER_TASK_30_STACK);

#ifndef BOARD_UBLOX_C027
    // Target running
    //target_set_state(RESET_RUN_WITH_DEBUG);
#endif

    // start semihost task
    semihost_init();
    semihost_enable();

    while(1) {
        os_evt_wait_or(   FLAGS_MAIN_RESET              // Put target in reset state
                        | FLAGS_MAIN_90MS               // 90mS tick
                        | FLAGS_MAIN_30MS               // 30mS tick
                        | FLAGS_MAIN_POWERDOWN          // Power down interface
                        | FLAGS_MAIN_DISABLEDEBUG       // Power down interface
#ifdef USE_USB_EJECT_INSERT
                        | FLAGS_MAIN_USB_DISCONNECT     // Disable target debug
                        | FLAGS_MAIN_USB_MEDIA_EJECT,   // Eject file system
#else
                        | FLAGS_MAIN_USB_DISCONNECT,    // Disable target debug
#endif
                        NO_TIMEOUT);

        // Find out what event happened
        flags = os_evt_get();

        if (flags & FLAGS_MAIN_USB_DISCONNECT) {
            usb_busy = USB_IDLE;                         // USB not busy
            usb_state_count = 4;
            usb_state = USB_DISCONNECT_CONNECT;        // disconnect the usb
        }

#ifdef USE_USB_EJECT_INSERT
        if (flags & FLAGS_MAIN_USB_MEDIA_EJECT) {
            EjectInsertMediaMode = EJECT_INSERT_WAIT_TO_EJECT;
            EjectInsertMediaCounter = EJECT_INSERT_DELAY_500MS;
        }
#endif

        if (flags & FLAGS_MAIN_RESET) {
            cdc_led_state = LED_OFF;
            gpio_set_cdc_led(0);
            //usbd_cdc_ser_flush();
            if (send_uID) {
                // set the target in reset to not receive char on the serial port
                target_set_state(RESET_HOLD);

                // send uid
                id_str = get_uid_string();
                USBD_CDC_ACM_DataSend(id_str, strlen((const char *)id_str));
                send_uID = 0;
            }
            // Reset target
            target_set_state(RESET_RUN);
            cdc_led_state = LED_FLASH;
            gpio_set_cdc_led(1);
            button_activated = 0;
        }

        if (flags & FLAGS_MAIN_POWERDOWN) {
            // Stop semihost task
            semihost_disable();

            // Disable debug
            target_set_state(NO_DEBUG);

            // Disconnect USB
            usbd_connect(0);

            // Turn off LED
            gpio_set_dap_led(0);
            gpio_set_cdc_led(0);
            gpio_set_msd_led(0);

            // TODO: put the interface chip in sleep mode
            while (1) {    }
        }

        if (flags & FLAGS_MAIN_DISABLEDEBUG) {
            // Stop semihost task
            semihost_disable();

            // Disable debug
            target_set_state(NO_DEBUG);
        }

        if (flags & FLAGS_MAIN_90MS) {
            if (!button_activated) {
                gpio_enable_button_flag(main_task_id, FLAGS_MAIN_RESET);
                button_activated = 1;
            }

#ifdef USE_USB_EJECT_INSERT
            if (EjectInsertMediaMode == EJECT_INSERT_WAIT_TO_EJECT) {
                if (--EjectInsertMediaCounter == 0) {
                    // Have waited ~0.5 second, time to eject media
                    EjectInsertMediaMode = EJECT_INSERT_WAIT_TO_INSERT;
                    EjectInsertMediaCounter = EJECT_INSERT_DELAY_500MS;
                    USBD_MSC_MediaReady = __FALSE;
                }
            }
            if ((EjectInsertMediaMode == EJECT_INSERT_WAIT_TO_INSERT) && !USBD_MSC_MediaReadyEx) {
                // The host computer have questioned the state and received
                // the message that the media has been removed
                if (--EjectInsertMediaCounter == 0) {
                    // Have waited ~0.5 seconds after ejecting, time to insert media
                    EjectInsertMediaMode = EJECT_INSERT_INACTIVE;
                    USBD_MSC_MediaReady = __TRUE;
                }
            }
#endif

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
                        usbd_connect(0);
                        usb_state = USB_DISCONNECTED;
                    }
                    break;

                case USB_DISCONNECT_CONNECT:
                    // Wait until USB is idle before disconnecting
                    if ((usb_busy == USB_IDLE) && (DECZERO(usb_state_count) == 0)) {
                        usbd_connect(0);
                        usb_state = USB_CONNECTING;
                        // Update HTML file
                        update_html_file();
						// Delay the connecting state before reconnecting to the host - improved usage with VMs
						usb_state_count = 10; //(90ms * 10 = 900ms)
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
                            serial_task_id = os_tsk_create_user(serial_process, SERIAL_TASK_PRIORITY, (void *)stk_serial_task, SERIAL_TASK_STACK);
                            thread_started = 1;
                        }
                        usb_state = USB_CONNECTED;
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
            if (dap_led_usb_activity && ((dap_led_state == LED_FLASH) || (dap_led_state == LED_FLASH_PERMANENT))) {
                // Flash DAP LED ONCE
                if (dap_led_value) {
                    dap_led_value = 0;
                } else {
                    dap_led_value = 1; // Turn on
                    if (dap_led_state == LED_FLASH) {
                        dap_led_usb_activity = 0;
                    }
                }

                // Update hardware
                gpio_set_dap_led(dap_led_value);
            }

            if (msd_led_usb_activity && ((msd_led_state == LED_FLASH) || (msd_led_state == LED_FLASH_PERMANENT))) {
                // Flash MSD LED ONCE
                if (msd_led_value) {
                    msd_led_value = 0;
                } else {
                    msd_led_value = 1; // Turn on
                    if (msd_led_state == LED_FLASH) {
                        msd_led_usb_activity = 0;
                    }
                }

                // Update hardware
                gpio_set_msd_led(msd_led_value);
            }

            if (cdc_led_usb_activity && ((cdc_led_state == LED_FLASH) || (cdc_led_state == LED_FLASH_PERMANENT))) {
                // Flash CDC LED ONCE
                if (cdc_led_value) {
                    cdc_led_value = 0;
                } else {
                    cdc_led_value = 1; // Turn on
                    if (cdc_led_state == LED_FLASH) {
                        cdc_led_usb_activity = 0;
                    }
                }

                // Update hardware
                gpio_set_cdc_led(cdc_led_value);
            }

        }
    }
}

// Main Program
int main (void) {
  /* Allow the board to do some last initialization before the main task is started */
  board_init();

  os_sys_init_user(main_task, MAIN_TASK_PRIORITY, stk_main_task, MAIN_TASK_STACK);
}
