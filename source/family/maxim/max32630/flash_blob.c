/**
 * @file    flash_blob.c
 * @brief   Flash algorithm for the MAX32630
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

#define FLC_BASE        0x40002000
#define CLOCK_DIV       0x00000060
#define BURST_SIZE      0x00000020
#define FLASH_BASE      0x00000000
#define FLASH_SIZE      MB(2)
#define FLASH_SECTOR    KB(8)

static const uint32_t flash_algo_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x44494986, 0x68826808, 0x0f07f012, 0x2001bf1c, 0x68494770, 0x20006041, 0x48804770, 0x68004448,
    0xf0216881, 0x60814170, 0x47702000, 0xb510487b, 0x68044448, 0xf8d6f000, 0x68a0b9b0, 0x407ff420,
    0x402af440, 0x68a060a0, 0x0002f040, 0x68a060a0, 0x0f02f010, 0x68a0d1fb, 0x4070f020, 0x6a6060a0,
    0x0002f010, 0xbd10bf08, 0xbd102001, 0x4605b570, 0x4448486a, 0xf0006804, 0xb9b8f8b5, 0xf42068a0,
    0xf440407f, 0x60a040aa, 0x68a06025, 0x0004f040, 0x68a060a0, 0x0f04f010, 0x68a0d1fb, 0x4070f020,
    0x6a6060a0, 0x0002f010, 0xbd70bf08, 0xbd702001, 0x41f0e92d, 0x8164f8df, 0x44c8460d, 0xf8d84606,
    0x46174000, 0x0f03f012, 0xf000d17e, 0x2800f88b, 0x68a0d17a, 0x6000f020, 0x68a060a0, 0x0010f040,
    0xe00d60a0, 0xf8576026, 0x63200b04, 0xf04068a0, 0x60a00001, 0xf01068a0, 0xd1fb0f01, 0x1d361f2d,
    0xd3022d04, 0x0f1ff016, 0xf8d8d1ec, 0x28800008, 0x2d10d122, 0x68a0d320, 0x0010f020, 0x68a060a0,
    0x6000f040, 0x602660a0, 0x0b04f857, 0xf8576320, 0x63600b04, 0x0b04f857, 0xf85763a0, 0x63e00b04,
    0xf04068a0, 0x60a00001, 0xf01068a0, 0xd1fb0f01, 0x36103d10, 0xd2e62d10, 0xd3172d04, 0xf02068a0,
    0x60a06000, 0xf04068a0, 0x60a00010, 0xf8576026, 0x63200b04, 0xf04068a0, 0x60a00001, 0xf01068a0,
    0xd1fb0f01, 0x1d361f2d, 0xd2ef2d04, 0x68a0b1dd, 0x6000f020, 0x68a060a0, 0x0010f040, 0x683a60a0,
    0x31fff04f, 0xea6220ff, 0x40190300, 0x1e6d0200, 0x6026d1f9, 0x68a06321, 0x0001f040, 0x68a060a0,
    0x0f01f010, 0xe000d1fb, 0x68a0e009, 0x4070f020, 0x6a6060a0, 0x0002f010, 0xe8bdbf08, 0x200181f0,
    0x81f0e8bd, 0x4448480d, 0x68816800, 0x0f07f011, 0x6a41d109, 0x2100b149, 0x6a416241, 0x2102b129,
    0x6a416241, 0x2001b109, 0x68814770, 0x4170f021, 0x5100f041, 0x20006081, 0x00004770, 0x00000004,
    0x00000000, FLC_BASE, CLOCK_DIV, BURST_SIZE,
};

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const sector_info_t sectors_info[] = {
    FLASH_BASE, FLASH_SECTOR,
};

static const program_target_t flash = {
    0x20000021, // Init
    0x2000003b, // UnInit
    0x2000004d, // EraseChip
    0x2000008d, // EraseSector
    0x200000d1, // ProgramPage
    0x00000000, // Verify

    {
        0x20000001, // BKPT : start of blob + 1
        0x20000240, // RSB  : blob start + header + rw data offset
        0x20000500  // RSP  : stack pointer
    },

    0x20000000 + 0x00000A00, // mem buffer location
    0x20000000,              // location to write prog_blob in target RAM
    sizeof(flash_algo_blob), // prog_blob size
    flash_algo_blob,         // address of prog_blob
    0x00000400               // ram_to_flash_bytes_to_be_written
};
