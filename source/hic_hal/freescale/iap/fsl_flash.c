/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#include "fsl_flash.h"
#include "compiler.h"
#define USE_RAM_FUNC

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @name Misc utility defines
 * @{
 */
#ifndef ALIGN_DOWN
#define ALIGN_DOWN(x, a) ((x) & (uint32_t)(-((int32_t)(a))))
#endif
#ifndef ALIGN_UP
#define ALIGN_UP(x, a) (-((int32_t)((uint32_t)(-((int32_t)(x))) & (uint32_t)(-((int32_t)(a))))))
#endif

#define BYTES_JOIN_TO_WORD_1_3(x, y) ((((uint32_t)(x)&0xFFU) << 24) | ((uint32_t)(y)&0xFFFFFFU))
#define BYTES_JOIN_TO_WORD_2_2(x, y) ((((uint32_t)(x)&0xFFFFU) << 16) | ((uint32_t)(y)&0xFFFFU))
#define BYTES_JOIN_TO_WORD_3_1(x, y) ((((uint32_t)(x)&0xFFFFFFU) << 8) | ((uint32_t)(y)&0xFFU))
#define BYTES_JOIN_TO_WORD_1_1_2(x, y, z) \
    ((((uint32_t)(x)&0xFFU) << 24) | (((uint32_t)(y)&0xFFU) << 16) | ((uint32_t)(z)&0xFFFFU))
#define BYTES_JOIN_TO_WORD_1_2_1(x, y, z) \
    ((((uint32_t)(x)&0xFFU) << 24) | (((uint32_t)(y)&0xFFFFU) << 8) | ((uint32_t)(z)&0xFFU))
#define BYTES_JOIN_TO_WORD_2_1_1(x, y, z) \
    ((((uint32_t)(x)&0xFFFFU) << 16) | (((uint32_t)(y)&0xFFU) << 8) | ((uint32_t)(z)&0xFFU))
#define BYTES_JOIN_TO_WORD_1_1_1_1(x, y, z, w)                                                      \
    ((((uint32_t)(x)&0xFFU) << 24) | (((uint32_t)(y)&0xFFU) << 16) | (((uint32_t)(z)&0xFFU) << 8) | \
     ((uint32_t)(w)&0xFFU))
/*@}*/

/*! @brief Data flash IFR map Field*/
#if defined(FSL_FEATURE_FLASH_IS_FTFE) && FSL_FEATURE_FLASH_IS_FTFE
#define DFLASH_IFR_READRESOURCE_START_ADDRESS 0x8003F8U
#else /* FSL_FEATURE_FLASH_IS_FTFL == 1 or FSL_FEATURE_FLASH_IS_FTFA = =1 */
#define DFLASH_IFR_READRESOURCE_START_ADDRESS 0x8000F8U
#endif

/*!
 * @name Reserved FlexNVM size (For a variety of purposes) defines
 * @{
 */
#define FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED 0xFFFFFFFFU
#define FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_RESERVED 0xFFFFU
/*@}*/

/*!
 * @name Flash Program Once Field defines
 * @{
 */
#if defined(FSL_FEATURE_FLASH_IS_FTFA) && FSL_FEATURE_FLASH_IS_FTFA
/* FTFA parts(eg. K80, KL80, L5K) support both 4-bytes and 8-bytes unit size */
#define FLASH_PROGRAM_ONCE_MIN_ID_8BYTES \
    0x10U /* Minimum Index indcating one of Progam Once Fields which is accessed in 8-byte records */
#define FLASH_PROGRAM_ONCE_MAX_ID_8BYTES \
    0x13U /* Maximum Index indcating one of Progam Once Fields which is accessed in 8-byte records */
#define FLASH_PROGRAM_ONCE_IS_4BYTES_UNIT_SUPPORT 1
#define FLASH_PROGRAM_ONCE_IS_8BYTES_UNIT_SUPPORT 1
#elif defined(FSL_FEATURE_FLASH_IS_FTFE) && FSL_FEATURE_FLASH_IS_FTFE
/* FTFE parts(eg. K65, KE18) only support 8-bytes unit size */
#define FLASH_PROGRAM_ONCE_IS_4BYTES_UNIT_SUPPORT 0
#define FLASH_PROGRAM_ONCE_IS_8BYTES_UNIT_SUPPORT 1
#elif defined(FSL_FEATURE_FLASH_IS_FTFL) && FSL_FEATURE_FLASH_IS_FTFL
/* FTFL parts(eg. K20) only support 4-bytes unit size */
#define FLASH_PROGRAM_ONCE_IS_4BYTES_UNIT_SUPPORT 1
#define FLASH_PROGRAM_ONCE_IS_8BYTES_UNIT_SUPPORT 0
#endif
/*@}*/

/*!
 * @name Flash security status defines
 * @{
 */
#define FLASH_SECURITY_STATE_KEYEN 0x80U
#define FLASH_SECURITY_STATE_UNSECURED 0x02U
#define FLASH_NOT_SECURE 0x01U
#define FLASH_SECURE_BACKDOOR_ENABLED 0x02U
#define FLASH_SECURE_BACKDOOR_DISABLED 0x04U
/*@}*/

/*!
 * @name Flash controller command numbers
 * @{
 */
#define FTFx_VERIFY_BLOCK 0x00U                    /*!< RD1BLK*/
#define FTFx_VERIFY_SECTION 0x01U                  /*!< RD1SEC*/
#define FTFx_PROGRAM_CHECK 0x02U                   /*!< PGMCHK*/
#define FTFx_READ_RESOURCE 0x03U                   /*!< RDRSRC*/
#define FTFx_PROGRAM_LONGWORD 0x06U                /*!< PGM4*/
#define FTFx_PROGRAM_PHRASE 0x07U                  /*!< PGM8*/
#define FTFx_ERASE_BLOCK 0x08U                     /*!< ERSBLK*/
#define FTFx_ERASE_SECTOR 0x09U                    /*!< ERSSCR*/
#define FTFx_PROGRAM_SECTION 0x0BU                 /*!< PGMSEC*/
#define FTFx_VERIFY_ALL_BLOCK 0x40U                /*!< RD1ALL*/
#define FTFx_READ_ONCE 0x41U                       /*!< RDONCE or RDINDEX*/
#define FTFx_PROGRAM_ONCE 0x43U                    /*!< PGMONCE or PGMINDEX*/
#define FTFx_ERASE_ALL_BLOCK 0x44U                 /*!< ERSALL*/
#define FTFx_SECURITY_BY_PASS 0x45U                /*!< VFYKEY*/
#define FTFx_SWAP_CONTROL 0x46U                    /*!< SWAP*/
#define FTFx_ERASE_ALL_BLOCK_UNSECURE 0x49U        /*!< ERSALLU*/
#define FTFx_VERIFY_ALL_EXECUTE_ONLY_SEGMENT 0x4AU /*!< RD1XA*/
#define FTFx_ERASE_ALL_EXECUTE_ONLY_SEGMENT 0x4BU  /*!< ERSXA*/
#define FTFx_PROGRAM_PARTITION 0x80U               /*!< PGMPART)*/
#define FTFx_SET_FLEXRAM_FUNCTION 0x81U            /*!< SETRAM*/
                                                   /*@}*/

/*!
 * @name Common flash register info defines
 * @{
 */
#if defined(FTFA)
#define FTFx FTFA
#define FTFx_BASE FTFA_BASE
#define FTFx_FSTAT_CCIF_MASK FTFA_FSTAT_CCIF_MASK
#define FTFx_FSTAT_RDCOLERR_MASK FTFA_FSTAT_RDCOLERR_MASK
#define FTFx_FSTAT_ACCERR_MASK FTFA_FSTAT_ACCERR_MASK
#define FTFx_FSTAT_FPVIOL_MASK FTFA_FSTAT_FPVIOL_MASK
#define FTFx_FSTAT_MGSTAT0_MASK FTFA_FSTAT_MGSTAT0_MASK
#define FTFx_FSEC_SEC_MASK FTFA_FSEC_SEC_MASK
#define FTFx_FSEC_KEYEN_MASK FTFA_FSEC_KEYEN_MASK
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_RAM) && FSL_FEATURE_FLASH_HAS_FLEX_RAM
#define FTFx_FCNFG_RAMRDY_MASK FTFA_FCNFG_RAMRDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_RAM */
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM) && FSL_FEATURE_FLASH_HAS_FLEX_NVM
#define FTFx_FCNFG_EEERDY_MASK FTFA_FCNFG_EEERDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_NVM */
#elif defined(FTFE)
#define FTFx FTFE
#define FTFx_BASE FTFE_BASE
#define FTFx_FSTAT_CCIF_MASK FTFE_FSTAT_CCIF_MASK
#define FTFx_FSTAT_RDCOLERR_MASK FTFE_FSTAT_RDCOLERR_MASK
#define FTFx_FSTAT_ACCERR_MASK FTFE_FSTAT_ACCERR_MASK
#define FTFx_FSTAT_FPVIOL_MASK FTFE_FSTAT_FPVIOL_MASK
#define FTFx_FSTAT_MGSTAT0_MASK FTFE_FSTAT_MGSTAT0_MASK
#define FTFx_FSEC_SEC_MASK FTFE_FSEC_SEC_MASK
#define FTFx_FSEC_KEYEN_MASK FTFE_FSEC_KEYEN_MASK
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_RAM) && FSL_FEATURE_FLASH_HAS_FLEX_RAM
#define FTFx_FCNFG_RAMRDY_MASK FTFE_FCNFG_RAMRDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_RAM */
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM) && FSL_FEATURE_FLASH_HAS_FLEX_NVM
#define FTFx_FCNFG_EEERDY_MASK FTFE_FCNFG_EEERDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_NVM */
#elif defined(FTFL)
#define FTFx FTFL
#define FTFx_BASE FTFL_BASE
#define FTFx_FSTAT_CCIF_MASK FTFL_FSTAT_CCIF_MASK
#define FTFx_FSTAT_RDCOLERR_MASK FTFL_FSTAT_RDCOLERR_MASK
#define FTFx_FSTAT_ACCERR_MASK FTFL_FSTAT_ACCERR_MASK
#define FTFx_FSTAT_FPVIOL_MASK FTFL_FSTAT_FPVIOL_MASK
#define FTFx_FSTAT_MGSTAT0_MASK FTFL_FSTAT_MGSTAT0_MASK
#define FTFx_FSEC_SEC_MASK FTFL_FSEC_SEC_MASK
#define FTFx_FSEC_KEYEN_MASK FTFL_FSEC_KEYEN_MASK
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_RAM) && FSL_FEATURE_FLASH_HAS_FLEX_RAM
#define FTFx_FCNFG_RAMRDY_MASK FTFL_FCNFG_RAMRDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_RAM */
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM) && FSL_FEATURE_FLASH_HAS_FLEX_NVM
#define FTFx_FCNFG_EEERDY_MASK FTFL_FCNFG_EEERDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_NVM */
#else
#error "Unknown flash controller"
#endif
/*@}*/

