/**
 * @file    mtb_wise1570.c
 * @brief   board ID for the MTB Wise 1570
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

 
#include "flash_manager.h"
#include "swd_host.h"

const char *board_id = "0460";

void prerun_board_config(void)
{
    flash_manager_set_page_erase(true);
    swd_set_reset_connect(CONNECT_UNDER_RESET);
}

#include "target_family.h"
#include "target_board.h"

extern target_cfg_t target_device;

const board_info_t g_board_info = {
    .infoVersion = 0x0,
    .board_id = "0460",
    .family_id = STUB_HW_RESET_FAMILY_ID,
    .daplink_url_name =       "MBED    HTM",
    .daplink_drive_name =       "DAPLINK    ",
    .daplink_target_url = "https://mbed.org/device/?code=@U?version=@V?target_id=@T",
    .target_cfg = &target_device,
};
