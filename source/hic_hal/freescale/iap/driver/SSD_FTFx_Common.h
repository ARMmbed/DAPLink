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

#ifndef _SSD_FTFx_COMMON_H_
#define _SSD_FTFx_COMMON_H_

#include "flash.h"
//#include "fsl_flash_features.h"
#include "stdint.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @name Flash Program Once Field defines
//@{
#define FLASH_PROGRAM_ONCE_MAX_ID_4BYTES    0x0F   //!< Maximum Index indcating one of Progam Once Fields which is accessed in 4-byte records
#define FLASH_PROGRAM_ONCE_UNIT_4BYTES         4
#define FLASH_PROGRAM_ONCE_MAX_ID_8BYTES    0x13   //!< Maximum Index indcating one of Progam Once Fields which is accessed in 8-byte records
#define FLASH_PROGRAM_ONCE_UNIT_8BYTES         8
//@}


//! @name Flash security status defines
//@{
#define FLASH_SECURITY_STATE_KEYEN         0x80
#define FLASH_SECURITY_STATE_UNSECURED     0x02
#define FLASH_NOT_SECURE                   0x01
#define FLASH_SECURE_BACKDOOR_ENABLED      0x02
#define FLASH_SECURE_BACKDOOR_DISABLED     0x04
//@}

//! @name Flash controller command numbers
//@{
#define FTFx_VERIFY_BLOCK                  0x00 //!< RD1BLK
#define FTFx_VERIFY_SECTION                0x01 //!< RD1SEC
#define FTFx_PROGRAM_CHECK                 0x02 //!< PGMCHK
#define FTFx_READ_RESOURCE                 0x03 //!< RDRSRC
#define FTFx_PROGRAM_LONGWORD              0x06 //!< PGM4
#define FTFx_PROGRAM_PHRASE                0x07 //!< PGM8
#define FTFx_ERASE_BLOCK                   0x08 //!< ERSBLK
#define FTFx_ERASE_SECTOR                  0x09 //!< ERSSCR
#define FTFx_PROGRAM_SECTION               0x0B //!< PGMSEC
#define FTFx_VERIFY_ALL_BLOCK              0x40 //!< RD1ALL
#define FTFx_READ_ONCE                     0x41 //!< RDONCE
#define FTFx_PROGRAM_ONCE                  0x43 //!< PGMONCE
#define FTFx_ERASE_ALL_BLOCK               0x44 //!< ERSALL
#define FTFx_SECURITY_BY_PASS              0x45 //!< VFYKEY
#define FTFx_ERASE_ALL_BLOCK_UNSECURE      0x49 //!< ERSALLU
//@}

//! @brief Flash block base address
#define FLASH_BLOCK_BASE                   0x00

#if defined(FTFA)
    #define FTFx                           FTFA
    #define FTFx_BASE                      FTFA_BASE

    #define FTFx_FSTAT_RD(x)               FTFA_FSTAT_REG(x)
    #define FTFx_FSTAT_WR(x, v)            FTFA_FSTAT_REG(x) = (v)

    #define FTFx_FSTAT_CCIF_MASK           FTFA_FSTAT_CCIF_MASK
    #define FTFx_FSTAT_RDCOLERR_MASK       FTFA_FSTAT_RDCOLERR_MASK
    #define FTFx_FSTAT_ACCERR_MASK         FTFA_FSTAT_ACCERR_MASK
    #define FTFx_FSTAT_FPVIOL_MASK         FTFA_FSTAT_FPVIOL_MASK
    #define FTFx_FSTAT_MGSTAT0_MASK        FTFA_FSTAT_MGSTAT0_MASK

    //#define FTFx_FCCOBx_WR(x, n, v)        FTFA_WR_FCCOB##n(x, v)
    #define FTFx_FCCOBx_WR(x, n, v)        FTFA_FCCOB##n##_REG(x) = (v)

    #define FTFx_FSEC_SEC_MASK             FTFA_FSEC_SEC_MASK
    #define FTFx_FSEC_KEYEN_MASK           FTFA_FSEC_KEYEN_MASK

