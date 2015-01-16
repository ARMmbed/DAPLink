/*
 * Copyright (c) 2013-2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "SSD_FTFx_Common.h"
#include "flash/flash.h"
#include "fsl_platform_status.h"
#include "fsl_platform_types.h"

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See flash.h for documentation of this function.
status_t flash_erase(flash_driver_t * driver, uint32_t start, uint32_t lengthInBytes, uint32_t key)
{
    // Validate the user key
    status_t returnCode = flash_check_user_key(key);
    if (returnCode)
    {
        return returnCode;
    }

    // Check the supplied address range.
    returnCode = flash_check_range(driver, start, lengthInBytes, FSL_FEATURE_FLASH_PFLASH_SECTOR_CMD_ADDRESS_ALIGMENT);
    if (returnCode)
    {
        return returnCode;
    }

    uint32_t endAddress;      // storing end address
    uint32_t numberOfSectors;  // number of sectors calculated by endAddress

    // calculating Flash end address
    endAddress = start + lengthInBytes - 1;

    // re-calculate the endAddress and align it to the start of the next sector
    // which will be used in the comparison below
    if (endAddress % driver->PFlashSectorSize)
    {
        numberOfSectors = endAddress / driver->PFlashSectorSize + 1;
        endAddress = numberOfSectors * driver->PFlashSectorSize - 1;
    }

    // the start address will increment to the next sector address
    // until it reaches the endAdddress
    while (start <= endAddress)
    {
        // preparing passing parameter to erase a flash block
        kFCCOBx[0] = start;
        HW_FTFx_FCCOBx_WR(FTFx_BASE, 0, FTFx_ERASE_SECTOR);

        // calling flash command sequence function to execute the command
        returnCode = flash_command_sequence();

        // calling flash callback function if it is available
        if (driver->PFlashCallback)
        {
            driver->PFlashCallback();
        }

        // checking the success of command execution
        if (kStatus_Success != returnCode)
        {
            break;
        }
        else
        {
            // Increment to the next sector
            start += driver->PFlashSectorSize;
        }
    }

    return(returnCode);
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

