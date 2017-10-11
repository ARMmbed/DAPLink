/**
 * @file    k20dx_bl.c
 * @brief   board ID and meta-data for the hardware interface circuit (HIC) based on the NXP K20DX
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

#include "target_config.h"
#include "daplink_addr.h"
#include "compiler.h"

const char *board_id   = "0000";

// Warning - changing the interface start will break backwards compatibility
COMPILER_ASSERT(DAPLINK_ROM_IF_START == KB(32));

// k20dx128 target information
target_cfg_t target_device = {
    .sector_size    = 1024,
    // Assume memory is regions are same size. Flash algo should ignore requests
    //  when variable sized sectors exist
    // .sector_cnt = ((.flash_end - .flash_start) / .sector_size);
    .sector_cnt     = (KB(95) / 1024),
    .flash_start    = DAPLINK_ROM_IF_START,
    .flash_end      = DAPLINK_ROM_IF_START + KB(95),
    .ram_start      = 0x1fffe000,
    .ram_end        = 0x20002000,
    /* .flash_algo not needed for bootloader */
};
