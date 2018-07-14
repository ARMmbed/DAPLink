/**
 * @file    pre_stm32f.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2018, Findlay Feng, All Rights Reserved
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

#ifndef STM32F1_ADDR_H
#define STM32F1_ADDR_H

#include "macro.h"

#define STM32F1_ROM_START               0x08000000
#define STM32F1_RAM_START               0x20000000

#define USE_HAL_DRIVER

#if defined(TARGET_STM32F102x4)
#define STM32F102x6
#define STM32F1_ROM_SIZE                KB(16)
#define STM32F1_RAM_SIZE                KB(4)
#elif defined(TARGET_STM32F102x6)
#define STM32F102x6
#define STM32F1_ROM_SIZE                KB(32)
#define STM32F1_RAM_SIZE                KB(6)
#elif defined(TARGET_STM32F102x8)
#define STM32F102xB
#define STM32F1_ROM_SIZE                KB(64)
#define STM32F1_RAM_SIZE                KB(10)
#elif defined(TARGET_STM32F102xB)
#define STM32F102xB
#define STM32F1_ROM_SIZE                KB(128)
#define STM32F1_RAM_SIZE                KB(16)
#elif defined(TARGET_STM32F103x4)
#define STM32F103x6
#define STM32F1_ROM_SIZE                KB(16)
#define STM32F1_RAM_SIZE                KB(4)
#elif defined(TARGET_STM32F103x6)
#define STM32F103x6
#define STM32F1_ROM_SIZE                KB(32)
#define STM32F1_RAM_SIZE                KB(10)
#elif defined(TARGET_STM32F103x8)
#define STM32F103xB
#define STM32F1_ROM_SIZE                KB(64)
#define STM32F1_RAM_SIZE                KB(20)
#elif defined(TARGET_STM32F103xB)
#define STM32F103xB
#define STM32F1_ROM_SIZE                KB(128)
#define STM32F1_RAM_SIZE                KB(20)
#elif defined(TARGET_STM32F103xC)
#define STM32F103xE
#define STM32F1_ROM_SIZE                KB(256)
#define STM32F1_RAM_SIZE                KB(64)
#elif defined(TARGET_STM32F103xD)
#define STM32F103xE
#define STM32F1_ROM_SIZE                KB(256)
#define STM32F1_RAM_SIZE                KB(64)
#elif defined(TARGET_STM32F103xE)
#define STM32F103xE
#define STM32F1_ROM_SIZE                KB(512)
#define STM32F1_RAM_SIZE                KB(64)
#elif defined(TARGET_STM32F103xF)
#define STM32F103xG
#define STM32F1_ROM_SIZE                KB(768)
#define STM32F1_RAM_SIZE                KB(96)
#elif defined(TARGET_STM32F103xG)
#define STM32F103xG
#define STM32F1_ROM_SIZE                MB(1)
#define STM32F1_RAM_SIZE                KB(96)
#elif defined(TARGET_STM32F105x8)
#define STM32F105xC
#define STM32F1_ROM_SIZE                KB(64)
#define STM32F1_RAM_SIZE                KB(64)
#elif defined(TARGET_STM32F105xB)
#define STM32F105xC
#define STM32F1_ROM_SIZE                KB(128)
#define STM32F1_RAM_SIZE                KB(64)
#elif defined(TARGET_STM32F105xC)
#define STM32F105xC
#define STM32F1_ROM_SIZE                KB(256)
#define STM32F1_RAM_SIZE                KB(64)
#elif defined(TARGET_STM32F107xB)
#define STM32F107xC
#define STM32F1_ROM_SIZE                KB(128)
#define STM32F1_RAM_SIZE                KB(64)
#elif defined(TARGET_STM32F107xC)
#define STM32F107xC
#define STM32F1_ROM_SIZE                KB(256)
#define STM32F1_RAM_SIZE                KB(64)
#else
#error "Please select first the target STM32F1xx device used in your application (in daplink_addr.h file)"
#endif

#if (defined(STM32F102x6) || defined(STM32F103x6) || defined(STM32F102xB) || defined(STM32F103xB))
#define STM32F1_SECTOR_SIZE             0x400
#endif /* STM32F102x6 || STM32F103x6 || STM32F102xB || STM32F103xB */

#if (defined(STM32F103xE) || defined(STM32F103xG) || defined(STM32F105xC) || defined(STM32F107xC))
#define STM32F1_SECTOR_SIZE             0x800
#endif /* STM32F102xB || STM32F103xB || STM32F103xG || STM32F105xC || STM32F107xC */

#endif
