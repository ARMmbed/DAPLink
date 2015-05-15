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
#include "flash_hal.h"
#include "target_config.h"
#include "intelhex.h"
#include "string.h"
#include "FlashOS.h"

#define MSC_BLOCK_SIZE (512)

static target_flash_status_t program_hex(uint8_t *buf, uint32_t size);
static target_flash_status_t program_bin(uint32_t addr, uint8_t *buf, uint32_t size);
static void set_hex_state_vars(void);
static extension_t file_extension;

static inline uint32_t test_range(const uint32_t test, const uint32_t min, const uint32_t max)
{
    return ((test < min) || (test > max)) ? 0 : 1;
}

uint8_t validate_bin_nvic(uint8_t *buf)
{
    // test for known required NVIC entries
    //  00 is stack pointer (RAM address)
    //  04 is Reset vector  (FLASH address)
    //  08 NMI_Handler      (FLASH address)
    //  12 HardFault_Handler(FLASH address)
    uint32_t i = 4, nvic_val = 0;
    // test the initial SP value
    memcpy(&nvic_val, buf+0, sizeof(nvic_val));
    if (0 == test_range(nvic_val, target_device.ram_start, target_device.ram_end)) {
        return 0;
    }
    // Reset_Handler
    // NMI_Handler
    // HardFault_Handler
    for ( ; i <=12; i+=4) {
        memcpy(&nvic_val, buf+i, sizeof(nvic_val));
        if (0 == test_range(nvic_val, target_device.flash_start, target_device.flash_end)) {
            return 0;
        }
    }
    
    return 1;
}

uint8_t validate_hexfile(uint8_t *buf)
{
    // look here for known hex records
    // add hex identifier b[0] == ':' && b[8] == {'0', '2', '3', '4', '5'}
    return ((buf[0] == ':') && ((buf[8] == '0') || (buf[8] == '2') || (buf[8] == '3') || (buf[8] == '4') || (buf[8] == '5'))) ? 1 : 0;
}

target_flash_status_t target_flash_init(extension_t ext)
{
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
    if (0 != flash_erase_sector_svc(sector*target_device.sector_size)) {
        return TARGET_FAIL_ERASE_SECTOR;
    }
    return TARGET_OK;
}

target_flash_status_t target_flash_erase_chip(void)
{
    for (uint32_t i = target_device.flash_start; i < target_device.flash_end; i += target_device.sector_size) {
        if (0 != target_flash_erase_sector(i/target_device.sector_size)) {
            return TARGET_FAIL_ERASE_ALL;
        }
    }
    return TARGET_OK;
}

static target_flash_status_t program_bin(uint32_t addr, uint8_t *buf, uint32_t size)
{
    // called from msc logic so assumed that the smallest size is 512 (size of sector)
    //  flash algo must support this as minimum size.
    //  ToDO: akward requirement. look at flash algo flexibility in flash write sizes
    if (0 != flash_program_page_svc(addr + target_device.flash_start, size, buf)) {
        return TARGET_FAIL_WRITE;
    }
    return TARGET_OK;
}

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
static uint8_t hex_buffer[MSC_BLOCK_SIZE+256] = {0};
static volatile uint32_t target_ram_idx = 0;         // running count of amount of data in target RAM

static void set_hex_state_vars(void)
{
    memset(bin_buffer, 0, sizeof(bin_buffer));
    memset(hex_buffer, 0, sizeof(hex_buffer));
    target_ram_idx = 0;
}


static target_flash_status_t flexible_program_block(uint32_t addr, uint8_t *buf, uint32_t size)
{
    // store the block start address if aligned with the programming size
    uint32_t target_flash_address = (addr / MSC_BLOCK_SIZE) * MSC_BLOCK_SIZE;

    // write to target RAM
    memcpy(hex_buffer+target_ram_idx, buf, size);
    target_ram_idx += size;
    // program a block if necessary
    if (target_ram_idx >= MSC_BLOCK_SIZE) {
        if (0 != flash_program_page_svc(target_flash_address + target_device.flash_start, MSC_BLOCK_SIZE, hex_buffer)) {
            return TARGET_FAIL_WRITE;
        }
        target_ram_idx -= MSC_BLOCK_SIZE;
        // cleanup
        if (target_ram_idx > 0) {
            // write excess data to target RAM at bottom of buffer. This re-aligns offsets that may occur based on hex formatting
            memcpy(hex_buffer, buf+(size-target_ram_idx), target_ram_idx);
            memset(hex_buffer+target_ram_idx, 0, sizeof(hex_buffer)-target_ram_idx);
        }
    }
    return TARGET_OK;
}
    
static target_flash_status_t program_hex(uint8_t *buf, uint32_t size)
{
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
            if ( (bin_start_address % MSC_BLOCK_SIZE) > target_ram_idx ) {
                flexible_program_block(bin_start_address, (uint8_t *)ff_buffer, (bin_start_address % MSC_BLOCK_SIZE) - target_ram_idx);
            }
            return flexible_program_block(bin_start_address, bin_buffer, bin_buf_written);
        }
        else if (HEX_PARSE_UNALIGNED == status) {
            // Check if buffer has not been filled with anything, then simply return because hex line was not complete
            if (bin_buf_written != 0) {
                // unaligned address. Write data to target RAM and force pad with ff //00
                // Check if ram index needs to be shifted
                if ( (bin_start_address % MSC_BLOCK_SIZE) > target_ram_idx ) {
                    flexible_program_block(bin_start_address, (uint8_t *)ff_buffer, (bin_start_address % MSC_BLOCK_SIZE) - target_ram_idx);
                }
                flexible_program_block(bin_start_address, bin_buffer, bin_buf_written);
            }
            
            // Check if rest of page needs to be padded
            if (target_ram_idx != 0) {
                flexible_program_block(bin_start_address+bin_buf_written, (uint8_t *)ff_buffer, MSC_BLOCK_SIZE-target_ram_idx);
            }
                    
            // incrememntal offset to finish the block
            size -= block_amt_parsed;
            buf += block_amt_parsed;
        }
        else if (HEX_PARSE_EOF == status) {
            // Check if there is content in ram buffer that needs to be written to chip
            if (bin_buf_written != 0) {
                // Check if ram index needs to be shifted
                if ( (bin_start_address % MSC_BLOCK_SIZE) > target_ram_idx ) {
                    flexible_program_block(bin_start_address, (uint8_t *)ff_buffer, (bin_start_address % MSC_BLOCK_SIZE) - target_ram_idx);
                }
                flexible_program_block(bin_start_address, bin_buffer, bin_buf_written);
            }
            
            // Check if rest of page needs to be padded
            if (target_ram_idx != 0) {
                flexible_program_block(bin_start_address+bin_buf_written, (uint8_t *)ff_buffer, MSC_BLOCK_SIZE-target_ram_idx);
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
