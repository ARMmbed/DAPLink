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

#include <string.h>

#include "flash_decoder.h"
#include "util.h"
#include "macro.h"
#include "daplink.h"
#include "validation.h"
#include "flash_manager.h"
#include "target_config.h"  // for target_device

// Set to 1 to enable debugging
#define DEBUG_FLASH_DECODER     0

#if DEBUG_FLASH_DECODER
    #include "daplink_debug.h"
    #define flash_decoder_printf    debug_msg
#else
    #define flash_decoder_printf(...)
#endif

typedef enum {
    DATA_MODE_UNSET,
    DATA_MODE_STREAM,
    DATA_MODE_ADDR,
} data_mode_t;

typedef enum {
    DECODER_STATE_CLOSED,
    DECODER_STATE_OPEN,
    DECODER_STATE_STREAM,
    DECODER_STATE_ADDR,
    DECODER_STATE_ERROR
} decoder_state_t;

static uint8_t flash_buf[48];
static decoder_state_t state = DECODER_STATE_CLOSED;
static uint32_t flash_buf_pos;
static uint32_t initial_offset;
static uint32_t current_offset;
static uint32_t offset_to_absolute;
static bool flash_initialized;

static error_t get_flash(flash_decoder_type_t type, uint32_t addr, bool addr_valid, uint32_t * start_addr, const flash_intf_t ** flash_intf);
static error_t write_addr(uint32_t addr, const uint8_t * data, uint32_t size, bool sequential, bool addr_absolute);


flash_decoder_type_t flash_decoder_detect_type(const uint8_t * data, uint32_t size, uint32_t addr, bool addr_valid)
{
    daplink_info_t info;
    util_assert(size >= 48);

    // Check if this is a daplink image
    memcpy(&info, data + DAPLINK_INFO_OFFSET, sizeof(info));
    if (DAPLINK_HDK_ID == info.hdk_id) {
        if (DAPLINK_BUILD_KEY_IF == info.build_key) {
            // Interface update
            return FLASH_DECODER_TYPE_INTERFACE;
        } else if (DAPLINK_BUILD_KEY_BL == info.build_key) {
            // Bootloader update
            return FLASH_DECODER_TYPE_BOOTLOADER;
        } else {
            return FLASH_DECODER_TYPE_UNKNOWN;
        }
    }

    // Check if a valid vector table for the target can be found
    if (validate_bin_nvic(data)) {
        return FLASH_DECODER_TYPE_TARGET;
    }

    // If an address is specified then the data can be decoded
    if (addr_valid) {
        // TODO - future improvement - make sure address is within target's flash
        return FLASH_DECODER_TYPE_TARGET;
    }

    return FLASH_DECODER_TYPE_UNKNOWN;
}

error_t flash_decoder_open(void)
{
    flash_decoder_printf("flash_decoder_open()\r\n");
    // Stream must not be open already
    if (state != DECODER_STATE_CLOSED) {
        util_assert(0);
        return ERROR_INTERNAL;
    }

    memset(flash_buf, 0xff, sizeof(flash_buf));
    state = DECODER_STATE_OPEN;
    flash_buf_pos = 0;
    initial_offset = 0;
    current_offset = 0;
    offset_to_absolute = 0;
    flash_initialized = false;

    return ERROR_SUCCESS;
}

error_t flash_decoder_write(const uint8_t * data, uint32_t size, uint32_t addr, bool addr_valid)
{
    error_t status;
    flash_decoder_printf("flash_decoder_write(size=0x%x, addr=0x%x, addr_valid=%i)\r\n",
                         size, addr, addr_valid);

    // Set mode if it has not been set
    if (DECODER_STATE_OPEN == state) {
        state = addr_valid ? DECODER_STATE_ADDR : DECODER_STATE_STREAM;
        if (DECODER_STATE_ADDR == state) {
            initial_offset = addr;
        } else {
            initial_offset = 0;
        }
        current_offset = initial_offset;
        flash_decoder_printf("     mode=%s, initial_offset=0x%x\r\n",
                             (DECODER_STATE_STREAM == state) ? "mode_stream" : "mode_addr", initial_offset);
    }

    // Write the data
    status = ERROR_INTERNAL;
    if ((DECODER_STATE_STREAM == state) && !addr_valid) {
        bool sequential = true;
        bool absolute = false;
        status = write_addr(current_offset, data, size, sequential, absolute);
        current_offset += size;
    } else if ((DECODER_STATE_ADDR == state) && addr_valid) {
        bool sequential = addr == current_offset;
        bool absolute = true;
        status = write_addr(addr, data, size, sequential, absolute);
        current_offset += size;
    } else {
        util_assert(0);
    }

    // check for errors
    if (status != ERROR_SUCCESS) {
        state = DECODER_STATE_ERROR;
    }

    return status;
}

