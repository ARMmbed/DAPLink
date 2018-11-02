/**
 * @file    flash_blob.c
 * @brief   Flash algorithm for the TMPM066
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

static const uint32_t tmpm066_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x22004930, 0x22b1700a, 0x492f710a, 0x60084449, 0x47702000, 0x47702000, 0x47702001, 0xb410482a,
    0x68014448, 0x22aa2005, 0x18080200, 0x24156502, 0x01e42355, 0x620b1909, 0x65042480, 0x620b6502,
    0x65012110, 0x6a014821, 0xd0fc07c9, 0x2000bc10, 0x491d4770, 0x4449b430, 0x2105680a, 0x020923aa,
    0x650b1851, 0x24552515, 0x195201ed, 0x25806214, 0x650b650d, 0x21306214, 0x48146001, 0x07c96a01,
    0xbc30d0fc, 0x47702000, 0xb430490f, 0x68094449, 0x24aa2305, 0x18cb021b, 0x2515651c, 0x01ed2455,
    0x620c1949, 0x651921a0, 0xca082100, 0x1c496003, 0xdbfa2920, 0x6a014805, 0xd0fc07c9, 0x2000bc30,
    0x00004770, 0x400f2000, 0x00000004, 0x41fff000, 0x00000000, 0x00000000
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
    0x2000003D, // EraseChip
    0x20000073, // EraseSector
    0x200000A9, // ProgramPage
    0x00000000, // Verify
    
    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x200000F0,
        0x20000800
    },
    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(tmpm066_blob),     // prog_blob size
    tmpm066_blob,             // address of prog_blob
    0x00000080                // ram_to_flash_bytes_to_be_written
};
