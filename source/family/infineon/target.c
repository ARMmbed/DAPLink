/*******************************************************************************
* @file    target.c
* @brief   Target information for the target MCU
*
********************************************************************************
* Copyright (2019-2021) Cypress Semiconductor Corporation (an Infineon company)
* or an affiliate of Cypress Semiconductor Corporation.
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
*****************************************************************************/

#include <stdbool.h>

#include "target_config.h"
#include "error.h"
#include "PSOC6xxx.h"
#include "psoc5lp.h"
#include "kit_defs.h"

// The files xxx_flash_blob.c must only be included in target.c
#include "flash_algo_CY8C6xx7_DAPLink_c_blob.c"
#include "flash_algo_CY8C6xx5_DAPLink_c_blob.c"
#include "flash_algo_CY8C6xxA_DAPLink_c_blob.c"
#include "flash_algo_CYxx64xx_DAPLink_c_blob.c"
#include "flash_algo_CYxx64xA_DAPLink_c_blob.c"
#include "flash_algo_CYxx64x5_DAPLink_c_blob.c"
#include "flash_algo_CY8C6xxx_WFLASH_DAPLink_c_blob.c"
#include "flash_algo_CY8C6xxA_WFLASH_DAPLink_c_blob.c"
#include "flash_algo_CY8C6xxx_SFLASH_DAPLink_c_blob.c"
#include "flash_algo_CY8C6xxA_SFLASH_DAPLink_c_blob.c"
#include "flash_algo_CY8C6xxx_SMIF_DAPLink_c_blob.c"
#include "flash_algo_CY8C6xxA_SMIF_DAPLink_c_blob.c"
#include "flash_algo_CY8C6xxx_SMIF_S25FS512S_DAPLink_c_blob.c"
#include "flash_algo_CY8C6xxx_SMIF_S25Hx512T_DAPLink_c_blob.c"
#include "flash_algo_CY8C6xxA_SMIF_S25Hx512T_DAPLink_c_blob.c"

#define IMAGE_MAGIC                 (0x96f3b83du)
#define VECTORS_TO_CHECK_CNT        (4u)

#define U_MB(size)                        ((size) * 1024u * 1024u)
#define U_KB(size)                        ((size) * 1024u)

static const sector_info_t p6_sectors_info[] =
{
    { PSOC6_MAIN_FLASH_START_ADDR, 512u },    // Internal flash
    { PSOC6_XIP_START_ADDR, U_KB(256u) },     // SMIF
};

// default target information
target_cfg_t target_device = {
    .sectors_info   = p6_sectors_info,
    .sector_info_length = (sizeof(p6_sectors_info))/(sizeof(sector_info_t)),
    .flash_regions    = {
        [0u] = { .start = PSOC6_MAIN_FLASH_START_ADDR, .end = PSOC6_MAIN_FLASH_START_ADDR + U_MB(2u),
            .flash_algo = (program_target_t *) &CY8C6xxA_flash_prog, .flags = (uint32_t)kRegionIsDefault },    // Main Flash
        [1u] = { .start = PSOC6_WFLASH_START_ADDR,     .end = PSOC6_WFLASH_START_ADDR + U_KB(32u),
            .flash_algo = (program_target_t *) &CY8C6xxA_WFLASH_flash_prog },                        // WFLASH
        [2u] = { .start = PSOC6_SFLASH_START_ADDR,     .end = PSOC6_SFLASH_START_ADDR + U_KB(32u),
            .flash_algo = (program_target_t *) &CY8C6xxA_SFLASH_flash_prog },                        // SFLASH
        [3u] = { .start = PSOC6_XIP_START_ADDR,        .end = PSOC6_XIP_START_ADDR + U_MB(64u),
            .flash_algo = (program_target_t *) &CY8C6xxA_SMIF_flash_prog },                // SMIF
        [4u] = {},
        [5u] = {},
        [6u] = {},
        [7u] = {},
        [8u] = {},
        [9u] = {}
    },
    .ram_regions      = {
        [0u] = { .start = PSOC6_SRAM_START_ADDR, .end = PSOC6_SRAM_START_ADDR + U_KB(288u) },
        [1u] = {},
        [2u] = {},
        [3u] = {},
        [4u] = {},
        [5u] = {},
        [6u] = {},
        [7u] = {},
        [8u] = {},
        [9u] = {}
    }
};

