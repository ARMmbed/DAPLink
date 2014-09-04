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

#if !defined(_FLASH_FEATURES_H_)
#define _FLASH_FEATURES_H_
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#if (defined(CPU_MK20DX128VMP5) || defined(CPU_MK20DN128VMP5) || defined(CPU_MK20DX64VMP5) || \
    defined(CPU_MK20DN64VMP5) || defined(CPU_MK20DX32VMP5) || defined(CPU_MK20DN32VMP5) || \
    defined(CPU_MK20DX128VLH5) || defined(CPU_MK20DN128VLH5) || defined(CPU_MK20DX64VLH5) || \
    defined(CPU_MK20DN64VLH5) || defined(CPU_MK20DX32VLH5) || defined(CPU_MK20DN32VLH5) || \
    defined(CPU_MK20DX128VFM5) || defined(CPU_MK20DN128VFM5) || defined(CPU_MK20DX64VFM5) || \
    defined(CPU_MK20DN64VFM5) || defined(CPU_MK20DX32VFM5) || defined(CPU_MK20DN32VFM5) || \
    defined(CPU_MK20DX128VFT5) || defined(CPU_MK20DN128VFT5) || defined(CPU_MK20DX64VFT5) || \
    defined(CPU_MK20DN64VFT5) || defined(CPU_MK20DX32VFT5) || defined(CPU_MK20DN32VFT5) || \
    defined(CPU_MK20DX128VLF5) || defined(CPU_MK20DN128VLF5) || defined(CPU_MK20DX64VLF5) || \
    defined(CPU_MK20DN64VLF5) || defined(CPU_MK20DX32VLF5) || defined(CPU_MK20DN32VLF5))

    #define FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS (0)
    #define FSL_FEATURE_FTFx_WORD_SIZE       (2)
    #define FSL_FEATURE_FTFx_LONGWORD_SIZE   (4)
    #define FSL_FEATURE_FTFx_PHRASE_SIZE     (4)
    #define FSL_FEATURE_FTFx_REGION_COUNT    (32)
    #define FSL_FEATURE_FTFx_PBLOCK_COUNT    (1)
    #define FSL_FEATURE_FTFx_PSECTOR_SIZE    (1024)

#elif (defined(CPU_MK22FN128VDC10) || defined(CPU_MK22FN128VLL10) || defined(CPU_MK22FN128VMP10) || \
     defined(CPU_MK22FN128VLH10))

    #define FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS (0)
    #define FSL_FEATURE_FTFx_WORD_SIZE       (2)
    #define FSL_FEATURE_FTFx_LONGWORD_SIZE   (4)
    #define FSL_FEATURE_FTFx_PHRASE_SIZE     (8)
    #define FSL_FEATURE_FTFx_REGION_COUNT    (32)
    #define FSL_FEATURE_FTFx_PBLOCK_COUNT    (1)
    #define FSL_FEATURE_FTFx_PSECTOR_SIZE    (2048)

#elif (defined(CPU_MK24FN256VDC12))

    #define FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS (0)
    #define FSL_FEATURE_FTFx_WORD_SIZE       (2)
    #define FSL_FEATURE_FTFx_LONGWORD_SIZE   (4)
    #define FSL_FEATURE_FTFx_PHRASE_SIZE     (4)
    #define FSL_FEATURE_FTFx_REGION_COUNT    (32)
    #define FSL_FEATURE_FTFx_PBLOCK_COUNT    (1)
    #define FSL_FEATURE_FTFx_PSECTOR_SIZE    (4096)

#elif (defined(CPU_MK22FN512VDC12) || defined(CPU_MK22FN512VLH12) || defined(CPU_MK22FN512VLL12))

    #define FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS (0)
    #define FSL_FEATURE_FTFx_WORD_SIZE       (2)
    #define FSL_FEATURE_FTFx_LONGWORD_SIZE   (4)
    #define FSL_FEATURE_FTFx_PHRASE_SIZE     (8)
    #define FSL_FEATURE_FTFx_REGION_COUNT    (32)
    #define FSL_FEATURE_FTFx_PBLOCK_COUNT    (1)
    #define FSL_FEATURE_FTFx_PSECTOR_SIZE    (2048)

