/* Flash algorithm for LPC84x IAP 64kB Flash
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2026 Arm Limited, All Rights Reserved
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

// Generated from 'LPC84x_64.FLM' (LPC84x IAP 64kB Flash)
// Originating from 'Keil.LPC800_DFP.1.10.2.pack'
// digest = 7456044e6eab538adb83363713d1bf756d11efc6acce4f93e184587a8b8b97bc, file size = 12556
// algo version = 0x101, algo size = 356 (0x164)
static const uint32_t LPC84x_64_flash_prog_blob[] = {
    0xe7fdbe00,
    0x47700a80, 0x48474948, 0x60084449, 0x22004847, 0x21016102, 0x61426141, 0x61816141, 0x20024943,
    0x70083940, 0x47704610, 0x47702000, 0x4c40b5f8, 0x444c2032, 0x25004621, 0xc461263f, 0x483c3114,
    0x44484f3c, 0x91003c0c, 0x696047b8, 0xd10d2800, 0xc4612034, 0x44484834, 0x60206800, 0x3c0c4834,
    0x99004448, 0x696047b8, 0xd0002800, 0xbdf82001, 0x4d2fb5f8, 0x20320a84, 0xc511444d, 0x310c4629,
    0x4e2c482b, 0x460f602c, 0x3d084448, 0x696847b0, 0xd10e2800, 0xc5112034, 0x602c4823, 0x68004448,
    0x48236068, 0x44484639, 0x47b03d08, 0x28006968, 0x2001d000, 0xb5f8bdf8, 0x00064614, 0xcc03d10e,
    0x18406862, 0x18896821, 0x68a11840, 0x68e11840, 0x69211840, 0x42401840, 0x3c086160, 0x0ab04d14,
    0x2132444d, 0x60296068, 0x462960a8, 0x4f113114, 0x91004628, 0x696847b8, 0xd1112800, 0x60ac2033,
    0x20ffc541, 0x60683001, 0x44484807, 0x60a86800, 0x3d084807, 0x99004448, 0x696847b8, 0xd0002800,
    0xbdf82001, 0x00002ee0, 0x00000004, 0x40048040, 0x00000008, 0x0f001ff1, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

// Start address of flash
static const uint32_t flash_start = 0x00000000;
// Size of flash
static const uint32_t flash_size = 0x00010000;

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const sector_info_t sectors_info[] = {
    {0x00000000, 0x00000400},
};

static const program_target_t flash = {
    0x10000009, // Init
    0x1000002d, // UnInit
    0x10000031, // EraseChip
    0x10000075, // EraseSector
    0x100000bb, // ProgramPage
    0x00000000, // Verify

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x10000001,
        0x1000013c,
        0x10000a00
    },

    // mem buffer location
    0x10000a00,
    // location to write prog_blob in target RAM
    0x10000000,
    // prog_blob size
    sizeof(LPC84x_64_flash_prog_blob),
    // address of prog_blob
    LPC84x_64_flash_prog_blob,
    // ram_to_flash_bytes_to_be_written
    0x00000100
};
