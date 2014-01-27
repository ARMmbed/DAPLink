/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
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
#ifndef TARGET_FLASH_H
#define TARGET_FLASH_H

#include "target_struct.h"
#include "swd_host.h"
#include <stdint.h>

#define FLASH_SECTOR_SIZE           (1024)

#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (0x400)

static uint8_t target_flash_init(uint32_t clk);
static uint8_t target_flash_uninit(void);
static uint8_t target_flash_erase_chip(void);
static uint8_t target_flash_erase_sector(uint32_t adr);
static uint8_t target_flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size);


static const uint32_t KL26Z_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    /*0x020*/ 0xb510492eL, 0x60084449, 0x2100482d, 0x482e6001, 0x44484a2c, 0x2100e9c0, 0x2280f44f, 0x2926082,
    /*0x040*/ 0xf04f60c2L, 0xe9c052a0L, 0xf8802105L, 0x1e491020, 0xf0006241L, 0x2800f937, 0x2001d000, 0x2000bd10,
    /*0x060*/ 0xb5084770L, 0x447a4a21, 0xf3c19200L, 0x460102cf, 0x2300481d, 0xf0004448L, 0x2800f9df, 0x2001d000,
    /*0x080*/ 0xb510bd08L, 0x44794919, 0x39204817, 0xf0004448L, 0x2800f89e, 0x2001d000, 0xb510bd10L, 0x447b4b13,
    /*0x0A0*/ 0x48114601, 0xf44f3b38L, 0x44486200, 0xf8c3f000L, 0xd0002800L, 0xbd102001L, 0x460cb538, 0x4479490b,
    /*0x0C0*/ 0x91003958L, 0x48084601, 0x46224613, 0xf0004448L, 0x2800f966, 0x2001d000, 0xbd38, 0x4,
    /*0x0E0*/ 0x40048100, 0x40020000, 0x8, 0x9f, 0x2480b51c, 0x1004f88d, 0x2005f88d, 0x4000f88d,
    /*0x100*/ 0x2105466a, 0xbd1c4798L, 0x4604b570, 0x25006800, 0x61b7803, 0x2370d5fc, 0x20007003, 0x280ce03a,
    /*0x120*/ 0xe8dfd236L, 0xa06f000, 0x1a16120e, 0x2a26221e, 0x6826322e, 0x71f37813, 0x6826e02a, 0x71b37853,
    /*0x140*/ 0x6826e026, 0x71737893, 0x6826e022, 0x713378d3, 0x6826e01e, 0x72f37913, 0x6826e01a, 0x72b37953,
    /*0x160*/ 0x6826e016, 0x72737993, 0x6826e012, 0x723379d3, 0x6826e00e, 0x73f37a13, 0x6826e00a, 0x73b37a53,
    /*0x180*/ 0x6826e006, 0x73737a93, 0x6826e002, 0x73337ad3, 0xb2c01c40L, 0xd9c24288L, 0x20806821, 0xe0037008L,
    /*0x1A0*/ 0x1c416a60, 0x4780d000, 0x78006820, 0xd5f70600L, 0x78006820, 0xd5010681L, 0xe0062504L, 0xd50106c1L,
    /*0x1C0*/ 0xe0022508L, 0xd00007c0L, 0x46282510, 0xb508bd70L, 0x460b2244, 0x2000f88d, 0x2100466a, 0xbd084798L,
    /*0x1E0*/ 0x4614b538, 0xd002078aL, 0x7080f44f, 0x6843bd38, 0xd803428bL, 0x441a6882, 0xd80c428aL, 0x428b68c3,
    /*0x200*/ 0x6902d803, 0x428a441a, 0x2002d801, 0x1ac9bd38, 0x100f501, 0x1ac9e000, 0xf88d2208L, 0xc0a2000,
    /*0x220*/ 0x2001f88d, 0xf88d0a0aL, 0xf88d2002L, 0x466a1003, 0x47a02103, 0xe92dbd38L, 0x460745f8, 0x46164698,
    /*0x240*/ 0x2000687b, 0xf44f198aL, 0x428b6500, 0x68bcd803, 0x4294441c, 0x68fbd20d, 0xd803428bL, 0x441c693c,
    /*0x260*/ 0xd2024294L, 0xe8bd2002L, 0x1acc85f8, 0x400f504, 0x1acce000, 0xf1f5fbb4L, 0x4111fb05, 0xf44fb111L,
    /*0x280*/ 0xe7f07080L, 0xf1f5fbb6L, 0x6111fb05, 0x2001b1a9, 0xf88de7e9L, 0xc20a000, 0x1f88d, 0xf88d0a20L,
    /*0x2A0*/ 0xf88d0002L, 0x466a4003, 0x46382103, 0x47984643, 0xd1d82800L, 0x442c1b76, 0xf04fe001L, 0x2e000a09,
    /*0x2C0*/ 0xe7d0d1e7L, 0x6801b5f0, 0x780a2400, 0xd5fc0612L, 0x700a2270, 0x21036802, 0x680171d1, 0x718d2580,
    /*0x2E0*/ 0x21006802, 0x68037151, 0x711a22fc, 0x73d16802, 0x70156802, 0x78136802, 0xd5fc061bL, 0x7a127a56,
    /*0x300*/ 0xc0ff002, 0x2280f44f, 0xf1bc1057L, 0xd2170f10L, 0xf00ce8dfL, 0xd0a0808, 0x8151310, 0x27240815,
    /*0x320*/ 0x808132a, 0xe00b6102L, 0x3270f44f, 0xf44fe7faL, 0xe7f73260L, 0x3240f44f, 0x6107e7f4, 0x6101e000,
    /*0x340*/ 0x20ff006, 0xd2242a10L, 0xf002e8dfL, 0x16131111, 0x221f1c19, 0x11112725, 0x11111111, 0x4280f44f,
    /*0x360*/ 0xf44fe7e0L, 0xe7dd4200L, 0x3280f44f, 0x61c1e7da, 0xf44fe00fL, 0xe7fa5180L, 0x6100f44f, 0xf44fe7f7L,
    /*0x380*/ 0xe7f46180L, 0x7100f44f, 0xf44fe7f1L, 0xe7ee7180L, 0x462061c5, 0x2140bdf0, 0x2120e7e9, 0xe92de7e7L,
    /*0x3A0*/ 0x461647fc, 0x461d4607, 0x198a2000, 0x8028f8ddL, 0xd003078bL, 0x7080f44f, 0x87fce8bdL, 0xd00107b3L,
    /*0x3C0*/ 0xe7f92001L, 0x428b687b, 0x68bcd803, 0x4294441c, 0x68fbd20c, 0xd803428bL, 0x441c693c, 0xd2014294L,
    /*0x3E0*/ 0xe7e92002L, 0xf5041accL, 0xe0000400L, 0xf04f1accL, 0x2e000a06, 0xf88dd0e0L, 0xc20a000, 0x1f88d,
    /*0x400*/ 0xf88d0a20L, 0xf88d0002L, 0x78e84003, 0x4f88d, 0xf88d78a8L, 0x78680005, 0x6f88d, 0xf88d7828L,
    /*0x420*/ 0x466a0007, 0x46382107, 0x47984643, 0xd1c32800L, 0x1f361d24, 0xe7dc1d2dL, 0x41fce92d, 0x9d086846L,
    /*0x440*/ 0x4c2eb01, 0xd803428eL, 0x44376887, 0xd80a428fL, 0x428f68c7, 0xf8d0d804L, 0x4467c010, 0xd802428fL,
    /*0x460*/ 0xe8bd2002L, 0x42a681fc, 0x6887d805, 0x42a74437, 0x1b89d301, 0x68c6e009, 0xd90342a6L, 0x44376907,
    /*0x480*/ 0xd3ed42a7L, 0xf5011b89L, 0x24080100, 0xf6f4fbb1L, 0x1416fb04, 0xf44fb114L, 0xe7e27080L, 0xf88d2401L,
    /*0x4A0*/ 0xc0c4000, 0x4001f88d, 0xf88d0a0cL, 0xf88d4002L, 0xa111003, 0x1004f88d, 0x2005f88d, 0x3006f88d,
    /*0x4C0*/ 0x2106466a, 0xe7cc47a8L, 0xfffffffeL, 0x0, 0x0,
};

