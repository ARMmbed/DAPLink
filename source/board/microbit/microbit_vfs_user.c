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
#include "stdlib.h"

#include "vfs_manager.h"
#include "macro.h"
#include "error.h"
#include "util.h"
#include "settings.h"
#include "target_reset.h"
#include "daplink.h"
#include "IO_config.h"      // for NVIC_SystemReset
#include "version_git.h"
#include "info.h"
#include "gpio.h"           // for gpio_get_sw_reset
#include "swd_host.h"
#include "daplink_debug.h"

#define MINV(a,b) (a<b?a:b)

/**
  * Micro:Bit specific VFS files:
  *  - MBITFS.HTM filesystem webpage interface.
  *  - FLASH.BIN binary representation of the filesystem.
  *  - FLASHJS.TXT javascript array of the filesystem
  *
  * Files are provided by three read file handles: read_microbit_flash_bin, 
  * read_microbit_flash_js, and read_file_microbit_htm.
  *
  * The location of the file system in flash is not known at compile time, being
  * set dynamically by the DAL. The location and size are stored in a key-value pair
  * structure, at a known page. Before anything else, these need to be established by calling 
  * microbit_get_flash_meta_data.
  *
  * board_vfs_add_files() is called in vfs_user.c provided BOARD_VFS_ADD_FILES is set,
  * and calls the relevant vfs_manager.h functions to add these files to the VFS.
  *
  */

// The location in flash of the NRF51822 key/value pair set.
#define KEY_VALUE_FLASH_LOC 0x3BC08

// The fixed size of each key/value pair entry.
#define KEY_VALUE_LEN       48

// The number of key value pairs.
#define KEY_VALUE_SIZE      21

// The key for the flash storage location
#define KEY_STRING "MBFS_START"

// Read functions for the MicroBit VFS.
static uint32_t read_microbit_flash_bin(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors);
static uint32_t read_microbit_flash_js(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors);
static uint32_t read_file_microbit_htm(uint32_t sector, uint8_t *data, uint32_t n);

// MBITFS.HTM contents (in microbit_html.c)
extern char microbit_html[];

// Initialization function, to obtain the flash start (microbit_flash_start) and size (microbit_flash_size).
static int microbit_get_flash_meta(uint32_t * flash_start, uint32_t * flash_size);

static uint32_t microbit_flash_start = 0;
static uint32_t microbit_flash_size = 0;

/**
  * called in vfs_user.c: vfs_build().
  *
  * Adds the files specific to the microbit.
  * Requires that BOARD_VFS_ADD_FILES
  */
void board_vfs_add_files() {
    if(microbit_get_flash_meta(&microbit_flash_start, &microbit_flash_size)) {
            
        microbit_flash_size *= 1024;
        vfs_create_file("FLASH   BIN", read_microbit_flash_bin, 0, microbit_flash_size);
        vfs_create_file("FLASHJS TXT", read_microbit_flash_js, 0, (microbit_flash_size*5)+12);
        //vfs_create_file("FLASHJS TXT", read_microbit_flash_js, 0, (microbit_flash_size * 5) + 12);

        int file_size = strlen(microbit_html);
        vfs_create_file("MBITFS  HTM", read_file_microbit_htm, 0, file_size);
    }            
}

/**
  * Function to initialize the microbit_flash_start and microbit_flash_size internal variables.
  * These must be set before any other functions can be called.
  *
  * These values are set by the MicroBit DAL in the keyvalue store.
  *
  * @param flash_start pointer to the start of flash location variable to set.
  * @param flash_size pointer to the size of flash variable to set.
  * @return non-zero on error.
  */
