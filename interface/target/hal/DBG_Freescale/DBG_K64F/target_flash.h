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

#define FLASH_SECTOR_SIZE                  (4096)

#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (0x400)

static uint8_t target_flash_init(uint32_t clk);
static uint8_t target_flash_uninit(void);
static uint8_t target_flash_erase_chip(void);
static uint8_t target_flash_erase_sector(uint32_t adr);
static uint8_t target_flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size);


static const uint32_t K64F_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    /*0x020*/ 0x4770ba40, 0x4770ba40, 0x4770ba40, 0x4770ba40, 0x4770ba40, 0x4770ba40, 0x4770ba40, 0x4770ba40, 
    /*0x040*/ 0x4770ba40, 0x4770ba40, 0x4770ba40, 0x4770ba40, 0x4770ba40, 0x4770ba40, 0x4770bac0, 0x4770bac0, 
    /*0x060*/ 0x4770bac0, 0x4770bac0, 0x4770bac0, 0x4770bac0, 0x4770bac0, 0x4770bac0, 0x4770bac0, 0x4770bac0, 
    /*0x080*/ 0x4770bac0, 0x4770bac0, 0x4770bac0, 0x4770bac0, 0xb510492dL, 0x60084449, 0xf24c482cL, 0x81c15120L, 
    /*0x0A0*/ 0x1128f64d, 0x880181c1L, 0x101f021, 0x48288001, 0xf0004448L, 0x2800f8d5, 0x2001d000, 0x2000bd10, 
    /*0x0C0*/ 0x460a4770, 0x48224601, 0x2300b510, 0xf0004448L, 0x2800f9c1, 0x2001d000, 0xb57fbd10L, 0x4605460c, 
    /*0x0E0*/ 0xa9032000L, 0x100e9cd, 0x48199002, 0x46224613, 0x44484629, 0xf9f0f000L, 0x9803b110L, 0xbd70b004L, 
    /*0x100*/ 0xe7fb1928L, 0xb5104812L, 0xf0004448L, 0x2800f865, 0x2001d000, 0x490ebd10, 0x4449b510, 0x460168ca, 
    /*0x120*/ 0x4448480b, 0xf82ef000L, 0xd0002800L, 0xbd102001L, 0x4601460b, 0xb5104806L, 0xf0004448L, 0x2800f93d, 
    /*0x140*/ 0x2001d000, 0xbd10, 0x4, 0x40052000, 0x8, 0x2170480a, 0x21807001, 0x78017001, 
    /*0x160*/ 0xd5fc0609L, 0x6817800, 0x2067d501, 0x6c14770, 0x2068d501, 0x7c04770, 0x2069d0fc, 0x4770, 
    /*0x180*/ 0x40020000, 0x41f0e92d, 0x460d4614, 0xf886f000L, 0xd11b2800L, 0x1e64442c, 0xd0050521L, 0xeb012101L, 
    /*0x1A0*/ 0x1e423114, 0x3401eb02, 0x447e4e09, 0x8024f8dfL, 0x42a52709, 0x6830d80a, 0xf8886005L, 0xf7ff7007L, 
    /*0x1C0*/ 0x2800ffc9, 0xf505d102L, 0xe7f25580L, 0x81f0e8bdL, 0x3be, 0x40020000, 0x4903b120, 0x71c82044, 
    /*0x1E0*/ 0xbfb8f7ffL, 0x47702004, 0x40020000, 0x4903b120, 0x71c82049, 0xbfaef7ffL, 0x47702004, 0x40020000, 
    /*0x200*/ 0xb132b138L, 0xd2102905L, 0xf001e8dfL, 0xb090905, 0x2004000d, 0x68c04770, 0x20006010, 0x68404770, 
    /*0x220*/ 0x6880e7fa, 0x6800e7f8, 0x206ae7f6, 0x4770, 0xb161b168L, 0x78804809, 0x203f000, 0xd0082a02L, 
    /*0x240*/ 0xebb22202L, 0xd1061f90L, 0x70082001, 0x2004e004, 0x20004770, 0x700ae7f9, 0x47702000, 0x40020000, 
    /*0x260*/ 0x4a19b1b0, 0x4b196cd1, 0x6103f3c1, 0xf833447bL, 0x3091011, 0x2300d00e, 0x3100e9c0, 0xf096cd1, 
    /*0x280*/ 0x2101d00a, 0xf44f6081L, 0x60c15180, 0x47702000, 0x47702004, 0x47702064, 0xe7f32102L, 0xea41b128L, 
    /*0x2A0*/ 0x75b0302, 0x2065d003, 0x20044770, 0x68034770, 0xd804428bL, 0x44116840, 0x42884418, 0x2066d201, 
    /*0x2C0*/ 0x20004770, 0x4770, 0x40048000, 0x2dc, 0x45f0e92d, 0x1eb0a9, 0x460d4617, 0xd00b4604L, 
    /*0x2E0*/ 0xffdcf7ffL, 0xd1082800L, 0xeb056861L, 0xf5b10807L, 0xd9054f00L, 0xe005094bL, 0xb0292004L, 0x85f0e8bdL, 
    /*0x300*/ 0x6380f44f, 0x22006824, 0x10dea4f, 0x4702fb03, 0x7022f841, 0x2a201c52, 0x2200d9f8, 0xa094f8dfL, 
    /*0x320*/ 0xe001ac21L, 0xd2082a08L, 0xc010f89aL, 0xfc02fa2cL, 0xc01f00c, 0xc002f804L, 0xf1a2e016L, 0x2f080708, 
    /*0x340*/ 0xf89ad202L, 0xe00ac011L, 0x710f1a2, 0xd2022f08L, 0xc012f89aL, 0xf89ae003L, 0xf1a2c013L, 0xfa2c0718L, 
    /*0x360*/ 0xf00cfc07L, 0x54a70701, 0x2a201c52, 0x2200d3da, 0x46174694, 0xe01046a2L, 0x4022f851, 0xd80b42acL, 
    /*0x380*/ 0x482eb01, 0x42ac6864, 0xf81ad906L, 0xf10c4002L, 0xb9040c01L, 0x441d1c7f, 0x45451c52, 0xb11fd3ecL, 
    /*0x3A0*/ 0xd1034567L, 0xe0022101L, 0xe0002100L, 0x70312102, 0xe7a3, 0x40020000, 0x47f0e92d, 0x14461d, 
    /*0x3C0*/ 0xd01e460eL, 0xf7ff461aL, 0x2800ff69, 0x4f0ed11a, 0xf8df447fL, 0xf04fa038L, 0x2d000807, 0x6838d012, 
    /*0x3E0*/ 0x68396006, 0x60486820, 0x68606839, 0x60883408, 0x8007f88aL, 0xfeaef7ffL, 0xd1032800L, 0x3d083608, 
    /*0x400*/ 0x2004e7eb, 0x87f0e8bdL, 0x198, 0x40020000, 0xb1d1b1d8L, 0x20004a0e, 0xf0037893L, 0x2b020303, 
    /*0x420*/ 0x2045d014, 0x780871d0, 0x784872d0, 0x78887290, 0x78c87250, 0x79087210, 0x794873d0, 0x79887390, 
    /*0x440*/ 0x79c87350, 0xf7ff7310L, 0x2004be85, 0x4770, 0x40020000, 0x47f0e92d, 0x4614469a, 0x4605460e, 
    /*0x460*/ 0xff1cf7ffL, 0xd1252800L, 0x101e9d5, 0xf8f1fbb0L, 0xf1c84271L, 0x40010000, 0x42b5424d, 0x4445d100, 
    /*0x480*/ 0x1bafb1bc, 0xd90042a7L, 0x480b4627, 0x44780939, 0x60066800, 0x22014809, 0xa0a71c2, 0x728172c2, 
    /*0x4A0*/ 0xa009f880L, 0xfe56f7ffL, 0xd1032800L, 0x443e1be4, 0x2000e7e5, 0x87f0e8bdL, 0xda, 0x40020000, 
    /*0x4C0*/ 0x4804b128, 0x71c22240, 0xf7ff7181L, 0x2004be43, 0x4770, 0x40020000, 0x4df0e92d, 0xe9dd001cL, 
    /*0x4E0*/ 0x46168709, 0xd025460dL, 0xfed8f7ffL, 0xd11f2800L, 0xb04cf8dfL, 0xf8df44fbL, 0x2e00a04c, 0xf8dbd018L, 
    /*0x500*/ 0x600d1000, 0xf88a2202L, 0x9a082007L, 0x200bf88a, 0xf8db, 0x60816821, 0xfe1cf7ffL, 0xf1b8b160L, 
    /*0x520*/ 0xd0010f00L, 0x5000f8c8, 0xd0012f00L, 0x60392100, 0x8df0e8bdL, 0xe7fb2004L, 0x1d241f36, 0xe7dc1d2dL, 
    /*0x540*/ 0x74, 0x40020000, 0xfffffffeL, 0x0, 0x80000, 0x100000, 0x200000, 0x400000, 
    /*0x560*/ 0x800000, 0x1000000, 0x1000000, 0x40020004, 0x0, 0x0, 
};

static const TARGET_FLASH flash = {
    0x20000091, // Init
    0x200000BF, // UnInit
    0x20000105, // EraseChip
    0x20000117, // EraseSector
    0x20000131, // ProgramPage

    // RAM start + 1, RAM start + size of algo??, top of stack = start of RAM + 4096??
	{0x20000001, 0x20000000+sizeof(K64F_FLM), 0x20001000}, // {breakpoint, static_base, stack_pointer}

    // any valid RAM location with +512 bytes of headroom
	0x20003000, // program_buffer
    // start of RAM
	0x20000000, // algo_start
    // size of array above
	sizeof(K64F_FLM), // algo_size
    // flash algo instruction array
	K64F_FLM,  // image

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
        for (i = 0; i < 16; i++) {
            if (data[i] != 0xff) {
                return 0;
            }
        }

//         if ((data[14] != 0xff) || (data[15] != 0xff)) {
//             return 0;
//         }
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
