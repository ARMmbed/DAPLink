/**
 * @file    rtl8195am.c
 * @brief   board ID for the Realtek RTL8195AM board
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2014-2017, Realtek Semiconductor Corp., All Rights Reserved
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

#include "string.h"
#include "target_family.h"
#include "target_board.h"

static uint8_t validate_bin_nvic(const uint8_t *buf)
{
    const char header[] = {0x99, 0x99, 0x96, 0x96, 0x3F, 0xCC, 0x66, 0xFC,
                           0xC0, 0x33, 0xCC, 0x03, 0xE5, 0xDC, 0x31, 0x62};

    return !memcmp(header, buf, sizeof(header));
}



extern target_cfg_t target_device;

const board_info_t g_board_info = {
    .infoVersion = 0x0,
    .board_id = "4600",
    .family_id = REALTEK_RTL8195AM_FAMILY_ID,
    .flags = kEnablePageErase,
    .daplink_url_name =       "MBED    HTM",
    .daplink_drive_name =       "DAPLINK    ",
    .daplink_target_url = "https://mbed.org/device/?code=@U?version=@V?target_id=@T",
    //.prerun_board_config = prerun_board_config,
    .validate_bin_nvic = validate_bin_nvic,
    .target_cfg = &target_device,
};

