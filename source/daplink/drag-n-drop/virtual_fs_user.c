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

#include <stdbool.h>
#include <ctype.h>

#include "main.h"
#include "RTL.h"
#include "rl_usb.h"
#include "virtual_fs.h"
#include "daplink_debug.h"
#include "validation.h"
#include "info.h"
#include "config_settings.h"
#include "daplink.h"
#include "virtual_fs_user.h"
#include "util.h"
#include "version_git.h"
#include "IO_config.h"
#include "target_reset.h"
#include "file_stream.h"
#include "error.h"

// Set to 1 to enable debugging
#define DEBUG_VIRTUAL_FS_USER     0

#if DEBUG_VIRTUAL_FS_USER
    #define vfs_user_printf    debug_msg
#else
    #define vfs_user_printf(...)
#endif

typedef struct {
    vfs_file_t file_to_program;     // A pointer to the directory entry of the file being programmed
    vfs_sector_t start_sector;      // Start sector of the file being programmed
    vfs_sector_t file_next_sector;  // Expected next sector of the file
    uint32_t size_processed;        // Size of the file read so far
    uint32_t file_size;             // Size of the file indicated by root dir.  Only allowed to increase
    bool transfer_finished;         // Transfer done, ignore all further writes
    bool stream_open;               // State of the stream
    bool stream_started;            // Stream processing started. This only gets reset remount
    bool stream_finished;           // Stream processing is done. This only gets reset remount
    bool stream_optional_finish;    // True if the stream processing can be considered done
    bool file_info_optional_finish; // True if the file transfer can be considered done
    bool transfer_timeout;          // Set if the transfer was finished because of a timeout. This only gets reset remount
    stream_type_t stream;           // Current stream or STREAM_TYPE_NONE is stream is closed.  This only gets reset remount
} file_transfer_state_t;

typedef enum {
    VFS_USER_STATE_DISCONNECTED,
    VFS_USER_STATE_RECONNECTING,
    VFS_USER_STATE_CONNECTED
} vfs_user_state_t;

// Must be bigger than 4x the flash size of the biggest supported
// device.  This is to accomidate for hex file programming.
static const uint32_t disc_size = MB(8);

static const file_transfer_state_t default_transfer_state = {
    VFS_FILE_INVALID,
    VFS_INVALID_SECTOR,
    VFS_INVALID_SECTOR,
    0,
    0,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    STREAM_TYPE_NONE,
};

static const uint8_t mbed_redirect_file[512] =
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

static const vfs_filename_t assert_file = "ASSERT  TXT";
static const uint32_t connect_delay_ms = 0;
static const uint32_t disconnect_delay_ms = 500;
static const uint32_t reconnect_delay_ms = 2500;    // Must be above 1s for windows (more for linux)

static uint32_t usb_buffer[VFS_SECTOR_SIZE/sizeof(uint32_t)];
static error_t fail_reason = ERROR_SUCCESS;
static file_transfer_state_t file_transfer_state;
static char assert_buf[64 + 1];
static uint16_t assert_line;

// These variables can be access from multiple threads
// so access to them must be synchronized
static vfs_user_state_t vfs_state;
static vfs_user_state_t vfs_state_next;
static uint32_t vfs_state_remaining_ms;

static OS_MUT sync_mutex;
static OS_TID sync_thread = 0;

// Synchronization functions
static void sync_init(void);
static void sync_assert_usb_thread(void);
static void sync_lock(void);
static void sync_unlock(void);

static void vfs_user_disconnect_delay(void);
static bool changing_state(void);
static void build_filesystem(void);
static void file_change_handler(const vfs_filename_t filename, vfs_file_change_t change, vfs_file_t file, vfs_file_t new_file_data);
static void file_data_handler(uint32_t sector, const uint8_t *buf, uint32_t num_of_sectors);
static uint32_t get_file_size(vfs_read_cb_t read_func);

static uint32_t read_file_mbed_htm(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors);
static uint32_t read_file_details_txt(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors);
static uint32_t read_file_fail_txt(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors);
static uint32_t read_file_assert_txt(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors);

static void transfer_update_file_info(vfs_file_t file, uint32_t start_sector, uint32_t size, stream_type_t stream);
static void transfer_stream_open(stream_type_t stream, uint32_t start_sector);
static void transfer_stream_data(uint32_t sector, const uint8_t * data, uint32_t size);
static void transfer_update_state(error_t status);

