/**
 *******************************************************************************
 * @file    tmpm366_fc.c
 * @brief   This file provides API functions for FC driver.
 * @version V2.0.2.4
 * @date    2014/10/14
 * 
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm366_fc.h"

/** @addtogroup TX03_Periph_Driver
  * @{
  */

/** @defgroup FC 
  * @brief FC driver modules
  * @{
  */

/** @defgroup FC_Private_Defines
  * @{
  */
#define SECBIT_SECBIT_PASSWORD   ((uint32_t)0xA74A9D23)
#define SECBIT_SECBIT_SET        ((uint32_t)0x00000001)
#define SECBIT_SECBIT_CLEAR      ((uint32_t)0xFFFFFFFE)

#define SECBIT_SECURITY_MASK     ((uint32_t)0x00000001)
#define SECBIT_DISABLE_FLAG      ((uint32_t)0x00000000)

#define FLCS_BUSY_MASK           ((uint32_t)0x00000001)
#define FLCS_BUSY_FLAG           ((uint32_t)0x00000000)

#ifdef TSB_M366FD
#define FLCS_BLOCK0_MASK         ((uint32_t)0x00010000)
#define FLCS_BLOCK1_MASK         ((uint32_t)0x00020000)
#define FLCS_BLOCK2_MASK         ((uint32_t)0x00040000)
#define FLCS_BLOCK3_MASK         ((uint32_t)0x00080000)
#define FLCS_BLOCK4_MASK         ((uint32_t)0x00100000)
#define FLCS_BLOCK5_MASK         ((uint32_t)0x00200000)
#endif                          /*TSB_M366FD */

#define FLCS_BLOCK_FLAG          ((uint32_t)0x00000000)

#define FC_CMD_BC1_ADDR                     ((uint32_t)0x00005400)
#define FC_CMD_BC2_ADDR                     ((uint32_t)0x0000AA00)

#define FC_ERASE_CHIP_OVER_TIME             ((uint32_t)0x000FFFFF)      /* FC_ERASE_CHIP_OVER_TIME > 0.4s */
#define FC_ERASE_BLOCK_OVER_TIME            ((uint32_t)0x0003FFFF)      /* FC_ERASE_BLOCK_OVER_TIME > 0.1s */
#define FC_WRITE_PAGE_OVER_TIME             ((uint32_t)0x00000FFF)      /* FC_WRITE_PAGE_OVER_TIME > 1.25ms */
#define FC_SET_PROTECT_STATE_OVER_TIME      ((uint32_t)0x000FFFFF)

#define FLASH_AREA                  ((uint32_t)(FLASH_START_ADDR & 0xFFF80000U))

#ifdef TSB_M366FD
#define FC_PBA_BLOCK_0              ((uint32_t *) (FLASH_AREA + 0x00000000U))
#define FC_PBA_BLOCK_1              ((uint32_t *) (FLASH_AREA + 0x00000200U))
#define FC_PBA_BLOCK_2              ((uint32_t *) (FLASH_AREA + 0x00000400U))
#define FC_PBA_BLOCK_3              ((uint32_t *) (FLASH_AREA + 0x00000600U))
#define FC_PBA_BLOCK_4              ((uint32_t *) (FLASH_AREA + 0x00020000U))
#define FC_PBA_BLOCK_5              ((uint32_t *) (FLASH_AREA + 0x00020200U))
#endif                          /*TSB_M366FD */

#define FC_PBA_BLOCK_GROUP_0        ((uint32_t *) (FLASH_AREA + 0x00000000U))
#define FC_PBA_BLOCK_GROUP_1        ((uint32_t *) (FLASH_AREA + 0x00020000U))

#define FC_BLOCK_ADDR_MASK          ((uint32_t)0xFFFF8000)

#define FC_BLOCK_SIZE_32KB          ((uint32_t)0x00008000)

#define FC_PAGE_SIZE                ((uint32_t)(FLASH_PAGE_SIZE/4U))

/** @} */
/* End of group FC_Private_Defines */

/** @defgroup FC_Private_FunctionPrototypes
  * @{
  */
uint8_t FC_AddrToBlockNum(uint32_t Addr);

