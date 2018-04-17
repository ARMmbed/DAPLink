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
 // Device: stm32f412re
 
 #include "flash_blob.h"

static const uint32_t _flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x03004601, 0x28200e00, 0x0940d302, 0xe0051d00, 0xd3022810, 0x1cc00900, 0x0880e000, 0xd50102c9,
    0x43082110, 0x482c4770, 0x6081492a, 0x6081492b, 0x22f068c1, 0x60c14311, 0x06806940, 0x4829d406,
    0x60014927, 0x60412106, 0x60814927, 0x47702000, 0x69414821, 0x43112201, 0x20006141, 0x481e4770,
    0x21f068c2, 0x60c2430a, 0x61424a20, 0x23026942, 0x6142431a, 0x061268c2, 0xd0040f12, 0x430a68c2,
    0x200160c2, 0x20004770, 0x20004770, 0x48124770, 0x68c36811, 0x431322f0, 0x4b1560c3, 0x1c894019,
    0x68c16141, 0xd4fc03c9, 0x060968c1, 0xd0040f09, 0x431168c1, 0x200160c1, 0x20004770, 0xb5104770,
    0x68124c05, 0x69644b0a, 0x401c401a, 0xd10042a2, 0xbd101840, 0x08192a3b, 0x40023c00, 0x4c5d6e7f,
    0x00005555, 0x40003000, 0x00000fff, 0x0fffaaec, 0x0ffffffc, 0x00000000
};

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const sector_info_t sectors_info[] = {
    {0x1fffc000, 0x00000004},
};

static const program_target_t flash = {
    0x20000047, // Init
    0x20000071, // UnInit
    0x2000007f, // EraseChip
    0x200000ab, // EraseSector
    0x200000af, // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x20000114,
        0x20000800
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(_flash_prog_blob),   // prog_blob size
    _flash_prog_blob,           // address of prog_blob
    0x00000004       // ram_to_flash_bytes_to_be_written
};
