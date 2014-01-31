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

#define FLASH_SECTOR_SIZE                  (1024)

#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (0x400)

static uint8_t target_flash_init(uint32_t clk);
static uint8_t target_flash_uninit(void);
static uint8_t target_flash_erase_chip(void);
static uint8_t target_flash_erase_sector(uint32_t adr);
static uint8_t target_flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size);


static const uint32_t K20D50M_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    /* Init */
    /*0x020*/ 0xb510493aL, 0x60084449, 0xf24c4839L, 0x81c15120L, 0x1128f64d, 0x880181c1L, 0x101f021, 0x48368001,
    /*0x040*/ 0x44484934, 0x3200f44f, 0x21006001, 0x1201e9c0, 0x60c202d2, 0x52a0f04f, 0xf44f6142L, 0x61826200,
    /*0x060*/ 0x1020f880, 0x62411e49, 0xf93af000L, 0xd0002800L, 0xbd102001L, 0x47702000, 0x4a28b508, 0x9200447aL,
    /*0x080*/ 0x2cff3c1, 0x48244601, 0x44482300, 0xf9d3f000L, 0xd0002800L, 0xbd082001L, 0x4920b508, 0x481e4479,
    /*0x0A0*/ 0x44483920, 0xf8a1f000L, 0x481cb970, 0x4b1c4478, 0x447b3830, 0x48189000, 0xf2402204L, 0x4448410c,
    /*0x0C0*/ 0xf96cf000L, 0xd0002800L, 0xbd082001L, 0x4b13b510, 0x4601447b, 0x3b544810, 0x6280f44f, 0xf0004448L,
    /*0x0E0*/ 0x2800f8b8, 0x2001d000, 0xb538bd10L, 0x490b460c, 0x39744479, 0x46019100, 0x46134807, 0x44484622,
    /*0x100*/ 0xf94cf000L, 0xd0002800L, 0xbd382001L, 0x4, 0x40052000, 0x40020000, 0x8, 0xa5,
    /*0x120*/ 0x412, 0x4604b570, 0x25006800, 0x61b7803, 0x2370d5fc, 0x20007003, 0x280ce03a, 0xe8dfd236L,
    /*0x140*/ 0xa06f000, 0x1a16120e, 0x2a26221e, 0x6826322e, 0x71f37813, 0x6826e02a, 0x71b37853, 0x6826e026,
    /*0x160*/ 0x71737893, 0x6826e022, 0x713378d3, 0x6826e01e, 0x72f37913, 0x6826e01a, 0x72b37953, 0x6826e016,
    /*0x180*/ 0x72737993, 0x6826e012, 0x723379d3, 0x6826e00e, 0x73f37a13, 0x6826e00a, 0x73b37a53, 0x6826e006,
    /*0x1A0*/ 0x73737a93, 0x6826e002, 0x73337ad3, 0xb2c01c40L, 0xd9c24288L, 0x20806821, 0xe0037008L, 0x1c416a60,
    /*0x1C0*/ 0x4780d000, 0x78006820, 0xd5f70600L, 0x78006820, 0xd5010681L, 0xe0062504L, 0xd50106c1L, 0xe0022508L,
    /*0x1E0*/ 0xd00007c0L, 0x46282510, 0xb508bd70L, 0x460b2244, 0x2000f88d, 0x2100466a, 0xbd084798L, 0x4614b538,
    /*0x200*/ 0xd002078aL, 0x7080f44f, 0x6843bd38, 0xd803428bL, 0x441a6882, 0xd80c428aL, 0x428b68c3, 0x6902d803,
    /*0x220*/ 0x428a441a, 0x2002d801, 0x1ac9bd38, 0x100f501, 0x1ac9e000, 0xf88d2208L, 0xc0a2000, 0x2001f88d,
    /*0x240*/ 0xf88d0a0aL, 0xf88d2002L, 0x466a1003, 0x47a02103, 0xe92dbd38L, 0x460745f8, 0x46164698, 0x2000687b,
    /*0x260*/ 0xf44f198aL, 0x428b6580, 0x68bcd803, 0x4294441c, 0x68fbd20d, 0xd803428bL, 0x441c693c, 0xd2024294L,
    /*0x280*/ 0xe8bd2002L, 0x1acc85f8, 0x400f504, 0x1acce000, 0xf1f5fbb4L, 0x4111fb05, 0xf44fb111L, 0xe7f07080L,
    /*0x2A0*/ 0xf1f5fbb6L, 0x6111fb05, 0x2001b1a9, 0xf88de7e9L, 0xc20a000, 0x1f88d, 0xf88d0a20L, 0xf88d0002L,
    /*0x2C0*/ 0x466a4003, 0x46382103, 0x47984643, 0xd1d82800L, 0x442c1b76, 0xf04fe001L, 0x2e000a09, 0xe7d0d1e7L,
    /*0x2E0*/ 0x6801b5f0, 0x780a2400, 0xd5fc0612L, 0x700a2270, 0x21036802, 0x680171d1, 0x718d2580, 0x21006802,
    /*0x300*/ 0x68037151, 0x711a22fc, 0x73d16802, 0x70156802, 0x78136802, 0xd5fc061bL, 0x7a127a56, 0xc0ff002,
    /*0x320*/ 0x4200f44f, 0xf1bc1057L, 0xd2110f10L, 0xf00ce8dfL, 0xf0d0a08, 0x8080808, 0x80d1e0f, 0x8080808,
    /*0x340*/ 0xe0056102L, 0x42c0f44f, 0x6107e7fa, 0x6101e000, 0x20ff006, 0xd21b2a10L, 0xf002e8dfL, 0xd0b0b0b,
    /*0x360*/ 0x19161310, 0xb0b1e1c, 0xb0b0b0b, 0x5200f44f, 0x61c1e7e6, 0xf44fe00cL, 0xe7fa6100L, 0x6180f44f,
    /*0x380*/ 0xf44fe7f7L, 0xe7f47100L, 0x7180f44f, 0x61c5e7f1, 0xbdf04620L, 0xe7ec2140L, 0xe7ea2120L, 0x47fce92d,
    /*0x3A0*/ 0x46074616, 0x2000461d, 0xf8dd198aL, 0x78b8028, 0xf44fd003L, 0xe8bd7080L, 0x7b387fc, 0x2001d001,
    /*0x3C0*/ 0x687be7f9, 0xd803428bL, 0x441c68bc, 0xd20c4294L, 0x428b68fb, 0x693cd803, 0x4294441c, 0x2002d201,
    /*0x3E0*/ 0x1acce7e9, 0x400f504, 0x1acce000, 0xa06f04f, 0xd0e02e00L, 0xa000f88dL, 0xf88d0c20L, 0xa200001,
    /*0x400*/ 0x2f88d, 0x4003f88d, 0xf88d78e8L, 0x78a80004, 0x5f88d, 0xf88d7868L, 0x78280006, 0x7f88d,
    /*0x420*/ 0x2107466a, 0x46434638, 0x28004798, 0x1d24d1c3, 0x1d2d1f36, 0xe92de7dcL, 0x684641fc, 0xeb019d08L,
    /*0x440*/ 0x428e0482, 0x6887d803, 0x428f4437, 0x68c7d80a, 0xd804428fL, 0xc010f8d0L, 0x428f4467, 0x2002d802,
    /*0x460*/ 0x81fce8bdL, 0xd80542a6L, 0x44376887, 0xd30142a7L, 0xe0091b89L, 0x42a668c6, 0x6907d903, 0x42a74437,
    /*0x480*/ 0x1b89d3ed, 0x100f501, 0xfbb12404L, 0xfb04f6f4L, 0xb1141416L, 0x7080f44f, 0x2401e7e2, 0x4000f88d,
    /*0x4A0*/ 0xf88d0c0cL, 0xa0c4001, 0x4002f88d, 0x1003f88d, 0xf88d0a11L, 0xf88d1004L, 0xf88d2005L, 0x466a3006,
    /*0x4C0*/ 0x47a82106, 0xe7cc, 0xfffffffeL, 0x0, 0xffffffffL, 0xfffffffeL, 0x0, 0x0
};


static const TARGET_FLASH flash = {
    0x20000020, // Init
    0x20000074, // UnInit
    0x20000098, // EraseChip
    0x200000cc, // EraseSector
    0x200000ea, // ProgramPage

    {0x20000001, 0x200004e0, 0x20000c00}, // {breakpoint, static_base, stack_pointer}

    0x20001c00, // program_buffer
    0x20000000, // algo_start
    0x000004e0, // algo_size
    K20D50M_FLM,  // image

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
