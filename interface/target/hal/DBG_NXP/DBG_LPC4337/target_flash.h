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

static const uint32_t LPC4337_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    /*0x020*/ 0x2f00f5b0, 0xf5a0d301L, 0xf5b02000L, 0xd2013f80L, 0x47700b40, 0xeb012107L, 0x47704010, 0x20fab508, 
    /*0x040*/ 0x48829000, 0x4a826ec1, 0x4a824011, 0x66c14311, 0x4a816c41, 0x4a814011, 0xf0214311L, 0x64410101, 
    /*0x060*/ 0x1e419800, 0x28009100, 0x487dd1fa, 0x44482131, 0x21006001, 0x487b6041, 0x49796802, 0x44494878, 
    /*0x080*/ 0x44483114, 0x20004790, 0x2000bd08, 0xe92d4770L, 0x4c734ff8, 0x832f04f, 0x2700444c, 0xf8c4260eL, 
    /*0x0A0*/ 0xe9c48000L, 0xf04f7601L, 0x60e75582, 0x2100f8d5, 0x114f104, 0x91004620L, 0x69604790, 0xf440b118L, 
    /*0x0C0*/ 0xe8bd006aL, 0xf04f8ff8L, 0xf8c40b34L, 0xe9c4b000L, 0xf8df7601L, 0xe9c4a194L, 0xf8d5a703L, 0x48602100, 
    /*0x0E0*/ 0x44489900, 0x69604790, 0x4960b108, 0xe9c4e01fL, 0xf04f8700L, 0xe9c40801L, 0xf8d56802L, 0x48582100, 
    /*0x100*/ 0x44489900, 0x69604790, 0x4959b108, 0xf8c4e00fL, 0xe9c4b000L, 0xe9c47601L, 0xf8d5a803L, 0x48502100, 
    /*0x120*/ 0x44489900, 0x69604790, 0xd0ca2800L, 0x43084951, 0xe92de7c7L, 0x460547f0, 0xff72f7ffL, 0x46044e48, 
    /*0x140*/ 0x444e2032, 0x2800f44f, 0x400e9c6, 0x454560b4, 0x2001d301, 0x2000e000, 0x5782f04f, 0xf8d760f0L, 
    /*0x160*/ 0x493f2100, 0x4449483e, 0x468a3114, 0x47904448, 0xb1086970L, 0xe0154940L, 0xe9c62034L, 0x483a0400, 
    /*0x180*/ 0xe9c64545L, 0xd3014002L, 0xe0002001L, 0x61302000, 0x2100f8d7, 0x46514832, 0x47904448, 0x28006970, 
    /*0x1A0*/ 0x4936d001, 0xe8bd4308L, 0xe92d87f0L, 0x461447f0, 0xd10e0005L, 0x100e9d4, 0xe9d44408L, 0x44111202, 
    /*0x1C0*/ 0x69214408, 0x69614408, 0x69a14408, 0x42404408, 0x462861e0, 0xff24f7ffL, 0x21324e21, 0xf44f444eL, 
    /*0x1E0*/ 0xe9c62800L, 0x60b01000, 0xd3014545L, 0xe0002001L, 0xf04f2000L, 0x60f05782, 0x2100f8d7, 0x48184918, 
    /*0x200*/ 0x31144449, 0x4448468a, 0x69704790, 0x491cb108, 0x2033e019, 0x45456030, 0xf045d302L, 0xe00150d8L, 
    /*0x220*/ 0x50d0f045, 0x401e9c6, 0x7000f44f, 0x480e60f0, 0xf8d76130L, 0x480a2100, 0x44484651, 0x69704790, 
    /*0x240*/ 0xd0b02800L, 0x4308490f, 0xe7ad, 0x40050000, 0xe0fff7feL, 0x1000800, 0xe000cc3dL, 0x1070040, 
    /*0x260*/ 0x4, 0x10400100, 0x17700, 0xea1000, 0xea2000, 0xea3000, 0xea4000, 0xea5000, 
    /*0x280*/ 0xea6000, 0xea7000, 0x0, 
};

static const TARGET_FLASH flash = {
    0x1000003D, // Init
    0x1000008B, // UnInit
    0x1000008F, // EraseChip
    0x10000133, // EraseSector
    0x100001AB, // ProgramPage

    // static_base : base address is address of Execution Region PrgData in map file
    //               to access global/static data
    // stack_pointer : Initial stack pointer
    {0x10000001, 0x10000300, 0x10001000}, // {breakpoint, static_base, stack_pointer}

    0x10000500, // program_buffer
    0x10000000, // algo_start
    0x0000028c, // algo_size

/*
    0x10000000   ----------------------  algo_start
                 | FLASH ALGORITHM    |
    0x10000001   |                    |  breakpoint
    0x1000003d   |                    |  Init
    0x1000008b   |                    |  UnInit
    0x1000008f   |                    |  EraseChip
    0x10000133   |                    |  EraseSector
    0x100001ab   |                    |  ProgramPage
                 |                    |
                 ----------------------  algo_start + algo_size
                 |      unused        |
    0x10000300   ----------------------  static_base
                 | GLOBAL/STATIC DATA |
                 |                    |
    0x10000500   ----------------------  program_buffer
                 | DATA TO BE WRITTEN |
                 |                    |
                 ----------------------  program_buffer + ram_to_flash_bytes_to_be_written
                 |      unused        |
                 ----------------------
                 | SPACE FOR STACK    |
    0x10001000   ----------------------  stack_pointer
                 |      unused        |
                 ----------------------
                 | 32 BYTES FOR IAP   |
    0x10??????   ----------------------  CHIP MEMORY LIMIT
*/
  
    LPC4337_FLM, // image

    512          // ram_to_flash_bytes_to_be_written
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
