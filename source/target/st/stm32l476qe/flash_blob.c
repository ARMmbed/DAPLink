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

static const uint32_t STM32L4xx_512_flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x6a094955, 0xd5040289, 0x42884954, 0x2001d301, 0x20004770, 0x48504770, 0x60814951, 0x60814951,
    0x60012100, 0x31faf244, 0x6a006101, 0xd40803c0, 0xf245484d, 0x60015155, 0x60412106, 0x71fff640,
    0x20006081, 0x48444770, 0xf0416941, 0x61414100, 0x47702000, 0x47702001, 0xf64ab510, 0x494222aa,
    0xe000483d, 0x6903600a, 0xd4fb03db, 0x34faf244, 0xf2486104, 0x61430304, 0xf4436943, 0x61433380,
    0x600ae000, 0x03db6903, 0x2100d4fb, 0x69016141, 0xd0024221, 0x20016104, 0x2000bd10, 0xb570bd10,
    0xf7ff4604, 0x4a30ffad, 0xf64a492b, 0x280123aa, 0xf3c4d002, 0xe00325c7, 0x25c6f3c4, 0x6013e000,
    0x03e4690c, 0xf244d4fb, 0x610c34fa, 0xeb062602, 0xea4505c5, 0x614820c0, 0xf4406948, 0x61483080,
    0x6013e000, 0x03c06908, 0x2000d4fb, 0x69086148, 0xd0014020, 0x2001610c, 0xb5f0bd70, 0xf0211dc9,
    0xf64a0107, 0x4e1825aa, 0xe0004c13, 0x69236035, 0xd4fb03db, 0x37faf244, 0xf04f6127, 0xf04f0e01,
    0xe0150c00, 0xe014f8c4, 0x60036813, 0x60436853, 0x6035e000, 0x03db6923, 0xf8c4d4fb, 0x6923c014,
    0xd002423b, 0x20016127, 0x3008bdf0, 0x32083908, 0xd1e72900, 0xbdf02000, 0x40022000, 0x08040000,
    0x45670123, 0xcdef89ab, 0x40003000, 0x00000000
};

static const program_target_t flash = {
    0x20000037, // Init
    0x20000067, // UnInit
    0x20000079, // EraseChip
    0x200000bf, // EraseSector
    0x2000011b, // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x2000018c,
        0x20000800
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(STM32L4xx_512_flash_prog_blob),   // prog_blob size
    STM32L4xx_512_flash_prog_blob,           // address of prog_blob
    0x00000400       // ram_to_flash_bytes_to_be_written
};
