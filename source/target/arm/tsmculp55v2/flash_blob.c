/**
 * @file    flash_blob.c
 * @brief   Flash algorithm for the TSMCULP55V2
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2016-2016, ARM Limited, All Rights Reserved
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

static const uint32_t TSMCULP55V2_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0xf000b510, 0x2000f844, 0x2000bd10, 0xb5104770, 0xf0002000, 0x200af851, 0xf8f0f000, 0xf0002001, 
    0x2000f84b, 0xb510bd10, 0xf89af000, 0xbd102000, 0xf000b510, 0x2000f8a6, 0xf000bd10, 0xf000b8b9, 
    0x0000b8c8, 0x68004870, 0xf000496f, 0x3908000f, 0x29306809, 0x496cd106, 0x68091f09, 0xd10129b8, 
    0xd001280b, 0x47702001, 0x47702000, 0x6b404867, 0x001ff000, 0xd0042811, 0xd0052812, 0x30fff04f, 
    0xf44f4770, 0x47703000, 0x2080f44f, 0x49604770, 0x4449b500, 0x60082000, 0x4080f04f, 0xf7ff6048, 
    0x6088ffe5, 0x4000f04f, 0x485860c8, 0x61c14959, 0x07896981, 0xbd00d4fc, 0x4955b5f0, 0x44492208, 
    0xe9d12806, 0xe9d13502, 0x49504600, 0xe8dfd20c, 0x0c03f000, 0x37271e15, 0x07806988, 0x628cd4fc, 
    0x6988614a, 0xd1fc07c0, 0x6988bdf0, 0xd4fc0780, 0x614a628b, 0x07c06988, 0xbdf0d1fc, 0x07806988, 
    0x628ed4fc, 0x6988614a, 0xd1fc07c0, 0x6988bdf0, 0xd4fc0780, 0x614a628d, 0x07c06988, 0xbdf0d1fc, 
    0x07806988, 0x628cd4fc, 0x6988614a, 0xd1fc07c0, 0x07806988, 0x628bd4fc, 0x07c06988, 0xbdf0d1fc, 
    0x07806988, 0x628ed4fc, 0x6988614a, 0xd1fc07c0, 0x07806988, 0x628dd4fc, 0x07c06988, 0xbdf0d1fc, 
    0x6288492a, 0x61482004, 0x07c06988, 0x4770d1fc, 0xe9c24a26, 0x2001010a, 0x69906150, 0xd1fc07c0, 
    0xb5704770, 0x460e4614, 0x23004605, 0x18e1e00a, 0xf8d15ce0, 0xea401001, 0x46282101, 0xffe8f7ff, 
    0x1d2d1d1b, 0xd3f242b3, 0xbd702000, 0x47706800, 0x2300b530, 0x18d5e00a, 0xf8d55cd4, 0xea445001, 
    0x68042505, 0xd10342ac, 0x1d1b1d00, 0xd3f2428b, 0xb510bd30, 0x0613088c, 0x4302ea43, 0xea432100, 
    0x43132302, 0x6802e006, 0xd001429a, 0xbd102001, 0x1d091d00, 0xd3f642a1, 0xbd102000, 0xe0002100, 
    0x42811c49, 0x4770d3fc, 0x40009fe8, 0x40009000, 0x00000004, 0x00200b43, 0x00000000, 
};

static const program_target_t flash = {
    0x20000021, // Init
    0x2000002B, // UnInit
    0x2000002F, // EraseChip
    0x20000047, // EraseSector
    0x20000051, // ProgramPage
    
    // breakpoint = RAM start + 1
    // RSB : base address is address of Execution Region PrgData in map file
    //       to access global/static data
    // RSP : Initial stack pointer
    {
        0x20000001, // breakpoint instruction address
        0x20000000 + 0x20 + 0x218,  // static base register value (image start + header + static base offset)
        0x20000c00  // initial stack pointer
    },

    0x20001c00, // program_buffer, any valid RAM location with +512 bytes of headroom
    0x20000000, // algo_start, start of RAM
    sizeof(TSMCULP55V2_FLM), // algo_size, size of array above
    TSMCULP55V2_FLM,  // image, flash algo instruction array
    512        // ram_to_flash_bytes_to_be_written
};