static const TARGET_FLASH flash = {
    0x20000021, // Init
    0x2000005F, // UnInit
    0x20000083, // EraseChip
    0x2000009B, // EraseSector
    0x200000B9, // ProgramPage

    {0x20000001, 0x20000000+sizeof(KL26Z_FLM), 0x20001000}, // {stackSize, breakpoint, RSB, RSP}

    0x20002000, // program_buffer
    0x20000000, // algo_start
    sizeof(KL26Z_FLM), // algo_size
    KL26Z_FLM,  // image

    512,        // ram_to_flash_bytes_to_be_written
};


static uint8_t target_flash_init(uint32_t clk) {
    // Download flash programming algorithm to target and initialise.
    if (!swd_write_memory(flash.algo_start, (uint8_t *)flash.image, flash.algo_size)) {
        return 0;
    }

    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.init, 0, 0 /* clk value is not used */, 0, 0)) {
        return 0;
    }

    return 1;
}

static uint8_t target_flash_erase_sector(unsigned int sector) {
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_sector, sector * 0x400, 0, 0, 0)) {
        return 0;
    }

    return 1;
}

static uint8_t target_flash_erase_chip(void) {
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_chip, 0, 0, 0, 0)) {
        return 0;
    }

    return 1;
}

static uint8_t check_security_bits(uint32_t flashAddr, uint8_t *data) {
    uint16_t i = 0;
    // check to not write the security bit!!!!
    if (flashAddr == 0x400) {
        for (i = 0; i < 12; i++) {
            if (data[i] != 0xff) {
                return 0;
            }
        }
        if ((data[14] != 0xff) || (data[15] != 0xff))
            return 0;
    }
    return 1;
}

static uint8_t target_flash_program_page(uint32_t addr, uint8_t * buf, uint32_t size)
{
    uint32_t bytes_written = 0;

    // call a target dependent function to check if
    // we don't want to write specific bits (flash security bits, ...)
    if (!check_security_bits(addr, buf)) {
        return 0;
    }

    // Program a page in target flash.
    if (!swd_write_memory(flash.program_buffer, buf, size)) {
        return 0;
    }

    while(bytes_written < size) {
        if (!swd_flash_syscall_exec(&flash.sys_call_param,
                                    flash.program_page,
                                    addr,
                                    flash.ram_to_flash_bytes_to_be_written,
                                    flash.program_buffer + bytes_written, 0)) {
            return 0;
        }

        bytes_written += flash.ram_to_flash_bytes_to_be_written;
        addr += flash.ram_to_flash_bytes_to_be_written;
    }

    return 1;
}


#endif
