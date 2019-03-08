/*******************************************************************************
* File Name: ModeButton.h
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h.
*  Information on using these APIs can be found in the System Reference Guide.
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

#if !defined(CY_PINS_ModeButton_ALIASES_H) /* Pins ModeButton_ALIASES_H */
#define CY_PINS_ModeButton_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants
***************************************/
#define ModeButton_0			(ModeButton__0__PC)
#define ModeButton_0_INTR	((uint16)((uint16)0x0001u << ModeButton__0__SHIFT))

#define ModeButton_INTR_ALL	 ((uint16)(ModeButton_0_INTR))

#endif /* End Pins ModeButton_ALIASES_H */


/* [] END OF FILE */
