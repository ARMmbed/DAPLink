/**
 * @file    target_reset.c
 * @brief   Target reset for the m2354kjfae
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2021 Nuvoton Technology Corp. All Rights Reserved
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

#include "target_family.h"
#include "target_config.h"
#include "swd_host.h"
#include "flash_blob.c"

#define SCS_DHCSR          0xE000EDF0
#define SCS_DHCSR_S_SDE    0x00100000
#define SCU_SRAMNSSET      0x4002F024

static program_target_t flash;

static uint8_t m2354_target_set_state(target_state_t state)
{
    uint32_t i, val, load_address;

    if (state == RESET_PROGRAM) {
        if (!swd_set_target_state_sw(state)) {
            return 0;
        }

        if (!swd_read_word(SCS_DHCSR, &val)) {
            return 0;
        }

        if (val & SCS_DHCSR_S_SDE) {
            load_address = 0x20000000;
        } else {
            if (!swd_read_word(0x10000000 + SCU_SRAMNSSET, &val)) {
                return 0;
            }

            for (i = 0; i < 15; i++) {
                if (val & (1 << i)) {
                    break;
                }
            }

            load_address = 0x30000000 + (0x4000 * i);
        }

        const program_target_t M2354_flash = M2354_PROGRAM_TARGET(load_address);
        memcpy(&flash, &M2354_flash, sizeof(program_target_t));

        for (i = 0; i < MAX_REGIONS; i++) {
            if (target_device.flash_regions[i].start & 0x10000000) {
                target_device.flash_regions[i].flash_algo = (program_target_t *)&flash;
            }
        }

        return 1;
    }

    return swd_set_target_state_sw(state);
}

const target_family_descriptor_t g_target_family_m2354 = {
    .target_set_state = m2354_target_set_state,
};

const target_family_descriptor_t *g_target_family = &g_target_family_m2354;
