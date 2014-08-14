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
// Declarations
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See flash.h for documentation of this function.
status_t flash_program(flash_driver_t * driver, uint32_t start, uint32_t * src, uint32_t lengthInBytes)
{
    if (src == NULL)
    {
        return kStatus_InvalidArgument;
    }

    // Check the supplied address range.
    status_t returnCode = flash_check_range(driver, start, lengthInBytes, FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE);
    if (returnCode)
    {
        return returnCode;
    }

    while (lengthInBytes > 0)
    {
        // preparing passing parameter to program the flash block
        kFCCOBx[0] = start;
        kFCCOBx[1] = *src++;
#if (FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE == 4)
        HW_FTFx_FCCOBx_WR(FTFx_BASE, 0, FTFx_PROGRAM_LONGWORD);
#elif (FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE == 8)
        kFCCOBx[2] = *src++;
        HW_FTFx_FCCOBx_WR(FTFx_BASE, 0, FTFx_PROGRAM_PHRASE);
#else
        #error "Untreated program unit size"
#endif

        // calling flash command sequence function to execute the command
        returnCode = flash_command_sequence();

        // calling flash callback function if it is available
        if (driver->PFlashCallback)
        {
            driver->PFlashCallback();
        }

        // checking for the success of command execution
        if (kStatus_Success != returnCode)
        {
            break;
        }
        else
        {
            // update start address for next iteration
            start += FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE;

            // update lengthInBytes for next iteration
            lengthInBytes -= FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE;
        }
    }

    return(returnCode);
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

