/**
 * @file    nrf51822/target_nrf51.c
 * @brief   Target information for the nRF51822
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2021, Arm Limited, All Rights Reserved
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

// The file flash_blob.c must only be included in target.c
#include "flash_blob.c"

// target information for model with 16 KB RAM
target_cfg_t target_device_nrf51822_16 = {
    .version                        = kTargetConfigVersion,
    .sectors_info                   = sectors_info_nrf51,
    .sector_info_length             = (sizeof(sectors_info_nrf51))/(sizeof(sector_info_t)),
    .flash_regions[0].start         = 0,
    .flash_regions[0].end           = KB(256),
    .flash_regions[0].flags         = kRegionIsDefault,
    .flash_regions[0].flash_algo    = (program_target_t *) &flash_nrf51,
    .ram_regions[0].start           = 0x20000000,
    .ram_regions[0].end             = 0x20004000,
    .erase_reset                    = 1,
    .target_vendor                  = "NordicSemiconductor",
    .target_part_number             = "nRF51822_xxAA",
};

// target information for model with 32 KB RAM
target_cfg_t target_device_nrf51822_32 = {
    .version                        = kTargetConfigVersion,
    .sectors_info                   = sectors_info_nrf51,
    .sector_info_length             = (sizeof(sectors_info_nrf51))/(sizeof(sector_info_t)),
    .flash_regions[0].start         = 0,
    .flash_regions[0].end           = KB(256),
    .flash_regions[0].flags         = kRegionIsDefault,
    .flash_regions[0].flash_algo    = (program_target_t *) &flash_nrf51,
    .ram_regions[0].start           = 0x20000000,
    .ram_regions[0].end             = 0x20008000,
    .erase_reset                    = 1,
    .target_vendor                  = "NordicSemiconductor",
    .target_part_number             = "nRF51822_xxAA",
};
