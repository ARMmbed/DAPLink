/* Flash OS Routines (Automagically Generated)
 * Copyright (c) 2009-2019 ARM Limited
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

static const uint32_t _flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x03004601, 0x28200e00, 0x0940d302, 0xe0051d00, 0xd3022810, 0x1cc00900, 0x0880e000, 0xd50102c9,
    0x43082110, 0x48424770, 0x60414940, 0x60414941, 0x60012100, 0x22f068c1, 0x60c14311, 0x06806940,
    0x483ed406, 0x6001493c, 0x60412106, 0x6081493c, 0x47702000, 0x69014836, 0x43110542, 0x20006101,
    0xb5104770, 0x69014832, 0x43212404, 0x69016101, 0x431103a2, 0x49336101, 0xe0004a30, 0x68c36011,
    0xd4fb03db, 0x43a16901, 0x20006101, 0xb530bd10, 0xffb6f7ff, 0x68ca4926, 0x431a23f0, 0x240260ca,
    0x690a610c, 0x0e0006c0, 0x610a4302, 0x03e26908, 0x61084310, 0x4a214823, 0x6010e000, 0x03ed68cd,
    0x6908d4fb, 0x610843a0, 0x060068c8, 0xd0030f00, 0x431868c8, 0x200160c8, 0xb570bd30, 0x1cc94d14,
    0x68eb0889, 0x26f00089, 0x60eb4333, 0x612b2300, 0xe0174b15, 0x431c692c, 0x6814612c, 0x68ec6004,
    0xd4fc03e4, 0x0864692c, 0x612c0064, 0x062468ec, 0xd0040f24, 0x433068e8, 0x200160e8, 0x1d00bd70,
    0x1f091d12, 0xd1e52900, 0xbd702000, 0x45670123, 0x40023c00, 0xcdef89ab, 0x00005555, 0x40003000,
    0x00000fff, 0x0000aaaa, 0x00000201, 0x00000000
};

// Start address of flash
static const uint32_t flash_start = 0x08000000;
// Size of flash
static const uint32_t flash_size = 0x00080000;

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const sector_info_t sectors_info[] = {
    { 0x08000000, 0x00004000 },
    { 0x08010000, 0x00010000 },
    { 0x08020000, 0x00020000 },
};

static const program_target_t flash = {
    0x20000047, // Init
    0x20000075, // UnInit
    0x20000083, // EraseChip
    0x200000af, // EraseSector
    0x200000fb, // ProgramPage
    0x0,        // Verify
    
    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x2000016c,
        0x20000800
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(_flash_prog_blob),   // prog_blob size
    _flash_prog_blob,           // address of prog_blob
    0x00000400       // ram_to_flash_bytes_to_be_written
};
