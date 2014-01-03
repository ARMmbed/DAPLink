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


static const uint32_t KL46Z_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    /*0x020*/ 0xb5104936L, 0x60084449, 0x21004835, 0x48366001, 0x44484a34, 0x22016002, 0x4926041, 0x2926082, 
    /*0x040*/ 0x220560c2, 0x61420692, 0x3122201, 0x46026182, 0x70113220, 0x62411e49, 0xf93bf000L, 0xd0002800L, 
    /*0x060*/ 0xbd102001L, 0x47702000, 0xb5084a28L, 0x349447a, 0xc0a9200, 0x48244601, 0x44482300, 0xf9c6f000L, 
    /*0x080*/ 0xd0002800L, 0xbd082001L, 0x4920b508, 0x481e4479, 0x44483920, 0xf8a3f000L, 0xd10d2800L, 0x4478481b, 
    /*0x0A0*/ 0x38324b1b, 0x9000447bL, 0x22044817, 0x44484919, 0xf969f000L, 0xd0002800L, 0xbd082001L, 0x4b13b510, 
    /*0x0C0*/ 0x4601447b, 0x22014810, 0x2923b54, 0xf0004448L, 0x2800f8b8, 0x2001d000, 0xb538bd10L, 0x490b460c, 
    /*0x0E0*/ 0x39744479, 0x46019100, 0x46134807, 0x44484622, 0xf949f000L, 0xd0002800L, 0xbd382001L, 0x4, 
    /*0x100*/ 0x40048100, 0x40020000, 0x8, 0xa9, 0x560, 0x40c, 0x4604b570, 0x25006800, 
    /*0x120*/ 0x61b7803, 0x2370d5fc, 0x20007003, 0x3e03a, 0xfa5df000L, 0xf0b070c, 0x1f1b1713, 0x2f2b2723, 
    /*0x140*/ 0x68263633, 0x71f37813, 0x6826e02a, 0x71b37853, 0x6826e026, 0x71737893, 0x6826e022, 0x713378d3, 
    /*0x160*/ 0x6826e01e, 0x72f37913, 0x6826e01a, 0x72b37953, 0x6826e016, 0x72737993, 0x6826e012, 0x723379d3, 
    /*0x180*/ 0x6826e00e, 0x73f37a13, 0x6826e00a, 0x73b37a53, 0x6826e006, 0x73737a93, 0x6826e002, 0x73337ad3, 
    /*0x1A0*/ 0xb2c01c40L, 0xd9c24288L, 0x20806821, 0xe0037008L, 0x1c416a60, 0x4780d000, 0x78006820, 0xd5f70600L, 
    /*0x1C0*/ 0x78006820, 0xd5010681L, 0xe0062504L, 0xd50106c1L, 0xe0022508L, 0xd00007c0L, 0x46282510, 0xb508bd70L, 
    /*0x1E0*/ 0x2244460b, 0x700a4669, 0x2100466a, 0xbd084798L, 0x4614b538, 0xd002078aL, 0x300120ff, 0x6842bd38, 
    /*0x200*/ 0xd803428aL, 0x18d36883, 0xd80d428bL, 0x428b68c3, 0x6902d803, 0x428a189a, 0x2002d801, 0x2201bd38, 
    /*0x220*/ 0x5d21ac9, 0xe0001889L, 0x22081a89, 0x701a466b, 0x705a0c0a, 0x709a0a0a, 0x466a70d9, 0x47a02103, 
    /*0x240*/ 0xb5ffbd38L, 0x4615b081, 0x27019a01, 0x26006852, 0x2bf1948, 0xd804428aL, 0x689b9b01, 0x428318d3, 
    /*0x260*/ 0x9a01d20fL, 0x428a68d2, 0x9b01d804L, 0x18d3691b, 0xd2014283L, 0xe0292602L, 0x21011a88, 0x184405c9, 
    /*0x280*/ 0x1a8ce000, 0x46204639, 0xf904f000L, 0xd0022900L, 0x360126ff, 0x4639e01a, 0xf0004628L, 0x2900f8fb, 
    /*0x2A0*/ 0x2601d012, 0x2009e012, 0x70084669, 0x70480c20, 0x70880a20, 0x9b0470ccL, 0x2103466a, 0x47989801, 
    /*0x2C0*/ 0xd1030006L, 0x19e41bed, 0xd1ec2d00L, 0xb0054630L, 0xb5f0bdf0L, 0x24006801, 0x612780a, 0x2270d5fc, 
    /*0x2E0*/ 0x6802700a, 0x71d12103, 0x22806801, 0x6803718a, 0x71592100, 0x23fc6805, 0x6803712b, 0x680373d9, 
    /*0x300*/ 0x6802701a, 0x61b7813, 0x7a55d5fc, 0x7177a12, 0xf3f2201, 0x10560412, 0xf000003bL, 0x910f968, 
    /*0x320*/ 0xb0d0b0b, 0xb0b0b0b, 0x90d0b0b, 0xe090b0b, 0xe0026102L, 0xe0006101L, 0x72a6106, 0x130f12, 
    /*0x340*/ 0xf955f000L, 0xc090910, 0x1815120f, 0x91f1d1b, 0x9090909, 0x61c10a09, 0xbdf04620L, 0x3092101, 
    /*0x360*/ 0x2101e7f9, 0xe7f602c9L, 0x2892101, 0x2101e7f3, 0xe7f00249L, 0x310121ff, 0x2180e7ed, 0x2140e7eb, 
    /*0x380*/ 0x2120e7e9, 0xb5fee7e7L, 0x46074616, 0x2000461d, 0x78b198a, 0x20ffd002, 0xbdfe3001L, 0xd00107b3L, 
    /*0x3A0*/ 0xbdfe2001L, 0x428b687b, 0x68bcd803, 0x4294191c, 0x68fbd20d, 0xd803428bL, 0x191c693c, 0xd2014294L, 
    /*0x3C0*/ 0xbdfe2002L, 0x1ac92201, 0x188c05d2, 0x1acce01b, 0x2006e019, 0x70084669, 0x70480c20, 0x70880a20, 
    /*0x3E0*/ 0x78e870cc, 0x78a87108, 0x78687148, 0x78287188, 0x9b0871c8L, 0x2107466a, 0x47984638, 0xd1e02800L, 
    /*0x400*/ 0x1f361d24, 0x2e001d2d, 0xbdfed1e3L, 0x4615b5fe, 0x68424604, 0x184000a8, 0x428a461e, 0x68a3d803, 
    /*0x420*/ 0x428b18d3, 0x68e3d808, 0xd803428bL, 0x19db6927, 0xd801428bL, 0xbdfe2002L, 0xd8054282L, 0x18d368a3, 
    /*0x440*/ 0xd3014283L, 0xe00a1a8fL, 0x428268e2, 0x6923d903, 0x428318d3, 0x1a88d3ee, 0x5c92101, 0x21041847, 
    /*0x460*/ 0xf0004638L, 0x2900f817, 0x20ffd002, 0xbdfe3001L, 0x46692001, 0xc387008, 0xa387048, 0x70cf7088, 
    /*0x480*/ 0x71080a28, 0x718e714d, 0x466a9b08, 0x46202106, 0xbdfe4798L, 0x9032200, 0xd32c428bL, 0x428b0a03, 
    /*0x4A0*/ 0x2300d311, 0xe04e469cL, 0x430b4603, 0x2200d43c, 0x428b0843, 0x903d331, 0xd31c428bL, 0x428b0a03, 
    /*0x4C0*/ 0x4694d301, 0x9c3e03f, 0xd301428bL, 0x1ac001cb, 0x9834152, 0xd301428bL, 0x1ac0018b, 0x9434152, 
    /*0x4E0*/ 0xd301428bL, 0x1ac0014b, 0x9034152, 0xd301428bL, 0x1ac0010b, 0x8c34152, 0xd301428bL, 0x1ac000cb, 
    /*0x500*/ 0x8834152, 0xd301428bL, 0x1ac0008b, 0x8434152, 0xd301428bL, 0x1ac0004b, 0x1a414152, 0x4601d200, 
    /*0x520*/ 0x46104152, 0xe05d4770L, 0xd0000fcaL, 0x10034249, 0x4240d300, 0x22004053, 0x903469c, 0xd32d428bL, 
    /*0x540*/ 0x428b0a03, 0x22fcd312, 0xba120189L, 0x428b0a03, 0x189d30c, 0x428b1192, 0x189d308, 0x428b1192, 
    /*0x560*/ 0x189d304, 0x1192d03a, 0x989e000, 0x428b09c3, 0x1cbd301, 0x41521ac0, 0x428b0983, 0x18bd301, 
    /*0x580*/ 0x41521ac0, 0x428b0943, 0x14bd301, 0x41521ac0, 0x428b0903, 0x10bd301, 0x41521ac0, 0x428b08c3, 
    /*0x5A0*/ 0xcbd301, 0x41521ac0, 0x428b0883, 0x8bd301, 0x41521ac0, 0x843d2d9, 0xd301428bL, 0x1ac0004b, 
    /*0x5C0*/ 0x1a414152, 0x4601d200, 0x41524663, 0x4610105b, 0x4240d301, 0xd5002b00L, 0x47704249, 0x105b4663, 
    /*0x5E0*/ 0x4240d300, 0x2000b501, 0x46c046c0, 0xb430bd02L, 0x1e644674, 0x1c647825, 0xd20042abL, 0x5d63461d, 
    /*0x600*/ 0x18e3005b, 0x4718bc30, 0xfffffffeL, 0x0, 0xffffffffL, 0xfffffffeL, 0x0, 0x0, 
    /*0x620*/ 
};


static const TARGET_FLASH flash = {
    0x20000020, // init
    0x20000064, // uninit
    0x20000088, // erase_chip
    0x200000bc, // erase_sector
    0x200000da, // program_page

    {0x20000001, 0x2000061C, 0x20001000}, // {breakpoint, const data (flashprg), stack_pointer}

    0x20002000, // program_buffer
    0x20000000, // algo_start
    0x0000061C, // algo_size
    KL46Z_FLM,  // image

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