static void insert(uint8_t *buf, uint8_t* new_str, uint32_t strip_count);
static void update_html_file(uint8_t *buf, uint32_t bufsize);


// Rebuild the virtual filesystem.  This must only be called
// when mass storage is inactive.
void vfs_user_enable(bool enable)
{
    sync_lock();
    if (enable) {
        if (VFS_USER_STATE_DISCONNECTED == vfs_state_next) {
            vfs_state_remaining_ms = connect_delay_ms;
            vfs_state_next = VFS_USER_STATE_CONNECTED;
        }
    } else {
        vfs_state_remaining_ms = disconnect_delay_ms;
        vfs_state_next = VFS_USER_STATE_DISCONNECTED;
    }
    sync_unlock();
}

void vfs_user_remount(void)
{
    sync_lock();
    // Only start a remount if in the connected state and not in a transition
    if (!changing_state() && (VFS_USER_STATE_CONNECTED == vfs_state)) {
        vfs_state_next = VFS_USER_STATE_RECONNECTING;
        vfs_state_remaining_ms = disconnect_delay_ms;
    }
    sync_unlock();
}

void vfs_user_init(bool enable)
{
    sync_assert_usb_thread();

    build_filesystem();
    if (enable) {
        vfs_state = VFS_USER_STATE_CONNECTED;
        vfs_state_next = VFS_USER_STATE_CONNECTED;
        USBD_MSC_MediaReady = 1;
    } else {
        vfs_state = VFS_USER_STATE_DISCONNECTED;
        vfs_state_next = VFS_USER_STATE_DISCONNECTED;
        USBD_MSC_MediaReady = 0;
    }
}

void vfs_user_periodic(uint32_t elapsed_ms)
{
    vfs_user_state_t vfs_state_local;
    vfs_user_state_t vfs_state_local_prev;
    sync_assert_usb_thread();
    sync_lock();

    // Return immediately if the desired state has been reached
    if (!changing_state()) {
        sync_unlock();
        return;
    }

    // Wait until the required amount of time has passed
    // before changing state
    if (vfs_state_remaining_ms > 0) {
        vfs_state_remaining_ms -= MIN(elapsed_ms, vfs_state_remaining_ms);
        sync_unlock();
        return;
    }

    vfs_user_printf("vfs_user_periodic()\r\n");

    // Transistion to new state
    vfs_state_local_prev = vfs_state;
    vfs_state = vfs_state_next;
    switch (vfs_state) {
        case VFS_USER_STATE_RECONNECTING:
            // Transition back to the connected state
            vfs_state_next = VFS_USER_STATE_CONNECTED;
            vfs_state_remaining_ms = reconnect_delay_ms;
            break;
        default:
            // No state change logic required in other states
            break;
    }
    vfs_state_local = vfs_state;
    sync_unlock();

    // Processing when leaving a state
    vfs_user_printf("    state %i->%i\r\n", vfs_state_local_prev, vfs_state_local);
    switch (vfs_state_local_prev) {
        case VFS_USER_STATE_DISCONNECTED:
            // No action needed
            break;
        case VFS_USER_STATE_RECONNECTING:
            // No action needed
            break;
        case VFS_USER_STATE_CONNECTED:
            // Close ongoing transfer if there is one
            if (!file_transfer_state.transfer_finished) {
                vfs_user_printf("    transfer timeout\r\n");
                file_transfer_state.transfer_timeout = true;
                transfer_update_state(ERROR_SUCCESS);
            }
            util_assert(file_transfer_state.transfer_finished);
            // Reset if programming was successful  //TODO - move to flash layer
            if (daplink_is_bootloader() && (ERROR_SUCCESS == fail_reason)) {
                NVIC_SystemReset();
            }
            // If hold in bootloader has been set then reset after usb is disconnected
            if (daplink_is_interface() && config_ram_get_hold_in_bl()) {
                NVIC_SystemReset();
            }
            // Resume the target if configured to do so //TODO - move to flash layer
            if (config_get_auto_rst()) {
                target_set_state(RESET_RUN);
            }
            break;
    }

    // Processing when entering a state
    switch (vfs_state_local) {
        case VFS_USER_STATE_DISCONNECTED:
            USBD_MSC_MediaReady = 0;
            break;
        case VFS_USER_STATE_RECONNECTING:
            USBD_MSC_MediaReady = 0;
            break;
        case VFS_USER_STATE_CONNECTED:
            build_filesystem();
            USBD_MSC_MediaReady = 1;
            break;
    }
    return;
}

