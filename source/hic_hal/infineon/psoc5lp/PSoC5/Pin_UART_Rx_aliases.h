/*******************************************************************************
* File Name: Pin_UART_Rx.h
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

#if !defined(CY_PINS_Pin_UART_Rx_ALIASES_H) /* Pins Pin_UART_Rx_ALIASES_H */
#define CY_PINS_Pin_UART_Rx_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants
***************************************/
#define Pin_UART_Rx_0			(Pin_UART_Rx__0__PC)
#define Pin_UART_Rx_0_INTR	((uint16)((uint16)0x0001u << Pin_UART_Rx__0__SHIFT))

#define Pin_UART_Rx_INTR_ALL	 ((uint16)(Pin_UART_Rx_0_INTR))

#endif /* End Pins Pin_UART_Rx_ALIASES_H */


/* [] END OF FILE */
