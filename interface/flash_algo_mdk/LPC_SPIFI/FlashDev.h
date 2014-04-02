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
#ifndef FLASHDEV_H
#define FLASHDEV_H

// SPIFI Interfaces
#if defined(LPC1800_SPIFI)
  // LPC18xx/LPC43xx SPIFI devices start at 0x14000000 or 0x80000000
  #define FLASH_START             0x14000000
  // Buffer to save sector contents on partial sector writes
  // Up to 72K on LPC4330/LPC4350, 40K on other LPC18xx/LPC43xx
  #define SECTOR_BUF              ((char *)0x10080000)
#else
  #error "Interface not implemented"
#endif

// SPIFI Devices
// Use default for mbed HDK - Geometry defined in flash programmer
#if defined(SPIFI_4M_64K)
  // SPIFI_4M_64K - 4MB flash, 64 x 64KB sectors (Spansion S25FL032P or equiv)
  #warning "SPIFI driver requires 64KB buffer"
  #define FLASH_DEV_NAME          "LPC18xx/43xx SPIFI 4M/64K"
  #define FLASH_DEV_SIZE          (4 * 1024 * 1024) // 4 MB
  #define FLASH_SECTOR_SIZE       (64 * 1024)
#elif defined(SPIFI_8M_4K)
  // SPIFI_8M_4K - 8MB flash, 2048 x 4KB sectors (Winbond W25Q64FV or equiv)
  #define FLASH_DEV_NAME          "LPC18xx/43xx SPIFI 8M/4K"
  #define FLASH_DEV_SIZE          (8 * 1024 * 1024) // 8 MB
  #define FLASH_SECTOR_SIZE       (4 * 1024)
#else // default
  // SPIFI_4M_4K - 4MB flash, 1024 x 4KB sectors (Winbond W25Q32FV or equiv)
  #define FLASH_DEV_NAME          "LPC18xx/43xx SPIFI 4M/4K"
  #define FLASH_DEV_SIZE          (4 * 1024 * 1024) // 4 MB
  #define FLASH_SECTOR_SIZE       (4 * 1024)
#endif  

#endif
