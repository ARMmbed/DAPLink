/*******************************************************************************
* File Name: SWDIO.h
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

#if !defined(CY_PINS_SWDIO_ALIASES_H) /* Pins SWDIO_ALIASES_H */
#define CY_PINS_SWDIO_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants
***************************************/
#define SWDIO_0			(SWDIO__0__PC)
#define SWDIO_0_INTR	((uint16)((uint16)0x0001u << SWDIO__0__SHIFT))

#define SWDIO_INTR_ALL	 ((uint16)(SWDIO_0_INTR))

#endif /* End Pins SWDIO_ALIASES_H */


/* [] END OF FILE */
