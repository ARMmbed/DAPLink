/**
 * @file    target_reset.c
 * @brief   Target reset for the cc3220sf
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
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
#include "target_family.h"

#define GPRCM_0_APPS_SOFT_RESET_ADDR   0x4402D000
#define GPRCM_0_RESET_MCU_VALUE        0x1
#define GPRCM_0_RESET_MCU_PERIPH_VALUE 0x2

static uint8_t target_set_state(TARGET_RESET_STATE state)
{
    swd_set_soft_reset(VECTRESET);
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

const target_family_descriptor_t g_ti_family = {
    .family_id = kTI_Cc3220sf_FamilyID,
    .target_set_state = target_set_state,
};

