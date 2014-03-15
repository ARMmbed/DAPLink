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
 
#ifndef VIRTUAL_FS_H
#define VIRTUAL_FS_H

#include <stdint.h>
#include "device_cfg.h"

// These error symbols messages need to match the text in reason_array and the order too
#define SWD_ERROR               0
#define BAD_EXTENSION_FILE      1
#define NOT_CONSECUTIVE_SECTORS 2
#define SWD_PORT_IN_USE         3
#define RESERVED_BITS           4
#define BAD_START_SECTOR        5
#define TIMEOUT                 6

// These are defines to build the filesystem
#define NUM_FATS                             2
#define SECTORS_ROOT_IDX                    (1 + NUM_FATS*MBR_SECTORS_PER_FAT)
#define SECTORS_FIRST_FILE_IDX              (SECTORS_ROOT_IDX + 2)
#define SECTORS_SYSTEM_VOLUME_INFORMATION   (SECTORS_FIRST_FILE_IDX  + WANTED_SECTORS_PER_CLUSTER)
#define SECTORS_INDEXER_VOLUME_GUID         (SECTORS_SYSTEM_VOLUME_INFORMATION + WANTED_SECTORS_PER_CLUSTER)
#define SECTORS_MBED_HTML_IDX               (SECTORS_INDEXER_VOLUME_GUID + WANTED_SECTORS_PER_CLUSTER)
#define SECTORS_ERROR_FILE_IDX              (SECTORS_MBED_HTML_IDX + WANTED_SECTORS_PER_CLUSTER)

// CONSTANTS
#define WANTED_SIZE_IN_BYTES        ((FLASH_SIZE_KB + 16 + 8)*1024)
#define WANTED_SECTORS_PER_CLUSTER  (8)
#define FLASH_PROGRAM_PAGE_SIZE     (512)
#define MBR_BYTES_PER_SECTOR        (512)

// DERIVED
#define MBR_NUM_NEEDED_SECTORS  (WANTED_SIZE_IN_BYTES / MBR_BYTES_PER_SECTOR)
#define MBR_NUM_NEEDED_CLUSTERS (MBR_NUM_NEEDED_SECTORS / WANTED_SECTORS_PER_CLUSTER)

/* Need 3 sectors/FAT for every 1024 clusters */
#define MBR_SECTORS_PER_FAT     (3*((MBR_NUM_NEEDED_CLUSTERS + 1023)/1024))

/* Macro to help fill the two FAT tables with the empty sectors without
   adding a lot of test #ifs inside the sectors[] declaration below */
#if (MBR_SECTORS_PER_FAT == 1)
#   define EMPTY_FAT_SECTORS
#elif (MBR_SECTORS_PER_FAT == 2)
#   define EMPTY_FAT_SECTORS  {fat2,0},
#elif (MBR_SECTORS_PER_FAT == 3)
#   define EMPTY_FAT_SECTORS  {fat2,0},{fat2,0},
#elif (MBR_SECTORS_PER_FAT == 6)
#   define EMPTY_FAT_SECTORS  {fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},
#elif (MBR_SECTORS_PER_FAT == 9)
#   define EMPTY_FAT_SECTORS  {fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},
#elif (MBR_SECTORS_PER_FAT == 12)
#   define EMPTY_FAT_SECTORS  {fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},
#else
#   error "Unsupported number of sectors per FAT table"
#endif

#define DIRENTS_PER_SECTOR  (MBR_BYTES_PER_SECTOR / sizeof(FatDirectoryEntry_t))

__packed typedef struct {
    uint8_t boot_sector[11];

    /* DOS 2.0 BPB - Bios Parameter Block, 13 bytes */
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_logical_sectors;
    uint8_t  num_fats;
    uint16_t max_root_dir_entries;
    uint16_t total_logical_sectors;            /* If num is too large for 16 bits, set to 0 and use big_sectors_on_drive */
    uint8_t  media_descriptor;
    uint16_t logical_sectors_per_fat;          /* Need 3 sectors/FAT for every 1024 clusters */

    /* DOS 3.31 BPB - Bios Parameter Block, 12 bytes */
    uint16_t physical_sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t big_sectors_on_drive;             /* Use if total_logical_sectors=0, needed for large number of sectors */

    /* Extended BIOS Parameter Block, 26 bytes */
    uint8_t  physical_drive_number;
    uint8_t  not_used;
    uint8_t  boot_record_signature;
    uint32_t volume_id;
    char     volume_label[11];
    char     file_system_type[8];

    /* bootstrap data in bytes 62-509 */
    uint8_t  bootstrap[448];

    /* Mandatory value at bytes 510-511, must be 0xaa55 */
    uint16_t signature;
} mbr_t;

__packed typedef struct FatDirectoryEntry {
    uint8_t filename[11];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t creation_time_ms;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t accessed_date;
    uint16_t first_cluster_high_16;
    uint16_t modification_time;
    uint16_t modification_date;
    uint16_t first_cluster_low_16;
    uint32_t filesize;
} FatDirectoryEntry_t;

typedef enum {
    BIN_FILE,
    PAR_FILE,
    DOW_FILE,
    CRD_FILE,
    SPI_FILE,
    UNSUP_FILE, /* Valid extension, but not supported */
    SKIP_FILE,  /* Unknown extension, typically Long File Name entries */
} FILE_TYPE;

typedef struct {
    FILE_TYPE type;
    char extension[3];
    uint32_t flash_offset;
} FILE_TYPE_MAPPING;

typedef struct sector {
    const uint8_t * sect;
    unsigned int length;
} SECTOR;

extern uint32_t usb_buffer[];
extern SECTOR sectors[];
extern const uint8_t * reason_array[];
extern const uint8_t sect6[];
extern const uint8_t sect7[];
extern const uint8_t fail[];

#endif
