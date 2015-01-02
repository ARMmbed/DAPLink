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

#include "target_config.h"
#include "target_flash.h"

#ifdef __cplusplus
  extern "C" {
#endif

typedef struct {
    uint8_t boot_sector[11];
    /* DOS 2.0 BPB - Bios Parameter Block, 11 bytes */
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_logical_sectors;
    uint8_t  num_fats;
    uint16_t max_root_dir_entries;
    uint16_t total_logical_sectors;
    uint8_t  media_descriptor;
    uint16_t logical_sectors_per_fat;
    /* DOS 3.31 BPB - Bios Parameter Block, 12 bytes */
    uint16_t physical_sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t big_sectors_on_drive;
    /* Extended BIOS Parameter Block, 26 bytes */
    uint8_t  physical_drive_number;
    uint8_t  not_used;
    uint8_t  boot_record_signature;
    uint32_t volume_id;
    char     volume_label[11];
    char     file_system_type[8];
    /* bootstrap data in bytes 62-509 */
    uint8_t  bootstrap[448];
    /* These entries in place of bootstrap code are the *nix partitions */
    //uint8_t  partition_one[16];
    //uint8_t  partition_two[16];
    //uint8_t  partition_three[16];
    //uint8_t  partition_four[16];
    /* Mandatory value at bytes 510-511, must be 0xaa55 */
    uint16_t signature;
} __attribute__((packed)) mbr_t;

// cannot exceed 512 conseutive bytes or media read logic fails
extern mbr_t mbr;

typedef struct file_allocation_table {
    uint8_t f[512];
} file_allocation_table_t;

typedef union FatDirectoryEntry {
    uint8_t data[32];
    struct {
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
    } __attribute__((packed)) ;
} __attribute__((packed)) FatDirectoryEntry_t;

// to save RAM all files must be in the first root dir entry (512 bytes)
//  but 2 actually exist on disc (32 entries) to accomodate hidden OS files,
//  folders and metadata 
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

typedef struct virtual_media {
    uint8_t *sect;
    uint32_t length;
} virtual_media_t;

extern virtual_media_t fs[];

void configure_fail_txt(target_flash_status_t reason);
void virtual_fs_init(void);

#ifdef __cplusplus
}
#endif

#endif