/** @} */
/* End of group FC_Private_FunctionPrototypes */

/** @defgroup FC_Private_Functions
  * @{
  */
/**
  * @brief  Convert address to block number
  * @param  Addr
  * @retval BlockNum
  *
  */
__attribute__((section("ram_func")))
uint8_t FC_AddrToBlockNum(uint32_t Addr)
{
    uint8_t BlockNum = FC_BLOCK_4;
    uint32_t temp = (Addr - FLASH_START_ADDR) / FC_BLOCK_SIZE_32KB;
    switch (temp) {
#ifdef TSB_M366FD
    case 0U:
        BlockNum = FC_BLOCK_4;
        break;
    case 1U:
        BlockNum = FC_BLOCK_5;
        break;
    case 2U:
    case 3U:
        BlockNum = FC_BLOCK_3;
        break;
    case 4U:
    case 5U:
    case 6U:
    case 7U:
        BlockNum = FC_BLOCK_2;
        break;
    case 8U:
    case 9U:
    case 10U:
    case 11U:
        BlockNum = FC_BLOCK_1;
        break;
    case 12U:
    case 13U:
    case 14U:
    case 15U:
        BlockNum = FC_BLOCK_0;
        break;
#endif                          /*TSB_M366FD */
    default:
        /* Do nothing */
        break;
    }
    return BlockNum;
}

/** @} */
/* End of group FC_Private_Functions */

/** @defgroup FC_Exported_Functions
  * @{
  */

/**
  * @brief  Set the value of SECBIT register.
  * @param  NewState: The value of SECBIT register.
  *   This parameter can be one of the following values:
  *   DISABLE or ENABLE.
  * @retval None.
  */
__attribute__((section("ram_func")))
void FC_SetSecurityBit(FunctionalState NewState)
{
    uint32_t tmp = TSB_FC->SECBIT;
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE) {
        /* Set FCSECBIT<SECBIT> to "1" that enable security function  */
        tmp |= SECBIT_SECBIT_SET;
    } else {
        /* Set FCSECBIT<SECBIT> to "0" that disable security function  */
        tmp &= SECBIT_SECBIT_CLEAR;
    }
    TSB_FC->SECBIT = SECBIT_SECBIT_PASSWORD;
    TSB_FC->SECBIT = tmp;
}

/**
  * @brief  Get the value of SECBIT register.
  * @param  None.
  * @retval DISABLE or ENABLE.
  *
  */
__attribute__((section("ram_func")))
FunctionalState FC_GetSecurityBit(void)
{
    uint32_t tmp = 0U;
    FunctionalState retval = ENABLE;

    tmp = TSB_FC->SECBIT & SECBIT_SECURITY_MASK;

    /* Check disable flag */
    if (tmp == SECBIT_DISABLE_FLAG) {
        retval = DISABLE;
    } else {                    /* Check enable flag */
        retval = ENABLE;
    }
    return retval;
}

/**
  * @brief  Get the status of the flash auto operation.
  * @param  None.
  * @retval BUSY or DONE.
  *
  */
__attribute__((section("ram_func")))
WorkState FC_GetBusyState(void)
{
    uint32_t tmp = 0U;
    WorkState retval = DONE;

    tmp = TSB_FC->FLCS & FLCS_BUSY_MASK;

    /* Check busy flag */
    if (tmp == FLCS_BUSY_FLAG) {
        retval = BUSY;
    } else {                    /* Check ready flag */
        retval = DONE;
    }
    return retval;
}

/**
  * @brief  Get the specified block protection state.
  * @param  BlockNum: The flash block number.
  *   This parameter can be one of the following values:
  *   FC_BLOCK_0, FC_BLOCK_1,FC_BLOCK_2,
  *   FC_BLOCK_3, FC_BLOCK_4, FC_BLOCK_5.
  * @retval DISABLE or ENABLE.
  *
  */
