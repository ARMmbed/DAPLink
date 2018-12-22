/**
 * @file    dipdap_cc3220sf.c
 * @brief   board ID for the dipdap cc3220sf board
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2018, ARM Limited, All Rights Reserved
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


extern target_cfg_t target_device;

const uint32_t cookieList[]=
{
    0x5AA5A55A,
    0x000FF800,
    0xEFA3247D
};

// Override the weak validate_bin_nvic function. The weak function expects NVIC at the beginning of the flash.
// On CC3220SF, the beginning of the flash is the cookie list, which allows the boot ROM code to jump into onchip flash directly bypassing external flash.
static uint8_t validate_bin_nvic(const uint8_t *buf)
{
    return (memcmp(buf, cookieList, sizeof(cookieList)) == 0);
}

const board_info_t g_board_info = {
    .infoVersion = 0x0,
    .board_id = "3300",
    .family_id = TI_FAMILY_ID,
    .flags = kEnablePageErase,
    .daplink_url_name =       "MBED    HTM",
    .daplink_drive_name =       "DAPLINK    ",
    .daplink_target_url = "https://mbed.org/device/?code=@U?version=@V?target_id=@T",
    .validate_bin_nvic = validate_bin_nvic,
    .target_cfg = &target_device,
};
