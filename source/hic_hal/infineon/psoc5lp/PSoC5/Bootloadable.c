/****************************************************************************//**
* \file Bootloadable.c
* \version 1.60
*
* \brief
*   Provides an API for the Bootloadable application.
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

#include "Bootloadable.h"

/**
 \defgroup functions_group Functions
 @{
*/

#if (CYDEV_PROJ_TYPE == CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER)
static cystatus Bootloadable_WriteFlashByte(const uint32 address, const uint8 inputValue) CYLARGE \
                               ;
#endif /*(CYDEV_PROJ_TYPE == CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER)*/

/*******************************************************************************
* Function Name: Bootloadable_Load
****************************************************************************//**
*
* \brief
*  Schedules the Bootloader/Launcher to be launched and then performs
*  a software reset to launch it
*
* \return
*  This method will never return. It will load a new application and reset
*  the device.
*
*******************************************************************************/
void Bootloadable_Load(void)
{
    /* Schedule Bootloader to start after reset */
    Bootloadable_SET_RUN_TYPE(Bootloadable_SCHEDULE_BTLDR);

    CySoftwareReset();
}

#if (CYDEV_PROJ_TYPE == CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER)
/*******************************************************************************
* Function Name: Bootloadable_GetActiveApplication
****************************************************************************//**
*
* \brief
*   Gets the application which will be loaded after a next reset event.
*   NOTE Intended for the combination project type ONLY!
*
* \return
*  A number of the current active application set in the metadata section.
*  \n  0 - app#0 is set as active.
*  \n  1 - app#1 is set as active.
*
* \note If neither of the applications is set active, then the API returns 0x02.
*
*******************************************************************************/
uint8 Bootloadable_GetActiveApplication(void) CYSMALL \

{
    uint8 result = Bootloadable_MD_BTLDB_ACTIVE_NONE;

    if (0u != Bootloadable_GET_CODE_DATA( \
              Bootloadable_MD_BTLDB_ACTIVE_OFFSET(Bootloadable_MD_BTLDB_ACTIVE_0)))
    {
        result = Bootloadable_MD_BTLDB_ACTIVE_0;
    }
    else if (0u != Bootloadable_GET_CODE_DATA( \
                   Bootloadable_MD_BTLDB_ACTIVE_OFFSET(Bootloadable_MD_BTLDB_ACTIVE_1)))
    {
        result = Bootloadable_MD_BTLDB_ACTIVE_1;
    }
    else
    {
        /*Do nothing, result is none*/
    }

    return (result);
}

/*******************************************************************************
* Function Name: Bootloadable_SetActiveApplication
****************************************************************************//**
*
* \brief
*   Sets the application which will be loaded after a next reset event.
*
* \details
* Theory:
*   This API sets in the Flash (metadata section) the given active application
*   number.
*
*   NOTE The active application number is not set directly, but the boolean
*   mark instead means that the application is active or not for the relative
*   metadata. Both metadata sections are updated. For example, if the second
*   application is to be set active, then in the metadata section for the first
*   application there will be a "0" written, which means that it is not active, and
*   for the second metadata section there will be a "1" written, which means that it is
*   active.
*
*   NOTE Intended for the combination project type ONLY!
*
* \param appId
*   The active application number to be written to flash (metadata section)
*   NOTE Possible values are:
*   0 - for the first application
*   1 - for the second application.
*   Any other number is considered invalid.
*
* \return
*   A status of writing to flash operation.
*   \n CYRET_SUCCESS - Returned if appId was successfully changed.
*   \n CYRET_BAD_PARAM - Returned if the parameter appID passed to the function has the
*                       same value as the active application ID.
*   \note - The other non-zero value is considered as a failure during writing to flash.
*
*   \note - This API does not update Bootloader_activeApp variable.
*
*******************************************************************************/
cystatus Bootloadable_SetActiveApplication(uint8 appId) CYSMALL \

{
    cystatus result = CYRET_SUCCESS;

    uint8 CYDATA idx;

    /* If invalid application number */
    if (appId > Bootloadable_MD_BTLDB_ACTIVE_1)
    {
        result = CYRET_BAD_PARAM;
    }
    else
    {
        /* If appID has same value as active application ID */
        if (1u == Bootloadable_GET_CODE_DATA(Bootloadable_MD_BTLDB_ACTIVE_OFFSET(appId)))
        {
            result = CYRET_BAD_PARAM;
        }
        else
        {
            /* Updating metadata section */
            for(idx = 0u; idx < Bootloadable_MAX_NUM_OF_BTLDB; idx++)
            {
                result |= Bootloadable_WriteFlashByte((uint32) Bootloadable_MD_BTLDB_ACTIVE_OFFSET(idx), \
                                                                                            (uint8)(idx == appId));
            }
        }
    }

    return (result);
}

