/**
 * @file    main.c
 * @brief   Entry point for interface program logic
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "string.h"
#include "stdio.h"
#include "stdint.h"

#include "RTL.h"
#include "rl_usb.h"
#include "main.h"
#include "board.h"
#include "gpio.h"
#include "uart.h"
#include "tasks.h"
#include "target_reset.h"
#include "swd_host.h"
#include "info.h"
#include "vfs_manager.h"
#include "settings.h"
#include "daplink.h"
#include "util.h"
#include "DAP.h"
#include "bootloader.h"
#include "cortex_m.h"

// Event flags for main task
// Timers events
#define FLAGS_MAIN_90MS         (1 << 0)
#define FLAGS_MAIN_30MS         (1 << 1)
// Reset events
#define FLAGS_MAIN_RESET        (1 << 2)
// Other Events
#define FLAGS_MAIN_POWERDOWN    (1 << 4)
#define FLAGS_MAIN_DISABLEDEBUG (1 << 5)
#define FLAGS_MAIN_PROC_USB     (1 << 9)
// Used by hid when no longer idle
#define FLAGS_MAIN_HID_SEND     (1 << 10)
// Used by cdc when an event occurs
#define FLAGS_MAIN_CDC_EVENT    (1 << 11)
// Used by msd when flashing a new binary
#define FLAGS_LED_BLINK_30MS    (1 << 6)

// Timing constants (in 90mS ticks)
// USB busy time (~3 sec)
#define USB_BUSY_TIME           (33)
// Delay before a USB device connect may occur (~1 sec)
#define USB_CONNECT_DELAY       (11)
// Timeout for USB being configured (~2 sec)
#define USB_CONFIGURE_TIMEOUT   (22)
// Delay before target may be taken out of reset or reprogrammed after startup
#define STARTUP_DELAY           (1)

// Decrement to zero
#define DECZERO(x)              (x ? --x : 0)

// Reference to our main task
OS_TID main_task_id;

// USB busy LED state; when TRUE the LED will flash once using 30mS clock tick
static uint8_t hid_led_usb_activity = 0;
static uint8_t cdc_led_usb_activity = 0;
static uint8_t msc_led_usb_activity = 0;
static main_led_state_t hid_led_state = MAIN_LED_FLASH;
static main_led_state_t cdc_led_state = MAIN_LED_FLASH;
static main_led_state_t msc_led_state = MAIN_LED_FLASH;

// Global state of usb
main_usb_connect_t usb_state;
static bool usb_test_mode = false;

static U64 stk_timer_30_task[TIMER_TASK_30_STACK / sizeof(U64)];
static U64 stk_dap_task[DAP_TASK_STACK / sizeof(U64)];
static U64 stk_main_task[MAIN_TASK_STACK / sizeof(U64)];

// Timer task, set flags every 30mS and 90mS
__task void timer_task_30mS(void)
{
    uint8_t i = 0;
    os_itv_set(3); // 30mS

    while (1) {
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

// Send next hid packet
void main_hid_send_event(void)
{
    os_evt_set(FLAGS_MAIN_HID_SEND, main_task_id);
    return;
}

// Start CDC processing
void main_cdc_send_event(void)
{
    os_evt_set(FLAGS_MAIN_CDC_EVENT, main_task_id);
    return;
}

void main_usb_set_test_mode(bool enabled)
{
    usb_test_mode = enabled;
}

void USBD_SignalHandler()
{
    isr_evt_set(FLAGS_MAIN_PROC_USB, main_task_id);
}

void HardFault_Handler()
{
    util_assert(0);
    SystemReset();

    while (1); // Wait for reset
}

extern __task void hid_process(void);
extern void hid_send_packet(void);
extern void cdc_process_event(void);
__attribute__((weak)) void prerun_board_config(void) {}
__attribute__((weak)) void prerun_target_config(void) {}

__task void main_task(void)
{
    // State processing
    uint16_t flags = 0;
    // LED
    gpio_led_state_t hid_led_value = GPIO_LED_OFF;
    gpio_led_state_t cdc_led_value = GPIO_LED_OFF;
    gpio_led_state_t msc_led_value = GPIO_LED_OFF;
    // USB
    uint32_t usb_state_count = USB_BUSY_TIME;
    uint32_t usb_no_config_count = USB_CONFIGURE_TIMEOUT;
    // thread running after usb connected started
    uint8_t thread_started = 0;
    // button state
    main_reset_state_t main_reset_button_state = MAIN_RESET_RELEASED;
    // Initialize settings - required for asserts to work
    config_init();
    // Update bootloader if it is out of date
    bootloader_check_and_update();
    // Get a reference to this task
    main_task_id = os_tsk_self();
    // leds
    gpio_init();
    // Turn off LED
    gpio_set_hid_led(GPIO_LED_OFF);
    gpio_set_cdc_led(GPIO_LED_OFF);
    gpio_set_msc_led(GPIO_LED_OFF);
    // Initialize the DAP
    DAP_Setup();
    // do some init with the target before USB and files are configured
    prerun_board_config();
    prerun_target_config();
    // Update versions and IDs
    info_init();
    // USB
    usbd_init();
    vfs_mngr_fs_enable(true);
    usbd_connect(0);
    usb_state = USB_CONNECTING;
    usb_state_count = USB_CONNECT_DELAY;
    // Start timer tasks
    os_tsk_create_user(timer_task_30mS, TIMER_TASK_30_PRIORITY, (void *)stk_timer_30_task, TIMER_TASK_30_STACK);

    while (1) {
        os_evt_wait_or(FLAGS_MAIN_RESET             // Put target in reset state
                       | FLAGS_MAIN_90MS            // 90mS tick
                       | FLAGS_MAIN_30MS            // 30mS tick
                       | FLAGS_MAIN_POWERDOWN       // Power down interface
                       | FLAGS_MAIN_DISABLEDEBUG    // Disable target debug
                       | FLAGS_MAIN_PROC_USB        // process usb events
                       | FLAGS_MAIN_HID_SEND        // send hid packet
                       | FLAGS_MAIN_CDC_EVENT       // cdc event
                       , NO_TIMEOUT);
        // Find out what event happened
        flags = os_evt_get();

        if (flags & FLAGS_MAIN_PROC_USB) {
            if (usb_test_mode) {
                // When in USB test mode Insert a delay to
                // simulate worst-case behavior.
                os_dly_wait(1);
            }
            USBD_Handler();
        }

        if (flags & FLAGS_MAIN_RESET) {
            target_set_state(RESET_RUN);
        }

        if (flags & FLAGS_MAIN_POWERDOWN) {
            // Disable debug
            target_set_state(NO_DEBUG);
            // Disable board power before USB is disconnected.
            gpio_set_board_power(false);
            // Disconnect USB
            usbd_connect(0);
            // Turn off LED
            gpio_set_hid_led(GPIO_LED_OFF);
            gpio_set_cdc_led(GPIO_LED_OFF);
            gpio_set_msc_led(GPIO_LED_OFF);

            // TODO: put the interface chip in sleep mode
            while (1);
        }

        if (flags & FLAGS_MAIN_DISABLEDEBUG) {
            // Disable debug
            target_set_state(NO_DEBUG);
        }

        if (flags & FLAGS_MAIN_HID_SEND) {
            hid_send_packet();
        }

        if (flags & FLAGS_MAIN_CDC_EVENT) {
            cdc_process_event();
        }

        if (flags & FLAGS_MAIN_90MS) {
            // Update USB busy status
            vfs_mngr_periodic(90); // FLAGS_MAIN_90MS

            // Update USB connect status
            switch (usb_state) {
                case USB_DISCONNECTING:
                    usb_state = USB_DISCONNECTED;
                    // Disable board power before USB is disconnected.
                    gpio_set_board_power(false);
                    usbd_connect(0);
                    break;

                case USB_CONNECTING:
                    // Wait before connecting
                    if (DECZERO(usb_state_count) == 0) {
                        usbd_connect(1);
                        usb_state = USB_CHECK_CONNECTED;
                        // Reset connect timeout
                        usb_no_config_count = USB_CONFIGURE_TIMEOUT;
                    }

                    break;

                case USB_CHECK_CONNECTED:
                    if (usbd_configured()) {
                        if (!thread_started) {
                            os_tsk_create_user(hid_process, DAP_TASK_PRIORITY, (void *)stk_dap_task, DAP_TASK_STACK);
                            thread_started = 1;
                        }

                        // Let the HIC enable power to the target now that high power has been negotiated.
                        gpio_set_board_power(true);

                        usb_state = USB_CONNECTED;
                    }
                    else if (DECZERO(usb_no_config_count) == 0) {
                        // USB configuration timed out, which most likely indicates that the HIC is
                        // powered by a USB wall wart or similar power source. Go ahead and enable
                        // board power.
                        gpio_set_board_power(true);
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

            // DAP LED
            if (hid_led_usb_activity && ((hid_led_state == MAIN_LED_FLASH) || (hid_led_state == MAIN_LED_FLASH_PERMANENT))) {

                // Toggle LED value
                hid_led_value = GPIO_LED_ON == hid_led_value ? GPIO_LED_OFF : GPIO_LED_ON;

                // If in flash mode stop after one cycle
                if ((GPIO_LED_OFF == hid_led_value) && (MAIN_LED_FLASH == hid_led_state)) {
                    hid_led_usb_activity = 0;
                }

                // Update hardware
                gpio_set_hid_led(hid_led_value);
            }

            // MSD LED
            if (msc_led_usb_activity && ((msc_led_state == MAIN_LED_FLASH) || (msc_led_state == MAIN_LED_FLASH_PERMANENT))) {

                // Toggle LED value
                msc_led_value = GPIO_LED_ON == msc_led_value ? GPIO_LED_OFF : GPIO_LED_ON;

                // If in flash mode stop after one cycle
                if ((GPIO_LED_OFF == msc_led_value) && (MAIN_LED_FLASH == msc_led_state)) {
                    msc_led_usb_activity = 0;
                }

                // Update hardware
                gpio_set_msc_led(msc_led_value);
            }

            // CDC LED
            if (cdc_led_usb_activity && ((cdc_led_state == MAIN_LED_FLASH) || (cdc_led_state == MAIN_LED_FLASH_PERMANENT))) {

                // Toggle LED value
                cdc_led_value = GPIO_LED_ON == cdc_led_value ? GPIO_LED_OFF : GPIO_LED_ON;

                // If in flash mode stop after one cycle
                if ((GPIO_LED_OFF == cdc_led_value) && (MAIN_LED_FLASH == cdc_led_state)) {
                    cdc_led_usb_activity = 0;
                }

                // Update hardware
                gpio_set_cdc_led(cdc_led_value);
            }
        }
    }
}

int main(void)
{
    // Explicitly set the vector table since the bootloader might not set
    // it to what we expect.
#if DAPLINK_ROM_BL_SIZE > 0
    SCB->VTOR = SCB_VTOR_TBLOFF_Msk & DAPLINK_ROM_IF_START;
#endif
    os_sys_init_user(main_task, MAIN_TASK_PRIORITY, stk_main_task, MAIN_TASK_STACK);
}