/*!
 * @brief Enumeration for access segment property.
 */
enum _flash_access_segment_property
{
    kFLASH_accessSegmentBase = 256UL,
};

/*!
 * @brief Enumeration for acceleration ram property.
 */
enum _flash_acceleration_ram_property
{
    kFLASH_accelerationRamSize = 0x400U
};

/*!
 * @brief Enumeration for flash config area.
 */
enum _flash_config_area_range
{
    kFLASH_configAreaStart = 0x400U,
    kFLASH_configAreaEnd = 0x40FU
};

/*! @brief program Flash block base address*/
#define PFLASH_BLOCK_BASE 0x00U

/*! @brief Total flash region count*/
#define FSL_FEATURE_FTFx_REGION_COUNT (32U)

/*!
 * @name Flash register access type defines
 * @{
 */
#if FLASH_DRIVER_IS_FLASH_RESIDENT
#define FTFx_REG_ACCESS_TYPE volatile uint8_t *
#define FTFx_REG32_ACCESS_TYPE volatile uint32_t *
#endif /* FLASH_DRIVER_IS_FLASH_RESIDENT */
       /*@}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief Internal function Flash command sequence. Called by driver APIs only*/
static status_t flash_command_sequence(flash_config_t *config);

/*! @brief Perform the cache clear to the flash*/
void flash_cache_clear(flash_config_t *config);

