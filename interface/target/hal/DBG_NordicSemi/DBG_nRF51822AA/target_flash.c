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
 
#include "sam3u.h"
#include "RTL.h"
#include "debug_cm.h"
#include "target_reset.h"
#include "swd_host.h"
#include "gpio.h"
#include "target_flash.h"
#include "target_config.h"
#include "flash_blob.h"
#include "DAP_config.h"
#include "intelhex.h"

#include "string.h"

// maybe a parameter in write but this is just for now.
extension_t file_extension;

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
    if (0 == test_range(nvic_sp, target_device.ram_start, target_device.ram_end)) {
        return 0;
    }
    // test the initial reset vector
    memcpy(&nvic_rv, buf+4, sizeof(nvic_rv));
    if (0 == test_range(nvic_rv, target_device.flash_start, target_device.flash_end)) {
        return 0;
    }
    
    return 1;
}

uint8_t validate_hexfile(uint8_t *buf)
{
    // look here for known hex records
    // add hex identifier b[0] == ':' && b[8] == {'0', '2', '3', '4', '5'}
    if (buf[0] == ':') {
        if ((buf[8] == '0') ||
            (buf[8] == '2') ||
            (buf[8] == '3') ||
            (buf[8] == '4') ||
            (buf[8] == '5'))
        {
            return 1;
        }
    }
    return 0;
}

target_flash_status_t target_flash_init(extension_t ext)
{
    if (!target_set_state(RESET_PROGRAM)) {
        return TARGET_FAIL_RESET;
    }
    
    // Download flash programming algorithm to target and initialise.
    if (!swd_write_memory(flash.algo_start, (uint8_t *)flash.image, flash.algo_size)) {
        return TARGET_FAIL_ALGO_DL;
    }

    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.init, target_device.flash_start, 0 /* clk value is not used */, 0, 0)) {
        return TARGET_FAIL_INIT;
    }
    
    file_extension = ext;
    if (HEX == file_extension) {
        reset_hex_parser();
    }
    
    return TARGET_OK;
}


target_flash_status_t target_flash_erase_sector(unsigned int sector) {
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_sector, sector*target_device.sector_size, 0, 0, 0)) {
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
    
    target_set_state(RESET_PROGRAM);
    //target_flash_init();    
    
    return TARGET_OK;
}

static uint8_t bin_buffer[256] = {0};

target_flash_status_t target_flash_program_page(uint32_t addr, uint8_t * buf, uint32_t size)
{
    uint32_t bytes_written = 0, i = 0;
    uint32_t bin_start_address = 0, bin_buf_written = 512, block_size = 512, block_amt_parsed = 0;
    hex_parse_status_t status;
    //ToDO - Fix this decode and program logic. May require some more helper methods
    if (HEX == file_extension) {
        do {
            status = parse_hex_blob(buf, size, &block_amt_parsed, bin_buffer, sizeof(bin_buffer), &bin_start_address, &bin_buf_written);
            if ((HEX_PARSE_EOF == status) || (HEX_PARSE_OK == status)) {
                if ((block_size != block_amt_parsed) && (status != HEX_PARSE_EOF)) {
                    // error state in parsing or record type
                    block_amt_parsed = block_amt_parsed;
                    //error("block parse amt failure\n");
                }
                //print the decoded file contents here
                for (i = 0; i < bin_buf_written; i++) {
                    //pc.putc(bin_buffer[i]);
                }
                block_size = 512;
                buf += block_amt_parsed;
            }
            if (HEX_PARSE_UNALIGNED == status) {
                //try to program the data here
                for (i = 0; i < bin_buf_written; i++) {
                    //pc.putc(bin_buffer[i]);
                }
                // pad the rest of the flash buffer with 0xff
                for (i = bin_buf_written; i < 512; i++) {
                    //pc.putc(0xff);
                }
                // incrememntal offset to finish the block
                block_size = (512 - block_amt_parsed);
                buf += block_amt_parsed;
            }
            if (HEX_PARSE_CKSUM_FAIL == status) {
                // programming failure recorded to usere here
                //error("cksum failure\n");
            }
            if (HEX_PARSE_UNINIT == status) {
                //error("parser logic failure\n");
            }
            
        } while(HEX_PARSE_EOF != status); // eject msc    
    // this was the previous bin only logic. need to try and consolidate
    } else {
        target_flash_status_t status = TARGET_OK;
        addr += target_device.flash_start;
        
        // we need to erase a sector
        if (addr % target_device.sector_size == 0) {
            status = target_flash_erase_sector(addr / target_device.sector_size);
            if (status != TARGET_OK) {
                return status;
            }
        }

        // Program a page in target RAM to be programmed.
        if (!swd_write_memory(flash.program_buffer, buf, size)) {
            return TARGET_FAIL_ALGO_DATA_SEQ;
        }

        while(bytes_written < size) {
            if (!swd_flash_syscall_exec(&flash.sys_call_param,
                                        flash.program_page,
                                        addr,
                                        flash.ram_to_flash_bytes_to_be_written,
                                        flash.program_buffer + bytes_written, 0)) {
                return TARGET_FAIL_WRITE;
            }

            bytes_written += flash.ram_to_flash_bytes_to_be_written;
            addr += flash.ram_to_flash_bytes_to_be_written;
        }
    }

    return TARGET_OK;
}
