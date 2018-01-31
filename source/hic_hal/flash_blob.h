/**
 * @file    flash_blob.h
 * @brief   
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

#ifndef FLASH_BLOB_H
#define FLASH_BLOB_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t breakpoint;
    uint32_t static_base;
    uint32_t stack_pointer;
} program_syscall_t;

typedef struct {
    const uint32_t  init;
    const uint32_t  uninit;
    const uint32_t  erase_chip;
    const uint32_t  erase_sector;
    const uint32_t  program_page;
    const program_syscall_t sys_call_s;
    const uint32_t  program_buffer;
    const uint32_t  algo_start;
    const uint32_t  algo_size;
    const uint32_t *algo_blob;
    const uint32_t  program_buffer_size;
} program_target_t;

#ifdef __cplusplus
}
#endif

#endif
