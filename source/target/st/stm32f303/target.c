/**
 * @file    target.c
 * @brief   Target information for stm32f303k8 chips
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
    .sector_size    = KB(2),
    .sector_cnt     = 32,
    .flash_start    = 0x08000000,
    .flash_end      = 0x08000000 + KB(64),
    .ram_start      = 0x20000000,
    .ram_end        = 0x20000000 + KB(12),
    .flash_algo     = (program_target_t *) &flash,
};

// From table 45 of the STM32F303x6/STM32F303x8 datasheet Rev 5
// t_prog = 60us per half-word, worst case
// t_ME = 40ms total, worst case

uint32_t target_chip_erase_time(uint32_t chipSize) {
    // Allow for an additional 10ms of overhead
    return 10 + 40;
}

uint32_t target_chip_program_time(uint16_t blockSize) {
    // 60us per half-word is approximately ~62ms/2KiB sector
    return (blockSize + 2047) / 2048 * 62;
}
