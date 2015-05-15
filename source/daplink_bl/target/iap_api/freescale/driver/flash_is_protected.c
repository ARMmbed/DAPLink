/*
 * Copyright (c) 2013, Freescale Semiconductor, Inc.
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
#include "device/fsl_device_registers.h"

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See flash.h for documentation of this function.
status_t flash_is_protected(flash_driver_t * driver, uint32_t start, uint32_t lengthInBytes,
                            flash_protection_state_t * protection_state)
{
    if (protection_state == NULL)
    {
        return kStatus_InvalidArgument;
    }

    uint32_t endAddress;   // end address for protection check
    uint32_t protectionRegionSize;  // size of flash protection region
    uint32_t regionCheckedCounter;  // increments each time the flash address was checked for
                                    // protection status
    uint32_t regionCounter;  // incrementing variable used to increment through the flash
                             // protection regions
    uint32_t protectStatusCounter;  // increments each time a flash region was detected as protected

    uint8_t flashRegionProtectStatus[FSL_FEATURE_FTFx_REGION_COUNT]; // array of the protection status for each
                                                    // protection region
    uint32_t flashRegionAddress[FSL_FEATURE_FTFx_REGION_COUNT+1];  // array of the start addresses for each flash
                                                  // protection region. Note this is REGION_COUNT+1
                                                  // due to requiring the next start address after
                                                  // the end of flash for loop-check purposes below

    // Check the supplied address range.
    status_t returnCode = flash_check_range(driver, start, lengthInBytes, FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE);
    if (returnCode)
    {
        return returnCode;
    }

    // calculating Flash end address
    endAddress = start + lengthInBytes;

    // Calculate the size of the flash protection region
    // If the flash density is > 32KB, then protection region is 1/32 of total flash density
    // Else if flash density is < 32KB, then flash protection region is set to 1KB
    if (driver->PFlashTotalSize > 32*1024)
    {
        protectionRegionSize = (driver->PFlashTotalSize) / FSL_FEATURE_FTFx_REGION_COUNT;
    }
    else
    {
        protectionRegionSize = 1024;
    }

    // populate the flashRegionAddress array with the start address of each flash region
    regionCounter = 0;  // make sure regionCounter is initialized to 0 first

    // populate up to 33rd element of array, this is the next address after end of flash array
    while (regionCounter <= FSL_FEATURE_FTFx_REGION_COUNT)
    {
        flashRegionAddress[regionCounter] = driver->PFlashBlockBase +
                                            protectionRegionSize*regionCounter;
        regionCounter++;
    }

    // populate flashRegionProtectStatus array with status information
    // Protection status for each region is stored in the FPROT[3:0] registers
    // Each bit represents one region of flash
    // 4 registers * 8-bits-per-register = 32-bits (32-regions)
    // The convention is:
    // FPROT3[bit 0] is the first protection region (start of flash memory)
    // FPROT0[bit 7] is the last protection region (end of flash memory)
    // regionCounter is used to determine which FPROT[3:0] register to check for protection status
    // Note: FPROT=1 means NOT protected, FPROT=0 means protected
    regionCounter = 0;  // make sure regionCounter is initialized to 0 first
    while (regionCounter < FSL_FEATURE_FTFx_REGION_COUNT)
    {
        if (regionCounter < 8)
        {
            flashRegionProtectStatus[regionCounter] = ((FTFx->FPROT3) >> regionCounter)&(0x01);
        }
        else if ((regionCounter >= 8)&& (regionCounter < 16))
        {
            flashRegionProtectStatus[regionCounter] = ((FTFx->FPROT2) >> (regionCounter-8))&(0x01);
        }
        else if ((regionCounter >= 16)&& (regionCounter < 24))
        {
            flashRegionProtectStatus[regionCounter] = ((FTFx->FPROT1) >> (regionCounter-16))&(0x01);
        }
        else
        {
            flashRegionProtectStatus[regionCounter] = ((FTFx->FPROT0) >> (regionCounter-24))&(0x01);
        }
        regionCounter++;
    }

    // loop through the flash regions and check
    // desired flash address range for protection status
    // loop stops when it is detected that start has exceeded the endAddress
    regionCounter = 0;  // make sure regionCounter is initialized to 0 first
    regionCheckedCounter = 0;
    protectStatusCounter = 0; // make sure protectStatusCounter is initialized to 0 first
    while (start < endAddress)
    {
        // check to see if the address falls within this protection region
        // Note that if the entire flash is to be checked, the last protection
        // region checked would consist of the last protection start address and
        // the start address following the end of flash
        if ((start >= flashRegionAddress[regionCounter]) &&
            (start < flashRegionAddress[regionCounter+1]))
        {
            // increment regionCheckedCounter to indicate this region was checked
            regionCheckedCounter++;

            // check the protection status of this region
            // Note: FPROT=1 means NOT protected, FPROT=0 means protected
            if (!flashRegionProtectStatus[regionCounter])
            {
                // increment protectStatusCounter to indicate this region is protected
                protectStatusCounter++;
            }
            start += protectionRegionSize;  // increment to an address within the next region
        }
        regionCounter++;  // increment regionCounter to check for the next flash protection region
    }

    // if protectStatusCounter == 0, then no region of the desired flash region is protected
    if (protectStatusCounter == 0)
    {
        *protection_state  = kFlashProtection_Unprotected;
    }
    // if protectStatusCounter == regionCheckedCounter, then each region checked was protected
    else if (protectStatusCounter == regionCheckedCounter)
    {
        *protection_state  = kFlashProtection_Protected;
    }
    // if protectStatusCounter != regionCheckedCounter, then protection status is mixed
    // In other words, some regions are protected while others are unprotected
    else
    {
        *protection_state  = kFlashProtection_Mixed;
    }

  return(returnCode);
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

