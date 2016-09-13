/**
 * @file    app_config_id_5500.c
 * @brief   Information for board 5500
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

// RZ/A1H target information

target_cfg_t target_device = {
    .board_id   = "5500",
    .secret     = "xxxxxxxx",
    .sector_size    = 4096,
    // Assume memory is regions are same size. Flash algo should ignore requests
    //  when variable sized sectors exist
    // .sector_cnt = ((.flash_end - .flash_start) / .sector_size);
    .sector_cnt     = (MB(8) / 4096),
    .flash_start    = 0,
    .flash_end      = MB(8),
    .ram_start      = 0x10000000,
    .ram_end        = 0x10002000,
    .disc_size      = MB(8)
};