/*******************************************************************************
* Function Name: Bootloadable_WriteFlashByte
****************************************************************************//**
*
* \brief
*   This API writes to flash the specified data.
*
* \param address
*   The address in flash.
*
* \param inputValue
*   One-byte data.
*
* \return
*   A status of the writing to flash procedure.
*
*******************************************************************************/
static cystatus Bootloadable_WriteFlashByte(const uint32 address, const uint8 inputValue) CYLARGE \

{
    cystatus result = CYRET_SUCCESS;
    uint32 flsAddr = address - CYDEV_FLASH_BASE;
    uint8  rowData[CYDEV_FLS_ROW_SIZE];

    #if !(CY_PSOC4)
        uint8 arrayId = ( uint8 )(flsAddr / CYDEV_FLS_SECTOR_SIZE);
    #endif  /* !(CY_PSOC4) */

    #if (CY_PSOC4)
        uint16 rowNum = ( uint16 )(flsAddr / CYDEV_FLS_ROW_SIZE);
    #else
        uint16 rowNum = ( uint16 )((flsAddr % CYDEV_FLS_SECTOR_SIZE) / CYDEV_FLS_ROW_SIZE);
    #endif  /* (CY_PSOC4) */

    uint32 baseAddr = address - (address % CYDEV_FLS_ROW_SIZE);
    uint16 idx;

    for(idx = 0u; idx < CYDEV_FLS_ROW_SIZE; idx++)
    {
        rowData[idx] = (uint8)Bootloadable_GET_CODE_DATA(baseAddr + idx);
    }

    rowData[address % CYDEV_FLS_ROW_SIZE] = inputValue;

    #if(CY_PSOC4)
        result = CySysFlashWriteRow((uint32) rowNum, rowData);
    #else
        result = CyWriteRowData(arrayId, rowNum, rowData);
    #endif  /* (CY_PSOC4) */

    #if(CY_PSOC5)
        /***************************************************************************
        * When writing to flash, data in the instruction cache can become stale.
        * Therefore, the cache data does not correlate to the data just written to
        * flash. A call to CyFlushCache() is required to invalidate the data in the
        * cache and force fresh information to be loaded from flash.
        ***************************************************************************/
        CyFlushCache();
    #endif /* (CY_PSOC5) */
    return (result);
}
#endif /*(CYDEV_PROJ_TYPE == CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER)*/
/** @} functions_group */

/*******************************************************************************
* The following code is OBSOLETE and must not be used.
*******************************************************************************/
void Bootloadable_SetFlashByte(uint32 address, uint8 runType)
{
    uint32 flsAddr = address - CYDEV_FLASH_BASE;
    uint8  rowData[CYDEV_FLS_ROW_SIZE];

    #if !(CY_PSOC4)
        uint8 arrayId = ( uint8 )(flsAddr / CYDEV_FLS_SECTOR_SIZE);
    #endif  /* !(CY_PSOC4) */

    #if (CY_PSOC4)
        uint16 rowNum = ( uint16 )(flsAddr / CYDEV_FLS_ROW_SIZE);
    #else
        uint16 rowNum = ( uint16 )((flsAddr % CYDEV_FLS_SECTOR_SIZE) / CYDEV_FLS_ROW_SIZE);
    #endif  /* (CY_PSOC4) */

    uint32 baseAddr = address - (address % CYDEV_FLS_ROW_SIZE);
    uint16 idx;


    for (idx = 0u; idx < CYDEV_FLS_ROW_SIZE; idx++)
    {
        rowData[idx] = Bootloadable_GET_CODE_DATA(baseAddr + idx);
    }
    rowData[address % CYDEV_FLS_ROW_SIZE] = runType;

    #if(CY_PSOC4)
        (void) CySysFlashWriteRow((uint32) rowNum, rowData);
    #else
        (void) CyWriteRowData(arrayId, rowNum, rowData);
    #endif  /* (CY_PSOC4) */

    #if(CY_PSOC5)
        /***************************************************************************
        * When writing to flash, data in the instruction cache can become obsolete.
        * Therefore, the cache data does not correlate to the data just written to
        * flash. A call to CyFlushCache() is required to invalidate the data in the
        * cache and force fresh information to be loaded from flash.
        ***************************************************************************/
        CyFlushCache();
    #endif /* (CY_PSOC5) */
}

/* [] END OF FILE */