#elif (defined(CPU_MK64FN1M0VMD12))

    #define FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS (0)
    #define FSL_FEATURE_FTFx_WORD_SIZE       (2)
    #define FSL_FEATURE_FTFx_LONGWORD_SIZE   (4)
    #define FSL_FEATURE_FTFx_PHRASE_SIZE     (8)
    #define FSL_FEATURE_FTFx_REGION_COUNT    (32)
    #define FSL_FEATURE_FTFx_PBLOCK_COUNT    (2)
    #define FSL_FEATURE_FTFx_PSECTOR_SIZE    (4096)

#elif (defined(CPU_MK64FX512VMD12))

    #define FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS (0)
    #define FSL_FEATURE_FTFx_WORD_SIZE       (2)
    #define FSL_FEATURE_FTFx_LONGWORD_SIZE   (4)
    #define FSL_FEATURE_FTFx_PHRASE_SIZE     (8)
    #define FSL_FEATURE_FTFx_REGION_COUNT    (32)
    #define FSL_FEATURE_FTFx_PBLOCK_COUNT    (1)
    #define FSL_FEATURE_FTFx_PSECTOR_SIZE    (2048)

#elif (defined(CPU_MKL25Z32VFM4) || defined(CPU_MKL25Z64VFM4) || defined(CPU_MKL25Z128VFM4) || \
       defined(CPU_MKL25Z32VFT4) || defined(CPU_MKL25Z64VFT4) || defined(CPU_MKL25Z128VFT4) || \
       defined(CPU_MKL25Z32VLH4) || defined(CPU_MKL25Z64VLH4) || defined(CPU_MKL25Z128VLH4) || \
       defined(CPU_MKL25Z32VLK4) || defined(CPU_MKL25Z64VLK4) || defined(CPU_MKL25Z128VLK4))

    #define FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS (1)
    #define FSL_FEATURE_FTFx_WORD_SIZE       (2)
    #define FSL_FEATURE_FTFx_LONGWORD_SIZE   (4)
    #define FSL_FEATURE_FTFx_PHRASE_SIZE     (4)
    #define FSL_FEATURE_FTFx_REGION_COUNT    (32)
    #define FSL_FEATURE_FTFx_PBLOCK_COUNT    (1)
    #define FSL_FEATURE_FTFx_PSECTOR_SIZE    (1024)

#elif (defined(CPU_MKL26Z32VFM4) || defined(CPU_MKL26Z64VFM4) || defined(CPU_MKL26Z128VFM4) || \
       defined(CPU_MKL26Z32VFT4) || defined(CPU_MKL26Z64VFT4) || defined(CPU_MKL26Z128VFT4) || \
       defined(CPU_MKL26Z32VLH4) || defined(CPU_MKL26Z64VLH4) || defined(CPU_MKL26Z128VLH4) || \
       defined(CPU_MKL26Z256VLH4) || defined(CPU_MKL26Z256VMP4) || defined(CPU_MKL26Z128VLL4) || \
       defined(CPU_MKL26Z256VLL4) || defined(CPU_MKL26Z128VMC4) || defined(CPU_MKL26Z256VMC4))

    #define FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS (1)
    #define FSL_FEATURE_FTFx_WORD_SIZE       (2)
    #define FSL_FEATURE_FTFx_LONGWORD_SIZE   (4)
    #define FSL_FEATURE_FTFx_PHRASE_SIZE     (4)
    #define FSL_FEATURE_FTFx_REGION_COUNT    (32)
    #define FSL_FEATURE_FTFx_PBLOCK_COUNT    (1)
    #define FSL_FEATURE_FTFx_PSECTOR_SIZE    (1024)

#elif (defined(CPU_MKL02Z32VFM4) || defined(CPU_MKL02Z16VFM4) || defined(CPU_MKL02Z16VFK4) || \
       defined(CPU_MKL02Z32VFK4) || defined(CPU_MKL02Z8VFG4) || defined(CPU_MKL02Z16VFG4) || \
       defined(CPU_MKL02Z32VFG4) || defined(CPU_MKL02Z32CAF4))

    #define FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS (1)
    #define FSL_FEATURE_FTFx_WORD_SIZE       (2)
    #define FSL_FEATURE_FTFx_LONGWORD_SIZE   (4)
    #define FSL_FEATURE_FTFx_PHRASE_SIZE     (4)
    #define FSL_FEATURE_FTFx_REGION_COUNT    (32)
    #define FSL_FEATURE_FTFx_PBLOCK_COUNT    (1)
    #define FSL_FEATURE_FTFx_PSECTOR_SIZE    (1024)

