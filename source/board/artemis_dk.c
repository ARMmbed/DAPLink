/**
 * @file    microbit.c
 * @brief   board ID for the SparkFun Artemis Development Kit
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
#include "fsl_device_registers.h"
#include "IO_Config.h"
#include "DAP.h"
#include "target_family.h"
#include "target_board.h"

const char *const board_id_artemis_dk_v10 = "A127";

typedef enum
{
    BOARD_VERSION_v10 = 0,
} artemis_dk_version_t;

static void set_board_id(artemis_dk_version_t board_version)
{
    switch (board_version)
    {
    case BOARD_VERSION_v10:
        target_device.rt_board_id = board_id_artemis_dk_v10;
        break;
    default:
        target_device.rt_board_id = board_id_artemis_dk_v10;
        break;
    }
}

// Called in main_task() to init before USB and files are configured
static void prerun_board_config(void)
{
    // in the future you could auto-detect board version here
    artemis_dk_version_t board_version = (artemis_dk_version_t)BOARD_VERSION_v10;
    set_board_id(board_version);
}

// USB HID override function return 1 if the activity is trivial or response is null
uint8_t usbd_hid_no_activity(uint8_t *buf)
{
    if (buf[0] == ID_DAP_Vendor3 && buf[1] == 0)
        return 1;
    else
        return 0;
}

const board_info_t g_board_info = {
    .info_version = 0x1,
    .family_id = kAmbiq_ama3b1kk_FamilyID,
    .daplink_url_name = "ARTEMIS_HTM",
    .daplink_drive_name = "ARTEMIS    ",
    .daplink_target_url = "https://www.sparkfun.com/artemis",
    .prerun_board_config = prerun_board_config,
    .target_cfg = &target_device,
    .board_vendor = "SparkFun",
    .board_name = "Artemis Development Kit",
};
