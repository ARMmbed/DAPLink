/**
 * @file    target.c
 * @brief   Target information for the kl05z
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
    .sector_size    = 1024,
    .sector_cnt     = (KB(32) / 1024),
    .flash_start    = 0,
    .flash_end      = KB(32),
    .ram_start      = 0x1FFFFC00,
    .ram_end        = 0x20000C00,
    .flash_algo     = (program_target_t *) &flash,
};

// From table 21 of the Kinetis KL05 datasheet Rev 4
// t_pgm4 = 145us per word, worst case
// t_ersall = 500ms total, worst case

uint32_t target_chip_erase_time(uint32_t chipSize) {
    // Allow for an additional 10ms of overhead
    return 10 + 500;
}

uint32_t target_chip_program_time(uint16_t blockSize) {
    // 145us per word is approximately ~38ms/1KiB sector
    return (blockSize + 1023) / 1024 * 38;
}
