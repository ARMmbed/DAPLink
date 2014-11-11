
#ifndef VIRTUAL_FS_H
#define VIRTUAL_FS_H

// these are done at runtime and will be part of the config struct
#define WANTED_SIZE_IN_BYTES 1024*16384
#define WANTED_SECTORS_PER_CLUSTER  (8) //mbr.sectors_per_cluster
#define MBR_BYTES_PER_SECTOR 512
#define NUM_NEEDED_SECTORS  (WANTED_SIZE_IN_BYTES / MBR_BYTES_PER_SECTOR)   //flashsize + mbr + fat*2 + root dir / mbr.bytes_per_sector
#define NUM_NEEDED_CLUSTERS (NUM_NEEDED_SECTORS / WANTED_SECTORS_PER_CLUSTER)
///* Need 3 sectors/FAT for every 1024 clusters */
#define SECTORS_PER_FAT     (3*((NUM_NEEDED_CLUSTERS + 1023)/1024))

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

__packed typedef struct {
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
//    uint8_t  partition_one[16];
//    uint8_t  partition_two[16];
//    uint8_t  partition_three[16];
//    uint8_t  partition_four[16];
    /* Mandatory value at bytes 510-511, must be 0xaa55 */
    uint16_t signature;
} mbr_t;

extern mbr_t mbr;

typedef struct file_allocation_table {
    uint8_t f[512];
} file_allocation_table_t;

__packed typedef union FatDirectoryEntry {
    uint8_t data[32];
    __packed struct {
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
    };
} FatDirectoryEntry_t;

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

typedef struct fs_items {
    uint8_t *sect;
    uint32_t length;
} fs_items_t;

extern fs_items_t fs[];

void virtual_fs_init(void);

#ifdef __cplusplus
}
#endif

#endif
