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
#include <rl_usb.h>

#include <stdint.h>

#include "tasks.h"
#include "flash_hal.h"
#include "main.h"
#include "gpio.h"
#include "version.h"
#include "vector_table.h"

// Reference to our main task
OS_TID mainTask, ledTask;

#define INITIAL_SP      (*(uint32_t *)(START_APP_ADDRESS))
#define RESET_HANDLER   (*(uint32_t *)(START_APP_ADDRESS + 4))

#define TRANSFER_FINISHED_SUCCESS       (1 << 0)
#define TRANSFER_FINISHED_FAIL          (1 << 1)

void main_transfer_finished(uint8_t success) {
    if (success) {
        isr_evt_set(TRANSFER_FINISHED_SUCCESS, mainTask);
    } else {
        isr_evt_set(TRANSFER_FINISHED_FAIL, mainTask);
    }
}

__asm void modify_stack_pointer_and_start_app(uint32_t r0_sp, uint32_t r1_pc) {
    MOV SP, R0
    BX R1
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

__task void main_task(void) {
    BOOL led_state = __FALSE;
    uint8_t flags, time_blink_led;
    
	mainTask=os_tsk_self();
	ledTask = os_tsk_create(led_task, LED_TASK_PRIORITY);

    update_html_file();

    usbd_init();
    usbd_connect(__TRUE);

    os_evt_wait_or(TRANSFER_FINISHED_SUCCESS | TRANSFER_FINISHED_FAIL, NO_TIMEOUT);

    os_dly_wait(200);

    usbd_connect(__FALSE);

    // Find out what event happened
    flags = os_evt_get();
	
    time_blink_led = (flags & TRANSFER_FINISHED_SUCCESS) ? 10 : 50;
	// if we blink here, dont do it in a thread
	os_tsk_delete(ledTask);
	
    while(1) {
        gpio_set_msd_led(led_state);
        led_state = !led_state;
        os_dly_wait(time_blink_led);
    }
}

// Main Program
int main (void)
{	
    gpio_init();

    if (!gpio_get_pin_loader_state()) {
        os_sys_init(main_task);
    }

    relocate_vector_table();

    // modify stack pointer and start app
    modify_stack_pointer_and_start_app(INITIAL_SP, RESET_HANDLER);
}
