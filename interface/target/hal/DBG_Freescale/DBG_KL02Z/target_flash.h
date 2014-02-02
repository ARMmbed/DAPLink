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
    0x4770ba40, 0x4770ba40, 0x4770ba40, 0x4770ba40, 0x4770ba40, 0x4770ba40, 0x4770ba40, 0x4770ba40, 
    0x4770ba40, 0x4770bac0, 0x4770bac0, 0x4770bac0, 0x4770bac0, 0x4770bac0, 0x4770bac0, 0x4770bac0, 
    0x4770bac0, 0x4770bac0, 0xb510492c, 0x60082000, 0x4448482b, 0xf8a6f000, 0xd0002800, 0xbd102001, 
    0x47702000, 0xb5104826, 0xf0004448, 0xb920f891, 0x21004823, 0xf0004448, 0x4922f925, 0xf44268ca, 
    0x60ca6280, 0xd0002800, 0xbd102001, 0x4c1cb570, 0x444c4605, 0x46204601, 0xf00068e2, 0xb928f84f, 
    0x46292300, 0x68e24620, 0xf8d6f000, 0x68ca4915, 0x6280f442, 0x280060ca, 0x2001d000, 0xb5febd70, 
    0x460b460c, 0x46014606, 0x4615480d, 0xf0004448, 0xb950f89b, 0x90002000, 0x90029001, 0x462b4808, 
    0x46314622, 0xf0004448, 0x4906f8f9, 0xf44268ca, 0x60ca6280, 0xd0002800, 0xbdfe2001, 0x40048100, 
    0x00000004, 0xf0003000, 0x2170480b, 0xb6727001, 0x70012180, 0x06097801, 0xb662d5fc, 0x06817800, 
    0x2067d501, 0x06c14770, 0x2068d501, 0x07c04770, 0x2069d0fc, 0x00004770, 0x40020000, 0x41f0e92d, 
    0x460d4614, 0xf846f000, 0xd11b2800, 0x1e64442c, 0xd00505a1, 0xeb012101, 0x1e422194, 0x2481eb02, 
    0x447e4e09, 0x8024f8df, 0x42a52709, 0x6830d80a, 0xf8886005, 0xf7ff7007, 0x2800ffc7, 0xf505d102, 
    0xe7f26580, 0x81f0e8bd, 0x00000206, 0x40020000, 0x4903b120, 0x71c82044, 0xbfb6f7ff, 0x47702004, 
    0x40020000, 0x4916b190, 0x4a166cc9, 0x6103f3c1, 0xf832447a, 0x03091011, 0x2200d00a, 0x2100e9c0, 
    0x60812101, 0x60c10289, 0x47704610, 0x47702004, 0x47702064, 0xea41b128, 0x079b0302, 0x2065d003, 
    0x20044770, 0x68034770, 0xd804428b, 0x44116840, 0x42884418, 0x2066d201, 0x20004770, 0x00004770, 
    0x40048000, 0x00000198, 0x47f0e92d, 0x0014461d, 0xd01a460e, 0xf7ff461a, 0x2800ffdd, 0x4f0cd116, 
    0xf8df447f, 0xf04fa030, 0x2d000806, 0x6838d00e, 0x68396006, 0x6048cc01, 0x8007f88a, 0xff64f7ff, 
    0xd1032800, 0x1f2d1d36, 0x2004e7ef, 0x87f0e8bd, 0x00000148, 0x40020000, 0x47f0e92d, 0x4614469a, 
    0x4605460e, 0xffb6f7ff, 0xd1252800, 0x0101e9d5, 0xf8f1fbb0, 0xf1c84271, 0x40010000, 0x42b5424d, 
    0x4445d100, 0x1bafb1bc, 0xd90042a7, 0x480b4627, 0x447808b9, 0x60066800, 0x22014809, 0x0a0a71c2, 
    0x728172c2, 0xa009f880, 0xff2ef7ff, 0xd1032800, 0x443e1be4, 0x2000e7e5, 0x87f0e8bd, 0x000000d6, 
    0x40020000, 0x4804b128, 0x71c22240, 0xf7ff7181, 0x2004bf1b, 0x00004770, 0x40020000, 0x4df0e92d, 
    0xe9dd001c, 0x46168709, 0xd025460d, 0xff72f7ff, 0xd11f2800, 0xb04cf8df, 0xf8df44fb, 0x2e00a04c, 
    0xf8dbd018, 0x600d1000, 0xf88a2202, 0x9a082007, 0x200bf88a, 0x0000f8db, 0x60816821, 0xfef4f7ff, 
    0xf1b8b160, 0xd0010f00, 0x5000f8c8, 0xd0012f00, 0x60392100, 0x8df0e8bd, 0xe7fb2004, 0x1d241f36, 
    0xe7dc1d2d, 0x00000070, 0x40020000, 0x00040002, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 
    0x00000000, 0x00000000, 0x00200000, 0x40020004, 0x00000000, 
};

static const TARGET_FLASH flash = {
    0x20000069, // Init
    0x20000081, // UnInit
    0x20000085, // EraseChip
    0x200000AD, // EraseSector
    0x200000DF, // ProgramPage

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
