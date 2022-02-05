/**
 * @file    kl27z_microbit_bl.c
 * @brief   board ID and meta-data for the hardware interface circuit (HIC) based on the NXP KL27Z
 *
 * DAPLink Interface Firmware
 * Copyright 2020 NXP
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
#include "fsl_device_registers.h"
#include "gpio.h"
#include "board_id.h"

// Warning - changing the interface start will break backwards compatibility
COMPILER_ASSERT(DAPLINK_ROM_IF_START == KB(32));
COMPILER_ASSERT(DAPLINK_ROM_IF_SIZE == (KB(128) - KB(32) - KB(1)));

/**
* List of start and size for each size of flash sector
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address start + size.
*/
static const sector_info_t sectors_info[] = {
    {DAPLINK_ROM_IF_START, 1024},
 };

// kl26z128 target information
target_cfg_t target_device = {
    .version                    = kTargetConfigVersion,
    .sectors_info               = sectors_info,
    .sector_info_length         = (sizeof(sectors_info))/(sizeof(sector_info_t)),
    .flash_regions[0].start     = DAPLINK_ROM_IF_START,
    .flash_regions[0].end       = DAPLINK_ROM_IF_START + DAPLINK_ROM_IF_SIZE,
    .flash_regions[0].flags     = kRegionIsDefault,
    .ram_regions[0].start       = 0x1FFFE000,
    .ram_regions[0].end         = 0x20006000,
};

//bootloader has no family
const target_family_descriptor_t *g_target_family = NULL;

const board_info_t g_board_info = {
    .info_version = kBoardInfoVersion,
    .board_id = BOARD_ID_MB_2_DEFAULT,
    .daplink_url_name =       "HELP_FAQHTM",
    .daplink_drive_name =       "MAINTENANCE",
    .daplink_target_url = "https://microbit.org/device/?id=@B&v=@V&bl=1",
    .target_cfg = &target_device,
};

bool reset_button_pressed()
{
    bool btn_pressed = false;
    // Bypass button check if we are waking from Low Leakage Wakeup Reset
    if (RCM->SRS0 & RCM_SRS0_WAKEUP_MASK) {
        btn_pressed = false;
    }
    else {
        btn_pressed = gpio_get_reset_btn();
    }

    return btn_pressed;
}
