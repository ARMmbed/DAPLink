/*******************************************************************************
* File Name: LED_Red.h
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

#if !defined(CY_PINS_LED_Red_H) /* Pins LED_Red_H */
#define CY_PINS_LED_Red_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "LED_Red_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 LED_Red__PORT == 15 && ((LED_Red__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes
***************************************/

/**
* \addtogroup group_general
* @{
*/
void    LED_Red_Write(uint8 value);
void    LED_Red_SetDriveMode(uint8 mode);
uint8   LED_Red_ReadDataReg(void);
uint8   LED_Red_Read(void);
void    LED_Red_SetInterruptMode(uint16 position, uint16 mode);
uint8   LED_Red_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the LED_Red_SetDriveMode() function.
     *  @{
     */
        #define LED_Red_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define LED_Red_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define LED_Red_DM_RES_UP          PIN_DM_RES_UP
        #define LED_Red_DM_RES_DWN         PIN_DM_RES_DWN
        #define LED_Red_DM_OD_LO           PIN_DM_OD_LO
        #define LED_Red_DM_OD_HI           PIN_DM_OD_HI
        #define LED_Red_DM_STRONG          PIN_DM_STRONG
        #define LED_Red_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */

/* Digital Port Constants */
#define LED_Red_MASK               LED_Red__MASK
#define LED_Red_SHIFT              LED_Red__SHIFT
#define LED_Red_WIDTH              1u

/* Interrupt constants */
#if defined(LED_Red__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in LED_Red_SetInterruptMode() function.
     *  @{
     */
        #define LED_Red_INTR_NONE      (uint16)(0x0000u)
        #define LED_Red_INTR_RISING    (uint16)(0x0001u)
        #define LED_Red_INTR_FALLING   (uint16)(0x0002u)
        #define LED_Red_INTR_BOTH      (uint16)(0x0003u)
    /** @} intrMode */
/** @} group_constants */

    #define LED_Red_INTR_MASK      (0x01u)
#endif /* (LED_Red__INTSTAT) */


/***************************************
*             Registers
***************************************/

/* Main Port Registers */
/* Pin State */
#define LED_Red_PS                     (* (reg8 *) LED_Red__PS)
/* Data Register */
#define LED_Red_DR                     (* (reg8 *) LED_Red__DR)
/* Port Number */
#define LED_Red_PRT_NUM                (* (reg8 *) LED_Red__PRT)
/* Connect to Analog Globals */
#define LED_Red_AG                     (* (reg8 *) LED_Red__AG)
/* Analog MUX bux enable */
#define LED_Red_AMUX                   (* (reg8 *) LED_Red__AMUX)
/* Bidirectional Enable */
#define LED_Red_BIE                    (* (reg8 *) LED_Red__BIE)
/* Bit-mask for Aliased Register Access */
#define LED_Red_BIT_MASK               (* (reg8 *) LED_Red__BIT_MASK)
/* Bypass Enable */
#define LED_Red_BYP                    (* (reg8 *) LED_Red__BYP)
/* Port wide control signals */
#define LED_Red_CTL                    (* (reg8 *) LED_Red__CTL)
/* Drive Modes */
#define LED_Red_DM0                    (* (reg8 *) LED_Red__DM0)
#define LED_Red_DM1                    (* (reg8 *) LED_Red__DM1)
#define LED_Red_DM2                    (* (reg8 *) LED_Red__DM2)
/* Input Buffer Disable Override */
#define LED_Red_INP_DIS                (* (reg8 *) LED_Red__INP_DIS)
/* LCD Common or Segment Drive */
#define LED_Red_LCD_COM_SEG            (* (reg8 *) LED_Red__LCD_COM_SEG)
/* Enable Segment LCD */
#define LED_Red_LCD_EN                 (* (reg8 *) LED_Red__LCD_EN)
/* Slew Rate Control */
#define LED_Red_SLW                    (* (reg8 *) LED_Red__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define LED_Red_PRTDSI__CAPS_SEL       (* (reg8 *) LED_Red__PRTDSI__CAPS_SEL)
/* Double Sync Enable */
#define LED_Red_PRTDSI__DBL_SYNC_IN    (* (reg8 *) LED_Red__PRTDSI__DBL_SYNC_IN)
/* Output Enable Select Drive Strength */
#define LED_Red_PRTDSI__OE_SEL0        (* (reg8 *) LED_Red__PRTDSI__OE_SEL0)
#define LED_Red_PRTDSI__OE_SEL1        (* (reg8 *) LED_Red__PRTDSI__OE_SEL1)
/* Port Pin Output Select Registers */
#define LED_Red_PRTDSI__OUT_SEL0       (* (reg8 *) LED_Red__PRTDSI__OUT_SEL0)
#define LED_Red_PRTDSI__OUT_SEL1       (* (reg8 *) LED_Red__PRTDSI__OUT_SEL1)
/* Sync Output Enable Registers */
#define LED_Red_PRTDSI__SYNC_OUT       (* (reg8 *) LED_Red__PRTDSI__SYNC_OUT)

/* SIO registers */
#if defined(LED_Red__SIO_CFG)
    #define LED_Red_SIO_HYST_EN        (* (reg8 *) LED_Red__SIO_HYST_EN)
    #define LED_Red_SIO_REG_HIFREQ     (* (reg8 *) LED_Red__SIO_REG_HIFREQ)
    #define LED_Red_SIO_CFG            (* (reg8 *) LED_Red__SIO_CFG)
    #define LED_Red_SIO_DIFF           (* (reg8 *) LED_Red__SIO_DIFF)
#endif /* (LED_Red__SIO_CFG) */

/* Interrupt Registers */
#if defined(LED_Red__INTSTAT)
    #define LED_Red_INTSTAT            (* (reg8 *) LED_Red__INTSTAT)
    #define LED_Red_SNAP               (* (reg8 *) LED_Red__SNAP)

	#define LED_Red_0_INTTYPE_REG 		(* (reg8 *) LED_Red__0__INTTYPE)
#endif /* (LED_Red__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_LED_Red_H */


/* [] END OF FILE */