__attribute__((section("ram_func")))
FunctionalState FC_GetBlockProtectState(uint8_t BlockNum)
{
    uint32_t tmp = FLCS_BLOCK_FLAG;
    FunctionalState retval = ENABLE;

    /* Check the parameters */
    assert_param(IS_FC_BLOCK_NUM(BlockNum));

    switch (BlockNum) {
    case FC_BLOCK_0:
        tmp = TSB_FC->FLCS & FLCS_BLOCK0_MASK;
        break;
    case FC_BLOCK_1:
        tmp = TSB_FC->FLCS & FLCS_BLOCK1_MASK;
        break;
    case FC_BLOCK_2:
        tmp = TSB_FC->FLCS & FLCS_BLOCK2_MASK;
        break;
    case FC_BLOCK_3:
        tmp = TSB_FC->FLCS & FLCS_BLOCK3_MASK;
        break;
    case FC_BLOCK_4:
        tmp = TSB_FC->FLCS & FLCS_BLOCK4_MASK;
        break;
    case FC_BLOCK_5:
        tmp = TSB_FC->FLCS & FLCS_BLOCK5_MASK;
        break;
    default:
        /* Do nothing */
        break;
    }
    if (tmp == FLCS_BLOCK_FLAG) {
        /* Return protection status of each block */
        retval = DISABLE;
    } else {
        retval = ENABLE;
    }
    return retval;
}

/**
  * @brief  Program the protection bit to make the specified block protected.
  * @param  BlockNum: The flash block number.
  *   This parameter can be one of the following values:
  *   FC_BLOCK_0, FC_BLOCK_1,FC_BLOCK_2,
  *   FC_BLOCK_3, FC_BLOCK_4, FC_BLOCK_5.
  * @retval FC_SUCCESS, FC_ERROR_PROTECTED, FC_ERROR_OVER_TIME.
  *
  */
__attribute__((section("ram_func")))
FC_Result FC_ProgramBlockProtectState(uint8_t BlockNum)
{
    FC_Result retval = FC_SUCCESS;
    volatile uint32_t *addr1 = (uint32_t *) (FLASH_START_ADDR + FC_CMD_BC1_ADDR);
    volatile uint32_t *addr2 = (uint32_t *) (FLASH_START_ADDR + FC_CMD_BC2_ADDR);
    volatile uint32_t *PBA = FC_PBA_BLOCK_4;
    uint32_t counter = FC_SET_PROTECT_STATE_OVER_TIME;

    /* Check the parameters */
    assert_param(IS_FC_BLOCK_NUM(BlockNum));

    switch (BlockNum) {
#ifdef TSB_M366FD
    case FC_BLOCK_0:
        PBA = FC_PBA_BLOCK_0;
        break;
    case FC_BLOCK_1:
        PBA = FC_PBA_BLOCK_1;
        break;
    case FC_BLOCK_2:
        PBA = FC_PBA_BLOCK_2;
        break;
    case FC_BLOCK_3:
        PBA = FC_PBA_BLOCK_3;
        break;
    case FC_BLOCK_4:
        PBA = FC_PBA_BLOCK_4;
        break;
    case FC_BLOCK_5:
        PBA = FC_PBA_BLOCK_5;
        break;
#endif                          /*TSB_M366FD */
    default:
        /* Do nothing */
        break;
    }

    if (ENABLE == FC_GetBlockProtectState(BlockNum)) {
        retval = FC_ERROR_PROTECTED;
    } else {
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 1 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 2 */
        *addr1 = (uint32_t) 0x0000009A; /* bus cycle 3 */
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 4 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 5 */
        *addr1 = (uint32_t) 0x0000009A; /* bus cycle 6 */
        *PBA = (uint32_t) 0x0000009A;   /* bus cycle 7 */
        __DSB();

        while (BUSY == FC_GetBusyState()) {     /* check if FLASH is busy with overtime counter */
            if (!(counter--)) { /* check overtime */
                retval = FC_ERROR_OVER_TIME;
                break;
            } else {
                /* Do nothing */
            }
        }
    }

    return retval;
}

/**
  * @brief  Erase the protection bits to make the specified blocks unprotected.
  * @param  BlockGroup: Block group for erasing of the protection bits.
  *   This parameter can be one of the following values:
  *   FC_BLOCK_GROUP_1: block 4 through 5.
  *   FC_BLOCK_GROUP_0: the other blocks.
  * @retval FC_SUCCESS, FC_ERROR_OVER_TIME.
  *
  */
