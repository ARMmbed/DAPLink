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
 // Device: stm32l082cz
 
 #include "flash_blob.h"

static const uint32_t _flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0xd0012a01, 0xd1172a02, 0x6981483b, 0x0212220f, 0x61814311, 0x60c14939, 0x60c14939, 0x61014939,
    0x61014939, 0x02c069c0, 0x4839d406, 0x60014937, 0x60412106, 0x60814937, 0x47702000, 0xd0012801,
    0xd1082802, 0x6841482c, 0x43112202, 0x68416041, 0x43112201, 0x20006041, 0xb5304770, 0x684a4926,
    0x4322154c, 0x684a604a, 0x432a2508, 0x2200604a, 0x48296002, 0xe0004a26, 0x698b6010, 0xd1fb07db,
    0x43a06848, 0x68486048, 0x604843a8, 0xbd302000, 0x47702001, 0x4c18b5f0, 0x15252300, 0x313f2608,
    0x468c0989, 0x6861e024, 0x60614329, 0x43316861, 0x21406061, 0xc080ca80, 0x29001f09, 0x4916d1fa,
    0x07ff69a7, 0x4f12d002, 0xe7f96039, 0x050969a1, 0xd0060f09, 0x210f69a0, 0x43080209, 0x200161a0,
    0x6861bdf0, 0x606143a9, 0x43b16861, 0x1c5b6061, 0xd8d8459c, 0xbdf02000, 0x40022000, 0x89abcdef,
    0x02030405, 0x8c9daebf, 0x13141516, 0x00005555, 0x40003000, 0x00000fff, 0x0000aaaa, 0x00000000
};

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const sector_info_t sectors_info[] = {
    {0x08000000, 0x00000080},
};

static const program_target_t flash = {
    0x20000021, // Init
    0x2000005d, // UnInit
    0x12000001f, // EraseChip
    0x2000007b, // EraseSector
    0x200000b5, // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x2000013c,
        0x20000800
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(_flash_prog_blob),   // prog_blob size
    _flash_prog_blob,           // address of prog_blob
    0x00000400       // ram_to_flash_bytes_to_be_written
};
