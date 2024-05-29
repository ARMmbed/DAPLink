/* Flash algorithm for AMA3B2KK IAP
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2024 Arm Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
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

// Generated from 'Apollo3p.FLM'
// Originated from 'AmbiqMicro.Apollo_DFP.1.4.1.pack'
static const uint32_t ama3b2kk_flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    // flash algo begins
    /* 0x0020 */ 0x2040f04f, 0x61812147, 0x61412100, 0x46086181, 0x20004770, 0xe92d4770, 0x4e3341f0, 0x4f332500,
    /* 0x0040 */ 0x444e462c, 0x68324621, 0x47904638, 0x1c644328, 0x2c044605, 0x2800d3f6, 0x2001d000, 0x81f0e8bd,
    /* 0x0060 */ 0x0cc1b510, 0x3245f3c0, 0xd8082903, 0xd8062a3f, 0x44484825, 0x48256843, 0x28004798, 0x2001d000,
    /* 0x0080 */ 0xe92dbd10, 0x460747f0, 0x07884616, 0x2001d000, 0x0491eb00, 0x8070f8df, 0x0006ea47, 0xa06cf8df,
    /* 0x00A0 */ 0x44c80780, 0xe023d11f, 0x7f00f5b4, 0x00a5d201, 0xf44fe001, 0x49166500, 0x44492000, 0xf816e004,
    /* 0x00C0 */ 0xf8012b01, 0x1c402b01, 0xd3f842a8, 0xf8d84910, 0x08abc008, 0x0203f027, 0x46504449, 0xb98047e0,
    /* 0x00E0 */ 0x0495eba4, 0x2c00442f, 0x2000d1de, 0x87f0e8bd, 0x5008f8d8, 0x463a4623, 0x46504631, 0x280047a8,
    /* 0x0100 */ 0x2001d0f4, 0x0000e7f2, 0x00000004, 0x12344321, 0x00000024, 0x00000000, 0x0800004d, 0x08000051,
    /* 0x0120 */ 0x08000055, 0x08000059, 0x0800005d, 0x08000061, 0x08000065, 0x08000069, 0x00000000, 0x00000000,
};

// Start address of flash
static const uint32_t flash_start = 0x0000C000;
// Size of flash
static const uint32_t flash_size = 0x001f4000;

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const uint32_t sectors_info[] = {
    0x0000C000,
    0x00002000,
};

/*
    0x10000000   ----------------------  algo_start
                 | FLASH ALGORITHM    |
    0x10000001   |                    |  breakpoint
    0x10000021   |                    |  Init
    0x10000033   |                    |  UnInit
    0x10000037   |                    |  EraseChip
    0x10000061   |                    |  EraseSector
    0x10000083   |                    |  ProgramPage
                 |                    |
                 ----------------------  algo_start + algo_size
                 |      unused        |
    0x10000140   ----------------------  static_base
                 | GLOBAL/STATIC DATA |
                 |                    |
    0x10000400   ----------------------  program_buffer
                 | DATA TO BE WRITTEN |
                 |                    |
                 ----------------------  program_buffer + ram_to_flash_bytes_to_be_written
                 |      unused        |
                 ----------------------
                 | SPACE FOR STACK    |
    0x10002000   ----------------------  stack_pointer
                 |      unused        |
                 ----------------------
                 | 32 BYTES FOR IAP   |
    0x100C0000   ----------------------  CHIP MEMORY LIMIT
*/


static const program_target_t flash = {
    .init = 0x10000021,
    .uninit = 0x10000033,
    .erase_chip = 0x10000037,
    .erase_sector = 0x10000061,
    .program_page = 0x10000083,
    .verify = 0x0,

    {
        .breakpoint = 0x10000001,
        .static_base = 0x10000140,
        .stack_pointer = 0x10002000
    },

    .program_buffer = 0x10000000 + 0x00001000,     // mem buffer location
    .algo_start = 0x10000000,                      // location to write prog_blob in target RAM
    .algo_size = sizeof(ama3b2kk_flash_prog_blob), // prog_blob size
    .algo_blob = ama3b2kk_flash_prog_blob,         // address of prog_blob
    .program_buffer_size = 0x0000200               // ram_to_flash_bytes_to_be_written
};
