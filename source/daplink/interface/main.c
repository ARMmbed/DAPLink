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

static U64 stk_main_task[MAIN_TASK_STACK / sizeof(U64)];

// Forward reset from the user pressing the reset button
// Boards which tie the reset pin directly to the target
// should override this function with a stub that does nothing
__attribute__((weak))
void target_forward_reset(bool assert_reset)
{
    // Stubbed out for update only build
}

// Functions called from other tasks to trigger events in the main task
// parameter should be reset type??
void main_reset_target(uint8_t send_unique_id)
{
    // Stubbed out for update only build
}

// Flash HID LED using 30mS tick
void main_blink_hid_led(main_led_state_t permanent)
{
    // Stubbed out for update only build
}

// Flash CDC LED using 30mS tick
void main_blink_cdc_led(main_led_state_t permanent)
{
    // Stubbed out for update only build
}

// Flash MSC LED using 30mS tick
void main_blink_msc_led(main_led_state_t permanent)
{
    // Stubbed out for update only build
}

// Power down the interface
void main_powerdown_event(void)
{
    // Stubbed out for update only build
}

// Disable debug on target
void main_disable_debug_event(void)
{
    // Stubbed out for update only build
}

// Send next hid packet
void main_hid_send_event(void)
{
    // Stubbed out for update only build
}

// Start CDC processing
void main_cdc_send_event(void)
{
    // Stubbed out for update only build
}

void main_usb_set_test_mode(bool enabled)
{
    // Stubbed out for update only build
}

void USBD_SignalHandler()
{
    // Stubbed out for update only build
}

void HardFault_Handler()
{
    util_assert(0);
    SystemReset();

    while (1); // Wait for reset
}

__task void main_task(void)
{
    // Initialize settings - required for asserts to work
    config_init();

    // Update bootloader if it is out of date
    bootloader_check_and_update();

    // Switch into bootloader mode
    config_ram_set_hold_in_bl(true);
    NVIC_SystemReset();

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