void usbd_msc_init(void)
{
    sync_init();
    build_filesystem();
    vfs_state = VFS_USER_STATE_DISCONNECTED;
    vfs_state_next = VFS_USER_STATE_DISCONNECTED;
    vfs_state_remaining_ms = 0;
    USBD_MSC_MediaReady = 0;
}

void usbd_msc_read_sect(uint32_t sector, uint8_t *buf, uint32_t num_of_sectors)
{
    sync_assert_usb_thread();

    // dont proceed if we're not ready
    if (!USBD_MSC_MediaReady) {
        return;
    }

    // indicate msc activity
    main_blink_msc_led(MAIN_LED_OFF);

    vfs_read(sector, buf, num_of_sectors);
}

void usbd_msc_write_sect(uint32_t sector, uint8_t *buf, uint32_t num_of_sectors)
{
    sync_assert_usb_thread();

    if (!USBD_MSC_MediaReady) {
        return;
    }

    // Restart the disconnect counter on every packet
    // so the device does not detach in the middle of a
    // transfer.
    vfs_user_disconnect_delay();

    if (file_transfer_state.transfer_finished) {
        return;
    }

    // indicate msc activity
    main_blink_msc_led(MAIN_LED_OFF);

    vfs_write(sector, buf, num_of_sectors);
    file_data_handler(sector, buf, num_of_sectors);
}

static void sync_init(void)
{
    sync_thread = os_tsk_self();
    os_mut_init(&sync_mutex);
}

static void sync_assert_usb_thread(void)
{
    util_assert(os_tsk_self() == sync_thread);
}

static void sync_lock(void)
{
    os_mut_wait(&sync_mutex, 0xFFFF);
}

static void sync_unlock(void)
{
    os_mut_release(&sync_mutex);
}

static void vfs_user_disconnect_delay()
{
    if (VFS_USER_STATE_CONNECTED == vfs_state) {
        vfs_state_remaining_ms = disconnect_delay_ms;
    }
}

static bool changing_state()
{
    return vfs_state != vfs_state_next;
}

static void build_filesystem()
{
    uint32_t file_size;

    // Update anything that could have changed file system state
    file_transfer_state = default_transfer_state;

    // Setup the filesystem based on target parameters
    vfs_init(daplink_drive_name, disc_size);
    vfs_set_file_change_callback(file_change_handler);

    // MBED.HTM
    file_size = get_file_size(read_file_mbed_htm);
    vfs_create_file(daplink_url_name, read_file_mbed_htm, 0, file_size);

    // DETAILS.TXT
    file_size = get_file_size(read_file_details_txt);
    vfs_create_file("DETAILS TXT", read_file_details_txt, 0, file_size);

    // FAIL.TXT
    if (fail_reason != ERROR_SUCCESS) {
        file_size = get_file_size(read_file_fail_txt);
        vfs_create_file("FAIL    TXT", read_file_fail_txt, 0, file_size);
    }

    // ASSERT.TXT
    if (config_ram_get_assert(assert_buf, sizeof(assert_buf), &assert_line)) {
        file_size = get_file_size(read_file_assert_txt);
        vfs_create_file(assert_file, read_file_assert_txt, 0, file_size);
    }

    // Set mass storage parameters
    USBD_MSC_MemorySize = vfs_get_total_size();
    USBD_MSC_BlockSize  = VFS_SECTOR_SIZE;
    USBD_MSC_BlockGroup = 1;
    USBD_MSC_BlockCount = USBD_MSC_MemorySize / USBD_MSC_BlockSize;
    USBD_MSC_BlockBuf   = (uint8_t *)usb_buffer;
}

