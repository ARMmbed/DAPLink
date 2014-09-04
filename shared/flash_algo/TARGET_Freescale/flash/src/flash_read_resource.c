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
#include "device/fsl_device_registers.h"
#include "fsl_platform_status.h"
#include "flash_densities.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////


enum __flash_read_resource_alignment
{
    kFlashReadResource_AlignSizeInBytes = 4
};

enum _flash_read_resource_unit_size
{
    kFlashReadResource_UnitInBytes = 4
};

//! @brief Enumeration for the maximum size of special-purpose flash resource
enum __flash_read_resource_max_size
{
    kFlashResource_FlashIFRSizeInBytes = 256,
    kFlashResource_VersionIdSizeInBytes = 8,
};


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
status_t flash_check_resource_range(uint32_t start, uint32_t lengthInBytes, flash_read_resource_option_t option);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See flash.h for documentation of this function.
status_t flash_read_resource(flash_driver_t * driver, uint32_t start, uint32_t *dst, uint32_t lengthInBytes, flash_read_resource_option_t option)
{
    if (driver == NULL || dst == NULL)
    {
        return kStatus_InvalidArgument;
    }

    // Check the supplied address range.
    status_t returnCode =  flash_check_resource_range(start, lengthInBytes, option);
    if (returnCode != kStatus_Success)
    {
        return returnCode;
    }

    while(lengthInBytes > 0)
    {
        // preparing passing parameter
        kFCCOBx[0] = start;
        HW_FTFx_FCCOBx_WR(FTFx_BASE, 0, FTFx_READ_RESOURCE);
        HW_FTFx_FCCOBx_WR(FTFx_BASE, 8, option);

        // calling flash command sequence function to execute the command
        returnCode = flash_command_sequence();

        if (kStatus_Success != returnCode)
        {
            break;
        }

         // fetch data
         *dst++ = kFCCOBx[1];

        // update start address for next iteration
        start += kFlashReadResource_UnitInBytes;
        // update lengthInBytes for next iteration
        lengthInBytes -= kFlashReadResource_UnitInBytes;
    }

    return (returnCode);
}


status_t flash_check_resource_range(uint32_t start, uint32_t lengthInBytes, flash_read_resource_option_t option)
{
    if (option > kFlashResource_VersionID)
    {
        return kStatus_InvalidArgument;
    }

    uint32_t maxReadbleAddress = kFlashResource_FlashIFRSizeInBytes;
    if (option == kFlashResource_VersionID)
    {
        maxReadbleAddress = kFlashResource_VersionIdSizeInBytes;
    }

    if ( (start & (kFlashReadResource_AlignSizeInBytes - 1) )
        ||(lengthInBytes & (kFlashReadResource_AlignSizeInBytes - 1)))
    {
        return kStatus_FlashAlignmentError;
    }

    if ( (start + lengthInBytes) > maxReadbleAddress)
    {
        return kStatus_FlashAddressError;
    }

    return kStatus_Success;
}


////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

