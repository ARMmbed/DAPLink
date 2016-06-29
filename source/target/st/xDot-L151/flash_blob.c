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

static const uint32_t output_flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x4603b510, 0xd0022a01, 0xd1222a02, 0xbf00e000, 0x68004874, 0x6070f440, 0x60204c72, 0x4c714872, 
    0x60203c0c, 0x60204871, 0x1d244871, 0x48716020, 0x486c6020, 0x68001d00, 0x1080f400, 0xf245b940, 
    0x4c6d5055, 0x20066020, 0xf6406060, 0x60a070ff, 0xbf00bf00, 0xbd102000, 0x29014601, 0x2902d002, 
    0xe000d10f, 0x485fbf00, 0x68003814, 0x0002f040, 0x3a144a5c, 0x46106010, 0xf0406800, 0x60100001, 
    0xbf00bf00, 0x47702000, 0x48564601, 0x68003814, 0x7000f440, 0x3a144a53, 0x46106010, 0xf0406800, 
    0x60100008, 0x60082000, 0xf64ae003, 0x4a5220aa, 0x484c6010, 0xf0006800, 0x28000001, 0x4849d1f5, 
    0x68003814, 0x7000f420, 0x3a144a46, 0x46106010, 0xf0206800, 0x60100008, 0x47702000, 0x20014603, 
    0xb5304770, 0x24804603, 0x00fff101, 0x01fff020, 0x3814483c, 0xf4406800, 0x4d3a6080, 0x60283d14, 
    0x68004628, 0x0008f040, 0xbf006028, 0x6810e005, 0x1d1b6018, 0x1f091d12, 0xb1091f24, 0xd1f62c00, 
    0xf64ae003, 0x4d3420aa, 0x482e6028, 0xf0006800, 0x28000001, 0x482bd1f5, 0xf4006800, 0xb1386070, 
    0x68004828, 0x6070f440, 0x60284d26, 0xbd302001, 0x38144824, 0xf4206800, 0x4d226080, 0x60283d14, 
    0x68004628, 0x0008f020, 0x46286028, 0xf4406800, 0x60286080, 0x68004628, 0x0008f040, 0x24806028, 
    0x6810e005, 0x1d1b6018, 0x1f091d12, 0xb1091f24, 0xd1f62c00, 0xf64ae003, 0x4d1720aa, 0x48116028, 
    0xf0006800, 0x28000001, 0x480ed1f5, 0xf4006800, 0xb1386070, 0x6800480b, 0x6070f440, 0x60284d09, 
    0xe7c42001, 0x38144807, 0xf4206800, 0x4d056080, 0x60283d14, 0x68004628, 0x0008f020, 0x20006028, 
    0x0000e7b5, 0x40023c18, 0x89abcdef, 0x02030405, 0x8c9daebf, 0x13141516, 0x40003000, 0x00000000, 
    
};

static const program_target_t flash = {
    0x20000021, // Init
    0x20000079, // UnInit
    0x200000A9, // EraseChip - hacky! was empty so I put in the EraseSector addr
    0x200000A9, // EraseSector
    0x20000103, // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000000 + 0x00000001,
        0x20000000 + 0x00000020 + 0x000001fc,
        0x20000800
    },

    0x20000000 + 0x00000A00,   // mem buffer location
    0x20000000,                // location to write prog_blob in target RAM
    sizeof(output_flash_prog_blob), // prog_blob size
    output_flash_prog_blob,         // address of prog_blob
    0x00000200                 // ram_to_flash_bytes_to_be_written
};
