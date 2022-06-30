/**
 * @file    flash_hal_stm32h743ii.c
 * @brief
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

#include "flash_hal.h"        // FlashOS Structures
#include "target_config.h"    // target_device
#include "stm32h7xx.h"
#include "string.h"
#include "target_board.h"
#include "daplink_addr.h"
#include "util.h"

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

uint32_t Init(uint32_t adr, uint32_t clk, uint32_t fnc)
{
    //
    // No special init required
    //
    return (0);
}

uint32_t UnInit(uint32_t fnc)
{
    //
    // No special uninit required
    //
    return (0);
}

uint32_t EraseChip(void)
{
   FLASH_EraseInitTypeDef erase_init;
   uint32_t error;
   uint32_t ret = 0;
   if (g_board_info.target_cfg) {
        // Called from the bootloader. Interface flashing only concerns 1 flash region.
        // The start in the board info excludes the bootloader. The HAL erase takes a
        // sector index, so need to calculate which sector the start address refers to.
        util_assert(((g_board_info.target_cfg->flash_regions[0].end - g_board_info.target_cfg->flash_regions[0].start) % FLASH_SECTOR_SIZE == 0) &&
                    ((g_board_info.target_cfg->flash_regions[0].start - FLASH_BASE) % FLASH_SECTOR_SIZE == 0));

        memset(&erase_init, 0, sizeof(erase_init));

        uint8_t start_sector_index = (g_board_info.target_cfg->flash_regions[0].start - FLASH_BASE) / FLASH_SECTOR_SIZE;
        uint8_t number_of_sectors = (g_board_info.target_cfg->flash_regions[0].end - g_board_info.target_cfg->flash_regions[0].start) / FLASH_SECTOR_SIZE;
        erase_init.TypeErase = FLASH_TYPEERASE_SECTORS;
        erase_init.Sector = start_sector_index;
        erase_init.Banks = FLASH_BANK_1;
        erase_init.NbSectors = number_of_sectors;

        HAL_FLASH_Unlock();
        if (HAL_FLASHEx_Erase(&erase_init, &error) != HAL_OK) {
            ret = 1;
        }
        HAL_FLASH_Lock();

   } else {
       ret = 1;
   }
   return ret;
}

uint32_t EraseSector(uint32_t adr)
{
    util_assert((adr - FLASH_BASE) % FLASH_SECTOR_SIZE == 0);

    FLASH_EraseInitTypeDef erase_init;
    uint32_t error;
    uint32_t ret = 0;

    memset(&erase_init, 0, sizeof(erase_init));

    uint8_t sector_index = (adr - FLASH_BASE) / FLASH_SECTOR_SIZE;
    erase_init.TypeErase = FLASH_TYPEERASE_SECTORS;
    erase_init.Sector = sector_index;
    erase_init.Banks = FLASH_BANK_1;
    erase_init.NbSectors = 1;

    HAL_FLASH_Unlock();
    if (HAL_FLASHEx_Erase(&erase_init, &error) != HAL_OK) {
        ret = 1;
    }
    HAL_FLASH_Lock();

    return ret;
}

uint32_t ProgramPage(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    util_assert(sz % 32 == 0); // this chip does 256bit writes

    uint32_t i;
    uint32_t ret = 0;

    HAL_FLASH_Unlock();
    for (i = 0; i < sz / 32; i++) {
        ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, adr + i * 32, (uint32_t)buf + i * 32);
        if (ret != HAL_OK) {
            ret = 1;
            break;
        }
    }
    HAL_FLASH_Lock();

    return ret;
}
