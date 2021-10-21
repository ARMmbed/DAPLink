/**
 * @file    target.c
 * @brief   Target information for the m2354kjfae
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

#include "target_config.h"
#include "flash_blob.c"

// target information
target_cfg_t target_device = {
    .version                        = kTargetConfigVersion,
    .sectors_info                   = sectors_info,
    .sector_info_length             = (sizeof(sectors_info))/(sizeof(sector_info_t)),
    .flash_regions[0].start         = 0x00000000,
    .flash_regions[0].end           = 0x00100000,
    .flash_regions[0].flags         = kRegionIsDefault,
    .flash_regions[0].flash_algo    = (program_target_t *)&M2354_flash,
    .flash_regions[1].start         = 0x10000000,
    .flash_regions[1].end           = 0x10000000 + 0x100000,
    .flash_regions[1].flash_algo    = (program_target_t *)&M2354_flash,
    .flash_regions[2].start         = 0x00100000,
    .flash_regions[2].end           = 0x00100000 + 0x4000,
    .flash_regions[2].flash_algo    = (program_target_t *)&M2354_flash,
    .flash_regions[3].start         = 0x00110000,
    .flash_regions[3].end           = 0x00110000 + 0x2000,
    .flash_regions[3].flash_algo    = (program_target_t *)&M2354_flash,
    .ram_regions[0].start           = 0x20000000,
    .ram_regions[0].end             = 0x20000000 + 0x40000,
    .ram_regions[1].start           = 0x30000000,
    .ram_regions[1].end             = 0x30000000 + 0x40000,
    .target_vendor                  = "Nuvoton",
    .target_part_number             = "M2354KJFAE",
};
