/*******************************************************************************
* File Name: Pin_UART_Tx.h
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
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

#if !defined(CY_PINS_Pin_UART_Tx_H) /* Pins Pin_UART_Tx_H */
#define CY_PINS_Pin_UART_Tx_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_UART_Tx_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_UART_Tx__PORT == 15 && ((Pin_UART_Tx__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes
***************************************/

/**
* \addtogroup group_general
* @{
*/
void    Pin_UART_Tx_Write(uint8 value);
void    Pin_UART_Tx_SetDriveMode(uint8 mode);
uint8   Pin_UART_Tx_ReadDataReg(void);
uint8   Pin_UART_Tx_Read(void);
void    Pin_UART_Tx_SetInterruptMode(uint16 position, uint16 mode);
uint8   Pin_UART_Tx_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Pin_UART_Tx_SetDriveMode() function.
     *  @{
     */
        #define Pin_UART_Tx_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Pin_UART_Tx_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Pin_UART_Tx_DM_RES_UP          PIN_DM_RES_UP
        #define Pin_UART_Tx_DM_RES_DWN         PIN_DM_RES_DWN
        #define Pin_UART_Tx_DM_OD_LO           PIN_DM_OD_LO
        #define Pin_UART_Tx_DM_OD_HI           PIN_DM_OD_HI
        #define Pin_UART_Tx_DM_STRONG          PIN_DM_STRONG
        #define Pin_UART_Tx_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */

/* Digital Port Constants */
#define Pin_UART_Tx_MASK               Pin_UART_Tx__MASK
#define Pin_UART_Tx_SHIFT              Pin_UART_Tx__SHIFT
#define Pin_UART_Tx_WIDTH              1u

/* Interrupt constants */
#if defined(Pin_UART_Tx__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Pin_UART_Tx_SetInterruptMode() function.
     *  @{
     */
        #define Pin_UART_Tx_INTR_NONE      (uint16)(0x0000u)
        #define Pin_UART_Tx_INTR_RISING    (uint16)(0x0001u)
        #define Pin_UART_Tx_INTR_FALLING   (uint16)(0x0002u)
        #define Pin_UART_Tx_INTR_BOTH      (uint16)(0x0003u)
    /** @} intrMode */
/** @} group_constants */

    #define Pin_UART_Tx_INTR_MASK      (0x01u)
#endif /* (Pin_UART_Tx__INTSTAT) */


/***************************************
*             Registers
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_UART_Tx_PS                     (* (reg8 *) Pin_UART_Tx__PS)
/* Data Register */
#define Pin_UART_Tx_DR                     (* (reg8 *) Pin_UART_Tx__DR)
/* Port Number */
#define Pin_UART_Tx_PRT_NUM                (* (reg8 *) Pin_UART_Tx__PRT)
/* Connect to Analog Globals */
#define Pin_UART_Tx_AG                     (* (reg8 *) Pin_UART_Tx__AG)
/* Analog MUX bux enable */
#define Pin_UART_Tx_AMUX                   (* (reg8 *) Pin_UART_Tx__AMUX)
/* Bidirectional Enable */
#define Pin_UART_Tx_BIE                    (* (reg8 *) Pin_UART_Tx__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_UART_Tx_BIT_MASK               (* (reg8 *) Pin_UART_Tx__BIT_MASK)
/* Bypass Enable */
#define Pin_UART_Tx_BYP                    (* (reg8 *) Pin_UART_Tx__BYP)
/* Port wide control signals */
#define Pin_UART_Tx_CTL                    (* (reg8 *) Pin_UART_Tx__CTL)
/* Drive Modes */
#define Pin_UART_Tx_DM0                    (* (reg8 *) Pin_UART_Tx__DM0)
#define Pin_UART_Tx_DM1                    (* (reg8 *) Pin_UART_Tx__DM1)
#define Pin_UART_Tx_DM2                    (* (reg8 *) Pin_UART_Tx__DM2)
/* Input Buffer Disable Override */
#define Pin_UART_Tx_INP_DIS                (* (reg8 *) Pin_UART_Tx__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_UART_Tx_LCD_COM_SEG            (* (reg8 *) Pin_UART_Tx__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_UART_Tx_LCD_EN                 (* (reg8 *) Pin_UART_Tx__LCD_EN)
/* Slew Rate Control */
#define Pin_UART_Tx_SLW                    (* (reg8 *) Pin_UART_Tx__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_UART_Tx_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_UART_Tx__PRTDSI__CAPS_SEL)
/* Double Sync Enable */
#define Pin_UART_Tx_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_UART_Tx__PRTDSI__DBL_SYNC_IN)
/* Output Enable Select Drive Strength */
#define Pin_UART_Tx_PRTDSI__OE_SEL0        (* (reg8 *) Pin_UART_Tx__PRTDSI__OE_SEL0)
#define Pin_UART_Tx_PRTDSI__OE_SEL1        (* (reg8 *) Pin_UART_Tx__PRTDSI__OE_SEL1)
/* Port Pin Output Select Registers */
#define Pin_UART_Tx_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_UART_Tx__PRTDSI__OUT_SEL0)
#define Pin_UART_Tx_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_UART_Tx__PRTDSI__OUT_SEL1)
/* Sync Output Enable Registers */
#define Pin_UART_Tx_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_UART_Tx__PRTDSI__SYNC_OUT)

/* SIO registers */
#if defined(Pin_UART_Tx__SIO_CFG)
    #define Pin_UART_Tx_SIO_HYST_EN        (* (reg8 *) Pin_UART_Tx__SIO_HYST_EN)
    #define Pin_UART_Tx_SIO_REG_HIFREQ     (* (reg8 *) Pin_UART_Tx__SIO_REG_HIFREQ)
    #define Pin_UART_Tx_SIO_CFG            (* (reg8 *) Pin_UART_Tx__SIO_CFG)
    #define Pin_UART_Tx_SIO_DIFF           (* (reg8 *) Pin_UART_Tx__SIO_DIFF)
#endif /* (Pin_UART_Tx__SIO_CFG) */

/* Interrupt Registers */
#if defined(Pin_UART_Tx__INTSTAT)
    #define Pin_UART_Tx_INTSTAT            (* (reg8 *) Pin_UART_Tx__INTSTAT)
    #define Pin_UART_Tx_SNAP               (* (reg8 *) Pin_UART_Tx__SNAP)

	#define Pin_UART_Tx_0_INTTYPE_REG 		(* (reg8 *) Pin_UART_Tx__0__INTTYPE)
#endif /* (Pin_UART_Tx__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Pin_UART_Tx_H */


/* [] END OF FILE */