// Callback to handle changes to the root directory.  Should be used with vfs_set_file_change_callback
static void file_change_handler(const vfs_filename_t filename, vfs_file_change_t change, vfs_file_t file, vfs_file_t new_file_data)
{
    vfs_user_printf("virtual_fs_user file_change_handler(name=%*s, file=%p, change=%i)\r\n", 11, filename, file, change);

    if (VFS_FILE_CHANGED == change) {
        if (file == file_transfer_state.file_to_program) {
            stream_type_t stream;
            uint32_t size = vfs_file_get_size(new_file_data);
            vfs_sector_t sector = vfs_file_get_start_sector(new_file_data);
            stream = stream_type_from_name(filename);
            transfer_update_file_info(file, sector, size, stream);
        }
    }

    if (VFS_FILE_CREATED == change) {
        stream_type_t stream;
        if (!memcmp(filename, daplink_mode_file_name, sizeof(vfs_filename_t))) {
            if (daplink_is_interface()) {
                config_ram_set_hold_in_bl(true);
            } else {
                // Do nothing - bootloader will go to interface by default
            }
            vfs_user_remount();
        } else if (!memcmp(filename, "AUTO_RSTCFG", sizeof(vfs_filename_t))) {
            config_set_auto_rst(true);
            vfs_user_remount();
        } else if (!memcmp(filename, "HARD_RSTCFG", sizeof(vfs_filename_t))) {
            config_set_auto_rst(false);
            vfs_user_remount();
        } else if (!memcmp(filename, "ASSERT  ACT", sizeof(vfs_filename_t))) {
            // Test asserts
            util_assert(0);
        } else if (!memcmp(filename, "REFRESH ACT", sizeof(vfs_filename_t))) {
            // Remount to update the drive
            vfs_user_remount();
        } else if (STREAM_TYPE_NONE != stream_type_from_name(filename)) {
            stream = stream_type_from_name(filename);
            uint32_t size = vfs_file_get_size(new_file_data);
            vfs_sector_t sector = vfs_file_get_start_sector(new_file_data);
            transfer_update_file_info(file, sector, size, stream);
        } 
    }

    if (VFS_FILE_DELETED == change) {
        if (!memcmp(filename, assert_file, sizeof(vfs_filename_t))) {
            util_assert_clear();
        }
    }
}

// Handler for file data arriving over USB.  This function is responsible
// for detecting the start of a BIN/HEX file and performing programming
static void file_data_handler(uint32_t sector, const uint8_t *buf, uint32_t num_of_sectors)
{
    stream_type_t stream;
    uint32_t size;

    // this is the key for starting a file write - we dont care what file types are sent
    //  just look for something unique (NVIC table, hex, srec, etc) until root dir is updated
    if (!file_transfer_state.stream_started) {
        // look for file types we can program
        stream = stream_start_identify((uint8_t*)buf, VFS_SECTOR_SIZE * num_of_sectors);
        if (STREAM_TYPE_NONE != stream) {
            transfer_stream_open(stream, sector);
        }
    }

    if (file_transfer_state.stream_started) {

        // Ignore sectors coming before this file
        if (sector < file_transfer_state.start_sector) {
            return;
        }

        // sectors must be in order
        if (sector != file_transfer_state.file_next_sector) {
            vfs_user_printf("    SECTOR OUT OF ORDER\r\n");
            return;
        }

        size = VFS_SECTOR_SIZE * num_of_sectors;
        transfer_stream_data(sector, buf, size);
    }
}

// Get the filesize from a filesize callback.
// The file data must be null terminated for this to work correctly.
static uint32_t get_file_size(vfs_read_cb_t read_func)
{
    // USB buffer must not be in use when get_file_size is called
    static uint8_t * dummy_buffer = (uint8_t *)usb_buffer;

    // Determine size of the file by faking a read
    return read_func(0, dummy_buffer, 1);
}

// File callback to be used with vfs_add_file to return file contents
static uint32_t read_file_mbed_htm(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors)
{
    if (sector_offset != 0) {
        return 0;
    }
    update_html_file(data, VFS_SECTOR_SIZE);
    return strlen((const char *)data);
}

// File callback to be used with vfs_add_file to return file contents
static uint32_t read_file_details_txt(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors)
{
    uint32_t pos;
    const char * mode_str;
    char * buf = (char *)data;
    if (sector_offset != 0) {
        return 0;
    }
    
    pos = 0;
    pos += util_write_string(buf + pos, "# DAPLink Firmware - see https://mbed.com/daplink\r\n");

    // Unique ID
    pos += util_write_string(buf + pos, "Unique ID: ");
    pos += util_write_string(buf + pos, info_get_unique_id());
    pos += util_write_string(buf + pos, "\r\n");

    // HDK ID
    pos += util_write_string(buf + pos, "HDK ID: ");
    pos += util_write_string(buf + pos, info_get_hdk_id());
    pos += util_write_string(buf + pos, "\r\n");

    // Settings
    pos += util_write_string(buf + pos, "Auto Reset: ");
    pos += util_write_string(buf + pos, config_get_auto_rst() ? "1" : "0");
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
    #ifdef HID_ENDPOINT
    pos += util_write_string(buf + pos, ", CDC");
    #endif
    #ifdef CDC_ENDPOINT
    pos += util_write_string(buf + pos, ", CMSIS-DAP");
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

    return pos;
}