static uint8_t validate_psoc6_secured_bin_nvic(const uint8_t *buf)
{
    uint32_t nvic_val = 0u;
    // coverity[misra_c_2012_rule_21_15_violation]
    (void)memcpy((uint8_t *)&nvic_val, buf, sizeof(nvic_val));
    return ((nvic_val == IMAGE_MAGIC) ? 1u : 0u);
}

static uint8_t validate_psoc6_bin_nvic(const uint8_t *buf)
{
    uint32_t nvic_val;
    bool in_range = false;
    // test the initial SP value
    // coverity[misra_c_2012_rule_21_15_violation]
    (void)memcpy((uint8_t *)&nvic_val, buf, sizeof(nvic_val));

    const region_info_t *region = target_device.ram_regions;
    for (; ((region->start != 0u) || (region->end != 0u)); ++region)
    {
        if ((nvic_val >= region->start) && (nvic_val < region->end))
        {
            in_range = true;
            break;
        }
    }

    if (in_range)
    {
        // Reset_Handler
        // NMI_Handler
        // HardFault_Handler
        for (uint32_t i = sizeof(nvic_val); i < (sizeof(nvic_val)*VECTORS_TO_CHECK_CNT); i += sizeof(nvic_val))
        {
            // coverity[misra_c_2012_rule_21_15_violation]
            (void)memcpy((uint8_t *)&nvic_val, &buf[i], sizeof(nvic_val));
            if (nvic_val >= (PSOC6_SROM_START_ADDR + U_KB(128u)))
            {
                // nvic_val not points to SROM
                region = target_device.flash_regions;
                bool range_match = false;
                for (; (region->start != 0u) || (region->end != 0u); ++region)
                {
                    if ((nvic_val >= region->start) && (nvic_val < region->end))
                    {
                        range_match = true;
                        break;
                    }
                }
                if (!range_match)
                {
                    in_range = false;
                    break;
                }
            }
        }
        // All checks passed
    }
    return (in_range ? 1u : 0u);
}


