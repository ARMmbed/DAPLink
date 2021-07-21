/* Flash OS Routines (Automagically Generated)
 * Copyright (c) 2009-2019ARM Limited
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

// static const uint32_t STM32L475VG_1024_flash_prog_blob[] = {
//     0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
//     0x8f4ff3bf, 0x48584770, 0x49586800, 0x0d000500, 0xd0001840, 0x47702001, 0x6a004855, 0x0fc00280,
//     0xb5004770, 0xf7ff4602, 0x2801ffee, 0xf7ffd108, 0x2801fff3, 0x484fd104, 0xd3014282, 0xbd002001,
//     0xbd002000, 0x4602b500, 0xffddf7ff, 0xd0022801, 0x0d8002d0, 0x4948bd00, 0x40080ad0, 0xd5f90311,
//     0x300130ff, 0x4842bd00, 0x60814944, 0x60814944, 0x60012100, 0x61014943, 0x03c06a00, 0x4843d406,
//     0x60014941, 0x60412106, 0x60814941, 0x47702000, 0x49372001, 0x614807c0, 0x47702000, 0x47702001,
//     0x49384833, 0x13c16101, 0x69416141, 0x04122201, 0x61414311, 0x4a354937, 0x6011e000, 0x03db6903,
//     0x2100d4fb, 0x46086141, 0xb5104770, 0xf7ff4604, 0x4603ffa8, 0xf7ff4620, 0x4925ffb5, 0x610c4c29,
//     0x02d800c2, 0x43021c92, 0x6948614a, 0x04122201, 0x61484310, 0x8f4ff3bf, 0x4a244826, 0x6010e000,
//     0x03db690b, 0x2000d4fb, 0x69086148, 0xd0014020, 0x2001610c, 0xb5f0bd10, 0x4d151dc9, 0x4f1908c9,
//     0x612f00c9, 0x616b2300, 0xe0184c1a, 0x616b2301, 0x60036813, 0x60436853, 0x8f4ff3bf, 0xe0004b13,
//     0x692e601c, 0xd4fb03f6, 0x616b2300, 0x423b692b, 0x612fd002, 0xbdf02001, 0x39083008, 0x29003208,
//     0x2000d1e4, 0x0000bdf0, 0xe0042000, 0xfffffbcb, 0x40022000, 0x08080000, 0x000002ff, 0x45670123,
//     0xcdef89ab, 0x0000c3fa, 0x00005555, 0x40003000, 0x00000fff, 0x0000aaaa, 0x00000000
// };

// // Start address of flash
// static const uint32_t flash_start = 0x08000000;
// // Size of flash
// static const uint32_t flash_size  = 0x00100000;

// /**
// * List of start and size for each size of flash sector
// * The size will apply to all sectors between the listed address and the next address
// * in the list.
// * The last pair in the list will have sectors starting at that address and ending
// * at address start + size.
// */
// static const sector_info_t sectors_info[] = {
//     {0x08000000, 0x00000800},
// };

// static const program_target_t flash = {
//     .init = 0x20000087, // Init
//     .uninit = 0x200000b1, // UnInit
//     .erase_chip = 0x200000c1, // EraseChip
//     .erase_sector = 0x200000eb, // EraseSector
//     .program_page = 0x20000137, // ProgramPage
//     .verify = 0x0,        // Verify
    
//     // BKPT : start of blob + 1
//     // RSB  : blob start + header + rw data offset
//     // RSP  : stack pointer
//     {
//         .breakpoint = 0x20000001,
//         .static_base = 0x200001b8,
//         .stack_pointer = 0x20000400
//     },

//     .program_buffer = 0x20000000 + 0x00000A00,  // mem buffer location
//     .algo_start = 0x20000000,               // location to write prog_blob in target RAM
//     .algo_size = sizeof(STM32L475VG_1024_flash_prog_blob),   // prog_blob size
//     .algo_blob = STM32L475VG_1024_flash_prog_blob,           // address of prog_blob
//     .program_buffer_size = 0x00000400       // ram_to_flash_bytes_to_be_written
//     //.program_buffer_size = 512       // ram_to_flash_bytes_to_be_written
// };

