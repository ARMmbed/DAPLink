/**
 * @file    macro.h
 * @brief   useful things
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

#ifndef MACRO_H
#define MACRO_H

#ifdef __cplusplus
extern "C" {
#endif

#define ELEMENTS_IN_ARRAY(array)        (sizeof(array)/sizeof(array[0]))

#define MB(size)                        ((size) * 1024 * 1024)

#define KB(size)                        ((size) * 1024)

#define MIN(a,b)                        ((a) < (b) ? (a) : (b))

#define MAX(a,b)                        ((a) > (b) ? (a) : (b))

#define ROUND_UP(value, boundary)       ((value) + ((boundary) - (value)) % (boundary))

#define ROUND_DOWN(value, boundary)     ((value) - ((value) % (boundary)))

#ifdef __cplusplus
}
#endif

#endif