void init_flash_algo(uint32_t si_id)
{
    // Initialize main flash,WFlash, SFlash and SMIF programming algorithms
    // and set proper flsash geometry based on family type/silicon ID

    if (kit_has_sflash_restriction())
    {
        static const sector_info_t AUGUST_CYW43012_sectors_info[] =
        {
            { PSOC6_MAIN_FLASH_START_ADDR, 512u },    // Internal flash
            { PSOC6_XIP_START_ADDR, U_KB(4u) },       // SMIF
        };
        // PSoC64 & PSoC64 2M based kits
        // set secure binary file validation procedure
        g_target_family_psoc6.validate_bin_nvic = validate_psoc6_secured_bin_nvic;
        switch (get_kit_uid())
        {
        case CY8CPROTO_064_SB:
            target_device.flash_regions[0u].end = PSOC6_MAIN_FLASH_START_ADDR + U_MB(1u);
            target_device.flash_regions[0u].flash_algo = (program_target_t *) &CYxx64xx_flash_prog;
            target_device.flash_regions[1u].flash_algo = (program_target_t *) &CY8C6xxx_WFLASH_flash_prog;
            target_device.flash_regions[2u].start = PSOC6_XIP_START_ADDR;
            target_device.flash_regions[2u].end = PSOC6_XIP_START_ADDR + U_MB(64u);
            target_device.flash_regions[2u].flash_algo = (program_target_t *) &CY8C6xxx_SMIF_S25Hx512T_flash_prog;
            target_device.flash_regions[3u].start = 0u;
            target_device.flash_regions[3u].end = 0u;
            target_device.flash_regions[3u].flash_algo = NULL;
            break;

        case AUGUST_CYW43012:
            target_device.flash_regions[0u].end = PSOC6_MAIN_FLASH_START_ADDR + U_MB(1u);
            target_device.flash_regions[0u].flash_algo = (program_target_t *) &CYxx64xx_flash_prog;
            target_device.flash_regions[1u].flash_algo = (program_target_t *) &CY8C6xxx_WFLASH_flash_prog;
            target_device.flash_regions[2u].start = PSOC6_XIP_START_ADDR;
            target_device.flash_regions[2u].end = PSOC6_XIP_START_ADDR + U_MB(8u);
            target_device.flash_regions[2u].flash_algo = (program_target_t *) &CY8C6xxx_SMIF_flash_prog;
            target_device.flash_regions[3u].start = 0u;
            target_device.flash_regions[3u].end = 0u;
            target_device.flash_regions[3u].flash_algo = NULL;
            target_device.sectors_info = AUGUST_CYW43012_sectors_info;      /* sector size 4K for SMIF */
            break;

        case CY8CKIT_064S2_4343W:
        case CY8CKIT_064B0S2_4343W:
        case CY8CKIT_064S0S2_4343W:
            target_device.flash_regions[0u].end = PSOC6_MAIN_FLASH_START_ADDR + U_MB(2u);
            target_device.flash_regions[0u].flash_algo = (program_target_t *) &CYxx64xA_flash_prog;
            target_device.flash_regions[1u].flash_algo = (program_target_t *) &CY8C6xxA_WFLASH_flash_prog;
            target_device.flash_regions[2u].start = PSOC6_XIP_START_ADDR;
            target_device.flash_regions[2u].end = PSOC6_XIP_START_ADDR + U_MB(64u);
            target_device.flash_regions[2u].flash_algo = (program_target_t *) &CY8C6xxA_SMIF_flash_prog;
            target_device.flash_regions[3u].start = 0u;
            target_device.flash_regions[3u].end = 0u;
            target_device.flash_regions[3u].flash_algo = NULL;
            break;

        case CY8CPROTO_064B0S3:
            target_device.flash_regions[0u].end = PSOC6_MAIN_FLASH_START_ADDR + U_KB(512u);
            target_device.flash_regions[0u].flash_algo = (program_target_t *) &CYxx64x5_flash_prog;
            target_device.flash_regions[1u].flash_algo = (program_target_t *) &CY8C6xxA_WFLASH_flash_prog;
            target_device.flash_regions[2u].start = PSOC6_XIP_START_ADDR;
            target_device.flash_regions[2u].end = PSOC6_XIP_START_ADDR + U_MB(64u);
            target_device.flash_regions[2u].flash_algo = (program_target_t *) &CY8C6xxA_SMIF_S25Hx512T_flash_prog;
            target_device.flash_regions[3u].start = 0u;
            target_device.flash_regions[3u].end = 0u;
            target_device.flash_regions[3u].flash_algo = NULL;
            break;

        case CY8CPROTO_064B0S1_BLE:
            target_device.flash_regions[0u].end = PSOC6_MAIN_FLASH_START_ADDR + U_MB(1u);
            target_device.flash_regions[0u].flash_algo = (program_target_t *) &CYxx64xx_flash_prog;
            target_device.flash_regions[1u].flash_algo = (program_target_t *) &CY8C6xxx_WFLASH_flash_prog;
            target_device.flash_regions[2u].start = 0u;
            target_device.flash_regions[2u].end = 0u;
            target_device.flash_regions[2u].flash_algo = NULL;
            target_device.flash_regions[3u].start = 0u;
            target_device.flash_regions[3u].end = 0u;
            target_device.flash_regions[3u].flash_algo = NULL;
            break;

        default:
            // unknown board
            target_device.flash_regions[0u].start = 0u;
            target_device.flash_regions[0u].end = 0u;
            target_device.flash_regions[0u].flash_algo = NULL;
            target_device.flash_regions[1u].start = 0u;
            target_device.flash_regions[1u].end = 0u;
            target_device.flash_regions[1u].flash_algo = NULL;
            target_device.flash_regions[2u].start = 0u;
            target_device.flash_regions[2u].end = 0u;
            target_device.flash_regions[2u].flash_algo = NULL;
            target_device.flash_regions[3u].start = 0u;
            target_device.flash_regions[3u].end = 0u;
            target_device.flash_regions[3u].flash_algo = NULL;

            break;
        }
    }
    else
    {
        // set generic binary file validation procedure
        g_target_family_psoc6.validate_bin_nvic = validate_psoc6_bin_nvic;
        switch (si_id & PSOC6_FAMILY_ID_LO_MSK)
        {
            // PSoC6-BLE family
        case PSOC6A_BLE2_FAMILY_ID_LO:
            target_device.flash_regions[0u].end = PSOC6_MAIN_FLASH_START_ADDR + U_MB(1u);
            target_device.flash_regions[0u].flash_algo = (program_target_t *) &CY8C6xx7_flash_prog;
            target_device.flash_regions[1u].flash_algo = (program_target_t *) &CY8C6xxx_WFLASH_flash_prog;
            target_device.flash_regions[2u].flash_algo = (program_target_t *) &CY8C6xxx_SFLASH_flash_prog;
            if (get_kit_uid() == (uint16_t)CYW9P62S1_43012EVB_01)
            {
                // specific S25FS512S smif flashalgo used for CYW9P62S1_43012EVB_01 
                target_device.flash_regions[3u].flash_algo = (program_target_t *) &CY8C6xxx_SMIF_S25FS512S_flash_prog;
            }
            else
            {
                // default SFDP smif flashalgo used 
                target_device.flash_regions[3u].flash_algo = (program_target_t *) &CY8C6xxx_SMIF_flash_prog;
            }
            break;

            // PSoC6A-2M family
        case PSOC6A_2M_FAMILY_ID_LO:
            target_device.flash_regions[0u].end = PSOC6_MAIN_FLASH_START_ADDR + U_MB(2u);
            target_device.flash_regions[0u].flash_algo = (program_target_t *) &CY8C6xxA_flash_prog;
            target_device.flash_regions[1u].flash_algo = (program_target_t *) &CY8C6xxA_WFLASH_flash_prog;
            target_device.flash_regions[2u].flash_algo = (program_target_t *) &CY8C6xxA_SFLASH_flash_prog;
            target_device.flash_regions[3u].flash_algo = (program_target_t *) &CY8C6xxA_SMIF_flash_prog;
            break;

            // PSoC6A-512K family
        case PSOC6A_512K_FAMILY_ID_LO:
            target_device.flash_regions[0u].end = PSOC6_MAIN_FLASH_START_ADDR + U_KB(512u);
            target_device.flash_regions[0u].flash_algo = (program_target_t *) &CY8C6xx5_flash_prog;
            target_device.flash_regions[1u].flash_algo = (program_target_t *) &CY8C6xxA_WFLASH_flash_prog;
            target_device.flash_regions[2u].flash_algo = (program_target_t *) &CY8C6xxA_SFLASH_flash_prog;
            target_device.flash_regions[3u].flash_algo = (program_target_t *) &CY8C6xxA_SMIF_flash_prog;
            break;

            // unknown family or in case of MiniProg
        default:
            target_device.flash_regions[0u].start = 0u;
            target_device.flash_regions[0u].end = 0u;
            target_device.flash_regions[0u].flash_algo = NULL;
            target_device.flash_regions[1u].start = 0u;
            target_device.flash_regions[1u].end = 0u;
            target_device.flash_regions[1u].flash_algo = NULL;
            target_device.flash_regions[2u].start = 0u;
            target_device.flash_regions[2u].end = 0u;
            target_device.flash_regions[2u].flash_algo = NULL;
            target_device.flash_regions[3u].start = 0u;
            target_device.flash_regions[3u].end = 0u;
            target_device.flash_regions[3u].flash_algo = NULL;
            break;
        }
    }
}
