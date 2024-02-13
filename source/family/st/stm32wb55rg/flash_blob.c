/* Flash algorithm for STM32WB5x_M4_1024 Flash
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2023 Arm Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
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

// Generated from 'STM32WB5x_M4_1024.FLM' (STM32WB5x_M4_1024 Flash)
// Originating from 'Keil.STM32WBxx_DFP.1.3.0.pack'
// digest = 38c919a97147eb8c66aac4308cd7a3d9952f3d832795f0a36d7e960ad8630f85, file size = 16084
// algo version = 0x101, algo size = 648 (0x288)
static const uint32_t STM32WB5x_M4_1024_flash_prog_blob[] = {
    0xe7fdbe00,
    0x4770ba40, 0x4770bac0, 0x0030ea4f, 0x00004770, 0x204cf3c0, 0xf3bf4770, 0x47708f4f, 0x4892b672,
    0x29006941, 0x4991da03, 0x49916081, 0x6a006081, 0xd41003c0, 0xf64a488f, 0x600121aa, 0x60011049,
    0x60412106, 0x71fff640, 0x10c86081, 0x6008498a, 0x1f09207f, 0xb6626008, 0x47702000, 0x69414882,
    0x4100f041, 0x20006141, 0xb5104770, 0xf3c0b672, 0x487d224c, 0x03c96901, 0x6901d4fc, 0xd4fc0349,
    0x03096901, 0x6901d4fc, 0x34faf244, 0x61014321, 0xf6416941, 0x439973f8, 0x69416141, 0x0202f042,
    0x61414311, 0xf4416941, 0x61413180, 0x21aaf64a, 0xe0004a70, 0x69036011, 0xd4fb03db, 0xf0216941,
    0x61410102, 0x42216901, 0x6901d004, 0x61014321, 0xbd102001, 0x6100f04f, 0x1c496809, 0x6801d103,
    0x3180f441, 0xb6626001, 0xbd102000, 0xb672b530, 0x6a01485d, 0x05cb4a5f, 0x21aaf64a, 0xf890d510,
    0x495e0080, 0x6400f04f, 0x3500eb01, 0x4620e004, 0xffabf7ff, 0x5480f504, 0xd9f842ac, 0x6011e021,
    0x03db6903, 0x6903d4fb, 0x34faf244, 0x61034323, 0xf0436943, 0x61430304, 0xf4436943, 0x61433380,
    0x6011e000, 0x03db6903, 0x6941d4fb, 0x0104f021, 0x69016141, 0xd0044221, 0x43216901, 0x20016101,
    0xb662bd30, 0xbd302000, 0x4df0e92d, 0xb672b088, 0x2aaaf64a, 0xb0fcf8df, 0xe0014e3b, 0xa000f8cb,
    0x03db6933, 0xf244d4fa, 0x613737fa, 0x0e01f04f, 0xe014f8c6, 0xe05446ec, 0xe014f8c6, 0xd3132908,
    0x60036813, 0x60436853, 0x8f4ff3bf, 0x465b4654, 0x601ce000, 0x03ed6935, 0x6933d4fb, 0xd13b423b,
    0x39083008, 0xe03c3208, 0xe0042300, 0x4b01f812, 0x4023f84c, 0x428b1c5b, 0x2300d3f8, 0xf1c124ff,
    0xe0040508, 0x0801eb03, 0xf84c1c5b, 0x429d4028, 0xe9ddd8f8, 0xeb011300, 0x9b022103, 0x041b9c03,
    0x6304eb03, 0x91004419, 0x1304e9dd, 0x2103eb01, 0x9c079b06, 0xeb03041b, 0x44196304, 0x99009101,
    0x99016001, 0xf3bf6041, 0x21008f4f, 0x03db6933, 0x6933d4fc, 0xd004423b, 0x20016137, 0xe8bdb008,
    0x29008df0, 0x6970d1a8, 0x0001f020, 0xf04f6170, 0x68006000, 0xd0031c40, 0xf4206830, 0x60303080,
    0x2000b662, 0x0000e7ea, 0x58004000, 0x45670123, 0xcdef89ab, 0x40003000, 0x40002c04, 0x07fff000,
    0x00000000
};

// Start address of flash
static const uint32_t flash_start = 0x08000000;
// Size of flash
static const uint32_t flash_size = 0x00100000;

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const sector_info_t sectors_info[] = {
    {0x08000000, 0x00001000},
};

static const program_target_t flash = {
    0x20000021, // Init
    0x20000061, // UnInit
    0x200000f1, // EraseChip
    0x2000006f, // EraseSector
    0x2000016d, // ProgramPage
    0x00000000, // Verify

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x20000284,
        0x20000c00
    },

    // mem buffer location
    0x20000c00,
    // location to write prog_blob in target RAM
    0x20000000,
    // prog_blob size
    sizeof(STM32WB5x_M4_1024_flash_prog_blob),
    // address of prog_blob
    STM32WB5x_M4_1024_flash_prog_blob,
    // ram_to_flash_bytes_to_be_written
    0x00000400,
    // algo_flags (combination of kAlgoVerifyReturnsAddress, kAlgoSingleInitType and kAlgoSkipChipErase)
    0x00000000
};
