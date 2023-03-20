/**
 * @file    flash_blob.c
 * @brief   Flash algorithm for the nRF52 family
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2021, Arm Limited, All Rights Reserved
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

// This flash algo should work with all members of the nRF52 family
static const uint32_t nRF52832AA_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x47702000, 0x47702000, 0x4c2bb570, 0x60202002, 0x20014929, 0x60083108, 0x68284d28, 0xd00207c0,
    0x60202000, 0xf000bd70, 0xe7f6f833, 0x4c22b570, 0x60212102, 0x2f10f1b0, 0x491fd303, 0x31102001,
    0x491de001, 0x60081d09, 0xf0004d1c, 0x6828f821, 0xd0fa07c0, 0x60202000, 0xe92dbd70, 0xf8df41f0,
    0x088e8058, 0x46142101, 0xf8c84605, 0x4f131000, 0xc501cc01, 0x07c06838, 0x1e76d007, 0x2100d1f8,
    0x1000f8c8, 0xe8bd4608, 0xf00081f0, 0xe7f1f801, 0x6800480b, 0x00fff010, 0x490ad00c, 0x29006809,
    0x4908d008, 0x31fc4a08, 0xd00007c3, 0x1d09600a, 0xd1f90840, 0x00004770, 0x4001e504, 0x4001e400,
    0x40010404, 0x40010504, 0x6e524635, 0x00000000,
};

// Compiled from a fork of the pyocd/FlashAlgo repository (details in README):
// https://github.com/microbit-foundation/FlashAlgo/commit/03f07e6d635903e7272860cea72c84c078088a1a
// Compared to the nRF52832 version, this is about 25% faster using a new NVMC
// register (READYNEXT) only present in nRF52820, nRF52833, and nRF52840.
static const uint32_t nRF52833_flash_algo[] = {
    0xE00ABE00,
    0xf8d24a09, 0xf0133404, 0xd00b03ff, 0x2504f8d2, 0x4a06b142, 0x07d84906, 0x6011bf48, 0xf102085b,
    0xd1f80204, 0xbf004770, 0x40010000, 0x40010600, 0x6e524635, 0x47702000, 0x47702000, 0x4c09b510,
    0xf8c42302, 0x23013504, 0x350cf8c4, 0x3400f8d4, 0xd40207db, 0xffd4f7ff, 0x2000e7f8, 0x0504f8c4,
    0xbf00bd10, 0x4001e000, 0x4c0bb510, 0xf1b02302, 0xf8c42f10, 0xbf263504, 0xf8c42301, 0xf8c43514,
    0xf7ff0508, 0xf8d4ffbd, 0x07db3400, 0x2000d5f9, 0x0504f8c4, 0xbf00bd10, 0x4001e000, 0x4e0fb5f8,
    0x088d2301, 0x3504f8c6, 0x1a874614, 0x682219e3, 0x3404601a, 0x3408f8d6, 0xd40207da, 0xffa0f7ff,
    0x3d01e7f8, 0xf8d6d1f2, 0x07db3400, 0xf7ffd402, 0xe7f8ff97, 0xf8c62000, 0xbdf80504, 0x4001e000
};

/**
* List of start and size for each size of flash sector
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address start + size.
*/
static const sector_info_t sectors_info_nrf52[] = {
    {0, 4096},
 };

static const program_target_t flash_nrf52 = {
    .init = 0x20000021,
    .uninit = 0x20000025,
    .erase_chip = 0x20000029,
    .erase_sector = 0x2000004D,
    .program_page = 0x2000007B,
    .verify = 0x0,
    {
        .breakpoint = 0x20000001,
        .static_base = 0x20000020 + 0x00000150,
        .stack_pointer = 0x20001000
    },
    .program_buffer = 0x20000200,
    .algo_start = 0x20000000,
    .algo_size = 0x00000150,
    .algo_blob = nRF52832AA_FLM,
    .program_buffer_size = 512 // should be USBD_MSC_BlockSize
};

static const program_target_t flash_nrf52833 = {
    .init = 0x20000039,
    .uninit = 0x2000003d,
    .erase_chip = 0x20000041,
    .erase_sector = 0x2000006d,
    .program_page = 0x200000a1,
    .verify = 0x0,
    {
        .breakpoint = 0x20000001,
        .static_base = 0x200000e4,
        .stack_pointer = 0x20000300
    },
    .program_buffer = 0x20000000 + 0x00000A00,
    .algo_start = 0x20000000,
    sizeof(nRF52833_flash_algo),
    .algo_blob = nRF52833_flash_algo,
    .program_buffer_size = 512 // should be USBD_MSC_BlockSize
};
