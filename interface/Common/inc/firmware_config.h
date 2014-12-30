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
 
#ifndef FIRMWARE_CONFIG_H
#define FIRMWARE_CONFIG_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define KB(x)   (x*1024)
#define MB(x)   (KB(x)*1024)

/**
  @addtogroup
  @{
 */

/**
 @struct fw_cfg_t
 @brief  The firmware configuration struct has unique about the chip its running on.
 */ 
typedef  struct fw_cfg_t fw_cfg_t;
struct __attribute__ ((__packed__)) fw_cfg_t {
    uint8_t  board_id[4];    /*!< A unique identifier for the bootloader or OpenLINK application */
    uint8_t  secret[8];      /*!< Mangaged by mbed.org To request a BOARD_SECRET email support@mbed.org Do not commit this */
    uint32_t sector_size;    /*!< Number of bytes in a sector used by flash erase and write and usb_buffer sizing */
    uint32_t sector_cnt;     /*!< Number of sectors a device has */
    uint32_t flash_start;    /*!< Address of the application entry point */
    uint32_t flash_end;      /*!< Address where the flash ends */
    uint32_t ram_start;      /*!< Lowest contigous RAM address the application uses */
    uint32_t ram_end;        /*!< Highest contigous RAM address the application uses */
};

extern fw_cfg_t const app;

#ifdef __cplusplus
}
#endif

// dirty hack until virtual_fs is re-written to not use macros...
#if defined(BOOTLOADER) && defined(K20DX128)
// TARGET_MK20DX,  K20DX128,  CPU_MK20DX128VFM5,  __RTX,  BOOTLOADER, 
#include "MK20D5.h"
#define FLASH_SIZE_KB   (128-32)
#define INITIAL_SP      (*(uint32_t *)(APP_START_ADR))
#define RESET_HANDLER   (*(uint32_t *)(APP_START_ADR + 4))
#endif

#if defined (DBG_KL25Z)
// TARGET_MK20D5,  __RTX,  OFFSET_VTABLE_32K,  DBG_KL25Z,  BOARD_FRDM_KL25Z,  INTERFACE_GEN_32KHZ,  OPENSDA_BOOTLOADER, 
#define FLASH_SIZE_KB   128
// L ID is 0x001c0020
// K ID is 0x001c0000
#endif

/**
 @}
 */

#endif
