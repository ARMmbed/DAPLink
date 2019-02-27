/**
 * @file    target.c
 * @brief   Target information for the rapid-iot
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
#include "flash_blob_k64.c"
#include "flash_blob_kw40.c"
#include "flash_blob_kw41.c"

// default target information is k64f
target_cfg_t target_device = {
    .sector_size    = 4096,
#ifdef BOARD_RAPID_IOT
    /* Rapid-IoT boards reserve flash at the beginning for bootloader */
    .sector_cnt         = (0xEC000 / 4096),
    .flash_start        = 0x00014000,
#else
    /* Hexiwear boards do not have a bootloader at the start of flash */
    .sector_cnt         = (MB(1) / 4096),
    .flash_start        = 0x00000000,
#endif
    .flash_end          = MB(1),
    .ram_start          = 0x1fff0000,
    .ram_end            = 0x20030000,
    .flash_algo         = (program_target_t *) &flash_k64,
};

target_cfg_t target_device_kw40 = {
    .sector_size    = 1024,
    .sector_cnt     = (KB(160) / 1024),
    .flash_start    = 0,
    .flash_end      = KB(160),
    .ram_start      = 0x1FFFF000,
    .ram_end        = 0x20004000,
    .flash_algo     = (program_target_t *) &flash_kw40,
};

target_cfg_t target_device_kw41 = {
    .sector_size    = 2048,
    /* Rapid-IoT boards reserve flash at the beginning for bootloader */
    .sector_cnt     = (0x7C000 / 2048),
    .flash_start    = 0x4000,
    .flash_end      = KB(512),
    .ram_start      = 0x1FFF8000,
    .ram_end        = 0x20018000,
    .flash_algo     = (program_target_t *) &flash_kw41,
};
