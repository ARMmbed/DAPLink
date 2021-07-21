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

static const uint32_t stm32wb55_flash_prog_blob[] = {
    0xE00ABE00, 
    0xb085b480, 0x6078af00, 0xf103687b, 0x60fb4378, 0x0b1b68fb, 0x37144618, 0xbc8046bd, 0x00004770,
    0xaf00b480, 0x4b05bf00, 0xf403691b, 0x2b003380, 0xbf00d1f9, 0x46bdbf00, 0x4770bc80, 0x58004000,
    0xaf00b480, 0x4b05bf00, 0xf403691b, 0x2b002300, 0xbf00d1f9, 0x46bdbf00, 0x4770bc80, 0x58004000,
    0xb083b480, 0x4b08af00, 0x3080f8d3, 0x607bb2db, 0x031b687b, 0x687b607b, 0x6300f103, 0x370c4618,
    0xbc8046bd, 0xbf004770, 0x58004000, 0xb085b480, 0x60f8af00, 0x2300e9c7, 0x683a68fb, 0xf3bf601a,
    0xbf008f6f, 0x0100e9d7, 0x0200f04f, 0x0300f04f, 0x2300000a, 0x310468f9, 0x600b4613, 0x3714bf00,
    0xbc8046bd, 0x00004770, 0xb085b480, 0x60f8af00, 0x607a60b9, 0x695b4b0a, 0xda052b00, 0x4a094b08,
    0x4b07609a, 0x609a4a08, 0x691b4b05, 0xf0434a04, 0x611303b8, 0x46182300, 0x46bd3714, 0x4770bc80,
    0x58004000, 0x45670123, 0xcdef89ab, 0xb085b480, 0x6078af00, 0x681b4b0b, 0x4a0a60fb, 0xf42368fb,
    0x601353f8, 0x68fb4a07, 0x4b066013, 0x4a05695b, 0x4300f043, 0x23006153, 0x37144618, 0xbc8046bd,
    0xbf004770, 0x58004000, 0xb084b580, 0x6078af00, 0xff86f7ff, 0x687b4602, 0xd3014293, 0xe0302301,
    0xf7ff6878, 0x60f8ff4d, 0xff5af7ff, 0xff68f7ff, 0x691b4b16, 0xf0434a15, 0x611303b8, 0x22024b13,
    0x4b12615a, 0x68fb695a, 0xf40300db, 0x490f63ff, 0x614b4313, 0x695b4b0d, 0xf4434a0c, 0x61533380,
    0xff3ef7ff, 0x691b4b09, 0x03b8f003, 0xd0072b00, 0x691b4b06, 0xf0434a05, 0x611303b8, 0xe0002301,
    0x46182300, 0x46bd3710, 0xbf00bd80, 0x58004000, 0xb082b580, 0xf04faf00, 0x607b6300, 0x6878e00b,
    0xffb2f7ff, 0x2b004603, 0x2301d001, 0x687be00a, 0x5380f503, 0xf7ff607b, 0x4602ff33, 0x4293687b,
    0x2300d3ed, 0x37084618, 0xbd8046bd, 0xb086b5b0, 0x60f8af00, 0x607a60b9, 0x330368bb, 0x0303f023,
    0x68fb60bb, 0x0307f023, 0xf7ff60fb, 0x4602ff19, 0x429368fb, 0x2300d301, 0xf7ffe07c, 0xf7fffef1,
    0x4b3efeff, 0x4a3d691b, 0x03b8f043, 0x4b3b6113, 0x7280f04f, 0xf04f615a, 0xf04f0200, 0xe9c70300,
    0xe0632304, 0x695b4b35, 0xf0434a34, 0x61530301, 0xfef6f7ff, 0x68fb4602, 0xd30d4293, 0x695b4b2f,
    0xf0234a2e, 0x61530301, 0x691b4b2c, 0xf0434a2b, 0x611303b8, 0xe04d2301, 0x681b687b, 0xf04f461a,
    0xe9c70300, 0x687b2304, 0x681b3304, 0xf04f4618, 0xf04f0100, 0xf04f0200, 0x00030300, 0xe9d72200,
    0x18840104, 0x0503eb41, 0x4504e9c7, 0x2304e9d7, 0xf7ff68f8, 0x68fbfedb, 0x60fb3308, 0x3308687b,
    0x68bb607b, 0x60bb3b08, 0xfe9af7ff, 0x691b4b13, 0x0301f003, 0xd10d2b00, 0x695b4b10, 0xf0234a0f,
    0x61530301, 0x691b4b0d, 0xf0434a0c, 0x611303b8, 0xe00f2301, 0x691b4b09, 0xf0434a08, 0x61130301,
    0x695b4b06, 0xf0234a05, 0x61530301, 0x2b0068bb, 0x2300d198, 0x37184618, 0xbdb046bd, 0x58004000
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
    0x08000000, 0x00001000,
};

static const program_target_t flash = {
    0x200000cd, // Init
    0x20000111, // UnInit
    0x200001d5, // EraseChip
    0x2000014d, // EraseSector
    0x20000211, // ProgramPage
    0x0, // Verify

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x20000344,
        0x20000600
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(stm32wb55_flash_prog_blob),   // prog_blob size
    stm32wb55_flash_prog_blob,           // address of prog_blob
    0x00000400       // ram_to_flash_bytes_to_be_written
};