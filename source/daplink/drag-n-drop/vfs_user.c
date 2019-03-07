/**
 * @file    vfs_user.c
 * @brief   Implementation of vfs_user.h
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "stdbool.h"
#include "ctype.h"
#include "string.h"

#include "vfs_manager.h"
#include "macro.h"
#include "error.h"
#include "util.h"
#include "settings.h"
#include "target_reset.h"
#include "daplink.h"
#include "version_git.h"
#include "info.h"
#include "gpio.h"           // for gpio_get_sw_reset
#include "flash_intf.h"     // for flash_intf_target
#include "cortex_m.h"
#include "target_board.h"

// Must be bigger than 4x the flash size of the biggest supported
// device.  This is to accomodate for hex file programming.
static const uint32_t disc_size = MB(64);

static const char mbed_redirect_file[] =
    "<!doctype html>\r\n"
    "<!-- mbed Platform Website and Authentication Shortcut -->\r\n"
    "<html>\r\n"
    "<head>\r\n"
    "<meta charset=\"utf-8\">\r\n"
    "<title>mbed Website Shortcut</title>\r\n"
    "</head>\r\n"
    "<body>\r\n"
    "<script>\r\n"
    "window.location.replace(\"@R\");\r\n"
    "</script>\r\n"
    "</body>\r\n"
    "</html>\r\n";

static const char error_prefix[] = "error: ";
static const char error_type_prefix[] = "type: ";

static const vfs_filename_t assert_file = "ASSERT  TXT";

static uint8_t file_buffer[VFS_SECTOR_SIZE];
static char assert_buf[64 + 1];
static uint16_t assert_line;
static assert_source_t assert_source;
static uint32_t remount_count;

static uint32_t get_file_size(vfs_read_cb_t read_func);

static uint32_t read_file_mbed_htm(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors);
static uint32_t read_file_details_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors);
static uint32_t read_file_fail_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors);
static uint32_t read_file_assert_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors);
static uint32_t read_file_need_bl_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors);

static uint32_t update_html_file(uint8_t *data, uint32_t datasize);
static uint32_t update_details_txt_file(uint8_t *data, uint32_t datasize);
static void erase_target(void);

static uint32_t expand_info(uint8_t *buf, uint32_t bufsize);

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

void vfs_user_build_filesystem()
{
    uint32_t file_size;
    vfs_file_t file_handle;
    // Setup the filesystem based on target parameters
    vfs_init(get_daplink_drive_name(), disc_size);
    // MBED.HTM
    file_size = get_file_size(read_file_mbed_htm);
    vfs_create_file(get_daplink_url_name(), read_file_mbed_htm, 0, file_size);
    // DETAILS.TXT
    file_size = get_file_size(read_file_details_txt);
    vfs_create_file("DETAILS TXT", read_file_details_txt, 0, file_size);

    // FAIL.TXT
    if (vfs_mngr_get_transfer_status() != ERROR_SUCCESS) {
        file_size = get_file_size(read_file_fail_txt);
        vfs_create_file("FAIL    TXT", read_file_fail_txt, 0, file_size);
    }

    // ASSERT.TXT
    if (config_ram_get_assert(assert_buf, sizeof(assert_buf), &assert_line, &assert_source)) {
        file_size = get_file_size(read_file_assert_txt);
        file_handle = vfs_create_file(assert_file, read_file_assert_txt, 0, file_size);
        vfs_file_set_attr(file_handle, (vfs_file_attr_bit_t)0); // Remove read only attribute
    }

    // NEED_BL.TXT
    volatile uint32_t bl_start = DAPLINK_ROM_BL_START; // Silence warnings about null pointer
    volatile uint32_t if_start = DAPLINK_ROM_IF_START; // Silence warnings about null pointer

    if (daplink_is_interface() &&
            (DAPLINK_ROM_BL_SIZE > 0) &&
            (0 == memcmp((void *)bl_start, (void *)if_start, DAPLINK_MIN_WRITE_SIZE))) {
        // If the bootloader contains a copy of the interfaces vector table
        // then an error occurred when updating so warn that the bootloader is
        // missing.
        file_size = get_file_size(read_file_need_bl_txt);
        vfs_create_file("NEED_BL TXT", read_file_need_bl_txt, 0, file_size);
    }
}

// Callback to handle changes to the root directory.  Should be used with vfs_set_file_change_callback
void vfs_user_file_change_handler(const vfs_filename_t filename, vfs_file_change_t change, vfs_file_t file, vfs_file_t new_file_data)
{
    // Allow settings to be changed if automation mode is
    // enabled or if the user is holding the reset button
    bool btn_pressed = gpio_get_reset_btn();

    if (!btn_pressed && !config_get_automation_allowed()) {
        return;
    }

    if (VFS_FILE_CHANGED == change) {
        // Unused
    }

    if (VFS_FILE_CREATED == change) {
        if (!memcmp(filename, daplink_mode_file_name, sizeof(vfs_filename_t))) {
            if (daplink_is_interface()) {
                config_ram_set_hold_in_bl(true);
            } else {
                // Do nothing - bootloader will go to interface by default
            }

            vfs_mngr_fs_remount();
        } else if (!memcmp(filename, "AUTO_RSTCFG", sizeof(vfs_filename_t))) {
            config_set_auto_rst(true);
            vfs_mngr_fs_remount();
        } else if (!memcmp(filename, "HARD_RSTCFG", sizeof(vfs_filename_t))) {
            config_set_auto_rst(false);
            vfs_mngr_fs_remount();
        } else if (!memcmp(filename, "ASSERT  ACT", sizeof(vfs_filename_t))) {
            // Test asserts
            util_assert(0);
        } else if (!memcmp(filename, "REFRESH ACT", sizeof(vfs_filename_t))) {
            // Remount to update the drive
            vfs_mngr_fs_remount();
        } else if (!memcmp(filename, "AUTO_ON CFG", sizeof(vfs_filename_t))) {
            config_set_automation_allowed(true);
            vfs_mngr_fs_remount();
        } else if (!memcmp(filename, "AUTO_OFFCFG", sizeof(vfs_filename_t))) {
            config_set_automation_allowed(false);
            vfs_mngr_fs_remount();
        } else if (!memcmp(filename, "ERASE   ACT", sizeof(vfs_filename_t))) {
            erase_target();
            vfs_mngr_fs_remount();
        } else if (!memcmp(filename, "OVFL_ON CFG", sizeof(vfs_filename_t))) {
            config_set_overflow_detect(true);
            vfs_mngr_fs_remount();
        } else if (!memcmp(filename, "OVFL_OFFCFG", sizeof(vfs_filename_t))) {
            config_set_overflow_detect(false);
            vfs_mngr_fs_remount();
        } else if (!memcmp(filename, "MSD_ON  ACT", sizeof(vfs_filename_t))) {
            config_ram_set_disable_msd(false);
            vfs_mngr_fs_remount();
        } else if (!memcmp(filename, "MSD_OFF ACT", sizeof(vfs_filename_t))) {
            config_ram_set_disable_msd(true);
            vfs_mngr_fs_remount();
        }
    }

    if (VFS_FILE_DELETED == change) {
        if (!memcmp(filename, assert_file, sizeof(vfs_filename_t))) {
            // Clear assert and remount to update the drive
            util_assert_clear();
            vfs_mngr_fs_remount();
        }
    }
}

void vfs_user_disconnecting()
{
    // Reset if programming was successful  //TODO - move to flash layer
    if (daplink_is_bootloader() && (ERROR_SUCCESS == vfs_mngr_get_transfer_status())) {
        SystemReset();
    }

    // If hold in bootloader has been set then reset after usb is disconnected
    if (daplink_is_interface() && (config_ram_get_hold_in_bl() || config_ram_get_disable_msd()==1)) {
        SystemReset();
    }

    remount_count++;
}

// Get the filesize from a filesize callback.
// The file data must be null terminated for this to work correctly.
static uint32_t get_file_size(vfs_read_cb_t read_func)
{
    // Determine size of the file by faking a read
    return read_func(0, file_buffer, 1);
}

// File callback to be used with vfs_add_file to return file contents
static uint32_t read_file_mbed_htm(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors)
{
    if (sector_offset != 0) {
        return 0;
    }

    return update_html_file(data, VFS_SECTOR_SIZE);
}

// File callback to be used with vfs_add_file to return file contents
static uint32_t read_file_details_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors)
{

    if (sector_offset != 0) {
        return 0;
    }

    return update_details_txt_file(data, VFS_SECTOR_SIZE);
}

// Text representation of each error type, starting from the rightmost bit
static const char* const error_type_names[] = {
    "internal",
    "transient",
    "user",
    "target",
    "interface"
};

COMPILER_ASSERT(1 << ARRAY_SIZE(error_type_names) == ERROR_TYPE_MASK + 1);

// File callback to be used with vfs_add_file to return file contents
static uint32_t read_file_fail_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors)
{
    uint32_t size = 0;
    char *buf = (char *)data;
    error_t status = vfs_mngr_get_transfer_status();
    const char *contents = error_get_string(status);
    error_type_t type = error_get_type(status);

    if (sector_offset != 0) {
        return 0;
    }

    size += util_write_string(buf + size, error_prefix);
    size += util_write_string(buf + size, contents);
    size += util_write_string(buf + size, "\r\n");
    size += util_write_string(buf + size, error_type_prefix);

    // Write each applicable error type, separated by commas
    int index = 0;
    bool first = true;
    while (type && index < ARRAY_SIZE(error_type_names)) {
        if (!first) {
            size += util_write_string(buf + size, ", ");
        }
        if (type & 1) {
            size += util_write_string(buf + size, error_type_names[index]);
            first = false;
        }
        index++;
        type >>= 1;
    }

    size += util_write_string(buf + size, "\r\n");
    return size;
}

// File callback to be used with vfs_add_file to return file contents
static uint32_t read_file_assert_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors)
{
    uint32_t pos;
    const char *source_str;
    char *buf = (char *)data;
    uint32_t * hexdumps = 0;
    uint8_t valid_hexdumps = 0;
    uint8_t index = 0;
    
    if (sector_offset != 0) {
        return 0;
    }

    pos = 0;

    if (ASSERT_SOURCE_BL == assert_source) {
        source_str = "Bootloader";
    } else if (ASSERT_SOURCE_APP == assert_source) {
        source_str = "Application";
    } else {
        source_str = 0;
    }

    pos += util_write_string(buf + pos, "Assert\r\n");
    pos += util_write_string(buf + pos, "File: ");
    pos += util_write_string(buf + pos, assert_buf);
    pos += util_write_string(buf + pos, "\r\n");
    pos += util_write_string(buf + pos, "Line: ");
    pos += util_write_uint32(buf + pos, assert_line);
    pos += util_write_string(buf + pos, "\r\n");

    if (source_str != 0) {
        pos += util_write_string(buf + pos, "Source: ");
        pos += util_write_string(buf + pos, source_str);
        pos += util_write_string(buf + pos, "\r\n");
    }

    valid_hexdumps = config_ram_get_hexdumps(&hexdumps);
    if ((valid_hexdumps > 0) && (hexdumps != 0)) {
        //print hexdumps
        pos += util_write_string(buf + pos, "Hexdumps\r\n");
        while ((index < valid_hexdumps) && ((pos + 10) < VFS_SECTOR_SIZE)) { //hexdumps + newline is always 10 characters
            pos += util_write_hex32(buf + pos, hexdumps[index++]);
            pos += util_write_string(buf + pos, "\r\n");
        }
    }
    
    return pos;
}

// File callback to be used with vfs_add_file to return file contents
static uint32_t read_file_need_bl_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors)
{
    const char *contents = "A bootloader update was started but unable to complete.\r\n"
                           "Reload the bootloader to fix this error message.\r\n";
    uint32_t size = strlen(contents);

    if (sector_offset != 0) {
        return 0;
    }

    memcpy(data, contents, size);
    return size;
}


static uint32_t update_html_file(uint8_t *data, uint32_t datasize)
{
    char *buf = (char *)data;
    //Needed by expand_info strlen
    memset(buf, 0, datasize);
    memcpy(buf, mbed_redirect_file, strlen(mbed_redirect_file));
    //expand
    return expand_info(data, datasize);
}

static uint32_t update_details_txt_file(uint8_t *data, uint32_t datasize)
{
    uint32_t pos=0;
    const char *mode_str;

    char *buf = (char *)data;

    //Needed by expand_info strlen
    memset(buf, 0, datasize);

    pos += util_write_string(buf + pos, "# DAPLink Firmware - see https://mbed.com/daplink\r\n");
    // Unique ID
    pos += util_write_string(buf + pos, "Unique ID: @U\r\n");
    // HIC ID
    pos += util_write_string(buf + pos, "HIC ID: @D\r\n");
    // Settings
    pos += util_write_string(buf + pos, "Auto Reset: ");
    pos += util_write_string(buf + pos, config_get_auto_rst() ? "1" : "0");
    pos += util_write_string(buf + pos, "\r\n");
    pos += util_write_string(buf + pos, "Automation allowed: ");
    pos += util_write_string(buf + pos, config_get_automation_allowed() ? "1" : "0");
    pos += util_write_string(buf + pos, "\r\n");
    pos += util_write_string(buf + pos, "Overflow detection: ");
    pos += util_write_string(buf + pos, config_get_overflow_detect() ? "1" : "0");
    pos += util_write_string(buf + pos, "\r\n");
    // Current mode
    mode_str = daplink_is_bootloader() ? "Bootloader" : "Interface";
    pos += util_write_string(buf + pos, "Daplink Mode: ");
    pos += util_write_string(buf + pos, mode_str);
    pos += util_write_string(buf + pos, "\r\n");
    // Current build's version
    pos += util_write_string(buf + pos, mode_str);
    pos += util_write_string(buf + pos, " Version: @V\r\n");

    // Other builds version (bl or if)
    if (!daplink_is_bootloader() && info_get_bootloader_present()) {
        pos += util_write_string(buf + pos, "Bootloader Version: ");
        pos += util_write_uint32_zp(buf + pos, info_get_bootloader_version(), 4);
        pos += util_write_string(buf + pos, "\r\n");
    }

    if (!daplink_is_interface() && info_get_interface_present()) {
        pos += util_write_string(buf + pos, "Interface Version: ");
        pos += util_write_uint32_zp(buf + pos, info_get_interface_version(), 4);
        pos += util_write_string(buf + pos, "\r\n");
    }

    // GIT sha
    pos += util_write_string(buf + pos, "Git SHA: ");
    pos += util_write_string(buf + pos, GIT_COMMIT_SHA);
    pos += util_write_string(buf + pos, "\r\n");
    // Local modifications when making the build
    pos += util_write_string(buf + pos, "Local Mods: ");
    pos += util_write_uint32(buf + pos, GIT_LOCAL_MODS);
    pos += util_write_string(buf + pos, "\r\n");
    // Supported USB endpoints
    pos += util_write_string(buf + pos, "USB Interfaces: ");
#ifdef MSC_ENDPOINT
    pos += util_write_string(buf + pos, "MSD");
#endif
#ifdef CDC_ENDPOINT
    pos += util_write_string(buf + pos, ", CDC");
#endif
#ifdef HID_ENDPOINT
    pos += util_write_string(buf + pos, ", HID");
#endif
#if (WEBUSB_INTERFACE)
    pos += util_write_string(buf + pos, ", WebUSB");
#endif
    pos += util_write_string(buf + pos, "\r\n");

    // CRC of the bootloader (if there is one)
    if (info_get_bootloader_present()) {
        pos += util_write_string(buf + pos, "Bootloader CRC: 0x");
        pos += util_write_hex32(buf + pos, info_get_crc_bootloader());
        pos += util_write_string(buf + pos, "\r\n");
    }

    // CRC of the interface
    pos += util_write_string(buf + pos, "Interface CRC: 0x");
    pos += util_write_hex32(buf + pos, info_get_crc_interface());
    pos += util_write_string(buf + pos, "\r\n");

    // Number of remounts that have occurred
    pos += util_write_string(buf + pos, "Remount count: ");
    pos += util_write_uint32(buf + pos, remount_count);
    pos += util_write_string(buf + pos, "\r\n");

    //Target URL
    pos += util_write_string(buf + pos, "URL: @R\r\n");

    return expand_info(data, datasize);
}

// Fill buf with the contents of the mbed redirect file by
// expanding the special characters in mbed_redirect_file.
static uint32_t expand_info(uint8_t *buf, uint32_t bufsize)
{
    uint8_t *orig_buf = buf;
    uint8_t *insert_string;

    do {
        // Look for key or the end of the string
        while ((*buf != '@') && (*buf != 0)) {
            buf++;
        }

        // If key was found then replace it
        if ('@' == *buf) {
            switch (*(buf + 1)) {
                case 'm':
                case 'M':   // MAC address
                    insert_string = (uint8_t *)info_get_mac();
                    break;

                case 'u':
                case 'U':   // UUID
                    insert_string = (uint8_t *)info_get_unique_id();
                    break;

                case 'b':
                case 'B':   // Board ID
                    insert_string = (uint8_t *)info_get_board_id();
                    break;

                case 'h':
                case 'H':   // Host ID
                    insert_string = (uint8_t *)info_get_host_id();
                    break;

                case 't':
                case 'T':   // Target ID
                    insert_string = (uint8_t *)info_get_target_id();
                    break;

                case 'd':
                case 'D':   // HIC
                    insert_string = (uint8_t *)info_get_hic_id();
                    break;

                case 'v':
                case 'V':   // Firmware version
                    insert_string = (uint8_t *)info_get_version();
                    break;

                case 'r':
                case 'R':   // URL replacement
                    insert_string = (uint8_t *)get_daplink_target_url();
                    break;

                default:
                    insert_string = (uint8_t *)"ERROR";
                    break;
            }

            // Remove strip_count characters from the start of buf and then insert
            // insert_string at the new start of buf.
            uint32_t buf_len = strlen((const char *)buf);
            uint32_t str_len = strlen((const char *)insert_string);
            //buffer overflow check on insert
            if( (buf + str_len + buf_len - 2) < (orig_buf+bufsize)){
                // push out string
                memmove(buf + str_len, buf + 2, buf_len - 2);
                // insert
                memcpy(buf, insert_string, str_len);
            }else{
                //stop the string expansion and leave as it is
                buf += buf_len;
                break;
            }

        }
    } while (*buf != '\0');

    return (buf - orig_buf);
}

// Initialize flash algo, erase flash, uninit algo
static void erase_target(void)
{
    flash_intf_target->init();
    flash_intf_target->erase_chip();
    flash_intf_target->uninit();
}