#elif defined(FTFE)
    #define FTFx                           FTFE
    #define FTFx_BASE                      FTFE_BASE

    #define FTFx_FSTAT_RD(x)               FTFE_FSTAT_REG(x)
    #define FTFx_FSTAT_WR(x, v)            FTFE_FSTAT_REG(x) = (v)

    #define FTFx_FSTAT_CCIF_MASK           FTFE_FSTAT_CCIF_MASK
    #define FTFx_FSTAT_RDCOLERR_MASK       FTFE_FSTAT_RDCOLERR_MASK
    #define FTFx_FSTAT_ACCERR_MASK         FTFE_FSTAT_ACCERR_MASK
    #define FTFx_FSTAT_FPVIOL_MASK         FTFE_FSTAT_FPVIOL_MASK
    #define FTFx_FSTAT_MGSTAT0_MASK        FTFE_FSTAT_MGSTAT0_MASK

    //#define FTFx_FCCOBx_WR(x, n, v)        FTFE_WR_FCCOB##n(x, v)
    #define FTFx_FCCOBx_WR(x, n, v)        FTFE_FCCOB##n##_REG(x) = (v)

    #define FTFx_FSEC_SEC_MASK             FTFE_FSEC_SEC_MASK
    #define FTFx_FSEC_KEYEN_MASK           FTFE_FSEC_KEYEN_MASK

#elif defined(FTFL)
    #define FTFx                           FTFL
    #define FTFx_BASE                      FTFL_BASE

    #define FTFx_FSTAT_RD(x)               FTFL_FSTAT_REG(x)
    #define FTFx_FSTAT_WR(x, v)            FTFL_FSTAT_REG(x) = (v)

    #define FTFx_FSTAT_CCIF_MASK           FTFL_FSTAT_CCIF_MASK
    #define FTFx_FSTAT_RDCOLERR_MASK       FTFL_FSTAT_RDCOLERR_MASK
    #define FTFx_FSTAT_ACCERR_MASK         FTFL_FSTAT_ACCERR_MASK
    #define FTFx_FSTAT_FPVIOL_MASK         FTFL_FSTAT_FPVIOL_MASK
    #define FTFx_FSTAT_MGSTAT0_MASK        FTFL_FSTAT_MGSTAT0_MASK

    //#define FTFx_FCCOBx_WR(x, n, v)        FTFL_WR_FCCOB##n(x, v)
    #define FTFx_FCCOBx_WR(x, n, v)        FTFL_FCCOB##n##_REG(x) = (v)

    #define FTFx_FSEC_SEC_MASK             FTFL_FSEC_SEC_MASK
    #define FTFx_FSEC_KEYEN_MASK           FTFL_FSEC_KEYEN_MASK

#else
    #error "Unknown flash controller"
#endif

////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////

//! @brief Word-aligned access to FCCOB registers.
//!
//! The FCCOB registers are arranged in an order such that writing little-endian
//! words comes out correctly. For instance, FCCOB3 is the LSB of word 0.
//!   - (Word 0) 0:FCCOB3, 1:FCCOB2, 2:FCCOB1, 3:FCCOB0
//!   - (Word 1) 4:FCCOB7, 5:FCCOB6, 6:FCCOB5, 7:FCCOB4
//!   - (Word 2) 8:FCCOBB, 9:FCCOBA, 10:FCCOB9, 11:FCCOB8
extern volatile uint32_t * const restrict kFCCOBx;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

// Internal function Flash command sequence. Called by driver APIs only
status_t flash_command_sequence(void);

//! @brief Validates the range and alignment of the given address range.
status_t flash_check_range(flash_driver_t * driver, uint32_t startAddress, uint32_t lengthInBytes, uint32_t alignmentBaseline);

//! @brief Validates the given user key for flash erase APIs.
status_t flash_check_user_key(uint32_t key);

//! @brief Validates the range and alignment of the given address range.
status_t flash_check_access_ifr_range(flash_driver_t * driver, uint32_t index, uint32_t lengthInBytes);
#endif /* _SSD_FTFx_COMMON_H_ */
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

