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

#define TARGET_AUTO_INCREMENT_PAGE_SIZE    0x1000

static uint8_t target_flash_init(uint32_t clk);
static uint8_t target_flash_uninit(void);
static uint8_t target_flash_erase_chip(void);
static uint8_t target_flash_erase_sector(uint32_t adr);
static uint8_t target_flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size);

static const uint32_t LPC1114_FLM[] = {
    /*0x000*/ 0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2, 
    /*0x020*/ 0x47700b00, 0x21004841, 0x22016301, 0x63416342, 0x6b416342, 0xd0fc07c9, 0x493c6382, 0x39402002, 
    /*0x040*/ 0x20006008, 0x20004770, 0xb5f84770, 0x20324c38, 0x2500444c, 0x46222607, 0x4621c261, 0x4f353114, 
    /*0x060*/ 0x91004620, 0x696047b8, 0xd10a2800, 0x46212034, 0x4831c161, 0x462060e0, 0x47b89900, 0x28006960, 
    /*0x080*/ 0x2001d000, 0xb5f8bdf8, 0x0b044d29, 0x2032444d, 0x4629606c, 0x311460ac, 0x4e266028, 0x4628460f, 
    /*0x0A0*/ 0x696847b0, 0xd10b2800, 0x2034606c, 0x602860ac, 0x60e84821, 0x46284639, 0x696847b0, 0xd0002800, 
    /*0x0C0*/ 0xbdf82001, 0x4614b5f8, 0xd10e0006, 0x68206861, 0x184068e2, 0x188968a1, 0x69211840, 0x69611840, 
    /*0x0E0*/ 0x69a11840, 0x42401840, 0x4d1161e0, 0x444d0b30, 0x60682132, 0x60a86029, 0x31144629, 0x46284f0d, 
    /*0x100*/ 0x47b89100, 0x28006968, 0x606ed10e, 0x60ac2033, 0x20ff6028, 0x60e83001, 0x61284807, 0x99004628, 
    /*0x120*/ 0x696847b8, 0xd0002800, 0xbdf82001, 0x40048040, 0x00000004, 0x1fff1ff1, 0x00002ee0, 0x00000000, 
};

static const TARGET_FLASH flash = {
    0x10000025, // Init
    0x10000047, // UnInit
    0x1000004B, // EraseChip
    0x10000087, // EraseSector
    0x100000C5, // ProgramPage

    // RSB : base adreess is address of Execution Region PrgData in map file
    //       to access global/static data
    // RSP : Initial stack pointer
    {0x10000001, 0x10000020+0x00000120, 0x10001000-32}, // {breakpoint, RSB, RSP}

    0x10000200, // program_buffer
    0x10000000, // algo_start
    0x00000150, // algo_size
    LPC1114_FLM,// image
    256         // ram_to_flash_bytes_to_be_written
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
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_sector, sector*0x1000, 0, 0, 0)) {
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
