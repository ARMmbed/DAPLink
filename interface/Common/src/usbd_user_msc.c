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

#include "target_flash.h"
#include "target_reset.h"
#include "DAP_config.h"
#include "dap.h"
#include "main.h"
#include "tasks.h"
#include "semihost.h"
#include "version.h"
#include "swd_host.h"
#include "usb_buf.h"

extern uint32_t usb_buffer[];
//extern USB_CONNECT usb_state;

__packed typedef struct {
    uint8_t boot_sector[11];
    /* DOS 2.0 BPB - Bios Parameter Block, 11 bytes */
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

typedef struct sector {
    const uint8_t * sect;
    unsigned int length;
} SECTOR;

#define MEDIA_DESCRIPTOR        (0xF0)

static const mbr_t mbr = {
    .boot_sector = {
        0xEB,0x3C, // x86 Jump
        0x90,      // NOP
        'M','S','D','0','S','5','.','0' // OEM Name in text (8 chars max)
    },
    .bytes_per_sector         = 512,
    .sectors_per_cluster      = 16,
    .reserved_logical_sectors = 1,
    
    .num_fats                 = 1, // 2 recommended
    .max_root_dir_entries     = 0x20,
    .total_logical_sectors    = 0xffff,//0x0020, //sizeof drive//((MBR_NUM_NEEDED_SECTORS > 32768) ? 0 : MBR_NUM_NEEDED_SECTORS),
    .media_descriptor         = 0xF0,
    .logical_sectors_per_fat  = 2,
    .physical_sectors_per_track = 1,
    .heads = 1,
    .hidden_sectors = 0,
     
    .big_sectors_on_drive = 0, //((MBR_NUM_NEEDED_SECTORS > 32768) ? MBR_NUM_NEEDED_SECTORS : 0),
    .physical_drive_number = 0,
    .not_used = 0,
    .boot_record_signature = 0x29,
    .volume_id = 0x27021974,
    .volume_label = {'M','B','E','D','D','A','P','L','I','N','K'},
    .file_system_type = {'F','A','T','1','2',' ',' ',' '},

    /* Executable boot code that starts the operating system */
    .bootstrap = {
        0x33,0xC9,0x8E,0xD1,0xBC,0xF0,0x7B,0x8E,0xD9,0xB8,0x00,0x20,0x8E,0xC0,0xFC,0xBD,
        0x00,0x7C,0x38,0x4E,0x24,0x7D,0x24,0x8B,0xC1,0x99,0xE8,0x3C,0x01,0x72,0x1C,0x83,
        0xEB,0x3A,0x66,0xA1,0x1C,0x7C,0x26,0x66,0x3B,0x07,0x26,0x8A,0x57,0xFC,0x75,0x06,
        0x80,0xCA,0x02,0x88,0x56,0x02,0x80,0xC3,0x10,0x73,0xEB,0x33,0xC9,0x8A,0x46,0x10,
        0x98,0xF7,0x66,0x16,0x03,0x46,0x1C,0x13,0x56,0x1E,0x03,0x46,0x0E,0x13,0xD1,0x8B,
        0x76,0x11,0x60,0x89,0x46,0xFC,0x89,0x56,0xFE,0xB8,0x20,0x00,0xF7,0xE6,0x8B,0x5E,
        0x0B,0x03,0xC3,0x48,0xF7,0xF3,0x01,0x46,0xFC,0x11,0x4E,0xFE,0x61,0xBF,0x00,0x00,
        0xE8,0xE6,0x00,0x72,0x39,0x26,0x38,0x2D,0x74,0x17,0x60,0xB1,0x0B,0xBE,0xA1,0x7D,
        0xF3,0xA6,0x61,0x74,0x32,0x4E,0x74,0x09,0x83,0xC7,0x20,0x3B,0xFB,0x72,0xE6,0xEB,
        0xDC,0xA0,0xFB,0x7D,0xB4,0x7D,0x8B,0xF0,0xAC,0x98,0x40,0x74,0x0C,0x48,0x74,0x13,
        0xB4,0x0E,0xBB,0x07,0x00,0xCD,0x10,0xEB,0xEF,0xA0,0xFD,0x7D,0xEB,0xE6,0xA0,0xFC,
        0x7D,0xEB,0xE1,0xCD,0x16,0xCD,0x19,0x26,0x8B,0x55,0x1A,0x52,0xB0,0x01,0xBB,0x00,
        0x00,0xE8,0x3B,0x00,0x72,0xE8,0x5B,0x8A,0x56,0x24,0xBE,0x0B,0x7C,0x8B,0xFC,0xC7,
        0x46,0xF0,0x3D,0x7D,0xC7,0x46,0xF4,0x29,0x7D,0x8C,0xD9,0x89,0x4E,0xF2,0x89,0x4E,
        0xF6,0xC6,0x06,0x96,0x7D,0xCB,0xEA,0x03,0x00,0x00,0x20,0x0F,0xB6,0xC8,0x66,0x8B,
        0x46,0xF8,0x66,0x03,0x46,0x1C,0x66,0x8B,0xD0,0x66,0xC1,0xEA,0x10,0xEB,0x5E,0x0F,
        0xB6,0xC8,0x4A,0x4A,0x8A,0x46,0x0D,0x32,0xE4,0xF7,0xE2,0x03,0x46,0xFC,0x13,0x56,
        0xFE,0xEB,0x4A,0x52,0x50,0x06,0x53,0x6A,0x01,0x6A,0x10,0x91,0x8B,0x46,0x18,0x96,
        0x92,0x33,0xD2,0xF7,0xF6,0x91,0xF7,0xF6,0x42,0x87,0xCA,0xF7,0x76,0x1A,0x8A,0xF2,
        0x8A,0xE8,0xC0,0xCC,0x02,0x0A,0xCC,0xB8,0x01,0x02,0x80,0x7E,0x02,0x0E,0x75,0x04,
        0xB4,0x42,0x8B,0xF4,0x8A,0x56,0x24,0xCD,0x13,0x61,0x61,0x72,0x0B,0x40,0x75,0x01,
        0x42,0x03,0x5E,0x0B,0x49,0x75,0x06,0xF8,0xC3,0x41,0xBB,0x00,0x00,0x60,0x66,0x6A,
        0x00,0xEB,0xB0,0x4E,0x54,0x4C,0x44,0x52,0x20,0x20,0x20,0x20,0x20,0x20,0x0D,0x0A,
        0x52,0x65,0x6D,0x6F,0x76,0x65,0x20,0x64,0x69,0x73,0x6B,0x73,0x20,0x6F,0x72,0x20,
        0x6F,0x74,0x68,0x65,0x72,0x20,0x6D,0x65,0x64,0x69,0x61,0x2E,0xFF,0x0D,0x0A,0x44,
        0x69,0x73,0x6B,0x20,0x65,0x72,0x72,0x6F,0x72,0xFF,0x0D,0x0A,0x50,0x72,0x65,0x73,
        0x73,0x20,0x61,0x6E,0x79,0x20,0x6B,0x65,0x79,0x20,0x74,0x6F,0x20,0x72,0x65,0x73,
        0x74,0x61,0x72,0x74,0x0D,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xAC,0xCB,0xD8
    },
    .signature = 0xAA55,
};


typedef struct file_allocation_table {
    uint8_t f[1024];
} file_allocation_table_t;

static const file_allocation_table_t fat1 = {
    .f = {
        0xF0, 0xFF, 
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xFF, 0x0F,
    }
};

static const uint8_t file1_contents[512] = "This is the file contents";

#include "version_git.h"

#if GIT_LOCAL_MODS == 1
    #warning "Building with local modifications."
    #define GIT_LOCAL_MODS_STR "Yes"
#else
    #define GIT_LOCAL_MODS_STR "No"
#endif

static const unsigned char WebSide[512] = {
"<!-- mbed Microcontroller Website and Authentication Shortcut -->\r\n"
"<!-- Version: " FW_BUILD " Build: " __DATE__ " " __TIME__ " Git Commit SHA: "  GIT_COMMIT_SHA " Git local mods:" GIT_LOCAL_MODS_STR"-->\r\n"
"<html>\r\n"
"<head>\r\n"
"<meta http-equiv=\"refresh\" content=\"0; url=http://mbed.org/device/?code=@A\"/>\r\n"
"<title>mbed Website Shortcut</title>\r\n"
"</head>\r\n"
"<body></body>\r\n"
"</html>\r\n"
"\r\n"};

typedef struct root_dir {
    FatDirectoryEntry_t dir;
    FatDirectoryEntry_t f1;
    FatDirectoryEntry_t f2;
    FatDirectoryEntry_t f3;
    FatDirectoryEntry_t f4;
    FatDirectoryEntry_t f5;
    FatDirectoryEntry_t f6;
    FatDirectoryEntry_t f7;
    FatDirectoryEntry_t f8;
    FatDirectoryEntry_t f9;
    FatDirectoryEntry_t f10;
    FatDirectoryEntry_t f11;
    FatDirectoryEntry_t f12;
    FatDirectoryEntry_t f13;
    FatDirectoryEntry_t f14;
    FatDirectoryEntry_t f15;
} root_dir_t;

root_dir_t dir = {
    .dir = {
    /*uint8_t[11] */ .filename = "MBED       ",
    /*uint8_t */ .attributes = 0x28,
    /*uint8_t */ .reserved = 0x00,
    /*uint8_t */ .creation_time_ms = 0x00,
    /*uint16_t*/ .creation_time = 0x0000,
    /*uint16_t*/ .creation_date = 0x0000,
    /*uint16_t*/ .accessed_date = 0x0000,
    /*uint16_t*/ .first_cluster_high_16 = 0x0000,
    /*uint16_t*/ .modification_time = 0x8E41,
    /*uint16_t*/ .modification_date = 0x32bb,
    /*uint16_t*/ .first_cluster_low_16 = 0x0000,
    /*uint32_t*/ .filesize = 0x00000000
    },
    .f1 = {
    /*uint8_t[11] */ .filename = "ABOUT   TXT",
    /*uint8_t */ .attributes = 0x01,
    /*uint8_t */ .reserved = 0x00,
    /*uint8_t */ .creation_time_ms = 0x00,
    /*uint16_t*/ .creation_time = 0x0000,
    /*uint16_t*/ .creation_date = 0x0021,
    /*uint16_t*/ .accessed_date = 0xbb32,
    /*uint16_t*/ .first_cluster_high_16 = 0x0000,
    /*uint16_t*/ .modification_time = 0x83dc,
    /*uint16_t*/ .modification_date = 0x32bb,
    /*uint16_t*/ .first_cluster_low_16 = 0x0002,
    /*uint32_t*/ .filesize = sizeof(file1_contents)
    },
    .f2  = {
    /*uint8_t[11] */ .filename = "MBED    HTM",
    /*uint8_t */ .attributes = 0x01,
    /*uint8_t */ .reserved = 0x00,
    /*uint8_t */ .creation_time_ms = 0x00,
    /*uint16_t*/ .creation_time = 0x0000,
    /*uint16_t*/ .creation_date = 0x0021,
    /*uint16_t*/ .accessed_date = 0xbb32,
    /*uint16_t*/ .first_cluster_high_16 = 0x0000,
    /*uint16_t*/ .modification_time = 0x83dc,
    /*uint16_t*/ .modification_date = 0x32bb,
    /*uint16_t*/ .first_cluster_low_16 = 0x0003,
    /*uint32_t*/ .filesize = sizeof(WebSide)
    },
    .f3  = {0},
    .f4  = {0},
    .f5  = {0},
    .f6  = {0},
    .f7  = {0},
    .f8  = {0},
    .f9  = {0},
    .f10 = {0},
    .f11 = {0},
    .f12 = {0},
    .f13 = {0},
    .f14 = {0},
    .f15 = {0},
};

typedef struct fs_items {
    uint8_t *sect;
    uint32_t length;
} fs_items_t;

const uint8_t blank_reigon[512] = {0};

const fs_items_t fs[] = {
    // fs setup
    {(uint8_t *)&mbr, sizeof(mbr)},
    {(uint8_t *)&fat1, sizeof(fat1)},
    //{(uint8_t *)&fat1, sizeof(fat1)},
    
    // root dir
    {(uint8_t *)&dir, sizeof(dir)},
    
    //file contents
    {(uint8_t *)&file1_contents, sizeof(file1_contents)},
    
    //empty area (16*512 is start of data reigion need to pad between files - 1
    {(uint8_t *)&blank_reigon, sizeof(blank_reigon)}, {(uint8_t *)&blank_reigon, sizeof(blank_reigon)},
    {(uint8_t *)&blank_reigon, sizeof(blank_reigon)}, {(uint8_t *)&blank_reigon, sizeof(blank_reigon)},
    {(uint8_t *)&blank_reigon, sizeof(blank_reigon)}, {(uint8_t *)&blank_reigon, sizeof(blank_reigon)},
    {(uint8_t *)&blank_reigon, sizeof(blank_reigon)}, {(uint8_t *)&blank_reigon, sizeof(blank_reigon)},
    {(uint8_t *)&blank_reigon, sizeof(blank_reigon)}, {(uint8_t *)&blank_reigon, sizeof(blank_reigon)},
    {(uint8_t *)&blank_reigon, sizeof(blank_reigon)}, {(uint8_t *)&blank_reigon, sizeof(blank_reigon)},
    {(uint8_t *)&blank_reigon, sizeof(blank_reigon)}, {(uint8_t *)&blank_reigon, sizeof(blank_reigon)},
    {(uint8_t *)&blank_reigon, sizeof(blank_reigon)},
    
    {(uint8_t *)&WebSide, sizeof(WebSide)},
    
    // end of fs data
    {(uint8_t *)0, 0},
};


U8 Memory[2048];                       /* MSC Memory in RAM                  */
U8 BlockBuf[512];

void virtual_fs_init(void)
{
    // ToDO: config fs specific things here that cant be done at compile time
    dir.f1.filesize = strlen((const char *)file1_contents);
    dir.f2.filesize = strlen((const char *)WebSide);
}

void usbd_msc_init () {    
    
    virtual_fs_init();
    
    USBD_MSC_MemorySize = 8192*512;
    USBD_MSC_BlockSize  = 512;
    USBD_MSC_BlockGroup = 1;
    USBD_MSC_BlockCount = USBD_MSC_MemorySize / USBD_MSC_BlockSize;
    USBD_MSC_BlockBuf   = BlockBuf;

    USBD_MSC_MediaReady = __TRUE;
}

void usbd_msc_read_sect (U32 block, U8 *buf, U32 num_of_blocks) {
    fs_items_t fs_read_info = {0,0};
    uint32_t req_block_offset = block * USBD_MSC_BlockSize;
    uint32_t fs_expand_sector_offset = 0;
    uint32_t sector_offset = 0;
    uint8_t i = 0;
    
    // dont proceed if we're not ready
    if (!USBD_MSC_MediaReady) {
        return;
    }
    
    // find the location of the data that is being requested by the host
    // fs[i].length 0 is a dummy end sector
    // fs_read_info.sect is a pointer to the sector we need data from
    while((fs[i].length != 0) && (fs_read_info.sect == 0)) {
        // accumulate the length of the sectors
        fs_expand_sector_offset += fs[i].length;
        // the data is in this sector
        if (req_block_offset < fs_expand_sector_offset) {
            fs_read_info.sect = fs[i].sect;
            // can have more than one block in a sector - normalize the block number
            sector_offset = fs[i].length - (fs_expand_sector_offset - (block * USBD_MSC_BlockSize));
        }
        i++;
    }
    // now send the data
    if (fs_read_info.sect != 0) {
        memcpy(buf, &fs_read_info.sect[sector_offset], num_of_blocks * USBD_MSC_BlockSize);
    }
    else {
        memset(buf, 0, num_of_blocks * USBD_MSC_BlockSize);
    }
}

#define DEBUG_MSC_FILE_TRANSFER
#include "stdio.h"

// known extension types
static const char *known_extensions[] = {
    "BIN",
    "bin",
    0,
};

static uint32_t extension_is_known(const FatDirectoryEntry_t dir_entry)
{
    uint32_t i = 0;
    while (known_extensions[i] != 0) {
        if (0 == strncmp(known_extensions[i], (const char *)&dir_entry.filename[8], 3)) {
            // we may see the entry with a false filesize. Validate both or keep looking
            return (dir_entry.filesize) ? dir_entry.filesize : 0;
        }
        i++;
    }
    return 0;
}

void usbd_msc_write_sect (U32 block, U8 *buf, U32 num_of_blocks) {    
    FatDirectoryEntry_t tmp_file = {0};
    uint32_t i = 0;
    static int32_t start_block = -1;
    static uint32_t amt_written = 0, amt_to_write = 0;
    
    if (!USBD_MSC_MediaReady) {
        return;
    }
#if defined(DEBUG_MSC_FILE_TRANSFER)
            char block_msg[32] = {0};
            sprintf(block_msg, "block: 0x%08X\r\n", block);
            USBD_CDC_ACM_DataSend((uint8_t *)&block_msg, strlen(block_msg));
            os_dly_wait(5);
#endif    
    // reset parsing when the mbr is sent
    if (block == 0) {
        start_block = -1;
        amt_written = 0;
        amt_to_write = 0;
    }
    
    // start looking for the new file and if we know how to parse it
    for( ; i < USBD_MSC_BlockSize/sizeof(tmp_file); i++) {
        memcpy(&tmp_file, &buf[i * sizeof(tmp_file)], sizeof(tmp_file));
        // ToDO: do a better lookup here for all known extensions
        if (extension_is_known(tmp_file)) {
            start_block = tmp_file.first_cluster_low_16;
            amt_to_write = tmp_file.filesize;
#if defined(DEBUG_MSC_FILE_TRANSFER)
            char msg[32] = {0};
            sprintf(msg, "start sector 0x%08X 0x%08X\r\n", 
                tmp_file.first_cluster_high_16,
                tmp_file.first_cluster_low_16);
            USBD_CDC_ACM_DataSend((uint8_t *)&msg, strlen(msg));
            os_dly_wait(5);
#endif
        }
    }
    
    // now we are receiving file data for a known file type (ToDO: dont like the 12 need to test different OS's)
    if (block >= (start_block * 12) && amt_written < amt_to_write) {
        amt_written += USBD_MSC_BlockSize;
// compare parsed file across the CDC link
#if defined(DEBUG_MSC_FILE_TRANSFER)
//        USBD_CDC_ACM_DataSend(buf, USBD_MSC_BlockSize);
//        os_dly_wait(50);
#endif        
    }
}





























//static const FatDirectoryEntry_t drive = {
//    /*uint8_t[11] */ .filename = "MBED       ",
//    /*uint8_t */ .attributes = 0x28,
//    /*uint8_t */ .reserved = 0x00,
//    /*uint8_t */ .creation_time_ms = 0x00,
//    /*uint16_t*/ .creation_time = 0x0000,
//    /*uint16_t*/ .creation_date = 0x0000,
//    /*uint16_t*/ .accessed_date = 0x0000,
//    /*uint16_t*/ .first_cluster_high_16 = 0x0000,
//    /*uint16_t*/ .modification_time = 0x8E41,
//    /*uint16_t*/ .modification_date = 0x32bb,
//    /*uint16_t*/ .first_cluster_low_16 = 0x0000,
//    /*uint32_t*/ .filesize = 0x00000000,
//};


//static const FatDirectoryEntry_t file1 = {
//    /*uint8_t[11] */ .filename = "ABOUT   TXT",
//    /*uint8_t */ .attributes = 0x21,
//    /*uint8_t */ .reserved = 0x00,
//    /*uint8_t */ .creation_time_ms = 0x00,
//    /*uint16_t*/ .creation_time = 0x0000,
//    /*uint16_t*/ .creation_date = 0x0021,
//    /*uint16_t*/ .accessed_date = 0xbb32,
//    /*uint16_t*/ .first_cluster_high_16 = 0x0000,
//    /*uint16_t*/ .modification_time = 0x83dc,
//    /*uint16_t*/ .modification_date = 0x32bb,
//    /*uint16_t*/ .first_cluster_low_16 = 0x0002,
//    /*uint32_t*/ .filesize = sizeof(file1_contents)
//};

//static const uint8_t fat2[] = {0};

//static const uint8_t fail[] = {
//    'F','A','I','L',' ',' ',' ',' ',                   // Filename
//    'T','X','T',                                       // Filename extension
//    0x20,                                              // File attributes
//    0x18,0xB1,0x74,0x76,0x8E,0x41,0x8E,0x41,0x00,0x00, // Reserved
//    0x8E,0x76,                                         // Time created or last updated
//    0x8E,0x41,                                         // Date created or last updated
//    0x06,0x00,                                         // Starting cluster number for file
//    0x07,0x00,0x00,0x0                                 // File size in bytes
//};

//// first 16 of the max 32 (mbr.max_root_dir_entries) root dir entries
//static const uint8_t root_dir1[] = {
//    // volume label "MBED"
//    'M', 'B', 'E', 'D', 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x0, 0x0, 0x0, 0x0,
//    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x85, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

//    // Hidden files to keep mac happy

//    // .fseventsd (LFN + normal entry)  (folder, size 0, cluster 2)
//    0x41, 0x2E, 0x0, 0x66, 0x0, 0x73, 0x0, 0x65, 0x0, 0x76, 0x0, 0xF, 0x0, 0xDA, 0x65, 0x0,
//    0x6E, 0x0, 0x74, 0x0, 0x73, 0x0, 0x64, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF,

//    0x46, 0x53, 0x45, 0x56, 0x45, 0x4E, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x12, 0x0, 0x47, 0x7D, 0x75,
//    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x7D, 0x75, 0x8E, 0x41, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0,

//    // .metadata_never_index (LFN + LFN + normal entry)  (size 0, cluster 0)
//    0x42, 0x65, 0x0, 0x72, 0x0, 0x5F, 0x0, 0x69, 0x0, 0x6E, 0x0, 0xF, 0x0, 0xA8, 0x64, 0x0,
//    0x65, 0x0, 0x78, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF,

//    0x1, 0x2E, 0x0, 0x6D, 0x0, 0x65, 0x0, 0x74, 0x0, 0x61, 0x0, 0xF, 0x0, 0xA8, 0x64, 0x0,
//    0x61, 0x0, 0x74, 0x0, 0x61, 0x0, 0x5F, 0x0, 0x6E, 0x0, 0x0, 0x0, 0x65, 0x0, 0x76, 0x0,

//    0x4D, 0x45, 0x54, 0x41, 0x44, 0x41, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x22, 0x0, 0x32, 0x85, 0x75,
//    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x85, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

//    // .Trashes (LFN + normal entry)  (size 0, cluster 0)
//    0x41, 0x2E, 0x0, 0x54, 0x0, 0x72, 0x0, 0x61, 0x0, 0x73, 0x0, 0xF, 0x0, 0x25, 0x68, 0x0,
//    0x65, 0x0, 0x73, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF,

//    0x54, 0x52, 0x41, 0x53, 0x48, 0x45, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x22, 0x0, 0x32, 0x85, 0x75,
//    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x85, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

//    // Hidden files to keep windows 8.1 happy
//    0x42, 0x20, 0x00, 0x49, 0x00, 0x6E, 0x00, 0x66, 0x00, 0x6F, 0x00, 0x0F, 0x00, 0x72, 0x72, 0x00,
//    0x6D, 0x00, 0x61, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x00, 0x00,

//    0x01, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x0F, 0x00, 0x72, 0x6D, 0x00,
//    0x20, 0x00, 0x56, 0x00, 0x6F, 0x00, 0x6C, 0x00, 0x75, 0x00, 0x00, 0x00, 0x6D, 0x00, 0x65, 0x00,

//    0x53, 0x59, 0x53, 0x54, 0x45, 0x4D, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x16, 0x00, 0xA5, 0x85, 0x8A,
//    0x73, 0x43, 0x73, 0x43, 0x00, 0x00, 0x86, 0x8A, 0x73, 0x43, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,

//    // mbed html file (size 512, cluster 3)
//    'M', 'B', 'E', 'D', 0x20, 0x20, 0x20, 0x20, 'H', 'T', 'M', 0x20, 0x18, 0xB1, 0x74, 0x76,
//    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x8E, 0x76, 0x8E, 0x41, 0x05, 0x0, 0x00, 0x02, 0x0, 0x0,
//};

// last 16 of the max 32 (mbr.max_root_dir_entries) root dir entries
//static const uint8_t root_dir2[] = {0};

//static const uint8_t sect5[] = {
//    // .   (folder, size 0, cluster 2)
//    0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x32, 0x0, 0x47, 0x7D, 0x75,
//    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x88, 0x75, 0x8E, 0x41, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0,

//    // ..   (folder, size 0, cluster 0)
//    0x2E, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x0, 0x47, 0x7D, 0x75,
//    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x7D, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

//    // NO_LOG  (size 0, cluster 0)
//    0x4E, 0x4F, 0x5F, 0x4C, 0x4F, 0x47, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x8, 0x32, 0x85, 0x75,
//    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x85, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
//};

//static const uint8_t sect6[] = {
//    0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x00, 0xA5, 0x85, 0x8A,
//    0x73, 0x43, 0x73, 0x43, 0x00, 0x00, 0x86, 0x8A, 0x73, 0x43, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,

//    0x2E, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x00, 0xA5, 0x85, 0x8A,
//    0x73, 0x43, 0x73, 0x43, 0x00, 0x00, 0x86, 0x8A, 0x73, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

//    // IndexerVolumeGuid (size0, cluster 0)
//    0x42, 0x47, 0x00, 0x75, 0x00, 0x69, 0x00, 0x64, 0x00, 0x00, 0x00, 0x0F, 0x00, 0xFF, 0xFF, 0xFF,
//    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,

//    0x01, 0x49, 0x00, 0x6E, 0x00, 0x64, 0x00, 0x65, 0x00, 0x78, 0x00, 0x0F, 0x00, 0xFF, 0x65, 0x00,
//    0x72, 0x00, 0x56, 0x00, 0x6F, 0x00, 0x6C, 0x00, 0x75, 0x00, 0x00, 0x00, 0x6D, 0x00, 0x65, 0x00,

//    0x49, 0x4E, 0x44, 0x45, 0x58, 0x45, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x20, 0x00, 0xA7, 0x85, 0x8A,
//    0x73, 0x43, 0x73, 0x43, 0x00, 0x00, 0x86, 0x8A, 0x73, 0x43, 0x04, 0x00, 0x4C, 0x00, 0x00, 0x00
//};

//static const uint8_t sect7[] = {
//	0x7B, 0x00, 0x39, 0x00, 0x36, 0x00, 0x36, 0x00, 0x31, 0x00, 0x39, 0x00, 0x38, 0x00, 0x32, 0x00,
//	0x30, 0x00, 0x2D, 0x00, 0x37, 0x00, 0x37, 0x00, 0x44, 0x00, 0x31, 0x00, 0x2D, 0x00, 0x34, 0x00,
//	0x46, 0x00, 0x38, 0x00, 0x38, 0x00, 0x2D, 0x00, 0x38, 0x00, 0x46, 0x00, 0x35, 0x00, 0x33, 0x00,
//	0x2D, 0x00, 0x36, 0x00, 0x32, 0x00, 0x44, 0x00, 0x39, 0x00, 0x37, 0x00, 0x46, 0x00, 0x35, 0x00,
//	0x46, 0x00, 0x34, 0x00, 0x46, 0x00, 0x46, 0x00, 0x39, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00
//};


//SECTOR sectors[] = {
//    /* Reserved Sectors: Master Boot Record */
//    {(const uint8_t *)&mbr , sizeof(mbr)},

//    /* FAT Region: FAT1 */
//    {(const uint8_t *)&fat1, sizeof(fat1)},   // fat1, sect1
//    
//    EMPTY_FAT_SECTORS

//    /* FAT Region: FAT2 */
//    {fat2, 0},              // fat2, sect1
//    EMPTY_FAT_SECTORS

//    /* Root Directory Region */
//    //{root_dir1, sizeof(root_dir1)}, // first 16 of the max 32 (mbr.max_root_dir_entries) root dir entries
//    //{root_dir2, sizeof(root_dir2)}, // last 16 of the max 32 (mbr.max_root_dir_entries) root dir entries

//    /* Data Region */

//    // Section for mac compatibility
//    //{sect5, sizeof(sect5)},
//    //{sect6, sizeof(sect6)},
//    //{sect7, sizeof(sect7)},

//    // contains mbed.htm
//    //{(const uint8_t *)usb_buffer, 512},
//};

//typedef struct file_system {
//    mbr_t *br;
//    
//    fat_t *fat;
//    
//    FatDirectoryEntry_t *root;
//    FatDirectoryEntry_t *f1;
//        
//} file_system_t;

//#if defined(DBG_LPC1768)
//#   define WANTED_SIZE_IN_KB                        (512)
//#elif defined(DBG_KL02Z)
//#   define WANTED_SIZE_IN_KB                        (32)
//#elif defined(DBG_KL05Z)
//#   define WANTED_SIZE_IN_KB                        (32)
//#elif defined(DBG_K24F256)
//#   define WANTED_SIZE_IN_KB                        (256)
//#elif defined(DBG_KL25Z)
//#   define WANTED_SIZE_IN_KB                        (128)
//#elif defined(DBG_KL26Z)
//#   define WANTED_SIZE_IN_KB                        (128)
//#elif defined(DBG_KL46Z)
//#   define WANTED_SIZE_IN_KB                        (256)
//#elif defined(DBG_K20D50M)
//#   define WANTED_SIZE_IN_KB                        (128)
//#elif defined(DBG_K22F)
//#   define WANTED_SIZE_IN_KB                        (512)
//#elif defined(DBG_K64F)
//#   define WANTED_SIZE_IN_KB                        (1024)
//#elif defined(DBG_LPC812)
//#   define WANTED_SIZE_IN_KB                        (16)
//#elif defined(DBG_LPC1114)
//#   define WANTED_SIZE_IN_KB                        (32)
//#elif defined(DBG_LPC4330)
//#   if defined(BOARD_BAMBINO_210E)
//#       define WANTED_SIZE_IN_KB                    (8192)
//#   else
//#       define WANTED_SIZE_IN_KB                    (4096)
//#   endif
//#elif defined(DBG_LPC1549)
//#   define WANTED_SIZE_IN_KB                        (512)
//#elif defined(DBG_LPC11U68)
//#   define WANTED_SIZE_IN_KB                        (256)
//#elif defined(DBG_LPC4337)
//#   define WANTED_SIZE_IN_KB                        (1024)
//#endif

////------------------------------------------------------------------- CONSTANTS
//#define WANTED_SIZE_IN_BYTES        ((WANTED_SIZE_IN_KB + 16 + 8)*1024)
//#define WANTED_SECTORS_PER_CLUSTER  (8)

//#define FLASH_PROGRAM_PAGE_SIZE         (512)
//#define MBR_BYTES_PER_SECTOR            (512)

////--------------------------------------------------------------------- DERIVED

//#define MBR_NUM_NEEDED_SECTORS  (WANTED_SIZE_IN_BYTES / MBR_BYTES_PER_SECTOR)
//#define MBR_NUM_NEEDED_CLUSTERS (MBR_NUM_NEEDED_SECTORS / WANTED_SECTORS_PER_CLUSTER)

///* Need 3 sectors/FAT for every 1024 clusters */
//#define MBR_SECTORS_PER_FAT     (3*((MBR_NUM_NEEDED_CLUSTERS + 1023)/1024))

///* Macro to help fill the two FAT tables with the empty sectors without
//   adding a lot of test #ifs inside the sectors[] declaration below */
//#if (MBR_SECTORS_PER_FAT == 1)
//#   define EMPTY_FAT_SECTORS
//#elif (MBR_SECTORS_PER_FAT == 2)
//#   define EMPTY_FAT_SECTORS  {fat2,0},
//#elif (MBR_SECTORS_PER_FAT == 3)
//#   define EMPTY_FAT_SECTORS  {fat2,0},{fat2,0},
//#elif (MBR_SECTORS_PER_FAT == 6)
//#   define EMPTY_FAT_SECTORS  {fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},
//#elif (MBR_SECTORS_PER_FAT == 9)
//#   define EMPTY_FAT_SECTORS  {fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},
//#elif (MBR_SECTORS_PER_FAT == 12)
//#   define EMPTY_FAT_SECTORS  {fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},{fat2,0},
//#else
//#   error "Unsupported number of sectors per FAT table"
//#endif

//#define DIRENTS_PER_SECTOR  (MBR_BYTES_PER_SECTOR / sizeof(FatDirectoryEntry_t))

//#define SECTORS_ROOT_IDX        (1 + mbr.num_fats*MBR_SECTORS_PER_FAT)
//#define SECTORS_FIRST_FILE_IDX  (SECTORS_ROOT_IDX + 2)
//#define SECTORS_SYSTEM_VOLUME_INFORMATION (SECTORS_FIRST_FILE_IDX  + WANTED_SECTORS_PER_CLUSTER)
//#define SECTORS_INDEXER_VOLUME_GUID       (SECTORS_SYSTEM_VOLUME_INFORMATION + WANTED_SECTORS_PER_CLUSTER)
//#define SECTORS_MBED_HTML_IDX   (SECTORS_INDEXER_VOLUME_GUID + WANTED_SECTORS_PER_CLUSTER)
//#define SECTORS_ERROR_FILE_IDX  (SECTORS_MBED_HTML_IDX + WANTED_SECTORS_PER_CLUSTER)

////---------------------------------------------------------------- VERIFICATION

///* Sanity check */
//#if (MBR_NUM_NEEDED_CLUSTERS > 4084)
//  /* Limited by 12 bit cluster addresses, i.e. 2^12 but only 0x002..0xff5 can be used */
//#   error Too many needed clusters, increase WANTED_SECTORS_PER_CLUSTER
//#endif

//#if ((WANTED_SECTORS_PER_CLUSTER * MBR_BYTES_PER_SECTOR) > 32768)
//#   error Cluster size too large, must be <= 32KB
//#endif

////-------------------------------------------------------------------- TYPEDEFS


//typedef enum {
//    BIN_FILE,
//    PAR_FILE,
//    DOW_FILE,
//    CRD_FILE,
//    SPI_FILE,
//    UNSUP_FILE, /* Valid extension, but not supported */
//    SKIP_FILE,  /* Unknown extension, typically Long File Name entries */
//} FILE_TYPE;

//typedef struct {
//    FILE_TYPE type;
//    char extension[3];
//    uint32_t flash_offset;
//} FILE_TYPE_MAPPING;

    
//typedef struct file {
//    FatDirectoryEntry_t files[];
//}file_t;

//file_t ff = {
//    {
//        drive, 
//        file1
//    }
//};

//static uint32_t size;
//static uint32_t nb_sector;
//static uint32_t current_sector;
//static uint8_t sector_received_first;
//static uint8_t root_dir_received_first;
//static uint32_t jtag_flash_init;
//static uint32_t flashPtr;
//static uint8_t need_restart_usb;
//static uint8_t flash_started;
//static uint32_t start_sector;
//static uint32_t theoretical_start_sector = 7;
//static uint8_t msc_event_timeout;
//static uint8_t good_file;
//static uint8_t program_page_error;
//static uint8_t maybe_erase;
//static uint32_t previous_sector;
//static uint32_t begin_sector;
//static uint8_t task_first_started;
//static uint8_t listen_msc_isr = 1;
//static uint8_t drag_success = 1;
//static uint8_t reason = 0;
//static uint32_t flash_addr_offset = 0;

//#define SWD_ERROR               0
//#define BAD_EXTENSION_FILE      1
//#define NOT_CONSECUTIVE_SECTORS 2
//#define SWD_PORT_IN_USE         3
//#define RESERVED_BITS           4
//#define BAD_START_SECTOR        5
//#define TIMEOUT                 6

//static uint8_t * reason_array[] = {
//    "SWD ERROR",
//    "BAD EXTENSION FILE",
//    "NOT CONSECUTIVE SECTORS",
//    "SWD PORT IN USE",
//    "RESERVED BITS",
//    "BAD START SECTOR",
//    "TIMEOUT",
//};

//#define MSC_TIMEOUT_SPLIT_FILES_EVENT   (0x1000)
//#define MSC_TIMEOUT_START_EVENT         (0x2000)
//#define MSC_TIMEOUT_STOP_EVENT          (0x4000)
//#define MSC_TIMEOUT_RESTART_EVENT       (0x8000)

//// 30 s timeout
//#define TIMEOUT_S 3000

//static U64 msc_task_stack[MSC_TASK_STACK/8];

// Reference to the msc task
//static OS_TID msc_valid_file_timeout_task_id;

//static void init(uint8_t jtag);
//static void initDisconnect(uint8_t success);

// this task is responsible to check
// when we receive a root directory where there
// is a valid .bin file and when we have received
// all the sectors that we don't receive new valid sectors
// after a certain timeout
//__task void msc_valid_file_timeout_task(void) {
//    uint32_t flags = 0;
//    OS_RESULT res;
//    uint32_t start_timeout_time = 0, time_now = 0;
//    uint8_t timer_started = 0;
//    msc_valid_file_timeout_task_id = os_tsk_self();
//    while (1) {
//        res = os_evt_wait_or(MSC_TIMEOUT_SPLIT_FILES_EVENT | MSC_TIMEOUT_START_EVENT | MSC_TIMEOUT_STOP_EVENT | MSC_TIMEOUT_RESTART_EVENT, 100);

//        if (res == OS_R_EVT) {

//            flags = os_evt_get();

//            if (flags & MSC_TIMEOUT_SPLIT_FILES_EVENT) {
//                msc_event_timeout = 1;
//                os_dly_wait(50);

//                if (msc_event_timeout == 1) {
//                    // if the program reaches this point -> it means that no sectors have been received in the meantime
//                    initDisconnect(1);
//                    msc_event_timeout = 0;
//                }
//            }

//            if (flags & MSC_TIMEOUT_START_EVENT) {
//                start_timeout_time = os_time_get();
//                timer_started = 1;
//            }

//            if (flags & MSC_TIMEOUT_STOP_EVENT) {
//                timer_started = 0;
//            }

//            if (flags & MSC_TIMEOUT_RESTART_EVENT) {
//                if (timer_started) {
//                    start_timeout_time = os_time_get();
//                }
//            }

//        } else {
//            if (timer_started) {
//                time_now = os_time_get();
//                // timeout
//                if ((time_now - start_timeout_time) > TIMEOUT_S) {
//                    timer_started = 0;
//                    reason = TIMEOUT;
//                    initDisconnect(0);
//                }
//            }
//        }
//    }
//}

//void init(uint8_t jtag) {
//    size = 0;
//    nb_sector = 0;
//    current_sector = 0;
//    if (jtag) {
//        jtag_flash_init = 0;
//        theoretical_start_sector = (drag_success) ? 7 : 8;
//        good_file = 0;
//        program_page_error = 0;
//        maybe_erase = 0;
//        previous_sector = 0;
//    }
//    begin_sector = 0;
//    flashPtr = 0;
//    sector_received_first = 0;
//    root_dir_received_first = 0;
//    need_restart_usb = 0;
//    flash_started = 0;
//    start_sector = 0;
//    msc_event_timeout = 0;
//    USBD_MSC_BlockBuf   = (uint8_t *)usb_buffer;
//    listen_msc_isr = 1;
//    flash_addr_offset = 0;
//}

//void failSWD() {
//    reason = SWD_ERROR;
//    initDisconnect(0);
//}

//extern DAP_Data_t DAP_Data;  // DAP_Data.debug_port

//#ifdef BOARD_UBLOX_C027
//#include "read_uid.h"
//#endif

//static void initDisconnect(uint8_t success) {
//#if defined(BOARD_UBLOX_C027)
//    int autorst = (good_file == 2) && success;
//    int autocrp = (good_file == 3) && success;
//    if (autocrp)
//    {
//        // first we need to discoonect the usb stack
//        usbd_connect(0);

//        enter_isp();
//    }
//#else
//    int autorst = 0;
//#endif
//    drag_success = success;
//    if (autorst)
//        swd_set_target_state(RESET_RUN);
//    main_blink_msd_led(0);
//    init(1);
//    isr_evt_set(MSC_TIMEOUT_STOP_EVENT, msc_valid_file_timeout_task_id);
//    if (!autorst)
//    {
//        // event to disconnect the usb
//        main_usb_disconnect_event();
//    }
//    semihost_enable();
//}

//extern uint32_t SystemCoreClock;

//int jtag_init() {
//    if (DAP_Data.debug_port != DAP_PORT_DISABLED) {
//        need_restart_usb = 1;
//    }

//    if ((jtag_flash_init != 1) && (DAP_Data.debug_port == DAP_PORT_DISABLED)) {
//        if (need_restart_usb == 1) {
//            reason = SWD_PORT_IN_USE;
//            initDisconnect(0);
//            return 1;
//        }

//        semihost_disable();

//        PORT_SWD_SETUP();

//        target_set_state(RESET_PROGRAM);
//        if (!target_flash_init(SystemCoreClock)) {
//            failSWD();
//            return 1;
//        }

//        jtag_flash_init = 1;
//    }
//    return 0;
//}


//static const FILE_TYPE_MAPPING file_type_infos[] = {
//    { BIN_FILE, {'B', 'I', 'N'}, 0x00000000 },
//    { BIN_FILE, {'b', 'i', 'n'}, 0x00000000 },
//    { PAR_FILE, {'P', 'A', 'R'}, 0x00000000 },//strange extension on win IE 9...
//    { DOW_FILE, {'D', 'O', 'W'}, 0x00000000 },//strange extension on mac...
//    { CRD_FILE, {'C', 'R', 'D'}, 0x00000000 },//strange extension on linux...
//    { UNSUP_FILE, {0,0,0},     0            },//end of table marker
//};

//static FILE_TYPE get_file_type(const FatDirectoryEntry_t* pDirEnt, uint32_t* pAddrOffset) {
//    int i;
//    char e0 = pDirEnt->filename[8];
//    char e1 = pDirEnt->filename[9];
//    char e2 = pDirEnt->filename[10];
//    char f0 = pDirEnt->filename[0];
//    for (i = 0; file_type_infos[i].type != UNSUP_FILE; i++) {
//        if ((e0 == file_type_infos[i].extension[0]) &&
//            (e1 == file_type_infos[i].extension[1]) &&
//            (e2 == file_type_infos[i].extension[2])) {
//            *pAddrOffset = file_type_infos[i].flash_offset;
//            return file_type_infos[i].type;
//        }
//    }

//    // Now test if the file has a valid extension and a valid name.
//    // This is to detect correct but unsupported 8.3 file names.
//    if (( ((e0 >= 'a') && (e0 <= 'z')) || ((e0 >= 'A') && (e0 <= 'Z')) ) &&
//        ( ((e1 >= 'a') && (e1 <= 'z')) || ((e1 >= 'A') && (e1 <= 'Z')) || (e1 == 0x20) ) &&
//        ( ((e2 >= 'a') && (e2 <= 'z')) || ((e2 >= 'A') && (e2 <= 'Z')) || (e2 == 0x20) ) &&
//        ( ((f0 >= 'a') && (f0 <= 'z')) || ((f0 >= 'A') && (f0 <= 'Z')) ) &&
//           (f0 != '.' &&
//           (f0 != '_')) ) {
//        *pAddrOffset = 0;
//        return UNSUP_FILE;
//    }

//    *pAddrOffset = 0;
//    return SKIP_FILE;
//}

// take a look here: http://cs.nyu.edu/~gottlieb/courses/os/kholodov-fat.html
// to have info on fat file system
//int search_bin_file(uint8_t * root, uint8_t sector) {
//    // idx is a pointer inside the root dir
//    // we begin after all the existing entries
//    int idx = 0;
//    uint8_t found = 0;
//    uint32_t i = 0;
//    uint32_t move_sector_start = 0, nb_sector_to_move = 0;
//    FILE_TYPE file_type;
//    uint8_t hidden_file = 0, adapt_th_sector = 0;
//    uint32_t offset = 0;

//    FatDirectoryEntry_t* pDirEnts = (FatDirectoryEntry_t*)root;

//    if (sector == SECTORS_ROOT_IDX) {
//        // move past known existing files in the root dir
//        idx = (drag_success == 1) ? 12 : 13;
//    }

//    // first check that we did not receive any directory
//    // if we detect a directory -> disconnect / failed
//    for (i = idx; i < DIRENTS_PER_SECTOR; i++) {
//        if (pDirEnts[i].attributes & 0x10) {
//            reason = BAD_EXTENSION_FILE;
//            initDisconnect(0);
//            return -1;
//        }
//    }

//    // now do the real search for a valid .bin file
//    for (i = idx; i < DIRENTS_PER_SECTOR; i++) {

//        // Determine file type and get the flash offset
//        file_type = get_file_type(&pDirEnts[i], &offset);

//        if (file_type == BIN_FILE || file_type == PAR_FILE ||
//            file_type == DOW_FILE || file_type == CRD_FILE || file_type == SPI_FILE) {

//            hidden_file = (pDirEnts[i].attributes & 0x02) ? 1 : 0;

//            // compute the size of the file
//            size = pDirEnts[i].filesize;

//            if (size == 0) {
//              // skip empty files
//                continue;
//            }

//            // read the cluster number where data are stored (ignoring the
//            // two high bytes in the cluster number)
//            //
//            // Convert cluster number to sector number by moving past the root
//            // dir and fat tables.
//            //
//            // The cluster numbers start at 2 (0 and 1 are never used).
//            begin_sector = (pDirEnts[i].first_cluster_low_16 - 2) * WANTED_SECTORS_PER_CLUSTER + SECTORS_FIRST_FILE_IDX;

//            // compute the number of sectors
//            nb_sector = (size + MBR_BYTES_PER_SECTOR - 1) / MBR_BYTES_PER_SECTOR;

//            if ( (pDirEnts[i].filename[0] == '_') ||
//                 (pDirEnts[i].filename[0] == '.') ||
//                 (hidden_file && ((pDirEnts[i].filename[0] == '_') || (pDirEnts[i].filename[0] == '.'))) ||
//                 ((pDirEnts[i].filename[0] == 0xE5) && (file_type != CRD_FILE) && (file_type != PAR_FILE))) {
//                if (theoretical_start_sector == begin_sector) {
//                    adapt_th_sector = 1;
//                }
//                size = 0;
//                nb_sector = 0;
//                continue;
//            }

//            // if we receive a file with a valid extension
//            // but there has been program page error previously
//            // we fail / disconnect usb
//            if ((program_page_error == 1) && (maybe_erase == 0) && (start_sector >= begin_sector)) {
//                reason = RESERVED_BITS;
//                initDisconnect(0);
//                return -1;
//            }

//            adapt_th_sector = 0;

//            // on mac, with safari, we receive all the files with some more sectors at the beginning
//            // we have to move the sectors... -> 2x slower
//            if ((start_sector != 0) && (start_sector < begin_sector) && (current_sector - (begin_sector - start_sector) >= nb_sector)) {

//                // we need to copy all the sectors
//                // we don't listen to msd interrupt
//                listen_msc_isr = 0;

//                move_sector_start = (begin_sector - start_sector)*MBR_BYTES_PER_SECTOR;
//                nb_sector_to_move = (nb_sector % 2) ? nb_sector/2 + 1 : nb_sector/2;
//                for (i = 0; i < nb_sector_to_move; i++) {
//                    if (!swd_read_memory(move_sector_start + i*FLASH_SECTOR_SIZE, (uint8_t *)usb_buffer, FLASH_SECTOR_SIZE)) {
//                        failSWD();
//                        return -1;
//                    }
//                    if (!target_flash_erase_sector(i)) {
//                        failSWD();
//                        return -1;
//                    }
//                    if (!target_flash_program_page(i*FLASH_SECTOR_SIZE, (uint8_t *)usb_buffer, FLASH_SECTOR_SIZE)) {
//                        failSWD();
//                        return -1;
//                    }
//                }
//                initDisconnect(1);
//                return -1;
//            }

//            found = 1;
//            idx = i; // this is the file we want
//            good_file = 1;
//#if defined(BOARD_UBLOX_C027)
//            if (0 == memcmp((const char*)pDirEnts[i].filename, "~AUTORST", 8))
//                good_file = 2;
//            else if (0 == memcmp((const char*)pDirEnts[i].filename, "~AUTOCRP", 8))
//                good_file = 3;
//#endif
//            flash_addr_offset = offset;
//            break;
//        }
//        // if we receive a new file which does not have the good extension
//        // fail and disconnect usb
//        else if (file_type == UNSUP_FILE) {
//            reason = BAD_EXTENSION_FILE;
//            initDisconnect(0);
//            return -1;
//        }
//    }

//    if (adapt_th_sector) {
//        theoretical_start_sector += nb_sector;
//        init(0);
//    }
//    return (found == 1) ? idx : -1;
//}


//void usbd_msc_read_sect (uint32_t block, uint8_t *buf, uint32_t num_of_blocks) {
//    if (USBD_MSC_MediaReady) {
//        memcpy(buf, &DiskImage[block * USBD_MSC_BlockSize], num_of_blocks * USBD_MSC_BlockSize);
//    }
//    
////    if ((usb_state != USB_CONNECTED) || (listen_msc_isr == 0))
////        return;

////    if (USBD_MSC_MediaReady) {
////        // blink led not permanently
////        main_blink_msd_led(0);
////        memset(buf, 0, 512);

////        // Handle MBR, FAT1 sectors, FAT2 sectors, root1, root2 and mac file
////        if (block <= SECTORS_FIRST_FILE_IDX) {
////            memcpy(buf, sectors[block].sect, sectors[block].length);

////            // add new entry in FAT
////            if ((block == 1) && (drag_success == 0)) {
////                buf[9] = 0xff;
////                buf[10] = 0x0f;
////            } else if ((block == SECTORS_ROOT_IDX) && (drag_success == 0)) {
////                /* Appends a new directory entry at the end of the root file system.
////                    The entry is a copy of "fail[]" and the size is updated to match the
////                    length of the error reason string. The entry's set to point to cluster
////                    4 which is the first after the mbed.htm file."
////                */
////                memcpy(buf + sectors[block].length, fail, 16*2);
////                // adapt size of file according fail reason
////                buf[sectors[block].length + 28] = strlen((const char *)reason_array[reason]);
////                buf[sectors[block].length + 26] = 6;
////            }
////        }
//////        // send System Volume Information
//////        else if (block == SECTORS_SYSTEM_VOLUME_INFORMATION) {
//////            memcpy(buf, sect6, sizeof(sect6));
//////        }
//////        // send System Volume Information/IndexerVolumeGuid
//////        else if (block == SECTORS_INDEXER_VOLUME_GUID) {
//////            memcpy(buf, sect7, sizeof(sect7));
//////        }
////        // send mbed.html
////        else if (block == SECTORS_MBED_HTML_IDX) {
////            update_html_file();
////        }
////        // send error message file
////        else if (block == SECTORS_ERROR_FILE_IDX) {
////            memcpy(buf, reason_array[reason], strlen((const char *)reason_array[reason]));
////        }
////    }
//}

//static int programPage() {
//    //The timeout task's timer is resetted every 256kB that is flashed.
//    if ((flashPtr >= 0x40000) && ((flashPtr & 0x3ffff) == 0)) {
//        isr_evt_set(MSC_TIMEOUT_RESTART_EVENT, msc_valid_file_timeout_task_id);
//    }

//    // if we have received two sectors, write into flash
//    if (!target_flash_program_page(flashPtr + flash_addr_offset, (uint8_t *)usb_buffer, FLASH_PROGRAM_PAGE_SIZE)) {
//        // even if there is an error, adapt flashptr
//        flashPtr += FLASH_PROGRAM_PAGE_SIZE;
//        return 1;
//    }

//    // if we just wrote the last sector -> disconnect usb
//    if (current_sector == nb_sector) {
//        initDisconnect(1);
//        return 0;
//    }

//    flashPtr += FLASH_PROGRAM_PAGE_SIZE;

//    return 0;
//}


//void usbd_msc_write_sect (uint32_t block, uint8_t *buf, uint32_t num_of_blocks) {
//    if (USBD_MSC_MediaReady) {
//        memcpy(&Memory[block * USBD_MSC_BlockSize], buf, num_of_blocks * USBD_MSC_BlockSize);
//    }    
////    int idx_size = 0;

////    if ((usb_state != USB_CONNECTED) || (listen_msc_isr == 0))
////        return;

////    // we recieve the root directory
////    if ((block == SECTORS_ROOT_IDX) || (block == (SECTORS_ROOT_IDX+1))) {
////        // try to find a .bin file in the root directory
////        idx_size = search_bin_file(buf, block);

////        // .bin file exists
////        if (idx_size != -1) {

////            if (sector_received_first == 0) {
////                root_dir_received_first = 1;
////            }

////            // this means that we have received the sectors before root dir (linux)
////            // we have to flush the last page into the target flash
////            if ((sector_received_first == 1) && (current_sector == nb_sector) && (jtag_flash_init == 1)) {
////                if (msc_event_timeout == 0) {
////                    msc_event_timeout = 1;
////                    isr_evt_set(MSC_TIMEOUT_SPLIT_FILES_EVENT, msc_valid_file_timeout_task_id);
////                }
////                return;
////            }

////            // means that we are receiving additional sectors
////            // at the end of the file ===> we ignore them
////            if ((sector_received_first == 1) && (start_sector == begin_sector) && (current_sector > nb_sector) && (jtag_flash_init == 1)) {
////                initDisconnect(1);
////                return;
////            }
////        }
////    }
////    if (block >= SECTORS_ERROR_FILE_IDX) {

////        main_usb_busy_event();

////        if (root_dir_received_first == 0) {
////            sector_received_first = 1;
////        }

////        // if we don't receive consecutive sectors
////        // set maybe erase in case we receive other sectors
////        if ((previous_sector != 0) && ((previous_sector + 1) != block)) {
////            maybe_erase = 1;
////            return;
////        }

////        if (!flash_started && (block > theoretical_start_sector)) {
////            theoretical_start_sector = block;
////        }

////        // init jtag if needed
////        if (jtag_init() == 1) {
////            return;
////        }

////        if (jtag_flash_init == 1) {

////            main_blink_msd_led(1);

////            // We erase the chip if we received unrelated data before (mac compatibility)
////            if (maybe_erase && (block == theoretical_start_sector)) {
////                // avoid erasing the internal flash if only the external flash will be updated
////                if (flash_addr_offset == 0) {
////                    if (!target_flash_erase_chip()) {
////                    return;
////                    }
////                }
////                maybe_erase = 0;
////                program_page_error = 0;
////            }

////            // drop block < theoretical_sector
////            if (theoretical_start_sector > block) {
////                return;
////            }

////            if ((flash_started == 0) && (theoretical_start_sector == block)) {
////                flash_started = 1;
////                isr_evt_set(MSC_TIMEOUT_START_EVENT, msc_valid_file_timeout_task_id);
////                start_sector = block;
////            }

////            // at the beginning, we need theoretical_start_sector == block
////            if ((flash_started == 0) && (theoretical_start_sector != block)) {
////                reason = BAD_START_SECTOR;
////                initDisconnect(0);
////                return;
////            }

////            // not consecutive sectors detected
////            if ((flash_started == 1) && (maybe_erase == 0) && (start_sector != block) && (block != (start_sector + current_sector))) {
////                reason = NOT_CONSECUTIVE_SECTORS;
////                initDisconnect(0);
////                return;
////            }

////            // if we receive a new sector
////            // and the msc thread has been started (we thought that the file has been fully received)
////            // we kill the thread and write the sector in flash
////            if (msc_event_timeout == 1) {
////                msc_event_timeout = 0;
////            }

////            if (flash_started && (block == theoretical_start_sector)) {
////                // avoid erasing the internal flash if only the external flash will be updated
////                if (flash_addr_offset == 0) {
////                    if (!target_flash_erase_chip()) {
////                    return;
////                    }
////                }
////                maybe_erase = 0;
////                program_page_error = 0;
////            }

////            previous_sector = block;
////            current_sector++;
////            if (programPage() == 1) {
////                if (good_file) {
////                    reason = RESERVED_BITS;
////                    initDisconnect(0);
////                    return;
////                }
////                program_page_error = 1;
////                return;
////            }
////        }
////    }
//}


//void usbd_msc_init () {
//    USBD_MSC_MemorySize = 4096;
//    USBD_MSC_BlockSize  = 512;
//    USBD_MSC_BlockGroup = 1;
//    USBD_MSC_BlockCount = USBD_MSC_MemorySize / USBD_MSC_BlockSize;
//    USBD_MSC_BlockBuf   = BlockBuf;

//    //memcpy(Memory, DiskImage, 2048);
//    USBD_MSC_MediaReady = __TRUE;
////    if (!task_first_started) {
////        task_first_started = 1;
////        os_tsk_create_user(msc_valid_file_timeout_task, MSC_TASK_PRIORITY, msc_task_stack, MSC_TASK_STACK);
////    }

////    USBD_MSC_MemorySize = MBR_NUM_NEEDED_SECTORS * MBR_BYTES_PER_SECTOR;
////    USBD_MSC_BlockSize  = 512;
////    USBD_MSC_BlockGroup = 1;
////    USBD_MSC_BlockCount = USBD_MSC_MemorySize / USBD_MSC_BlockSize;
////    USBD_MSC_BlockBuf   = (uint8_t *)usb_buffer;
////    USBD_MSC_MediaReady = __TRUE;
//}
