/**
 * @file    flash_blob.h
 * @brief   Flash algorithm for the atsam4e
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
#include "target_reset.h"
#include "stdint.h"
#include "system_SAM3U.h"
#include "debug_cm.h"
#include "RTL.h"



const uint32_t flash_algorithm_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x44494944, 0x200f6008, 0x02004943, 0x20016008, 0x03c04942, 0x4b436148, 0x62584841, 0x48424a41,
    0x62103ac0, 0x3880483f, 0x07c96a81, 0x6b11d0fc, 0x00890889, 0x63111c49, 0x07096a81, 0x2101d5fc,
    0x6a816311, 0xd5fc0709, 0x1c404835, 0x20006258, 0x28024770, 0x4830d115, 0x60414934, 0x07c96881,
    0x68c2d0fc, 0xd4050791, 0x31fe4930, 0x68816041, 0xd0fc07c9, 0xd5040751, 0x6041492d, 0x07c96881,
    0x2000d0fc, 0x49294770, 0x39084823, 0x68816041, 0xd0fc07c9, 0x47702000, 0x491f4a1e, 0x6812444a,
    0x0a401a80, 0x00800880, 0x04001c80, 0x481f0a02, 0x43101f00, 0x68886048, 0xd0fc07c0, 0x1f80481b,
    0x604a4302, 0x07c06888, 0x6888d0fc, 0x0f400700, 0x2001d000, 0xb5304770, 0x4b0f4c0e, 0x6824444c,
    0x1cc91b04, 0x08890a64, 0xe0020089, 0x1f09ca20, 0x2900c020, 0x0420d1fa, 0x0a00490c, 0x4308390c,
    0x68986058, 0xd0fc07c0, 0x07006898, 0xd0000f40, 0xbd302001, 0x00000004, 0x400e0a00, 0x400e1840,
    0x504d4300, 0x400e04c0, 0x01370f01, 0x5a00000d, 0x5a00020c, 0x00000000, 0x00000000,
};

static const TARGET_FLASH flash = {
    0x20000021, // Init
    0x20000073, // UnInit
    0x200000A7, // EraseChip
    0x200000B9, // EraseSector
    0x200000F7, // ProgramPage
    // breakpoint = RAM start + 1
    // RSB : base address is address of Execution Region PrgData in map file
    //       to access global/static data
    // RSP : Initial stack pointer
    {
        0x20000001, // breakpoint instruction address
        0x20000000 + 0x20 + 0x134,  // static base register value (image start + header + static base offset)
        0x20000800 // initial stack pointer
    },

    0x20000900, // flash_buffer, any valid RAM location with > 512 bytes of working room and proper alignment
    0x20000000, // algo_start, start of RAM
    sizeof(flash_algorithm_blob), // algo_size, size of array above
    flash_algorithm_blob, // image, flash algo instruction array
    512              // ram_to_flash_bytes_to_be_written
};


#endif
