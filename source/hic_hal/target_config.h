/**
 * @file    target_config.h
 * @brief   
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

#include "stddef.h"
#include "stdint.h"
 
#include "flash_blob.h"
#include "macro.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 @addtogroup
 @{
*/

// This can vary from target to target and should be in the structure or flash blob
#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (1024)

/**
 @struct target_cfg_t
 @brief  The firmware configuration struct has unique about the chip its running on.
 */
typedef struct target_cfg {
    uint32_t sector_size;           /*!< Number of bytes in a sector used by flash erase and filesystem */
    uint32_t sector_cnt;            /*!< Number of sectors a device has */
    uint32_t flash_start;           /*!< Address of the application entry point */
    uint32_t flash_end;             /*!< Address where the flash ends */
    uint32_t ram_start;             /*!< Lowest contigous RAM address the application uses */
    uint32_t ram_end;               /*!< Highest contigous RAM address the application uses */
    program_target_t *flash_algo;   /*!< A pointer to the flash algorithm structure */
    uint8_t erase_reset;            /*!< Reset after performing an erase */
} target_cfg_t;

extern const target_cfg_t target_device;

#ifdef __cplusplus
}
#endif

#endif
