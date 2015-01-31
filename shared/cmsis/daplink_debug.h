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
 
#ifndef DAPLINK_DEBUG_H
#define DAPLINK_DEBUG_H

#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "RTL.h"
#include "rl_usb.h"

#ifdef __cplusplus
 extern "C" {
#endif

//#define MSC_DEBUG

#if defined (MSC_DEBUG)
static inline uint32_t daplink_debug_print(const char* format, ...)
{
    char buf[128] = {0};
    int32_t r = 0, s = 0;
    va_list arg;
    
    va_start(arg, format);
    r = vsprintf(buf, format, arg);
    if(r > sizeof(buf)) {
        return 0;
    }
    va_end(arg);
    s = USBD_CDC_ACM_DataSend((uint8_t *)&buf, strlen(buf));
    os_dly_wait(1);
    
    return (r == s) ? (uint32_t)r : 0;
}

static inline uint32_t daplink_debug(uint8_t *buf, uint32_t size)
{
    int32_t s = USBD_CDC_ACM_DataSend((uint8_t *)&buf, size);
    os_dly_wait(5);
    return (uint32_t)s;
}

#else

static inline uint32_t daplink_debug_print(const char* format, ...)
{
    return 1;
}

static inline uint32_t daplink_debug(uint8_t *data, uint32_t size)
{
    return 1;
}

#endif

#define debug_msg(fmt, args...) daplink_debug_print(fmt, ## args);
#define debug_data(buf, size) daplink_debug(buf, size);

#ifdef __cplusplus
 }
#endif

#endif
