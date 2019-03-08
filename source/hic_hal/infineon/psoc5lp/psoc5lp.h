/*******************************************************************************
* @file    psoc5lp.h
* @brief   CMSIS Peripheral Access Layer for PSOC5LP
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

#if !defined(PSOC5LP_H)
#define PSOC5LP_H /**< Symbol preventing repeated inclusion */
/*
 * \brief CMSIS includes
 */
#include <stdbool.h>

#include "core_cm3_psoc5.h"

#define KP3_MODE_BULK                          (0x00u)
#define KP3_MODE_HID                           (0x01u)
#define KP3_MODE_BULK2UARTS                    (0x02u)

uint8_t interrogate_kit_hw_id(void);
uint8_t get_kit_hw_id(void);
bool kit_has_sflash_restriction(void);
void SetKitProgActiveApp(uint8_t mode);

#endif /* PSOC5LP_H */
