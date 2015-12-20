#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

// SPIFI flash sectors on current devices are 4K to 64K. Use a smaller sector
// size for portability and to fit within the max LPC11U35 USB buffer (2K)
#define FLASH_SECTOR_SIZE           (2048)

#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (4096)


#define MIN_FLASH_ADDRESS 0x00000
#define MAX_FLASH_ADDRESS 0x10000

#define MIN_RAM_ADDRESS 0x20000000
#define MAX_RAM_ADDRESS 0x20030000


#endif