// File callback to be used with vfs_add_file to return file contents
static uint32_t read_file_fail_txt(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors)
{
    const char* contents = (const char*)error_get_string(fail_reason);
    uint32_t size = strlen(contents);
    if (sector_offset != 0) {
        return 0;
    }
    memcpy(data, contents, size);
    return size;
}

// File callback to be used with vfs_add_file to return file contents
static uint32_t read_file_assert_txt(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors)
{
    uint32_t pos;
    char * buf = (char *)data;
    if (sector_offset != 0) {
        return 0;
    }
    pos = 0;
    
    pos += util_write_string(buf + pos, "Assert\r\n");
    pos += util_write_string(buf + pos, "File: ");
    pos += util_write_string(buf + pos, assert_buf);
    pos += util_write_string(buf + pos, "\r\n");
    pos += util_write_string(buf + pos, "Line: ");
    pos += util_write_uint32(buf + pos, assert_line);
    pos += util_write_string(buf + pos, "\r\n");

    return pos;
}

// Update the tranfer state with file information
static void transfer_update_file_info(vfs_file_t file, uint32_t start_sector, uint32_t size, stream_type_t stream)
{
    vfs_user_printf("virtual_fs_user transfer_update_file_info(file=%p, start_sector=%i, size=%i)\r\n", file, start_sector, size);
    if (file_transfer_state.transfer_finished) {
        util_assert(0);
        return;
    }

    // Initialize the directory entry if it has not been set
    if (VFS_FILE_INVALID == file_transfer_state.file_to_program) {
        file_transfer_state.file_to_program = file;
        if (file != VFS_FILE_INVALID) {
            vfs_user_printf("    file_to_program=%p\r\n", file);
        }
    }
    // Initialize the starting sector if it has not been set
    if (VFS_INVALID_SECTOR == file_transfer_state.start_sector) {
        file_transfer_state.start_sector = start_sector;
        if (start_sector != VFS_INVALID_SECTOR) {
            vfs_user_printf("    start_sector=%i\r\n", start_sector);
        }
    }
    // Initialize the stream if it has not been set
    if (STREAM_TYPE_NONE == file_transfer_state.stream) {
        file_transfer_state.stream = stream;
        if (stream != STREAM_TYPE_NONE) {
            vfs_user_printf("    stream=%i\r\n", stream);
        }
    }

    // Check - File size must be the same or bigger
    if (size < file_transfer_state.file_size) {
        vfs_user_printf("    error: file size changed from %i to %i\r\n", file_transfer_state.file_size, size);
        transfer_update_state(ERROR_ERROR_DURING_TRANSFER);
        return;
    }
    // Check - Starting sector must be the same  - this is optional for file info since it may not be present initially
    if ((VFS_INVALID_SECTOR != start_sector) && (start_sector != file_transfer_state.start_sector)) {
        vfs_user_printf("    error: starting sector changed from %i to %i\r\n", file_transfer_state.start_sector, start_sector);
        transfer_update_state(ERROR_ERROR_DURING_TRANSFER);
        return;
    }
    // Check - stream must be the same
    if (stream != file_transfer_state.stream) {
        vfs_user_printf("    error: changed types during transfer from %i to %i\r\n", stream, file_transfer_state.stream);
        transfer_update_state(ERROR_ERROR_DURING_TRANSFER);
        return;
    }

    // Update values - Size is the only value that can change and it can only increase.
    if (size > file_transfer_state.file_size) {
        file_transfer_state.file_size = size;
        vfs_user_printf("    updated size=%i\r\n", size);
    }

    transfer_update_state(ERROR_SUCCESS);
}

