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

#include "assert.h"
#include "SSD_FTFx_Common.h"
#include "flash.h"
#include "fsl_device_registers.h"
#include "fsl_platform_status.h"
#include "string.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! @brief Flash Command Sequence
//!
//! This function is used to perform the command write sequence to the flash.
//!
//! @param index Maximum index value of the command array.
//! @param pCommandArray Pointer to the array which contains the data to be
//!        written to the FCCOB registers.
//!
//! @return An error code or kStatus_Success
//!
////////////////////////////////////////////////////////////////////////////////
#if BL_TARGET_FLASH
__attribute__((section("ram_func")))
#endif
status_t flash_command_sequence(void)
{
    // clear RDCOLERR & ACCERR & FPVIOL flag in flash status register
    FTFx_FSTAT_WR(FTFx, FTFx_FSTAT_RDCOLERR_MASK | FTFx_FSTAT_ACCERR_MASK | FTFx_FSTAT_FPVIOL_MASK);

    // clear CCIF bit
    FTFx_FSTAT_WR(FTFx, FTFx_FSTAT_CCIF_MASK);

    // check CCIF bit of the flash status register, wait till it is set
    while (!(FTFx_FSTAT_RD(FTFx) & FTFx_FSTAT_CCIF_MASK));

    // Check error bits
    // Get flash status register value
    uint8_t registerValue = FTFx->FSTAT;

    // checking access error
    if (registerValue & FTFx_FSTAT_ACCERR_MASK)
    {
        return kStatus_FlashAccessError;
    }
    // checking protection error
    else if (registerValue & FTFx_FSTAT_FPVIOL_MASK)
    {
        return kStatus_FlashProtectionViolation;
    }
    // checking MGSTAT0 non-correctable error
    else if (registerValue & FTFx_FSTAT_MGSTAT0_MASK)
    {
        return kStatus_FlashCommandFailure;
    }

    return kStatus_Success;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

