/**
 * @file    target_reset.c
 * @brief   Target reset for the cc3220sf
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2018, ARM Limited, All Rights Reserved
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

#include "target_reset.h"
#include "swd_host.h"

#define GPRCM_0_APPS_SOFT_RESET_ADDR   0x4402D000
#define GPRCM_0_RESET_MCU_VALUE        0x1
#define GPRCM_0_RESET_MCU_PERIPH_VALUE 0x2

void target_before_init_debug(void)
{
    return;
}

uint8_t target_unlock_sequence(void)
{
    return 1;
}

uint8_t target_set_state(TARGET_RESET_STATE state)
{
    if (state == POST_FLASH_RESET)
    {
        // TI recommends reset via writing to PRCM register instead of AIRCR.
        // Reset apps processor and associated peripheral.
        uint32_t write_value = GPRCM_0_RESET_MCU_PERIPH_VALUE;
        swd_write_memory(GPRCM_0_APPS_SOFT_RESET_ADDR, (uint8_t *)&write_value, sizeof(write_value));
        return 1;
    }
    return swd_set_target_state_sw(state);
}

uint8_t security_bits_set(uint32_t addr, uint8_t *data, uint32_t size)
{
    return 0;
}

