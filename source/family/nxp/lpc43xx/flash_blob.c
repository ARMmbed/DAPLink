/* Flash algorithm for LPC18xx/43xx IAP 512kB Flash Bank A/B
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2022 Arm Limited, All Rights Reserved
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

// Generated from 'LPC18xx43xx_512_BA.FLM' (LPC18xx/43xx IAP 512kB Flash Bank A)
//            and 'LPC18xx43xx_512_BB.FLM' (LPC18xx/43xx IAP 512kB Flash Bank B)
// Originating from 'Keil.LPC4300_DFP.2.9.0.pack'
// digest = 61c40f68a6d637e3be566d503ac3f8dba13b6f39c7caae1a268f8945e468bd3f, file size = 12560
// algo version = 0x101, algo size = 452 (0x1c4)
static const uint32_t LPC18xx43xx_512_flash_prog_blob[] = {
    0xe7fdbe00,
    0x0e400300, 0xd3012807, 0x1dc008c0, 0xb5704770, 0x220d4956, 0x06524449, 0x42906048, 0x2001d001,
    0x2000e000, 0x4a536088, 0x62d04851, 0x4c534852, 0x444c6008, 0x4e522031, 0x46216020, 0x68323118,
    0x4620460d, 0x20004790, 0x203661a0, 0x68326020, 0x46204629, 0x69a04790, 0xd0002800, 0xbd702001,
    0x47702000, 0x4c45b5f8, 0x444c2032, 0x270e2600, 0x4d3ec4c1, 0x444d4621, 0x602068a8, 0x310c4840,
    0x483e6802, 0x44483c0c, 0x47909100, 0x280069a0, 0x2034d10f, 0x6828c4c1, 0x68a86020, 0x48386060,
    0x68023c0c, 0x99004835, 0x47904448, 0x280069a0, 0x2001d000, 0xb5f8bdf8, 0xffa2f7ff, 0x46044d2f,
    0x444d2032, 0x4e29c511, 0x444e602c, 0x606868b0, 0x46294f2b, 0x48293110, 0x4448683a, 0x91003d08,
    0x69a84790, 0xd10f2800, 0xc5112034, 0x6830602c, 0x68b06068, 0x482160a8, 0x4448683a, 0x99003d08,
    0x69a84790, 0xd0002800, 0xbdf82001, 0xb082b5f7, 0xf7ff4607, 0x4c19ff75, 0x444c2132, 0x60216060,
    0x60a04d12, 0x68a8444d, 0x60e04e15, 0x31184621, 0x46206832, 0x47909100, 0x280069a0, 0x2033d113,
    0x9804c481, 0x20016020, 0x60600280, 0x60a06828, 0x60e068a8, 0x68324809, 0x3c084448, 0x47909900,
    0x280069a0, 0x2001d000, 0xbdf0b005, 0x00000004, 0x01000800, 0x40050040, 0x00002ee0, 0x00000010,
    0x10400100, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

// Start address of flash
static const uint32_t flash_start = 0x1a000000;
// Size of flash
static const uint32_t flash_size = 0x00080000;

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const sector_info_t sectors_info[] = {
    {0x1a000000, 0x00002000},
    {0x1a010000, 0x00010000},
    {0x1b000000, 0x00002000},
    {0x1b010000, 0x00010000},
};

static const program_target_t flash = {
    0x20000013, // Init
    0x20000065, // UnInit
    0x20000069, // EraseChip
    0x200000bb, // EraseSector
    0x20000111, // ProgramPage
    0x00000000, // Verify

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x20000188,
        0x20000c00
    },

    // mem buffer location
    0x20000c00,
    // location to write prog_blob in target RAM
    0x20000000,
    // prog_blob size
    sizeof(LPC18xx43xx_512_flash_prog_blob),
    // address of prog_blob
    LPC18xx43xx_512_flash_prog_blob,
    // ram_to_flash_bytes_to_be_written
    0x00000400
};
