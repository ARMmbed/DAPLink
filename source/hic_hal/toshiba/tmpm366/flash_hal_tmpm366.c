/**
 * @file    flash_hal_tmpm366.c
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
#include "target_config.h"    // target_device
#include "cortex_m.h"
#include "tmpm366_fc.h"

#define PAGE_SIZE               (512U)

__attribute__((section("ram_func")))
static int flash_write(uint32_t addr_dest, uint32_t len, uint32_t *addr_source)
{
    uint32_t size;
    uint32_t *source;
    uint32_t *dest;

    dest   = (uint32_t *)addr_dest;
    source = addr_source;
    size   = len;

    while (size > PAGE_SIZE) {
        // write one page every time
        if (FC_SUCCESS == FC_WritePage((uint32_t)dest, source)) {
            // Do not thing
        } else {
            return 1;
        }

        size = size - PAGE_SIZE;
        dest = dest + PAGE_SIZE / 4U;
        source = source + PAGE_SIZE / 4U;
    }

    // write the last data, no more than one page
    if (FC_SUCCESS == FC_WritePage((uint32_t)dest, source)) {
        // Do not thing
    } else {
        return 1;
    }

    return 0;
}

__attribute__((section("ram_func")))
uint32_t Init(uint32_t adr, uint32_t clk, uint32_t fnc)
{
    // Select flash core clock source
    TSB_CG_PLLSEL_PLLSEL = 1U;
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
    int retval = 1;
    // Return value 0 == O.K.
    // Return value 1 == Error
    cortex_int_state_t state = cortex_int_get_and_disable();
    // Erase the contents of the entire chip
    if (FC_SUCCESS == FC_EraseChip()) {
        retval = 0;
    }
    cortex_int_restore(state);
    return retval;
}

__attribute__((section("ram_func")))
uint32_t EraseSector(uint32_t adr)
{
    int retval = 1;
    cortex_int_state_t state = cortex_int_get_and_disable();
    // Erase the contents of the specified block
    if (FC_SUCCESS == FC_EraseBlock(adr)) {
        retval = 0;
    }
    cortex_int_restore(state);
    return retval;
}

__attribute__((section("ram_func")))
uint32_t ProgramPage(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    int retval = 1;
    cortex_int_state_t state = cortex_int_get_and_disable();
    retval = flash_write(adr, sz, buf);
    cortex_int_restore(state);
    return retval;
}
