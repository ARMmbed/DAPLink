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
#ifndef TARGET_FLASH_COMMON_H
#define TARGET_FLASH_COMMON_H

#include "stdint.h"

#ifdef __cplusplus
  extern "C" {
#endif

//ToDo: move all into .c and extern access. causing duplicate entries in memory
      
typedef enum extension {
    UNKNOWN = 0,
    BIN,
    HEX,
} extension_t;

// known extension types
// CRD - chrome
// PAR - IE
// Extensions dont matter much if you're looking for specific file data
//  other than size parsing but hex and srec have specific EOF records
static const char *const known_extensions[] = {
    "BIN",
    "bin",
    "HEX",
    "hex",
    0,
};

typedef enum target_flash_status {
    TARGET_OK = 0,
    TARGET_FAIL_RESET,
    TARGET_FAIL_ALGO_DL,
    TARGET_FAIL_ALGO_DATA_SEQ,
    TARGET_FAIL_INIT,
    TARGET_FAIL_SECURITY_BITS,
    TARGET_FAIL_UNLOCK,
    TARGET_FAIL_ERASE_SECTOR,
    TARGET_FAIL_ERASE_ALL,
    TARGET_FAIL_WRITE,
    TARGET_FAIL_INVALID_HEX_FILE,
    TARGET_HEX_FILE_EOF,
}target_flash_status_t;

static const char *const fail_txt_contents[] = {
    "",
    "The interface firmware FAILED to reset/halt the target MCU\r\n",
    "The interface firmware FAILED to download the flash programming algorithms to the target MCU\r\n",
    "The interface firmware FAILED to download the flash data contents to be programmed\r\n",
    "The interface firmware FAILED to initialize the target MCU\r\n",
    "The interface firmware ABORTED programming. Image is trying to set security bits\r\n",
    "The interface firmware FAILED to unlock the target for programming\r\n",
    "Flash algorithm erase sector command FAILURE\r\n",
    "Flash algorithm erase all command FAILURE\r\n",
    "Flash algorithm write command FAILURE\r\n",
    "The interface firmware FAILED to parse the hex file\r\n",
};

//! @brief Verify that security will not be enabled.
target_flash_status_t check_security_bits(uint32_t flashAddr, uint8_t *data);

//! @name Flash programming operations
//@{
uint8_t validate_bin_nvic(uint8_t *buf);
uint8_t validate_hexfile(uint8_t *buf);
target_flash_status_t target_flash_init(extension_t ext);
target_flash_status_t target_flash_uninit(void);
target_flash_status_t target_flash_erase_chip(void);
target_flash_status_t target_flash_erase_sector(uint32_t adr);
target_flash_status_t target_flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size);
//@}

#ifdef __cplusplus
  }
#endif

#endif // TARGET_FLASH_COMMON_H
