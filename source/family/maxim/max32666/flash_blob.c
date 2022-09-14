/**
 * @file    flash_blob.c
 * @brief   Flash algorithm for the MAX32666
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
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

#define CLOCK_DIV       0x00000060
#define BURST_SIZE      0x00000080

#define FLC0_BASE       0x40029000
#define FLASH0_BASE     0x10000000 // 
#define FLASH0_SIZE     0x00080000 // 

#define FLC1_BASE       0x40029400 // second flash controller
#define FLASH1_BASE     0x10080000 // second block of flash
#define FLASH1_SIZE     0x00080000 // 

#define FLASH_SIZE      MB(1)
#define FLASH_SECTOR    KB(8)

#define FLASH_BASE     FLASH0_BASE

static const uint32_t maxim_flash_prog_blob[] = {
    0xE00ABE00, 
    0x444949a4, 0x68826808, 0x0f07f012, 0x68cad10b, 0x69486042, 0x6948b148, 0xf0126882, 0xbf040f07,
    0x604168c9, 0x2001d001, 0x20004770, 0x49994770, 0x68084449, 0xf0226882, 0x60824270, 0xb1206948,
    0x68816948, 0x4170f021, 0x20006081, 0xb5104770, 0x444c4c90, 0xf0006820, 0xb928f8ef, 0xb1286960,
    0xf0006960, 0xb108f8e9, 0xbd102001, 0xbd102000, 0x44494988, 0x4282698a, 0x6809bf88, 0x688ad802,
    0x1a806949, 0xf012688a, 0xd1270f07, 0xb13a6a4a, 0x624a2200, 0xb11a6a4a, 0x624a2202, 0xb9ea6a4a,
    0xf022688a, 0xf0424270, 0x608a5200, 0xf422688a, 0xf442427f, 0x608a42aa, 0x68886008, 0x0004f040,
    0x68886088, 0x0f04f010, 0x6888d1fb, 0x4070f020, 0x6a486088, 0x0002f010, 0x4770bf08, 0x47702001,
    0x4c6cb430, 0x69a3444c, 0xbf9d4283, 0x1ac068a3, 0x68236963, 0x0f03f012, 0xf8d3bf04, 0xf01cc008,
    0xd17d0f07, 0xc024f8d3, 0x0f00f1bc, 0xf04fd011, 0xf8c30c00, 0xf8d3c024, 0xf1bcc024, 0xd0080f00,
    0x0c02f04f, 0xc024f8c3, 0xc024f8d3, 0x0f00f1bc, 0xf8d3d17f, 0xf02cc008, 0xf04c4c70, 0xf8c35c00,
    0xe011c008, 0xf8526018, 0xf8c3cb04, 0xf8d3c030, 0xf04cc008, 0xf8c30c01, 0xf8d3c008, 0xf01cc008,
    0xd1fa0f01, 0x1f091d00, 0xd3022904, 0x0f1ff010, 0xf8d4d1e8, 0xf1bcc010, 0xd1210f80, 0xd31f2910,
    0xf8526018, 0xf8c3cb04, 0xf852c030, 0xf8c3cb04, 0xf852c034, 0xf8c3cb04, 0xf852c038, 0xf8c3cb04,
    0xf8d3c03c, 0xf04cc008, 0xf8c30c01, 0xf8d3c008, 0xf01cc008, 0xd1fa0f01, 0x39103010, 0xd2df2910,
    0xd3132904, 0xf8526018, 0xf8c3cb04, 0xf8d3c030, 0xf04cc008, 0xf8c30c01, 0xf8d3c008, 0xf01cc008,
    0xd1fa0f01, 0x1f091d00, 0xd2eb2904, 0x6812b1b9, 0x34fff04f, 0x0cfff04f, 0x050cea62, 0xe000402c,
    0xea4fe017, 0x1e492c0c, 0x6018d1f6, 0x6898631c, 0x0001f040, 0x68986098, 0x0f01f010, 0x6898d1fb,
    0x4070f020, 0x6a586098, 0x0002f010, 0xbc30bf04, 0x20014770, 0x4770bc30, 0xf0116881, 0xd1260f07,
    0xb1396a41, 0x62412100, 0xb1196a41, 0x62412102, 0xb9e16a41, 0xf0216881, 0xf0414170, 0x60815100,
    0xf4216881, 0xf441417f, 0x6081412a, 0xf0416881, 0x60810102, 0xf0116881, 0xd1fb0f02, 0xf0216881,
    0x60814170, 0xf0106a40, 0xbf080002, 0x20014770, 0x00004770, 0x00000004, 0x00000000,
    FLC0_BASE, 	FLASH0_BASE,FLASH0_SIZE,CLOCK_DIV, 	BURST_SIZE, FLC1_BASE, 	FLASH1_BASE,FLASH1_SIZE,
};

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const sector_info_t sectors_info[] = {
    FLASH0_BASE, FLASH_SECTOR,
};

static const program_target_t flash = {
    0x20000005, // Init
    0x20000033, // UnInit
    0x20000053, // EraseChip
    0x20000075, // EraseSector
    0x200000e5, // ProgramPage
    0x0, // Verify

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x2000029c,
        0x20000500
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(maxim_flash_prog_blob),   // prog_blob size
    maxim_flash_prog_blob,           // address of prog_blob
    0x00002000       // ram_to_flash_bytes_to_be_written
};
