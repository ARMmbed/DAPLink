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
 
#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

#include <stddef.h>
#include "stdint.h"

#ifdef __cplusplus
  extern "C" {
#endif

/**
 @addtogroup
 @{
*/
    
#define kB(x)   (x*1024)
#define MB(x)   (kB(x)*1024)

// Not sure what this is suppose to mean. used in swd_host and needs to be looked further into
#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (4096)

// 'kvld' in hex - key valid
#define CFG_KEY             0x6b766c64
#define CFG_VALID(dev)      (NULL != (dev).cfg && CfG_KEY == (dev).cfg->key)
// Size of the structure not including the member 'end'.  This define is used
// instead of sizeof() so the exact size can be obtained, rather than
// the 4 byte aligned size that sizeof() returns.
#define CFG_SIZE_CURRENT    (offsetof(cfg_setting_t, end))

// WARNING - THIS STRUCTURE RESIDES IN NON-VOLATILE STORAGE! 
// Be careful with changes:
// -Only add new members to end end of this structure
// -Do not change the order of members in this structure
typedef struct cfg_setting {
    uint32_t key;               // Magic key to indicate a valid record
    uint16_t offset_of_end;     // Offset of the last member from the start

    // Configurable values
    uint8_t auto_rst;
    
    // Add new members here
    
    uint8_t end; // Must be last member
} cfg_setting_t;

/**
 @struct target_cfg_t
 @brief  The firmware configuration struct has unique about the chip its running on.
 */ 
typedef struct target_cfg {
    uint8_t  board_id[4];       /*!< A unique identifier for the bootloader or DAPLink application */
    uint8_t  secret[8];         /*!< Mangaged by mbed.org To request a secret email support@mbed.org */
    uint32_t sector_size;       /*!< Number of bytes in a sector used by flash erase and filesystem */
    uint32_t sector_cnt;        /*!< Number of sectors a device has */
    uint32_t flash_start;       /*!< Address of the application entry point */
    uint32_t flash_end;         /*!< Address where the flash ends */
    uint32_t ram_start;         /*!< Lowest contigous RAM address the application uses */
    uint32_t ram_end;           /*!< Highest contigous RAM address the application uses */
    uint32_t disc_size;         /*!< Size of USB disc (largest known application file) */
    const char *url;            /*!< The URL that the shortcut on disc should direct to */
    uint8_t url_name[11];       /*!< Name of the .htm redirect file on disc */
    uint8_t drive_name[11];     /*!< Name of the MSC drive that */
    const cfg_setting_t *cfg;   /*!< A structure of data used to configure behaviour */
} target_cfg_t;

extern const target_cfg_t target_device;

#ifdef __cplusplus
  }
#endif

#endif
