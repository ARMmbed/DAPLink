/**
 * @file    tmpm46b_stk.c
 * @brief   board ID for the Embedded Artists TMPM46B Display Module
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
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
#include "target_family.h"
#include "target_board.h"


const board_info_t g_board_info = {
    .infoVersion = 0x0,
    .board_id = "7013",
    .family_id = kToshiba_Tz_FamilyID,
    .daplink_drive_name =       "AdBun-M46B ",
    .daplink_target_url = "https://os.mbed.com/platforms/TMPM46B/",
    .target_cfg = &target_device,
};