#elif (defined(CPU_MK70FN1M0VMJ12))

    #define FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS (0)
    #define FSL_FEATURE_FTFx_WORD_SIZE       (2)
    #define FSL_FEATURE_FTFx_LONGWORD_SIZE   (4)
    #define FSL_FEATURE_FTFx_PHRASE_SIZE     (8)
    #define FSL_FEATURE_FTFx_REGION_COUNT    (32)
    #define FSL_FEATURE_FTFx_PBLOCK_COUNT    (4)
    #define FSL_FEATURE_FTFx_PSECTOR_SIZE    (4096)

#elif (defined(CPU_MK70FX512VMJ12))

    #define FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS (0)
    #define FSL_FEATURE_FTFx_WORD_SIZE       (2)
    #define FSL_FEATURE_FTFx_LONGWORD_SIZE   (4)
    #define FSL_FEATURE_FTFx_PHRASE_SIZE     (8)
    #define FSL_FEATURE_FTFx_REGION_COUNT    (32)
    #define FSL_FEATURE_FTFx_PBLOCK_COUNT    (2)
    #define FSL_FEATURE_FTFx_PSECTOR_SIZE    (4096)

#elif (defined(CPU_MKL05Z8VFK4) || defined(CPU_MKL05Z16VFK4) || defined(CPU_MKL05Z32VFK4) || \
    defined(CPU_MKL05Z8VLC4) || defined(CPU_MKL05Z16VLC4) || defined(CPU_MKL05Z32VLC4) || \
    defined(CPU_MKL05Z8VFM4) || defined(CPU_MKL05Z16VFM4) || defined(CPU_MKL05Z32VFM4) || \
    defined(CPU_MKL05Z16VLF4) || defined(CPU_MKL05Z32VLF4))

    #define FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS (1)
    #define FSL_FEATURE_FTFx_WORD_SIZE       (2)
    #define FSL_FEATURE_FTFx_LONGWORD_SIZE   (4)
    #define FSL_FEATURE_FTFx_PHRASE_SIZE     (4)
    #define FSL_FEATURE_FTFx_REGION_COUNT    (32)
    #define FSL_FEATURE_FTFx_PBLOCK_COUNT    (1)
    #define FSL_FEATURE_FTFx_PSECTOR_SIZE    (1024)

#elif (defined(CPU_MKL46Z128VLH4) || defined(CPU_MKL46Z256VLH4) || defined(CPU_MKL46Z128VLL4) || \
    defined(CPU_MKL46Z256VLL4) || defined(CPU_MKL46Z128VMC4) || defined(CPU_MKL46Z256VMC4))

    #define FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS (1)
    #define FSL_FEATURE_FTFx_WORD_SIZE       (2)
    #define FSL_FEATURE_FTFx_LONGWORD_SIZE   (4)
    #define FSL_FEATURE_FTFx_PHRASE_SIZE     (4)
    #define FSL_FEATURE_FTFx_REGION_COUNT    (32)
    #define FSL_FEATURE_FTFx_PBLOCK_COUNT    (2)
    #define FSL_FEATURE_FTFx_PSECTOR_SIZE    (1024)
#elif defined(CPU_MK24FN1M0VDC12) || defined(CPU_MK24FN1M0VLQ12) || defined(CPU_MK63FN1M0VLQ12) || defined(CPU_MK63FN1M0VMD12)

    #define FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS (1)
    #define FSL_FEATURE_FTFx_WORD_SIZE       (2)
    #define FSL_FEATURE_FTFx_LONGWORD_SIZE   (4)
    #define FSL_FEATURE_FTFx_PHRASE_SIZE     (4)
    #define FSL_FEATURE_FTFx_REGION_COUNT    (32)
    #define FSL_FEATURE_FTFx_PBLOCK_COUNT    (2)
    #define FSL_FEATURE_FTFx_PSECTOR_SIZE    (4096)

#else
    #error "No valid CPU defined"
#endif


#endif // _FLASH_FEATURES_H_
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

