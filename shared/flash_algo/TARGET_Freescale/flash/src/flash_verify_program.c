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
status_t flash_verify_program(flash_driver_t * driver,
                              uint32_t start,
                              uint32_t lengthInBytes,
                              const uint8_t * expectedData,
                              flash_margin_value_t margin,
                              uint32_t * failedAddress,
                              uint8_t * failedData)
{
    if (expectedData == NULL)
    {
        return kStatus_InvalidArgument;
    }

    status_t returnCode = flash_check_range(driver, start, lengthInBytes, FSL_FEATURE_FLASH_PFLASH_CHECK_CMD_ADDRESS_ALIGMENT);
    if (returnCode)
    {
        return returnCode;
    }

    while (lengthInBytes)
    {
        // preparing passing parameter to program check the flash block
        kFCCOBx[0] = start;
        HW_FTFx_FCCOBx_WR(FTFx_BASE, 0, FTFx_PROGRAM_CHECK);
        HW_FTFx_FCCOBx_WR(FTFx_BASE, 4, margin);
        kFCCOBx[2] = *(uint32_t *)expectedData;

        // calling flash command sequence function to execute the command
        returnCode = flash_command_sequence();

        // checking for the success of command execution
        if (kStatus_Success != returnCode)
        {
            if (failedAddress)
            {
                *failedAddress  =  start;
            }
            if (failedData)
            {
                *(uint32_t *)failedData = 0;
            }

        // Read fail returned data: if K70, Nevis2, L1PT, L2K are selected
        //! @todo Use a feature macro to determine whether this is supported.
// #if ((FTFx_KX_512K_512K_16K_4K_4K == FLASH_DERIVATIVE) || (FTFx_KX_1024K_0K_16K_4K_0K == FLASH_DERIVATIVE)\
//         ||(FTFx_NX_256K_32K_2K_2K_1K == FLASH_DERIVATIVE)||(FTFx_NX_128K_32K_2K_2K_1K == FLASH_DERIVATIVE)\
//         ||(FTFx_NX_96K_32K_2K_2K_1K == FLASH_DERIVATIVE)||(FTFx_NX_64K_32K_2K_2K_1K == FLASH_DERIVATIVE)\
//         ||(FTFx_LX_128K_0K_0K_1K_0K == FLASH_DERIVATIVE)||(FTFx_LX_96K_0K_0K_1K_0K == FLASH_DERIVATIVE)\
//         ||(FTFx_LX_64K_0K_0K_1K_0K == FLASH_DERIVATIVE)||(FTFx_LX_32K_0K_0K_1K_0K == FLASH_DERIVATIVE)\
//         ||(FTFx_LX_8K_0K_0K_1K_0K == FLASH_DERIVATIVE))
//                 *failedData     = 0x0;
//                 *(failedData+1) = 0x0;
//                 *(failedData+2) = 0x0;
//                 *(failedData+3) = 0x0;
// #else //other derivative
// #if (ENDIANNESS == BIG_ENDIAN)  // Big Endian
//                 *(failedData)   = FTFA->FCCOB4;
//                 *(failedData+1) = FTFA->FCCOB5;
//                 *(failedData+2) = FTFA->FCCOB6;
//                 *(failedData+3) = FTFA->FCCOB7;
// #else // Little Endian
//                 *(failedData+3) = FTFA->FCCOB4;
//                 *(failedData+2) = FTFA->FCCOB5;
//                 *(failedData+1) = FTFA->FCCOB6;
//                 *(failedData)   = FTFA->FCCOB7;
// #endif //of ENDIANNESS
// #endif // of FLASH_DERIVATIVE

            break;
        }

        lengthInBytes -= FSL_FEATURE_FLASH_PFLASH_CHECK_CMD_ADDRESS_ALIGMENT;
        expectedData += FSL_FEATURE_FLASH_PFLASH_CHECK_CMD_ADDRESS_ALIGMENT;
        start += FSL_FEATURE_FLASH_PFLASH_CHECK_CMD_ADDRESS_ALIGMENT;
    }

    return(returnCode);
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

