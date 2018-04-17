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
 // Device: lpc54114
 
 #include "flash_blob.h"

static const uint32_t _flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x47700bc0, 0x48464947, 0x60084449, 0x20004946, 0x60486008, 0x60084945, 0x07412002, 0x20007008,
    0x20004770, 0xb5f84770, 0x20324c41, 0x4621444c, 0x26072500, 0x3114c461, 0x4f3e483d, 0x3c0c4448,
    0x47b89100, 0x28006960, 0x2034d10d, 0x4835c461, 0x68004448, 0x48366020, 0x44483c0c, 0x47b89900,
    0x28006960, 0x2001d000, 0xb5f8bdf8, 0x0bc44d30, 0x444d2032, 0x4629c511, 0x482d310c, 0x602c4e2d,
    0x4448460f, 0x47b03d08, 0x28006968, 0x2034d10e, 0x4824c511, 0x4448602c, 0x60686800, 0x46394824,
    0x3d084448, 0x696847b0, 0xd0002800, 0xbdf82001, 0x4614b5f8, 0xd10e0006, 0x6862cc03, 0x68211840,
    0x18401889, 0x184068a1, 0x184068e1, 0x18406921, 0x61604240, 0x4d163c08, 0x444d0bf0, 0x60682132,
    0x60a86029, 0x31144629, 0x46284f12, 0x47b89100, 0x28006968, 0x2033d111, 0xc54160ac, 0x02802001,
    0x48086068, 0x68004448, 0x480960a8, 0x44483d08, 0x47b89900, 0x28006968, 0x2001d000, 0x0000bdf8,
    0x00002ee0, 0x00000004, 0x40000280, 0x40000380, 0x00000008, 0x03000205, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
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
    0x20000025, // Init
    0x20000043, // UnInit
    0x20000047, // EraseChip
    0x2000008b, // EraseSector
    0x200000d1, // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x20000158,
        0x20000800
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(_flash_prog_blob),   // prog_blob size
    _flash_prog_blob,           // address of prog_blob
    0x00000400       // ram_to_flash_bytes_to_be_written
};
