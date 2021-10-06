/*******************************************************************************

* @file    FlashPrg.C
* @brief   Flash Programming Functions adapted for CY8C5xxLP Device 258kB Flash
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

#include "CyFlash.h"
#include "Bootloadable.h"
#include "EEPROM_ModeStorage.h"
#include "psoc5lp.h"
#include "flash_hal.h"

//Results constants
#define SUCCESS (0x00u)
#define FAIL    (0x01u)

//Chip characteristics constants
#define ROW_SIZE                (0x100u)
#define ROW_ADDITIONAL_SIZE     (0x020u)
#define ROWS_COUNT              (0x100u)
#define EEPROM_ARRAY_ID         (0x40u)
#define EEPROM_ROW_SIZE         (0x10u)
#define EEPROM_START_ADDR       (0x40008000u)
#define EEPROM_HEX_START_ADDR   (0x00040000u)
#define EEPROM_HEX_END_ADDR     (0x00040800u)
#define ERASED_VALUE            (0x00u)

#define ARRAY_ID(addr)          (((addr) >> 16) & 0xFFu)
#define ROW_ID(addr)            (((addr) >>  8) & 0xFFu)
#define EEPROM_ROW_ID(addr)     (((addr) >>  4) & 0xFFu)

#define APP_INACTIVE            (0u)
#define APP_ACTIVE              (1u)

/* Addresses of activity bytes in bootloader metadata */
#define MD_SIZEOF               (64u)
#define ACTIVE_OFFSET           (16u)
#define KIT_PROG_ACTIVE_ADDR    (CYDEV_FLASH_BASE + (CYDEV_FLASH_SIZE - (0u * CYDEV_FLS_ROW_SIZE) - MD_SIZEOF) + ACTIVE_OFFSET)
#define CUSTOM_APP_ACTIVE_ADDR  (CYDEV_FLASH_BASE + (CYDEV_FLASH_SIZE - (1u * CYDEV_FLS_ROW_SIZE) - MD_SIZEOF) + ACTIVE_OFFSET)

/*************************************************************************
*   Purpose:    Erases the addressed row and then programs it with data
*               both user data and ECC/configurtation data.
*   Parameter:
*               IN arrayID      - Array ID address
*               IN rowID        - Row ID
*               IN size         - Size of Row
*               OUT buf         - Buffer which is writing to the Flash
*   Return:
*           0 on success, an error code otherwise
*************************************************************************/
static cystatus WriteRow(uint8_t arrayID, uint16_t rowID, uint32_t size, const uint8_t *buf)
{
    (void)size;
    return CyWriteRowData(arrayID, rowID, buf);
}

/*************************************************************************
*   Purpose:    Erases the addressed row.
*   Parameter:
*               IN address      - flash address
*   Return:
*           0 on success, an error code otherwise
*************************************************************************/
uint32_t EraseSector(uint32_t adr)
{
    uint8_t arrayID = (uint8_t)ARRAY_ID(adr);
    uint16_t rowID = (uint16_t)ROW_ID(adr);
    return CyFlash_EraseRow(arrayID, rowID);
}

/*************************************************************************
*   Purpose: Initialize Flash Programming Functions
*   Parameter:
*               IN adr  - Device Base Address
*               IN clk  - Clock Frequency (Hz)
*               IN fnc  - Function Code (1 - Erase, 2 - Program, 3 - Verify)
*   Return:
*               0 on success, an error code otherwise
*************************************************************************/
uint32_t Init(uint32_t adr, uint32_t clk, uint32_t fnc)
{
    (void)adr;
    (void)clk;
    (void)fnc;
    (void)CySetTemp();
    return SUCCESS;                                  // Finished without Errors
}

/*************************************************************************
*   Purpose: De-Initialize Flash Programming Functions
*   Parameter:
*               IN fnc  - Function Code (1 - Erase, 2 - Program, 3 - Verify)
*   Return:
*               0 on success, an error code otherwise
*************************************************************************/
uint32_t UnInit(uint32_t fnc)
{
    (void)fnc;
    return SUCCESS;                                  // Finished without Errors
}

