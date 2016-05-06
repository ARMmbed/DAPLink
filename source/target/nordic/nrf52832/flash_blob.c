/**
 * @file    flash_blob.c
 * @brief   Flash algorithm for the nrf51
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

static const uint32_t nRF52832AA_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x47702000, 0x47702000, 0x4b3eb4f0, 0x60182002, 0x2001493d, 0x4c3d6008, 0x4e3e4d3d, 0xc0f8f8df, 
    0x68204a3e, 0x0f01f010, 0x2000d003, 0xbcf06018, 0x68284770, 0x00fff010, 0x6831bf1e, 0x46612900, 
    0xf010d0ef, 0xbf180f01, 0x1d09600a, 0xd1f80840, 0xb4f0e7e7, 0x21024b2b, 0xf1b06019, 0xbf262f10, 
    0x492f2001, 0x6008492f, 0x4d2a4c29, 0xc0a8f8df, 0x4e264a2a, 0xf0106820, 0xbf1e00ff, 0x29006829, 
    0xd0064661, 0x0f01f010, 0x600abf18, 0x08401d09, 0x6830d1f8, 0x0f01f010, 0x2000d0ec, 0xbcf06018, 
    0xe92d4770, 0xf8df0df0, 0x088db05c, 0xf8cb2101, 0x4f161000, 0x8058f8df, 0xa058f8df, 0x4c174e16, 
    0x1b04f852, 0x1b04f840, 0xf0116839, 0xd0070f01, 0xd1f51e6d, 0xf8cb2000, 0xe8bd0000, 0x47700df0, 
    0x1000f8d8, 0x01fff011, 0xf8dabf1e, 0x2b003000, 0xd0e94633, 0x0f01f011, 0x601cbf18, 0x08491d1b, 
    0xe7e1d1f8, 0x4001e504, 0x4001e50c, 0x4001e400, 0x40010404, 0x40010504, 0x40010600, 0x6e524635, 
    0x4001e514, 0x4001e508, 0x00000000, 
};

static const program_target_t flash = {
    .init = 0x20000021,
    .uninit = 0x20000025,
    .erase_chip = 0x20000029,
    .erase_sector = 0x20000073,
    .program_page = 0x200000C3,
    {
        .breakpoint = 0x20000001,
        .static_base = 0x20000020 + 0x00000128,
        .stack_pointer = 0x20000800
    },
    .program_buffer = 0x00000A00,
    .algo_start = 0x20000000,
    .algo_size = sizeof(nRF52832AA_FLM),
    .algo_blob = nRF52832AA_FLM,
    .program_buffer_size = 512 // should be USBD_MSC_BlockSize
};
