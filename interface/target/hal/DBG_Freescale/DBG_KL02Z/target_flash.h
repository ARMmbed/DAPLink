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

#define FLASH_SECTOR_SIZE                  1024

#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (0x400)

static uint8_t target_flash_init(uint32_t clk);
static uint8_t target_flash_uninit(void);
static uint8_t target_flash_erase_chip(void);
static uint8_t target_flash_erase_sector(uint32_t adr);
static uint8_t target_flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size);


static const uint32_t KL02Z_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    /*0x020*/ 0xb510492dL, 0x60084449, 0x2100482c, 0x482d6001, 0x44484a2b, 0x22016002, 0x3d26041, 0x22056082, 
    /*0x040*/ 0x61420692, 0x61814602, 0x70113220, 0x62411e49, 0xf901f000L, 0xd0002800L, 0xbd102001L, 0x47702000, 
    /*0x060*/ 0xb5084a21L, 0x349447a, 0xc0a9200, 0x481d4601, 0x44482300, 0xf937f000L, 0xd0002800L, 0xbd082001L, 
    /*0x080*/ 0x4919b510, 0x48174479, 0x44483920, 0xf891f000L, 0xd0002800L, 0xbd102001L, 0x4b13b510, 0x4601447b, 
    /*0x0A0*/ 0x22014810, 0x2923b38, 0xf0004448L, 0x2800f88b, 0x2001d000, 0xb538bd10L, 0x490b460c, 0x39584479, 
    /*0x0C0*/ 0x46019100, 0x46134807, 0x44484622, 0xf8c8f000L, 0xd0002800L, 0xbd382001L, 0x4, 0x40048100, 
    /*0x0E0*/ 0x40020000, 0x8, 0x85, 0x4604b570, 0x25006800, 0x61b7803, 0x2370d5fc, 0x20007003, 
    /*0x100*/ 0x3e03a, 0xf9e0f000L, 0xf0b070c, 0x1f1b1713, 0x2f2b2723, 0x68263633, 0x71f37813, 0x6826e02a, 
    /*0x120*/ 0x71b37853, 0x6826e026, 0x71737893, 0x6826e022, 0x713378d3, 0x6826e01e, 0x72f37913, 0x6826e01a, 
    /*0x140*/ 0x72b37953, 0x6826e016, 0x72737993, 0x6826e012, 0x723379d3, 0x6826e00e, 0x73f37a13, 0x6826e00a, 
    /*0x160*/ 0x73b37a53, 0x6826e006, 0x73737a93, 0x6826e002, 0x73337ad3, 0xb2c01c40L, 0xd9c24288L, 0x20806821, 
    /*0x180*/ 0xe0037008L, 0x1c416a60, 0x4780d000, 0x78006820, 0xd5f70600L, 0x78006820, 0xd5010681L, 0xe0062504L, 
    /*0x1A0*/ 0xd50106c1L, 0xe0022508L, 0xd00007c0L, 0x46282510, 0xb508bd70L, 0x2244460b, 0x700a4669, 0x2100466a, 
    /*0x1C0*/ 0xbd084798L, 0xb081b5ffL, 0x9a014615L, 0x68522600, 0x428a1948, 0x9b01d804L, 0x18d3689b, 0xd2104283L, 
    /*0x1E0*/ 0x68d29a01, 0xd804428aL, 0x691b9b01, 0x428318d3, 0x2602d201, 0x1a88e02c, 0x5c92101, 0x27001844, 
    /*0x200*/ 0x1a8ce002, 0x2bf2701, 0x46204639, 0xf8aff000L, 0xd0022900L, 0x360126ff, 0x4639e01a, 0xf0004628L, 
    /*0x220*/ 0x2900f8a6, 0x2601d012, 0x2009e012, 0x70084669, 0x70480c20, 0x70880a20, 0x9b0470ccL, 0x2103466a, 
    /*0x240*/ 0x47989801, 0xd1030006L, 0x19e41bed, 0xd1ec2d00L, 0xb0054630L, 0x4601bdf0, 0x61082000, 0x477061c8, 
    /*0x260*/ 0x4616b5fe, 0x461d4607, 0x198a2000, 0xd002078bL, 0x300120ff, 0x7b3bdfe, 0x2001d001, 0x687bbdfe, 
    /*0x280*/ 0xd803428bL, 0x191c68bc, 0xd20d4294L, 0x428b68fb, 0x693cd803, 0x4294191c, 0x2002d201, 0x2201bdfe, 
    /*0x2A0*/ 0x5d21ac9, 0xe01b188cL, 0xe0191accL, 0x46692006, 0xc207008, 0xa207048, 0x70cc7088, 0x710878e8, 
    /*0x2C0*/ 0x714878a8, 0x71887868, 0x71c87828, 0x466a9b08, 0x46382107, 0x28004798, 0x1d24d1e0, 0x1d2d1f36, 
    /*0x2E0*/ 0xd1e32e00L, 0xb5febdfeL, 0x46044615, 0xa86842, 0x461e1840, 0xd803428aL, 0x18d368a3, 0xd808428bL, 
    /*0x300*/ 0x428b68e3, 0x6927d803, 0x428b19db, 0x2002d801, 0x4282bdfe, 0x68a3d805, 0x428318d3, 0x1a8fd301, 
    /*0x320*/ 0x68e2e00a, 0xd9034282L, 0x18d36923, 0xd3ee4283L, 0x21011a88, 0x184705c9, 0x46382104, 0xf817f000L, 
    /*0x340*/ 0xd0022900L, 0x300120ff, 0x2001bdfe, 0x70084669, 0x70480c38, 0x70880a38, 0xa2870cf, 0x714d7108, 
    /*0x360*/ 0x9b08718eL, 0x2106466a, 0x47984620, 0x2200bdfe, 0x428b0903, 0xa03d32c, 0xd311428bL, 0x469c2300, 
    /*0x380*/ 0x4603e04e, 0xd43c430bL, 0x8432200, 0xd331428bL, 0x428b0903, 0xa03d31c, 0xd301428bL, 0xe03f4694L, 
    /*0x3A0*/ 0x428b09c3, 0x1cbd301, 0x41521ac0, 0x428b0983, 0x18bd301, 0x41521ac0, 0x428b0943, 0x14bd301, 
    /*0x3C0*/ 0x41521ac0, 0x428b0903, 0x10bd301, 0x41521ac0, 0x428b08c3, 0xcbd301, 0x41521ac0, 0x428b0883, 
    /*0x3E0*/ 0x8bd301, 0x41521ac0, 0x428b0843, 0x4bd301, 0x41521ac0, 0xd2001a41L, 0x41524601, 0x47704610, 
    /*0x400*/ 0xfcae05d, 0x4249d000, 0xd3001003L, 0x40534240, 0x469c2200, 0x428b0903, 0xa03d32d, 0xd312428bL, 
    /*0x420*/ 0x18922fc, 0xa03ba12, 0xd30c428bL, 0x11920189, 0xd308428bL, 0x11920189, 0xd304428bL, 0xd03a0189L, 
    /*0x440*/ 0xe0001192L, 0x9c30989, 0xd301428bL, 0x1ac001cb, 0x9834152, 0xd301428bL, 0x1ac0018b, 0x9434152, 
    /*0x460*/ 0xd301428bL, 0x1ac0014b, 0x9034152, 0xd301428bL, 0x1ac0010b, 0x8c34152, 0xd301428bL, 0x1ac000cb, 
    /*0x480*/ 0x8834152, 0xd301428bL, 0x1ac0008b, 0xd2d94152L, 0x428b0843, 0x4bd301, 0x41521ac0, 0xd2001a41L, 
    /*0x4A0*/ 0x46634601, 0x105b4152, 0xd3014610L, 0x2b004240, 0x4249d500, 0x46634770, 0xd300105bL, 0xb5014240L, 
    /*0x4C0*/ 0x46c02000, 0xbd0246c0L, 0x4674b430, 0x78251e64, 0x42ab1c64, 0x461dd200, 0x5b5d63, 0xbc3018e3L, 
    /*0x4E0*/ 0x4718, 0xfffffffeL, 0x0, 0x0, 
};

static const TARGET_FLASH flash = {
    0x20000021, // Init
    0x2000005D, // UnInit
    0x20000081, // EraseChip
    0x20000099, // EraseSector
    0x200000B7, // ProgramPage

    {0x20000001, 0x20000000+sizeof(KL02Z_FLM), 0x20000800}, // {breakpoint, const data (flashprg), stack_pointer}

    0x20000a00, // program_buffer, reserve 512 byte buffer for the flash to be written
    0x20000000, // algo_start
    sizeof(KL02Z_FLM), // algo_size
    KL02Z_FLM,  // image

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
