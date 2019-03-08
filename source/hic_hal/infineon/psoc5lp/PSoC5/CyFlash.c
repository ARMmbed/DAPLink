/***************************************************************************//**
* \file CyFlash.c
* \version 5.70
*
* \brief Provides an API for the FLASH/EEPROM.
*
* \note This code is endian agnostic.
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

/*  The number of EEPROM arrays */
#define CY_FLASH_EEPROM_NUMBER_ARRAYS                 (1u)


/*******************************************************************************
* Holds the die temperature, updated by CySetTemp(). Used for flash writing.
* The first byte is the sign of the temperature (0 = negative, 1 = positive).
* The second byte is the magnitude.
*******************************************************************************/
uint8 dieTemperature[CY_FLASH_DIE_TEMP_DATA_SIZE];

#if(CYDEV_ECC_ENABLE == 0)
    static uint8 * rowBuffer = 0;
#endif  /* (CYDEV_ECC_ENABLE == 0) */


static cystatus CySetTempInt(void);


/*******************************************************************************
* Function Name: CyFlash_Start
****************************************************************************//**
*
*  Enable the Flash.
*
*******************************************************************************/
void CyFlash_Start(void)
{
    uint8 interruptState;

    interruptState = CyEnterCriticalSection();


    /***************************************************************************
    * Enable SPC clock. This also internally enables the 36MHz IMO, since this
    * is required for the SPC to function.
    ***************************************************************************/
    CY_FLASH_PM_ACT_CFG0_REG    |= CY_FLASH_PM_ACT_CFG0_EN_CLK_SPC;
    CY_FLASH_PM_ALTACT_CFG0_REG |= CY_FLASH_PM_ALTACT_CFG0_EN_CLK_SPC;


    /***************************************************************************
    * The wake count defines the number of Bus Clock cycles it takes for the
    * flash or eeprom to wake up from a low power mode independent of the chip
    * power mode. Wake up time for these blocks is 5 us.
    * The granularity of this register is 2 Bus Clock cycles, so a value of 0x1E
    * (30d) defines the wake up time as 60 cycles of the Bus Clock.
    * This register needs to be written with a value dependent on the Bus Clock
    * frequency so that the duration of the cycles is equal to or greater than
    * the 5 us delay required.
    ***************************************************************************/
    CY_FLASH_SPC_FM_EE_WAKE_CNT_REG = CY_FLASH_SPC_FM_EE_WAKE_CNT_80MHZ;


    /***************************************************************************
    * Enable flash. Active flash macros consume current, but re-enabling a
    * disabled flash macro takes 5us. If the CPU attempts to fetch out of the
    * macro during that time, it will be stalled. This bit allows the flash to
    * be enabled even if the CPU is disabled, which allows a quicker return to
    * code execution.
    ***************************************************************************/
    CY_FLASH_PM_ACT_CFG12_REG    |= CY_FLASH_PM_ACT_CFG12_EN_FM;
    CY_FLASH_PM_ALTACT_CFG12_REG |= CY_FLASH_PM_ALTACT_CFG12_EN_FM;

    while(0u == (CY_FLASH_SPC_FM_EE_CR_REG & CY_FLASH_EE_EE_AWAKE))
    {
        /* Non-zero status denotes that the EEPROM/Flash is awake & powered. */
    }

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CyFlash_Stop
****************************************************************************//**
*
*  Disable the Flash.
*
* \sideeffect
*  This setting is ignored as long as the CPU is currently running.  This will
*  only take effect when the CPU is later disabled.
*
*******************************************************************************/
void CyFlash_Stop(void)
{
    uint8 interruptState;

    interruptState = CyEnterCriticalSection();

    CY_FLASH_PM_ACT_CFG12_REG    &= ((uint8)(~CY_FLASH_PM_ACT_CFG12_EN_FM));
    CY_FLASH_PM_ALTACT_CFG12_REG &= ((uint8)(~CY_FLASH_PM_ALTACT_CFG12_EN_FM));

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CyFlash_EraseRow
****************************************************************************//**
*
*  Erases a single row of flash. Reports success or reason for failure.
*  The API does not return until the erase operation is complete.
*
*  \param arrayID:    ID of the array to erase.
*   The arrays in the part are sequential starting at the first ID for the
*   Flash memory type.
*  \param rowAddress: Row address within the specified arrayId.
*
*  \return
*  status:
*   CYRET_SUCCESS if successful.
*   CYRET_LOCKED if the SPC is already in use.
*   CYRET_CANCELED if command not accepted
*   CYRET_UNKNOWN if there was an SPC error.
*   CYRET_BAD_PARAM if one or more invalid parameters
*
*******************************************************************************/
cystatus CyFlash_EraseRow(uint8 arrayId, uint16 rowAddress)
{
    cystatus status = CYRET_SUCCESS;

    if (arrayId > CY_SPC_LAST_FLASH_ARRAYID)
    {
        status = CYRET_BAD_PARAM;
    }
    else if(rowAddress > (CY_FLASH_NUMBER_ROWS/CY_FLASH_NUMBER_ARRAYS))
    {
        status = CYRET_BAD_PARAM;
    }
    else
    {
        if(CySpcLock() == CYRET_SUCCESS)
        {
            /* Erase flash row */
            status = CySpcEraseRow(arrayId, rowAddress, dieTemperature[0u], dieTemperature[1u]);

            if(CYRET_STARTED == status)
            {
                while(CY_SPC_BUSY)
                {
                    /* Wait for SPC to finish and get SPC status */
                    CyDelayUs(1u);
                }

                /* Hide SPC status */
                if(CY_SPC_STATUS_SUCCESS == CY_SPC_READ_STATUS)
                {
                    status = CYRET_SUCCESS;
                }
                else
                {
                    status = CYRET_UNKNOWN;
                }
            }
	        else
            {
                status = CYRET_CANCELED;
            }
            CySpcUnlock();
        }
        else
        {
            status = CYRET_LOCKED;
        }
    }

    return(status);
}


/*******************************************************************************
* Function Name: CySetTempInt
****************************************************************************//**
*
*  Sends a command to the SPC to read the die temperature. Sets a global value
*  used by the Write function. This function must be called once before
*  executing a series of Flash writing functions.
*
* \return
*  status:
*   CYRET_SUCCESS - if successful
*   CYRET_LOCKED  - if Flash writing already in use
*   CYRET_UNKNOWN - if there was an SPC error
*
*******************************************************************************/
static cystatus CySetTempInt(void)
{
    cystatus status;

    /* Make sure SPC is powered */
    CySpcStart();

    /* Plan for failure. */
    status = CYRET_UNKNOWN;

    if(CySpcLock() == CYRET_SUCCESS)
    {
        /* Write the command. */
        if(CYRET_STARTED == CySpcGetTemp(CY_TEMP_NUMBER_OF_SAMPLES))
        {
            do
            {
                if(CySpcReadData(dieTemperature, CY_FLASH_DIE_TEMP_DATA_SIZE) == CY_FLASH_DIE_TEMP_DATA_SIZE)
                {
                    status = CYRET_SUCCESS;

                    while(CY_SPC_BUSY)
                    {
                        /* Spin until idle. */
                        CyDelayUs(1u);
                    }
                    break;
                }

            } while(CY_SPC_BUSY);
        }

        CySpcUnlock();
    }
    else
    {
        status = CYRET_LOCKED;
    }

    return (status);
}


/*******************************************************************************
* Function Name: CyFlashGetSpcAlgorithm
****************************************************************************//**
*
*  Sends a command to the SPC to download code into RAM.
*
* \return
*  status:
*   CYRET_SUCCESS - if successful
*   CYRET_LOCKED  - if Flash writing already in use
*   CYRET_UNKNOWN - if there was an SPC error
*
*******************************************************************************/
cystatus CyFlashGetSpcAlgorithm(void)
{
    cystatus status;

    /* Make sure SPC is powered */
    CySpcStart();

    if(CySpcLock() == CYRET_SUCCESS)
    {
        status = CySpcGetAlgorithm();

        if(CYRET_STARTED == status)
        {
            while(CY_SPC_BUSY)
            {
                /* Spin until idle. */
                CyDelayUs(1u);
            }

            if(CY_SPC_STATUS_SUCCESS == CY_SPC_READ_STATUS)
            {
                status = CYRET_SUCCESS;
            }
        }
        CySpcUnlock();
    }
    else
    {
        status = CYRET_LOCKED;
    }

    return (status);
}


/*******************************************************************************
* Function Name: CySetTemp
****************************************************************************//**
*
*  This is a wraparound for CySetTempInt(). It is used to return the second
*  successful read of the temperature value.
*
* \return
*  status:
*   CYRET_SUCCESS if successful.
*   CYRET_LOCKED  if Flash writing already in use
*   CYRET_UNKNOWN if there was an SPC error.
*
*  uint8 dieTemperature[2]:
*   Holds the die temperature for the flash writing algorithm. The first byte is
*   the sign of the temperature (0 = negative, 1 = positive). The second byte is
*   the magnitude.
*
*******************************************************************************/
cystatus CySetTemp(void)
{
    cystatus status = CyFlashGetSpcAlgorithm();

    if(status == CYRET_SUCCESS)
    {
        status = CySetTempInt();
    }

    return (status);
}


/*******************************************************************************
* Function Name: CySetFlashEEBuffer
****************************************************************************//**
*
*  Sets the user supplied temporary buffer to store SPC data while performing
*  Flash and EEPROM commands. This buffer is only necessary when the Flash ECC
*  is disabled.
*
*  \param buffer:
*   The address of a block of memory to store temporary memory. The size of the
*   block of memory is CYDEV_FLS_ROW_SIZE + CYDEV_ECC_ROW_SIZE.
*
* \return
*  status:
*   CYRET_SUCCESS if successful.
*   CYRET_BAD_PARAM if the buffer is NULL
*
*******************************************************************************/
cystatus CySetFlashEEBuffer(uint8 * buffer)
{
    cystatus status = CYRET_SUCCESS;

    CySpcStart();

    #if(CYDEV_ECC_ENABLE == 0)

        if(NULL == buffer)
        {
            rowBuffer = rowBuffer;
            status = CYRET_BAD_PARAM;
        }
        else if(CySpcLock() != CYRET_SUCCESS)
        {
            rowBuffer = rowBuffer;
            status = CYRET_LOCKED;
        }
        else
        {
            rowBuffer = buffer;
            CySpcUnlock();
        }

    #else

        /* To suppress warning */
        buffer = buffer;

    #endif  /* (CYDEV_ECC_ENABLE == 0u) */

    return(status);
}


/*******************************************************************************
* Function Name: CyWriteRowData
****************************************************************************//**
*
*  Sends a command to the SPC to load and program a row of data in
*  Flash or EEPROM.
*
*  \param arrayID:    ID of the array to write.
*   The type of write, Flash or EEPROM, is determined from the array ID.
*   The arrays in the part are sequential starting at the first ID for the
*   specific memory type. The array ID for the Flash memory lasts from 0x00 to
*   0x3F and for the EEPROM memory it lasts from 0x40 to 0x7F.
*  \param rowAddress: rowAddress of flash row to program.
*  \param rowData:    Array of bytes to write.
*
* \return
*  status:
*   CYRET_SUCCESS if successful.
*   CYRET_LOCKED if the SPC is already in use.
*   CYRET_CANCELED if command not accepted
*   CYRET_UNKNOWN if there was an SPC error.
*
*******************************************************************************/
cystatus CyWriteRowData(uint8 arrayId, uint16 rowAddress, const uint8 * rowData)
{
    uint16 rowSize;
    cystatus status;

    rowSize = (arrayId > CY_SPC_LAST_FLASH_ARRAYID) ? CYDEV_EEPROM_ROW_SIZE : CYDEV_FLS_ROW_SIZE;
    status = CyWriteRowFull(arrayId, rowAddress, rowData, rowSize);

    return(status);
}


/*******************************************************************
* If "Enable Error Correcting Code (ECC)" and "Store Configuration
* Data in ECC" DWR options are disabled, ECC section is available
* for user data.
*******************************************************************/
#if ((CYDEV_ECC_ENABLE == 0u) && (CYDEV_CONFIGURATION_ECC == 0u))

    /*******************************************************************************
    * Function Name: CyWriteRowConfig
    ****************************************************************************//**
    *
    *  Sends a command to the SPC to load and program a row of config data in the
    *  Flash. This function is only valid for Flash array IDs (not for EEPROM).
    *
    *  \param arrayId:      ID of the array to write
    *   The arrays in the part are sequential starting at the first ID for the
    *   specific memory type. The array ID for the Flash memory lasts
    *   from 0x00 to 0x3F.
    *  \param rowAddress:   The address of the sector to erase.
    *  \param rowECC:       The array of bytes to write.
    *
    * \return
    *  status:
    *   CYRET_SUCCESS if successful.
    *   CYRET_LOCKED if the SPC is already in use.
    *   CYRET_CANCELED if command not accepted
    *   CYRET_UNKNOWN if there was an SPC error.
    *
    *******************************************************************************/
    cystatus CyWriteRowConfig(uint8 arrayId, uint16 rowAddress, const uint8 * rowECC)\

    {
        cystatus status;

        status = CyWriteRowFull(arrayId, rowAddress, rowECC, CYDEV_ECC_ROW_SIZE);

        return (status);
    }

#endif  /* ((CYDEV_ECC_ENABLE == 0u) && (CYDEV_CONFIGURATION_ECC == 0u)) */



/*******************************************************************************
* Function Name: CyWriteRowFull
****************************************************************************//**
*  Sends a command to the SPC to load and program a row of data in the Flash.
*  rowData array is expected to contain Flash and ECC data if needed.
*
*  \param arrayId:    FLASH or EEPROM array id.
*  \param rowData:    Pointer to a row of data to write.
*  \param rowNumber:  Zero based number of the row.
*  \param rowSize:    Size of the row.
*
* \return
*  CYRET_SUCCESS if successful.
*  CYRET_LOCKED if the SPC is already in use.
*  CYRET_CANCELED if command not accepted
*  CYRET_UNKNOWN if there was an SPC error.
*
*******************************************************************************/
cystatus CyWriteRowFull(uint8 arrayId, uint16 rowNumber, const uint8* rowData, uint16 rowSize) \

{
    cystatus status = CYRET_SUCCESS;

    if((arrayId <=  CY_SPC_LAST_FLASH_ARRAYID) && (arrayId > (CY_FLASH_NUMBER_ARRAYS  + CY_SPC_FIRST_FLASH_ARRAYID)))
    {
        status = CYRET_BAD_PARAM;
    }

    if(arrayId > CY_SPC_LAST_EE_ARRAYID)
    {
        status = CYRET_BAD_PARAM;
    }

    if((arrayId >= CY_SPC_FIRST_EE_ARRAYID) && (arrayId > (CY_FLASH_EEPROM_NUMBER_ARRAYS + CY_SPC_FIRST_EE_ARRAYID)))
    {
        status = CYRET_BAD_PARAM;
    }

    if(arrayId <=  CY_SPC_LAST_FLASH_ARRAYID)
    {
        /* Flash */
        if(rowNumber > (CY_FLASH_NUMBER_ROWS/CY_FLASH_NUMBER_ARRAYS))
        {
            status = CYRET_BAD_PARAM;
        }
    }
    else
    {
        /* EEPROM */
        if(rowNumber > (CY_EEPROM_NUMBER_ROWS/CY_FLASH_EEPROM_NUMBER_ARRAYS))
        {
            status = CYRET_BAD_PARAM;
        }

        if(CY_EEPROM_SIZEOF_ROW != rowSize)
        {
            status = CYRET_BAD_PARAM;
        }
    }

    if(rowData == NULL)
    {
        status = CYRET_BAD_PARAM;
    }


    if(status == CYRET_SUCCESS)
    {
        if(CySpcLock() == CYRET_SUCCESS)
        {
            /* Load row data into SPC internal latch */
            status = CySpcLoadRowFull(arrayId, rowNumber, rowData, rowSize);

            if(CYRET_STARTED == status)
            {
                while(CY_SPC_BUSY)
                {
                    /* Wait for SPC to finish and get SPC status */
                    CyDelayUs(1u);
                }

                /* Hide SPC status */
                if(CY_SPC_STATUS_SUCCESS == CY_SPC_READ_STATUS)
                {
                    status = CYRET_SUCCESS;
                }
                else
                {
                    status = CYRET_UNKNOWN;
                }

                if(CYRET_SUCCESS == status)
                {
                    /* Erase and program flash with data from SPC interval latch */
                    status = CySpcWriteRow(arrayId, rowNumber, dieTemperature[0u], dieTemperature[1u]);

                    if(CYRET_STARTED == status)
                    {
                        while(CY_SPC_BUSY)
                        {
                            /* Wait for SPC to finish and get SPC status */
                            CyDelayUs(1u);
                        }

                        /* Hide SPC status */
                        if(CY_SPC_STATUS_SUCCESS == CY_SPC_READ_STATUS)
                        {
                            status = CYRET_SUCCESS;
                        }
                        else
                        {
                            status = CYRET_UNKNOWN;
                        }
                    }
                }
            }
            CySpcUnlock();
        }   /* if(CySpcLock() == CYRET_SUCCESS) */
        else
        {
            status = CYRET_LOCKED;
        }
    }

    return(status);
}


/*******************************************************************************
* Function Name: CyFlash_SetWaitCycles
****************************************************************************//**
*
*  Sets the number of clock cycles the cache will wait before it samples data
*  coming back from the Flash. This function must be called before increasing
*  the CPU clock frequency. It can optionally be called after lowering the CPU
*  clock frequency in order to improve the CPU performance.
*
*  \param uint8 freq:
*   Frequency of operation in Megahertz.
*
*******************************************************************************/
void CyFlash_SetWaitCycles(uint8 freq)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /***************************************************************************
    * The number of clock cycles the cache will wait before it samples data
    * coming back from the Flash must be equal or greater to to the CPU frequency
    * outlined in clock cycles.
    ***************************************************************************/

    if (freq < CY_FLASH_CACHE_WS_1_FREQ_MAX)
    {
        CY_FLASH_CONTROL_REG = (CY_FLASH_CONTROL_REG & (uint8)(~CY_FLASH_CACHE_WS_VALUE_MASK)) |
                                    CY_FLASH_CACHE_WS_1_VALUE_MASK;
    }
    else if (freq < CY_FLASH_CACHE_WS_2_FREQ_MAX)
    {
        CY_FLASH_CONTROL_REG = (CY_FLASH_CONTROL_REG & (uint8)(~CY_FLASH_CACHE_WS_VALUE_MASK)) |
                                    CY_FLASH_CACHE_WS_2_VALUE_MASK;
    }
    else if (freq < CY_FLASH_CACHE_WS_3_FREQ_MAX)
    {
        CY_FLASH_CONTROL_REG = (CY_FLASH_CONTROL_REG & (uint8)(~CY_FLASH_CACHE_WS_VALUE_MASK)) |
                                    CY_FLASH_CACHE_WS_3_VALUE_MASK;
    }
#if (CY_PSOC5)
    else if (freq < CY_FLASH_CACHE_WS_4_FREQ_MAX)
    {
        CY_FLASH_CONTROL_REG = (CY_FLASH_CONTROL_REG & (uint8)(~CY_FLASH_CACHE_WS_VALUE_MASK)) |
                                    CY_FLASH_CACHE_WS_4_VALUE_MASK;
    }
    else if (freq <= CY_FLASH_CACHE_WS_5_FREQ_MAX)
    {
        CY_FLASH_CONTROL_REG = (CY_FLASH_CONTROL_REG & (uint8)(~CY_FLASH_CACHE_WS_VALUE_MASK)) |
                                    CY_FLASH_CACHE_WS_5_VALUE_MASK;
    }
#endif  /* (CY_PSOC5) */
    else
    {
        /* Halt CPU in debug mode if frequency is invalid */
        CYASSERT(0u != 0u);
    }

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CyEEPROM_Start
****************************************************************************//**
*
*  Enable the EEPROM.
*
*******************************************************************************/
void CyEEPROM_Start(void)
{
    uint8 interruptState;

    interruptState = CyEnterCriticalSection();


    /***************************************************************************
    * Enable SPC clock. This also internally enables the 36MHz IMO, since this
    * is required for the SPC to function.
    ***************************************************************************/
    CY_FLASH_PM_ACT_CFG0_REG    |= CY_FLASH_PM_ACT_CFG0_EN_CLK_SPC;
    CY_FLASH_PM_ALTACT_CFG0_REG |= CY_FLASH_PM_ALTACT_CFG0_EN_CLK_SPC;


    /***************************************************************************
    * The wake count defines the number of Bus Clock cycles it takes for the
    * flash or EEPROM to wake up from a low power mode independent of the chip
    * power mode. Wake up time for these blocks is 5 us.
    * The granularity of this register is 2 Bus Clock cycles, so a value of 0x1E
    * (30d) defines the wake up time as 60 cycles of the Bus Clock.
    * This register needs to be written with a value dependent on the Bus Clock
    * frequency so that the duration of the cycles is equal to or greater than
    * the 5 us delay required.
    ***************************************************************************/
    CY_FLASH_SPC_FM_EE_WAKE_CNT_REG = CY_FLASH_SPC_FM_EE_WAKE_CNT_80MHZ;


    /***************************************************************************
    * Enable EEPROM. Re-enabling an EEPROM macro takes 5us. During this time,
    * the EE will not acknowledge a PHUB request.
    ***************************************************************************/
    CY_FLASH_PM_ACT_CFG12_REG    |= CY_FLASH_PM_ACT_CFG12_EN_EE;
    CY_FLASH_PM_ALTACT_CFG12_REG |= CY_FLASH_PM_ALTACT_CFG12_EN_EE;

    while(0u == (CY_FLASH_SPC_FM_EE_CR_REG & CY_FLASH_EE_EE_AWAKE))
    {
        /* Non-zero status denotes that the EEPROM/Flash is awake & powered. */
    }

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CyEEPROM_Stop
****************************************************************************//**
*
*  Disable the EEPROM.
*
*******************************************************************************/
void CyEEPROM_Stop (void)
{
    uint8 interruptState;

    interruptState = CyEnterCriticalSection();

    CY_FLASH_PM_ACT_CFG12_REG    &= ((uint8)(~CY_FLASH_PM_ACT_CFG12_EN_EE));
    CY_FLASH_PM_ALTACT_CFG12_REG &= ((uint8)(~CY_FLASH_PM_ALTACT_CFG12_EN_EE));

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CyEEPROM_ReadReserve
****************************************************************************//**
*
*  Request access to the EEPROM for reading and wait until access is available.
*
*******************************************************************************/
void CyEEPROM_ReadReserve(void)
{
    /* Make request for PHUB to have access */
    CY_FLASH_EE_SCR_REG |= CY_FLASH_EE_SCR_AHB_EE_REQ;

    while (0u == (CY_FLASH_EE_SCR_REG & CY_FLASH_EE_SCR_AHB_EE_ACK))
    {
        /* Wait for acknowledgment from PHUB */
    }
}


/*******************************************************************************
* Function Name: CyEEPROM_ReadRelease
****************************************************************************//**
*
*  Release the read reservation of the EEPROM.
*
*******************************************************************************/
void CyEEPROM_ReadRelease(void)
{
    CY_FLASH_EE_SCR_REG &= (uint8)(~CY_FLASH_EE_SCR_AHB_EE_REQ);
}


/* [] END OF FILE */
