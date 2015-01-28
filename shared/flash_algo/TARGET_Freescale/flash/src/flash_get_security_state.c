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
status_t flash_get_security_state(flash_driver_t * driver, flash_security_state_t * state)
{
    if ((driver == NULL) ||
        (state == NULL))
    {
        return kStatus_InvalidArgument;
    }

    // store data read from flash register
    uint8_t registerValue;

    // Get flash security register value
    registerValue = FTFx->FSEC;

    // check the status of the flash security bits in the security register
    if (FLASH_SECURITY_STATE_UNSECURED == (registerValue & BM_FTFx_FSEC_SEC))
    {
        // Flash in unsecured state
        *state = kFlashNotSecure;
    }
    else
    {
        // Flash in secured state
        // check for backdoor key security enable bit
        if (FLASH_SECURITY_STATE_KEYEN == (registerValue & BM_FTFx_FSEC_KEYEN))
        {
            // Backdoor key security enabled
            *state = kFlashSecureBackdoorEnabled;
        }
        else
        {
            // Backdoor key security disabled
            *state = kFlashSecureBackdoorDisabled;
        }
    }

    return(kStatus_Success);
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

