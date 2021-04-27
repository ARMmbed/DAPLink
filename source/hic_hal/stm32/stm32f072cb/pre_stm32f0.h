/**
 * @file    pre_stm32f.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
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

#ifndef _PRE_STM32F0_H
#define _PRE_STM32F0_H

//#include "macro.h"

#define STM32F0_ROM_START               0x08000000
#define STM32F0_RAM_START               0x20000000
 
#define STM32F0_ROM_SIZE                0x00020000  // 128 Kb
#define STM32F0_RAM_SIZE                0x00004000  //  16 Kb

#if (defined(STM32F072xB))
#define STM32F0_SECTOR_SIZE             0x800
#endif /* STM32F072xB */


#endif /* _PRE_STM32F0_H */