static const uint32_t stm32l475_flash_prog_blob[] = {
    0xE00ABE00, 
    0xb085b480, 0x6078af00, 0xf103687b, 0x60fb4378, 0x0adb68fb, 0x37144618, 0xbc8046bd, 0x00004770,
    0xb085b480, 0x60f8af00, 0x607a60b9, 0x4a124b11, 0x4b10609a, 0x609a4a11, 0x691b4b0e, 0xf0434a0d,
    0x611303b8, 0x69db4b0b, 0x0320f003, 0xd10a2b00, 0xf2454b0b, 0x601a5255, 0x22064b09, 0x4b08605a,
    0x72fff640, 0x2300609a, 0x37144618, 0xbc8046bd, 0xbf004770, 0x40022000, 0x45670123, 0xcdef89ab,
    0x40003000, 0xb085b480, 0x6078af00, 0x681b4b0b, 0x4a0a60fb, 0xf42368fb, 0x601353f8, 0x68fb4a07,
    0x4b066013, 0x4a05695b, 0x4300f043, 0x23006153, 0x37144618, 0xbc8046bd, 0xbf004770, 0x40022000,
    0xaf00b480, 0x695b4b10, 0xf0434a0f, 0x61530304, 0x695b4b0d, 0xf4434a0c, 0x61533380, 0x4b0be003,
    0x22aaf64a, 0x4b08601a, 0xf403691b, 0x2b003380, 0x4b05d1f5, 0x4a04695b, 0x0304f023, 0x23006153,
    0x46bd4618, 0x4770bc80, 0x40022000, 0x40003000, 0xb084b580, 0x6078af00, 0xf7ff6878, 0x60f8ff71,
    0x691b4b1d, 0xf0434a1c, 0x611303b8, 0x22024b1a, 0x4b19615a, 0x68fb695a, 0xf40300db, 0x491663ff,
    0x614b4313, 0x695b4b14, 0xf4434a13, 0x61533380, 0x4b12e003, 0x22aaf64a, 0x4b0f601a, 0xf403691b,
    0x2b003380, 0x4b0cd1f5, 0x4a0b695b, 0x0302f023, 0x4b096153, 0xf003691b, 0x2b0003b8, 0x4b06d007,
    0x4a05691b, 0x03b8f043, 0x23016113, 0x2300e000, 0x37104618, 0xbd8046bd, 0x40022000, 0x40003000,
    0xb085b480, 0x60f8af00, 0x607a60b9, 0x330368bb, 0x0303f023, 0x4b2160bb, 0x4a20691b, 0x03b8f043,
    0x4b1e6113, 0x615a2200, 0x4b1ce02f, 0x4a1b695b, 0x7300f443, 0x0301f043, 0x68fb6153, 0x6812687a,
    0xbf00601a, 0x691b4b15, 0x3380f403, 0xd1f92b00, 0x695b4b12, 0xf0234a11, 0x61530301, 0x691b4b0f,
    0x03b8f003, 0xd0072b00, 0x691b4b0c, 0xf0434a0b, 0x611303b8, 0xe00c2301, 0x330468fb, 0x687b60fb,
    0x607b3304, 0x3b0468bb, 0x68bb60bb, 0xd1cc2b00, 0x46182300, 0x46bd3714, 0x4770bc80, 0x40022000
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
static const sector_info_t sectors_info[] = {
    0x08000000, 0x00000800,
};

static const program_target_t flash = {
    0x20000025, // Init
    0x20000089, // UnInit
    0x200000c5, // EraseChip
    0x20000115, // EraseSector
    0x200001a5, // ProgramPage
    0x0, // Verify

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x20000244,
        0x20000500
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(stm32l475_flash_prog_blob),   // prog_blob size
    stm32l475_flash_prog_blob,           // address of prog_blob
    0x00000400       // ram_to_flash_bytes_to_be_written
};