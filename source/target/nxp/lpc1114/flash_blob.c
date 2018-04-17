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
 // Device: lpc1114
 
 #include "flash_blob.h"

static const uint32_t _flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x47700b00, 0x48454946, 0x60084449, 0x22004845, 0x21016302, 0x63426341, 0x63816341, 0x20024941,
    0x70083940, 0x47704610, 0x47702000, 0x4c3eb5f8, 0x444c2032, 0x26072500, 0xc2614622, 0x31144621,
    0x46204f3a, 0x47b89100, 0x28006960, 0x2034d10c, 0xc1614621, 0x44484832, 0x60e06800, 0x99004620,
    0x696047b8, 0xd0002800, 0xbdf82001, 0x4d2eb5f8, 0x444d0b04, 0x606c2032, 0x60ac4629, 0x60283114,
    0x460f4e2a, 0x47b04628, 0x28006968, 0x606cd10d, 0x60ac2034, 0x48226028, 0x44484639, 0x60e86800,
    0x47b04628, 0x28006968, 0x2001d000, 0xb5f8bdf8, 0x46140006, 0x6861d10e, 0x68e26820, 0x68a11840,
    0x18401889, 0x18406921, 0x18406961, 0x184069a1, 0x61e04240, 0x0b304d14, 0x2132444d, 0x60296068,
    0x462960a8, 0x4f113114, 0x91004628, 0x696847b8, 0xd1102800, 0x2033606e, 0x602860ac, 0x02802001,
    0x480760e8, 0x68004448, 0x46286128, 0x47b89900, 0x28006968, 0x2001d000, 0x0000bdf8, 0x00002ee0,
    0x00000004, 0x40048040, 0x00000008, 0x1fff1ff1, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const sector_info_t sectors_info[] = {
    {0x00000000, 0x00001000},
};

static const program_target_t flash = {
    0x10000025, // Init
    0x10000049, // UnInit
    0x1000004d, // EraseChip
    0x1000008d, // EraseSector
    0x100000cf, // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x10000001,
        0x10000150,
        0x10000800
    },

    0x10000000 + 0x00000A00,  // mem buffer location
    0x10000000,               // location to write prog_blob in target RAM
    sizeof(_flash_prog_blob),   // prog_blob size
    _flash_prog_blob,           // address of prog_blob
    0x00000400       // ram_to_flash_bytes_to_be_written
};
