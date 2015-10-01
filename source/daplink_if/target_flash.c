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
 
#include "target_flash.h"
#include "target_reset.h"
#include "gpio.h"
#include "validation.h"
#include "target_config.h"
#include "intelhex.h"
#include "string.h"
#include "swd_host.h"

static target_flash_status_t program_hex(uint8_t *buf, uint32_t size);
static target_flash_status_t program_bin(uint32_t addr, uint8_t *buf, uint32_t size);
static void set_hex_state_vars(void);
static extension_t file_extension;

target_flash_status_t target_flash_init(extension_t ext)
{
    const program_target_t * const flash = target_device.flash_algo;

    if (0 == target_set_state(RESET_PROGRAM)) {
        return TARGET_FAIL_RESET;
    }
    
    // Download flash programming algorithm to target and initialise.
    if (0 == swd_write_memory(flash->algo_start, (uint8_t *)flash->algo_blob, flash->algo_size)) {
        return TARGET_FAIL_ALGO_DL;
    }

    if (0 == swd_flash_syscall_exec(&flash->sys_call_s, flash->init, target_device.flash_start, 0, 0, 0)) {
        return TARGET_FAIL_INIT;
    }
    
    file_extension = ext;
    if (HEX == file_extension) {
        reset_hex_parser();
        set_hex_state_vars();
    }
    return target_flash_erase_chip();
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

target_flash_status_t target_flash_erase_sector(uint32_t sector)
{
    const program_target_t * const flash = target_device.flash_algo;
    if (0 == swd_flash_syscall_exec(&flash->sys_call_s, flash->erase_sector, sector*target_device.sector_size, 0, 0, 0)) {
        return TARGET_FAIL_ERASE_SECTOR;
    }
    return TARGET_OK;
}

target_flash_status_t target_flash_erase_chip(void)
{
    const program_target_t * const flash = target_device.flash_algo;
    if (0 == swd_flash_syscall_exec(&flash->sys_call_s, flash->erase_chip, 0, 0, 0, 0)) {
        return TARGET_FAIL_ERASE_ALL;
    }
    return TARGET_OK;
}

#if 0
static target_flash_status_t program_bin(uint32_t addr, uint8_t *buf, uint32_t size)
{
    const program_target_t * const flash = target_device.flash_algo;

    // called from msc logic so assumed that the smallest size is 512 (size of sector)
    //  flash algo must support this as minimum size.
    //  ToDO: akward requirement. look at flash algo flexibility in flash write sizes
    while (size >= flash->program_buffer_size) {
        // Export a hook to verify the flash protect is NOT set
        if (1 == security_bits_set(addr, buf, size)) {
            return TARGET_FAIL_SECURITY_BITS;
        }

//        // we need to erase a sector
//        if (addr % target_device.sector_size == 0) {
//            if (TARGET_OK != target_flash_erase_sector(addr / target_device.sector_size)) {
//                return TARGET_FAIL_ERASE_SECTOR;
//            }
//        }
        // Write a page in target RAM to be programmed.
        if (0 == swd_write_memory(flash->program_buffer, buf, size)) {
            return TARGET_FAIL_ALGO_DATA_SEQ;
        }
        // Exectue a program flash sequence on the target device
        if (0 == swd_flash_syscall_exec(
            & flash->sys_call_s
            , flash->program_page
            , addr + target_device.flash_start
            , flash->program_buffer_size
            , flash->program_buffer
            , 0)
            ) {
            return TARGET_FAIL_WRITE;
        }
        size -= flash->program_buffer_size;
    }
    return TARGET_OK;
}
#else

target_flash_status_t target_flash_erase_address(uint32_t address)
{
    const program_target_t * const flash = target_device.flash_algo;
    if (0 == swd_flash_syscall_exec(&flash->sys_call_s, flash->erase_sector, address, 0, 0, 0)) {
        return TARGET_FAIL_ERASE_SECTOR;
    }
    return TARGET_OK;
}

target_flash_status_t program_bin(uint32_t addr, uint8_t * buf, uint32_t size)
{
    const program_target_t * const flash = target_device.flash_algo;

    uint32_t bytes_written = 0;
    target_flash_status_t status = TARGET_OK;

	
    // we need to erase a sector
    if (addr % target_device.sector_size == 0) {
#if defined(BOARD_LPC4337) || defined(BOARD_LPC11U68)
		// takes care about different sector size
		if ((addr < SECTOR_BOUNDARY) || ((addr >= SECTOR_BOUNDARY) && (addr % SECTOR_BOUNDARY_ALIGN == 0))) {
			status = target_flash_erase_address(addr);
		}
#else
        status = target_flash_erase_address(addr);
#endif
		if (status != TARGET_OK) {
            return status;
        }
    }

    // Program a page in target flash.
    if (!swd_write_memory(flash->program_buffer, buf, size)) {
        return TARGET_FAIL_ALGO_DATA_SEQ;
    }

    while(bytes_written < size) {
        if (!swd_flash_syscall_exec(&flash->sys_call_s,
                                    flash->program_page,
                                    addr,
                                    flash->program_buffer_size,
                                    flash->program_buffer + bytes_written,
                                    0 )) {
            return TARGET_FAIL_WRITE;
        }

        bytes_written += flash->program_buffer_size;
        addr += flash->program_buffer_size;
    }

    return TARGET_OK;
}
#endif

static const uint8_t ff_buffer[512] = {
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};
static uint8_t bin_buffer[256] = {0};
static volatile uint32_t target_ram_idx = 0;         // running count of amount of data in target RAM

static void set_hex_state_vars(void)
{
    memset(bin_buffer, 0, sizeof(bin_buffer));
    target_ram_idx = 0;
}


static target_flash_status_t flexible_program_block(uint32_t addr, uint8_t *buf, uint32_t size)
{
    const program_target_t * const flash = target_device.flash_algo;

    // store the block start address if aligned with the programming size
    uint32_t target_flash_address = (addr / flash->program_buffer_size) * flash->program_buffer_size;
    // check if security bits were set. Could be an odd alignment boundry that breaks (bin_buf only has part of security region)
    if (1 == security_bits_set(target_flash_address, buf, size)) {
        return TARGET_FAIL_SECURITY_BITS;
    }
//    // possibly erase a sector
//    if (target_flash_address % target_device.sector_size == 0) {
//        if (TARGET_OK != target_flash_erase_sector(target_flash_address / target_device.sector_size)) {
//            return TARGET_FAIL_ERASE_SECTOR;
//        }
//    }
    // write to target RAM
    if (0 == swd_write_memory(flash->program_buffer+target_ram_idx, buf, size)) {
        return TARGET_FAIL_ALGO_DATA_SEQ;
    }
    target_ram_idx += size;
    // program a block if necessary
    if (target_ram_idx >= flash->program_buffer_size) {
        if (0 == swd_flash_syscall_exec(
            & flash->sys_call_s
            , flash->program_page
            , target_flash_address + target_device.flash_start
            , flash->program_buffer_size
            , flash->program_buffer
            , 0)
            ) {
            return TARGET_FAIL_WRITE;
        }
        target_ram_idx -= flash->program_buffer_size;
        // cleanup
        if (target_ram_idx > 0) {
            // write excess data to target RAM at bottom of buffer. This re-aligns offsets that may occur based on hex formatting
            if (0 == swd_write_memory(flash->program_buffer, buf+(size-target_ram_idx), target_ram_idx)) {
                return TARGET_FAIL_ALGO_DATA_SEQ;
            }
        }
    }
    return TARGET_OK;
}
    
static target_flash_status_t program_hex(uint8_t *buf, uint32_t size)
{
    const program_target_t * const flash = target_device.flash_algo;
    hexfile_parse_status_t status = HEX_PARSE_UNINIT;
    uint32_t bin_start_address = 0; // Decoded from the hex file, the binary buffer data starts at this address
    uint32_t bin_buf_written = 0;   // The amount of data in the binary buffer starting at address above
    uint32_t block_amt_parsed = 0;  // amount of data parsed in the block on the last call
    
    while (1) {
        // try to decode a block of hex data into bin data
        status = parse_hex_blob(buf, size, &block_amt_parsed, bin_buffer, sizeof(bin_buffer), &bin_start_address, &bin_buf_written);
        // the entire block of hex was decoded. This is a simple state
        if (HEX_PARSE_OK == status) {
            // Check if buffer has not been filled with anything, then simply return because hex line was not complete
            if (bin_buf_written == 0) {
                return TARGET_OK;
            }
            // hex file data decoded. Write to target RAM and program if necessary
            if ( (bin_start_address % flash->program_buffer_size) > target_ram_idx ) {
                flexible_program_block(bin_start_address, (uint8_t *)ff_buffer, (bin_start_address % flash->program_buffer_size) - target_ram_idx);
            }
            return flexible_program_block(bin_start_address, bin_buffer, bin_buf_written);
        }
        else if (HEX_PARSE_UNALIGNED == status) {
            // Check if buffer has not been filled with anything, then simply return because hex line was not complete
            if (bin_buf_written != 0) {
                // unaligned address. Write data to target RAM and force pad with ff //00
                // Check if ram index needs to be shifted
                if ( (bin_start_address % flash->program_buffer_size) > target_ram_idx ) {
                    flexible_program_block(bin_start_address, (uint8_t *)ff_buffer, (bin_start_address % flash->program_buffer_size) - target_ram_idx);
                }
                flexible_program_block(bin_start_address, bin_buffer, bin_buf_written);
            }
            
            // Check if rest of page needs to be padded
            if (target_ram_idx != 0) {
                flexible_program_block(bin_start_address+bin_buf_written, (uint8_t *)ff_buffer, flash->program_buffer_size-target_ram_idx);
            }
                    
            // incrememntal offset to finish the block
            size -= block_amt_parsed;
            buf += block_amt_parsed;
        }
        else if (HEX_PARSE_EOF == status) {
            // Check if there is content in ram buffer that needs to be written to chip
            if (bin_buf_written != 0) {
                // Check if ram index needs to be shifted
                if ( (bin_start_address % flash->program_buffer_size) > target_ram_idx ) {
                    flexible_program_block(bin_start_address, (uint8_t *)ff_buffer, (bin_start_address % flash->program_buffer_size) - target_ram_idx);
                }
                flexible_program_block(bin_start_address, bin_buffer, bin_buf_written);
            }
            
            // Check if rest of page needs to be padded
            if (target_ram_idx != 0) {
                flexible_program_block(bin_start_address+bin_buf_written, (uint8_t *)ff_buffer, flash->program_buffer_size-target_ram_idx);
            }
            //target_ram_idx = 0;
            return TARGET_HEX_FILE_EOF;
        }
        else if (HEX_PARSE_CKSUM_FAIL == status) {
            return TARGET_FAIL_HEX_CKSUM;
        }
        else if ((HEX_PARSE_UNINIT == status) || (HEX_PARSE_FAILURE == status)) {
            return TARGET_FAIL_HEX_PARSER;
        }
    }
}
