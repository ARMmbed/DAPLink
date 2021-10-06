/*******************************************************************************
* File Name: LED_Amber.h
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

#if !defined(CY_PINS_LED_Amber_H) /* Pins LED_Amber_H */
#define CY_PINS_LED_Amber_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "LED_Amber_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 LED_Amber__PORT == 15 && ((LED_Amber__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes
***************************************/

/**
* \addtogroup group_general
* @{
*/
void    LED_Amber_Write(uint8 value);
void    LED_Amber_SetDriveMode(uint8 mode);
uint8   LED_Amber_ReadDataReg(void);
uint8   LED_Amber_Read(void);
void    LED_Amber_SetInterruptMode(uint16 position, uint16 mode);
uint8   LED_Amber_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the LED_Amber_SetDriveMode() function.
     *  @{
     */
        #define LED_Amber_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define LED_Amber_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define LED_Amber_DM_RES_UP          PIN_DM_RES_UP
        #define LED_Amber_DM_RES_DWN         PIN_DM_RES_DWN
        #define LED_Amber_DM_OD_LO           PIN_DM_OD_LO
        #define LED_Amber_DM_OD_HI           PIN_DM_OD_HI
        #define LED_Amber_DM_STRONG          PIN_DM_STRONG
        #define LED_Amber_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */

/* Digital Port Constants */
#define LED_Amber_MASK               LED_Amber__MASK
#define LED_Amber_SHIFT              LED_Amber__SHIFT
#define LED_Amber_WIDTH              1u

/* Interrupt constants */
#if defined(LED_Amber__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in LED_Amber_SetInterruptMode() function.
     *  @{
     */
        #define LED_Amber_INTR_NONE      (uint16)(0x0000u)
        #define LED_Amber_INTR_RISING    (uint16)(0x0001u)
        #define LED_Amber_INTR_FALLING   (uint16)(0x0002u)
        #define LED_Amber_INTR_BOTH      (uint16)(0x0003u)
    /** @} intrMode */
/** @} group_constants */

    #define LED_Amber_INTR_MASK      (0x01u)
#endif /* (LED_Amber__INTSTAT) */


/***************************************
*             Registers
***************************************/

/* Main Port Registers */
/* Pin State */
#define LED_Amber_PS                     (* (reg8 *) LED_Amber__PS)
/* Data Register */
#define LED_Amber_DR                     (* (reg8 *) LED_Amber__DR)
/* Port Number */
#define LED_Amber_PRT_NUM                (* (reg8 *) LED_Amber__PRT)
/* Connect to Analog Globals */
#define LED_Amber_AG                     (* (reg8 *) LED_Amber__AG)
/* Analog MUX bux enable */
#define LED_Amber_AMUX                   (* (reg8 *) LED_Amber__AMUX)
/* Bidirectional Enable */
#define LED_Amber_BIE                    (* (reg8 *) LED_Amber__BIE)
/* Bit-mask for Aliased Register Access */
#define LED_Amber_BIT_MASK               (* (reg8 *) LED_Amber__BIT_MASK)
/* Bypass Enable */
#define LED_Amber_BYP                    (* (reg8 *) LED_Amber__BYP)
/* Port wide control signals */
#define LED_Amber_CTL                    (* (reg8 *) LED_Amber__CTL)
/* Drive Modes */
#define LED_Amber_DM0                    (* (reg8 *) LED_Amber__DM0)
#define LED_Amber_DM1                    (* (reg8 *) LED_Amber__DM1)
#define LED_Amber_DM2                    (* (reg8 *) LED_Amber__DM2)
/* Input Buffer Disable Override */
#define LED_Amber_INP_DIS                (* (reg8 *) LED_Amber__INP_DIS)
/* LCD Common or Segment Drive */
#define LED_Amber_LCD_COM_SEG            (* (reg8 *) LED_Amber__LCD_COM_SEG)
/* Enable Segment LCD */
#define LED_Amber_LCD_EN                 (* (reg8 *) LED_Amber__LCD_EN)
/* Slew Rate Control */
#define LED_Amber_SLW                    (* (reg8 *) LED_Amber__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define LED_Amber_PRTDSI__CAPS_SEL       (* (reg8 *) LED_Amber__PRTDSI__CAPS_SEL)
/* Double Sync Enable */
#define LED_Amber_PRTDSI__DBL_SYNC_IN    (* (reg8 *) LED_Amber__PRTDSI__DBL_SYNC_IN)
/* Output Enable Select Drive Strength */
#define LED_Amber_PRTDSI__OE_SEL0        (* (reg8 *) LED_Amber__PRTDSI__OE_SEL0)
#define LED_Amber_PRTDSI__OE_SEL1        (* (reg8 *) LED_Amber__PRTDSI__OE_SEL1)
/* Port Pin Output Select Registers */
#define LED_Amber_PRTDSI__OUT_SEL0       (* (reg8 *) LED_Amber__PRTDSI__OUT_SEL0)
#define LED_Amber_PRTDSI__OUT_SEL1       (* (reg8 *) LED_Amber__PRTDSI__OUT_SEL1)
/* Sync Output Enable Registers */
#define LED_Amber_PRTDSI__SYNC_OUT       (* (reg8 *) LED_Amber__PRTDSI__SYNC_OUT)

/* SIO registers */
#if defined(LED_Amber__SIO_CFG)
    #define LED_Amber_SIO_HYST_EN        (* (reg8 *) LED_Amber__SIO_HYST_EN)
    #define LED_Amber_SIO_REG_HIFREQ     (* (reg8 *) LED_Amber__SIO_REG_HIFREQ)
    #define LED_Amber_SIO_CFG            (* (reg8 *) LED_Amber__SIO_CFG)
    #define LED_Amber_SIO_DIFF           (* (reg8 *) LED_Amber__SIO_DIFF)
#endif /* (LED_Amber__SIO_CFG) */

/* Interrupt Registers */
#if defined(LED_Amber__INTSTAT)
    #define LED_Amber_INTSTAT            (* (reg8 *) LED_Amber__INTSTAT)
    #define LED_Amber_SNAP               (* (reg8 *) LED_Amber__SNAP)

	#define LED_Amber_0_INTTYPE_REG 		(* (reg8 *) LED_Amber__0__INTTYPE)
#endif /* (LED_Amber__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_LED_Amber_H */


/* [] END OF FILE */
