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


// 'scfg' in hex - key valid
#define STORAGE_CFG_KEY             0x73636667

typedef __PACKED_STRUCT storage_cfg_tag {
    uint32_t        key;            // Magic key to indicate a valid record
    char            fileName[STORAGE_CFG_FILENAME_SIZE];
    uint8_t         fileVisible;
    uint32_t        fileSize;
    uint32_t        fileEncWindowStart;
    uint32_t        fileEncWindowEnd;
} storage_cfg_t;


static storage_cfg_t __ALIGNED(4) s_storage_cfg = {
    .key = STORAGE_CFG_KEY,
    .fileName = STORAGE_CFG_FILENAME,
    .fileVisible = STORAGE_CFG_FILEVISIBLE,
    .fileSize = STORAGE_CFG_FILESIZE,
    .fileEncWindowStart = 0,
    .fileEncWindowEnd = 0,
};


void storate_reset_config()
{
    // Return flash config (RAM) to default values
    s_storage_cfg.key = STORAGE_CFG_KEY;
    memcpy(s_storage_cfg.fileName, STORAGE_CFG_FILENAME, 11);
    s_storage_cfg.fileVisible = STORAGE_CFG_FILEVISIBLE;
    s_storage_cfg.fileSize = STORAGE_CFG_FILESIZE;
    s_storage_cfg.fileEncWindowStart = 0;
    s_storage_cfg.fileEncWindowEnd = 0;
}

void storage_init() {
    // Load Config from Flash if present
    storage_cfg_t *pflashConfigROM = (storage_cfg_t *)STORAGE_CONFIG_ADDRESS;
    if (STORAGE_CFG_KEY == pflashConfigROM->key) {
        memcpy(&s_storage_cfg, pflashConfigROM, sizeof(storage_cfg_t));
    } else {
        storate_reset_config();
    }
}

storage_status_t storage_cfg_write()
{
    uint32_t status = STORAGE_SUCCESS;

    // Check first is config is already present in flash
    // If differences are found, erase and write new config
    if (0 != memcmp(&s_storage_cfg, (void *)STORAGE_CONFIG_ADDRESS, sizeof(storage_cfg_t))) {
        status = storage_erase_flash_page(STORAGE_CONFIG_ADDRESS);
        if (status == STORAGE_SUCCESS) {
            status = storage_program_flash(STORAGE_CONFIG_ADDRESS, sizeof(storage_cfg_t), (uint8_t *) &s_storage_cfg);
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
            memcpy(s_storage_cfg.fileName, filename, 11);
        } else {
            // If disallowed extension is requested, .bin will be used
            memcpy(s_storage_cfg.fileName, filename, 8);
            memcpy(&s_storage_cfg.fileName[8], "BIN", 3);
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
        s_storage_cfg.fileSize = file_size;
    } else {
        status = STORAGE_ERROR;
    }
    return status;
}

storage_status_t storage_cfg_set_file_visible(const bool file_visible)
{
    s_storage_cfg.fileVisible = file_visible;
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
    return &s_storage_cfg.fileName[0];
}

uint32_t storage_cfg_get_file_size(void)
{
    return s_storage_cfg.fileSize;
}

uint8_t storage_cfg_get_file_visible(void)
{
    return s_storage_cfg.fileVisible;
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