/*! @brief Validates the range and alignment of the given address range.*/
static status_t flash_check_range(flash_config_t *config,
                                  uint32_t startAddress,
                                  uint32_t lengthInBytes,
                                  uint32_t alignmentBaseline);
/*! @brief Gets the right address, sector and block size of current flash type which is indicated by address.*/
static status_t flash_get_matched_operation_info(flash_config_t *config,
                                                 uint32_t address,
                                                 flash_operation_config_t *info);
/*! @brief Validates the given user key for flash erase APIs.*/
static status_t flash_check_user_key(uint32_t key);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Access to FTFx->FCCOB */
#if defined(FSL_FEATURE_FLASH_IS_FTFA) && FSL_FEATURE_FLASH_IS_FTFA
volatile uint32_t *const kFCCOBx = (volatile uint32_t *)&FTFA->FCCOB3;
#elif defined(FSL_FEATURE_FLASH_IS_FTFE) && FSL_FEATURE_FLASH_IS_FTFE
volatile uint32_t *const kFCCOBx = (volatile uint32_t *)&FTFE->FCCOB3;
#elif defined(FSL_FEATURE_FLASH_IS_FTFL) && FSL_FEATURE_FLASH_IS_FTFL
volatile uint32_t *const kFCCOBx = (volatile uint32_t *)&FTFL->FCCOB3;
#else
#error "Unknown flash controller"
#endif

/*! @brief Access to FTFx->FPROT */
#if defined(FSL_FEATURE_FLASH_IS_FTFA) && FSL_FEATURE_FLASH_IS_FTFA
volatile uint32_t *const kFPROT = (volatile uint32_t *)&FTFA->FPROT3;
#elif defined(FSL_FEATURE_FLASH_IS_FTFE) && FSL_FEATURE_FLASH_IS_FTFE
volatile uint32_t *const kFPROT = (volatile uint32_t *)&FTFE->FPROT3;
#elif defined(FSL_FEATURE_FLASH_IS_FTFL) && FSL_FEATURE_FLASH_IS_FTFL
volatile uint32_t *const kFPROT = (volatile uint32_t *)&FTFL->FPROT3;
#else
#error "Unknown flash controller"
#endif

