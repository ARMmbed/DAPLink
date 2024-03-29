/**
 * @file    target.c
 * @brief   Target information for the lpc4367
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2022, Arm Limited, All Rights Reserved
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
#include "daplink_debug.h"
// The file flash_blob.c must only be included in target.c
#include "flash_blob.c"

// target information
target_cfg_t target_device = {
    .version                        = kTargetConfigVersion,
    .sectors_info                   = sectors_info,
    .sector_info_length             = (sizeof(sectors_info))/(sizeof(sector_info_t)),
    .flash_regions[0].start         = 0x1a000000,
    .flash_regions[0].end           = 0x1a000000 + KB(512),
    .flash_regions[0].flags         = kRegionIsDefault,
    .flash_regions[0].flash_algo    = (program_target_t *) &flash,
    .flash_regions[1].start         = 0x1b000000,
    .flash_regions[1].end           = 0x1b000000 + KB(512),
    .flash_regions[1].flags         = 0,
    .flash_regions[1].flash_algo    = (program_target_t *) &flash,
    .ram_regions[0].start           = 0x10000000,
    .ram_regions[0].end             = 0x10000000 + KB(32),
    .target_vendor                  = "NXP",
    .target_part_number             = "LPC4367JET100",
};
