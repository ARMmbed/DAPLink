/* File Version : 4.1.0.517
 * Flash OS Routines (Automagically Generated)
 * Copyright (c) 2009-2015 ARM Limited
 *
 * Copyright (c) 2020 Cypress Semiconductor Corporation
 *
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

#if !defined(CYXX64X5_FLASH_BLOB_C)
#define CYXX64X5_FLASH_BLOB_C

#include "flash_blob.h"


// coverity[misra_c_2012_rule_8_9_violation]
static const uint32_t CYxx64x5_flash_prog_blob[] = {
    0xE00ABE00u, 0x062D780Du, 0x24084068u, 0xD3000040u, 0x1E644058u, 0x1C49D1FAu, 0x2A001E52u, 0x4770D1F2u,
    0xf000b510u, 0xf000f973u, 0xbd10f801u, 0xf000b510u, 0xf000f9a9u, 0x2000f9b6u, 0xb510bd10u, 0xf9a8f000u,
    0xbd102000u, 0xf000b510u, 0xbd10fac5u, 0x2401b570u, 0x07244d2au, 0x1b2812a6u, 0x42b01c40u, 0xd3054620u,
    0xfac3f000u, 0xd10a2800u, 0xe00619a4u, 0xffeaf7ffu, 0xd1042800u, 0x34ff34ffu, 0x42ac3402u, 0xbd70d9ebu,
    0xf000b510u, 0xbd10fabdu, 0x4605b570u, 0x2000460cu, 0x4628e008u, 0xfa9ef000u, 0xd1052800u, 0x1e6435ffu,
    0x350235ffu, 0xd1f42c00u, 0xb510bd70u, 0x48144604u, 0x02492101u, 0xf0004448u, 0x4911fb37u, 0x44494620u,
    0xfaadf000u, 0xb530bd10u, 0xe0042300u, 0x5cc45cd5u, 0xd10242a5u, 0x428b1c5bu, 0x18c0d3f8u, 0xb530bd30u,
    0x20004604u, 0xe0054603u, 0x42955ce5u, 0x2001d001u, 0x1c5bbd30u, 0xd3f7428bu, 0x0000bd30u, 0x1005ffffu,
    0x00000044u, 0xf7ffb510u, 0xbd10ff98u, 0xf7ffb510u, 0xbd10ff9du, 0xf7ffb510u, 0xbd10ff95u, 0x4611b510u,
    0xffaef7ffu, 0xb510bd10u, 0xffcdf7ffu, 0xb510bd10u, 0xffd5f7ffu, 0x0000bd10u, 0x68004827u, 0x0f000500u,
    0x2000d001u, 0x20014770u, 0xb5704770u, 0x444c4c23u, 0x28007820u, 0xf7ffd10bu, 0x2501ffefu, 0x72204068u,
    0xf0004620u, 0x2800fa0bu, 0x2000d004u, 0x481b7020u, 0xbd704448u, 0x26007860u, 0xd11d2801u, 0x280078a0u,
    0x2802d006u, 0x2805d009u, 0x280ed00cu, 0xe013d00fu, 0x28e278e0u, 0x7266d110u, 0x78e0e00eu, 0xd10b28e4u,
    0xe0082002u, 0x28e778e0u, 0x2005d106u, 0x78e0e003u, 0xd10128eau, 0x7260200eu, 0x28007a20u, 0xf000d005u,
    0x2800f9bbu, 0x72a5d001u, 0x72a6e000u, 0xe7cd2001u, 0x44484802u, 0x47707800u, 0x40200000u, 0x00000008u,
    0x4c61b570u, 0x444c4961u, 0x1e486925u, 0x60651828u, 0xfa9ef000u, 0x7020217du, 0x1e4800c9u, 0xf0001828u,
    0x60a0fa97u, 0x60e003c0u, 0xb570bd70u, 0x68204c58u, 0x48570701u, 0x008a0f09u, 0x18133880u, 0x07426c18u,
    0xd0050f52u, 0x2a014853u, 0x2a04d00fu, 0xe001d10du, 0xe00a4851u, 0x06c26818u, 0x0ed2207du, 0x2a100200u,
    0x2a11d003u, 0x2001d101u, 0x290003c0u, 0x2901d002u, 0xe04ad024u, 0x69114a49u, 0x689107cbu, 0x00890fdbu,
    0x29030f89u, 0x2500d005u, 0xd0152900u, 0xd0132901u, 0x2501e000u, 0x40192100u, 0xd0364329u, 0x038e6811u,
    0x0bb66851u, 0x681204c9u, 0x01d20cc9u, 0x1c6d0fd5u, 0xfa4ef000u, 0xe0254370u, 0xe7ec2101u, 0x31c04937u,
    0x07cb6809u, 0x0fdb4935u, 0x680a3180u, 0x0f920092u, 0xd0052a03u, 0x2a002500u, 0x2a01d026u, 0xe000d024u,
    0x22002501u, 0x432a401au, 0x680ad00fu, 0x680a0653u, 0x04d20e5bu, 0x0ed26809u, 0x0ecd02c9u, 0x46114358u,
    0xfa26f000u, 0xf0004629u, 0x6821fa23u, 0x06894c1eu, 0x40c80f89u, 0x4922444cu, 0x69096160u, 0x1c490e09u,
    0xfa16f000u, 0xbd7061a0u, 0xe7db2201u, 0x4c16b510u, 0x7860444cu, 0xd00a2800u, 0xff0ef7ffu, 0xd0022800u,
    0xf9b2f000u, 0xf000e001u, 0x2000f9c9u, 0xbd107060u, 0x4d0db570u, 0x46042601u, 0x03f6444du, 0x68e8e003u,
    0xf992f000u, 0x42b41ba4u, 0x68a8d8f9u, 0xf0004360u, 0xbd70f98bu, 0xb5104904u, 0x78094449u, 0xf0004348u,
    0xbd10f983u, 0xe7f5e7e4u, 0x00000014u, 0x000f4240u, 0x40260380u, 0x016e3600u, 0x007a1200u, 0x40260580u,
    0x40210000u, 0x688048adu, 0x444949adu, 0x47706008u, 0x688248aau, 0x444949aau, 0x42916809u, 0x6081d000u,
    0xb5104770u, 0xfed1f7ffu, 0x28007a00u, 0x2001d000u, 0x30102101u, 0x40814aa1u, 0x4ba16890u, 0x444b4208u,
    0xd1016018u, 0x60904308u, 0xb5f7bd10u, 0x499d460cu, 0x25000140u, 0x27011846u, 0x0fc169f0u, 0x43084620u,
    0x43884021u, 0xd1074078u, 0x1c6d9902u, 0xd203428du, 0xf7ff2001u, 0xe7efff9du, 0xbdfe4078u, 0x460eb570u,
    0x24004990u, 0x18450140u, 0x0fc06828u, 0x1c64d106u, 0xd20342b4u, 0xf7ff2001u, 0xe7f5ff8bu, 0x40482101u,
    0xb5f7bd70u, 0x46154f88u, 0x24004606u, 0x60286830u, 0x290a0f01u, 0x2100d004u, 0xd10342b8u, 0xe0022001u,
    0xe7f92101u, 0x43082000u, 0x9901d107u, 0x428c1c64u, 0x2001d203u, 0xff6cf7ffu, 0x2101e7e8u, 0xbdfe4048u,
    0xb081b5f3u, 0xf7ff4605u, 0x2800feb3u, 0xf7ffd003u, 0x7a00fe6cu, 0xf7ffe003u, 0x2101fe5fu, 0x28004048u,
    0x2801d003u, 0x2001d004u, 0x2700bdfeu, 0xe0024c6du, 0x27014c6cu, 0x217d3420u, 0x463800c9u, 0xffaef7ffu,
    0xd1f12800u, 0x17c607e8u, 0xd0011c76u, 0x444d4d67u, 0x200160e5u, 0x4a6660a0u, 0x46382100u, 0xff85f7ffu,
    0xd1e12800u, 0x31f521ffu, 0x2e009a02u, 0x4628d001u, 0x4620e001u, 0xf7ff300cu, 0xbdfeffa4u, 0xb085b5ffu,
    0x461d460fu, 0x20044616u, 0x990e9c10u, 0xff86f7ffu, 0x28006020u, 0x4669d11eu, 0x80089805u, 0x9503804fu,
    0x46684d50u, 0x35809602u, 0x202060e8u, 0x210060a8u, 0x9a0f2004u, 0xff59f7ffu, 0x28006020u, 0x68e8d10au,
    0x29006841u, 0x2101d001u, 0x28006021u, 0x6880d002u, 0xbdf0b009u, 0xe7fb2001u, 0x2002b51fu, 0x9000a903u,
    0x91022300u, 0x461a9001u, 0x20042106u, 0xffc6f7ffu, 0x28009803u, 0x2000d002u, 0xbd10b004u, 0xe7fb2001u,
    0x2400b57fu, 0x247d9403u, 0x4d39ae03u, 0x960200e4u, 0x94009501u, 0xffb2f7ffu, 0xbd70b004u, 0x4604b51cu,
    0x20014669u, 0xff6cf7ffu, 0xd1192800u, 0xa9019800u, 0x70600a00u, 0x70a09800u, 0x0c009800u, 0x20ff7160u,
    0xf7ff3002u, 0x2800ff5du, 0x9901d10au, 0x70e10a09u, 0x71219901u, 0x0f12030au, 0x71a20209u, 0x71e10f09u,
    0x4922bd1cu, 0x4823b508u, 0x60084449u, 0xf7ff4669u, 0xbd08ff47u, 0xb508491du, 0x44494602u, 0x6008481eu,
    0x4669604au, 0xff3cf7ffu, 0x4918bd08u, 0x4602b508u, 0x481a4449u, 0x604a6008u, 0xf7ff4669u, 0xbd08ff31u,
    0x4a12b538u, 0x444a4603u, 0x60104815u, 0x340724ffu, 0x60936054u, 0x466960d1u, 0xff22f7ffu, 0xb538bd38u,
    0x46034a0au, 0x480f444au, 0x24ff6010u, 0x60543407u, 0x60d16093u, 0xf7ff4669u, 0xbd38ff13u, 0x40221000u,
    0x00000004u, 0x40220000u, 0xf0000005u, 0x00000030u, 0x00003a98u, 0x0a000100u, 0x1c000100u, 0x14000100u,
    0x06000100u, 0x05000100u, 0x08801c80u, 0x1c40d003u, 0xd1fc1e80u, 0x4770bf00u, 0x8010f3efu, 0x4770b672u,
    0x8810f380u, 0x00004770u, 0xf7ffb510u, 0x4808fdbeu, 0x04006900u, 0x48070e01u, 0x44481c49u, 0xf0006800u,
    0x4905f847u, 0x60084449u, 0xfd9af7ffu, 0x0000bd10u, 0x40210000u, 0x0000002cu, 0x00000024u, 0xf7ffb510u,
    0x4808fda4u, 0x04006900u, 0x48070e01u, 0x44481c49u, 0xf0006800u, 0x4905f82du, 0x60084449u, 0xfd80f7ffu,
    0x0000bd10u, 0x40210080u, 0x00000028u, 0x00000024u, 0xc004e001u, 0x29041f09u, 0x078bd2fbu, 0x8002d501u,
    0x07c91c80u, 0x7002d000u, 0x29004770u, 0x07c3d00bu, 0x7002d002u, 0x1e491c40u, 0xd3042902u, 0xd5020783u,
    0x1c808002u, 0xe7e31e89u, 0xe7ee2200u, 0xe7df2200u, 0x09032200u, 0xd32c428bu, 0x428b0a03u, 0x2300d311u,
    0xe04e469cu, 0x430b4603u, 0x2200d43cu, 0x428b0843u, 0x0903d331u, 0xd31c428bu, 0x428b0a03u, 0x4694d301u,
    0x09c3e03fu, 0xd301428bu, 0x1ac001cbu, 0x09834152u, 0xd301428bu, 0x1ac0018bu, 0x09434152u, 0xd301428bu,
    0x1ac0014bu, 0x09034152u, 0xd301428bu, 0x1ac0010bu, 0x08c34152u, 0xd301428bu, 0x1ac000cbu, 0x08834152u,
    0xd301428bu, 0x1ac0008bu, 0x08434152u, 0xd301428bu, 0x1ac0004bu, 0x1a414152u, 0x4601d200u, 0x46104152u,
    0xe05d4770u, 0xd0000fcau, 0x10034249u, 0x4240d300u, 0x22004053u, 0x0903469cu, 0xd32d428bu, 0x428b0a03u,
    0x22fcd312u, 0xba120189u, 0x428b0a03u, 0x0189d30cu, 0x428b1192u, 0x0189d308u, 0x428b1192u, 0x0189d304u,
    0x1192d03au, 0x0989e000u, 0x428b09c3u, 0x01cbd301u, 0x41521ac0u, 0x428b0983u, 0x018bd301u, 0x41521ac0u,
    0x428b0943u, 0x014bd301u, 0x41521ac0u, 0x428b0903u, 0x010bd301u, 0x41521ac0u, 0x428b08c3u, 0x00cbd301u,
    0x41521ac0u, 0x428b0883u, 0x008bd301u, 0x41521ac0u, 0x0843d2d9u, 0xd301428bu, 0x1ac0004bu, 0x1a414152u,
    0x4601d200u, 0x41524663u, 0x4610105bu, 0x4240d301u, 0xd5002b00u, 0x47704249u, 0x105b4663u, 0x4240d300u,
    0x2000b501u, 0x46c046c0u, 0x0000bd02u, 0x00000000u, 0x00000001u, 0xffffff00u, 0xffffffffu, 0x0000ffffu,
    0x00000104u, 0x003d0900u, 0x00000fa0u, 0x07d00000u, 0x003d0900u, 0x007a1200u, 0x003d0900u
};

// coverity[misra_c_2012_rule_8_9_violation]
static const program_target_t CYxx64x5_flash_prog =
    {
        .init =         PSOC6_SRAM_START_ADDR + 0x00000021u, // Init
        .uninit =       PSOC6_SRAM_START_ADDR + 0x00000105u, // UnInit
        .erase_chip =   PSOC6_SRAM_START_ADDR + 0x0000010du, // EraseChip
        .erase_sector = PSOC6_SRAM_START_ADDR + 0x00000115u, // EraseSector
        .program_page = PSOC6_SRAM_START_ADDR + 0x0000011du, // ProgramPage
        .verify =       PSOC6_SRAM_START_ADDR + 0x00000127u, // Verify
        // BKPT : start of blob + 1
        // RSB  : blob start + header + rw data offset
        // RSP  : stack pointer
        .sys_call_s =
            {
                PSOC6_SRAM_START_ADDR + 0x00000001u,
                PSOC6_SRAM_START_ADDR + 0x0000088cu,
                PSOC6_SRAM_START_ADDR + 0x000014fcu
            },
        .program_buffer = PSOC6_SRAM_START_ADDR + 0x00001500u,      // mem buffer location
        .algo_start = PSOC6_SRAM_START_ADDR,                       // location to write prog_blob in target RAM
        .algo_size = sizeof(CYxx64x5_flash_prog_blob),  // prog_blob size
        .algo_blob = CYxx64x5_flash_prog_blob,          // address of prog_blob
        .program_buffer_size = 0x00000200u,              // ram_to_flash_bytes_to_be_written
        .algo_flags = (kAlgoVerifyReturnsAddress | kAlgoSingleInitType)
    };
#endif
