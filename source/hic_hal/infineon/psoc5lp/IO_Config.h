/*******************************************************************************

* @file    IO_Config.h
* @brief   This file provides special hw functions for Cypress kits
*
********************************************************************************
* Copyright (2019-2021) Cypress Semiconductor Corporation (an Infineon company)
* or an affiliate of Cypress Semiconductor Corporation.
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
*****************************************************************************/


#if !defined(IO_CONFIG_H)
#define IO_CONFIG_H /**< Symbol preventing repeated inclusion */
#include "psoc5lp.h"
#include "CyLib.h"
#include "compiler.h"
#include "daplink.h"

// This GPIO configuration is only valid for the PSOC5LP HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_PSOC5LP);

bool kit_has_three_led(void);

#endif