__attribute__((section("ram_func")))
FC_Result FC_EraseBlockProtectState(uint8_t BlockGroup)
{
    FC_Result retval = FC_SUCCESS;
    volatile uint32_t *addr1 = (uint32_t *) (FLASH_START_ADDR + FC_CMD_BC1_ADDR);
    volatile uint32_t *addr2 = (uint32_t *) (FLASH_START_ADDR + FC_CMD_BC2_ADDR);
    volatile uint32_t *PBA = FC_PBA_BLOCK_GROUP_0;
    uint32_t counter = FC_SET_PROTECT_STATE_OVER_TIME;

    /* Check the parameters */
    assert_param(IS_FC_BLOCK_GROUP(BlockGroup));

    switch (BlockGroup) {
    case FC_BLOCK_GROUP_0:
        PBA = FC_PBA_BLOCK_GROUP_0;
        break;
    case FC_BLOCK_GROUP_1:
        PBA = FC_PBA_BLOCK_GROUP_1;
        break;
    default:
        /* Do nothing */
        break;
    }

    *addr1 = (uint32_t) 0x000000AA;     /* bus cycle 1 */
    *addr2 = (uint32_t) 0x00000055;     /* bus cycle 2 */
    *addr1 = (uint32_t) 0x0000006A;     /* bus cycle 3 */
    *addr1 = (uint32_t) 0x000000AA;     /* bus cycle 4 */
    *addr2 = (uint32_t) 0x00000055;     /* bus cycle 5 */
    *addr1 = (uint32_t) 0x0000006A;     /* bus cycle 6 */
    *PBA = (uint32_t) 0x0000006A;       /* bus cycle 7 */
    __DSB();

    while (BUSY == FC_GetBusyState()) { /* check if FLASH is busy with overtime counter */
        if (!(counter--)) {     /* check overtime */
            retval = FC_ERROR_OVER_TIME;
            break;
        } else {
            /* Do nothing */
        }
    }

    return retval;
}

/**
  * @brief  Write data to the specified page.
  * @param  PageAddr: The page start address.
  *         Data: The pointer to data buffer to be written into the page.
  *         The data size should be 512Byte.
  * @retval FC_SUCCESS, FC_ERROR_PROTECTED, FC_ERROR_OVER_TIME.
  *
  */
__attribute__((section("ram_func")))
FC_Result FC_WritePage(uint32_t PageAddr, uint32_t * Data)
{
    FC_Result retval = FC_SUCCESS;
    volatile uint32_t *addr1 = (uint32_t *) (FLASH_START_ADDR + FC_CMD_BC1_ADDR);
    volatile uint32_t *addr2 = (uint32_t *) (FLASH_START_ADDR + FC_CMD_BC2_ADDR);
    volatile uint32_t *addr3 = (uint32_t *) PageAddr;
    uint32_t counter = FC_WRITE_PAGE_OVER_TIME;
    uint32_t i = 0U;
    uint32_t *source = Data;
    uint8_t BlockNum = FC_BLOCK_4;

    assert_param(IS_FC_ADDR(PageAddr)); /* Check whether it is in the flash address range */
    assert_param(IS_FC_PAGE_ADDR(PageAddr));    /* Check whether it is a page start address */
    assert_param(IS_POINTER_NOT_NULL(Data));

    BlockNum = FC_AddrToBlockNum(PageAddr);

    if (ENABLE == FC_GetBlockProtectState(BlockNum)) {
        retval = FC_ERROR_PROTECTED;
    } else {
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 1 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 2 */
        *addr1 = (uint32_t) 0x000000A0; /* bus cycle 3 */
        for (i = 0U; i < FC_PAGE_SIZE; i++) {   /* bus cycle 4~131 */
            *addr3 = *source;
            source++;
        }
        __DSB();

        while (BUSY == FC_GetBusyState()) {     /* check if FLASH is busy with overtime counter */
            if (!(counter--)) { /* check overtime */
                retval = FC_ERROR_OVER_TIME;
                break;
            } else {
                /* Do nothing */
            }
        }
    }

    return retval;
}

