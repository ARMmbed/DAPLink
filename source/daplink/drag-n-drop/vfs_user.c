/**
 * @file    vfs_user.c
 * @brief   Implementation of vfs_user.h
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

// Must be bigger than 4x the flash size of the biggest supported
// device.  This is to accomodate for hex file programming.
static const uint32_t disc_size = MB(64);

static uint8_t file_buffer[VFS_SECTOR_SIZE];
static uint32_t remount_count;

static uint32_t get_file_size(vfs_read_cb_t read_func);

static uint32_t read_file_details_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors);
static uint32_t read_file_fail_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors);

void vfs_user_build_filesystem()
{
    uint32_t file_size;
    // Setup the filesystem based on target parameters
    vfs_init(daplink_drive_name, disc_size);
    // DETAILS.TXT
    file_size = get_file_size(read_file_details_txt);
    vfs_create_file("DETAILS TXT", read_file_details_txt, 0, file_size);

    // FAIL.TXT
    if (vfs_mngr_get_transfer_status() != ERROR_SUCCESS) {
        file_size = get_file_size(read_file_fail_txt);
        vfs_create_file("FAIL    TXT", read_file_fail_txt, 0, file_size);
    }
}

// Callback to handle changes to the root directory.  Should be used with vfs_set_file_change_callback
void vfs_user_file_change_handler(const vfs_filename_t filename, vfs_file_change_t change, vfs_file_t file, vfs_file_t new_file_data)
{
    // Allow settings to be changed if automation mode is
    // enabled or if the user is holding the reset button
    bool btn_pressed = !gpio_get_sw_reset();

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
        } else if (!memcmp(filename, "REFRESH ACT", sizeof(vfs_filename_t))) {
            // Remount to update the drive
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
    if (daplink_is_interface() && config_ram_get_hold_in_bl()) {
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
static uint32_t read_file_details_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors)
{
    uint32_t pos;
    const char *mode_str;
    char *buf = (char *)data;

    if (sector_offset != 0) {
        return 0;
    }

    pos = 0;
    pos += util_write_string(buf + pos, "# DAPLink Firmware - see https://mbed.com/daplink\r\n");
    // Unique ID
    pos += util_write_string(buf + pos, "Unique ID: ");
    pos += util_write_string(buf + pos, info_get_unique_id());
    pos += util_write_string(buf + pos, "\r\n");
    // HIC ID
    pos += util_write_string(buf + pos, "HIC ID: ");
    pos += util_write_string(buf + pos, info_get_hic_id());
    pos += util_write_string(buf + pos, "\r\n");
    // Current mode
    mode_str = daplink_is_bootloader() ? "Bootloader" : "Interface";
    pos += util_write_string(buf + pos, "Daplink Mode: ");
    pos += util_write_string(buf + pos, mode_str);
    pos += util_write_string(buf + pos, "\r\n");
    // Current build's version
    pos += util_write_string(buf + pos, mode_str);
    pos += util_write_string(buf + pos, " Version: ");
    pos += util_write_string(buf + pos, info_get_version());
    pos += util_write_string(buf + pos, "\r\n");

    // GIT sha
    pos += util_write_string(buf + pos, "Git SHA: ");
    pos += util_write_string(buf + pos, GIT_COMMIT_SHA);
    pos += util_write_string(buf + pos, "\r\n");
    // Local modifications when making the build
    pos += util_write_string(buf + pos, "Local Mods: ");
    pos += util_write_uint32(buf + pos, GIT_LOCAL_MODS);
    pos += util_write_string(buf + pos, "\r\n");

    // CRC of the bootloader (if there is one)
    if (info_get_bootloader_present()) {
        pos += util_write_string(buf + pos, "Bootloader CRC: 0x");
        pos += util_write_hex32(buf + pos, info_get_crc_bootloader());
        pos += util_write_string(buf + pos, "\r\n");
    }
    return pos;
}

// File callback to be used with vfs_add_file to return file contents
static uint32_t read_file_fail_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors)
{
    const char *contents = (const char *)error_get_string(vfs_mngr_get_transfer_status());
    uint32_t size = strlen(contents);

    if (sector_offset != 0) {
        return 0;
    }

    memcpy(data, contents, size);
    data[size] = '\r';
    size++;
    data[size] = '\n';
    size++;
    return size;
}
