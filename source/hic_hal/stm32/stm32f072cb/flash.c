/**
 * @file    flash.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2018, ARM Limited, All Rights Reserved
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

#include "FlashPrg.h"
#include "stm32f0xx.h"
#include "util.h"

extern FLASH_ProcessTypeDef pFlash;
extern void FLASH_PageErase(uint32_t PageAddress);

#define __wait_todo() util_assert(0)

static uint32_t FlashInitStatus = 0;

uint32_t Init(uint32_t adr, uint32_t clk, uint32_t fnc)
{
    FlashInitStatus |= 1 << fnc;
    if (FlashInitStatus)
    {
        HAL_FLASH_Unlock();
    }
    return 0;
}

uint32_t UnInit(uint32_t fnc)
{
    FlashInitStatus &= ~(1 << fnc);
    if (!FlashInitStatus)
    {
        HAL_FLASH_Lock();
    }
    return 0;
}

uint32_t BlankCheck(uint32_t adr, uint32_t sz, uint8_t pat)
{
    __wait_todo();
    return 0;
}

uint32_t EraseChip(void)
{
    __wait_todo();
    return 0;
}

uint32_t EraseSector(uint32_t adr)
{
    HAL_StatusTypeDef status = HAL_ERROR;
    util_assert(IS_FLASH_PROGRAM_ADDRESS(adr));
    util_assert(IS_FLASH_NB_PAGES(adr, 1));

    __HAL_LOCK(&pFlash);
#if defined(FLASH_BANK2_END)
    if (adr > FLASH_BANK1_END)
    {
        if (FLASH_WaitForLastOperationBank2(FLASH_TIMEOUT_VALUE) == HAL_OK)
        {
            FLASH_PageErase(adr);
            status = FLASH_WaitForLastOperationBank2(FLASH_TIMEOUT_VALUE);
            CLEAR_BIT(FLASH->CR2, FLASH_CR2_PER);
        }
    }
    else
#endif
    {
        if (FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE) == HAL_OK)
        {
            FLASH_PageErase(adr);
            status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
            CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
        }
    }

    __HAL_UNLOCK(&pFlash);
    return status != HAL_OK;
}

uint32_t ProgramPage(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    HAL_StatusTypeDef status;

    util_assert((adr & 0x3) == 0);
    util_assert((sz & 0x3) == 0);
    util_assert(IS_FLASH_PROGRAM_ADDRESS(adr));
    util_assert(IS_FLASH_PROGRAM_ADDRESS(adr + sz - 1));

    __HAL_LOCK(&pFlash);

#if defined(FLASH_BANK2_END)
    if (adr <= FLASH_BANK1_END)
    {
#endif /* FLASH_BANK2_END */
        status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
#if defined(FLASH_BANK2_END)
    }
    else
    {
        status = FLASH_WaitForLastOperationBank2(FLASH_TIMEOUT_VALUE);
    }
#endif /* FLASH_BANK2_END */

    while (status == HAL_OK && sz > 0)
    {
#if defined(FLASH_BANK2_END)
        if (adr <= FLASH_BANK1_END)
        {
#endif /* FLASH_BANK2_END */
            SET_BIT(FLASH->CR, FLASH_CR_PG);
            *(__IO uint16_t *)adr = *(uint16_t *)buf;
            status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
            CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
#if defined(FLASH_BANK2_END)
        }
        else
        {
            SET_BIT(FLASH->CR2, FLASH_CR2_PG);
            *(__IO uint16_t *)adr = buf;
            status = FLASH_WaitForLastOperationBank2(FLASH_TIMEOUT_VALUE);
            CLEAR_BIT(FLASH->CR2, FLASH_CR2_PG);
        }
#endif /* FLASH_BANK2_END */
        sz -= sizeof(uint16_t);
        adr += sizeof(uint16_t);
        buf = (uint32_t *)(((uint16_t *)buf) + 1);
    }

    __HAL_UNLOCK(&pFlash);

    return sz;
}

uint32_t Verify(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    __wait_todo();
    return 0;
}
