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
#include "version.h"
#include "config_settings.h"
#include "daplink.h"

typedef struct file_transfer_state {
    uint32_t start_block;
    uint32_t amt_to_write;
    uint32_t amt_written;
    uint32_t next_block_to_write;
    uint32_t transfer_started;
    uint32_t transfer_finished;
    extension_t file_type;
} file_transfer_state_t;

static uint32_t usb_buffer[512/sizeof(uint32_t)];
static file_transfer_state_t file_transfer_state;

void reset_file_transfer_state(void)
{
    static const file_transfer_state_t default_transfer_state = {0,0,0,0,0,0,UNKNOWN};
    file_transfer_state = default_transfer_state;
}

void usbd_msc_init(void)
{    
    // Setup the filesystem based on target parameters
    virtual_fs_init();

    USBD_MSC_MemorySize = mbr.bytes_per_sector * mbr.total_logical_sectors;
    USBD_MSC_BlockSize  = mbr.bytes_per_sector;
    USBD_MSC_BlockGroup = 1;
    USBD_MSC_BlockCount = USBD_MSC_MemorySize / USBD_MSC_BlockSize;
    USBD_MSC_BlockBuf   = (uint8_t *)usb_buffer;

    USBD_MSC_MediaReady = __TRUE;
}

void usbd_msc_read_sect(uint32_t block, uint8_t *buf, uint32_t num_of_blocks)
{
    virtual_media_t fs_read = {0,0};
    uint32_t max_known_fs_entry_addr = 0;
    uint32_t req_sector_offset = 0;
    uint32_t req_addr = block * USBD_MSC_BlockSize;
    uint8_t i = 0, real_data_present = 1;
    
    // dont proceed if we're not ready
    if (!USBD_MSC_MediaReady) {
        return;
    }
    
    // indicate msc activity
    main_blink_msc_led(MAIN_LED_OFF);
    
    // A block is requested from the host. We dont have a flat file system image on disc
    //  rather just the required bits (mbr, fat, root dir, file data). The fs structure 
    //  knows how these parts look without requiring them all to exist linearly in memory
    while((fs[i].length != 0) && (fs_read.sect == 0)) {
        // accumulate the length of the fs.sect(s) we have examined so far
        max_known_fs_entry_addr += fs[i].length;
        // determine if we have real system data or need to pad the transfer with 0
        if (req_addr < max_known_fs_entry_addr) {
            // we know this is where the data request is, store it for later transmission
            fs_read.sect = fs[i].sect;
            // sector can be larger than a block. Normalize the block number into the fs entry
            req_sector_offset = fs[i].length - (max_known_fs_entry_addr - req_addr);
            // determine if the inflated size is greater than the real size.
            if(req_sector_offset >= sizeof(fs[i].sect)) {
                real_data_present = 0;
            }
        }
        i++;
    }
    // now send the data if a known sector and valid data in memory - otherwise send 0's
    if (fs_read.sect != 0 && real_data_present == 1) {
        memcpy(buf, &fs_read.sect[req_sector_offset], num_of_blocks * USBD_MSC_BlockSize);
    }
    else {
        memset(buf, 0, num_of_blocks * USBD_MSC_BlockSize);
    }
    
    // Some files require runtime content. If one generate and overwrite the the read sequence
    //  buffer with the newer data (only works for files < 512 bytes and known location on fs)
    if (block == (mbr.reserved_logical_sectors + (mbr.logical_sectors_per_fat*mbr.num_fats) 
        + ((mbr.max_root_dir_entries*sizeof(FatDirectoryEntry_t)/mbr.bytes_per_sector)))) {
        update_html_file(buf, (num_of_blocks * USBD_MSC_BlockSize));
    }
}

static uint32_t filename_valid(uint8_t c)
{
    const char *valid_char = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    do {
        if(c == *valid_char) {
            return 1;
        }
    } while ((uint8_t)(*(valid_char++)) != '\0');
    return 0;
}

// known executable extension types
// CRD - chrome
// PAR - IE
// Extensions dont matter much if you're looking for specific file data
//  other than size parsing but hex and srec have specific EOF records
static const char *const known_exec_extensions[] = {
    "BIN",
    "bin",
    "HEX",
    "hex",
    0,
};

