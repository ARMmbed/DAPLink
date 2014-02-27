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

static const uint32_t LPC812_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    // flash algo begins here
    /* 0x20 */ 0x47700a80, 0x48454946, 0x60084449, 0x22004845, 0x21016302, 0x63426341, 0x63816341, 0x20024941,
    /* 0x40 */ 0x70083940, 0x47704610, 0x47702000, 0x4c3eb5f8, 0x444c2032, 0x260f2500, 0xc2614622L, 0x31144621,
    /* 0x60 */ 0x46204f3a, 0x47b89100, 0x28006960, 0x2034d10c, 0xc1614621L, 0x44484832, 0x60e06800, 0x99004620L,
    /* 0x80 */ 0x696047b8, 0xd0002800L, 0xbdf82001L, 0x4d2eb5f8, 0x444d0a84, 0x60282032, 0x46292000, 0x311460ac,
    /* 0xa0 */ 0x4e2a6068, 0x4628460f, 0x696847b0, 0xd10d2800L, 0x2034606c, 0x602860ac, 0x46394821, 0x68004448,
    /* 0xc0 */ 0x462860e8, 0x696847b0, 0xd0002800L, 0xbdf82001L, 0x6b5f8, 0xd10e4614L, 0x68206861, 0x184068e2,
    /* 0xe0 */ 0x188968a1, 0x69211840, 0x69611840, 0x69a11840, 0x42401840, 0x4d1461e0, 0x444d2132, 0x21006029,
    /* 0x100 */ 0xab06069, 0x462960a8, 0x4f103114, 0x91004628L, 0x696847b8, 0xd10f2800L, 0x2033606e, 0x602860ac,
    /* 0x120 */ 0x60e82040, 0x44484806, 0x61286800, 0x99004628L, 0x696847b8, 0xd0002800L, 0xbdf82001L, 0x2ee0,
    /* 0x140 */ 0x4, 0x40048040, 0x8, 0x1fff1ff1
};

static const TARGET_FLASH flash = {
    0x10000025, // init
    0x10000049, // uninit
    0x1000004d, // erase_chip
    0x1000008d, // erase_sector
    0x100000d1, // program_page

    {0x10000001, 0x10000300, 0x10001000}, // {breakpoint, RSB, RSP}

    0x10000400, // program_buffer
    0x10000000, // algo_start
    0x00000150, // algo_size
    LPC812_FLM, // image

    64          // ram_to_flash_bytes_to_be_written
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
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_sector, sector*0x400, 0, 0, 0)) {
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

static uint8_t target_flash_program_page(uint32_t addr, uint8_t * buf, uint32_t size)
{
    uint32_t bytes_written = 0;

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
