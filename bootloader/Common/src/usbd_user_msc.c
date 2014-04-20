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
#include <RTL.h>
#include <rl_usb.h>
#include <string.h>
#include "flash_hal.h"
#include "main.h"
#include "tasks.h"
#include "version.h"

#if defined(TARGET_LPC11U35)
#include <LPC11Uxx.h>
#elif defined(TARGET_MK20DX)
#include <MK20D5.h>
#endif

#if defined(TARGET_LPC11U35)	
#   define WANTED_SIZE_IN_KB  		(64)
#elif defined(TARGET_MK20DX)
#   define WANTED_SIZE_IN_KB        (128)
#endif

//------------------------------------------------------------------- CONSTANTS
#define WANTED_SIZE_IN_BYTES        ((WANTED_SIZE_IN_KB + 16 + 8)*1024)
#define WANTED_SECTORS_PER_CLUSTER  (8)

#define FLASH_PROGRAM_PAGE_SIZE         (512)
#define MBR_BYTES_PER_SECTOR            (512)

//--------------------------------------------------------------------- DERIVED

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

#define SECTORS_ROOT_IDX        (1 + mbr.num_fats*MBR_SECTORS_PER_FAT)
#define SECTORS_FIRST_FILE_IDX  (SECTORS_ROOT_IDX + 2)
#define SECTORS_SYSTEM_VOLUME_INFORMATION (SECTORS_FIRST_FILE_IDX  + WANTED_SECTORS_PER_CLUSTER)
#define SECTORS_INDEXER_VOLUME_GUID       (SECTORS_SYSTEM_VOLUME_INFORMATION + WANTED_SECTORS_PER_CLUSTER)
#define SECTORS_MBED_HTML_IDX   (SECTORS_INDEXER_VOLUME_GUID + WANTED_SECTORS_PER_CLUSTER)
#define SECTORS_ERROR_FILE_IDX  (SECTORS_MBED_HTML_IDX + WANTED_SECTORS_PER_CLUSTER)

//---------------------------------------------------------------- VERIFICATION

/* Sanity check */
#if (MBR_NUM_NEEDED_CLUSTERS > 4084)
  /* Limited by 12 bit cluster addresses, i.e. 2^12 but only 0x002..0xff5 can be used */
#   error Too many needed clusters, increase WANTED_SECTORS_PER_CLUSTER
#endif

#if ((WANTED_SECTORS_PER_CLUSTER * MBR_BYTES_PER_SECTOR) > 32768)
#   error Cluster size too large, must be <= 32KB
#endif

//-------------------------------------------------------------------- TYPEDEFS

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

//------------------------------------------------------------------------- END

uint32_t BlockBuf[1024/4];

typedef struct sector {
    const uint8_t * sect;
    unsigned int length;
} SECTOR;

#define MEDIA_DESCRIPTOR        (0xF0)

static const mbr_t mbr = {
    .boot_sector = {
        0xEB,0x3C, // x86 Jump
        0x90,      // NOP
        'M','S','W','I','N','4','.','1' // OEM Name in text
    },

    .bytes_per_sector         = MBR_BYTES_PER_SECTOR,
    .sectors_per_cluster      = WANTED_SECTORS_PER_CLUSTER,
    .reserved_logical_sectors = 1,
    .num_fats                 = 2,
    .max_root_dir_entries     = 32,
    .total_logical_sectors    = ((MBR_NUM_NEEDED_SECTORS > 32768) ? 0 : MBR_NUM_NEEDED_SECTORS),
    .media_descriptor         = MEDIA_DESCRIPTOR,
    .logical_sectors_per_fat  = MBR_SECTORS_PER_FAT, /* Need 3 sectors/FAT for every 1024 clusters */

    .physical_sectors_per_track = 1,
    .heads = 1,
    .hidden_sectors = 0,
    .big_sectors_on_drive = ((MBR_NUM_NEEDED_SECTORS > 32768) ? MBR_NUM_NEEDED_SECTORS : 0),

    .physical_drive_number = 0,
    .not_used = 0,
    .boot_record_signature = 0x29,
    .volume_id = 0x27021974,
    .volume_label = {'M','b','e','d',' ','U','S','B',' ',' ',' '},
    .file_system_type = {'F','A','T','1','2',' ',' ',' '},

    /* Executable boot code that starts the operating system */
    .bootstrap = {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    },
    .signature = 0xAA55,
};