/*!
 * @brief Table of pflash sizes.
 *
 *  The index into this table is the value of the SIM_FCFG1.PFSIZE bitfield.
 *
 *  The values in this table have been right shifted 10 bits so that they will all fit within
 *  an 16-bit integer. To get the actual flash density, you must left shift the looked up value
 *  by 10 bits.
 *
 *  Elements of this table have a value of 0 in cases where the PFSIZE bitfield value is
 *  reserved.
 *
 *  Code to use the table:
 *  @code
 *      uint8_t pfsize = (SIM->FCFG1 & SIM_FCFG1_PFSIZE_MASK) >> SIM_FCFG1_PFSIZE_SHIFT;
 *      flashDensity = ((uint32_t)kPFlashDensities[pfsize]) << 10;
 *  @endcode
 */
const uint16_t kPFlashDensities[] = {
    8,    /* 0x0 - 8192, 8KB */
    16,   /* 0x1 - 16384, 16KB */
    24,   /* 0x2 - 24576, 24KB */
    32,   /* 0x3 - 32768, 32KB */
    48,   /* 0x4 - 49152, 48KB */
    64,   /* 0x5 - 65536, 64KB */
    96,   /* 0x6 - 98304, 96KB */
    128,  /* 0x7 - 131072, 128KB */
    192,  /* 0x8 - 196608, 192KB */
    256,  /* 0x9 - 262144, 256KB */
    384,  /* 0xa - 393216, 384KB */
    512,  /* 0xb - 524288, 512KB */
    768,  /* 0xc - 786432, 768KB */
    1024, /* 0xd - 1048576, 1MB */
    1536, /* 0xe - 1572864, 1.5MB */
    /* 2048,  0xf - 2097152, 2MB */
};

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t FLASH_Init(flash_config_t *config)
{
    uint32_t flashDensity;

    if (config == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* calculate the flash density from SIM_FCFG1.PFSIZE */
    uint8_t pfsize = (SIM->FCFG1 & SIM_FCFG1_PFSIZE_MASK) >> SIM_FCFG1_PFSIZE_SHIFT;
    /* PFSIZE=0xf means that on customer parts the IFR was not correctly programmed.
     * We just use the pre-defined flash size in feature file here to support pre-production parts */
    if (pfsize == 0xf)
    {
        flashDensity = FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT * FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE;
    }
    else
    {
        flashDensity = ((uint32_t)kPFlashDensities[pfsize]) << 10;
    }

    /* fill out a few of the structure members */
    config->PFlashBlockBase = PFLASH_BLOCK_BASE;
    config->PFlashTotalSize = flashDensity;
    config->PFlashBlockCount = FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT;
    config->PFlashSectorSize = FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE;

#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL) && FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    config->PFlashAccessSegmentSize = kFLASH_accessSegmentBase << FTFx->FACSS;
    config->PFlashAccessSegmentCount = FTFx->FACSN;
#else
    config->PFlashAccessSegmentSize = 0;
    config->PFlashAccessSegmentCount = 0;
#endif /* FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL */

    config->PFlashCallback = NULL;

    config->FlexRAMBlockBase = FSL_FEATURE_FLASH_FLEX_RAM_START_ADDRESS;
    config->FlexRAMTotalSize = FSL_FEATURE_FLASH_FLEX_RAM_SIZE;

#if FLASH_SSD_IS_FLEXNVM_ENABLED
    {
        status_t returnCode;
        config->DFlashBlockBase = FSL_FEATURE_FLASH_FLEX_NVM_START_ADDRESS;
        returnCode = flash_update_flexnvm_memory_partition_status(config);
        if (returnCode != kStatus_FLASH_Success)
        {
            return returnCode;
        }
    }
#endif

    return kStatus_FLASH_Success;
}

status_t FLASH_SetCallback(flash_config_t *config, flash_callback_t callback)
{
    if (config == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    config->PFlashCallback = callback;

    return kStatus_FLASH_Success;
}

status_t FLASH_EraseAll(flash_config_t *config, uint32_t key)
{
    status_t returnCode;

    if (config == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* preparing passing parameter to erase all flash blocks */
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_ERASE_ALL_BLOCK, 0xFFFFFFU);

    /* Validate the user key */
    returnCode = flash_check_user_key(key);
    if (returnCode)
    {
        return returnCode;
    }

    /* calling flash command sequence function to execute the command */
    returnCode = flash_command_sequence(config);

    flash_cache_clear(config);

#if FLASH_SSD_IS_FLEXNVM_ENABLED
    /* Data flash IFR will be erased by erase all command, so we need to
     *  update FlexNVM memory partition status synchronously */
    if (returnCode == kStatus_FLASH_Success)
    {
        returnCode = flash_update_flexnvm_memory_partition_status(config);
    }
#endif

    return returnCode;
}

status_t FLASH_Erase(flash_config_t *config, uint32_t start, uint32_t lengthInBytes, uint32_t key)
{
    uint32_t sectorSize;
    flash_operation_config_t flashInfo;
    uint32_t endAddress;      /* storing end address */
    uint32_t numberOfSectors; /* number of sectors calculated by endAddress */
    status_t returnCode;

    flash_get_matched_operation_info(config, start, &flashInfo);

    /* Check the supplied address range. */
    returnCode = flash_check_range(config, start, lengthInBytes, flashInfo.sectorCmdAddressAligment);
    if (returnCode)
    {
        return returnCode;
    }

    start = flashInfo.convertedAddress;
    sectorSize = flashInfo.activeSectorSize;

    /* calculating Flash end address */
    endAddress = start + lengthInBytes - 1;

    /* re-calculate the endAddress and align it to the start of the next sector
     * which will be used in the comparison below */
    if (endAddress % sectorSize)
    {
        numberOfSectors = endAddress / sectorSize + 1;
        endAddress = numberOfSectors * sectorSize - 1;
    }

    /* the start address will increment to the next sector address
     * until it reaches the endAdddress */
    while (start <= endAddress)
    {
        /* preparing passing parameter to erase a flash block */
        kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_ERASE_SECTOR, start);

        /* Validate the user key */
        returnCode = flash_check_user_key(key);
        if (returnCode)
        {
            return returnCode;
        }

        /* calling flash command sequence function to execute the command */
        returnCode = flash_command_sequence(config);

        /* calling flash callback function if it is available */
        if (config->PFlashCallback)
        {
            config->PFlashCallback();
        }

        /* checking the success of command execution */
        if (kStatus_FLASH_Success != returnCode)
        {
            break;
        }
        else
        {
            /* Increment to the next sector */
            start += sectorSize;
        }
    }

    flash_cache_clear(config);

    return (returnCode);
}

status_t FLASH_Program(flash_config_t *config, uint32_t start, uint32_t *src, uint32_t lengthInBytes)
{
    status_t returnCode;
    flash_operation_config_t flashInfo;

    if (src == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    flash_get_matched_operation_info(config, start, &flashInfo);

    /* Check the supplied address range. */
    returnCode = flash_check_range(config, start, lengthInBytes, flashInfo.blockWriteUnitSize);
    if (returnCode)
    {
        return returnCode;
    }

    start = flashInfo.convertedAddress;

    while (lengthInBytes > 0)
    {
        /* preparing passing parameter to program the flash block */
        kFCCOBx[1] = *src++;
        if (4 == flashInfo.blockWriteUnitSize)
        {
            kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_PROGRAM_LONGWORD, start);
        }
        else if (8 == flashInfo.blockWriteUnitSize)
        {
            kFCCOBx[2] = *src++;
            kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_PROGRAM_PHRASE, start);
        }
        else
        {
        }

        /* calling flash command sequence function to execute the command */
        returnCode = flash_command_sequence(config);

        /* calling flash callback function if it is available */
        if (config->PFlashCallback)
        {
            config->PFlashCallback();
        }

        /* checking for the success of command execution */
        if (kStatus_FLASH_Success != returnCode)
        {
            break;
        }
        else
        {
            /* update start address for next iteration */
            start += flashInfo.blockWriteUnitSize;

            /* update lengthInBytes for next iteration */
            lengthInBytes -= flashInfo.blockWriteUnitSize;
        }
    }

    flash_cache_clear(config);

    return (returnCode);
}

