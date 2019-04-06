/**
 * @file    target.c
 * @brief   Target information for the lpc55S6X
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

// The file flash_blob.c must only be included in target.c
#include "flash_blob.c"

// target information
target_cfg_t target_device = {
    .sector_size          = 32768,
    .sector_cnt           = (KB(608) / 32768),
    .flash_start          = 0,
    .flash_end            = KB(608),
    .ram_start            = 0x20000000,
    .ram_end              = 0x20044000,
    .flash_algo           = (program_target_t *) &flash,
    .extra_flash[0].start = 0x10000000,
    .extra_flash[0].end   = 0x10000000 + KB(608),
    .extra_flash[0].flash_algo = (program_target_t *) &flash, //each extra flash region requires a flash algo
    .extra_ram[0].start   = 0x30000000,
    .extra_ram[0].end     = 0x30000000 + 0x00044000,
};
