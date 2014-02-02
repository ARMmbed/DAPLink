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
#include "device/fsl_device_registers.h"
#include "fsl_platform_status.h"
#include <string.h>

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
status_t flash_command_sequence(void)
{
    // clear RDCOLERR & ACCERR & FPVIOL flag in flash status register
    HW_FTFx_FSTAT_WR(BM_FTFx_FSTAT_RDCOLERR | BM_FTFx_FSTAT_ACCERR | BM_FTFx_FSTAT_FPVIOL);
    
    __disable_irq();

    // clear CCIF bit
    HW_FTFx_FSTAT_WR(BM_FTFx_FSTAT_CCIF);

    // check CCIF bit of the flash status register, wait till it is set
    while (!(HW_FTFx_FSTAT.B.CCIF));
    
    __enable_irq();

    // Check error bits
    // Get flash status register value
    uint8_t registerValue = FTFx->FSTAT;

    // checking access error
    if (registerValue & BM_FTFx_FSTAT_ACCERR)
    {
        return kStatus_FlashAccessError;
    }
    // checking protection error
    else if (registerValue & BM_FTFx_FSTAT_FPVIOL)
    {
        return kStatus_FlashProtectionViolation;
    }
    // checking MGSTAT0 non-correctable error
    else if (registerValue & BM_FTFx_FSTAT_MGSTAT0)
    {
        return kStatus_FlashCommandFailure;
    }

    return kStatus_Success;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

