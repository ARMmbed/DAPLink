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

#include "RTL.h"
#include "rl_usb.h"
#include "string.h"
#include "main.h"
#include "usb_buf.h"
#include "virtual_fs.h"
#include "daplink_debug.h"
#include "version.h"
#include "target_reset.h"

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
    main_blink_msd_led(0);
    
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
    
    // Some files require funtime content. If one generate and overwrite the the read sequence
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
    } while (*(valid_char++) != '\0');
    return 0;
}        

static uint32_t wanted_dir_entry(const FatDirectoryEntry_t dir_entry)
{
    uint32_t i = 0;
    // we may see the following. Validate all or keep looking
    //  entry with invalid or reserved first byte
    //  entry with a false filesize.
    while (known_extensions[i] != 0) {
        if(1 == filename_valid(dir_entry.filename[0])) {
            if (0 == strncmp(known_extensions[i], (const char *)&dir_entry.filename[8], 3)) {
                return (dir_entry.filesize) ? 1 : 0;
            }
        }
        i++;
    }
    return 0;
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

void usbd_msc_write_sect(uint32_t block, uint8_t *buf, uint32_t num_of_blocks)
{
    FatDirectoryEntry_t tmp_file = {0};
    extension_t start_type_identified = UNKNOWN;
    target_flash_status_t status = TARGET_OK;
    uint32_t i = 0;
    static struct {
        uint32_t start_block;
        uint32_t amt_to_write;
        uint32_t amt_written;
        uint32_t last_block_written;
        uint32_t transfer_started;
        extension_t file_type;
    } file_transfer_state = {0,0,0,0,0,UNKNOWN};
    
    if (!USBD_MSC_MediaReady) {
        return;
    }
    
    debug_msg("block: %d\r\n", block);
    // indicate msd activity
    main_blink_msd_led(0);
      
    // this is the key for starting a file write - we dont care what file types are sent
    //  just look for something unique (NVIC table, hex, srec, etc) until root dir is updated
    if (0 == file_transfer_state.transfer_started) {
        // look for file types we can program
        start_type_identified = identify_start_sequence(buf);
        if (start_type_identified != UNKNOWN) {
            debug_msg("%s", "FLASH INIT\r\n");
            
            // binary file transfer - reset parsing
            file_transfer_state.start_block = block;
            file_transfer_state.amt_to_write = 0xffffffff;
            file_transfer_state.amt_written = USBD_MSC_BlockSize;
            file_transfer_state.last_block_written = block;
            file_transfer_state.transfer_started = 1;
            file_transfer_state.file_type = start_type_identified;
            
            // prepare the target device
            status = target_flash_init(file_transfer_state.file_type);
            if (status != TARGET_OK) {
                file_transfer_state.transfer_started = 0;
                configure_fail_txt(status);
                main_usb_disconnect_event();
                return;
            }
            // writing in 2 places less than ideal but manageable
            debug_msg("%s", "FLASH WRITE\r\n");
            //debug_data(buf, USBD_MSC_BlockSize);
            status = target_flash_program_page((block-file_transfer_state.start_block)*USBD_MSC_BlockSize, buf, USBD_MSC_BlockSize*num_of_blocks);
            if (status != TARGET_OK) {
                file_transfer_state.transfer_started = 0;
                configure_fail_txt(status);
                main_usb_disconnect_event();
                return;
            }
            return;
        }
    }
    // if the root dir comes we should look at it and parse for info that can end a transfer
    else if ((block == ((mbr.num_fats * mbr.logical_sectors_per_fat) + 1)) || 
             (block == ((mbr.num_fats * mbr.logical_sectors_per_fat) + 2))) {
        // start looking for a known file and some info about it
        for( ; i < USBD_MSC_BlockSize/sizeof(tmp_file); i++) {
            memcpy(&tmp_file, &buf[i*sizeof(tmp_file)], sizeof(tmp_file));
            debug_msg("na:%.11s\tatrb:%8d\tsz:%8d\tst:%8d\tcr:%8d\tmod:%8d\taccd:%8d\r\n"
                , tmp_file.filename, tmp_file.attributes, tmp_file.filesize, tmp_file.first_cluster_low_16
                , tmp_file.creation_time_ms, tmp_file.modification_time, tmp_file.accessed_date);
            // test for a known dir entry file type and also that the filesize is greater than 0
            if (1 == wanted_dir_entry(tmp_file)) {
                file_transfer_state.amt_to_write = tmp_file.filesize;
            }
        }
    }

    // write data to media
    if ((block >= file_transfer_state.start_block) && 
        (file_transfer_state.transfer_started == 1)) {
        if (block >= file_transfer_state.start_block) {
            // check for contiguous transfer
            if (block != (file_transfer_state.last_block_written+1)) {
                // this is non-contigous transfer. need to wait for then next proper block
                debug_msg("%s", "BLOCK OUT OF ORDER\r\n");
            }
            else {
                debug_msg("%s", "FLASH WRITE\r\n");
                //debug_data(buf, USBD_MSC_BlockSize);
                status = target_flash_program_page((block-file_transfer_state.start_block)*USBD_MSC_BlockSize, buf, USBD_MSC_BlockSize*num_of_blocks);
                if (status != TARGET_OK) {
                    file_transfer_state.transfer_started = 0;
                    configure_fail_txt(status);
                    main_usb_disconnect_event();
#ifdef AUTO_RUN
                    if (TARGET_OK == status) {
                        target_set_state(RESET_RUN);
                    }
#endif
                    return;
                }
                // and do the housekeeping
                file_transfer_state.amt_written += USBD_MSC_BlockSize;
                file_transfer_state.last_block_written = block;
            }
        }
    }
    
    // see if a complete transfer occured by knowing it started and comparing filesize expectations
    if ((file_transfer_state.amt_written >= file_transfer_state.amt_to_write) && (file_transfer_state.transfer_started == 1 )){
        // do the disconnect - maybe write some programming stats to the file
        debug_msg("%s", "FLASH END\r\n");
        // we know the contents have been reveived. Time to eject
        file_transfer_state.transfer_started = 0;
        configure_fail_txt(status);
        main_usb_disconnect_event();
        return;
    }
    
    // There is one more known state where the root dir is updated with the amount of data transfered but not the whole file transfer was complete
    //  To handle this we need a state to kick off a timer for a fixed amount of time where we can receive more continous sectors and assume
    //  they are valid file data. This is only the case for bin files since the only known end is the filesize from the root dir entry.
}
