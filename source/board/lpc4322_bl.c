/**
 * @file    lpc4322_bl.c
 * @brief   board ID and meta-data for the hardware interface circuit (HIC) based on the NXP LPC4322
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
COMPILER_ASSERT(DAPLINK_ROM_IF_START == 0x1A010000);
COMPILER_ASSERT(DAPLINK_ROM_IF_SIZE == (0x1A030000 - 0x1A010000));

// lpc4322 target information
target_cfg_t target_device = {
    .sector_size    = 65536,
    // Assume memory is regions are same size. Flash algo should ignore requests
    //  when variable sized sectors exist
    // .sector_cnt = ((.flash_end - .flash_start) / .sector_size);
    .sector_cnt     = 2,
    .flash_start    = DAPLINK_ROM_IF_START,
    .flash_end      = DAPLINK_ROM_IF_START + DAPLINK_ROM_IF_SIZE,
    .ram_start      = 0x10000000,
    .ram_end        = 0x10008000,
    /* .flash_algo not needed for bootloader */
};
