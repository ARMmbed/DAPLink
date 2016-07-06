/**
* @file    flash_blob.c
* @brief   Flash algorithm for the stm32f411
 *
* DAPLink Interface Firmware
* Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License"); you may
* not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "flash_blob.h"

static const uint32_t stm32l151_flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0xbf182a01, 0xd11c2a02, 0x6801488a, 0x6170f441, 0x488a6001, 0x60014988, 0x60014989, 0x49891d00, 
    0x49896001, 0x48896001, 0xf4106800, 0xd1081f80, 0xf2454887, 0x60015155, 0x60412106, 0x71fff640, 
    0x20006081, 0x28014770, 0x2802bf18, 0x4881d108, 0xf0416801, 0x60010102, 0xf0416801, 0x60010101, 
    0x47702000, 0xc1ecf8df, 0x1000f8dc, 0x7100f441, 0x1000f8cc, 0x1000f8dc, 0x0108f041, 0x1000f8cc, 
    0x60012100, 0x6801486b, 0x0f01f011, 0xf64abf1c, 0x4a6f21aa, 0x6011d004, 0xf0136803, 0xd1fa0f01, 
    0x0000f8dc, 0x7000f420, 0x0000f8cc, 0x0000f8dc, 0x0008f020, 0x0000f8cc, 0x47702000, 0x495db470, 
    0x4c644b63, 0xf04f4e64, 0x25006000, 0x22aaf64a, 0xc000f8d4, 0x7c00f44c, 0xc000f8c4, 0xc000f8d4, 
    0x0c08f04c, 0xc000f8c4, 0xf8d16005, 0xf01cc000, 0xd0050f01, 0xf8d1601a, 0xf01cc000, 0xd1f90f01, 
    0xc000f8d4, 0x7c00f42c, 0xc000f8c4, 0xc000f8d4, 0x0c08f02c, 0xc000f8c4, 0x7080f500, 0xd3d742b0, 
    0x2000bc70, 0x20014770, 0xb4f04770, 0x31ff4e49, 0x01fff021, 0xf4436833, 0x60336380, 0xf0436833, 
    0x60330308, 0xc0ecf8df, 0x3000f8dc, 0xf0134c40, 0xf64a0f01, 0xd00525aa, 0xf8dc6025, 0xf0133000, 
    0xd1f90f01, 0xe0052380, 0x7b10f852, 0x7b04f840, 0x1f1b1f09, 0xbf182900, 0xd1f52b00, 0x3000f8dc, 
    0x0f01f013, 0x6025d005, 0x3000f8dc, 0x0f01f013, 0xf8dcd1f9, 0xf4133000, 0xd1366f70, 0xf4236833, 
    0x60336380, 0xf0236833, 0x60330308, 0xf4436833, 0x60336380, 0xf0436833, 0x60330308, 0x3000f8dc, 
    0x0f01f013, 0x6025d005, 0x3000f8dc, 0x0f01f013, 0x2380d1f9, 0xf852e005, 0xf8407b10, 0x1f097b04, 
    0x29001f1b, 0x2b00bf18, 0xf8dcd1f5, 0xf0100000, 0xd0050f01, 0xf8dc6025, 0xf0100000, 0xd1f90f01, 
    0x0000f8dc, 0x6f70f410, 0xf8dcd008, 0xf4400000, 0xf8cc6070, 0xbcf00000, 0x47702001, 0xf4206830, 
    0x60306080, 0xf0206830, 0x60300008, 0x2000bcf0, 0x00004770, 0x40023c18, 0x89abcdef, 0x40023c0c, 
    0x02030405, 0x8c9daebf, 0x13141516, 0x40023c1c, 0x40003000, 0x40023c04, 0x08040000, 0x00000000, 
    
};

static const program_target_t flash = {
    0x20000021, // Init
    0x20000067, // UnInit
    0x200000DD, // EraseChip
    0x20000085, // EraseSector
    0x2000014B, // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000000 + 0x00000001,
        0x20000000 + 0x00000020 + 0x0000025c,
        0x20000800
    },

    0x20000000 + 0x00000A00,   // mem buffer location
    0x20000000,                // location to write prog_blob in target RAM
    sizeof(stm32l151_flash_prog_blob), // prog_blob size
    stm32l151_flash_prog_blob,         // address of prog_blob
    0x00000200                 // ram_to_flash_bytes_to_be_written
};
