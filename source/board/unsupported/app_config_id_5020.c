/**
 * @file    app_config_id_5020.c
 * @brief   Information for board 5020
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

// frdm-k64f target information
target_cfg_t target_device = {
    .board_id   = "5020",
    .secret     = "xxxxxxxx",
    .sector_size    = 8192,
    // Assume memory is regions are same size. Flash algo should ignore requests
    //  when variable sized sectors exist
    // .sector_cnt = ((.flash_end - .flash_start) / .sector_size);
    .sector_cnt     = (MB(1) / 8192),
    .flash_start    = 0x00400000,
    .flash_end      = 0x00500000,
    .ram_start      = 0x20000000,
    .ram_end        = 0x20020000,
    .disc_size      = MB(1)
};
