/**
 * @file    flash_blob.c
 * @brief   Flash algorithm for the TMPM46B
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
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

static const uint32_t tmpm46b_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x22004984, 0x22b1600a, 0x4983604a, 0x60084449, 0x47702000, 0x47702000, 0x47702001, 0xc1f8f8df,
    0x44ccb410, 0x43bcf04f, 0x7177f240, 0xe9cc487b, 0xf8c03101, 0xf8d01140, 0x428a2140, 0xf443d1fb,
    0x21aa42a8, 0xf8dc7011, 0x22553004, 0x432af443, 0xf8dc701a, 0x23804004, 0x44a8f444, 0xf8dc7023,
    0xf4433004, 0x701943a8, 0x1004f8dc, 0x412af441, 0xf8dc700a, 0x21102004, 0x42a8f442, 0x6a017011,
    0x0f01f011, 0x2100d0fb, 0x1140f8c0, 0x1140f8d0, 0xd1fb2900, 0x2000bc10, 0xf8df4770, 0xb430c17c,
    0xf5b044cc, 0xd23c2f00, 0x41bcf04f, 0x1004f8cc, 0xf8cc2107, 0x460a1008, 0xf8c14958, 0xf8d12140,
    0x42933140, 0xf8dcd1fb, 0x22aa3004, 0x43a8f443, 0xf8dc701a, 0x23554004, 0x442af444, 0xf8dc7023,
    0x24805004, 0x45a8f445, 0xf8dc702c, 0xf4444004, 0x702244a8, 0x2004f8dc, 0x422af442, 0xf8dc7013,
    0x22303004, 0x70024318, 0xf0106a08, 0xd0fb0f01, 0xf8c12000, 0xf8d10140, 0x28000140, 0xbc30d1fb,
    0xf5b04770, 0xbf341f80, 0x493e493d, 0x1004f8cc, 0x2170bf34, 0x61e0f44f, 0x1008f8cc, 0xe92de7bb,
    0x4c3501f0, 0x2f00f5b0, 0xd256444c, 0x43bcf04f, 0x23076063, 0x469c60a3, 0xf8c34b30, 0xf8d3c140,
    0x45655140, 0x2900d1fb, 0x25aadd3d, 0x27a02655, 0xc004f8d4, 0x4ca8f44c, 0x5000f88c, 0xc004f8d4,
    0x4c2af44c, 0x6000f88c, 0xc004f8d4, 0x4ca8f44c, 0x7000f88c, 0x8004f8d4, 0xcb04f852, 0x0800ea48,
    0xc000f8c8, 0x8004f8d4, 0xcb04f852, 0x0800ea48, 0xc000f8c8, 0x8004f8d4, 0xcb08f852, 0x0800ea48,
    0xc000f8c8, 0x8004f8d4, 0xcc04f852, 0x0800ea48, 0xc000f8c8, 0x39103010, 0xc020f8d3, 0x0f01f01c,
    0x2900d0fa, 0x2000dcc4, 0x0140f8c3, 0x0140f8d3, 0xd1fb2800, 0x01f0e8bd, 0xf5b04770, 0xbf341f80,
    0x4b084b07, 0xbf346063, 0xf44f2370, 0x60a363e0, 0x0000e7a1, 0x400f2000, 0x00000004, 0x5dff0000,
    0x5e080000, 0x5e100000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const sector_info_t sectors_info[] = {
    {0x00000000, 0x00008000},
};

static const program_target_t flash = {
    0x20000021, // Init
    0x20000035, // UnInit
    0x2000003d, // EraseChip
    0x200000bb, // EraseSector
    0x2000015f, // ProgramPage
    0x00000000, // Verify

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x20000248,
        0x20000800
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(tmpm46b_blob),     // prog_blob size
    tmpm46b_blob,             // address of prog_blob
    0x00000200                // ram_to_flash_bytes_to_be_written
};
