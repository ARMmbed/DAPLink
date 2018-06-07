/* Flash OS Routines (Automagically Generated)
 * Copyright (c) 2009-2015 ARM Limited
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


#include "flash_blob.h"
static const uint32_t cc3220sf_flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x48514952, 0x05406108, 0x200060c8, 0xf5b01c40, 0xdbfb4ffa, 0xf8c12099, 0x23aa011c, 0x311cf8c1,
    0x72d5f44f, 0x211cf8c1, 0x0124f8c1, 0x3124f8c1, 0x2124f8c1, 0x68c04846, 0x4846b2c2, 0xd10b2a05,
    0xf0426fc2, 0xf0420282, 0x67c272c2, 0xf8c02201, 0x220020c4, 0x20c4f8c0, 0xf0226802, 0xf0420203,
    0x60020201, 0xf36f6bc2, 0xf2020209, 0x63c21255, 0xf2404a39, 0x60101001, 0x1c402000, 0x5ffaf5b0,
    0x2000dbfb, 0xf5b01c40, 0xdbfb5ffa, 0xf8c14833, 0x200000c4, 0xf5b01c40, 0xdbfb5ffa, 0x47702000,
    0x47702000, 0xf640482e, 0x61422201, 0x6081492d, 0xf0116881, 0xd1fb0f04, 0x401068c0, 0xf04fbf18,
    0x477030ff, 0x2201f640, 0x614a4925, 0x48266008, 0x68886088, 0x0f02f010, 0x68c8d1fb, 0xbf184010,
    0x30fff04f, 0xb4704770, 0xf2424b1d, 0xf8c36c01, 0x2900c014, 0x4c1dbf18, 0xf020d01d, 0x601d057f,
    0xf000e00b, 0xf106067c, 0xf5064680, 0xf852267d, 0xf8c65b04, 0x1d005100, 0xf0101f09, 0xd1010f7c,
    0xb90d6b1d, 0xd1ec2900, 0x6a1d621c, 0x0f01f015, 0x2900d1fb, 0x68d8d1e1, 0x000cea10, 0xf04fbf18,
    0xbc7030ff, 0x00004770, 0x30031820, 0x4402f000, 0x4402d000, 0x400f7000, 0x4402f82c, 0x00010008,
    0x400fd000, 0xa4420004, 0xa4420002, 0xa4420001, 0x00000000
};

// Start address of flash
static const uint32_t flash_start = 0x01000000;
// Size of flash
static const uint32_t flash_size = 0x00100000;

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const uint32_t sectors_info[] = {
    0x02000000, 0x00000800,
};

static const program_target_t flash = {
    0x20000021, // Init
    0x200000c1, // UnInit
    0x200000c5, // EraseChip
    0x200000e5, // EraseSector
    0x20000107, // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x20000190,
        0x20000800
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(cc3200sf_flash_prog_blob),   // prog_blob size
    cc3200sf_flash_prog_blob,           // address of prog_blob
    0x00100000       // ram_to_flash_bytes_to_be_written
};