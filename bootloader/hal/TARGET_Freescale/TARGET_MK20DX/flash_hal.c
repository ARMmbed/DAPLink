/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2014 ARM Limited
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

#include "flash_hal.h"        // FlashOS Structures
#include "flash/flash.h" // Flash driver

//! @brief Storage for flash driver data.
flash_driver_t g_flash;

int flash_hal_init(uint32_t clk)
{
    /* Write 0xC520 to the unlock register */
    WDOG->UNLOCK = 0xC520;
    /* Followed by 0xD928 to complete the unlock */
    WDOG->UNLOCK = 0xD928;
    /* Clear the WDOGEN bit to disable the watchdog */
    WDOG->STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;

    return (flash_init(&g_flash) != kStatus_Success);
}

int flash_hal_uninit(uint32_t fnc)
{
    return 0;
}

int flash_hal_erase_chip(void)
{
    int status = flash_erase_all(&g_flash, kFlashEraseKey);
    if (status == kStatus_Success)
    {
        status = flash_verify_erase_all(&g_flash, kFlashMargin_Normal);
    }
    flash_cache_clear();
    return (status != kStatus_Success);
}

int flash_hal_erase_sector(uint32_t adr)
{
    int status = flash_erase(&g_flash, adr, g_flash.PFlashSectorSize, kFlashEraseKey);
    if (status == kStatus_Success)
    {
        status = flash_verify_erase(&g_flash, adr, g_flash.PFlashSectorSize, kFlashMargin_Normal);
    }
    flash_cache_clear();
    return (status != kStatus_Success);
}

int flash_hal_program_page(uint32_t adr, uint32_t sz, unsigned char *buf)
{
    int status = flash_program(&g_flash, adr, (uint32_t *)buf, sz);
    if (status == kStatus_Success)
    {
        status = flash_verify_program(&g_flash, adr, sz,
                              (const uint8_t *)buf, kFlashMargin_User,
                              NULL, NULL);
    }
    flash_cache_clear();
    return (status != kStatus_Success);
}
