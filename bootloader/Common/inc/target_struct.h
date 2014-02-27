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
#ifndef TARGET_H
#define TARGET_H

#include <stdint.h>

typedef struct {
    uint32_t breakpoint;
    uint32_t static_base;
    uint32_t stack_pointer;
} FLASH_SYSCALL;

typedef struct {

    uint32_t init;
    uint32_t uninit;
    uint32_t erase_chip;
    uint32_t erase_sector;
    uint32_t program_page;

    FLASH_SYSCALL sys_call_param;

    uint32_t program_buffer;
    uint32_t algo_start;
    uint32_t algo_size;
    const uint32_t * image;

    uint32_t ram_to_flash_bytes_to_be_written;

} TARGET_FLASH;

typedef enum {
    RESET_HOLD,              // Hold target in reset
    RESET_PROGRAM,           // Reset target and setup for flash programming.
    RESET_RUN,               // Reset target and run normally
    RESET_RUN_WITH_DEBUG,    // Reset target and run with debug enabled (required for semihost)
    NO_DEBUG,                // Disable debug on running target
    DEBUG                    // Enable debug on running target
} TARGET_RESET_STATE;

#endif
