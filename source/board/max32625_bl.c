/**
 * @file    max32625_bl.c
 * @brief   board ID and meta-data for the hardware interface circuit (HIC) based on MAX32625
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
#include "target_board.h"
#include "target_family.h"

const char *board_id = "0000";

/* ME03 -- MAX32625 512KiB Flash, 160KiB RAM */
target_cfg_t target_device = {
    .sector_size    = 0x2000,
    // Assume memory is regions are same size. Flash algo should ignore requests
    //  when variable sized sectors exist
    // .sector_cnt = ((.flash_end - .flash_start) / .sector_size);
    .sector_cnt     = ((KB(512) - 0x10000) / 0x2000),
    .flash_start    = 0x00000000 + 0x10000,
    .flash_end      = 0x00000000 + KB(512),
    .ram_start      = 0x20000000,
    .ram_end        = 0x20028000
    /* .flash_algo not needed for bootloader */
};

//bootloader has no family
const target_family_descriptor_t *g_target_family = NULL;

const board_info_t g_board_info = {
    .infoVersion = 0x0,
    .board_id = "0000",
    .daplink_url_name =       "HELP_FAQHTM",
    .daplink_drive_name =       "MAINTENANCE",
    .daplink_target_url = "https://mbed.com/daplink",
    .target_cfg = &target_device,
};
