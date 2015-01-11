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
#include "target_config.h"
#include "swd_host.h"
#include "target_flash.inc"
#include "target_reset.h"
#include "DAP_config.h"
#include "string.h"

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
    return 0;
}

target_flash_status_t target_flash_init(extension_t ext)
{
    swd_init();
    
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

target_flash_status_t target_flash_erase_sector(unsigned int sector)
{
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_sector, sector * target_device.sector_size, 0, 0, 0)) {
        return TARGET_FAIL_ERASE_SECTOR;
    }

    return TARGET_OK;
}

target_flash_status_t target_flash_erase_chip(void)
{
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_chip, 0, 0, 0, 0)) {
        return TARGET_FAIL_ERASE_ALL;
    }

    return TARGET_OK;
}

// Check Flash Configuration Field bytes at address 0x400-0x40f to ensure that flash security
// won't be enabled.
//
// FCF bytes:
// [0x0-0x7]=backdoor key
// [0x8-0xb]=flash protection bytes
// [0xc]=FSEC:
//      [7:6]=KEYEN (2'b10 is backdoor key enabled, all others backdoor key disabled)
//      [5:4]=MEEN (2'b10 mass erase disabled, all other mass erase enabled)
//      [3:2]=FSLACC (2'b00 and 2'b11 factory access enabled, 2'b01 and 2'b10 factory access disabled)
//      [1:0]=SEC (2'b10 flash security disabled, all other flash security enabled)
// [0xd]=FOPT
// [0xe]=EEPROM protection bytes (FlexNVM devices only)
// [0xf]=data flash protection bytes (FlexNVM devices only)
//
// This function checks that:
// - 0x0-0xb==0xff
// - 0xe-0xf==0xff
// - FSEC=0xfe
//
// FOPT can be set to any value.
target_flash_status_t check_security_bits(uint32_t flashAddr, uint8_t *data)
{
    if (flashAddr == 0x400) {
        // make sure we can unsecure the device or dont program at all
        if ((data[0xc] & 0x30) == 0x20) {
            // Dont allow programming mass-erase disabled state
            return TARGET_FAIL_SECURITY_BITS;
        }
        // Security is OK long as we can mass-erase (comment the following out to enable target security)
        if ((data[0xc] & 0x03) != 0x02) {
            return TARGET_FAIL_SECURITY_BITS;
        }
    }
    return TARGET_OK;
}

target_flash_status_t target_flash_program_page(uint32_t addr, uint8_t * buf, uint32_t size)
{
    uint32_t bytes_written = 0;
    target_flash_status_t status = TARGET_OK;
    // we need to erase a sector
    if (addr % target_device.sector_size == 0) {
        status = target_flash_erase_sector(addr / target_device.sector_size);
        if (status != TARGET_OK) {
            return status;
        }
    }

    // call a target dependent function to check if
    // we don't want to write specific bits (flash security bits, ...)
    status = check_security_bits(addr, buf);
    if (status != TARGET_OK) {
        return status;
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

    return TARGET_OK;
}