static const uint8_t fat1[] = {
    MEDIA_DESCRIPTOR, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
};

static const uint8_t fat2[] = {0};

static const uint8_t fail[] = {
    'F','A','I','L',' ',' ',' ',' ',                   // Filename
    'T','X','T',                                       // Filename extension
    0x20,                                              // File attributes
    0x18,0xB1,0x74,0x76,0x8E,0x41,0x8E,0x41,0x00,0x00, // Reserved
    0x8E,0x76,                                         // Time created or last updated
    0x8E,0x41,                                         // Date created or last updated
    0x06,0x00,                                         // Starting cluster number for file
    0x07,0x00,0x00,0x0                                 // File size in bytes
};

// first 16 of the max 32 (mbr.max_root_dir_entries) root dir entries
static const uint8_t root_dir1[] = {
    // volume label "BOOTLOADER"
    'B', 'O', 'O', 'T', 'L', 'O', 'A', 'D', 'E', 'R', 0x20, 0x28, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x85, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    // Hidden files to keep mac happy

    // .fseventsd (LFN + normal entry)  (folder, size 0, cluster 2)
    0x41, 0x2E, 0x0, 0x66, 0x0, 0x73, 0x0, 0x65, 0x0, 0x76, 0x0, 0xF, 0x0, 0xDA, 0x65, 0x0,
    0x6E, 0x0, 0x74, 0x0, 0x73, 0x0, 0x64, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF,

    0x46, 0x53, 0x45, 0x56, 0x45, 0x4E, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x12, 0x0, 0x47, 0x7D, 0x75,
    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x7D, 0x75, 0x8E, 0x41, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0,

    // .metadata_never_index (LFN + LFN + normal entry)  (size 0, cluster 0)
    0x42, 0x65, 0x0, 0x72, 0x0, 0x5F, 0x0, 0x69, 0x0, 0x6E, 0x0, 0xF, 0x0, 0xA8, 0x64, 0x0,
    0x65, 0x0, 0x78, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF,

    0x1, 0x2E, 0x0, 0x6D, 0x0, 0x65, 0x0, 0x74, 0x0, 0x61, 0x0, 0xF, 0x0, 0xA8, 0x64, 0x0,
    0x61, 0x0, 0x74, 0x0, 0x61, 0x0, 0x5F, 0x0, 0x6E, 0x0, 0x0, 0x0, 0x65, 0x0, 0x76, 0x0,

    0x4D, 0x45, 0x54, 0x41, 0x44, 0x41, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x22, 0x0, 0x32, 0x85, 0x75,
    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x85, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    // .Trashes (LFN + normal entry)  (size 0, cluster 0)
    0x41, 0x2E, 0x0, 0x54, 0x0, 0x72, 0x0, 0x61, 0x0, 0x73, 0x0, 0xF, 0x0, 0x25, 0x68, 0x0,
    0x65, 0x0, 0x73, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF,

    0x54, 0x52, 0x41, 0x53, 0x48, 0x45, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x22, 0x0, 0x32, 0x85, 0x75,
    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x85, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    // Hidden files to keep windows 8.1 happy
    0x42, 0x20, 0x00, 0x49, 0x00, 0x6E, 0x00, 0x66, 0x00, 0x6F, 0x00, 0x0F, 0x00, 0x72, 0x72, 0x00, 
    0x6D, 0x00, 0x61, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x00, 0x00,
    
    0x01, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x0F, 0x00, 0x72, 0x6D, 0x00, 
    0x20, 0x00, 0x56, 0x00, 0x6F, 0x00, 0x6C, 0x00, 0x75, 0x00, 0x00, 0x00, 0x6D, 0x00, 0x65, 0x00,
    
    0x53, 0x59, 0x53, 0x54, 0x45, 0x4D, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x16, 0x00, 0xA5, 0x85, 0x8A, 
    0x73, 0x43, 0x73, 0x43, 0x00, 0x00, 0x86, 0x8A, 0x73, 0x43, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    // mbed html file (size 512, cluster 3)
    'B', 'O', 'O', 'T', 'L', 'O', 'A', 'D', 'H', 'T', 'M', 0x20, 0x18, 0xB1, 0x74, 0x76,
    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x8E, 0x76, 0x8E, 0x41, 0x05, 0x0, 0x00, 0x02, 0x0, 0x0,
};

