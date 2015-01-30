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

static target_flash_status_t target_flash_erase_chip(void);
static target_flash_status_t target_flash_erase_sector(uint32_t adr);
static target_flash_status_t program_hex(uint8_t *buf, uint32_t size);
static target_flash_status_t program_bin(uint32_t addr, uint8_t *buf, uint32_t size);
// maybe a parameter in write but this is just for now.
static extension_t file_extension;

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

target_flash_status_t target_flash_uninit(void)
{
    // when programming is complete the target should be put and held in reset
    return TARGET_OK;
}

target_flash_status_t target_flash_program_page(uint32_t addr, uint8_t * buf, uint32_t size)
{
    if (HEX == file_extension) {
        return program_hex(buf, size);
    }
    else if (BIN == file_extension) {
        return program_bin(addr, buf, size);
    }
    return TARGET_FAIL_UNKNOWN_APP_FORMAT;
}

static target_flash_status_t target_flash_erase_sector(unsigned int sector)
{
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_sector, sector*target_device.sector_size, 0, 0, 0)) {
        return TARGET_FAIL_ERASE_SECTOR;
    }
    return TARGET_OK;
}

static target_flash_status_t target_flash_erase_chip(void)
{
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_chip, 0, 0, 0, 0)) {   // 1 == O.K., 0 == Error
        return TARGET_FAIL_ERASE_ALL;
    }
    return TARGET_OK;
}

// size must be less than and evenly divisable by target.ram_to_flash_bytes_to_be_written
static target_flash_status_t program_bin(uint32_t addr, uint8_t *buf, uint32_t size)
{
    static uint32_t target_ram_idx = 0;
    if (flash.ram_to_flash_bytes_to_be_written % size != 0) {
        return TARGET_FAIL_WRITE_UNALIGNED;
    }
    if (size > flash.ram_to_flash_bytes_to_be_written) {
    // store the offset location in target RAM as blocks may be less than the write to flash amount
    target_ram_idx += size;
    // not all flash is at address 0 but binary files are assumed to be 0 + mutually
    //  known offset
    addr += target_device.flash_start;
    // Export a hook to verify the flash protect is NOT set
    if (1 == security_bits_set(addr, buf, size)) {
        return TARGET_FAIL_SECURITY_BITS;
    }
    // we need to erase a sector
    if (addr % target_device.sector_size == 0) {
        if (TARGET_OK != target_flash_erase_sector(addr / target_device.sector_size)) {
            return TARGET_FAIL_ERASE_SECTOR;
        }
    }
    // Write a page in target RAM to be programmed.
    if (0 == swd_write_memory(flash.program_buffer+target_ram_idx, buf, size)) {
        return TARGET_FAIL_ALGO_DATA_SEQ;
    }
    // make sure the amount of data in RAM is whats needed by the flash algo
    if (target_ram_idx == flash.ram_to_flash_bytes_to_be_written) {
        // transfer from RAM to flash
        target_ram_idx = 0;
        if (0 == swd_flash_syscall_exec(
            & flash.sys_call_param
            , flash.program_page
            , addr
            , flash.ram_to_flash_bytes_to_be_written
            , flash.program_buffer
            , 0)
            ) {
            return TARGET_FAIL_WRITE;
        }
    }
    return TARGET_OK;
}

static uint8_t bin_buffer[512] = {0};
static target_flash_status_t program_hex(uint8_t *buf, uint32_t size)
{
    static uint32_t target_ram_idx = 0;
    hex_parse_status_t status = HEX_PARSE_UNINIT;
    uint32_t bin_start_address = 0; // Decoded from the hex file, the binary buffer data starts at this address
    uint32_t bin_buf_written = 512; // The amount of data in the binary buffer starting at address above
    uint32_t block_size = size;     // the amount of data in a block to be decoded
    uint32_t block_amt_parsed = 0;  // amount of data parsed in the block on the last call
    while (1) {
        // try to decode a block of hex data into bin data
        status = parse_hex_blob(buf, block_size, &block_amt_parsed, bin_buffer, sizeof(bin_buffer), &bin_start_address, &bin_buf_written);
        if (HEX_PARSE_EOF == status) {
            // check if security bits were set
            if (1 == security_bits_set(bin_start_address, bin_buffer, bin_buf_written)) {
                return TARGET_FAIL_SECURITY_BITS;
            }
            // possibly erase a sector
            if (bin_start_address % target_device.sector_size == 0) {
                if (TARGET_OK != target_flash_erase_sector(bin_start_address / target_device.sector_size)) {
                    return TARGET_FAIL_ERASE_SECTOR;
                }
            }
            // write to target RAM
            if (0 == swd_write_memory(flash.program_buffer+target_ram_idx, bin_buffer, bin_buf_written)) {
                return TARGET_FAIL_ALGO_DATA_SEQ;
            }
            target_ram_idx += bin_buf_written;
            // pad RAM with ff
            if (0 == swd_write_memory(flash.program_buffer+target_ram_idx, (uint8_t *)buf_ff, flash.ram_to_flash_bytes_to_be_written-bin_buf_written)) {
                return TARGET_FAIL_ALGO_DATA_SEQ;
            }
            target_ram_idx += flash.ram_to_flash_bytes_to_be_written-bin_buf_written;
            // program
            if (0 == swd_flash_syscall_exec(&flash.sys_call_param, flash.program_page, bin_start_address, flash.ram_to_flash_bytes_to_be_written, flash.program_buffer, 0)) {
                return TARGET_FAIL_WRITE;
            }
            target_ram_idx = 0;
            return TARGET_HEX_FILE_EOF;
        }
        else if (HEX_PARSE_OK == status) {
            // check if security bits were set
            if (1 == security_bits_set(bin_start_address, bin_buffer, bin_buf_written)) {
                return TARGET_FAIL_SECURITY_BITS;
            }            
            // possibly erase a sector
            // write to target RAM
            // program if necessary
            block_size = 512;
            buf += block_amt_parsed;
        }
        else if (HEX_PARSE_UNALIGNED == status) {
            // check if security bits were set
            if (1 == security_bits_set(bin_start_address, bin_buffer, bin_buf_written)) {
                return TARGET_FAIL_SECURITY_BITS;
            }
            // possibly erase a sector
            // write to target RAM
            // pad RAM with ff
            // program
            // parse the rest of the buffer
            
            // incrememntal offset to finish the block
            block_size = (512 - block_amt_parsed);
            buf += block_amt_parsed;
        }
        else if (HEX_PARSE_CKSUM_FAIL == status) {
            return TARGET_FAIL_HEX_CKSUM;
        }
        else if (HEX_PARSE_UNINIT == status) {
            return TARGET_FAIL_HEX_PARSER;
        }
    }
}
