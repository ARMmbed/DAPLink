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
static const uint32_t stm32l475_flash_prog_blob[] = {
    0xE00ABE00, 
    0xb085b480, 0x6078af00, 0xf103687b, 0x60fb4378, 0x0adb68fb, 0x37144618, 0xbc8046bd, 0x00004770,
    0xb085b480, 0x60f8af00, 0x607a60b9, 0x4a124b11, 0x4b10609a, 0x609a4a11, 0x691b4b0e, 0xf0434a0d,
    0x611303b8, 0x69db4b0b, 0x0320f003, 0xd10a2b00, 0xf2454b0b, 0x601a5255, 0x22064b09, 0x4b08605a,
    0x72fff640, 0x2300609a, 0x37144618, 0xbc8046bd, 0xbf004770, 0x40022000, 0x45670123, 0xcdef89ab,
    0x40003000, 0xb085b480, 0x6078af00, 0x681b4b0b, 0x4a0a60fb, 0xf42368fb, 0x601353f8, 0x68fb4a07,
    0x4b066013, 0x4a05695b, 0x4300f043, 0x23006153, 0x37144618, 0xbc8046bd, 0xbf004770, 0x40022000,
    0xaf00b480, 0x695b4b16, 0xf0434a15, 0x61530304, 0x695b4b13, 0xf4434a12, 0x61534300, 0x695b4b10,
    0xf4434a0f, 0x61533380, 0x4b0ee003, 0x22aaf64a, 0x4b0b601a, 0xf403691b, 0x2b003380, 0x4b08d1f5,
    0x4a07695b, 0x0304f023, 0x4b056153, 0x4a04695b, 0x4300f423, 0x23006153, 0x46bd4618, 0x4770bc80,
    0x40022000, 0x40003000, 0xb084b580, 0x6078af00, 0xf7ff6878, 0x60f8ff65, 0x691b4b1d, 0xf0434a1c,
    0x611303b8, 0x22024b1a, 0x4b19615a, 0x68fb695a, 0xf40300db, 0x491663ff, 0x614b4313, 0x695b4b14,
    0xf4434a13, 0x61533380, 0x4b12e003, 0x22aaf64a, 0x4b0f601a, 0xf403691b, 0x2b003380, 0x4b0cd1f5,
    0x4a0b695b, 0x0302f023, 0x4b096153, 0xf003691b, 0x2b0003b8, 0x4b06d007, 0x4a05691b, 0x03b8f043,
    0x23016113, 0x2300e000, 0x37104618, 0xbd8046bd, 0x40022000, 0x40003000, 0xb085b480, 0x60f8af00,
    0x607a60b9, 0x330368bb, 0x0303f023, 0x4b2160bb, 0x4a20691b, 0x03b8f043, 0x4b1e6113, 0x615a2200,
    0x4b1ce02f, 0x4a1b695b, 0x7300f443, 0x0301f043, 0x68fb6153, 0x6812687a, 0xbf00601a, 0x691b4b15,
    0x3380f403, 0xd1f92b00, 0x695b4b12, 0xf0234a11, 0x61530301, 0x691b4b0f, 0x03b8f003, 0xd0072b00,
    0x691b4b0c, 0xf0434a0b, 0x611303b8, 0xe00c2301, 0x330468fb, 0x687b60fb, 0x607b3304, 0x3b0468bb,
    0x68bb60bb, 0xd1cc2b00, 0x46182300, 0x46bd3714, 0x4770bc80, 0x40022000, 0x00000000, 0x00000000
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
    0x2000012d, // EraseSector
    0x200001bd, // ProgramPage
    0x0, // Verify

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x20000264,
        0x20000500
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(stm32l475_flash_prog_blob),   // prog_blob size
    stm32l475_flash_prog_blob,           // address of prog_blob
    0x00000800       // ram_to_flash_bytes_to_be_written
};