// Update the tranfer state with new information
static void transfer_stream_open(stream_type_t stream, uint32_t start_sector)
{
    error_t status;
    util_assert(!file_transfer_state.stream_open);
    util_assert(start_sector != VFS_INVALID_SECTOR);
    vfs_user_printf("virtual_fs_user transfer_update_stream_open(stream=%i, start_sector=%i)\r\n",
                    stream, start_sector);

    // Initialize the starting sector if it has not been set
    if (VFS_INVALID_SECTOR == file_transfer_state.start_sector) {
        file_transfer_state.start_sector = start_sector;
        if (start_sector != VFS_INVALID_SECTOR) {
            vfs_user_printf("    start_sector=%i\r\n", start_sector);
        }
    }
    // Initialize the stream if it has not been set
    if (STREAM_TYPE_NONE == file_transfer_state.stream) {
        file_transfer_state.stream = stream;
        if (stream != STREAM_TYPE_NONE) {
            vfs_user_printf("    stream=%i\r\n", stream);
        }
    }

    // Check - Starting sector must be the same
    if (start_sector != file_transfer_state.start_sector) {
        vfs_user_printf("    error: starting sector changed from %i to %i\r\n", file_transfer_state.start_sector, start_sector);
        transfer_update_state(ERROR_ERROR_DURING_TRANSFER);
        return;
    }
    // Check - stream must be the same
    if (stream != file_transfer_state.stream) {
        vfs_user_printf("    error: changed types during tranfer from %i to %i\r\n", stream, file_transfer_state.stream);
        transfer_update_state(ERROR_ERROR_DURING_TRANSFER);
        return;
    }

    // Open stream
    status = stream_open(stream);
    vfs_user_printf("    stream_open stream=%i ret %i\r\n", stream, status);
    if (ERROR_SUCCESS == status) {
        file_transfer_state.file_next_sector = start_sector;
        file_transfer_state.stream_open = true;
        file_transfer_state.stream_started = true;
    }

    transfer_update_state(status);
}

// Update the tranfer state with new information
static void transfer_stream_data(uint32_t sector, const uint8_t * data, uint32_t size)
{
    error_t status;
    vfs_user_printf("virtual_fs_user transfer_stream_data(sector=%i, size=%i)\r\n", sector, size);
    vfs_user_printf("    file offset=0x%x, data=%x,%x,%x,%x,...\r\n",
        file_transfer_state.size_processed, data[0],data[1],data[2],data[3]);

    if (file_transfer_state.stream_finished) {
        // In this state steam processing has been finished but the
        // amount of data transferred still needs to be recorded
        vfs_user_printf("    stream closed so ignoring data\r\n", status);
        file_transfer_state.size_processed += size;
        file_transfer_state.file_next_sector = sector + size / VFS_SECTOR_SIZE;
        transfer_update_state(ERROR_SUCCESS);
        return;
    }

    util_assert(size % VFS_SECTOR_SIZE == 0);
    util_assert(file_transfer_state.stream_open);
    status = stream_write((uint8_t*)data, size);
    vfs_user_printf("    stream_write ret=%i\r\n", status);
    if (ERROR_SUCCESS_DONE == status) {
        status = stream_close();
        vfs_user_printf("    stream_close ret=%i\r\n", status);
        file_transfer_state.stream_open = false;
        file_transfer_state.stream_finished = true;
    } else if (ERROR_SUCCESS_DONE_OR_CONTINUE == status) {
        status = ERROR_SUCCESS;
        file_transfer_state.stream_optional_finish = true;
    } else {
        file_transfer_state.stream_optional_finish = false;
    }

    file_transfer_state.size_processed += size;
    file_transfer_state.file_next_sector = sector + size / VFS_SECTOR_SIZE;

    transfer_update_state(status);
}