status_t FLASH_VerifyEraseAll(flash_config_t *config, flash_margin_value_t margin)
{
    if (config == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* preparing passing parameter to verify all block command */
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_1_2(FTFx_VERIFY_ALL_BLOCK, margin, 0xFFFFU);

    /* calling flash command sequence function to execute the command */
    return flash_command_sequence(config);
}

status_t FLASH_VerifyErase(flash_config_t *config, uint32_t start, uint32_t lengthInBytes, flash_margin_value_t margin)
{
    /* Check arguments. */
    uint32_t blockSize;
    flash_operation_config_t flashInfo;
    uint32_t nextBlockStartAddress;
    uint32_t remainingBytes;
    status_t returnCode;

    flash_get_matched_operation_info(config, start, &flashInfo);

    returnCode = flash_check_range(config, start, lengthInBytes, flashInfo.sectionCmdAddressAligment);
    if (returnCode)
    {
        return returnCode;
    }

    flash_get_matched_operation_info(config, start, &flashInfo);
    start = flashInfo.convertedAddress;
    blockSize = flashInfo.activeBlockSize;

    nextBlockStartAddress = ALIGN_UP(start, blockSize);
    if (nextBlockStartAddress == start)
    {
        nextBlockStartAddress += blockSize;
    }

    remainingBytes = lengthInBytes;

    while (remainingBytes)
    {
        uint32_t numberOfPhrases;
        uint32_t verifyLength = nextBlockStartAddress - start;
        if (verifyLength > remainingBytes)
        {
            verifyLength = remainingBytes;
        }

        numberOfPhrases = verifyLength / flashInfo.sectionCmdAddressAligment;

        /* Fill in verify section command parameters. */
        kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_VERIFY_SECTION, start);
        kFCCOBx[1] = BYTES_JOIN_TO_WORD_2_1_1(numberOfPhrases, margin, 0xFFU);

        /* calling flash command sequence function to execute the command */
        returnCode = flash_command_sequence(config);
        if (returnCode)
        {
            return returnCode;
        }

        remainingBytes -= verifyLength;
        start += verifyLength;
        nextBlockStartAddress += blockSize;
    }

    return kStatus_FLASH_Success;
}