static uint32_t exec_file_entry(const FatDirectoryEntry_t dir_entry)
{
    uint32_t i = 0;
    // we may see the following. Validate all or keep looking
    //  entry with invalid or reserved first byte
    //  entry with a false filesize.
    while (known_exec_extensions[i] != 0) {
        if(1 == filename_valid(dir_entry.filename[0])) {
            if (0 == strncmp(known_exec_extensions[i], (const char *)&dir_entry.filename[8], 3)) {
                return (dir_entry.filesize) ? 1 : 0;
            }
        }
        i++;
    }
    return 0;
}

static bool data_same_ignore_case(const char * str1, const char * str2, uint32_t size)
{
    uint32_t i;
    for (i = 0; i < size; i++) {
        if (tolower(str1[i]) != tolower(str2[i])) {
            return false;
        }
    }
    return true;
}

static bool check_file_deleted(const FatDirectoryEntry_t * dir_entry, const char * filename)
{
    // Look for the file deletion 
    //  the correct name
    //  and a valid file size
    if(0xe5 == dir_entry->filename[0]) {
        const uint32_t size = sizeof(dir_entry->filename) - 1;
        if (data_same_ignore_case(&filename[1], (const char *)&dir_entry->filename[1], size)) {
            return true;
        }
    }
    return false;
}

static bool check_file_created(const FatDirectoryEntry_t * dir_entry, const char * filename)
{
    //  the correct name
    const uint32_t size = sizeof(dir_entry->filename);
    if (data_same_ignore_case(&filename[0], (const char *)&dir_entry->filename[0], size)) {
        return true;
    }
    return false;
}

static extension_t identify_start_sequence(uint8_t *buf)
{
    if (1 == validate_bin_nvic(buf)) {
        return BIN;
    }
    else if (1 == validate_hexfile(buf)) {
        return HEX;
    }
    return UNKNOWN;
}

// Start the usb disconnect timer.  If no more data arrives
// then fail with the given status.
static void start_disconnect_reconnect_timer(target_flash_status_t status)
{
    configure_fail_txt(status);
    main_msc_disconnect_event();
}

// Complete the transfer with the given status.  After this
// call no more data received over USB will get processed
// until after reconnecting. This should only be called
// from usbd_msc_write_sect and after calling it there
// should be a return to prevent further processing.
// Note - this function does not detach USB immediately.
//        It waits until usb is idle before disconnecting.
static void tranfer_complete(target_flash_status_t status)
{
    file_transfer_state.transfer_finished = 1;
    start_disconnect_reconnect_timer(status);
}

