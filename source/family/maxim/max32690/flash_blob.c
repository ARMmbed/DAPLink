/**
 * @file    flash_blob.c
 * @brief   Flash algorithm for the MAX32690
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2023 Analog Devices, Inc.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "flash_blob.h"

#define CLOCK_DIV       0x00000078
#define BURST_SIZE      0x00000080

#define FLC0_BASE       0x40029000
#define FLASH0_BASE     0x10000000 // 
#define FLASH0_SIZE     0x00300000 // 3MB
#define FLASH0_SECTOR   KB(16)

#define FLC1_BASE       0x40029400 // second flash controller
#define FLASH1_BASE     0x10300000 // second block of flash
#define FLASH1_SIZE     0x00040000 // 256KB
#define FLASH1_SECTOR   KB(8)


static const uint32_t maxim_flash_prog_blob[] = {
    0xE00ABE00, 
    0x444949a6, 0x68826808, 0x0f07f012, 0x68cad10b, 0x69486042, 0x6948b148, 0xf0126882, 0xbf040f07,
    0x604168c9, 0x2001d001, 0x20004770, 0x499b4770, 0x68084449, 0xf0226882, 0x60824270, 0xb1206948,
    0x68816948, 0x4170f021, 0x20006081, 0xb5104770, 0x444c4c92, 0xf0006820, 0xb928f8f4, 0xb1286960,
    0xf0006960, 0xb108f8ee, 0xbd102001, 0xbd102000, 0x4449498a, 0xb132698a, 0x4282698a, 0x688ad803,
    0x1a806949, 0x6809e000, 0xf012688a, 0xd1270f07, 0xb13a6a4a, 0x624a2200, 0xb11a6a4a, 0x624a2202,
    0xb9ea6a4a, 0xf022688a, 0xf0424270, 0x608a5200, 0xf422688a, 0xf442427f, 0x608a42aa, 0x68886008,
    0x0004f040, 0x68886088, 0x0f04f010, 0x6888d1fb, 0x4070f020, 0x6a486088, 0x0002f010, 0x4770bf08,
    0x47702001, 0x4c6db430, 0x69a3444c, 0x69a3b133, 0xd8034283, 0x1ac068a3, 0xe0006963, 0xf0126823,
    0xbf040f03, 0xc008f8d3, 0x0f07f01c, 0xf8d3d17d, 0xf1bcc024, 0xd0110f00, 0x0c00f04f, 0xc024f8c3,
    0xc024f8d3, 0x0f00f1bc, 0xf04fd008, 0xf8c30c02, 0xf8d3c024, 0xf1bcc024, 0xd17f0f00, 0xc008f8d3,
    0x4c70f02c, 0x5c00f04c, 0xc008f8c3, 0x6018e011, 0xcb04f852, 0xc030f8c3, 0xc008f8d3, 0x0c01f04c,
    0xc008f8c3, 0xc008f8d3, 0x0f01f01c, 0x1d00d1fa, 0x29041f09, 0xf010d302, 0xd1e80f1f, 0xc010f8d4,
    0x0f80f1bc, 0x2910d121, 0x6018d31f, 0xcb04f852, 0xc030f8c3, 0xcb04f852, 0xc034f8c3, 0xcb04f852,
    0xc038f8c3, 0xcb04f852, 0xc03cf8c3, 0xc008f8d3, 0x0c01f04c, 0xc008f8c3, 0xc008f8d3, 0x0f01f01c,
    0x3010d1fa, 0x29103910, 0x2904d2df, 0x6018d313, 0xcb04f852, 0xc030f8c3, 0xc008f8d3, 0x0c01f04c,
    0xc008f8c3, 0xc008f8d3, 0x0f01f01c, 0x1d00d1fa, 0x29041f09, 0xb1b9d2eb, 0xf04f6812, 0xf04f34ff,
    0xea620cff, 0x402c050c, 0xe017e000, 0x2c0cea4f, 0xd1f61e49, 0x631c6018, 0xf0406898, 0x60980001,
    0xf0106898, 0xd1fb0f01, 0xf0206898, 0x60984070, 0xf0106a58, 0xbf040002, 0x4770bc30, 0xbc302001,
    0x68814770, 0x0f07f011, 0x6a41d126, 0x2100b139, 0x6a416241, 0x2102b119, 0x6a416241, 0x6881b9e1,
    0x4170f021, 0x5100f041, 0x68816081, 0x417ff421, 0x412af441, 0x68816081, 0x0102f041, 0x68816081,
    0x0f02f011, 0x6881d1fb, 0x4170f021, 0x6a406081, 0x0002f010, 0x4770bf08, 0x47702001, 0x00000004,
    0x00000000, 
    FLC0_BASE, 	FLASH0_BASE,FLASH0_SIZE,CLOCK_DIV, 	BURST_SIZE, FLC1_BASE, 	FLASH1_BASE,FLASH1_SIZE
};

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const sector_info_t sectors_info[] = {
    {FLASH0_BASE, FLASH0_SECTOR},
    {FLASH1_BASE, FLASH1_SECTOR},
};

static const program_target_t flash = {
    0x20000005, // Init
    0x20000033, // UnInit
    0x20000053, // EraseChip
    0x20000075, // EraseSector
    0x200000e9, // ProgramPage
    0x0, // Verify

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x200002a4,
        0x20000500
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(maxim_flash_prog_blob),   // prog_blob size
    maxim_flash_prog_blob,           // address of prog_blob
    0x00004000       // ram_to_flash_bytes_to_be_written
};