// last 16 of the max 32 (mbr.max_root_dir_entries) root dir entries
static const uint8_t root_dir2[] = {0};

static const uint8_t sect5[] = {
    // .   (folder, size 0, cluster 2)
    0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x32, 0x0, 0x47, 0x7D, 0x75,
    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x88, 0x75, 0x8E, 0x41, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0,

    // ..   (folder, size 0, cluster 0)
    0x2E, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x0, 0x47, 0x7D, 0x75,
    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x7D, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    // NO_LOG  (size 0, cluster 0)
    0x4E, 0x4F, 0x5F, 0x4C, 0x4F, 0x47, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x8, 0x32, 0x85, 0x75,
    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x85, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

static const uint8_t sect6[] = {
    0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x00, 0xA5, 0x85, 0x8A, 
    0x73, 0x43, 0x73, 0x43, 0x00, 0x00, 0x86, 0x8A, 0x73, 0x43, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
    
    0x2E, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x00, 0xA5, 0x85, 0x8A, 
    0x73, 0x43, 0x73, 0x43, 0x00, 0x00, 0x86, 0x8A, 0x73, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    // IndexerVolumeGuid (size0, cluster 0)
    0x42, 0x47, 0x00, 0x75, 0x00, 0x69, 0x00, 0x64, 0x00, 0x00, 0x00, 0x0F, 0x00, 0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
    
    0x01, 0x49, 0x00, 0x6E, 0x00, 0x64, 0x00, 0x65, 0x00, 0x78, 0x00, 0x0F, 0x00, 0xFF, 0x65, 0x00, 
    0x72, 0x00, 0x56, 0x00, 0x6F, 0x00, 0x6C, 0x00, 0x75, 0x00, 0x00, 0x00, 0x6D, 0x00, 0x65, 0x00, 
    
    0x49, 0x4E, 0x44, 0x45, 0x58, 0x45, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x20, 0x00, 0xA7, 0x85, 0x8A, 
    0x73, 0x43, 0x73, 0x43, 0x00, 0x00, 0x86, 0x8A, 0x73, 0x43, 0x04, 0x00, 0x4C, 0x00, 0x00, 0x00
};

static const uint8_t sect7[] = {
	0x7B, 0x00, 0x39, 0x00, 0x36, 0x00, 0x36, 0x00, 0x31, 0x00, 0x39, 0x00, 0x38, 0x00, 0x32, 0x00, 
	0x30, 0x00, 0x2D, 0x00, 0x37, 0x00, 0x37, 0x00, 0x44, 0x00, 0x31, 0x00, 0x2D, 0x00, 0x34, 0x00, 
	0x46, 0x00, 0x38, 0x00, 0x38, 0x00, 0x2D, 0x00, 0x38, 0x00, 0x46, 0x00, 0x35, 0x00, 0x33, 0x00, 
	0x2D, 0x00, 0x36, 0x00, 0x32, 0x00, 0x44, 0x00, 0x39, 0x00, 0x37, 0x00, 0x46, 0x00, 0x35, 0x00, 
	0x46, 0x00, 0x34, 0x00, 0x46, 0x00, 0x46, 0x00, 0x39, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00
};    

SECTOR sectors[] = {
    /* Reserved Sectors: Master Boot Record */
    {(const uint8_t *)&mbr , 512},

    /* FAT Region: FAT1 */
    {fat1, sizeof(fat1)},   // fat1, sect1
    EMPTY_FAT_SECTORS

    /* FAT Region: FAT2 */
    {fat2, 0},              // fat2, sect1
    EMPTY_FAT_SECTORS

    /* Root Directory Region */
    {root_dir1, sizeof(root_dir1)}, // first 16 of the max 32 (mbr.max_root_dir_entries) root dir entries
    {root_dir2, sizeof(root_dir2)}, // last 16 of the max 32 (mbr.max_root_dir_entries) root dir entries

    /* Data Region */

    // Section for mac compatibility
    {sect5, sizeof(sect5)},

    // contains mbed.htm
    {(const uint8_t *)BlockBuf, 512},
};

static uint32_t size;
static uint32_t nb_sector;
static uint32_t current_sector;
static uint8_t sector_received_first;
static uint8_t root_dir_received_first;
static uint32_t jtag_flash_init;
static uint32_t flashPtr = START_APP_ADDRESS;
static uint8_t need_restart_usb;
static uint8_t flash_started;
static uint32_t start_sector;
static uint32_t theoretical_start_sector = 7;
static uint8_t msc_event_timeout;
static uint8_t good_file;
static uint8_t program_page_error;
static uint8_t maybe_erase;
static uint32_t previous_sector;
static uint32_t begin_sector;
static uint8_t task_first_started;
static uint8_t listen_msc_isr = 1;
static uint8_t drag_success = 1;
static uint8_t reason = 0;

#define SWD_ERROR               0
#define BAD_EXTENSION_FILE      1
#define NOT_CONSECUTIVE_SECTORS 2
#define SWD_PORT_IN_USE         3
#define RESERVED_BITS           4
#define BAD_START_SECTOR        5
#define TIMEOUT                 6

static uint8_t * reason_array[] = {
    "SWD ERROR",
    "BAD EXTENSION FILE",
    "NOT CONSECUTIVE SECTORS",
    "SWD PORT IN USE",
    "RESERVED BITS",
    "BAD START SECTOR",
    "TIMEOUT",
};

#define MSC_TIMEOUT_SPLIT_FILES_EVENT   (0x1000)
#define MSC_TIMEOUT_START_EVENT         (0x2000)
#define MSC_TIMEOUT_STOP_EVENT          (0x4000)
#define MSC_TIMEOUT_RESTART_EVENT       (0x8000)

#define PROGRAM_PAGE_EVENT              (0x0008)
#define FLASH_INIT_EVENT                (0x0004)

// 30 s timeout
#define TIMEOUT_S 3000

static U64 msc_task_stack[MSC_TASK_STACK/8];

// Reference to the msc task
static OS_TID msc_valid_file_timeout_task_id;
static OS_TID flash_programming_task_id;

static void init(uint8_t jtag);
static void initDisconnect(uint8_t success);

static void init(unsigned char jtag) {
    size = 0;
    nb_sector = 0;
    current_sector = 0;
    if (jtag) {
        jtag_flash_init = 0;
        theoretical_start_sector = (drag_success) ? 7 : 8;
        good_file = 0;
        program_page_error = 0;
        maybe_erase = 0;
        previous_sector = 0;
    }
    begin_sector = 0;
    flashPtr = START_APP_ADDRESS;
    sector_received_first = 0;
    root_dir_received_first = 0;
    need_restart_usb = 0;
    flash_started = 0;
    start_sector = 0;
    msc_event_timeout = 0;
    USBD_MSC_BlockBuf   = (uint8_t *)BlockBuf;
    listen_msc_isr = 1;
}

static void initDisconnect(uint8_t success) {
    drag_success = success;
    init(1);
    main_transfer_finished(success);
	isr_evt_set(MSC_TIMEOUT_STOP_EVENT, msc_valid_file_timeout_task_id);
}

static void disable_usb_irq(void){
#if defined(TARGET_LPC11U35)
    NVIC_DisableIRQ(USB_IRQn);
#elif defined(TARGET_MK20DX)
    NVIC_DisableIRQ(USB0_IRQn);
#endif
}

static void enable_usb_irq(void){
#if defined(TARGET_LPC11U35)
    NVIC_EnableIRQ(USB_IRQn);
#elif defined(TARGET_MK20DX)
    NVIC_EnableIRQ(USB0_IRQn);
#endif
}

static int program_page(unsigned long adr, unsigned long sz, unsigned char *buf) {
    // USB interrupt will then be re-enabled in the flash programming thread
    disable_usb_irq();
    isr_evt_set(PROGRAM_PAGE_EVENT, flash_programming_task_id);
    return 0;
}

static int program_sector() {
    // if we have received two sectors, write into flash
    if (!(current_sector % 2)) {
        //flash_erase_sector(flashPtr);
        if (program_page(flashPtr, 1024, (uint8_t *)BlockBuf)) {
            // even if there is an error, adapt flashptr
            flashPtr += 1024;
            return 1;
        }

        // if we just wrote the last sector -> disconnect usb
        if (current_sector == nb_sector) {
            initDisconnect(1);
            return 0;
        }

        flashPtr += 1024;

    }

    // we have to write the last sector which has a size of half a page
    if (current_sector == nb_sector) {
        if (current_sector % 2) {
            //flash_erase_sector(flashPtr);
            if (program_page(flashPtr, 1024, (uint8_t *)BlockBuf)) {
                return 1;
            }
        }
        initDisconnect(1);
    }
    return 0;
}

extern uint32_t SystemCoreClock;

__task void flash_programming_task(void) {
    uint32_t flags = 0, i;
    OS_RESULT res;
    flash_programming_task_id = os_tsk_self();
    while(1) {
        res = os_evt_wait_or(PROGRAM_PAGE_EVENT | FLASH_INIT_EVENT, NO_TIMEOUT);
        if (res == OS_R_EVT) {
            flags = os_evt_get();
            if (flags & PROGRAM_PAGE_EVENT) {
                flash_program_page_svc(flashPtr, 1024, (uint8_t *)BlockBuf);
                enable_usb_irq();

            }

            if (flags & FLASH_INIT_EVENT) {
                // init flash
                flash_hal_init(SystemCoreClock);

                // erase flash for new binary
                for(i = START_APP_ADDRESS; i < END_FLASH; i += SECTOR_SIZE) {
                    // TODO: failing to erase device flash
					flash_erase_sector_svc(i);
                }
                enable_usb_irq();
            }
        }
    }
}

// this task is responsible to check
// when we receive a root directory where there
// is a valid .bin file and when we have received
// all the sectors that we don't receive new valid sectors
// after a certain timeout
__task void msc_valid_file_timeout_task(void)
{
    uint32_t flags = 0;
    OS_RESULT res;
    uint32_t start_timeout_time = 0, time_now = 0;
    uint8_t timer_started = 0;
    msc_valid_file_timeout_task_id = os_tsk_self();
    while(1) {
        res = os_evt_wait_or(MSC_TIMEOUT_SPLIT_FILES_EVENT | MSC_TIMEOUT_START_EVENT | MSC_TIMEOUT_STOP_EVENT, 100);

        if (res == OS_R_EVT) {

            flags = os_evt_get();

            if (flags & MSC_TIMEOUT_SPLIT_FILES_EVENT) {
                msc_event_timeout = 1;
                os_dly_wait(50);

                if (msc_event_timeout == 1) {
                    // if the program reaches this point -> it means that no sectors have been received in the meantime
                    if (current_sector % 2) {
                        program_page(flashPtr, 1024, (uint8_t *)BlockBuf);
                    }
                    initDisconnect(1);
                    msc_event_timeout = 0;
                }
            }

            if (flags & MSC_TIMEOUT_START_EVENT) {
                start_timeout_time = os_time_get();
                timer_started = 1;
            }

            if (flags & MSC_TIMEOUT_STOP_EVENT) {
                timer_started = 0;
            }

        } else {
            if (timer_started) {
                time_now = os_time_get();
                // timeout
                if ((time_now - start_timeout_time) > TIMEOUT_S) {
                    timer_started = 0;
                    reason = TIMEOUT;
                    initDisconnect(0);
                }
            }
        }
    }
}

int jtag_init() 
{
    if (jtag_flash_init != 1) {
        if (need_restart_usb == 1) {
            reason = SWD_PORT_IN_USE;
            initDisconnect(0);
            return 1;
        }

        // USB interrupt will then be re-enabled in the flash programming thread
        disable_usb_irq();

        isr_evt_set(FLASH_INIT_EVENT, flash_programming_task_id);

        jtag_flash_init = 1;
    }
    return 0;
}


//void failSWD() {
//    reason = SWD_ERROR;
//    initDisconnect(0);
//}

static const FILE_TYPE_MAPPING file_type_infos[] = {
    { BIN_FILE, {'B', 'I', 'N'}, 0x00000000 },
    { BIN_FILE, {'b', 'i', 'n'}, 0x00000000 },
    { PAR_FILE, {'P', 'A', 'R'}, 0x00000000 },//strange extension on win IE 9...
    { DOW_FILE, {'D', 'O', 'W'}, 0x00000000 },//strange extension on mac...
    { CRD_FILE, {'C', 'R', 'D'}, 0x00000000 },//strange extension on linux...
    { UNSUP_FILE, {0,0,0},     0            },//end of table marker
};

static FILE_TYPE get_file_type(const FatDirectoryEntry_t* pDirEnt, uint32_t* pAddrOffset) {
    int i;
    char e0 = pDirEnt->filename[8];
    char e1 = pDirEnt->filename[9];
    char e2 = pDirEnt->filename[10];
    char f0 = pDirEnt->filename[0];
    for (i = 0; file_type_infos[i].type != UNSUP_FILE; i++) {
        if ((e0 == file_type_infos[i].extension[0]) &&
            (e1 == file_type_infos[i].extension[1]) &&
            (e2 == file_type_infos[i].extension[2])) {
            *pAddrOffset = file_type_infos[i].flash_offset;
            return file_type_infos[i].type;
        }
    }

    // Now test if the file has a valid extension and a valid name.
    // This is to detect correct but unsupported 8.3 file names.
    if (( ((e0 >= 'a') && (e0 <= 'z')) || ((e0 >= 'A') && (e0 <= 'Z')) ) &&
        ( ((e1 >= 'a') && (e1 <= 'z')) || ((e1 >= 'A') && (e1 <= 'Z')) || (e1 == 0x20) ) &&
        ( ((e2 >= 'a') && (e2 <= 'z')) || ((e2 >= 'A') && (e2 <= 'Z')) || (e2 == 0x20) ) &&
        ( ((f0 >= 'a') && (f0 <= 'z')) || ((f0 >= 'A') && (f0 <= 'Z')) ) &&
           (f0 != '.' &&
           (f0 != '_')) ) {
        *pAddrOffset = 0;
        return UNSUP_FILE;
    }

    *pAddrOffset = 0;
    return SKIP_FILE;
}

// take a look here: http://cs.nyu.edu/~gottlieb/courses/os/kholodov-fat.html
// to have info on fat file system
int search_bin_file(uint8_t * root, uint8_t sector) {
    // idx is a pointer inside the root dir
    // we begin after all the existing entries
    int idx = 0;
    uint8_t found = 0;
    uint32_t i = 0;
    FILE_TYPE file_type;
    uint8_t hidden_file = 0, adapt_th_sector = 0;
    uint32_t offset = 0;

    FatDirectoryEntry_t* pDirEnts = (FatDirectoryEntry_t*)root;

    if (sector == SECTORS_ROOT_IDX) {
        // move past known existing files in the root dir
        idx = (drag_success == 1) ? 12 : 13;
    }

    // first check that we did not receive any directory
    // if we detect a directory -> disconnect / failed
    for (i = idx; i < DIRENTS_PER_SECTOR; i++) {
        if (pDirEnts[i].attributes & 0x10) {
            reason = BAD_EXTENSION_FILE;
            initDisconnect(0);
            return -1;
        }
    }

    // now do the real search for a valid .bin file
    for (i = idx; i < DIRENTS_PER_SECTOR; i++) {

        // Determine file type and get the flash offset
        file_type = get_file_type(&pDirEnts[i], &offset);

        if (file_type == BIN_FILE || file_type == PAR_FILE ||
            file_type == DOW_FILE || file_type == CRD_FILE || file_type == SPI_FILE) {

            hidden_file = (pDirEnts[i].attributes & 0x02) ? 1 : 0;

            // compute the size of the file
            size = pDirEnts[i].filesize;

            if (size == 0) {
              // skip empty files
                continue;
            }

            // read the cluster number where data are stored (ignoring the
            // two high bytes in the cluster number)
            //
            // Convert cluster number to sector number by moving past the root
            // dir and fat tables.
            //
            // The cluster numbers start at 2 (0 and 1 are never used).
            begin_sector = (pDirEnts[i].first_cluster_low_16 - 2) * WANTED_SECTORS_PER_CLUSTER + SECTORS_FIRST_FILE_IDX;

            // compute the number of sectors
            nb_sector = (size + MBR_BYTES_PER_SECTOR - 1) / MBR_BYTES_PER_SECTOR;

            if ( (pDirEnts[i].filename[0] == '_') ||
                 (pDirEnts[i].filename[0] == '.') ||
                 (hidden_file && ((pDirEnts[i].filename[0] == '_') || (pDirEnts[i].filename[0] == '.'))) ||
                 ((pDirEnts[i].filename[0] == 0xE5) && (file_type != CRD_FILE) && (file_type != PAR_FILE))) {
                if (theoretical_start_sector == begin_sector) {
                    adapt_th_sector = 1;
                }
                size = 0;
                nb_sector = 0;
                continue;
            }

            // if we receive a file with a valid extension
            // but there has been program page error previously
            // we fail / disconnect usb
            if ((program_page_error == 1) && (maybe_erase == 0) && (start_sector >= begin_sector)) {
                reason = RESERVED_BITS;
                initDisconnect(0);
                return -1;
            }

            adapt_th_sector = 0;

            // on mac, with safari, we receive all the files with some more sectors at the beginning
            // we have to move the sectors... -> 2x slower
            if ((start_sector != 0) && (start_sector < begin_sector) && (current_sector - (begin_sector - start_sector) >= nb_sector)) {

                // we need to copy all the sectors
                // we don't listen to msd interrupt
                listen_msc_isr = 0;

//                 uint32_t move_sector_start = (begin_sector - start_sector)*MBR_BYTES_PER_SECTOR;
                uint32_t nb_sector_to_move = (nb_sector % 2) ? nb_sector/2 + 1 : nb_sector/2;
                for (i = 0; i < nb_sector_to_move; i++) {
					program_page(START_APP_ADDRESS + i*1024, 1024, (uint8_t *)BlockBuf);
                }
                initDisconnect(1);
                return -1;
            }

            found = 1;
            idx = i; // this is the file we want
            good_file = 1;
            break;
        }
        // if we receive a new file which does not have the good extension
        // fail and disconnect usb
        else if (file_type == UNSUP_FILE) {
            reason = BAD_EXTENSION_FILE;
            initDisconnect(0);
            return -1;
        }
		else {
			// TODO: left overs from bootloader, CMSIS integration. Not sure what it does
			idx += 32;
		}
    }

    if (adapt_th_sector) {
        theoretical_start_sector += nb_sector;
        init(0);
    }
    return (found == 1) ? idx : -1;
}


void usbd_msc_read_sect (uint32_t block, uint8_t *buf, uint32_t num_of_blocks)
{
    if (listen_msc_isr == 0)
        return;

    if (USBD_MSC_MediaReady) {
        // blink led not permanently
        //mainBlinkMsdLED(0);
        memset(buf, 0, 512);

        //if (block < 9) {
        if (block <= SECTORS_FIRST_FILE_IDX) {
            memcpy(buf, sectors[block].sect, sectors[block].length);

            // add new entry in FAT
            if ((block == 1) && (drag_success == 0)) {
                buf[9] = 0xff;
                buf[10] = 0x0f;
            //} else if ((block == 3) && (drag_success == 0)) {
            } else if ((block == SECTORS_ROOT_IDX) && (drag_success == 0)) {
                // Appends a new directory entry at the end of the root file system.
                //	The entry is a copy of "fail[]" and the size is updated to match the
                //	length of the error reason string. The entry's set to point to cluster
                //	4 which is the first after the mbed.htm file."
                memcpy(buf + sectors[block].length, fail, 16*2);
                // adapt size of file according fail reason
                buf[sectors[block].length + 28] = strlen((const char *)reason_array[reason]);
                buf[sectors[block].length + 26] = 6;
            }
        }
        // send System Volume Information
        else if (block == SECTORS_SYSTEM_VOLUME_INFORMATION) {
            memcpy(buf, sect6, sizeof(sect6));
        }
        // send System Volume Information/IndexerVolumeGuid
        else if (block == SECTORS_INDEXER_VOLUME_GUID) {
            memcpy(buf, sect7, sizeof(sect7));
        }
        // send mbed.html
        else if (block == SECTORS_MBED_HTML_IDX) {
            update_html_file();
        }
        else if (block == 9) {
            memcpy(buf, reason_array[reason], strlen((const char *)reason_array[reason]));
        }
    }
}

void usbd_msc_write_sect (uint32_t block, uint8_t *buf, uint32_t num_of_blocks)
{
    int idx_size = 0;

    if (listen_msc_isr == 0)
        return;

    // we recieve the root directory
    //if ((block == 3) || (block == 4)) {
    if ((block == SECTORS_ROOT_IDX) || (block == (SECTORS_ROOT_IDX+1))) {
        // try to find a .bin file in the root directory
        idx_size = search_bin_file(buf, block);

        // .bin file exists
        if (idx_size != -1) {

            if (sector_received_first == 0)
                root_dir_received_first = 1;

            // this means that we have received the sectors before root dir (linux)
            // we have to flush the last page into the target flash
            if ((sector_received_first == 1) && (current_sector == nb_sector) && (jtag_flash_init == 1)) {
                if (msc_event_timeout == 0) {
                    msc_event_timeout = 1;
                    isr_evt_set(MSC_TIMEOUT_SPLIT_FILES_EVENT, msc_valid_file_timeout_task_id);
                }
                return;
            }

            // means that we are receiving additionnal sectors
            // at the end of the file ===> we ignore them
            if ((sector_received_first == 1) && (start_sector == begin_sector) && (current_sector > nb_sector) && (jtag_flash_init == 1)) {
                initDisconnect(1);
                return;
            }
        }
    }
    //if (block > 4) {
    if (block >= SECTORS_ERROR_FILE_IDX) {

        //mainUSBBusyEvent();

        if (root_dir_received_first == 0)
            sector_received_first = 1;

        // if we don't receive consecutive sectors
        // set maybe erase in case we receive other sectors
        if ((previous_sector != 0) && ((previous_sector + 1) != block)) {
            maybe_erase = 1;
            return;
        }

        if (!flash_started && (block > theoretical_start_sector)) {
            theoretical_start_sector = block;
        }

        // init jtag if needed
        if (jtag_init() == 1) {
        	return;
        }

        if (jtag_flash_init == 1) {

            // we erase the chip if we received unrelated data before (mac compatibility)
            if (maybe_erase && (block == theoretical_start_sector)) {
                //if (!jtagEraseFlash())
                //    return;
                maybe_erase = 0;
                program_page_error = 0;
            }

            // drop block < theoretical_sector
            if (theoretical_start_sector > block) {
                return;
            }

            if ((flash_started == 0) && (theoretical_start_sector == block)) {
                flash_started = 1;
                isr_evt_set(MSC_TIMEOUT_START_EVENT, msc_valid_file_timeout_task_id);
                start_sector = block;
            }

            // at the beginning, we need theoretical_start_sector == block
            if ((flash_started == 0) && (theoretical_start_sector != block)) {
                reason = BAD_START_SECTOR;
                initDisconnect(0);
                return;
            }

            // not consecutive sectors detected
            if ((flash_started == 1) && (maybe_erase == 0) && (start_sector != block) && (block != (start_sector + current_sector))) {
                reason = NOT_CONSECUTIVE_SECTORS;
                initDisconnect(0);
                return;
            }

            // if we receive a new sector
            // and the msc thread has been started (we thought that the file has been fully received)
            // we kill the thread and write the sector in flash
            if (msc_event_timeout == 1) {
                msc_event_timeout = 0;
            }

            if (flash_started && (block == theoretical_start_sector)) {
                //if (!jtagEraseFlash())
                //    return;
                maybe_erase = 0;
                program_page_error = 0;
            }

            previous_sector = block;
            // adapt index in buffer
            current_sector++;
            USBD_MSC_BlockBuf = (current_sector % 2) ? (uint8_t *)BlockBuf + 512 : (uint8_t *)BlockBuf;
            if (program_sector() == 1) {
                if (good_file) {
                    reason = RESERVED_BITS;
                    initDisconnect(0);
                    return;
                }
                program_page_error = 1;
                return;
            }
        }
    }
}

void usbd_msc_init(void)
{
    if (!task_first_started) {
        task_first_started = 1;
        os_tsk_create_user(msc_valid_file_timeout_task, MSC_TASK_PRIORITY, msc_task_stack, MSC_TASK_STACK);
        os_tsk_create(flash_programming_task, FLASH_PROGRAMMING_TASK_PRIORITY);
    }

    //USBD_MSC_MemorySize = 512*512;
    USBD_MSC_MemorySize = MBR_NUM_NEEDED_SECTORS * MBR_BYTES_PER_SECTOR;
    USBD_MSC_BlockSize  = 512;
    USBD_MSC_BlockGroup = 1;
    USBD_MSC_BlockCount = USBD_MSC_MemorySize / USBD_MSC_BlockSize;
    USBD_MSC_BlockBuf   = (uint8_t *)BlockBuf;
    USBD_MSC_MediaReady = 1;
}

