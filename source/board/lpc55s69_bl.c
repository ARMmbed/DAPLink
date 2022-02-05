/**
 * @file    lpc55s69_bl.c
 * @brief   board ID and meta-data for the hardware interface circuit (HIC) based on the NXP LPC55S69
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2019-2020, Arm Limited, All Rights Reserved
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
#include "validation.h"
#include "flash_hal.h"

// Warning - changing the interface start will break backwards compatibility
COMPILER_ASSERT(DAPLINK_ROM_IF_START == (DAPLINK_ROM_START + KB(64)));

static uint8_t lpc55xx_bootloader_validate_nvic(const uint8_t *buf);

/**
* List of start and size for each size of flash sector
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address start + size.
*/
static const sector_info_t sectors_info[] = {
    {DAPLINK_ROM_IF_START, DAPLINK_SECTOR_SIZE},
 };

// lpc55s69 target information
target_cfg_t target_device = {
    .version                    = kTargetConfigVersion,
    .sectors_info               = sectors_info,
    .sector_info_length         = (sizeof(sectors_info))/(sizeof(sector_info_t)),
    .flash_regions[0].start     = DAPLINK_ROM_IF_START,
    .flash_regions[0].end       = DAPLINK_ROM_IF_START + DAPLINK_ROM_IF_SIZE,
    .flash_regions[0].flags     = kRegionIsDefault,
    .ram_regions[0].start       = 0x20000000,
    .ram_regions[0].end         = 0x20040000,
    /* .flash_algo not needed for bootloader */
};

/*!
 * Special target family for the LPC55xx bootloader. It's only purpose is to override the
 * validate_bin_nvic() routine to prevent bus faults from attempting to read erased flash.
 */
static const target_family_descriptor_t g_lpc55xx_bootloader_family = {
    .family_id = 0,
    .validate_bin_nvic = lpc55xx_bootloader_validate_nvic,
};

const target_family_descriptor_t *g_target_family = &g_lpc55xx_bootloader_family;

const board_info_t g_board_info = {
    .info_version = kBoardInfoVersion,
    .board_id = "0000",
    .daplink_url_name =   "HELP_FAQHTM",
    .daplink_drive_name = "BOOTLOADER",
    .daplink_target_url = "https://daplink.io",
    .target_cfg = &target_device,
};

//! @brief Customized NVIC validator.
//!
//! This NVIC validator first checks if the passed-in address points to the internal flash
//! memory. If so, an initial check is made to see if the flash is erased. If it is, then we
//! can't read from it or the flash controller will generate a bus fault. If the address is
//! either outside of flash, such as testing a new interface image in RAM, or the flash
//! sector is programmed, then the standard NVIC validator is called.
uint8_t lpc55xx_bootloader_validate_nvic(const uint8_t *buf)
{
    uint32_t addr = (uint32_t)buf;

    // If the address within internal flash?
    if (addr >= DAPLINK_ROM_START && addr < (DAPLINK_ROM_START + DAPLINK_ROM_SIZE)) {
        // If the flash sector is erased, then report that the NVIC is invalid. Otherwise
        // continue below and perform the usual NVIC validation test.
        if (!flash_is_readable(addr, 32)) {
            return 0;
        }
    }

    // Call original implementation.
    return validate_bin_nvic_base(buf);
}
