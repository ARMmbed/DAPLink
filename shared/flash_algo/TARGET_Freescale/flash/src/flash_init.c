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
#include "fsl_flash_features.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
enum
{
    kFlashAccessSegmentBase = 256UL,
};


////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

volatile uint32_t * const restrict kFCCOBx =
#if FSL_FEATURE_FLASH_IS_FTFA
    (volatile uint32_t *)&FTFA->FCCOB3;
#elif FSL_FEATURE_FLASH_IS_FTFE
    (volatile uint32_t *)&FTFE->FCCOB3;
#elif FSL_FEATURE_FLASH_IS_FTFL
    (volatile uint32_t *)&FTFL->FCCOB3;
#else
    #error "Unknown flash controller"
#endif


////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See flash.h for documentation of this function.
status_t flash_init(flash_driver_t * driver)
{
    if (driver == NULL)
    {
        return kStatus_InvalidArgument;
    }

    // calculate the flash density from SIM_FCFG1.PFSIZE
    uint32_t flashDensity = kFlashDensities[HW_SIM_FCFG1(SIM_BASE).B.PFSIZE] << 12;
    if (flashDensity == 0)
    {
        return kStatus_FlashSizeError;
    }

    // fill out a few of the structure members
    driver->PFlashBlockBase = FLASH_BLOCK_BASE;
    driver->PFlashTotalSize = flashDensity;
    driver->PFlashBlockCount = FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT;
    driver->PFlashSectorSize = FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE;

#if FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    driver->PFlashAccessSegmentSize =  kFlashAccessSegmentBase << FTFx->FACSS;
    driver->PFlashAccessSegmentCount = FTFx->FACSN;
#else
    driver->PFlashAccessSegmentSize =  0;
    driver->PFlashAccessSegmentCount = 0;
#endif // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL

    driver->PFlashCallback = NULL;

    // copy flash_run_command() to RAM
#if BL_TARGET_FLASH
    copy_flash_run_command();
#endif

    return kStatus_Success;
}

// See flash.h for documentation of this function.
status_t flash_register_callback(flash_driver_t * driver, flash_callback_t callback)
{
    if (driver == NULL)
    {
        return kStatus_InvalidArgument;
    }

    driver->PFlashCallback = callback;

    return kStatus_Success;
}



// See SSD_FTFx_Common.h for documentation of this function.
status_t flash_check_range(flash_driver_t * driver, uint32_t start, uint32_t lengthInBytes, uint32_t alignmentBaseline)
{
    if (driver == NULL)
    {
        return kStatus_InvalidArgument;
    }

    // Verify the start and length are alignmentBaseline aligned.
    if ((start & (alignmentBaseline - 1))
        || (lengthInBytes & (alignmentBaseline - 1)))
    {
        return kStatus_FlashAlignmentError;
    }

    // check for valid range of the target addresses
    if ((start < driver->PFlashBlockBase) ||
        ((start+lengthInBytes) > (driver->PFlashBlockBase + driver->PFlashTotalSize)))
    {
        return kStatus_FlashAddressError;
    }

    return kStatus_Success;
}

// See SSD_FTFx_Common.h for documentation of this function.
status_t flash_check_user_key(uint32_t key)
{
    // Validate the user key
    if (key != kFlashEraseKey)
    {
        return kStatus_FlashEraseKeyError;
    }

    return kStatus_Success;
}

// See SSD_FTFx_Common.h for documentation of this function.
status_t flash_check_access_ifr_range(flash_driver_t * driver, uint32_t index, uint32_t lengthInBytes)
{
    if (driver == NULL)
    {
        return kStatus_InvalidArgument;
    }

#if ! FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    if (index > FLASH_PROGRAM_ONCE_MAX_ID_4BYTES || lengthInBytes != FLASH_PROGRAM_ONCE_UNIT_4BYTES)
    {
        return kStatus_InvalidArgument;
    }
#else
    if (index <= FLASH_PROGRAM_ONCE_MAX_ID_4BYTES)
    {
        if (lengthInBytes != FLASH_PROGRAM_ONCE_UNIT_4BYTES)
        {
            return kStatus_InvalidArgument;
        }
    }
    else if (index <= FLASH_PROGRAM_ONCE_MAX_ID_8BYTES)
    {
        if (lengthInBytes != FLASH_PROGRAM_ONCE_UNIT_8BYTES)
        {
            return kStatus_InvalidArgument;
        }
    }
    else
    {
        return kStatus_InvalidArgument;
    }

#endif  // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL

    return kStatus_Success;

}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

