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
#include "target_reset.h"
#include <stdint.h>
#include "system_SAM3U.h"
#include <debug_cm.h>
#include <RTL.h>

#define FLASH_SECTOR_SIZE                  (1024)
#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (1024)

static uint8_t target_flash_init(uint32_t clk);
static uint8_t target_flash_uninit(void);
static uint8_t target_flash_erase_chip(void);
static uint8_t target_flash_erase_sector(uint32_t adr);
static uint8_t target_flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size);


static const uint32_t nRF51822AA_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    /*0x020*/ 0x47702000, 0x47702000, 0x4c26b570, 0x60602002, 0x60e02001, 0x68284d24, 0xd00207c0L, 0x60602000, 
    /*0x040*/ 0xf000bd70L, 0xe7f6f82cL, 0x4c1eb570, 0x60612102, 0x4288491e, 0x2001d302, 0xe0006160L, 0x4d1a60a0, 
    /*0x060*/ 0xf81df000L, 0x7c06828, 0x2000d0fa, 0xbd706060L, 0x4605b5f8, 0x4813088e, 0x46142101, 0x4f126041, 
    /*0x080*/ 0xc501cc01L, 0x7c06838, 0x1e76d006, 0x480dd1f8, 0x60412100, 0xbdf84608L, 0xf801f000L, 0x480ce7f2, 
    /*0x0A0*/ 0x6006840, 0xd00b0e00L, 0x6849490a, 0xd0072900L, 0x4a0a4909, 0xd00007c3L, 0x1d09600a, 0xd1f90840L, 
    /*0x0C0*/ 0x4770, 0x4001e500, 0x4001e400, 0x10001000, 0x40010400, 0x40010500, 0x40010600, 0x6e524635, 
    /*0x0E0*/ 0x0, 
};

static const TARGET_FLASH flash = {
    0x20000021, // Init
    0x20000025, // UnInit
    0x20000029, // EraseChip
    0x20000049, // EraseSector
    0x20000071, // ProgramPage

    // RSB : base adreess is address of Execution Region PrgData in map file
    //       to access global/static data
    // RSP : Initial stack pointer
    {
      0x20000001,             // breakpoint location
      0x20000020+0x00000150,  // static_base
      0x20001000              // stack_pointer
    },
    0x20000200,               // program_buffer
    0x20000000,               // algo_start
    0x00000150,               // algo_size
    nRF51822AA_FLM,           // image
    512                       // ram_to_flash_bytes_to_be_written
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
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_sector, sector*FLASH_SECTOR_SIZE, 0, 0, 0)) {
        return 0;
    }

    return 1;
}

static uint8_t target_flash_erase_chip(void) {
    //
	// 1 == O.K.
	// 0 == Error
	//
   if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_chip, 0, 0, 0, 0)) {   // 1 == O.K., 0 == Error
        return 0;
    }
    
    target_set_state(RESET_PROGRAM);
    target_flash_init(SystemCoreClock);    
    
    return 1;
}

uint8_t target_flash_program_page(uint32_t addr, uint8_t * buf, uint32_t size)
{
    uint32_t bytes_written = 0;
	  // Program a page in target flash.
    if (!swd_write_memory(flash.program_buffer, buf, size)) {
        return 0;
    }
    while(bytes_written < size) {
        if (!swd_flash_syscall_exec(&flash.sys_call_param,
                                    flash.program_page,
                                    addr,                                     // arg1
                                    flash.ram_to_flash_bytes_to_be_written,   // arg2
                                    flash.program_buffer + bytes_written, 0)) { // arg3, arg4
            return 0;
        }
        bytes_written += flash.ram_to_flash_bytes_to_be_written;
        addr += flash.ram_to_flash_bytes_to_be_written;
    }

    return 1;
}


#endif