static error_t write_addr(uint32_t addr, const uint8_t * data, uint32_t size, bool sequential, bool addr_absolute)
{
    error_t status;

    if (!flash_initialized) {
        uint32_t copy_size;
        flash_decoder_type_t flash_type;
        bool flash_type_known = false;

        // Buffer data until the flash type is known
        if (sequential) {
            // Copy data into buffer
            copy_size = MIN(size, sizeof(flash_buf) - flash_buf_pos);
            memcpy(&flash_buf[flash_buf_pos], data, copy_size);
            flash_buf_pos += copy_size;
            flash_decoder_printf("    buffering %i bytes\r\n", copy_size);

            // Update vars so they no longer include the buffered data
            data += copy_size;
            size -= copy_size;
            addr += copy_size;

            // If enough data has been buffered then determine the type
            if (flash_buf_pos >= sizeof(flash_buf)) {
                util_assert(sizeof(flash_buf) == flash_buf_pos);

                // Determine flash type and get info for it
                flash_type = flash_decoder_detect_type(flash_buf, flash_buf_pos, initial_offset, addr_absolute);
                flash_decoder_printf("    Buffering complete, setting flash_type=%i\r\n", flash_type);
                flash_type_known = true;
            }
        } else {
            flash_type = FLASH_DECODER_TYPE_TARGET;
            flash_decoder_printf("    Non sequential addr, setting flash_type=%i\r\n", flash_type);
            flash_type_known = true;
        }

        // If flash type is known initialize the flash manager
        if (flash_type_known) {
            const flash_intf_t * flash_intf;
            uint32_t flash_start_addr;

            status = get_flash(flash_type, initial_offset, addr_absolute, &flash_start_addr, &flash_intf);
            if (ERROR_SUCCESS != status) {
                return status;
            }
            offset_to_absolute = flash_start_addr - initial_offset;
            flash_decoder_printf("    flash_start_addr=0x%x, offset_to_absolute=0x%x\r\n", flash_start_addr, offset_to_absolute);
            
            // Initialize flash manager
            util_assert(!flash_initialized);
            status = flash_manager_init(flash_intf);
            flash_decoder_printf("    flash_manager_init ret %i\r\n", status);
            if (ERROR_SUCCESS != status) {
                return status;
            }
            flash_initialized = true;
        }
        
        // If flash has been initalized then write out buffered data
        if (flash_initialized) {
            uint32_t absolute_addr = initial_offset + offset_to_absolute;
            status = flash_manager_data(absolute_addr, flash_buf, flash_buf_pos);
            flash_decoder_printf("    Flushing buffer absolute_addr=0x%x, flash_buf_pos=%i, flash_manager_data ret=%i\r\n",
                                     absolute_addr, flash_buf_pos, status);
            if (ERROR_SUCCESS != status) {
                return status;
            }
        }
    }

    // Write data as normal if flash has been initialized
    if (flash_initialized) {
        uint32_t absolute_addr = addr + offset_to_absolute;
        status = flash_manager_data(absolute_addr, data, size);
        flash_decoder_printf("    Writing data, absolute_addr=0x%x, size=0x%x, flash_manager_data ret %i\r\n",
                             absolute_addr, size, status);
        if (ERROR_SUCCESS != status) {
            return status;
        }
    }
    return ERROR_SUCCESS;
}

error_t flash_decoder_close(void)
{
    error_t status = ERROR_SUCCESS;
    flash_decoder_printf("flash_decoder_close()\r\n");

    if (DECODER_STATE_CLOSED == state) {
        util_assert(0);
        return ERROR_INTERNAL;
    }
    state = DECODER_STATE_CLOSED;

    if (flash_initialized) {
        status = flash_manager_uninit();
        flash_decoder_printf("    flash_manager_uninit ret %i\r\n", status);
    }

    return status;
}

static error_t get_flash(flash_decoder_type_t type, uint32_t addr, bool addr_valid, uint32_t * start_addr, const flash_intf_t ** flash_intf)
{
    error_t status = ERROR_SUCCESS;
    uint32_t flash_start_local;
    const flash_intf_t * flash_intf_local = 0;

    *start_addr = 0;
    *flash_intf = 0;

    if (FLASH_DECODER_TYPE_TARGET == type) {
        flash_start_local = target_device.flash_start;
        flash_intf_local = flash_intf_target;
    } else if (!daplink_is_bootloader() && (FLASH_DECODER_TYPE_BOOTLOADER == type)) {
        if (addr_valid && (DAPLINK_ROM_BL_START != addr)) {
            // Address is wrong so display error message
            status = ERROR_FD_BL_UPDT_ADDR_WRONG;
            flash_start_local = 0;
            flash_intf_local = 0;
        } else {
            // Setup for update
            flash_start_local = DAPLINK_ROM_BL_START;
            flash_intf_local = flash_intf_iap_protected;
        }
    } else if (!(daplink_is_interface()) && (FLASH_DECODER_TYPE_INTERFACE == type)) {
        if (addr_valid && (DAPLINK_ROM_IF_START != addr)) {
            // Address is wrong so display error message
            status = ERROR_FD_INTF_UPDT_ADDR_WRONG;
            flash_start_local = 0;
            flash_intf_local = 0;
        } else {
            // Setup for update
            flash_start_local = DAPLINK_ROM_IF_START;
            flash_intf_local = flash_intf_iap_protected;
        }
    } else {
        status = ERROR_FD_UNSUPPORTED_UPDATE;
    }

    if (ERROR_SUCCESS != status) {
        return status;
    }
    if ((0 == flash_intf_local) || (0 == start_addr) || (0 == flash_intf)) {
        util_assert(0);
        return ERROR_INTERNAL;
    }

    *start_addr = flash_start_local;
    *flash_intf = flash_intf_local;

    return status;
}
