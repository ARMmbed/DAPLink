/**
 * @file    daplink_debug.h
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

#ifndef DAPLINK_DEBUG_H
#define DAPLINK_DEBUG_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DAPLINK_DEBUG
// #define DAPLINK_DEBUG
#endif

#if defined (DAPLINK_DEBUG)

uint32_t daplink_debug_print(const char *format, ...);
uint32_t daplink_debug(uint8_t *data, uint32_t size);

#else

static inline uint32_t daplink_debug_print(const char *format, ...)
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

#endif

#ifdef __cplusplus
}
#endif
