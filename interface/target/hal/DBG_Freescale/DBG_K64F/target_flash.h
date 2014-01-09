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

#define FLASH_SECTOR_SIZE                  (4096)

#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (0x400)

static uint8_t target_flash_init(uint32_t clk);
static uint8_t target_flash_uninit(void);
static uint8_t target_flash_erase_chip(void);
static uint8_t target_flash_erase_sector(uint32_t adr);
static uint8_t target_flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size);


static const uint32_t K64F_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    /*0x020*/ 0xb5104933L, 0x60084449, 0xf24c4832L, 0x81c15120L, 0x1128f64d, 0x880181c1L, 0x101f021, 0x482f8001,
    /*0x040*/ 0x4448492d, 0x1280f44f, 0x21006001, 0x1201e9c0, 0x52a0f04f, 0xf44f6142L, 0x61824280, 0x1020f880,
    /*0x060*/ 0x62411e49, 0xf92df000L, 0xd0002800L, 0xbd102001L, 0x47702000, 0x4a22b508, 0x9200447aL, 0x2cff3c1,
    /*0x080*/ 0x481e4601, 0x44482300, 0xf920f000L, 0xd0002800L, 0xbd082001L, 0x491ab510, 0x48184479, 0x44483920,
    /*0x0A0*/ 0xf893f000L, 0xd0002800L, 0xbd102001L, 0x4b14b510, 0x4601447b, 0x3b384811, 0x5280f44f, 0xf0004448L,
    /*0x0C0*/ 0x2800f8b8, 0x2001d000, 0xb508bd10L, 0x1dc94613, 0x207f021, 0x4479490a, 0x9100395eL, 0x48074601,
    /*0x0E0*/ 0xf0004448L, 0x2800f93b, 0x2001d000, 0xbd08, 0x4, 0x40052000, 0x40020000, 0x8,
    /*0x100*/ 0x89, 0x4604b570, 0x25006800, 0x61b7803, 0x2370d5fc, 0x20007003, 0x280ce03a, 0xe8dfd236L,
    /*0x120*/ 0xa06f000, 0x1a16120e, 0x2a26221e, 0x6826322e, 0x71f37813, 0x6826e02a, 0x71b37853, 0x6826e026,
    /*0x140*/ 0x71737893, 0x6826e022, 0x713378d3, 0x6826e01e, 0x72f37913, 0x6826e01a, 0x72b37953, 0x6826e016,
    /*0x160*/ 0x72737993, 0x6826e012, 0x723379d3, 0x6826e00e, 0x73f37a13, 0x6826e00a, 0x73b37a53, 0x6826e006,
    /*0x180*/ 0x73737a93, 0x6826e002, 0x73337ad3, 0xb2c01c40L, 0xd9c24288L, 0x20806821, 0xe0037008L, 0x1c416a60,
    /*0x1A0*/ 0x4780d000, 0x78006820, 0xd5f70600L, 0x78006820, 0xd5010681L, 0xe0062504L, 0xd50106c1L, 0xe0022508L,
    /*0x1C0*/ 0xd00007c0L, 0x46282510, 0xb508bd70L, 0x460b2244, 0x2000f88d, 0x2100466a, 0xbd084798L, 0x4614b538,
    /*0x1E0*/ 0xd002070aL, 0x7080f44f, 0x6843bd38, 0xd803428bL, 0x441a6882, 0xd80c428aL, 0x428b68c3, 0x6902d803,
    /*0x200*/ 0x428a441a, 0x2002d801, 0x1ac9bd38, 0x100f501, 0x1ac9e000, 0xf88d2208L, 0xc0a2000, 0x2001f88d,
    /*0x220*/ 0xf88d0a0aL, 0xf88d2002L, 0x466a1003, 0x47a02103, 0xe92dbd38L, 0x460745f8, 0x46164698, 0x2000687b,
    /*0x240*/ 0x428b198a, 0x68bcd803, 0x4294441c, 0x68fbd20e, 0xd803428bL, 0x441c693c, 0xd2024294L, 0xe8bd2002L,
    /*0x260*/ 0x1acc85f8, 0x400f504, 0xe0022500L, 0xf44f1accL, 0xfbb45580L, 0xfb05f1f5L, 0xb1114111L, 0x7080f44f,
    /*0x280*/ 0xfbb6e7edL, 0xfb05f1f5L, 0xb1a96111L, 0xe7e62001L, 0xa000f88dL, 0xf88d0c20L, 0xa200001, 0x2f88d,
    /*0x2A0*/ 0x4003f88d, 0x2103466a, 0x46434638, 0x28004798, 0x1b76d1d5, 0xe001442cL, 0xa09f04f, 0xd1e72e00L,
    /*0x2C0*/ 0x4601e7cd, 0x61082000, 0x477061c8, 0x41fce92d, 0x9d086846L, 0x1402eb01, 0xd803428eL, 0x44376887,
    /*0x2E0*/ 0xd80a428fL, 0x428f68c7, 0xf8d0d804L, 0x4467c010, 0xd802428fL, 0xe8bd2002L, 0x42a681fc, 0x6887d805,
    /*0x300*/ 0x42a74437, 0x1b89d301, 0x68c6e009, 0xd90342a6L, 0x44376907, 0xd3ed42a7L, 0xf5011b89L, 0x24100100,
    /*0x320*/ 0xf6f4fbb1L, 0x1416fb04, 0xf44fb114L, 0xe7e27080L, 0xf88d2401L, 0xc0c4000, 0x4001f88d, 0xf88d0a0cL,
    /*0x340*/ 0xf88d4002L, 0xa111003, 0x1004f88d, 0x2005f88d, 0x3006f88d, 0x2106466a, 0xe7cc47a8L, 0x43fee92d,
    /*0x360*/ 0x46074616, 0x2000461c, 0xf8dd198aL, 0x74b8028, 0xf44fd003L, 0xe8bd7080L, 0x77383fe, 0x2001d001,
    /*0x380*/ 0x687be7f9, 0xd803428bL, 0x441d68bd, 0xd20c4295L, 0x428b68fb, 0x693dd803, 0x4295441d, 0x2002d201,
    /*0x3A0*/ 0x1acde7e9, 0x500f505, 0x1acde02e, 0x2007e02c, 0xf88d, 0xf88d0c28L, 0xa280001, 0x2f88d,
    /*0x3C0*/ 0x5003f88d, 0xf88d78e0L, 0x78a00004, 0x5f88d, 0xf88d7860L, 0x78200006, 0x7f88d, 0xf88d79e0L,
    /*0x3E0*/ 0x79a00008, 0x9f88d, 0xf88d7960L, 0x7920000a, 0xbf88d, 0x210b466a, 0x46434638, 0x28004798,
    /*0x400*/ 0x3508d1b9, 0x34083e08, 0xd1d02e00L, 0xe7b3, 0xfffffffeL, 0x0, 0x0,
};


static const TARGET_FLASH flash = {
    0x20000021, // Init
    0x20000071, // UnInit
    0x20000095, // EraseChip
    0x200000AD, // EraseSector
    0x200000CB, // ProgramPage

    {0x20000001, 0x20000418, 0x20001000}, // {breakpoint, static_base, stack_pointer}

    0x20003000, // program_buffer
    0x20000000, // algo_start
    0x00000418, // algo_size
    K64F_FLM,  // image

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
