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
 
// common to all bootloader implementations
#include "main.h"
#include "gpio.h"
#include "tasks.h"
#include "mbed_htm.h"
#include "device_cfg.h"

// common to bootloader and CMSIS-DAP
#include "RTL.h"
#include "rl_usb.h"
#include "MK20D5.h"
#include "usbd_user_msc.h"
#include "validate_user_application.h"

// debug macros that use ITM0. Does current implementation not working with RTX or
//  when called from tasks maybe usless. Only good for debugging bootloader and 
//  CMSIS-DAP when compiled for non-bootloader use
#include "retarget.h"

__asm void modify_stack_pointer_and_start_app(uint32_t r0_sp, uint32_t r1_pc)
{
    MOV SP, R0
    BX R1
}

// Event flags for main task
// Timers events
#define FLAGS_MAIN_90MS           (1 << 0)
#define FLAGS_MAIN_30MS           (1 << 1)
// USB Events
#define FLAGS_MAIN_USB_DISCONNECT (1 << 3)
// Used by msd when flashing a new binary
#define FLAGS_LED_BLINK_30MS      (1 << 6)

// Timing constants (in 90mS ticks)
// USB busy time
#define USB_BUSY_TIME           (10)
// Delay before a USB device connect may occur
#define USB_CONNECT_DELAY       (2)
// Delay before target may be taken out of reset or reprogrammed after startup
#define STARTUP_DELAY           (0)
// Decrement to zero
#define DECZERO(x)              (x ? --x : 0)
#define NO_TIMEOUT              (0xffff)

// LED state
typedef enum {
    LED_OFF,
    LED_FLASH,
    LED_FLASH_PERMANENT
} LED_STATE;

// Global state of usb used in 
USB_CONNECT usb_state;

// Reference to our main task
OS_TID main_task_id, msc_task;

static uint8_t msd_led_usb_activity = 0;
static LED_STATE msd_led_state = LED_FLASH;

static USB_BUSY usb_busy;
static uint32_t usb_busy_count;

static U64 stk_timer_task[TIMER_TASK_STACK/8];
static U64 stk_main_task [MAIN_TASK_STACK/8];
static U64 stk_msc_task  [MSC_TASK_STACK/8];

// Timer task, set flags every 30mS and 90mS
__task void timer_task_30mS(void) {
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

// Flash MSD LED using 30mS tick
void main_blink_msd_led(uint8_t permanent) {
    msd_led_usb_activity = 1;
    msd_led_state = (permanent) ? LED_FLASH_PERMANENT : LED_FLASH;
    return;
}

// MSC data transfer in progress
void main_usb_busy_event(void) {
    usb_busy_count = USB_BUSY_TIME;
    usb_busy = USB_ACTIVE;
    return;
}

// A new binary has been flashed in the target
void main_usb_disconnect_event(void) {
    os_evt_set(FLAGS_MAIN_USB_DISCONNECT, main_task_id);
    return;
}

__task void main_task(void) 
{
    // State processing
    uint16_t flags;
    // LED
    uint8_t msd_led_value = 1;
    // USB
    uint32_t usb_state_count;

    // Get a reference to this task
    main_task_id = os_tsk_self();
    
    // Turn on LEDs
    gpio_set_dap_led(1);
    gpio_set_cdc_led(1);
    gpio_set_msd_led(1);

    // USB
    usbd_init();
    usbd_connect(0);
    usb_busy = USB_IDLE;
    usb_busy_count = 0;
    usb_state = USB_CONNECTING;
    usb_state_count = USB_CONNECT_DELAY;

    // Start timer tasks
    os_tsk_create_user(timer_task_30mS, TIMER_TASK_30_PRIORITY, (void *)stk_timer_task, TIMER_TASK_STACK);

    while(1) {
        // need to create a new event for programming failure
        os_evt_wait_or(   FLAGS_MAIN_90MS               // 90mS tick
                        | FLAGS_MAIN_30MS               // 30mS tick
                        | FLAGS_MAIN_USB_DISCONNECT     // Disable target debug
                        , NO_TIMEOUT );

        // Find out what event happened
        flags = os_evt_get();

        // this happens when programming is complete. Need to check if we were successful
        //  or not and alert the user. Currently we reset but a partial or bad application 
        //  flash can leave the device in a state that is undesired
        if (flags & FLAGS_MAIN_USB_DISCONNECT) {
            usb_busy = USB_IDLE;            // USB not busy
            usb_state_count = 10;           //(90ms * 10 = 900ms)
            usb_state = USB_DISCONNECTING;  // disconnect the usb
        }

        if (flags & FLAGS_MAIN_90MS) {
            // Update USB busy status
            switch (usb_busy) {

                case USB_ACTIVE:
                    if (DECZERO(usb_busy_count) == 0) {
                        usb_busy = USB_IDLE;
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
                    if (usb_busy == USB_IDLE && (DECZERO(usb_state_count) == 0)) {
                        usbd_connect(0);
                        os_tsk_delete(msc_task);
                        usb_state = USB_DISCONNECTED;
                    }
                    break;

                case USB_DISCONNECT_CONNECT:
                    // Wait until USB is idle before disconnecting
                    if ((usb_busy == USB_IDLE) && (DECZERO(usb_state_count) == 0)) {
                        usbd_connect(0);
                        usb_state = USB_CONNECTING;
						// Delay the connecting state before reconnecting to the host - improved usage with VMs
						usb_state_count = 10; //(90ms * 10 = 900ms)
                    }
                    break;

                case USB_CONNECTING:
                    // Wait before connecting
                    if (DECZERO(usb_state_count) == 0) {
                        msc_task = os_tsk_create_user(msc_valid_file_timeout_task, MSC_TASK_PRIORITY, stk_msc_task, MSC_TASK_STACK);
                        usbd_connect(1);
                        usb_state = USB_CHECK_CONNECTED;
                    }
                    break;

                case USB_CHECK_CONNECTED:
                    if(usbd_configured()) {
                        usb_state = USB_CONNECTED;
                    }
                    break;

                case USB_DISCONNECTED:
                    NVIC_SystemReset();
                    break;
                
                case USB_CONNECTED:
                default:
                    break;
            }
         }

        // 30mS tick used for flashing LED when USB is busy
        if (flags & FLAGS_MAIN_30MS) {
            if (msd_led_usb_activity && ((msd_led_state == LED_FLASH) || (msd_led_state == LED_FLASH_PERMANENT))) {
                // Flash MSD LED ONCE
                if (msd_led_value) {
                    msd_led_value = 0; // Turn off
                } else {
                    msd_led_value = 1; // Turn on
                    if (msd_led_state == LED_FLASH) {
                        msd_led_usb_activity = 0;
                    }
                }

                // Update hardware
                gpio_set_msd_led(msd_led_value);
            }
        }
    }
}

// Main Program
int main (void)
{
    // leds and button
    gpio_init();
    // check for invalid app image or rst button press
    if (gpio_get_rst_pin_state() && validate_user_application(APP_START_ADR))
    {
        // modify stack pointer and start app
        modify_stack_pointer_and_start_app(INITIAL_SP, RESET_HANDLER);
    }
    // config the usb interface descriptor and web auth token before USB connects
    unique_string_auth_config();
    // either the rst pin was pressed or we have an empty app region
    os_sys_init_user(main_task, MAIN_TASK_PRIORITY, stk_main_task, MAIN_TASK_STACK);
}
