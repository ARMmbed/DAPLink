/**
 * @file    storage_common.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2020 NXP
 * Copyright 2022 Micro:bit Educational Foundation
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
#include <string.h>

#include "storage.h"

#include "virtual_fs.h"
#include "cmsis_compiler.h"


// 'kvld' in hex - key valid
#define CFG_KEY             0x6b766c64

flashConfig_t gflashConfig = {
    .key = CFG_KEY,
    .fileName = STORAGE_CFG_FILENAME,
    .fileSize = STORAGE_CFG_FILESIZE,
    .fileVisible = STORAGE_CFG_FILEVISIBLE,
    .fileEncWindowStart = 0,
    .fileEncWindowEnd = 0,
};


void storate_reset_config()
{
    // Return flash config (RAM) to default values
    gflashConfig.key = CFG_KEY;
    memcpy(gflashConfig.fileName, STORAGE_CFG_FILENAME, 11);
    gflashConfig.fileSize = STORAGE_CFG_FILESIZE;
    gflashConfig.fileVisible = STORAGE_CFG_FILEVISIBLE;
    gflashConfig.fileEncWindowStart = 0;
    gflashConfig.fileEncWindowEnd = 0;
}

void storage_init() {
    // Load Config from Flash if present
    flashConfig_t *pflashConfigROM = (flashConfig_t *)STORAGE_CONFIG_ADDRESS;
    if (CFG_KEY == pflashConfigROM->key) {
        memcpy(&gflashConfig, pflashConfigROM, sizeof(flashConfig_t));
    } else {
        storate_reset_config();
    }
}

storage_status_t storage_cfg_write()
{
    uint32_t status = STORAGE_SUCCESS;

    // Check first is config is already present in flash
    // If differences are found, erase and write new config
    if (0 != memcmp(&gflashConfig, (void *)STORAGE_CONFIG_ADDRESS, sizeof(flashConfig_t))) {
        status = storage_erase_flash_page(STORAGE_CONFIG_ADDRESS);
        if (status == STORAGE_SUCCESS) {
            status = storage_program_flash(STORAGE_CONFIG_ADDRESS, sizeof(flashConfig_t), (uint8_t *) &gflashConfig);
        }
    }
    return status;
}

storage_status_t storage_cfg_erase()
{
    uint32_t status = STORAGE_SUCCESS;

    // Erase flash sector containing flash config
    status = storage_erase_flash_page(STORAGE_CONFIG_ADDRESS);
    if (status == STORAGE_SUCCESS) {
        storate_reset_config();
    }
    return status;
}

flashConfig_t* storage_get_cfg(void)
{
    return &gflashConfig;
}

bool storage_file_extension_allowed(const vfs_filename_t filename)
{
    const char *valid_extensions[] = {
        "BIN",
        "TXT",
        "CSV",
        "HTM",
        "WAV",
    };
    const uint32_t valid_extensions_len = sizeof(valid_extensions) / sizeof(valid_extensions[0]);

    // Check for invalid starting characters
    for (uint32_t i = 0; i < valid_extensions_len; i++) {
        if (0 == memcmp(&filename[8], valid_extensions[i], 3)) {
            return true;
        }
    }

    // Some checks failed so file extension is invalid
    return false;
}

storage_status_t storage_cfg_set_filename(const char * const filename)
{
    uint32_t status = STORAGE_SUCCESS;

    // Validate 8.3 filename
    if (filename_valid(filename)) {
        // Check allowed extensions (.bin, .txt, .csv, .htm, .wav)
        if (storage_file_extension_allowed(filename)) {
            memcpy(gflashConfig.fileName, filename, 11);
        } else {
            // If disallowed extension is requested, .bin will be used
            memcpy(gflashConfig.fileName, filename, 8);
            memcpy(&gflashConfig.fileName[8], "BIN", 3);
        }
    } else {
        status = STORAGE_ERROR;
    }
    return status;
}

storage_status_t storage_cfg_set_file_size(const uint32_t file_size)
{
    uint32_t status = STORAGE_SUCCESS;
    if (file_size <= STORAGE_SIZE) {
        gflashConfig.fileSize = file_size;
    } else {
        status = STORAGE_ERROR;
    }
    return status;
}

storage_status_t storage_cfg_set_file_visible(const bool file_visible)
{
    gflashConfig.fileVisible = file_visible;
    return STORAGE_SUCCESS;
}

storage_status_t storage_cfg_set_encoding_window(const uint32_t start, const uint32_t end)
{
    s_storage_cfg.fileEncWindowStart = start;
    s_storage_cfg.fileEncWindowEnd = end;
    return STORAGE_SUCCESS;
}

char* storage_cfg_get_filename(void)
{
    return &gflashConfig.fileName[0];
}

uint32_t storage_cfg_get_file_size(void)
{
    return gflashConfig.fileSize;
}

uint8_t storage_cfg_get_file_visible(void)
{
    return gflashConfig.fileVisible;
}

uint32_t storage_cfg_get_encoding_start()
{
    return s_storage_cfg.fileEncWindowStart;
}

uint32_t storage_cfg_get_encoding_end()
{
    return s_storage_cfg.fileEncWindowEnd;
}

uint8_t* storage_get_data_pointer(uint32_t adr)
{
    adr += STORAGE_ADDRESS_START;
    if (adr >= STORAGE_ADDRESS_END) {
        return NULL;
    }
    return (uint8_t *)adr;
}

storage_status_t storage_write(uint32_t adr, uint32_t sz, uint8_t *buf)
{
    adr += STORAGE_ADDRESS_START;
    if (
        adr < STORAGE_ADDRESS_START ||
        adr + sz > STORAGE_ADDRESS_END
    ) {
        return STORAGE_ERROR;
    }
    return storage_program_flash(adr, sz, buf);
}

storage_status_t storage_erase_sector(uint32_t adr)
{
    adr += STORAGE_ADDRESS_START;
    if (
        adr < STORAGE_ADDRESS_START ||
        adr > (STORAGE_ADDRESS_END - STORAGE_SECTOR_SIZE)
    ) {
        return STORAGE_ERROR;
    }
    return storage_erase_flash_page(adr);
}

storage_status_t storage_erase_range(uint32_t star_adr, uint32_t end_adr)
{
    storage_status_t status = STORAGE_SUCCESS;

    star_adr += STORAGE_ADDRESS_START;
    end_adr += STORAGE_ADDRESS_START;

    if (star_adr % STORAGE_SECTOR_SIZE == 0 &&
        end_adr % STORAGE_SECTOR_SIZE == 0 &&
        star_adr <= end_adr &&
        star_adr >= STORAGE_ADDRESS_START &&
        end_adr < STORAGE_ADDRESS_END
    ) {
        for (uint32_t addr = star_adr;
             addr <= end_adr && status == STORAGE_SUCCESS;
             addr += STORAGE_SECTOR_SIZE
        ) {
            status = storage_erase_flash_page(addr);
        }
    } else {
        status = STORAGE_ERROR;
    }

    return status;
}

storage_status_t storage_erase_all()
{
    storage_status_t status = storage_cfg_erase();
    if (STORAGE_SUCCESS == status) {
        status = storage_erase_range(0, STORAGE_SIZE - STORAGE_SECTOR_SIZE);
    }
    return status;
}
