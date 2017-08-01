/**
 * @file    flash_blob.c
 * @brief   Flash algorithm for the TMPM066
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

static const uint32_t tmpm066_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x2200492e, 0x22b1700a, 0x492d710a, 0x60084449, 0x47702000, 0x47702000, 0x47702001, 0xb5104828,
    0x68014448, 0x22aa2005, 0x18080200, 0x24156502, 0x01e42355, 0x620b1909, 0x65042480, 0x620b6502,
    0x65012110, 0x6a01481f, 0xd0fc07c9, 0xbd102000, 0x491bb530, 0x444923aa, 0x2105680a, 0x18510209,
    0x2515650b, 0x01ed2455, 0x62141952, 0x650d2580, 0x6214650b, 0x60012130, 0x6a014812, 0xd0fc07c9,
    0xbd302000, 0x490eb530, 0x44492305, 0x24aa6809, 0x18cb021b, 0x2515651c, 0x01ed2455, 0x620c1949,
    0x651921a0, 0xca082100, 0x1c496003, 0xdbfa2920, 0x6a014804, 0xd0fc07c9, 0xbd302000, 0x400f2000,
    0x00000004, 0x41fff000, 0x00000000, 0x00000000
};

static const program_target_t flash = {
    0x20000021, // Init
    0x20000035, // UnInit
    0x2000003d, // EraseChip
    0x20000071, // EraseSector
    0x200000A5, // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000000 + 0x00000001,
        0x200000E8,
        0x20001000 - 0x20
    },
    0x20000000 + 0x00000200,   // mem buffer location
    0x20000000,                // location to write prog_blob in target RAM
    sizeof(tmpm066_blob),      // prog_blob size
    tmpm066_blob,              // address of prog_blob
    0x00000080                 // program_buffer_size, largest size that can be written in a single call to program page
};