/*************************************************************************
*   Purpose: Program Page in Flash Memory
*   Parameter:
*               IN adr  - Page Start Address
*               IN  sz  - Page Size
*               IN buf  - Page Data
*   Return:
*               0 on success, an error code otherwise
*************************************************************************/
uint32_t ProgramPage(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    uint32_t hr = SUCCESS;
    uint8_t data[ROW_SIZE+ROW_ADDITIONAL_SIZE];

    uint8_t arrayID = (uint8_t)ARRAY_ID(adr);
    uint16_t rowID = (uint16_t)ROW_ID(adr);
    uint32_t size = sz;
    if (size > ROW_SIZE)
    {
        size = ROW_SIZE;
    }
    // coverity[misra_c_2012_rule_21_15_violation]
    (void)memcpy(data, buf, size);
    (void)memset(&data[size], 0, ROW_SIZE+ROW_ADDITIONAL_SIZE - size);

    //Programming Row Size is 256 bytes, but each row has additional 32-bytes
    //Configuration Data.
    //During programming process, the row latch needs to be loaded with all
    //the 288 bytes. In this scenario, the 256 code bytes are concatenated
    //with the 32 zero bytes:
    //There are two reasons for this:
    //  1. uVision doesn't support Configuration data programming.
    //  2. We always program rows by 288 bytes, since it coveres two cases:
    //      a) When Configuration data is supported by the silicon.
    //         Then 32-bytes will be programmed with zero bytes.
    //      b) And when configuration data isn't supported by the silicon.
    //         Then these extra 32-bytes will be ignored by SPC.
    //This step is done to program all flash rows.

    if ((adr >= EEPROM_HEX_START_ADDR) && (adr < EEPROM_HEX_END_ADDR))    //Program EEPROM if the address in range of 0x00040000 ... 0x00040800
    {
        for (uint32_t i = 0u; i < ROW_SIZE; i += EEPROM_ROW_SIZE)
        {
            rowID = (uint16_t)EEPROM_ROW_ID((adr + i));    //Calculate rowID
            const uint8_t *eepromData = (uint8_t *) (EEPROM_START_ADDR + (adr & 0xFFFFu) + i);    //Set address to EEPROM memory by given address - adr
            if (memcmp(eepromData, &data[i], EEPROM_ROW_SIZE) != 0) //If 16 byte EEPROM row buffer is not identical with 16 byte hex EEPROM row buffer
            {                                                                           //then program row to the EEPROM memory
                hr = WriteRow(EEPROM_ARRAY_ID, rowID, EEPROM_ROW_SIZE, &data[i]);
                if (hr != SUCCESS)
                {
                    break;
                }
            }
        }
    }
    else
    {   //Program Flash if the address in range of 0x00000000 .. 0x00040000
        hr = WriteRow(arrayID, rowID, ROW_SIZE+ROW_ADDITIONAL_SIZE, data);
    }

    return hr;
}

/******************************************************************************
*  WriteFlashByte
***************************************************************************//**
* This API writes to flash the specified data.
*
* @param[in] address        The address in flash.
* @param[in] inputValue     The one-byte data to write
*
******************************************************************************/
static void WriteFlashByte(uint32_t address, uint8_t inputValue)
{
    uint32_t flsAddr = address - CYDEV_FLASH_BASE;
    uint8_t rowData[CYDEV_FLS_ROW_SIZE];

    uint8_t arrayId = (uint8_t) (flsAddr / CYDEV_FLS_SECTOR_SIZE);
    uint16_t rowNum = (uint16_t) ((flsAddr % CYDEV_FLS_SECTOR_SIZE) / CYDEV_FLS_ROW_SIZE);
    uint32_t baseAddr = address - (address % CYDEV_FLS_ROW_SIZE);
    const uint8_t *src = (uint8_t *)((uint32_t)(CYDEV_FLASH_BASE)+baseAddr);

    (void)memcpy(rowData, src, CYDEV_FLS_ROW_SIZE);
    rowData[address % CYDEV_FLS_ROW_SIZE] = inputValue;

    (void)CyWriteRowData(arrayId, rowNum, rowData);

    /*
     * When writing to flash, data in the instruction cache can become stale
     * A call to CyFlushCache() is required to invalidate the data in the
     * cache and force fresh information to be loaded.
     */
    CyFlushCache();
}

/******************************************************************************
*  SetKitProgActiveApp
***************************************************************************//**
* Sets the KitProg as active application.
* Active application is application which will be loaded after a next reset event.
*
*  @note
*  The active application number is not set directly, but the boolean
*  mark instead means that the application is active or not for the relative
*  metadata. Both metadata sections are updated. For example, if the second
*  application is to be set active, then in the metadata section for the first
*  application there will be a "0" written, which means that it is not active,
*  and for the second metadata section there will be a "1" written, which means
*  that it is active.
*
******************************************************************************/
void SetKitProgActiveApp(uint8_t mode)
{
    uint8 buff[CYDEV_FLS_ROW_SIZE + CYDEV_ECC_ROW_SIZE];

    /*
    * Use CySetTemp API to initialize SPC.
    * This step enables writing to flash memory.
    */
    if ( CySetTemp() != CYRET_SUCCESS )
    {
        CyHalt(0x00u);
    }

    (void)CySetFlashEEBuffer(buff);

    /* Mark KitProg as active application */
    WriteFlashByte( KIT_PROG_ACTIVE_ADDR, APP_ACTIVE );

    /* Mark Custom Application as inactive application */
    WriteFlashByte( CUSTOM_APP_ACTIVE_ADDR, APP_INACTIVE );

    EEPROM_ModeStorage_Start();
    /* Provide a 10us delay for EEPROM data to stabilize. */
    CyDelayUs(10u);

    /* Write the MODE_BULK as current mode setting to the EEPROM. */
    if (CySetTemp() == CYRET_SUCCESS)
    {
        (void)EEPROM_ModeStorage_ByteWrite(mode, 0u, 0u);
    }
    EEPROM_ModeStorage_Stop();
    /* Provide a 20us delay to ensure that the SPC has completed the write operation. */
    CyDelayUs(20u);

    CySoftwareReset();
}

