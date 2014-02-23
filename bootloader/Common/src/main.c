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
 
// OS and Stacks
#include "RTL.h"
#include "rl_usb.h"
#include "tasks.h"

// app stuff
#include "main.h"
#include "gpio.h"
#include "vector_table.h"

// app stuff common
#include "version.h"
#include "flash.h"
#include "validate_user_application.h"

// libs
#include <stdio.h>

//temp hacking. Shouldnt need this
// Global state of usb
USB_CONNECT usb_state;

// Reference to our main task
OS_TID mainTask, ledTask;

#define INITIAL_SP      (*(uint32_t *)(START_APP_ADDRESS))
#define RESET_HANDLER   (*(uint32_t *)(START_APP_ADDRESS + 4))

#define TRANSFER_FINISHED_SUCCESS       (1 << 0)
#define TRANSFER_FINISHED_FAIL          (1 << 1)

__asm void modify_stack_pointer_and_start_app(uint32_t r0_sp, uint32_t r1_pc)
{
    MOV SP, R0
    BX R1
}

// Flash MSD LED using 30mS tick
void main_blink_msd_led(uint8_t permanent) {
//    msd_led_usb_activity=1;
//    msd_led_state = (permanent) ? LED_FLASH_PERMANENT : LED_FLASH;
//    return;
}

// MSC data transfer in progress
void main_usb_busy_event(void) {
//    usb_busy_count = USB_BUSY_TIME;
//    usb_busy = USB_ACTIVE;
//    //os_evt_set(FLAGS_MAIN_USB_BUSY, main_task_id);
//    return;
}

__task void led_task(void) {
    BOOL led_state = __FALSE;
    os_itv_set(100); // 100mS

    while(1) {
        gpio_set_cdc_led(led_state);
        led_state = !led_state;
        os_itv_wait();
    }
}

// called from init disconnect to end the file update
//void main_transfer_finished(uint8_t success) {
//    if (success) {
//        isr_evt_set(TRANSFER_FINISHED_SUCCESS, mainTask);
//    } else {
//        isr_evt_set(TRANSFER_FINISHED_FAIL, mainTask);
//    }
//}

__task void main_task(void)
{
    BOOL led_state = __FALSE;
    uint8_t flags = 0, i = 0;
    
		// create some tasks
		mainTask = os_tsk_self();
		ledTask = os_tsk_create(led_task, LED_TASK_PRIORITY);

    //update_html_file();	// taken care of by USB in new virt. file system
		// init and connect USB
    usbd_init();
    usbd_connect(__TRUE);
		usb_state = USB_CONNECTED;	// hack to maintain compatibility with CMSIS-DAP for now
	
		// wait for the file transfer to complete (pass or fail)
    os_evt_wait_or(TRANSFER_FINISHED_SUCCESS | TRANSFER_FINISHED_FAIL, NO_TIMEOUT);

    // delay the task
		os_dly_wait(200);

    // disconnect USB
		usbd_connect(__FALSE);

    // Find out what event happened
    flags = os_evt_get();
		
		// if we blink here, dont do it in a thread
		os_tsk_delete(ledTask);
		
		if(flags & TRANSFER_FINISHED_SUCCESS)
		{
				// make sure the reset button isnt pressed. If so, wait...
				while(!gpio_get_pin_loader_state());
				// reboot the MCU and the app should run
				mcu_reboot();
		}
	
		// programming failed so just flash to let the user know.
		//	This should be 2 flashes per second
    while(1) 
		{
				i = (i>10) ? 0 : (++i);
				os_dly_wait(50);
				if(i<5)
				{
						gpio_set_msd_led(led_state);
						led_state = !led_state;
				}
    }
}

int main (void)
{	
		printf("BOOTLOADER\n");
		// Configure the IO for LEDs and RST
		gpio_init();

		// check if the RST button is pressed or that we have an invalid CMSIS-DAP application
		if (!gpio_get_pin_loader_state() || !validate_user_application(START_APP_ADDRESS))
		{
        os_sys_init(main_task);	// start the main task to enumerate USB and wait for a CMSIS-DAP file
    }
		
		// looks like we do this for the CMSIS-DAP app. Seems risky since we are unaware of each other
		relocate_vector_table();

    // modify stack pointer and start app
    modify_stack_pointer_and_start_app(INITIAL_SP, RESET_HANDLER);
}
