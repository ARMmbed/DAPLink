/**
 * @file    target_flash.c
 * @brief   Implementation of target_flash.h
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

#include "target_config.h"
#include "target_reset.h"
#include "gpio.h"
#include "validation.h"
#include "target_config.h"
#include "intelhex.h"
#include "swd_host.h"
#include "flash_intf.h"
#include "util.h"
#include "settings.h"

extern const flash_intf_t *const flash_intf_target;

error_t board_target_flash_erase_chip(void)
{
    #define PERSIST_A 0x0003BC00
    #define PERSIST_B 0x0003B800
  
    static uint8_t bufferA[1024];
    static uint8_t bufferB[1024];
  
    swd_read_memory((uint32_t)PERSIST_A, bufferA, 1024);
    swd_read_memory((uint32_t)PERSIST_B, bufferB, 1024);
	
    error_t status = ERROR_SUCCESS;
    const program_target_t * const flash = target_device.flash_algo;
    if (0 == swd_flash_syscall_exec(&flash->sys_call_s, flash->erase_chip, 0, 0, 0, 0)) {
        return ERROR_ERASE_ALL;
    }
		
    flash_intf_target->program_page((uint32_t)PERSIST_A, bufferA, 1024);
    flash_intf_target->program_page((uint32_t)PERSIST_B, bufferB, 1024);

    // Reset and re-initialize the target after the erase if required
    if (target_device.erase_reset) {
        status = flash_intf_target->init();
    }

    return status;
}
