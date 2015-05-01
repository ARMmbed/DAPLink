#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

#define FLASH_SECTOR_SIZE           (1024)  /* 1024 is assuming that this value is in number of uint32_t's */

#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (0x1000)

#define MIN_FLASH_ADDRESS 0x00000
#define MAX_FLASH_ADDRESS 0x10000

#define MIN_RAM_ADDRESS 0x20000000
#define MAX_RAM_ADDRESS 0x20030000


#endif
