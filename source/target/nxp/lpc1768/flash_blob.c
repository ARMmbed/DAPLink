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
 // Device: lpc1768
 
 #include "flash_blob.h"

static const uint32_t _flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x28100b00, 0x08c0d301, 0x4770300e, 0x4c60b570, 0x444c2500, 0x61652036, 0x60204621, 0x31144a5d,
    0x47904620, 0x28006960, 0x2001d001, 0x69a0bd70, 0x4959227d, 0x01520e00, 0x28274449, 0xdc02d01c,
    0xd1062820, 0x2828e018, 0x2847d016, 0x2848d014, 0x600ad012, 0x60cd4951, 0x3a804a50, 0x23aa6015,
    0x235560d3, 0x220160d3, 0xd0112828, 0x2820dc06, 0x2827d00e, 0xe00bd106, 0xe7ea4a49, 0xd0072847,
    0xd0052848, 0x4845604d, 0x600238c0, 0xbd702000, 0xe7f8604a, 0x47702000, 0x4c3db5f8, 0x444c2032,
    0x261d2500, 0xc2614622, 0x31144621, 0x46204f39, 0x47b89100, 0x28006960, 0x2034d10c, 0xc1614621,
    0x44484835, 0x60e06800, 0x99004620, 0x696047b8, 0xd0002800, 0xbdf82001, 0xf7ffb5f8, 0x4d2cff91,
    0x444d4604, 0x606c2032, 0x60ac4629, 0x60283114, 0x460f4e28, 0x47b04628, 0x28006968, 0x606cd10d,
    0x60ac2034, 0x48246028, 0x44484639, 0x60e86800, 0x47b04628, 0x28006968, 0x2001d000, 0xb5f8bdf8,
    0x46140006, 0x6861d10e, 0x68e26820, 0x68a11840, 0x18401889, 0x18406921, 0x18406961, 0x184069a1,
    0x61e04240, 0xf7ff4630, 0x4d11ff5b, 0x444d2132, 0x60296068, 0x462960a8, 0x4f0e3114, 0x91004628,
    0x696847b8, 0xd1102800, 0x2033606e, 0x602860ac, 0x02802001, 0x480860e8, 0x68004448, 0x46286128,
    0x47b89900, 0x28006968, 0x2001d000, 0x0000bdf8, 0x00000008, 0x1fff1ff1, 0x00000004, 0x400fc100,
    0x00002ee0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000
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
    {0x00010000, 0x00008000},
};

static const program_target_t flash = {
    0x1000002d, // Init
    0x100000b5, // UnInit
    0x100000b9, // EraseChip
    0x100000f9, // EraseSector
    0x1000013f, // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x10000001,
        0x100001c4,
        0x10000800
    },

    0x10000000 + 0x00000A00,  // mem buffer location
    0x10000000,               // location to write prog_blob in target RAM
    sizeof(_flash_prog_blob),   // prog_blob size
    _flash_prog_blob,           // address of prog_blob
    0x00000400       // ram_to_flash_bytes_to_be_written
};
