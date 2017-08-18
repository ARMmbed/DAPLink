/**
 * @file    flash_blob.c
 * @brief   Flash algorithm for stm32f303k8 chips
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

#include "flash_blob.h"

static const uint32_t flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x49434844, 0x49446041, 0x21006041, 0x68c16001, 0x0114f041, 0x69c060c1, 0x7f80f410, 0x483fd108, 
    0x5155f245, 0x21066001, 0xf6406041, 0x608171ff, 0x47702000, 0x69014837, 0x0180f041, 0x20006101, 
    0x48344770, 0xf0416901, 0x61010104, 0xf0416901, 0x61010140, 0xf01168c1, 0xbf1c0f01, 0x21aaf64a, 
    0xd0044a2e, 0x68c36011, 0x0f01f013, 0x6901d1fa, 0x0104f021, 0x20006101, 0x49264770, 0xf042690a, 
    0x610a0202, 0x69086148, 0x0040f040, 0x68c86108, 0x0f01f010, 0xf64abf1c, 0x4a2020aa, 0x6010d004, 
    0xf01368cb, 0xd1fa0f01, 0xf0206908, 0x61080002, 0x47702000, 0xf0311c49, 0xbf180101, 0xd0244b15, 
    0xc010f8d3, 0x0c01f04c, 0xc010f8c3, 0xc000f8b2, 0xc000f8a0, 0xc00cf8d3, 0x0f01f01c, 0xf8d3d1fa, 
    0xf02cc010, 0xf8c30c01, 0xf8d3c010, 0xf01cc00c, 0xd0050f14, 0xf04068d8, 0x60d80014, 0x47702001, 
    0x1e891c80, 0x0202f102, 0x2000d1da, 0x00004770, 0x45670123, 0x40022000, 0xcdef89ab, 0x40003000, 
    0x00000000, 
};

static const program_target_t flash = {
    0x20000021, // Init
    0x20000055, // UnInit
    0x20000063, // EraseChip
    0x2000009B, // EraseSector
    0x200000D5, // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000000 + 0x00000001,
        0x20000000 + 0x00000020 + 0x00000120,
        0x20000800
    },

    0x20000000 + 0x00000A00,   // mem buffer location
    0x20000000,                // location to write prog_blob in target RAM
    sizeof(flash_prog_blob),   // prog_blob size
    flash_prog_blob,           // address of prog_blob
    0x00000200                 // ram_to_flash_bytes_to_be_written
};
