/** 
 * @file    uchar.h 
 * @brief   Compatibility for libcs without uchar.h
 * 
 * DAPLink Interface Firmware 
 * Copyright (c) 2023 Google LLC, All Rights Reserved 
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

#ifndef DAPLINK_UCHAR_H
#define DAPLINK_UCHAR_H

#if __has_include_next(<uchar.h>)
#include_next <uchar.h>
#else
#include <stdint.h>
typedef uint16_t char16_t;
#endif

#endif