status_t FLASH_VerifyProgram(flash_config_t *config,
                             uint32_t start,
                             uint32_t lengthInBytes,
                             const uint32_t *expectedData,
                             flash_margin_value_t margin,
                             uint32_t *failedAddress,
                             uint32_t *failedData)
{
    status_t returnCode;
    flash_operation_config_t flashInfo;

    if (expectedData == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    flash_get_matched_operation_info(config, start, &flashInfo);

    returnCode = flash_check_range(config, start, lengthInBytes, flashInfo.checkCmdAddressAligment);
    if (returnCode)
    {
        return returnCode;
    }

    start = flashInfo.convertedAddress;

    while (lengthInBytes)
    {
        /* preparing passing parameter to program check the flash block */
        kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_PROGRAM_CHECK, start);
        kFCCOBx[1] = BYTES_JOIN_TO_WORD_1_3(margin, 0xFFFFFFU);
        kFCCOBx[2] = *expectedData;

        /* calling flash command sequence function to execute the command */
        returnCode = flash_command_sequence(config);

        /* checking for the success of command execution */
        if (kStatus_FLASH_Success != returnCode)
        {
            if (failedAddress)
            {
                *failedAddress = start;
            }
            if (failedData)
            {
                *failedData = 0;
            }
            break;
        }

        lengthInBytes -= flashInfo.checkCmdAddressAligment;
        expectedData += flashInfo.checkCmdAddressAligment / sizeof(*expectedData);
        start += flashInfo.checkCmdAddressAligment;
    }

    return (returnCode);
}

#if FLASH_DRIVER_IS_FLASH_RESIDENT
/*!
 * @brief Run flash command
 *
 * This function should be copied to RAM for execution to make sure that code works
 * properly even flash cache is disabled.
 * It is for flash-resident bootloader only, not technically required for ROM or
 *  flashloader (RAM-resident bootloader).
 */

RAM_FUNCTION
void flash_run_command(FTFx_REG_ACCESS_TYPE ftfx_fstat)
{
    /* clear CCIF bit */
    *ftfx_fstat = FTFx_FSTAT_CCIF_MASK;

    /* Check CCIF bit of the flash status register, wait till it is set.
     * IP team indicates that this loop will always complete. */
    while (!((*ftfx_fstat) & FTFx_FSTAT_CCIF_MASK))
    {
    }
}
#endif /* FLASH_DRIVER_IS_FLASH_RESIDENT */

/*!
 * @brief Flash Command Sequence
 *
 * This function is used to perform the command write sequence to the flash.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @return An error code or kStatus_FLASH_Success
 */
static status_t flash_command_sequence(flash_config_t *config)
{
    uint8_t registerValue;

#if FLASH_DRIVER_IS_FLASH_RESIDENT
    /* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register */
    FTFx->FSTAT = FTFx_FSTAT_RDCOLERR_MASK | FTFx_FSTAT_ACCERR_MASK | FTFx_FSTAT_FPVIOL_MASK;

    /* We pass the ftfx_fstat address as a parameter to flash_run_comamnd() instead of using
     * pre-processed MICRO sentences or operating global variable in flash_run_comamnd()
     * to make sure that flash_run_command() will be compiled into position-independent code (PIC). */
    flash_run_command((FTFx_REG_ACCESS_TYPE)(&FTFx->FSTAT));
#else
    /* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register */
    FTFx->FSTAT = FTFx_FSTAT_RDCOLERR_MASK | FTFx_FSTAT_ACCERR_MASK | FTFx_FSTAT_FPVIOL_MASK;

    /* clear CCIF bit */
    FTFx->FSTAT = FTFx_FSTAT_CCIF_MASK;

    /* Check CCIF bit of the flash status register, wait till it is set.
     * IP team indicates that this loop will always complete. */
    while (!(FTFx->FSTAT & FTFx_FSTAT_CCIF_MASK))
    {
    }
#endif /* FLASH_DRIVER_IS_FLASH_RESIDENT */

    /* Check error bits */
    /* Get flash status register value */
    registerValue = FTFx->FSTAT;

    /* checking access error */
    if (registerValue & FTFx_FSTAT_ACCERR_MASK)
    {
        return kStatus_FLASH_AccessError;
    }
    /* checking protection error */
    else if (registerValue & FTFx_FSTAT_FPVIOL_MASK)
    {
        return kStatus_FLASH_ProtectionViolation;
    }
    /* checking MGSTAT0 non-correctable error */
    else if (registerValue & FTFx_FSTAT_MGSTAT0_MASK)
    {
        return kStatus_FLASH_CommandFailure;
    }
    else
    {
        return kStatus_FLASH_Success;
    }
}

