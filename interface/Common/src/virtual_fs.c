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
 
#include "virtual_fs.h"
#include "string.h"
#include "version.h"
#include "version_git.h"

// mbr is in RAM so the members can be updated at runtime to change drive capacity based
//  on target MCU that is attached
mbr_t mbr = {
    /*uint8_t[11]*/.boot_sector = {
        0xEB,0x3C, 0x90,
        'M','S','D','0','S','4','.','1' // OEM Name in text (8 chars max)
    },
    /*uint16_t*/.bytes_per_sector           = 0x0200,       // 512 bytes per sector
    /*uint8_t */.sectors_per_cluster        = 0x08,         // 4k cluser
    /*uint16_t*/.reserved_logical_sectors   = 0x0001,       // mbr is 1 sector
    /*uint8_t */.num_fats                   = 0x02,         // 2 FATs
    /*uint16_t*/.max_root_dir_entries       = 0x0020,       // 16 dir entries (max)
    /*uint16_t*/.total_logical_sectors      = 0x1f50,       // sector size * # of sectors = drive size
    /*uint8_t */.media_descriptor           = 0xf8,         // fixed disc = F8, removable = F0
    /*uint16_t*/.logical_sectors_per_fat    = 0x0001,       // FAT is 1k - ToDO:need to edit this
    /*uint16_t*/.physical_sectors_per_track = 0x0001,       // flat
    /*uint16_t*/.heads                      = 0x0001,       // flat
    /*uint32_t*/.hidden_sectors             = 0x00000000,   // before mbt, 0
    /*uint32_t*/.big_sectors_on_drive       = 0x00000000,   // 4k sector. not using large clusters
    /*uint8_t */.physical_drive_number      = 0x00,
    /*uint8_t */.not_used                   = 0x00,         // Current head. Linux tries to set this to 0x1
    /*uint8_t */.boot_record_signature      = 0x29,         // signature is present
    /*uint32_t*/.volume_id                  = 0x27021974,   // serial number
    // needs to match the root dir label
    /*char[11]*/.volume_label               = {'M','B','E','D',' ',' ',' ',' ','D','N','D'},
    // unused by msft - just a label (FAT, FAT12, FAT16)
    /*char[8] */.file_system_type           = {'F','A','T','1','2',' ',' ',' '},

    /* Executable boot code that starts the operating system */
    /*uint8_t[448]*/.bootstrap = {
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
    /*uint16_t*/.signature = 0xAA55,
};

// FAT is only valid for files on disc that are part of this file. Not writeable during
//  MSC transfer operations
static const file_allocation_table_t fat = {
    .f = {
        0xF8, 0xFF, 
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xFF, 0xFF,
        0x00, 0x00,
    }
};

// Tool for release awareness and tracking
#if (GIT_LOCAL_MODS == 1)
  #warning "Building with local modifications."
  #define GIT_LOCAL_MODS_STR "Yes"
#else
  #define GIT_LOCAL_MODS_STR "No"
#endif

static const uint8_t mbed_redirect_file[512] =
    "<!-- mbed Platform Website and Authentication Shortcut -->\r\n"
    "<html>\r\n"
    "<head>\r\n"
    "<meta http-equiv=\"refresh\" content=\"0; url=http://mbed.org/device/?code=@A\"/>\r\n"
    "<title>mbed Website Shortcut</title>\r\n"
    "</head>\r\n"
    "<body></body>\r\n"
    "</html>\r\n"
    "\r\n";

static const uint8_t details_file[512] =
    "Version: " FW_BUILD "\r\n"
    "Build:   " __DATE__ " " __TIME__ "\r\n"
    "Git Commit SHA: " GIT_COMMIT_SHA "\r\n"
    "Git Local mods: " GIT_LOCAL_MODS_STR "\r\n";

static const uint8_t fail_file[512] =
    "Placeholder for fail.txt data\r\n";

static FatDirectoryEntry_t const fail_txt_dir_entry = {
/*uint8_t[11] */ .filename = "FAIL    TXT",
/*uint8_t */ .attributes = 0x01,
/*uint8_t */ .reserved = 0x00,
/*uint8_t */ .creation_time_ms = 0x00,
/*uint16_t*/ .creation_time = 0x0000,
/*uint16_t*/ .creation_date = 0x0021,
/*uint16_t*/ .accessed_date = 0xbb32,
/*uint16_t*/ .first_cluster_high_16 = 0x0000,
/*uint16_t*/ .modification_time = 0x83dc,
/*uint16_t*/ .modification_date = 0x34bb,
/*uint16_t*/ .first_cluster_low_16 = 0x0004,    // always must be last sector
/*uint32_t*/ .filesize = sizeof(fail_file)
};

static FatDirectoryEntry_t const empty_dir_entry = {
/*uint8_t[11] */ .filename = {0},
/*uint8_t */ .attributes = 0x00,
/*uint8_t */ .reserved = 0x00,
/*uint8_t */ .creation_time_ms = 0x00,
/*uint16_t*/ .creation_time = 0x0000,
/*uint16_t*/ .creation_date = 0x0000,
/*uint16_t*/ .accessed_date = 0x0000,
/*uint16_t*/ .first_cluster_high_16 = 0x0000,
/*uint16_t*/ .modification_time = 0x0000,
/*uint16_t*/ .modification_date = 0x0000,
/*uint16_t*/ .first_cluster_low_16 = 0x0000,
/*uint32_t*/ .filesize = 0x0000
};


// Root directory in RAM to allow run-time modifications to contents. All files on disc must 
//  be accounted for in dir1
static root_dir_t dir1 = {
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
    .f1  = {
    /*uint8_t[11] */ .filename = "MBED    HTM",
    /*uint8_t */ .attributes = 0x01,
    /*uint8_t */ .reserved = 0x00,
    /*uint8_t */ .creation_time_ms = 0x00,
    /*uint16_t*/ .creation_time = 0x0000,
    /*uint16_t*/ .creation_date = 0x0021,
    /*uint16_t*/ .accessed_date = 0xbb32,
    /*uint16_t*/ .first_cluster_high_16 = 0x0000,
    /*uint16_t*/ .modification_time = 0x83dc,
    /*uint16_t*/ .modification_date = 0x34bb,
    /*uint16_t*/ .first_cluster_low_16 = 0x0002,
    /*uint32_t*/ .filesize = sizeof(mbed_redirect_file)
    },
    .f2 = {
    /*uint8_t[11] */ .filename = "DETAILS TXT",
    /*uint8_t */ .attributes = 0x01,
    /*uint8_t */ .reserved = 0x00,
    /*uint8_t */ .creation_time_ms = 0x00,
    /*uint16_t*/ .creation_time = 0x0000,
    /*uint16_t*/ .creation_date = 0x0021,
    /*uint16_t*/ .accessed_date = 0xbb32,
    /*uint16_t*/ .first_cluster_high_16 = 0x0000,
    /*uint16_t*/ .modification_time = 0x83dc,
    /*uint16_t*/ .modification_date = 0x30bb,
    /*uint16_t*/ .first_cluster_low_16 = 0x0003,
    /*uint32_t*/ .filesize = sizeof(details_file)
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
    .f15 = {0}
};

// dir2 is for hidden file and folder compatibility during OS writes (extended entry compatibility)
static const root_dir_t dir2 = {
    .dir = {0},
    .f1  = {0},
    .f2  = {0},
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
    .f15 = {0}
};

// dummy blank reigon used to padd the space between files
static const uint8_t blank_reigon[512] = {0};

// this is the composite file system. It's looked at by ready operations to determine what to send
//  when the host OS requests data
virtual_media_t fs[] = {
    // fs setup
    {(uint8_t *)&mbr, sizeof(mbr)},
    {(uint8_t *)&fat, sizeof(fat)},
    {(uint8_t *)&fat, sizeof(fat)},
    
    // root dir
    {(uint8_t *)&dir1, sizeof(dir1)},
    {(uint8_t *)&dir2, sizeof(dir2)},
    
    //start of file contents
    {(uint8_t *)&mbed_redirect_file, sizeof(mbed_redirect_file)},    
    //empty area between every file (8*512 is start of data reigion need to pad between files - 1
    {(uint8_t *)&blank_reigon, sizeof(blank_reigon)},
    {(uint8_t *)&details_file, sizeof(details_file)},
    {(uint8_t *)&blank_reigon, sizeof(blank_reigon)},
    {(uint8_t *)&fail_file,    sizeof(fail_file)},
    
    // add other meaningful file data entries here
    
    // end of fs data
    {(uint8_t *)0, 0},
};

// when a fail condition occurs we need to update the data stored on disc and also
//  the directory entry. fs[] entry and dir entry need to be looked at and may need
//  modification if/when more files are added to the file-system
void configure_fail_txt(target_flash_status_t reason)
{
    // set the dir entry to a file or empty it
    dir1.f3 = (TARGET_OK == reason) ? empty_dir_entry : fail_txt_dir_entry;
    // update the filesize (pass/fail)
    dir1.f3.filesize = strlen((const char *)fail_txt_contents[reason]);
    // and the memory that we point to (file contents)
    fs[9].sect = (uint8_t *)fail_txt_contents[reason];
}

// Update known entries and mbr data when the program boots
void virtual_fs_init(void)
{
    // 64KB is mbr, FATs, root dir, ect...
//    uint32_t wanted_size_in_bytes   = (target_device.disc_size + KB(64);
//    uint32_t number_sectors_needed  = (wanted_size_in_bytes / mbr.bytes_per_sector);
//    uint32_t number_clusters_needed = (number_sectors_needed / mbr.sectors_per_cluster);
//    uint32_t fat_sector_size =        (((number_clusters_needed / 1023) / 1024) * 3);
    // number of sectors = (media size in bytes) / bytes per sector
    mbr.total_logical_sectors = ((target_device.disc_size + kB(64)) / mbr.bytes_per_sector);
    // number of cluster = ((number of sectors) / sectors per cluster)
    // secotrs per fat   = (3 x ((number of clusters + 1023) / 1024))
    mbr.logical_sectors_per_fat = (3 * (((mbr.total_logical_sectors / mbr.sectors_per_cluster) + 1023) / 1024));
    // patch root direcotry entries
    //dir1.f1.filesize = strlen((const char *)mbed_redirect_file);
    dir1.f2.filesize = strlen((const char *)details_file);
    // patch fs entries (fat sizes and all blank regions)
    fs[1].length = sizeof(fat) * mbr.logical_sectors_per_fat;
    fs[2].length = fs[1].length;
    fs[6].length = sizeof(blank_reigon)*(mbr.sectors_per_cluster - 1);
    fs[8].length = sizeof(blank_reigon)*(mbr.sectors_per_cluster - 1);
}
