/**
 * @file    settings.c
 * @brief   Implementation of settings.h
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

#include "string.h"

#include "settings.h"
#include "target_config.h"
#include "compiler.h"
#include "cortex_m.h"
#include "daplink.h"

// 'kvld' in hex - key valid
#define CFG_KEY             0x6b766c64
#define SECTOR_BUFFER_SIZE  16

// WARNING - THIS STRUCTURE RESIDES IN RAM STORAGE!
// Be careful with changes:
// -Only add new members to end end of this structure
// -Do not change the order of members in this structure
// -Structure must remain packed so no padding bytes are added
typedef struct __attribute__((__packed__)) cfg_ram {
    uint32_t key;               // Magic key to indicate a valid record
    uint16_t size;              // Offset of the last member from the start

    // Configurable values
    uint8_t hold_in_bl;
    char assert_file_name[64 + 1];
    uint16_t assert_line;
    uint8_t assert_source;
    uint32_t reset_bl_to_if;
    uint32_t reset_if_to_bl;

    // Add new members here

} cfg_ram_t;

// Configuration RAM
static cfg_ram_t config_ram __attribute__((section("cfgram"), zero_init));
// Ram copy of RAM config
static cfg_ram_t config_ram_copy;

void config_init()
{
    uint32_t new_size;
    // Initialize RAM copy
    memset(&config_ram_copy, 0, sizeof(config_ram_copy));
    // Read settings from RAM if the key is valid
    new_size = sizeof(config_ram);

    if (CFG_KEY == config_ram.key) {
        uint32_t size = MIN(config_ram.size, sizeof(config_ram));
        new_size = MAX(config_ram.size, sizeof(config_ram));
        memcpy(&config_ram_copy, (void *)&config_ram, size);
        config_ram_copy.assert_file_name[sizeof(config_ram_copy.assert_file_name) - 1] = 0;
    }

    // Initialize RAM
    memset((void *)&config_ram, 0, sizeof(config_ram));
    config_ram.key = CFG_KEY;
    config_ram.size = new_size;
    // Copy assert info back over (must be explicitly cleared)
    memcpy(config_ram.assert_file_name,
           config_ram_copy.assert_file_name,
           sizeof(config_ram_copy.assert_file_name));
    config_ram.assert_line =  config_ram_copy.assert_line;
    config_ram.assert_source =  config_ram_copy.assert_source;
    config_ram.reset_bl_to_if = config_ram_copy.reset_bl_to_if;
    config_ram.reset_if_to_bl = config_ram_copy.reset_if_to_bl;
    config_rom_init();
}

void config_ram_set_hold_in_bl(bool hold)
{
    config_ram.hold_in_bl = hold;
}

void config_ram_set_assert(const char *file, uint16_t line)
{
    // Initialize
    uint32_t file_name_size = strlen(file) + 1;
    const char *start;
    uint32_t assert_buf_size = sizeof(config_ram.assert_file_name);
    uint32_t copy_size;
    memset(config_ram.assert_file_name, 0, sizeof(config_ram.assert_file_name));

    // Determine size to copy
    if (file_name_size <= assert_buf_size) {
        start = file;
        copy_size = file_name_size;
    } else {
        start = &file[file_name_size - assert_buf_size];
        copy_size = assert_buf_size;
    }

    // Write to ram
    memcpy(config_ram.assert_file_name, start, copy_size);
    config_ram.assert_line = line;

    if (daplink_is_bootloader()) {
        config_ram.assert_source = ASSERT_SOURCE_BL;
    } else if (daplink_is_interface()) {
        config_ram.assert_source = ASSERT_SOURCE_APP;
    } else {
        config_ram.assert_source = ASSERT_SOURCE_NONE;
    }
}

void config_ram_clear_assert()
{
    memset(config_ram.assert_file_name, 0, sizeof(config_ram.assert_file_name));
    config_ram.assert_line = 0;
}

bool config_ram_get_hold_in_bl()
{
    return config_ram.hold_in_bl;
}

bool config_ram_get_initial_hold_in_bl()
{
    return config_ram_copy.hold_in_bl;
}

uint32_t config_ram_get_bl_to_if()
{
    return config_ram.reset_bl_to_if;
}

void config_ram_set_bl_to_if(uint32_t val)
{
    config_ram.reset_bl_to_if = val;
}

uint32_t config_ram_get_if_to_bl()
{
    return config_ram.reset_if_to_bl;
}

void config_ram_set_if_to_bl(uint32_t val)
{
    config_ram.reset_if_to_bl = val;
}

bool config_ram_get_assert(char *buf, uint16_t buf_size, uint16_t *line, assert_source_t *source)
{
    // Initialize
    const char *start;
    uint32_t copy_size;
    uint32_t assert_size = strlen(config_ram.assert_file_name) + 1;

    if (0 != buf) {
        memset(buf, 0, buf_size);
    }

    if (0 != line) {
        *line = 0;
    }

    if (0 != source) {
        *source = ASSERT_SOURCE_NONE;
    }

    // If the string is empty then there is no assert
    if (0 == config_ram.assert_file_name[0]) {
        return false;
    }

    // Determine size to copy
    if (assert_size <= buf_size) {
        start = config_ram.assert_file_name;
        copy_size = assert_size;
    } else {
        start = &config_ram.assert_file_name[assert_size - buf_size];
        copy_size = buf_size;
    }

    // Copy data over
    if (0 != buf) {
        *line = config_ram.assert_line;
    }

    if (0 != line) {
        memcpy(buf, start, copy_size);
    }

    if (0 != source) {
        *source = (assert_source_t)config_ram.assert_source;
    }

    return true;
}
