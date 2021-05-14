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
#define DAPLINK_ROM_SIZE                0x00100000 // STM32F373CC Max Flash Memory Size - 1MB // 256

#define DAPLINK_RAM_START               0x20000000
#define DAPLINK_RAM_SIZE                0x00040000 // STM32F373CC Max RAM Memory Size - 256KB // 32

/* ROM sizes */
#define DAPLINK_ROM_BL_START            0x08000000
#define DAPLINK_ROM_BL_SIZE             0x00000000 // Bootloader size set to zero since we are compiling only Interface firmware

#define DAPLINK_ROM_CONFIG_ADMIN_START  0x08000000
#define DAPLINK_ROM_CONFIG_ADMIN_SIZE   0x0000C000

#define DAPLINK_ROM_IF_START            0x0800C000 // Start address of Interface Firmware in Flash Memory
#define DAPLINK_ROM_IF_SIZE             0x000EF400 // Max Size of Interface Firmware 

#define DAPLINK_ROM_CONFIG_USER_START   0x080FB400 // Start address of Initialized User Data in Flash Memory
#define DAPLINK_ROM_CONFIG_USER_SIZE    0x00004C00 // Size of Initialized User Data in Flash Memory

/* RAM sizes */

#define DAPLINK_RAM_APP_START           0x20000000 // Start address of Initialized User Data loaded to RAM from Flash Memory at startup
#define DAPLINK_RAM_APP_SIZE            0x00004F00 // Size of Initialized User Data in RAM

#define DAPLINK_RAM_SHARED_START        0x20004F00 // Start address of Uninitialized RAM region
#define DAPLINK_RAM_SHARED_SIZE         0x0003B100 // Size of Uninitialized RAM region

/* Flash Programming Info */

#define DAPLINK_SECTOR_SIZE             0x00000400
#define DAPLINK_MIN_WRITE_SIZE          0x00000400

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
#define DAPLINK_ROM_UPDATE_SIZE          DAPLINK_ROM_BL_SIZE

#else

#error "Build must be either bootloader or interface"

#endif

#endif
