/**
 * @file    flash_hal_stm32f103xb.c
 * @brief
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

#include "flash_hal.h"        // FlashOS Structures       //TODO - uncomment
#include "target_config.h"    // target_device
#include "gd32f10x.h"

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
__attribute__((section("ram_func")))
uint32_t Init(uint32_t adr, uint32_t clk, uint32_t fnc)
{
    //
    // No special init required
    //
    return (0);
}

__attribute__((section("ram_func")))
uint32_t UnInit(uint32_t fnc)
{
    //
    // No special uninit required
    //
    return (0);
}

__attribute__((section("ram_func")))
uint32_t EraseChip(void)
{
    uint32_t flash_addr;

    flash_addr = target_device.flash_start;
    FMC_Unlock();
    while(flash_addr < target_device.flash_end) {
        FMC_ErasePage(flash_addr);
        flash_addr += 0x400;
    }
    FMC_Lock();

    return (0);  // O.K.
}

__attribute__((section("ram_func")))
uint32_t EraseSector(uint32_t adr)
{
    FMC_Unlock();
    FMC_ErasePage(adr);
    FMC_Lock();

    return (0);  // O.K.
}

__attribute__((section("ram_func")))
uint32_t ProgramPage(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    FMC_State error_code;
    uint32_t  len;
    uint32_t  current_addr;

    current_addr = adr;
    len = sz;

    FMC_Unlock();
    while(len > 0) {
        error_code = FMC_ProgramWord(current_addr, *buf);
        if(error_code != FMC_READY)
            return 1;
        buf++;
        current_addr += 4;
        len -= 4;
    }
    FMC_Lock();

    return (0);                                  // Finished without Errors
}