void usbd_msc_write_sect(uint32_t block, uint8_t *buf, uint32_t num_of_blocks)
{
    FatDirectoryEntry_t tmp_file = {0};
    extension_t start_type_identified = UNKNOWN;
    target_flash_status_t status = TARGET_OK;
    uint32_t i = 0;
    
    if (!USBD_MSC_MediaReady) {
        return;
    }

    // Restart the disconnect counter on every packet
    // so the device does not detach in the middle of a
    // transfer.
    main_msc_delay_disconnect_event();

    if (file_transfer_state.transfer_finished) {
        debug_msg("%d: %s\r\n", __LINE__, "Transfer_failed - exit");
        return;
    }

    debug_msg("block: %d\r\n", block);
    // indicate msc activity
    main_blink_msc_led(MAIN_LED_OFF);

    // if the root dir comes we should look at it and parse for info that can end a transfer
    if ((block == ((mbr.num_fats * mbr.logical_sectors_per_fat) + 1)) || 
             (block == ((mbr.num_fats * mbr.logical_sectors_per_fat) + 2))) {
        // start looking for a known file and some info about it
        for( ; i < USBD_MSC_BlockSize/sizeof(tmp_file); i++) {
            memcpy(&tmp_file, &buf[i*sizeof(tmp_file)], sizeof(tmp_file));
            debug_msg("name:%*s\t attributes:%8d\t size:%8d\r\n", 11, tmp_file.filename, tmp_file.attributes, tmp_file.filesize);
            // test for a known dir entry file type and also that the filesize is greater than 0
            if (1 == exec_file_entry(tmp_file)) {
                file_transfer_state.amt_to_write = tmp_file.filesize;
                start_disconnect_reconnect_timer(TARGET_FAIL_TRANSFER_IN_PROGRESS);
            } else if (check_file_deleted(&tmp_file, virtual_fs_auto_rstcfg)) {
                debug_msg("%s", "USER CFG ACTION DETECTED\r\n");
                config_set_auto_rst(false);
                tranfer_complete(TARGET_OK);
                return;
            } else if (check_file_deleted(&tmp_file, virtual_fs_hard_rstcfg)) {
                debug_msg("%s", "USER CFG ACTION DETECTED\r\n");
                config_set_auto_rst(true);
                tranfer_complete(TARGET_OK);
                return;
            } else if (check_file_created(&tmp_file, daplink_mode_file_name)) {
                debug_msg("%s", "USER CFG ACTION DETECTED\r\n");
                if (daplink_is_interface()) {
                    config_ram_set_hold_in_bl(true);
                } else {
                    // Do nothing - bootloader will go to interface by default
                }
                tranfer_complete(TARGET_OK);
                return;
            }
        }
    }

    // this is the key for starting a file write - we dont care what file types are sent
    //  just look for something unique (NVIC table, hex, srec, etc) until root dir is updated
    if (0 == file_transfer_state.transfer_started) {
        // look for file types we can program
        start_type_identified = identify_start_sequence(buf);
        if (start_type_identified != UNKNOWN) {
            debug_msg("%s", "FLASH INIT\r\n");
            
            // binary file transfer - reset parsing
            file_transfer_state.start_block = block;
            if (0 == file_transfer_state.amt_to_write) {
                file_transfer_state.amt_to_write = 0xffffffff;
            }
            file_transfer_state.amt_written = USBD_MSC_BlockSize;
            file_transfer_state.next_block_to_write = block;
            file_transfer_state.transfer_started = 1;
            file_transfer_state.file_type = start_type_identified;
            start_disconnect_reconnect_timer(TARGET_FAIL_TRANSFER_IN_PROGRESS);
            
            // prepare the target device
            status = target_flash_init(file_transfer_state.file_type);
            if (status != TARGET_OK) {
                tranfer_complete(status);
                return;
            }
        }
    }
    
    // Transfer must have been started for processing beyond this point
    if (!file_transfer_state.transfer_started) {
        return;
    }

    // write data to media
    if (block >= file_transfer_state.start_block) {
        // check for contiguous transfer
        if (block == file_transfer_state.next_block_to_write) {
            debug_msg("%d: %s", __LINE__, "FLASH WRITE - ");
            status = target_flash_program_page((block-file_transfer_state.start_block)*USBD_MSC_BlockSize, buf, USBD_MSC_BlockSize*num_of_blocks);
            debug_msg("%d\r\n", status);
            if ((status != TARGET_OK) && (status != TARGET_HEX_FILE_EOF)) {
                tranfer_complete(status);
                return;
            }
            // and do the housekeeping
            file_transfer_state.amt_written += USBD_MSC_BlockSize;
            file_transfer_state.next_block_to_write = block + 1;
        } else {
            // this is non-contigous transfer. need to wait for then next proper block
            debug_msg("%s", "BLOCK OUT OF ORDER\r\n");
        }
    }

    // See if a complete transfer occured.
    if (file_transfer_state.amt_written >= file_transfer_state.amt_to_write) {
        // hex files since they have a record which indicates the end.
        if (TARGET_HEX_FILE_EOF == status) {
            debug_msg("%s", "FLASH END\r\n");
            tranfer_complete(TARGET_OK);
            return;
        }
        // set the status of binary file programming
        start_disconnect_reconnect_timer(TARGET_OK);
    } else {
        // If no more data comes in fail with the message
        // because an in progress tranfer timed out
        start_disconnect_reconnect_timer(TARGET_FAIL_TRANSFER_IN_PROGRESS);
    }
}
