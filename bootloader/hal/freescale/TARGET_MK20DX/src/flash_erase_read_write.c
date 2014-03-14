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
 
// common API for MSC to work from (CMSIS-DAP or BOOTLOADER)
#include "MK20D5.H"
#include "flash_erase_read_write.h"
#include "device_cfg.h"

// Specific respource for device FLASH read/write
#include "SSD_Types.h"
#include "SSD_FTFx.h"
#include "SSD_FTFx_Internal.h"
#include "ConfigureKeil.h"

/* FTFL module base */
#define FTFL_REG_BASE           0x40020000
#define EERAM_BLOCK_BASE        0x14000000
#define FLASH_CONFIG_FSEC       0x0000040C
/* k20 definitions */
#define PBLOCK_SIZE             0x00020000      /* 128 KB     */
#define EERAM_BLOCK_SIZE        0x00000800      /* 4 KB size  */
#define DEFLASH_BLOCK_BASE      0x10000000
#define WDOG_UNLOCK             *((volatile unsigned short *)0x4005200E)
#define WDOG_STCTRLH            *((volatile unsigned short *)0x40052000)
#define WDOG_STCTRLH_WDOGEN_MASK_X      0x00000001

static FLASH_SSD_CONFIG flashSSDConfig;

uint32_t flash_init(uint32_t clk)
{
    /* Write 0xC520 to the unlock register */
    WDOG_UNLOCK = 0xC520;
    /* Followed by 0xD928 to complete the unlock */
    WDOG_UNLOCK = 0xD928;
    /* Clear the WDOGEN bit to disable the watchdog */
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK_X;

    flashSSDConfig.ftfxRegBase        = FTFL_REG_BASE;           /* FTFL control register base */
    flashSSDConfig.PFlashBlockBase    = 0;                       /* base address of PFlash block */
    flashSSDConfig.PFlashBlockSize    = PBLOCK_SIZE;             /* size of PFlash block */
#if(DEBLOCK_SIZE != 0)
    flashSSDConfig.DFlashBlockBase    = DEFLASH_BLOCK_BASE;      /* base address of DFlash block */
#endif
    flashSSDConfig.EERAMBlockBase     = EERAM_BLOCK_BASE;        /* base address of EERAM block */
    flashSSDConfig.EERAMBlockSize     = EERAM_BLOCK_SIZE;        /* size of EERAM block */
    flashSSDConfig.DebugEnable        = 0;                       /* background debug mode enable bit */
    flashSSDConfig.CallBack           = NULL_CALLBACK;           /* pointer to callback function */
    if (FTFx_OK != pFlashInit(&flashSSDConfig)) {
        return 0;
    }
    return 1;
}

uint32_t flash_uninit(void)
{
    return 0;
}

static uint32_t erase_sector(uint32_t num)
{
    if (FTFx_OK != pFlashEraseSector(&flashSSDConfig, num*FLASH_SECTOR_SIZE, FTFx_PSECTOR_SIZE, pFlashCommandSequence)) {
        return 0;
    }
    return 1;
}

uint32_t flash_erase_chip(void)
{
    int i = APP_START_ADR;
    for( ; i<END_FLASH; i+=FLASH_SECTOR_SIZE) {
        if (!flash_erase_sector(i/FLASH_SECTOR_SIZE)) {
            return 0;
        }
    }
    return 1;
}

uint32_t __SVC_2 (uint32_t addr) 
{
    return erase_sector(addr);
}

uint32_t flash_erase_sector(uint32_t num)
{
    int res = 0;
    NVIC_DisableIRQ(USB0_IRQn);
    res = erase_sector_svc(num);
    NVIC_EnableIRQ(USB0_IRQn);
    return res;
}

uint32_t program_page(uint32_t adr, uint8_t * buf, uint32_t size)
{
    if (FTFx_OK != pFlashProgramLongword(&flashSSDConfig, adr, size, (uint32_t)buf, pFlashCommandSequence)) {
        return 0;
    }
    return 1;
}

uint32_t __SVC_3 (uint32_t adr, uint8_t * buf, uint32_t size)
{
    return program_page(adr, buf, size);
}
   

uint32_t flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size)
{
    int res = 0;
    NVIC_DisableIRQ(USB0_IRQn);
    res = program_page_svc(adr, buf, size);
    NVIC_EnableIRQ(USB0_IRQn);
    return res;
}

uint32_t read_memory(uint32_t adr, uint8_t *buf, uint32_t size)
{
	char *start_address = (char *)adr;
    while(size--) {
        *buf++ = *(char *)adr++;
    }
    return adr - *(char *)start_address;
}
