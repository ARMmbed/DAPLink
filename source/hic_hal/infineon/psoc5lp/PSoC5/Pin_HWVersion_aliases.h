/*******************************************************************************
* File Name: Pin_HWVersion.h
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

#if !defined(CY_PINS_Pin_HWVersion_ALIASES_H) /* Pins Pin_HWVersion_ALIASES_H */
#define CY_PINS_Pin_HWVersion_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants
***************************************/
#define Pin_HWVersion_0			(Pin_HWVersion__0__PC)
#define Pin_HWVersion_0_INTR	((uint16)((uint16)0x0001u << Pin_HWVersion__0__SHIFT))

#define Pin_HWVersion_1			(Pin_HWVersion__1__PC)
#define Pin_HWVersion_1_INTR	((uint16)((uint16)0x0001u << Pin_HWVersion__1__SHIFT))

#define Pin_HWVersion_2			(Pin_HWVersion__2__PC)
#define Pin_HWVersion_2_INTR	((uint16)((uint16)0x0001u << Pin_HWVersion__2__SHIFT))

#define Pin_HWVersion_3			(Pin_HWVersion__3__PC)
#define Pin_HWVersion_3_INTR	((uint16)((uint16)0x0001u << Pin_HWVersion__3__SHIFT))

#define Pin_HWVersion_4			(Pin_HWVersion__4__PC)
#define Pin_HWVersion_4_INTR	((uint16)((uint16)0x0001u << Pin_HWVersion__4__SHIFT))

#define Pin_HWVersion_INTR_ALL	 ((uint16)(Pin_HWVersion_0_INTR| Pin_HWVersion_1_INTR| Pin_HWVersion_2_INTR| Pin_HWVersion_3_INTR| Pin_HWVersion_4_INTR))

#endif /* End Pins Pin_HWVersion_ALIASES_H */


/* [] END OF FILE */
