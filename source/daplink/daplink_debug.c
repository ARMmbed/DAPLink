/**
 * @file    daplink_debug.c
 * @brief   optional trace messages useful in development
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2021, Arm Limited, All Rights Reserved
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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "cmsis_os2.h"
#include "rl_usb.h"
#include "util.h"

#if defined (DAPLINK_DEBUG)

#if defined (DAPLINK_DEBUG_RTT)

#include "debug/SEGGER_RTT.c"

uint32_t daplink_debug(uint8_t *buf, uint32_t size)
{
    return SEGGER_RTT_Write(0, buf, size);
}

#else

static const char error_msg[] = "\r\n<OVERFLOW>\r\n";

uint32_t daplink_debug(uint8_t *buf, uint32_t size)
{
    uint32_t total_free;
    uint32_t write_free;
    uint32_t error_len = strlen(error_msg);
    total_free = USBD_CDC_ACM_DataFree();

    if (total_free < error_len) {
        // No space
        return 0;
    }

    // Size available for writing
    write_free = total_free - error_len;
    size = MIN(write_free, size);
    USBD_CDC_ACM_DataSend(buf, size);

    if (write_free == size) {
        USBD_CDC_ACM_DataSend((uint8_t *)error_msg, error_len);
    }

    return size;
}
#endif

static char daplink_debug_buf[128] = {0};
uint32_t daplink_debug_print(const char *format, ...)
{
    uint32_t ret;
    int32_t r = 0;
    va_list arg;
    ret = 1;
    va_start(arg, format);
    r = vsnprintf(daplink_debug_buf, sizeof(daplink_debug_buf), format, arg);

    if (r >= sizeof(daplink_debug_buf)) {
        r = snprintf(daplink_debug_buf, sizeof(daplink_debug_buf), "<Error - string length %i exceeds print buffer>\r\n", r);
        ret = 0;
    }

    va_end(arg);
    daplink_debug((uint8_t *)daplink_debug_buf, r);
    return ret;
}

#endif