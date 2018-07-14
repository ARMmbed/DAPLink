/**
 * @file    stm32f1xx_bl.c
 * @brief   board ID and meta-data for the hardware interface circuit (HIC) based on the ST stmf1xx
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * Copyright (c) 2018, Findlay Feng, All Rights Reserved
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

target_cfg_t target_device = {
    .sector_size    = DAPLINK_SECTOR_SIZE,
    // Assume memory is regions are same size. Flash algo should ignore requests
    //  when variable sized sectors exist
    // .sector_cnt = ((.flash_end - .flash_start) / .sector_size);
    .sector_cnt     = DAPLINK_ROM_IF_SIZE / DAPLINK_SECTOR_SIZE,
    .flash_start    = DAPLINK_ROM_IF_START,
    .flash_end      = DAPLINK_ROM_IF_START + DAPLINK_ROM_IF_SIZE,
    .ram_start      = DAPLINK_RAM_START,
    .ram_end        = DAPLINK_RAM_START + DAPLINK_RAM_SIZE,
    /* .flash_algo not needed for bootloader */
};
