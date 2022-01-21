/**
 * @file    storage.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2020 NXP
 * Copyright 2021 Micro:bit Educational foundation
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

#ifndef STORAGE_H_
#define STORAGE_H_

#include <stdint.h>
#include "storage_config.h"

#include "virtual_fs.h"
#include "cmsis_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * For more information viisit the full spec:
 * https://github.com/microbit-foundation/spec-i2c-protocol/blob/main/spec/index.md#i2c-flash-interface
 *
 * ↓ Flash start address for storage                        ↓ Flash end address
 * ↓         ↓ Next flash page                              ↓
 * ┌---------┬----------------------------------------------┐
 * |[config] |[data.bin                      ]              |
 * └---------┴----------------------------------------------┘
 *           ↑ storage address 0x0000        ↑ storage address set by file size
 */
#define STORAGE_CONFIG_ADDRESS      STORAGE_FLASH_ADDRESS_START
#define STORAGE_CONFIG_SIZE         STORAGE_SECTOR_SIZE
#define STORAGE_ADDRESS_START       (STORAGE_CONFIG_ADDRESS + STORAGE_CONFIG_SIZE)
#define STORAGE_ADDRESS_END         STORAGE_FLASH_ADDRESS_END
#define STORAGE_SIZE                (STORAGE_ADDRESS_END - STORAGE_ADDRESS_START)
#define STORAGE_SECTOR_COUNT        (STORAGE_SIZE / STORAGE_SECTOR_SIZE)
#define STORAGE_CFG_FILENAME        "DATA    BIN"
#define STORAGE_CFG_FILENAME_SIZE   11
#define STORAGE_CFG_FILEVISIBLE     false
#define STORAGE_CFG_FILESIZE        (STORAGE_SIZE - STORAGE_SECTOR_SIZE)

typedef enum {
    STORAGE_SUCCESS = 0,
    STORAGE_ERROR
} storage_status_t;

void storage_init(void);
uint8_t* storage_get_data_pointer(uint32_t adr);
storage_status_t storage_write(uint32_t adr, uint32_t sz, uint8_t *buf);
storage_status_t storage_erase_sector(uint32_t adr);
storage_status_t storage_erase_range(uint32_t star_adr, uint32_t end_adr);
storage_status_t storage_erase_all(void);
storage_status_t storage_program_flash(uint32_t adr, uint32_t sz, uint8_t *buf);
storage_status_t storage_erase_flash_page(uint32_t adr);
storage_status_t storage_cfg_write(void);
storage_status_t storage_cfg_erase(void);
storage_status_t storage_cfg_set_filename(const char * const filename);
storage_status_t storage_cfg_set_file_size(const uint32_t file_size);
storage_status_t storage_cfg_set_file_visible(const bool file_visible);
storage_status_t storage_cfg_set_encoding_window(const uint32_t start, const uint32_t end);
char* storage_cfg_get_filename(void);
uint32_t storage_cfg_get_file_size(void);
uint8_t storage_cfg_get_file_visible(void);
uint32_t storage_cfg_get_encoding_start(void);
uint32_t storage_cfg_get_encoding_end(void);

#ifdef __cplusplus
}
#endif

#endif /* STORAGE_H_ */
