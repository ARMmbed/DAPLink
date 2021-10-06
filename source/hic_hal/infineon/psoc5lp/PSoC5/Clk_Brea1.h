/*******************************************************************************
* File Name: Clk_Brea1.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
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

#if !defined(CY_CLOCK_Clk_Brea1_H)
#define CY_CLOCK_Clk_Brea1_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void Clk_Brea1_Start(void) ;
void Clk_Brea1_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void Clk_Brea1_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void Clk_Brea1_StandbyPower(uint8 state) ;
void Clk_Brea1_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 Clk_Brea1_GetDividerRegister(void) ;
void Clk_Brea1_SetModeRegister(uint8 modeBitMask) ;
void Clk_Brea1_ClearModeRegister(uint8 modeBitMask) ;
uint8 Clk_Brea1_GetModeRegister(void) ;
void Clk_Brea1_SetSourceRegister(uint8 clkSource) ;
uint8 Clk_Brea1_GetSourceRegister(void) ;
#if defined(Clk_Brea1__CFG3)
void Clk_Brea1_SetPhaseRegister(uint8 clkPhase) ;
uint8 Clk_Brea1_GetPhaseRegister(void) ;
#endif /* defined(Clk_Brea1__CFG3) */

#define Clk_Brea1_Enable()                       Clk_Brea1_Start()
#define Clk_Brea1_Disable()                      Clk_Brea1_Stop()
#define Clk_Brea1_SetDivider(clkDivider)         Clk_Brea1_SetDividerRegister(clkDivider, 1u)
#define Clk_Brea1_SetDividerValue(clkDivider)    Clk_Brea1_SetDividerRegister((clkDivider) - 1u, 1u)
#define Clk_Brea1_SetMode(clkMode)               Clk_Brea1_SetModeRegister(clkMode)
#define Clk_Brea1_SetSource(clkSource)           Clk_Brea1_SetSourceRegister(clkSource)
#if defined(Clk_Brea1__CFG3)
#define Clk_Brea1_SetPhase(clkPhase)             Clk_Brea1_SetPhaseRegister(clkPhase)
#define Clk_Brea1_SetPhaseValue(clkPhase)        Clk_Brea1_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(Clk_Brea1__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define Clk_Brea1_CLKEN              (* (reg8 *) Clk_Brea1__PM_ACT_CFG)
#define Clk_Brea1_CLKEN_PTR          ((reg8 *) Clk_Brea1__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define Clk_Brea1_CLKSTBY            (* (reg8 *) Clk_Brea1__PM_STBY_CFG)
#define Clk_Brea1_CLKSTBY_PTR        ((reg8 *) Clk_Brea1__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define Clk_Brea1_DIV_LSB            (* (reg8 *) Clk_Brea1__CFG0)
#define Clk_Brea1_DIV_LSB_PTR        ((reg8 *) Clk_Brea1__CFG0)
#define Clk_Brea1_DIV_PTR            ((reg16 *) Clk_Brea1__CFG0)

/* Clock MSB divider configuration register. */
#define Clk_Brea1_DIV_MSB            (* (reg8 *) Clk_Brea1__CFG1)
#define Clk_Brea1_DIV_MSB_PTR        ((reg8 *) Clk_Brea1__CFG1)

/* Mode and source configuration register */
#define Clk_Brea1_MOD_SRC            (* (reg8 *) Clk_Brea1__CFG2)
#define Clk_Brea1_MOD_SRC_PTR        ((reg8 *) Clk_Brea1__CFG2)

#if defined(Clk_Brea1__CFG3)
/* Analog clock phase configuration register */
#define Clk_Brea1_PHASE              (* (reg8 *) Clk_Brea1__CFG3)
#define Clk_Brea1_PHASE_PTR          ((reg8 *) Clk_Brea1__CFG3)
#endif /* defined(Clk_Brea1__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define Clk_Brea1_CLKEN_MASK         Clk_Brea1__PM_ACT_MSK
#define Clk_Brea1_CLKSTBY_MASK       Clk_Brea1__PM_STBY_MSK

/* CFG2 field masks */
#define Clk_Brea1_SRC_SEL_MSK        Clk_Brea1__CFG2_SRC_SEL_MASK
#define Clk_Brea1_MODE_MASK          (~(Clk_Brea1_SRC_SEL_MSK))

#if defined(Clk_Brea1__CFG3)
/* CFG3 phase mask */
#define Clk_Brea1_PHASE_MASK         Clk_Brea1__CFG3_PHASE_DLY_MASK
#endif /* defined(Clk_Brea1__CFG3) */

#endif /* CY_CLOCK_Clk_Brea1_H */


/* [] END OF FILE */
