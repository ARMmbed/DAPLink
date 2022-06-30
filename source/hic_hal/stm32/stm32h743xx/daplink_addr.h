/**
 * @file    daplink_addr.h
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

#ifndef DAPLINK_ADDR_H
#define DAPLINK_ADDR_H

/* Device sizes */

#define DAPLINK_ROM_START               0x08000000
#define DAPLINK_ROM_SIZE                0x00100000 // has to be a multiple of the sector size, only bank 1 is used, the datasheet says under table 7 in the notes section that the memory boundary is limited to this on each bank

#define DAPLINK_RAM_START               0x24000000 // Execute from AXI SRAM, because it's powered on by default and the app is quite large due to the sector_buf static variable that is used to hold one sector (128Kb)
#define DAPLINK_RAM_SIZE                0x00080000 // divisible by DAPLINK_MIN_WRITE_SIZE

/* Flash Programming Info */

#define DAPLINK_SECTOR_SIZE             0x00020000 // this chip has a large 128Kb sector erase
#define DAPLINK_MIN_WRITE_SIZE          0x00000400

/* ROM sizes */

#define DAPLINK_ROM_BL_START            0x08000000
#define DAPLINK_ROM_BL_SIZE             0x00020000 // 128 kB bootloader -- 1 sector

#define DAPLINK_ROM_CONFIG_ADMIN_START  0x08020000
#define DAPLINK_ROM_CONFIG_ADMIN_SIZE   0x00020000 // 1 sector

#define DAPLINK_ROM_IF_START            0x08040000
#define DAPLINK_ROM_IF_SIZE             0x000A0000 // 5 sectors

#define DAPLINK_ROM_CONFIG_USER_START   0x080E0000
#define DAPLINK_ROM_CONFIG_USER_SIZE    0x00020000 // 1 sector

/* RAM sizes */

#define DAPLINK_RAM_APP_START           DAPLINK_RAM_START
#define DAPLINK_RAM_APP_SIZE            (DAPLINK_RAM_SIZE - DAPLINK_MIN_WRITE_SIZE) // leave space for RAM_SHARED

#define DAPLINK_RAM_SHARED_START        (DAPLINK_RAM_APP_START + DAPLINK_RAM_APP_SIZE)
#define DAPLINK_RAM_SHARED_SIZE         (DAPLINK_RAM_SIZE - DAPLINK_RAM_APP_SIZE) // fill the rest of RAM

/* Current build */

#if defined(DAPLINK_BL)

#define DAPLINK_ROM_APP_START            DAPLINK_ROM_BL_START
#define DAPLINK_ROM_APP_SIZE             DAPLINK_ROM_BL_SIZE
#define DAPLINK_ROM_UPDATE_START         DAPLINK_ROM_IF_START
#define DAPLINK_ROM_UPDATE_SIZE          DAPLINK_ROM_IF_SIZE

#elif defined(DAPLINK_IF)

#define DAPLINK_ROM_APP_START            DAPLINK_ROM_IF_START
#define DAPLINK_ROM_APP_SIZE             DAPLINK_ROM_IF_SIZE
#define DAPLINK_ROM_UPDATE_START         DAPLINK_ROM_BL_START
#define DAPLINK_ROM_UPDATE_SIZE          (DAPLINK_ROM_BL_SIZE + DAPLINK_ROM_CONFIG_ADMIN_SIZE)

#else

#error "Build must be either bootloader or interface"

#endif

#endif
