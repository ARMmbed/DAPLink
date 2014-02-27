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


static const uint32_t KL25Z_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    /* Init */
    /*0x020*/ 0xb510492fL, 0x60084449, 0x2100482e, 0x482f6001, 0x44484a2d, 0x22016002, 0x4926041, 0x2926082, 
    /*0x040*/ 0x220560c2, 0x61420692, 0x3122201, 0x46026182, 0x70113220, 0x62411e49, 0xf929f000L, 0xd0002800L, 
    /*0x060*/ 0xbd102001L, 0x47702000, 0xb5084a21L, 0x349447a, 0xc0a9200, 0x481d4601, 0x44482300, 0xf9b7f000L, 
    /*0x080*/ 0xd0002800L, 0xbd082001L, 0x4919b510, 0x48174479, 0x44483920, 0xf891f000L, 0xd0002800L, 0xbd102001L, 
    /*0x0A0*/ 0x4b13b510, 0x4601447b, 0x22014810, 0x2923b38, 0xf0004448L, 0x2800f8b4, 0x2001d000, 0xb538bd10L, 
    /*0x0C0*/ 0x490b460c, 0x39584479, 0x46019100, 0x46134807, 0x44484622, 0xf948f000L, 0xd0002800L, 0xbd382001L, 
    /*0x0E0*/ 0x4, 0x40048100, 0x40020000, 0x8, 0x85, 0x4604b570, 0x25006800, 0x61b7803, 
    /*0x100*/ 0x2370d5fc, 0x20007003, 0x3e03a, 0xfa60f000L, 0xf0b070c, 0x1f1b1713, 0x2f2b2723, 0x68263633, 
    /*0x120*/ 0x71f37813, 0x6826e02a, 0x71b37853, 0x6826e026, 0x71737893, 0x6826e022, 0x713378d3, 0x6826e01e, 
    /*0x140*/ 0x72f37913, 0x6826e01a, 0x72b37953, 0x6826e016, 0x72737993, 0x6826e012, 0x723379d3, 0x6826e00e, 
    /*0x160*/ 0x73f37a13, 0x6826e00a, 0x73b37a53, 0x6826e006, 0x73737a93, 0x6826e002, 0x73337ad3, 0xb2c01c40L, 
    /*0x180*/ 0xd9c24288L, 0x20806821, 0xe0037008L, 0x1c416a60, 0x4780d000, 0x78006820, 0xd5f70600L, 0x78006820, 
    /*0x1A0*/ 0xd5010681L, 0xe0062504L, 0xd50106c1L, 0xe0022508L, 0xd00007c0L, 0x46282510, 0xb508bd70L, 0x2244460b, 
    /*0x1C0*/ 0x700a4669, 0x2100466a, 0xbd084798L, 0x4614b538, 0xd002078aL, 0x300120ff, 0x6842bd38, 0xd803428aL, 
    /*0x1E0*/ 0x18d36883, 0xd80d428bL, 0x428b68c3, 0x6902d803, 0x428a189a, 0x2002d801, 0x2201bd38, 0x5d21ac9, 
    /*0x200*/ 0xe0001889L, 0x22081a89, 0x701a466b, 0x705a0c0a, 0x709a0a0a, 0x466a70d9, 0x47a02103, 0xb5ffbd38L, 
    /*0x220*/ 0x4615b081, 0x27019a01, 0x26006852, 0x2bf1948, 0xd804428aL, 0x689b9b01, 0x428318d3, 0x9a01d20fL, 
    /*0x240*/ 0x428a68d2, 0x9b01d804L, 0x18d3691b, 0xd2014283L, 0xe0292602L, 0x21011a88, 0x184405c9, 0x1a8ce000, 
    /*0x260*/ 0x46204639, 0xf907f000L, 0xd0022900L, 0x360126ff, 0x4639e01a, 0xf0004628L, 0x2900f8fe, 0x2601d012, 
    /*0x280*/ 0x2009e012, 0x70084669, 0x70480c20, 0x70880a20, 0x9b0470ccL, 0x2103466a, 0x47989801, 0xd1030006L, 
    /*0x2A0*/ 0x19e41bed, 0xd1ec2d00L, 0xb0054630L, 0xb5f0bdf0L, 0x24006801, 0x612780a, 0x2270d5fc, 0x6802700a, 
    /*0x2C0*/ 0x71d12103, 0x22806801, 0x6803718a, 0x71592100, 0x23fc6805, 0x6803712b, 0x680373d9, 0x6802701a, 
    /*0x2E0*/ 0x61b7813, 0x7a55d5fc, 0x7177a12, 0xf3f2201, 0x105603d2, 0xf000003bL, 0x910f96b, 0x9100e0b, 
    /*0x300*/ 0x10090909, 0x9090e1f, 0x11090909, 0xe0056102L, 0x3522203, 0x6106e7fa, 0x6101e000, 0xf12072a, 
    /*0x320*/ 0xf0000013L, 0xc10f955, 0x120f0c0c, 0x1d1b1815, 0xc0c0c1f, 0xd0c0c0c, 0x3522201, 0x61c1e7e6, 
    /*0x340*/ 0xbdf04620L, 0x2c92101, 0x2101e7f9, 0xe7f60289L, 0x2492101, 0x21ffe7f3, 0xe7f03101L, 0xe7ee2180L, 
    /*0x360*/ 0xe7ec2140L, 0xe7ea2120L, 0x4616b5fe, 0x461d4607, 0x198a2000, 0xd002078bL, 0x300120ff, 0x7b3bdfe, 
    /*0x380*/ 0x2001d001, 0x687bbdfe, 0xd803428bL, 0x191c68bc, 0xd20d4294L, 0x428b68fb, 0x693cd803, 0x4294191c, 
    /*0x3A0*/ 0x2002d201, 0x2201bdfe, 0x5d21ac9, 0xe01b188cL, 0xe0191accL, 0x46692006, 0xc207008, 0xa207048, 
    /*0x3C0*/ 0x70cc7088, 0x710878e8, 0x714878a8, 0x71887868, 0x71c87828, 0x466a9b08, 0x46382107, 0x28004798, 
    /*0x3E0*/ 0x1d24d1e0, 0x1d2d1f36, 0xd1e32e00L, 0xb5febdfeL, 0x46044615, 0xa86842, 0x461e1840, 0xd803428aL, 
    /*0x400*/ 0x18d368a3, 0xd808428bL, 0x428b68e3, 0x6927d803, 0x428b19db, 0x2002d801, 0x4282bdfe, 0x68a3d805, 
    /*0x420*/ 0x428318d3, 0x1a8fd301, 0x68e2e00a, 0xd9034282L, 0x18d36923, 0xd3ee4283L, 0x21011a88, 0x184705c9, 
    /*0x440*/ 0x46382104, 0xf817f000L, 0xd0022900L, 0x300120ff, 0x2001bdfe, 0x70084669, 0x70480c38, 0x70880a38, 
    /*0x460*/ 0xa2870cf, 0x714d7108, 0x9b08718eL, 0x2106466a, 0x47984620, 0x2200bdfe, 0x428b0903, 0xa03d32c, 
    /*0x480*/ 0xd311428bL, 0x469c2300, 0x4603e04e, 0xd43c430bL, 0x8432200, 0xd331428bL, 0x428b0903, 0xa03d31c, 
    /*0x4A0*/ 0xd301428bL, 0xe03f4694L, 0x428b09c3, 0x1cbd301, 0x41521ac0, 0x428b0983, 0x18bd301, 0x41521ac0, 
    /*0x4C0*/ 0x428b0943, 0x14bd301, 0x41521ac0, 0x428b0903, 0x10bd301, 0x41521ac0, 0x428b08c3, 0xcbd301, 
    /*0x4E0*/ 0x41521ac0, 0x428b0883, 0x8bd301, 0x41521ac0, 0x428b0843, 0x4bd301, 0x41521ac0, 0xd2001a41L, 
    /*0x500*/ 0x41524601, 0x47704610, 0xfcae05d, 0x4249d000, 0xd3001003L, 0x40534240, 0x469c2200, 0x428b0903, 
    /*0x520*/ 0xa03d32d, 0xd312428bL, 0x18922fc, 0xa03ba12, 0xd30c428bL, 0x11920189, 0xd308428bL, 0x11920189, 
    /*0x540*/ 0xd304428bL, 0xd03a0189L, 0xe0001192L, 0x9c30989, 0xd301428bL, 0x1ac001cb, 0x9834152, 0xd301428bL, 
    /*0x560*/ 0x1ac0018b, 0x9434152, 0xd301428bL, 0x1ac0014b, 0x9034152, 0xd301428bL, 0x1ac0010b, 0x8c34152, 
    /*0x580*/ 0xd301428bL, 0x1ac000cb, 0x8834152, 0xd301428bL, 0x1ac0008b, 0xd2d94152L, 0x428b0843, 0x4bd301, 
    /*0x5A0*/ 0x41521ac0, 0xd2001a41L, 0x46634601, 0x105b4152, 0xd3014610L, 0x2b004240, 0x4249d500, 0x46634770, 
    /*0x5C0*/ 0xd300105bL, 0xb5014240L, 0x46c02000, 0xbd0246c0L, 0x4674b430, 0x78251e64, 0x42ab1c64, 0x461dd200, 
    /*0x5E0*/ 0x5b5d63, 0xbc3018e3L, 0x4718, 0xfffffffeL 
};


static const TARGET_FLASH flash = {
    0x20000020, // init
    0x20000064, // uninit
    0x20000088, // erase_chip
    0x200000a0, // erase_sector
    0x200000be, // program_page

    {0x20000001, 0x200005ec, 0x20001000}, // {stackSize, breakpoint, RSB, RSP}

    0x20002000, // program_buffer
    0x20000000, // algo_start
    0x000005ec, // algo_size
    KL25Z_FLM,  // image

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
