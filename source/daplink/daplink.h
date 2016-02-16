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
#ifndef DAPLINK_H
#define DAPLINK_H

#include <stdbool.h>
#include <stdint.h>

#include "daplink_addr.h"
#include "compiler.h"

// Assert that regions are sequential with no gaps
// and that they take up all available space

// ROM check
COMPILER_ASSERT(DAPLINK_ROM_BL_START == DAPLINK_ROM_START);
COMPILER_ASSERT(DAPLINK_ROM_BL_START + DAPLINK_ROM_BL_SIZE == DAPLINK_ROM_CONFIG_ADMIN_START);
COMPILER_ASSERT(DAPLINK_ROM_CONFIG_ADMIN_START + DAPLINK_ROM_CONFIG_ADMIN_SIZE == DAPLINK_ROM_IF_START);
COMPILER_ASSERT(DAPLINK_ROM_IF_START + DAPLINK_ROM_IF_SIZE == DAPLINK_ROM_CONFIG_USER_START);
COMPILER_ASSERT(DAPLINK_ROM_CONFIG_USER_START + DAPLINK_ROM_CONFIG_USER_SIZE == DAPLINK_ROM_START + DAPLINK_ROM_SIZE);

// RAM check
COMPILER_ASSERT(DAPLINK_RAM_APP_START == DAPLINK_RAM_START);
COMPILER_ASSERT(DAPLINK_RAM_APP_START + DAPLINK_RAM_APP_SIZE == DAPLINK_RAM_SHARED_START);
COMPILER_ASSERT(DAPLINK_RAM_SHARED_START + DAPLINK_RAM_SHARED_SIZE == DAPLINK_RAM_START + DAPLINK_RAM_SIZE);

#define DAPLINK_BUILD_KEY_IF        0x9B939699
#define DAPLINK_BUILD_KEY_BL        0x9B939D93

#define DAPLINK_HIF_ID_K20DX        0x646c0000
#define DAPLINK_HIF_ID_KL26         0x646c0001
#define DAPLINK_HIF_ID_LPC11U35     0x646c0002
#define DAPLINK_HIF_ID_SAM3U2C      0x646c0003

#define DAPLINK_INFO_OFFSET         0x20

typedef struct {
    uint32_t build_key;
    uint32_t hif_id;
    uint32_t version;
} daplink_info_t;

bool daplink_is_bootloader(void);
bool daplink_is_interface(void);

#endif
