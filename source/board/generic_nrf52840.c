/**
 * @file    generic_nrf52840.c
 * @brief   board ID not assigned
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

#include "target_board.h"
#include "target_family.h"

extern target_cfg_t target_device_nrf52840;

#if 0
// To put this target_device struct on RAM ,
// target_device_nrf52840 must be declared const so that it gets stored in FLASH region.
target_cfg_t target_device = { // invalid by default
    .version        = kTargetConfigVersion,
    .rt_family_id   = kStub_SWSysReset_FamilyID,
    .rt_board_id    = "0000",
};
#endif

static void nrf_prerun_board_config(void)
{
#if 0
    target_device = target_device_nrf52840;
    target_device.rt_family_id = kNordic_Nrf52_FamilyID;
    target_device.rt_board_id = "0000";
#endif
    // Forcibly overwrite target_device_nrf52840 sturct in place.
    target_device_nrf52840.rt_family_id = kNordic_Nrf52_FamilyID;
    target_device_nrf52840.rt_board_id = "0000";
}

const board_info_t g_board_info = {
    .info_version = kBoardInfoVersion,
    .flags = kEnablePageErase,
    // .prerun_board_config = nrf_prerun_board_config,
    .family_id = kNordic_Nrf52_FamilyID,
    .board_id = "0000",
    .target_cfg = &target_device_nrf52840,
    .board_vendor = "N/A",
    .board_name = "Generic nRF52840",
    .daplink_target_url = "https://github.com/ARMmbed/DAPLink/",
};
