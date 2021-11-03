/**
 * @file    nrf52820_bl.c
 * @brief   board ID, meta-data, and additional configuration for micro:bit
 *          nRF52820 HIC 
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2021, ARM Limited, All Rights Reserved
 * Copyright (c) 2021 Micro:bit Educational Foundation
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
#include "daplink_addr.h"
#include "compiler.h"
#include "target_board.h"
#include "target_family.h"
#include "device.h"
#include "gpio.h"

// Warning - changing the interface start will break backwards compatibility
COMPILER_ASSERT(DAPLINK_ROM_IF_START == 0x00008000);
COMPILER_ASSERT(DAPLINK_ROM_IF_SIZE == KB(92));

/**
* List of start and size for each size of flash sector
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address start + size.
*/
static const sector_info_t sectors_info[] = {
    {DAPLINK_ROM_IF_START, 0x1000},
};

// nrf52820 target information
target_cfg_t target_device = {
    .sectors_info               = sectors_info,
    .sector_info_length         = (sizeof(sectors_info))/(sizeof(sector_info_t)),
    .flash_regions[0].start     = DAPLINK_ROM_IF_START,
    .flash_regions[0].end       = DAPLINK_ROM_IF_START + DAPLINK_ROM_IF_SIZE,
    .flash_regions[0].flags     = kRegionIsDefault,
    .ram_regions[0].start       = DAPLINK_RAM_START,
    .ram_regions[0].end         = DAPLINK_RAM_START + DAPLINK_RAM_SIZE,
    /* .flash_algo not needed for bootloader */
};

// bootloader has no family
const target_family_descriptor_t *g_target_family = NULL;

const board_info_t g_board_info = {
    .info_version               = kBoardInfoVersion,
    .board_id                   = "0000",
    .daplink_url_name           = "HELP_FAQHTM",
    .daplink_drive_name         = "MAINTENANCE",
    .daplink_target_url         = "https://microbit.org/device/?id=@B&v=@V&bl=1",
    .target_cfg                 = &target_device,
};

bool reset_button_pressed()
{
    bool btn_pressed = false;
    // Bypass button check if we are waking from System OFF via GPIO (reset button)
    if (NRF_POWER->RESETREAS & POWER_RESETREAS_OFF_Msk) {
        // Do not clear the flag as it might be needed in the DAPlink interface
        btn_pressed = false;
    } else {
        btn_pressed = gpio_get_reset_btn();
    }
    return btn_pressed;
}
