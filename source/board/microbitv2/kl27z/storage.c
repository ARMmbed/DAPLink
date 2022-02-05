/**
 * @file    storage.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "storage.h"

#include "fsl_flash.h"

extern flash_config_t g_flash;

storage_status_t storage_program_flash(uint32_t adr, uint32_t sz, uint8_t *buf)
{
    /* Verify data is word aligned */
    util_assert(!((uint32_t)buf & 0x3));

    int status = FLASH_Program(&g_flash, adr, (uint32_t *) buf, sz);
    if (status == kStatus_Success) {
        // Must use kFlashMargin_User, or kFlashMargin_Factory for verify program
        status = FLASH_VerifyProgram(&g_flash, adr, sz,
                              (uint32_t *) buf, kFLASH_marginValueUser,
                              NULL, NULL);
    }

    return (status == kStatus_Success) ? STORAGE_SUCCESS : STORAGE_ERROR;
}

storage_status_t storage_erase_flash_page(uint32_t adr)
{
    int status = FLASH_Erase(&g_flash, adr, g_flash.PFlashSectorSize, kFLASH_apiEraseKey);
    if (status == kStatus_Success) {
        status = FLASH_VerifyErase(&g_flash, adr, g_flash.PFlashSectorSize, kFLASH_marginValueNormal);
    }

    return (status == kStatus_Success) ? STORAGE_SUCCESS : STORAGE_ERROR;
}
