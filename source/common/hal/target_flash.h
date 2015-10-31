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
#ifndef TARGET_FLASH_H
#define TARGET_FLASH_H

#include "stdint.h"
#include "flash_blob.h"
#include "compiler.h"

#ifdef __cplusplus
  extern "C" {
#endif
      
typedef enum extension {
    UNKNOWN = 0,
    BIN,
    HEX,
} extension_t;

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
    TARGET_FAIL_WRITE_UNALIGNED,
    TARGET_FAIL_UNKNOWN_APP_FORMAT,
    TARGET_FAIL_HEX_CKSUM,
    TARGET_FAIL_HEX_PARSER,
    TARGET_FAIL_HEX_PROGRAM,
    TARGET_FAIL_HEX_INVALID_ADDRESS,
    TARGET_FAIL_HEX_INVALID_APP_OFFSET,
    TARGET_HEX_FILE_EOF,
    TARGET_FAIL_BIN_INVALID_ADDRESS,
    TARGET_FAIL_TRANSFER_IN_PROGRESS,
    TARGET_FAIL_ERROR_DURING_TRANSFER,

    // Add new values here

    TARGET_FLASH_STATUS_COUNT
} target_flash_status_t;

static const char *const fail_txt_contents[] = {
    // TARGET_OK
    "",
    // TARGET_FAIL_RESET
    "The interface firmware FAILED to reset/halt the target MCU\r\n",
    // TARGET_FAIL_ALGO_DL
    "The interface firmware FAILED to download the flash programming algorithms to the target MCU\r\n",
    // TARGET_FAIL_ALGO_DATA_SEQ
    "The interface firmware FAILED to download the flash data contents to be programmed\r\n",
    // TARGET_FAIL_INIT
    "The interface firmware FAILED to initialize the target MCU\r\n",
    // TARGET_FAIL_SECURITY_BITS
    "The interface firmware ABORTED programming. Image is trying to set security bits\r\n",
    // TARGET_FAIL_UNLOCK
    "The interface firmware FAILED to unlock the target for programming\r\n",
    // TARGET_FAIL_ERASE_SECTOR
    "Flash algorithm erase sector command FAILURE\r\n",
    // TARGET_FAIL_ERASE_ALL
    "Flash algorithm erase all command FAILURE\r\n",
    // TARGET_FAIL_WRITE
    "Flash algorithm write command FAILURE\r\n",
    // TARGET_FAIL_WRITE_UNALIGNED
    "Flash program address and data buffer unaligned\r\n",
    // TARGET_FAIL_UNKNOWN_APP_FORMAT
    "The application file format is unknown and cannot be parsed and/or processed.\r\n",
    // TARGET_FAIL_HEX_CKSUM
    "The hex file cannot be decoded. Checksum calculation failure occurred.\r\n",
    // TARGET_FAIL_HEX_PARSER
    "The hex file cannot be decoded. Parser logic failure occurred.\r\n",
    // TARGET_FAIL_HEX_PROGRAM
    "The hex file cannot be programmed. Logic failure occurred.\r\n",
    // TARGET_FAIL_HEX_INVALID_ADDRESS
    "The hex file you dropped isn't compatible with this mode or device. Are you in MAINTENANCE mode? See HELP FAQ.HTM\r\n",
    // TARGET_FAIL_HEX_INVALID_APP_OFFSET
    "The hex file offset load address is not correct .\r\n",
    // TARGET_HEX_FILE_EOF
    "",
    // TARGET_FAIL_BIN_INVALID_ADDRESS
    "The binary file is too large.\r\n",
    // TARGET_FAIL_TRANSFER_IN_PROGRESS
    "The transfer timed out.\r\n",
    // TARGET_FAIL_ERROR_DURING_TRANSFER
    "An error occurred during the transfer\n",
};

// target_flash_status_t and fail_txt_contents must stay in sync
COMPILER_ASSERT(TARGET_FLASH_STATUS_COUNT == sizeof(fail_txt_contents) / sizeof(fail_txt_contents[0]));

//! @brief Verify that security will not be enabled.
target_flash_status_t check_security_bits(uint32_t flashAddr, uint8_t *data);

//! @name Flash programming operations
//@{
target_flash_status_t target_flash_init(extension_t ext);
target_flash_status_t target_flash_uninit(void);
target_flash_status_t target_flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size);
target_flash_status_t target_flash_erase_sector(uint32_t sector);
target_flash_status_t target_flash_erase_chip(void);
//@}

#ifdef __cplusplus
  }
#endif

#endif // TARGET_FLASH_H
