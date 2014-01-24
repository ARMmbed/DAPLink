/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "..\FlashOS.H"        // FlashOS Structures
#include "flash/flash.h"
#include <string.h>

    /* FTFL module base */
#define FTFL_REG_BASE           0x40020000
#define EERAM_BLOCK_BASE        0x14000000

#if defined (MKP1024) || defined (MKPIFR_120MHZ)
  #define PBLOCK_SIZE             0x00100000      /* 1 MB       */
  #define EERAM_BLOCK_SIZE        0x00004000      /* 16 KB size */
  #define DEFLASH_BLOCK_BASE      0x10000000
#elif defined (MKP512X) || defined (MKD512)
  #define PBLOCK_SIZE             0x00080000      /* 512 KB     */
  #define EERAM_BLOCK_SIZE        0x00004000      /* 16 KB size */
  #define DEFLASH_BLOCK_BASE      0x10000000
#elif defined (MKP512)
  #define PBLOCK_SIZE             0x00080000      /* 512 KB     */
  #define EERAM_BLOCK_SIZE        0x00001000      /* 4 KB size  */
  #define DEFLASH_BLOCK_BASE      0x10000000
#elif defined (MKP128_50MHZ) || defined (MKP64_50MHZ) || defined (MKPIFR_50MHZ) ||\
  defined (MKP32_50MHZ) || defined (MKD32_50MHZ) || defined (MKDIFR_50MHZ) || defined (MKD32_72MHZ)

  #define PBLOCK_SIZE             0x00020000      /* 128 KB     */
  #define EERAM_BLOCK_SIZE        0x00000800      /* 4 KB size  */
  #define DEFLASH_BLOCK_BASE      0x10000000
#elif defined (MKP256_50MHZ) || defined (MKD64_50MHZ)

  #define PBLOCK_SIZE             0x00040000      /* 256 KB     */
  #define EERAM_BLOCK_SIZE        0x00000800      /* 4 KB size  */
  #define DEFLASH_BLOCK_BASE      0x10000000
#elif defined (MKP512_50MHZ)

  #define PBLOCK_SIZE             0x00080000      /* 512 KB     */
  #define EERAM_BLOCK_SIZE        0x00000800      /* 4 KB size  */
  #define DEFLASH_BLOCK_BASE      0x10000000
#else
  #define PBLOCK_SIZE             0x00040000      /* 256 KB     */
  #define EERAM_BLOCK_SIZE        0x00001000      /* 4 KB size  */
  #define DEFLASH_BLOCK_BASE      0x10000000
#endif

#define DATAFLASH_IFR_READRESOURCE_ADDRESS   0x100000FC

flash_driver_t g_flash;

unsigned long base_adr;
const unsigned long unsecureword = 0xFFFFFFFE;
/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
// #if defined (MKP128_48MHZ) || defined (MKP64_48MHZ) || defined (MKP32_48MHZ) \
//     || defined (MKPIFR_48MHZ) || defined (MKP16_48MHZ) || defined (MKP8_48MHZ)
//  #define SIM_COPC                      *((volatile unsigned long *)0x40048100)
// #else
//  #define WDOG_UNLOCK                   *((volatile unsigned short *)0x4005200E)
//  #define WDOG_STCTRLH                  *((volatile unsigned short *)0x40052000)
//  #define WDOG_STCTRLH_WDOGEN_MASK_X    0x00000001
// #endif
#define FLASH_CONFIG_FSEC             0x0000040C



int Init (unsigned long adr, unsigned long clk, unsigned long fnc)
{
    base_adr = adr;
    
#if defined (MKP128_48MHZ) || defined (MKP64_48MHZ) || defined (MKP32_48MHZ) \
    || defined (MKPIFR_48MHZ) || defined (MKP16_48MHZ) || defined (MKP8_48MHZ)
    SIM_COPC = 0x00u;
#else
    /* Write 0xC520 to the unlock register */
    WDOG_UNLOCK = 0xC520;
    /* Followed by 0xD928 to complete the unlock */
    WDOG_UNLOCK = 0xD928;
    /* Clear the WDOGEN bit to disable the watchdog */
    HW_WDOG_STCTRLH_CLR(BM_WDOG_STCTRLH_WDOGEN);
#endif

    return (flash_init(&g_flash) != kStatus_Success);
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {
  return (0);
}


/*  Blank Check Block in Flash Memory
 *    Parameter:      adr:  Block Start Address
 *                    sz:   Block Size (in bytes)
 *                    pat:  Block Pattern
 *    Return Value:   0 - OK,  1 - Failed
 */

int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat)
{
    return (flash_verify_erase(&g_flash, adr, sz, kFlashMargin_Normal) != kStatus_Success);
}

/*
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   (adr+sz) - OK, Failed Address
 */
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf)
{
    uint32_t failedAddress;
    status_t status = flash_verify_program(&g_flash, adr, sz,
                              (const uint8_t *)buf, kFlashMargin_Normal,
                              &failedAddress, NULL);

    if (status == kStatus_Success)
    {
        // Finished without Errors
        return (adr+sz);
    }
    else
    {
        return failedAddress;
    }
}

/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void)
{
    return (flash_erase_all(&g_flash) != kStatus_Success);
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */
int EraseSector (unsigned long adr)
{
    return (flash_erase(&g_flash, adr, g_flash.PFlashSectorSize) != kStatus_Success);
}

/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf)
{
    return (flash_program(&g_flash, adr, (uint32_t *)buf, sz) != kStatus_Success);
}

