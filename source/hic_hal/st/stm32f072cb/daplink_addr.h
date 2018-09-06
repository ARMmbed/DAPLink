/**
 * @file    daplink_addr.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2018, ARM Limited, All Rights Reserved
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

#include "pre_stm32f0.h"

#define DAPLINK_ROM_START               STM32F0_ROM_START
#define DAPLINK_ROM_SIZE                STM32F0_ROM_SIZE

#define DAPLINK_RAM_START               STM32F0_RAM_START
#define DAPLINK_RAM_SIZE                STM32F0_RAM_SIZE

/* ROM sizes */
/* stm32f072 can use STDFU */
#define DAPLINK_ROM_BL_SIZE             0x00000000
#define DAPLINK_ROM_CONFIG_ADMIN_SIZE   0x00000000
#define DAPLINK_ROM_CONFIG_USER_SIZE    0x00000800

#define DAPLINK_ROM_IF_SIZE             (DAPLINK_ROM_SIZE - \
                                         DAPLINK_ROM_BL_SIZE - \
                                         DAPLINK_ROM_CONFIG_ADMIN_SIZE - \
                                         DAPLINK_ROM_CONFIG_USER_SIZE)

#define DAPLINK_ROM_BL_START            DAPLINK_ROM_START
#define DAPLINK_ROM_CONFIG_ADMIN_START  (DAPLINK_ROM_BL_START + DAPLINK_ROM_BL_SIZE)
#define DAPLINK_ROM_IF_START            (DAPLINK_ROM_CONFIG_ADMIN_START + DAPLINK_ROM_CONFIG_ADMIN_SIZE)
#define DAPLINK_ROM_CONFIG_USER_START   (DAPLINK_ROM_IF_START + DAPLINK_ROM_IF_SIZE)

/* RAM sizes */
#define DAPLINK_RAM_APP_START           DAPLINK_RAM_START
#define DAPLINK_RAM_SHARED_START        (DAPLINK_RAM_APP_START + DAPLINK_RAM_APP_SIZE)

#define DAPLINK_RAM_APP_SIZE            (DAPLINK_RAM_SIZE - DAPLINK_RAM_SHARED_SIZE)
#define DAPLINK_RAM_SHARED_SIZE         0x00000100

/* Flash Programming Info */
#define DAPLINK_SECTOR_SIZE             STM32F0_SECTOR_SIZE
#define DAPLINK_MIN_WRITE_SIZE          2

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
