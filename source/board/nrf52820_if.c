/**
 * @file    nrf52820_if.c
 * @brief   board ID for nrf528xx on nrf52820
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2021, Arm Limited, All Rights Reserved
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

const char * const board_id_nrf52    = "FF01";
const char * const board_id_nrf52_64 = "FF02";
const char * const board_id_nrf52840 = "FF03";

extern target_cfg_t target_device_nrf52;
extern target_cfg_t target_device_nrf52_64;
extern target_cfg_t target_device_nrf52840;

static void nrf_prerun_board_config(void) {
#if defined(TARGET_NRF52)
    target_device = target_device_nrf52;
    target_device.rt_board_id = board_id_nrf52;
#elif defined(TARGET_NRF52_64)
    target_device = target_device_nrf52_64;
    target_device.rt_board_id = board_id_nrf52_64;
#elif defined(TARGET_NRF52840)
    target_device = target_device_nrf52840;
    target_device.rt_board_id = board_id_nrf52840;
#else
#error "No target defined"
#endif
}

const board_info_t g_board_info = {
    .info_version = kBoardInfoVersion,
    .family_id = kNordic_Nrf52_FamilyID,
    .info_version = kBoardInfoVersion,
    .flags = kEnablePageErase,
    .prerun_board_config = nrf_prerun_board_config,
    .target_cfg = &target_device,
};
