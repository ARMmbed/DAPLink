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
 
#include "RTL.h"
#include "debug_cm.h"
#include "target_reset.h"
#include "DAP_config.h"
#include "swd_host.h"
#include "gpio.h"
#include "target_flash.h"
#include "target_config.h"
#include "flash_blob.h"
#include "intelhex.h"

#include "string.h"

#define MIN_FLASH_ADDRESS   0x00000000
#define MAX_FLASH_ADDRESS   0x00020000
#define MIN_RAM_ADDRESS     0x20000000
#define MAX_RAM_ADDRESS     0x20004000

static extension_t input_file_ext = BIN;

static uint8_t bin_buffer[512];
static uint32_t bin_start_addr = 0;
static uint8_t *bin_ptr = bin_buffer;
const static uint8_t *bin_end = bin_buffer + sizeof(bin_buffer);
static uint8_t  chip_is_erased = 0;

static /*inline*/ uint32_t test_range(const uint32_t test, const uint32_t min, const uint32_t max)
{
    return ((test < min) || (test > max)) ? 0 : 1;
}

uint8_t validate_bin_nvic(uint8_t *buf)
{
    // test for known required NVIC entries
    //  0 is stack pointer (RAM address)
    //  1 is Reset vector  (FLASH address)
    uint32_t nvic_sp = 0;
    uint32_t nvic_rv = 0;
    // test the initial SP value
    memcpy(&nvic_sp, buf, sizeof(nvic_sp));
    if (0 == test_range(nvic_sp, MIN_RAM_ADDRESS, MAX_RAM_ADDRESS)) {
        return 0;
    }
    // test the initial reset vector
    memcpy(&nvic_rv, buf+4, sizeof(nvic_rv));
    if (0 == test_range(nvic_rv, MIN_FLASH_ADDRESS, MAX_FLASH_ADDRESS)) {
        return 0;
    }
    
    return 1;
}

uint8_t validate_hexfile(uint8_t *buf)
{
    if (buf[0] != ':' || buf[1] != '0' || buf[3] != '0' || buf[4] != '0' || buf[5] != '0' || buf[6] != '0') {
        return 0;
    }

    return 1;
}


target_flash_status_t target_flash_init(extension_t ext) {
    input_file_ext = ext;
    
    bin_ptr = bin_buffer;
    bin_start_addr = 0;
    chip_is_erased = 0;
    intelhex_reset();
    
    PORT_SWD_SETUP();
    if (!target_set_state(RESET_PROGRAM)) {
        return TARGET_FAIL_RESET;
    }
    
    // Download flash programming algorithm to target and initialise.
    if (!swd_write_memory(flash.algo_start, (uint8_t *)flash.image, flash.algo_size)) {
        return TARGET_FAIL_ALGO_DL;
    }

    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.init, 0, 0 /* clk value is not used */, 0, 0)) {
        return TARGET_FAIL_INIT;
    }

    return TARGET_OK;
}

target_flash_status_t target_flash_erase_sector(uint32_t adr) {
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_sector, adr, 0, 0, 0)) {
        return TARGET_FAIL_ERASE_SECTOR;
    }

    return TARGET_OK;
}

target_flash_status_t target_flash_erase_chip(void) {
    //
	// 1 == O.K.
	// 0 == Error
	//
   if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_chip, 0, 0, 0, 0)) {   // 1 == O.K., 0 == Error
        return TARGET_FAIL_ERASE_ALL;
    }
    
//    target_set_state(RESET_PROGRAM);
//     target_flash_init(input_file_ext);    
    
    return TARGET_OK;
}

target_flash_status_t target_flash_program_page_bin(uint32_t addr, uint8_t * buf, uint32_t size)
{
    uint32_t bytes_written = 0;
    
    if (addr == 0) {
        if (target_flash_erase_chip()) { 
            return TARGET_FAIL_ERASE_ALL;
        }
        
        chip_is_erased = 1;
    }
    
	  // Program a page in target flash.
    if (!swd_write_memory(flash.program_buffer, buf, size)) {
        return TARGET_FAIL_ALGO_DATA_SEQ;
    }
    
    while(bytes_written < size) {
        uint32_t bytes;
        if (size < flash.ram_to_flash_bytes_to_be_written) {
            bytes = size;
        } else {
            bytes = flash.ram_to_flash_bytes_to_be_written;
        }
        
        if ((0 == chip_is_erased) && (0 == (addr % target_device.sector_size))) {   // first byte of a sector
            if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_sector, addr, 0, 0, 0)) {
                return TARGET_FAIL_ERASE_SECTOR;
            }
        }
        
        if (!swd_flash_syscall_exec(&flash.sys_call_param,
                                    flash.program_page,
                                    addr,                                     // arg1
                                    bytes,                                    // arg2
                                    flash.program_buffer + bytes_written, 0)) { // arg3, arg4
            return TARGET_FAIL_WRITE;
        }
        
        bytes_written += flash.ram_to_flash_bytes_to_be_written;
        addr += flash.ram_to_flash_bytes_to_be_written;
    }

    return TARGET_OK;
}

target_flash_status_t target_flash_program_page(uint32_t addr, uint8_t * buf, uint32_t size)
{
    if (HEX == input_file_ext) {
        uint8_t *hex_ptr = buf;
        uint8_t *hex_end = buf + size;
        
        do {
            uint32_t hex_buf_size = hex_end - hex_ptr;
            uint32_t bin_buf_size = bin_end - bin_ptr;
            
            uint8_t *last_bin_ptr = bin_ptr;
            
            int32_t status = intelhex_parse(&hex_ptr, &bin_ptr, &addr, hex_buf_size, bin_buf_size);
            
            uint32_t bin_size = bin_ptr - bin_buffer;
            if (last_bin_ptr == bin_buffer) {
                bin_start_addr = addr;
            }
            
            if (0 > status) {
                // invalid hex file
                return TARGET_FAIL_INVALID_HEX_FILE;
            }
            
            if (INTELHEX_DONE == status) {
                if (bin_size) {
                    target_flash_status_t ret = target_flash_program_page_bin(bin_start_addr, bin_buffer, bin_size); 
                    if (ret) {
                        return ret;
                    }
                }
                
                return TARGET_HEX_FILE_EOF;
            }
            
            if (INTELHEX_TO_WRITE == status) {
                if (bin_size) {
                    target_flash_status_t ret = target_flash_program_page_bin(bin_start_addr, bin_buffer, bin_size);
                    if (ret) {
                        return ret;
                    }
                    
                    bin_ptr = bin_buffer;
                }
                
                continue;
            }
            
            if (bin_size >= sizeof(bin_buffer)) {
                target_flash_status_t ret = target_flash_program_page_bin(bin_start_addr, bin_buffer, bin_size);
                if (ret) {
                    return ret;
                }
                
                bin_ptr = bin_buffer;
            }
        } while (hex_ptr < hex_end);
        
        return TARGET_OK;
    } else {
        return target_flash_program_page_bin(addr, buf, size);
    }
    
}



