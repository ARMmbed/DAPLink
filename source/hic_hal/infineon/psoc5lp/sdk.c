/*******************************************************************************
* @file    sdk.c
* @brief   PSoC5LP specific initialization
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

#include "cyfitter_cfg.h"
#include "CyLib.h"
#include "LED_Amber.h"
#include "LED_Green.h"
#include "LED_Red.h"
#include "Timer_CSTick.h"
#include "Clk_Brea1.h"
#include "Clk_Brea2.h"
#include "sdk.h"


void sdk_init(void)
{
    CyGlobalIntEnable;

    /* Start Time source timer as soon as possible */
    Timer_CSTick_Start();

    LED_Amber_SetDriveMode(CY_PINS_DM_OD_HI);
    LED_Green_SetDriveMode(CY_PINS_DM_OD_HI);
    LED_Red_SetDriveMode(CY_PINS_DM_OD_HI);

    Clk_Brea1_Enable();
    Clk_Brea2_Enable();
}
