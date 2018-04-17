/* Flash OS Routines (Automagically Generated)
 * Copyright (c) 2009-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 // Device: xDot-L151
 
 #include "flash_blob.h"

static const uint32_t _flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x4603b510, 0xd0022a01, 0xd11d2a02, 0xbf00e000, 0x68004844, 0x6070f440, 0x60204c42, 0x4c414842,
    0x60203c0c, 0x60204841, 0x1d00483e, 0xf4106800, 0xd1081f80, 0x5055f245, 0x60204c3d, 0x60602006,
    0x70fff640, 0xbf0060a0, 0x2000bf00, 0x4601bd10, 0xd0022901, 0xd10a2902, 0xbf00e000, 0x38144831,
    0xf0406800, 0x4a2f0001, 0x60103a14, 0xbf00bf00, 0x47702000, 0xf44f4601, 0xf1017280, 0xf02000ff,
    0xe00301ff, 0x20aaf64a, 0x60184b29, 0x68004825, 0x0f01f010, 0xe00dd1f6, 0x60082000, 0xf64ae003,
    0x4b2320aa, 0x481f6018, 0xf0106800, 0xd1f60f01, 0x1f121d09, 0xd1ef2a00, 0x47702000, 0x20014603,
    0xb5304770, 0xf44f4603, 0xf1017480, 0xf02000ff, 0xe00301ff, 0x20aaf64a, 0x60284d15, 0x68004811,
    0x0f01f010, 0xe017d1f6, 0x3814480e, 0xf4206800, 0x4d0c7080, 0x60283d14, 0x60186810, 0xf64ae003,
    0x4d0b20aa, 0x48076028, 0xf0106800, 0xd1f60f01, 0x1d121d1b, 0x1f241f09, 0x2c00b109, 0x2000d1e4,
    0x0000bd30, 0x40023c18, 0x89abcdef, 0x02030405, 0x40003000, 0x00000000
};

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const sector_info_t sectors_info[] = {
    {0x08080000, 0x00000100},
};

static const program_target_t flash = {
    0x20000021, // Init
    0x2000006f, // UnInit
    0x12000001f, // EraseChip
    0x20000095, // EraseSector
    0x200000e3, // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x20000154,
        0x20000800
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(_flash_prog_blob),   // prog_blob size
    _flash_prog_blob,           // address of prog_blob
    0x00000100       // ram_to_flash_bytes_to_be_written
};
