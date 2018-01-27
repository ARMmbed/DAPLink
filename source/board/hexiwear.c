/**
 * @file    hexiwear.c
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
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
#include "swd_host.h"

const char *board_id = "0214";

#define K_SERIES     0
#define KW_SERIES    5

void prerun_board_config(void)
{
    // SIM peripheral   0x40047000
    // address offset   0x    1024
    uint32_t SDID = 0x40048024;
    uint32_t sdid;

    // get a hold of the target
    target_set_state(RESET_PROGRAM);

    swd_read_memory(SDID, (uint8_t *)&sdid, 4);

    if (((sdid >> 20) & 0xF) == KW_SERIES) {
        // Program to the KW40 flash
        extern target_cfg_t target_device_kw40;
        target_device = target_device_kw40;
    }

    // Let the target run
    target_set_state(RESET_RUN);
}
