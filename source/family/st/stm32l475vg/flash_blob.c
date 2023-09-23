/* Flash algorithm for STM32L4xx 1MB Flash
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2023 Arm Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
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

// Generated from 'STM32L4xx_1024.FLM' (STM32L4xx 1MB Flash)
// Originating from 'Keil.STM32L4xx_DFP.2.6.1.pack'
// digest = 653d7ff5bb73c3a78605010dc6484e06e05d8901873b0eb5c7a8fa14ab4e2917, file size = 15840
// algo version = 0x101, algo size = 552 (0x228)
static const uint32_t STM32L4xx_1024_flash_prog_blob[] = {
    0xe7fdbe00,
    0x8f4ff3bf, 0x487a4770, 0x497a6800, 0x0d000500, 0xd0051840, 0xd003282d, 0xd001282f, 0x47702001,
    0x47702000, 0x6a004874, 0x0fc00280, 0xb5004770, 0xf7ff4602, 0x2801ffe8, 0xf7ffd108, 0x2801fff3,
    0x486ed104, 0xd3014282, 0xbd002001, 0xbd002000, 0x4602b500, 0xffd7f7ff, 0xd0022801, 0x0d8002d0,
    0x4967bd00, 0x40080ad0, 0xd5f90311, 0x300130ff, 0x4861bd00, 0x60814963, 0x60814963, 0x60012100,
    0x61014962, 0x03c06a00, 0x4862d406, 0x60014960, 0x60412106, 0x60814960, 0x47702000, 0x49562001,
    0x614807c0, 0x47702000, 0x47702001, 0x49574852, 0x13c16101, 0x69416141, 0x04122201, 0x61414311,
    0x4a544956, 0x6011e000, 0x03db6903, 0x2100d4fb, 0x46086141, 0xb5104770, 0xf7ff4604, 0x4603ffa8,
    0xf7ff4620, 0x4944ffb5, 0x610c4c48, 0x02d800c2, 0x43021c92, 0x6948614a, 0x04122201, 0x61484310,
    0x8f4ff3bf, 0x4a434845, 0x6010e000, 0x03db690b, 0x2000d4fb, 0x69086148, 0xd0014020, 0x2001610c,
    0xb5f0bd10, 0xb0884b34, 0x611c4c38, 0x615c2400, 0xe059466e, 0x24014b30, 0x2908615c, 0x6813d315,
    0x68536003, 0xf3bf6043, 0x4c348f4f, 0x4b2a4d31, 0x602ce000, 0x03ff691f, 0x691bd4fb, 0x42234c2b,
    0x3008d13c, 0x32083908, 0x2300e03e, 0x7814e004, 0x1c52009d, 0x1c5b5174, 0xd3f8428b, 0x24ff2300,
    0x1a6d2508, 0x185fe003, 0x51f400bf, 0x429d1c5b, 0x9b01d8f9, 0x021b9900, 0x990218cb, 0x04099c03,
    0x19090624, 0x91001859, 0x99049b05, 0x18c9021b, 0x9c079b06, 0x0624041b, 0x18c9191b, 0x99009101,
    0x99016001, 0xf3bf6041, 0x4b0b8f4f, 0x691c2100, 0xd4fc03e4, 0x4c0d691b, 0xd0054223, 0x480b4906,
    0x20016108, 0xbdf0b008, 0xd1a32900, 0xe7f92000, 0xe0042000, 0xfffffbcb, 0x40022000, 0x08080000,
    0x000002ff, 0x45670123, 0xcdef89ab, 0x0000c3fa, 0x00005555, 0x40003000, 0x00000fff, 0x0000aaaa,
    0x00000000
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
    {0x08000000, 0x00000800},
};

static const program_target_t flash = {
    0x20000077, // Init
    0x200000a1, // UnInit
    0x200000b1, // EraseChip
    0x200000db, // EraseSector
    0x20000127, // ProgramPage
    0x00000000, // Verify

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x20000224,
        0x20000c00
    },

    // mem buffer location
    0x20000c00,
    // location to write prog_blob in target RAM
    0x20000000,
    // prog_blob size
    sizeof(STM32L4xx_1024_flash_prog_blob),
    // address of prog_blob
    STM32L4xx_1024_flash_prog_blob,
    // ram_to_flash_bytes_to_be_written
    0x00000400,
    // algo_flags (combination of kAlgoVerifyReturnsAddress, kAlgoSingleInitType and kAlgoSkipChipErase)
    0x00000000
};
