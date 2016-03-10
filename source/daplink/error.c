/**
 * @file    error.c
 * @brief   Implementation of error.h
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

#include "error.h"
#include "util.h"
#include "macro.h"
#include "compiler.h"

static const char *const error_message[] = {

    /* Shared errors */

    // ERROR_SUCCESS
    "Operation was successful",
    // ERROR_FAILURE
    "An error has occurred",
    // ERROR_INTERNAL
    "An internal error has occurred",

    /* VFS user errors */

    // ERROR_ERROR_DURING_TRANSFER
    "An error occurred during the transfer",
    // ERROR_TRANSFER_TIMEOUT
    "The transfer timed out.",
    // ERROR_FILE_BOUNDS
    "Possible mismatch between file size and size programmed",
    // ERROR_OOO_SECTOR
    "File sent out of order by PC. Target might not be programmed correctly.",

    /* Target flash errors */

    // ERROR_RESET
    "The interface firmware FAILED to reset/halt the target MCU",
    // ERROR_ALGO_DL
    "The interface firmware FAILED to download the flash programming algorithms to the target MCU",
    // ERROR_ALGO_DATA_SEQ
    "The interface firmware FAILED to download the flash data contents to be programmed",
    // ERROR_INIT
    "The interface firmware FAILED to initialize the target MCU",
    // ERROR_SECURITY_BITS
    "The interface firmware ABORTED programming. Image is trying to set security bits",
    // ERROR_UNLOCK
    "The interface firmware FAILED to unlock the target for programming",
    // ERROR_ERASE_SECTOR
    "Flash algorithm erase sector command FAILURE",
    // ERROR_ERASE_ALL
    "Flash algorithm erase all command FAILURE",
    // ERROR_WRITE
    "Flash algorithm write command FAILURE",

    /* File stream errors */

    // ERROR_SUCCESS_DONE
    "End of stream has been reached",
    // ERROR_SUCCESS_DONE_OR_CONTINUE
    "End of stream is unknown",
    // ERROR_HEX_CKSUM
    "The hex file cannot be decoded. Checksum calculation failure occurred.",
    // ERROR_HEX_PARSER
    "The hex file cannot be decoded. Parser logic failure occurred.",
    // ERROR_HEX_PROGRAM
    "The hex file cannot be programmed. Logic failure occurred.",
    // ERROR_HEX_INVALID_ADDRESS
    "The hex file you dropped isn't compatible with this mode or device. Are you in MAINTENANCE mode? See HELP FAQ.HTM",
    // ERROR_HEX_INVALID_APP_OFFSET
    "The hex file offset load address is not correct.",

    /* Flash decoder errors */

    // ERROR_FD_BL_UPDT_ADDR_WRONG
    "The starting address for the bootloader update is wrong.",
    // ERROR_FD_INTF_UPDT_ADDR_WRONG
    "The starting address for the interface update is wrong.",
    // ERROR_FD_UNSUPPORTED_UPDATE
    "The application file format is unknown and cannot be parsed and/or processed.",

    /* Flash IAP interface */

    // ERROR_IAP_INIT
    "In application programming initialization failed.",
    // ERROR_IAP_UNINIT
    "In application programming uninit failed.",
    // ERROR_IAP_WRITE
    "In application programming write failed.",
    // ERROR_IAP_ERASE_SECTOR
    "In application programming sector erase failed.",
    // ERROR_IAP_ERASE_ALL
    "In application programming mass erase failed.",
    // ERROR_IAP_OUT_OF_BOUNDS
    "In application programming aborted due to an out of bounds address.",
    // ERROR_IAP_UPDT_NOT_SUPPORTED
    "In application programming not supported on this device.",
    // ERROR_IAP_UPDT_INCOMPLETE
    "In application programming failed because the update sent was incomplete.",
    // ERROR_IAP_NO_INTERCEPT
    "",
    // ERROR_BL_UPDT_BAD_CRC
    "The bootloader CRC did not pass.",

};
COMPILER_ASSERT(ERROR_COUNT == ELEMENTS_IN_ARRAY(error_message));

const char *error_get_string(error_t error)
{
    const char *msg = 0;

    if (error < ERROR_COUNT) {
        msg = error_message[error];
    }

    if (0 == msg) {
        util_assert(0);
        msg = "";
    }

    return msg;
}