#if FLASH_DRIVER_IS_FLASH_RESIDENT
/*!
 * @brief Run flash cache clear command
 *
 * This function should be copied to RAM for execution to make sure that code works
 * properly even flash cache is disabled.
 * It is for flash-resident bootloader only, not technically required for ROM or
 * flashloader (RAM-resident bootloader).
 */

RAM_FUNCTION
void flash_cache_clear_command(FTFx_REG32_ACCESS_TYPE ftfx_reg)
{
#if defined(FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS) && FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS
    *ftfx_reg |= MCM_PLACR_CFCC_MASK;
#elif defined(FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS) && FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS
#if defined(FMC_PFB01CR_CINV_WAY_MASK)
    *ftfx_reg = (*ftfx_reg & ~FMC_PFB01CR_CINV_WAY_MASK) | FMC_PFB01CR_CINV_WAY(~0);
#else
    *ftfx_reg = (*ftfx_reg & ~FMC_PFB0CR_CINV_WAY_MASK) | FMC_PFB0CR_CINV_WAY(~0);
#endif
#elif defined(FSL_FEATURE_FLASH_HAS_MSCM_FLASH_CACHE_CONTROLS) && FSL_FEATURE_FLASH_HAS_MSCM_FLASH_CACHE_CONTROLS
    *ftfx_reg |= MSCM_OCMDR_OCMC1(2);
    *ftfx_reg |= MSCM_OCMDR_OCMC1(1);
#else
/*    #error "Unknown flash cache controller"  */
#endif /* FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS */
       /* Memory barriers for good measure.
        * All Cache, Branch predictor and TLB maintenance operations before this instruction complete */
    __ISB();
    __DSB();
}
#endif /* FLASH_DRIVER_IS_FLASH_RESIDENT */

/*!
 * @brief Flash Cache Clear
 *
 * This function is used to perform the cache clear to the flash.
 */
#if (defined(__ICCARM__))
#pragma optimize = none
void flash_cache_clear(flash_config_t *config)
#elif (defined(__CC_ARM))
#pragma push
#pragma O0
void flash_cache_clear(flash_config_t *config)
#elif (defined(__ARMCC_VERSION))
void flash_cache_clear(flash_config_t *config)
#elif (defined(__GNUC__))
/* #pragma GCC push_options */
/* #pragma GCC optimize("O0") */
void __attribute__((optimize("O0"))) flash_cache_clear(flash_config_t *config)
#else
#error "Unknown compiler"
#endif
{
#if FLASH_DRIVER_IS_FLASH_RESIDENT

/* We pass the ftfx register address as a parameter to flash_cache_clear_comamnd() instead of using
 * pre-processed MACROs or a global variable in flash_cache_clear_comamnd()
 * to make sure that flash_cache_clear_command() will be compiled into position-independent code (PIC). */
#if defined(FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS) && FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS
#if defined(MCM)
    flash_cache_clear_command((FTFx_REG32_ACCESS_TYPE)&MCM->PLACR);
#endif
#if defined(MCM0)
    flash_cache_clear_command((FTFx_REG32_ACCESS_TYPE)&MCM0->PLACR);
#endif
#if defined(MCM1)
    flash_cache_clear_command((FTFx_REG32_ACCESS_TYPE)&MCM1->PLACR);
#endif
#elif defined(FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS) && FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS
#if defined(FMC_PFB01CR_CINV_WAY_MASK)
    flash_cache_clear_command((FTFx_REG32_ACCESS_TYPE)&FMC->PFB01CR);
#else
    flash_cache_clear_command((FTFx_REG32_ACCESS_TYPE)&FMC->PFB0CR);
#endif
#elif defined(FSL_FEATURE_FLASH_HAS_MSCM_FLASH_CACHE_CONTROLS) && FSL_FEATURE_FLASH_HAS_MSCM_FLASH_CACHE_CONTROLS
    flash_cache_clear_command((FTFx_REG32_ACCESS_TYPE)&MSCM->OCMDR[0]);
#else
    /* #error "Unknown flash cache controller" */
    /* meaningless code, just a workaround to solve warning*/
    flash_cache_clear_command((FTFx_REG32_ACCESS_TYPE)0);
#endif /* FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS */

#else

#if defined(FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS) && FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS
#if defined(MCM)
    MCM->PLACR |= MCM_PLACR_CFCC_MASK;
#endif
#if defined(MCM0)
    MCM0->PLACR |= MCM_PLACR_CFCC_MASK;
#endif
#if defined(MCM1)
    MCM1->PLACR |= MCM_PLACR_CFCC_MASK;
#endif
#elif defined(FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS) && FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS
#if defined(FMC_PFB01CR_CINV_WAY_MASK)
    FMC->PFB01CR = (FMC->PFB01CR & ~FMC_PFB01CR_CINV_WAY_MASK) | FMC_PFB01CR_CINV_WAY(~0);
#else
    FMC->PFB0CR = (FMC->PFB0CR & ~FMC_PFB0CR_CINV_WAY_MASK) | FMC_PFB0CR_CINV_WAY(~0);
#endif
#elif defined(FSL_FEATURE_FLASH_HAS_MSCM_FLASH_CACHE_CONTROLS) && FSL_FEATURE_FLASH_HAS_MSCM_FLASH_CACHE_CONTROLS
    MSCM->OCMDR[0] |= MSCM_OCMDR_OCMC1(2);
    MSCM->OCMDR[0] |= MSCM_OCMDR_OCMC1(1);
#else
/*    #error "Unknown flash cache controller" */
#endif /* FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS */
#endif /* FLASH_DRIVER_IS_FLASH_RESIDENT */
}
#if (defined(__CC_ARM))
#pragma pop
#endif
#if (defined(__GNUC__))
/* #pragma GCC pop_options */
#endif

