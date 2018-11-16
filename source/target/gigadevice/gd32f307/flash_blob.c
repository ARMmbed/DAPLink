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

static const uint32_t gd32f30x_1M_flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0xf36f4975, 0x44490012, 0x48746008, 0x60012100, 0x60414973, 0x60414973, 0xf01069c0, 0xd1080f04,
    0xf2454871, 0x60015155, 0x60412106, 0x71fff640, 0x20006081, 0x48694770, 0xf0416901, 0x61010180,
    0xf0416d01, 0x65010180, 0x47702000, 0x69014863, 0x0104f041, 0x69016101, 0x0140f041, 0x68c16101,
    0x22aaf64a, 0x0f01f011, 0xd004495f, 0x68c3600a, 0x0f01f013, 0x6903d1fa, 0x0304f023, 0x6d036103,
    0x0304f043, 0x6d036503, 0x0340f043, 0x6cc36503, 0x0f01f013, 0x600ad004, 0xf0136cc3, 0xd1fa0f01,
    0xf0216d01, 0x65010104, 0x47702000, 0x4a4e494a, 0xf64a4449, 0x680923aa, 0x2c00f501, 0x45604947,
    0xf8d1d218, 0xf04cc010, 0xf8c10c02, 0x6148c010, 0xf0406908, 0x61080040, 0xf01068c8, 0xd0040f01,
    0x68c86013, 0x0f01f010, 0x6908d1fa, 0x0002f020, 0xe0176108, 0xc050f8d1, 0x0c02f04c, 0xc050f8c1,
    0x6d086548, 0x0040f040, 0x6cc86508, 0x0f01f010, 0x6013d004, 0xf0106cc8, 0xd1fa0f01, 0xf0206d08,
    0x65080002, 0x47702000, 0x1cc94b2b, 0xf021444b, 0x681b0103, 0x2c00f503, 0x45604b28, 0x2900d225,
    0xf8d3d048, 0xf04cc010, 0xf8c30c01, 0xf8d2c010, 0xf8c0c000, 0xf8d3c000, 0xf01cc00c, 0xd1fa0f01,
    0xc010f8d3, 0x0c01f02c, 0xc010f8c3, 0xc00cf8d3, 0x0f14f01c, 0x68d8d004, 0x0014f040, 0xe02360d8,
    0x1d121d00, 0xd1dc1f09, 0xb319e024, 0xc050f8d3, 0x0c01f04c, 0xc050f8c3, 0xc000f8d2, 0xc000f8c0,
    0xc04cf8d3, 0x0f01f01c, 0xf8d3d1fa, 0xf02cc050, 0xf8c30c01, 0xf8d3c050, 0xf01cc04c, 0xd0050f14,
    0xf0406cd8, 0x64d80014, 0x47702001, 0x1d121d00, 0xd1db1f09, 0x47702000, 0x00000004, 0x40022000,
    0x45670123, 0xcdef89ab, 0x40003000, 0x00000000, 0x00000000
};

// Start address of flash
static const uint32_t flash_start = 0x08000000;
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
    0x08000000, 0x00000800,
    0x08080000, 0x00001000,
};

static const program_target_t flash = {
    0x20000021, // Init
    0x20000057, // UnInit
    0x2000006d, // EraseChip
    0x200000cd, // EraseSector
    0x20000149, // ProgramPage
    0x0,        // Verify

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x2000020c,
        0x20000500
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(gd32f30x_1M_flash_prog_blob),   // prog_blob size
    gd32f30x_1M_flash_prog_blob,           // address of prog_blob
    0x00000400       // ram_to_flash_bytes_to_be_written
};
