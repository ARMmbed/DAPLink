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
status_t flash_get_property(flash_driver_t * driver, flash_property_t whichProperty, uint32_t * value)
{
    if ((driver == NULL) ||
        (value == NULL))
    {
        return kStatus_InvalidArgument;
    }

    switch (whichProperty)
    {
        case kFlashProperty_SectorSize:
           *value = driver->PFlashSectorSize;
           break;

        case kFlashProperty_TotalFlashSize:
           *value = driver->PFlashTotalSize;
           break;

        case kFlashProperty_BlockSize:
           *value = driver->PFlashTotalSize;
           break;

        case kFlashProperty_BlockCount:
           *value = driver->PFlashBlockCount;
           break;

        case kFlashProperty_FlashBlockBaseAddr:
           *value = driver->PFlashBlockBase;
           break;

        case kFlashProperty_FlashFacSupport:
#if defined (FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL)
            *value = FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL;
#else
            *value = 0;
#endif // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
            break;

        case kFlashProperty_FlashAccessSegmentSize:
            *value = driver->PFlashAccessSegmentSize;
            break;

        case kFlashProperty_FlashAccessSegmentCount:
            *value = driver->PFlashAccessSegmentCount;
            break;

        default:  // catch inputs that are not recognized
            return kStatus_FlashUnknownProperty;
    }

    return kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

