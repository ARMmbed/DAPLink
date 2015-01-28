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
status_t flash_security_bypass(flash_driver_t * driver, const uint8_t * backdoorKey)
{
    if ((driver == NULL) ||
        (backdoorKey == NULL))
    {
        return kStatus_InvalidArgument;
    }

    uint8_t registerValue;    // registerValue
    uint32_t returnCode;      // return code variable

    // set the default return code as kStatus_Success
    returnCode = kStatus_Success;

    //Get flash security register value
    registerValue = FTFx->FSEC;

    // Check to see if flash is in secure state (any state other than 0x2)
    // If not, then skip this since flash is not secure
    if (0x02 != (registerValue & 0x03))
    {
        // preparing passing parameter to erase a flash block
        HW_FTFx_FCCOBx_WR(FTFx_BASE, 0, FTFx_SECURITY_BY_PASS);
        HW_FTFx_FCCOBx_WR(FTFx_BASE, 4, backdoorKey[0]);
        HW_FTFx_FCCOBx_WR(FTFx_BASE, 5, backdoorKey[1]);
        HW_FTFx_FCCOBx_WR(FTFx_BASE, 6, backdoorKey[2]);
        HW_FTFx_FCCOBx_WR(FTFx_BASE, 7, backdoorKey[3]);
        HW_FTFx_FCCOBx_WR(FTFx_BASE, 8, backdoorKey[4]);
        HW_FTFx_FCCOBx_WR(FTFx_BASE, 9, backdoorKey[5]);
        HW_FTFx_FCCOBx_WR(FTFx_BASE, A, backdoorKey[6]);
        HW_FTFx_FCCOBx_WR(FTFx_BASE, B, backdoorKey[7]);

        // calling flash command sequence function to execute the command
        returnCode = flash_command_sequence();
    }

    return(returnCode);
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

