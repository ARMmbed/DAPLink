/**
 * @file    ep_kairos.c
 * @brief   board ID for the Embedded Planet Kairos board
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
    .info_version = kBoardInfoVersion,
    .board_id = "2601",
    .family_id = CREATE_FAMILY_ID(kNXP_VendorID, 0), //ID not maching the predefined family ids
    .flags = kEnablePageErase,
    .target_cfg = &target_device,
    .board_vendor = "Embedded Planet",
    .board_name = "Kairos",
};