// Check if the current transfer is still in progress, done, or if an error has occurred
static void transfer_update_state(error_t status)
{
    bool transfer_timeout;
    bool transfer_started;
    bool transfer_can_be_finished;
    bool transfer_must_be_finished;
    bool transfer_error;
    bool too_much_transfered;
    error_t local_status = status;

    if (file_transfer_state.transfer_finished) {
        util_assert(0);
        return;
    }

    // Update file info status
    file_transfer_state.file_info_optional_finish =
        (file_transfer_state.file_to_program != VFS_FILE_INVALID ) &&
        (file_transfer_state.size_processed >= file_transfer_state.file_size) &&
        (file_transfer_state.file_size > 0);

    transfer_error = local_status != ERROR_SUCCESS ? true : false;
    transfer_timeout = file_transfer_state.transfer_timeout;
    transfer_started = (VFS_FILE_INVALID != file_transfer_state.file_to_program) ||
                       (STREAM_TYPE_NONE != file_transfer_state.stream);
    too_much_transfered = file_transfer_state.size_processed > 
                          ROUND_UP(file_transfer_state.file_size, VFS_CLUSTER_SIZE);
    transfer_can_be_finished = file_transfer_state.file_info_optional_finish &&
                               file_transfer_state.stream_optional_finish;
    transfer_must_be_finished = file_transfer_state.stream_finished &&
                                file_transfer_state.file_info_optional_finish &&
                                !too_much_transfered;

    if (transfer_error) {
        // Local status already set
        file_transfer_state.transfer_finished = true;
    } if (transfer_timeout) {
        bool transfer_successful = !transfer_started || transfer_can_be_finished;
        local_status = transfer_successful ? ERROR_SUCCESS : ERROR_TRANSFER_TIMEOUT;
        file_transfer_state.transfer_finished = true;
    } else if (transfer_must_be_finished) {
        local_status = ERROR_SUCCESS;
        file_transfer_state.transfer_finished = true;
    }

    if (file_transfer_state.transfer_finished) {
        vfs_user_printf("virtual_fs_user transfer_update_state(status=%i)\r\n", status);
        vfs_user_printf("    file=%p, start_sect= %i, size=%i\r\n", 
            file_transfer_state.file_to_program, file_transfer_state.start_sector,
            file_transfer_state.file_size);
        vfs_user_printf("    stream=%i, size_processed=%i, opt_finish=%i, timeout=%i\r\n", 
            file_transfer_state.stream, file_transfer_state.size_processed,
            file_transfer_state.file_info_optional_finish, transfer_timeout);

        // Close the file stream if it is open
        if (file_transfer_state.stream_open) {
            error_t close_status;
            close_status = stream_close();
            vfs_user_printf("    stream closed ret=%i\r\n", close_status);
            file_transfer_state.stream_open = false;
            if (ERROR_SUCCESS == local_status) {
                local_status = close_status;
            }
        }

        if (too_much_transfered) {
            if (ERROR_SUCCESS == local_status) {
                local_status = ERROR_FILE_BOUNDS;
            }
        }

        // Set the fail reason 
        fail_reason = local_status;
        vfs_user_printf("    Transfer finished, status: %i=%s\r\n", fail_reason, error_get_string(fail_reason));
    }

    // If this state change is not from aborting a transfer
    // due to a remount then trigger a remount
    if (!transfer_timeout) {
        vfs_user_remount();
    }
}

// Remove strip_count characters from the start of buf and then insert
// new_str at the new start of buf.
static void insert(uint8_t *buf, uint8_t* new_str, uint32_t strip_count)
{
    uint32_t buf_len = strlen((const char *)buf);
    uint32_t str_len = strlen((const char *)new_str);
    // push out string
    memmove(buf + str_len, buf + strip_count, buf_len - strip_count);
    // insert
    memcpy(buf, new_str, str_len);
}

// Fill buf with the contents of the mbed redirect file by
// expanding the special characters in mbed_redirect_file.
static void update_html_file(uint8_t *buf, uint32_t bufsize)
{
    uint32_t size_left;
    uint8_t *orig_buf = buf;
    uint8_t *insert_string;

    // Zero out buffer so strlen operations don't go out of bounds
    memset(buf, 0, bufsize);
    memcpy(buf, mbed_redirect_file, strlen((const char *)mbed_redirect_file));
    do {
        // Look for key or the end of the string
        while ((*buf != '@') && (*buf != 0)) buf++;

        // If key was found then replace it
        if ('@' == *buf) {
            switch(*(buf+1)) {
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
                case 'D':   // HDK
                    insert_string = (uint8_t *)info_get_hdk_id();
                    break;

                case 'v':
                case 'V':   // Firmware version
                    insert_string = (uint8_t *)info_get_version();
                    break;

                case 'r':
                case 'R':   // URL replacement
                    insert_string = (uint8_t *)daplink_target_url;
                    break;

                default:
                    insert_string = (uint8_t *)"ERROR";
                    break;
            }
            insert(buf, insert_string, 2);
        }
    } while(*buf != '\0');
    size_left = buf - orig_buf;
    memset(buf, 0, bufsize - size_left);
}
