/*******************************************************************************
* File Name: LED_Green.h
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

#if !defined(CY_PINS_LED_Green_H) /* Pins LED_Green_H */
#define CY_PINS_LED_Green_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "LED_Green_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 LED_Green__PORT == 15 && ((LED_Green__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes
***************************************/

/**
* \addtogroup group_general
* @{
*/
void    LED_Green_Write(uint8 value);
void    LED_Green_SetDriveMode(uint8 mode);
uint8   LED_Green_ReadDataReg(void);
uint8   LED_Green_Read(void);
void    LED_Green_SetInterruptMode(uint16 position, uint16 mode);
uint8   LED_Green_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the LED_Green_SetDriveMode() function.
     *  @{
     */
        #define LED_Green_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define LED_Green_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define LED_Green_DM_RES_UP          PIN_DM_RES_UP
        #define LED_Green_DM_RES_DWN         PIN_DM_RES_DWN
        #define LED_Green_DM_OD_LO           PIN_DM_OD_LO
        #define LED_Green_DM_OD_HI           PIN_DM_OD_HI
        #define LED_Green_DM_STRONG          PIN_DM_STRONG
        #define LED_Green_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */

/* Digital Port Constants */
#define LED_Green_MASK               LED_Green__MASK
#define LED_Green_SHIFT              LED_Green__SHIFT
#define LED_Green_WIDTH              1u

/* Interrupt constants */
#if defined(LED_Green__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in LED_Green_SetInterruptMode() function.
     *  @{
     */
        #define LED_Green_INTR_NONE      (uint16)(0x0000u)
        #define LED_Green_INTR_RISING    (uint16)(0x0001u)
        #define LED_Green_INTR_FALLING   (uint16)(0x0002u)
        #define LED_Green_INTR_BOTH      (uint16)(0x0003u)
    /** @} intrMode */
/** @} group_constants */

    #define LED_Green_INTR_MASK      (0x01u)
#endif /* (LED_Green__INTSTAT) */


/***************************************
*             Registers
***************************************/

/* Main Port Registers */
/* Pin State */
#define LED_Green_PS                     (* (reg8 *) LED_Green__PS)
/* Data Register */
#define LED_Green_DR                     (* (reg8 *) LED_Green__DR)
/* Port Number */
#define LED_Green_PRT_NUM                (* (reg8 *) LED_Green__PRT)
/* Connect to Analog Globals */
#define LED_Green_AG                     (* (reg8 *) LED_Green__AG)
/* Analog MUX bux enable */
#define LED_Green_AMUX                   (* (reg8 *) LED_Green__AMUX)
/* Bidirectional Enable */
#define LED_Green_BIE                    (* (reg8 *) LED_Green__BIE)
/* Bit-mask for Aliased Register Access */
#define LED_Green_BIT_MASK               (* (reg8 *) LED_Green__BIT_MASK)
/* Bypass Enable */
#define LED_Green_BYP                    (* (reg8 *) LED_Green__BYP)
/* Port wide control signals */
#define LED_Green_CTL                    (* (reg8 *) LED_Green__CTL)
/* Drive Modes */
#define LED_Green_DM0                    (* (reg8 *) LED_Green__DM0)
#define LED_Green_DM1                    (* (reg8 *) LED_Green__DM1)
#define LED_Green_DM2                    (* (reg8 *) LED_Green__DM2)
/* Input Buffer Disable Override */
#define LED_Green_INP_DIS                (* (reg8 *) LED_Green__INP_DIS)
/* LCD Common or Segment Drive */
#define LED_Green_LCD_COM_SEG            (* (reg8 *) LED_Green__LCD_COM_SEG)
/* Enable Segment LCD */
#define LED_Green_LCD_EN                 (* (reg8 *) LED_Green__LCD_EN)
/* Slew Rate Control */
#define LED_Green_SLW                    (* (reg8 *) LED_Green__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define LED_Green_PRTDSI__CAPS_SEL       (* (reg8 *) LED_Green__PRTDSI__CAPS_SEL)
/* Double Sync Enable */
#define LED_Green_PRTDSI__DBL_SYNC_IN    (* (reg8 *) LED_Green__PRTDSI__DBL_SYNC_IN)
/* Output Enable Select Drive Strength */
#define LED_Green_PRTDSI__OE_SEL0        (* (reg8 *) LED_Green__PRTDSI__OE_SEL0)
#define LED_Green_PRTDSI__OE_SEL1        (* (reg8 *) LED_Green__PRTDSI__OE_SEL1)
/* Port Pin Output Select Registers */
#define LED_Green_PRTDSI__OUT_SEL0       (* (reg8 *) LED_Green__PRTDSI__OUT_SEL0)
#define LED_Green_PRTDSI__OUT_SEL1       (* (reg8 *) LED_Green__PRTDSI__OUT_SEL1)
/* Sync Output Enable Registers */
#define LED_Green_PRTDSI__SYNC_OUT       (* (reg8 *) LED_Green__PRTDSI__SYNC_OUT)

/* SIO registers */
#if defined(LED_Green__SIO_CFG)
    #define LED_Green_SIO_HYST_EN        (* (reg8 *) LED_Green__SIO_HYST_EN)
    #define LED_Green_SIO_REG_HIFREQ     (* (reg8 *) LED_Green__SIO_REG_HIFREQ)
    #define LED_Green_SIO_CFG            (* (reg8 *) LED_Green__SIO_CFG)
    #define LED_Green_SIO_DIFF           (* (reg8 *) LED_Green__SIO_DIFF)
#endif /* (LED_Green__SIO_CFG) */

/* Interrupt Registers */
#if defined(LED_Green__INTSTAT)
    #define LED_Green_INTSTAT            (* (reg8 *) LED_Green__INTSTAT)
    #define LED_Green_SNAP               (* (reg8 *) LED_Green__SNAP)

	#define LED_Green_0_INTTYPE_REG 		(* (reg8 *) LED_Green__0__INTTYPE)
#endif /* (LED_Green__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_LED_Green_H */


/* [] END OF FILE */
