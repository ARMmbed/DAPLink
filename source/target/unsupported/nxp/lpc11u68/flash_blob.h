/**
 * @file    flash_blob.h
 * @brief   Flash algorithm for the lpc11u68
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

#ifndef TARGET_FLASH_H
#define TARGET_FLASH_H

#include "target_struct.h"
#include "swd_host.h"
#include "stdint.h"

static const uint32_t LPC11U68_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    /*0x020*/ 0x28180b00, 0x8c0d301, 0x47703015, 0x21004843, 0x22016301, 0x63416342, 0x6b416342, 0xd0fc07c9L,
    /*0x040*/ 0x493e6382, 0x39402002, 0x20006008, 0x20004770, 0xb5f84770L, 0x20324c3a, 0x2500444c, 0x4622261c,
    /*0x060*/ 0x4621c261, 0x4f373114, 0x91004620L, 0x696047b8, 0xd10a2800L, 0x46212034, 0x4833c161, 0x462060e0,
    /*0x080*/ 0x47b89900, 0x28006960, 0x2001d000, 0xb5f8bdf8L, 0xffc6f7ffL, 0x46044d2a, 0x2032444d, 0x4629606c,
    /*0x0A0*/ 0x311460ac, 0x4e276028, 0x4628460f, 0x696847b0, 0xd10b2800L, 0x2034606c, 0x602860ac, 0x60e84822,
    /*0x0C0*/ 0x46284639, 0x696847b0, 0xd0002800L, 0xbdf82001L, 0x4614b5f8, 0xd10e0006L, 0x68206861, 0x184068e2,
    /*0x0E0*/ 0x188968a1, 0x69211840, 0x69611840, 0x69a11840, 0x42401840, 0x463061e0, 0xff92f7ffL, 0x21324d10,
    /*0x100*/ 0x6068444d, 0x60a86029, 0x31144629, 0x46284f0d, 0x47b89100, 0x28006968, 0x606ed10e, 0x60ac2033,
    /*0x120*/ 0x20ff6028, 0x60e83001, 0x61284807, 0x99004628L, 0x696847b8, 0xd0002800L, 0xbdf82001L, 0x40048040,
    /*0x140*/ 0x4, 0x1fff1ff1, 0x2ee0, 0x0,
};

static const TARGET_FLASH flash = {
    0x1000002d, // Init
    0x1000004f, // UnInit
    0x10000053, // EraseChip
    0x1000008f, // EraseSector
    0x100000d1, // ProgramPage

    // RSB : base address is address of Execution Region PrgData in map file
    //       to access global/static data
    // RSP : Initial stack pointer
    {0x10000001, 0x10000200, 0x10001000}, // {breakpoint, RSB, RSP}

    0x10000400, // program_buffer
    0x10000000, // algo_start
    0x00000150, // algo_size
    LPC11U68_FLM, // image

    256          // ram_to_flash_bytes_to_be_written
};

#endif