static int microbit_get_flash_meta(uint32_t * flash_start, uint32_t * flash_size) {

    swd_init();
    target_set_state(RESET_HOLD);

    static uint8_t buffer[48];

    for(int i=0;i<KEY_VALUE_SIZE;i++) {
        swd_read_memory(KEY_VALUE_FLASH_LOC, buffer, KEY_VALUE_LEN);

        if(strncmp((char*)((uint8_t*)buffer), KEY_STRING, strlen(KEY_STRING)) != 0) {
            continue;
        }
    
        memcpy(flash_start, ((uint8_t*)buffer)+16, sizeof(uint32_t));
        memcpy(flash_size, ((uint8_t*)buffer)+20, sizeof(uint32_t));

        swd_off();
        target_set_state(RESET_RUN);

        return 1;
    }

    swd_off();
    target_set_state(RESET_RUN);

    return 0;
}

/**
  * Callback read Function to read the microbit flash contents as a raw binary file.
  * 
  * @param sector_offset sector offset to read from.
  * @param data buffer to read into.
  * @param num_sectors number of sectors to read.
  * @return number of bytes read.
  */
static uint32_t read_microbit_flash_bin(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors) {
    
    swd_init();
    target_set_state(RESET_HOLD);
   
    uint32_t loc = microbit_flash_start + (VFS_SECTOR_SIZE * sector_offset);

    swd_read_memory(loc, data, (num_sectors * VFS_SECTOR_SIZE)); 
    
    swd_off();

    target_set_state(RESET_RUN);
    
    return (VFS_SECTOR_SIZE * num_sectors);
}

/**
  * Callback read function to read the microbit flash contents as a javascript array.
  * Each byte is represented as "0x??," - 5 bytes. 
  * The array is prepended with "var fl = [" - 10 bytes.
  * The array is appended with "];" - 2 bytes.
  *
  * The total size of the javascript file is 5n+12 (n = flash size).
  *
  * @param sector_offset sector offset to read from.
  * @param data buffer to read into
  * @param num_sectors number of sectors to read.
  * @return number of bytes read.
  */
static uint32_t read_microbit_flash_js(uint32_t sector, uint8_t* data, uint32_t num_sectors) {
  
    swd_init();
    target_set_state(RESET_HOLD);

    int max_file_size = (microbit_flash_size*5)+12; 

    int buffer_offset = 0; 

    for(int k=0; k < num_sectors && (sector * VFS_SECTOR_SIZE) < max_file_size; k++, sector++) {

        int wr = 0;

        int write = MINV( VFS_SECTOR_SIZE, 
                          (max_file_size - (sector * VFS_SECTOR_SIZE)) ) - 2;

        // Flash address to read from.
        int addr = microbit_flash_start + (sector * 102) +
                   (sector == 0 ? 0 : -2);

        if(sector == 0) {   
            
            // start of file, print header.
            sprintf((char*)data, "var fl = [");
            wr += 10;
            buffer_offset += 10;
        }

        for(; wr < write; wr += 5) {

            // Copy each byte from flash, print to *data

            uint8_t byte = 0;
            swd_read_memory(addr++, &byte, 1);

            sprintf((char*)&data[buffer_offset], "0x%02x,", byte);
            buffer_offset+=5;
        }

        if(sector != (max_file_size / VFS_SECTOR_SIZE)) {
            sprintf((char*)&data[buffer_offset], " \n");
            buffer_offset+=2;
        }
        else {

            // EOF, print array end.
            sprintf((char*)&data[buffer_offset-1], " ];");
            buffer_offset+=2;
        }
    }

    target_set_state(RESET_RUN);
    swd_off();

    // buffer_offset is the no. bytes written.
    return buffer_offset;
}

/**
  * Callback read function to read the MBITFS.HTM file
  *
  * Reads from microbit_html array.
  * @param sector offset sector to read from
  * @param data buffer to read into
  * @param n number of sectors to read
  * @return number of bytes read.
  */
static uint32_t read_file_microbit_htm(uint32_t sector, uint8_t *data, uint32_t n)
{
    uint32_t offset = sector * VFS_SECTOR_SIZE;
    int len = MIN( (n*VFS_SECTOR_SIZE) , (strlen(microbit_html) - offset) );
    if(len < 0) return 0;

    memcpy(data, microbit_html+offset, len);
    return len;
}
