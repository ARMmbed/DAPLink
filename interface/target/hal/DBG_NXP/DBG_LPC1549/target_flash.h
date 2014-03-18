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
    /*0x040*/ 0x47702000, 0x41f0e92d, 0x20324c3a, 0x2500444c, 0xe884263fL, 0xf1040061L, 0x4f370114, 0x46204688, 
    /*0x060*/ 0x696047b8, 0x2034b958, 0x61e884, 0x60e0f642, 0x464160e0, 0x47b84620, 0x28006960, 0x2001d000, 
    /*0x080*/ 0x81f0e8bdL, 0x41f0e92d, 0xb044d2a, 0x2032444d, 0x400e9c5, 0x114f105, 0x460f4e27, 0x60ac4628, 
    /*0x0A0*/ 0x696847b0, 0x2034b960, 0x400e9c5, 0x60e0f642, 0x4002e9c5, 0x46284639, 0x696847b0, 0xd0df2800L, 
    /*0x0C0*/ 0xe7dd2001L, 0x41f0e92d, 0x64614, 0xe9d4d10eL, 0x44080100, 0x1202e9d4, 0x44084411, 0x44086921, 
    /*0x0E0*/ 0x44086961, 0x440869a1, 0x61e04240, 0xb304d11, 0x2132444d, 0x1000e9c5, 0x114f105, 0x4f0e60a8, 
    /*0x100*/ 0x46284688, 0x696847b8, 0x2033b978, 0x600e9c5, 0x7080f44f, 0x4002e9c5, 0x60e0f642, 0x46416128, 
    /*0x120*/ 0x47b84628, 0x28006968, 0x2001d0aa, 0xe7a8, 0x40074080, 0x4, 0x3000205, 0x0, 
    /*0x140*/ 
};

static const TARGET_FLASH flash = {
    0x02000025, // Init
    0x02000041, // UnInit
    0x02000045, // EraseChip
    0x02000085, // EraseSector
    0x020000c5, // ProgramPage

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
