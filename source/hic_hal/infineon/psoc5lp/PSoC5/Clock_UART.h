/*******************************************************************************
* File Name: Clock_UART.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
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

#if !defined(CY_CLOCK_Clock_UART_H)
#define CY_CLOCK_Clock_UART_H

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

void Clock_UART_Start(void) ;
void Clock_UART_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void Clock_UART_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void Clock_UART_StandbyPower(uint8 state) ;
void Clock_UART_SetDividerRegister(uint16 clkDivider, uint8 restart)
                                ;
uint16 Clock_UART_GetDividerRegister(void) ;
void Clock_UART_SetModeRegister(uint8 modeBitMask) ;
void Clock_UART_ClearModeRegister(uint8 modeBitMask) ;
uint8 Clock_UART_GetModeRegister(void) ;
void Clock_UART_SetSourceRegister(uint8 clkSource) ;
uint8 Clock_UART_GetSourceRegister(void) ;
#if defined(Clock_UART__CFG3)
void Clock_UART_SetPhaseRegister(uint8 clkPhase) ;
uint8 Clock_UART_GetPhaseRegister(void) ;
#endif /* defined(Clock_UART__CFG3) */

#define Clock_UART_Enable()                       Clock_UART_Start()
#define Clock_UART_Disable()                      Clock_UART_Stop()
#define Clock_UART_SetDivider(clkDivider)         Clock_UART_SetDividerRegister(clkDivider, 1u)
#define Clock_UART_SetDividerValue(clkDivider)    Clock_UART_SetDividerRegister((clkDivider) - 1u, 1u)
#define Clock_UART_SetMode(clkMode)               Clock_UART_SetModeRegister(clkMode)
#define Clock_UART_SetSource(clkSource)           Clock_UART_SetSourceRegister(clkSource)
#if defined(Clock_UART__CFG3)
#define Clock_UART_SetPhase(clkPhase)             Clock_UART_SetPhaseRegister(clkPhase)
#define Clock_UART_SetPhaseValue(clkPhase)        Clock_UART_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(Clock_UART__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define Clock_UART_CLKEN              (* (reg8 *) Clock_UART__PM_ACT_CFG)
#define Clock_UART_CLKEN_PTR          ((reg8 *) Clock_UART__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define Clock_UART_CLKSTBY            (* (reg8 *) Clock_UART__PM_STBY_CFG)
#define Clock_UART_CLKSTBY_PTR        ((reg8 *) Clock_UART__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define Clock_UART_DIV_LSB            (* (reg8 *) Clock_UART__CFG0)
#define Clock_UART_DIV_LSB_PTR        ((reg8 *) Clock_UART__CFG0)
#define Clock_UART_DIV_PTR            ((reg16 *) Clock_UART__CFG0)

/* Clock MSB divider configuration register. */
#define Clock_UART_DIV_MSB            (* (reg8 *) Clock_UART__CFG1)
#define Clock_UART_DIV_MSB_PTR        ((reg8 *) Clock_UART__CFG1)

/* Mode and source configuration register */
#define Clock_UART_MOD_SRC            (* (reg8 *) Clock_UART__CFG2)
#define Clock_UART_MOD_SRC_PTR        ((reg8 *) Clock_UART__CFG2)

#if defined(Clock_UART__CFG3)
/* Analog clock phase configuration register */
#define Clock_UART_PHASE              (* (reg8 *) Clock_UART__CFG3)
#define Clock_UART_PHASE_PTR          ((reg8 *) Clock_UART__CFG3)
#endif /* defined(Clock_UART__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define Clock_UART_CLKEN_MASK         Clock_UART__PM_ACT_MSK
#define Clock_UART_CLKSTBY_MASK       Clock_UART__PM_STBY_MSK

/* CFG2 field masks */
#define Clock_UART_SRC_SEL_MSK        Clock_UART__CFG2_SRC_SEL_MASK
#define Clock_UART_MODE_MASK          (~(Clock_UART_SRC_SEL_MSK))

#if defined(Clock_UART__CFG3)
/* CFG3 phase mask */
#define Clock_UART_PHASE_MASK         Clock_UART__CFG3_PHASE_DLY_MASK
#endif /* defined(Clock_UART__CFG3) */

#endif /* CY_CLOCK_Clock_UART_H */


/* [] END OF FILE */
