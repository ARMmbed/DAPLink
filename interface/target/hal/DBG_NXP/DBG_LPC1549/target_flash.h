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

#define FLASH_SECTOR_SIZE           (1024)  /* 1024 is assuming that this value is in number of uint32_t's */

#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (0x1000)

static uint8_t target_flash_init(uint32_t clk);
static uint8_t target_flash_uninit(void);
static uint8_t target_flash_erase_chip(void);
static uint8_t target_flash_erase_sector(uint32_t adr);
static uint8_t target_flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size);

static const uint32_t LPC1549_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    /*0x020*/ 0x47700b00, 0x21004842, 0x60416001, 0x64012101, 0x319bf24c, 0x21026441, 0x1c80f840, 0x47702000, 
    /*0x040*/ 0x47702000, 0x41f0e92d, 0x20324c3a, 0x2500444c, 0x500e9c4, 0x114f104, 0x460f4e37, 0x60a54620, 
    /*0x060*/ 0x696047b0, 0x2034b960, 0x500e9c4, 0x60e0f642, 0x5002e9c4, 0x46204639, 0x696047b0, 0xd0002800L, 
    /*0x080*/ 0xe8bd2001L, 0xe92d81f0L, 0x4d2a41f0, 0x444d0b04, 0xe9c52032L, 0xf1050400L, 0x4e270114, 0x4628460f, 
    /*0x0A0*/ 0x47b060ac, 0xb9606968L, 0xe9c52034L, 0xf6420400L, 0xe9c560e0L, 0x46394002, 0x47b04628, 0x28006968, 
    /*0x0C0*/ 0x2001d0df, 0xe92de7ddL, 0x461441f0, 0xd10e0006L, 0x100e9d4, 0xe9d44408L, 0x44111202, 0x69214408, 
    /*0x0E0*/ 0x69614408, 0x69a14408, 0x42404408, 0x4d1161e0, 0x444d0b30, 0xe9c52132L, 0xf1051000L, 0x60a80114, 
    /*0x100*/ 0x46884f0d, 0x47b84628, 0xb9786968L, 0xe9c52033L, 0xf44f0600L, 0xe9c57080L, 0xf6424002L, 0x612860e0, 
    /*0x120*/ 0x46284641, 0x696847b8, 0xd0aa2800L, 0xe7a82001L, 0x40074080, 0x4, 0x3000205, 0x0, 
    /*0x140*/ 
};

static const TARGET_FLASH flash = {
    0x02000025, // Init
    0x02000041, // UnInit
    0x02000045, // EraseChip
    0x02000087, // EraseSector
    0x020000c7, // ProgramPage

    // RSB : base address is address of Execution Region PrgData in map file
    //       to access global/static data
    // RSP : Initial stack pointer
    {0x02000001, 0x02000200, 0x02001000}, // {breakpoint, RSB, RSP}

    0x02000400, // program_buffer
    0x02000000, // algo_start
    0x00000140, // algo_size
    LPC1549_FLM, // image

    256          // ram_to_flash_bytes_to_be_written
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