/**
  * @brief  Erase the contents of the specified block.
  * @param  BlockAddr: The block start address.
  * @retval FC_SUCCESS, FC_ERROR_PROTECTED, FC_ERROR_OVER_TIME.
  *
  */
__attribute__((section("ram_func")))
FC_Result FC_EraseBlock(uint32_t BlockAddr)
{
    FC_Result retval = FC_SUCCESS;
    volatile uint32_t *addr1 = (uint32_t *) (FLASH_START_ADDR + FC_CMD_BC1_ADDR);
    volatile uint32_t *addr2 = (uint32_t *) (FLASH_START_ADDR + FC_CMD_BC2_ADDR);
    volatile uint32_t *BA = 0U;
    uint32_t counter = FC_ERASE_BLOCK_OVER_TIME;
    uint8_t BlockNum = FC_BLOCK_4;

    BlockAddr &= FC_BLOCK_ADDR_MASK;
    BA = (uint32_t *) BlockAddr;

    assert_param(IS_FC_ADDR(BlockAddr));        /* Check whether it is in the flash address range */

    BlockNum = FC_AddrToBlockNum(BlockAddr);

    if (ENABLE == FC_GetBlockProtectState(BlockNum)) {
        retval = FC_ERROR_PROTECTED;
    } else {
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 1 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 2 */
        *addr1 = (uint32_t) 0x00000080; /* bus cycle 3 */
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 4 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 5 */
        *BA = (uint32_t) 0x00000030;    /* bus cycle 6 */
        __DSB();

        while (BUSY == FC_GetBusyState()) {     /* check if FLASH is busy with overtime counter */
            if (!(counter--)) { /* check overtime */
                retval = FC_ERROR_OVER_TIME;
                break;
            } else {
                /* Do nothing */
            }
        }
    }

    return retval;
}

/**
  * @brief  Erase the contents of the entire chip.
  * @param  None.
  * @retval FC_SUCCESS, FC_ERROR_PROTECTED, FC_ERROR_OVER_TIME.
  *
  */
__attribute__((section("ram_func")))
FC_Result FC_EraseChip(void)
{
    FC_Result retval = FC_SUCCESS;
    volatile uint32_t *addr1 = (uint32_t *) (FLASH_START_ADDR + FC_CMD_BC1_ADDR);
    volatile uint32_t *addr2 = (uint32_t *) (FLASH_START_ADDR + FC_CMD_BC2_ADDR);
    uint32_t counter = FC_ERASE_CHIP_OVER_TIME;
#ifdef TSB_M366FD
    FunctionalState result0 = FC_GetBlockProtectState(FC_BLOCK_0);
    FunctionalState result1 = FC_GetBlockProtectState(FC_BLOCK_1);
    FunctionalState result2 = FC_GetBlockProtectState(FC_BLOCK_2);
    FunctionalState result3 = FC_GetBlockProtectState(FC_BLOCK_3);
    FunctionalState result4 = FC_GetBlockProtectState(FC_BLOCK_4);
    FunctionalState result5 = FC_GetBlockProtectState(FC_BLOCK_5);
#endif                          /*TSB_M366FD */
    if (
#ifdef TSB_M366FD
           (ENABLE == result0) && (ENABLE == result1) && (ENABLE == result2) &&
           (ENABLE == result3) && (ENABLE == result4) && (ENABLE == result5)
#endif                          /*TSB_M366FD */
        ) {
        retval = FC_ERROR_PROTECTED;
    } else {
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 1 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 2 */
        *addr1 = (uint32_t) 0x00000080; /* bus cycle 3 */
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 4 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 5 */
        *addr1 = (uint32_t) 0x00000010; /* bus cycle 6 */
        __DSB();

        while (BUSY == FC_GetBusyState()) {     /* check if FLASH is busy with overtime counter */
            if (!(counter--)) { /* check overtime */
                retval = FC_ERROR_OVER_TIME;
                break;
            } else {
                /* Do nothing */
            }
        }
    }

    return retval;
}

/** @} */
/* End of group FC_Exported_Functions */

/** @} */
/* End of group FC */

/** @} */
/* End of group TX03_Periph_Driver */