/*! @brief Validates the range and alignment of the given address range.*/
static status_t flash_check_range(flash_config_t *config,
                                  uint32_t startAddress,
                                  uint32_t lengthInBytes,
                                  uint32_t alignmentBaseline)
{
    if (config == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* Verify the start and length are alignmentBaseline aligned. */
    if ((startAddress & (alignmentBaseline - 1)) || (lengthInBytes & (alignmentBaseline - 1)))
    {
        return kStatus_FLASH_AlignmentError;
    }

/* check for valid range of the target addresses */
#if !FLASH_SSD_IS_FLEXNVM_ENABLED
    if ((startAddress < config->PFlashBlockBase) ||
        ((startAddress + lengthInBytes) > (config->PFlashBlockBase + config->PFlashTotalSize)))
#else
    if (!(((startAddress >= config->PFlashBlockBase) &&
           ((startAddress + lengthInBytes) <= (config->PFlashBlockBase + config->PFlashTotalSize))) ||
          ((startAddress >= config->DFlashBlockBase) &&
           ((startAddress + lengthInBytes) <= (config->DFlashBlockBase + config->DFlashTotalSize)))))
#endif
    {
        return kStatus_FLASH_AddressError;
    }

    return kStatus_FLASH_Success;
}

/*! @brief Gets the right address, sector and block size of current flash type which is indicated by address.*/
static status_t flash_get_matched_operation_info(flash_config_t *config,
                                                 uint32_t address,
                                                 flash_operation_config_t *info)
{
    if (config == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* Clean up info Structure*/
    memset(info, 0, sizeof(flash_operation_config_t));

#if FLASH_SSD_IS_FLEXNVM_ENABLED
    if ((address >= config->DFlashBlockBase) && (address <= (config->DFlashBlockBase + config->DFlashTotalSize)))
    {
        info->convertedAddress = address - config->DFlashBlockBase + 0x800000U;
        info->activeSectorSize = FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE;
        info->activeBlockSize = config->DFlashTotalSize / FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT;

        info->blockWriteUnitSize = FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_WRITE_UNIT_SIZE;
        info->sectorCmdAddressAligment = FSL_FEATURE_FLASH_FLEX_NVM_SECTOR_CMD_ADDRESS_ALIGMENT;
        info->sectionCmdAddressAligment = FSL_FEATURE_FLASH_FLEX_NVM_SECTION_CMD_ADDRESS_ALIGMENT;
        info->resourceCmdAddressAligment = FSL_FEATURE_FLASH_FLEX_NVM_RESOURCE_CMD_ADDRESS_ALIGMENT;
        info->checkCmdAddressAligment = FSL_FEATURE_FLASH_FLEX_NVM_CHECK_CMD_ADDRESS_ALIGMENT;
    }
    else
#endif /* FLASH_SSD_IS_FLEXNVM_ENABLED */
    {
        info->convertedAddress = address;
        info->activeSectorSize = config->PFlashSectorSize;
        info->activeBlockSize = config->PFlashTotalSize / config->PFlashBlockCount;

        info->blockWriteUnitSize = FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE;
        info->sectorCmdAddressAligment = FSL_FEATURE_FLASH_PFLASH_SECTOR_CMD_ADDRESS_ALIGMENT;
        info->sectionCmdAddressAligment = FSL_FEATURE_FLASH_PFLASH_SECTION_CMD_ADDRESS_ALIGMENT;
        info->resourceCmdAddressAligment = FSL_FEATURE_FLASH_PFLASH_RESOURCE_CMD_ADDRESS_ALIGMENT;
        info->checkCmdAddressAligment = FSL_FEATURE_FLASH_PFLASH_CHECK_CMD_ADDRESS_ALIGMENT;
    }

    return kStatus_FLASH_Success;
}

/*! @brief Validates the given user key for flash erase APIs.*/
static status_t flash_check_user_key(uint32_t key)
{
    /* Validate the user key */
    if (key != kFLASH_apiEraseKey)
    {
        return kStatus_FLASH_EraseKeyError;
    }

    return kStatus_FLASH_Success;
}
