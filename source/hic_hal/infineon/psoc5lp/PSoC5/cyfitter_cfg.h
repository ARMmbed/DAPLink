/*******************************************************************************
* File Name: cyfitter_cfg.h
*
* Description:
* This file provides basic startup and mux configuration settings
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

#ifndef CYFITTER_CFG_H
#define CYFITTER_CFG_H

#include "cytypes.h"

extern void cyfitter_cfg(void);

/* Analog Set/Unset methods */
extern void SetAnalogRoutingPumps(uint8 enabled);
extern void AdcMux_Set(uint8 channel);
extern void AdcMux_Unset(uint8 channel);
extern void ADC_DelSig_AMux_Set(uint8 channel);
extern void ADC_DelSig_AMux_Unset(uint8 channel);
/* ADC_DelSig_AMux__addrTable is an implementation detail of the AMux.
   Code that depends on it may be incompatible with other versions
   of PSoC Creator. */
extern uint8 CYXDATA * const CYCODE ADC_DelSig_AMux__addrTable[2];
/* ADC_DelSig_AMux__maskTable is an implementation detail of the AMux.
   Code that depends on it may be incompatible with other versions
   of PSoC Creator. */
extern const uint8 CYCODE ADC_DelSig_AMux__maskTable[2];


#endif /* CYFITTER_CFG_H */

/*[]*/
