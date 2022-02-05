/**
 * @file    dap_strings.h
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
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

#include "cmsis_compiler.h"
#include "info.h"
#include "util.h"
#include "target_board.h"

#if !defined(CMSIS_DAP_PRODUCT_NAME)
#define CMSIS_DAP_PRODUCT_NAME "DAPLink"
#endif

//! Maximum output buffer length of all these functions.
#define MAX_DAP_STR_LEN (60)

//! @brief Utility to copy string to output buffer and return length.
//!
//! The source string is limited to the maximum output buffer size defined
//! by the MAX_DAP_STR_LEN macro.
static uint8_t return_dap_string(char *dst, const char *src)
{
    int length = MIN(MAX_DAP_STR_LEN, strlen(src) + 1);
    memcpy(dst, src, length);
    dst[MAX_DAP_STR_LEN - 1] = 0; // Ensure there's a terminating NULL.
    return length;
}

/** Get Vendor Name string.
\param str Pointer to buffer to store the string (max 60 characters).
\return String length (including terminating NULL character) or 0 (no string).
*/
__STATIC_INLINE uint8_t DAP_GetVendorString (char *str) {
#if defined(CMSIS_DAP_VENDOR_NAME)
    return return_dap_string(str, CMSIS_DAP_VENDOR_NAME);
#else
    (void)str;
    return (0U);
#endif
}

/** Get Product Name string.
\param str Pointer to buffer to store the string (max 60 characters).
\return String length (including terminating NULL character) or 0 (no string).
*/
__STATIC_INLINE uint8_t DAP_GetProductString (char *str) {
    return return_dap_string(str, CMSIS_DAP_PRODUCT_NAME);
}

/** Get Serial Number string.
\param str Pointer to buffer to store the string (max 60 characters).
\return String length (including terminating NULL character) or 0 (no string).
*/
__STATIC_INLINE uint8_t DAP_GetSerNumString (char *str) {
    return return_dap_string(str, info_get_unique_id());
}

/** Get Target Device Vendor string.
\param str Pointer to buffer to store the string (max 60 characters).
\return String length (including terminating NULL character) or 0 (no string).
*/
__STATIC_INLINE uint8_t DAP_GetTargetDeviceVendorString (char *str) {
    if (g_board_info.target_cfg && g_board_info.target_cfg->target_vendor) {
        return return_dap_string(str, g_board_info.target_cfg->target_vendor);
    }
    else {
        return (0U);
    }
}

/** Get Target Device Name string.
\param str Pointer to buffer to store the string (max 60 characters).
\return String length (including terminating NULL character) or 0 (no string).
*/
__STATIC_INLINE uint8_t DAP_GetTargetDeviceNameString (char *str) {
    if (g_board_info.target_cfg && g_board_info.target_cfg->target_part_number) {
        return return_dap_string(str, g_board_info.target_cfg->target_part_number);
    }
    else {
        return (0U);
    }
}

/** Get Target Board Vendor string.
\param str Pointer to buffer to store the string (max 60 characters).
\return String length (including terminating NULL character) or 0 (no string).
*/
__STATIC_INLINE uint8_t DAP_GetTargetBoardVendorString (char *str) {
    if (g_board_info.board_vendor) {
        return return_dap_string(str, g_board_info.board_vendor);
    }
    else {
        return (0U);
    }
}

/** Get Target Board Name string.
\param str Pointer to buffer to store the string (max 60 characters).
\return String length (including terminating NULL character) or 0 (no string).
*/
__STATIC_INLINE uint8_t DAP_GetTargetBoardNameString (char *str) {
    if (g_board_info.board_name) {
        return return_dap_string(str, g_board_info.board_name);
    }
    else {
        return (0U);
    }
}

/** Get Product Firmware Version string.
\param str Pointer to buffer to store the string (max 60 characters).
\return String length (including terminating NULL character) or 0 (no string).
*/
__STATIC_INLINE uint8_t DAP_GetProductFirmwareVersionString (char *str) {
    return return_dap_string(str, info_get_version());
}
