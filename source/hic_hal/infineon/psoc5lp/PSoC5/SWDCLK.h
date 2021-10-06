/*******************************************************************************
* File Name: SWDCLK.h
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

#if !defined(CY_PINS_SWDCLK_H) /* Pins SWDCLK_H */
#define CY_PINS_SWDCLK_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SWDCLK_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 SWDCLK__PORT == 15 && ((SWDCLK__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes
***************************************/

/**
* \addtogroup group_general
* @{
*/
void    SWDCLK_Write(uint8 value);
void    SWDCLK_SetDriveMode(uint8 mode);
uint8   SWDCLK_ReadDataReg(void);
uint8   SWDCLK_Read(void);
void    SWDCLK_SetInterruptMode(uint16 position, uint16 mode);
uint8   SWDCLK_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the SWDCLK_SetDriveMode() function.
     *  @{
     */
        #define SWDCLK_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define SWDCLK_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define SWDCLK_DM_RES_UP          PIN_DM_RES_UP
        #define SWDCLK_DM_RES_DWN         PIN_DM_RES_DWN
        #define SWDCLK_DM_OD_LO           PIN_DM_OD_LO
        #define SWDCLK_DM_OD_HI           PIN_DM_OD_HI
        #define SWDCLK_DM_STRONG          PIN_DM_STRONG
        #define SWDCLK_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */

/* Digital Port Constants */
#define SWDCLK_MASK               SWDCLK__MASK
#define SWDCLK_SHIFT              SWDCLK__SHIFT
#define SWDCLK_WIDTH              1u

/* Interrupt constants */
#if defined(SWDCLK__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SWDCLK_SetInterruptMode() function.
     *  @{
     */
        #define SWDCLK_INTR_NONE      (uint16)(0x0000u)
        #define SWDCLK_INTR_RISING    (uint16)(0x0001u)
        #define SWDCLK_INTR_FALLING   (uint16)(0x0002u)
        #define SWDCLK_INTR_BOTH      (uint16)(0x0003u)
    /** @} intrMode */
/** @} group_constants */

    #define SWDCLK_INTR_MASK      (0x01u)
#endif /* (SWDCLK__INTSTAT) */


/***************************************
*             Registers
***************************************/

/* Main Port Registers */
/* Pin State */
#define SWDCLK_PS                     (* (reg8 *) SWDCLK__PS)
/* Data Register */
#define SWDCLK_DR                     (* (reg8 *) SWDCLK__DR)
/* Port Number */
#define SWDCLK_PRT_NUM                (* (reg8 *) SWDCLK__PRT)
/* Connect to Analog Globals */
#define SWDCLK_AG                     (* (reg8 *) SWDCLK__AG)
/* Analog MUX bux enable */
#define SWDCLK_AMUX                   (* (reg8 *) SWDCLK__AMUX)
/* Bidirectional Enable */
#define SWDCLK_BIE                    (* (reg8 *) SWDCLK__BIE)
/* Bit-mask for Aliased Register Access */
#define SWDCLK_BIT_MASK               (* (reg8 *) SWDCLK__BIT_MASK)
/* Bypass Enable */
#define SWDCLK_BYP                    (* (reg8 *) SWDCLK__BYP)
/* Port wide control signals */
#define SWDCLK_CTL                    (* (reg8 *) SWDCLK__CTL)
/* Drive Modes */
#define SWDCLK_DM0                    (* (reg8 *) SWDCLK__DM0)
#define SWDCLK_DM1                    (* (reg8 *) SWDCLK__DM1)
#define SWDCLK_DM2                    (* (reg8 *) SWDCLK__DM2)
/* Input Buffer Disable Override */
#define SWDCLK_INP_DIS                (* (reg8 *) SWDCLK__INP_DIS)
/* LCD Common or Segment Drive */
#define SWDCLK_LCD_COM_SEG            (* (reg8 *) SWDCLK__LCD_COM_SEG)
/* Enable Segment LCD */
#define SWDCLK_LCD_EN                 (* (reg8 *) SWDCLK__LCD_EN)
/* Slew Rate Control */
#define SWDCLK_SLW                    (* (reg8 *) SWDCLK__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SWDCLK_PRTDSI__CAPS_SEL       (* (reg8 *) SWDCLK__PRTDSI__CAPS_SEL)
/* Double Sync Enable */
#define SWDCLK_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SWDCLK__PRTDSI__DBL_SYNC_IN)
/* Output Enable Select Drive Strength */
#define SWDCLK_PRTDSI__OE_SEL0        (* (reg8 *) SWDCLK__PRTDSI__OE_SEL0)
#define SWDCLK_PRTDSI__OE_SEL1        (* (reg8 *) SWDCLK__PRTDSI__OE_SEL1)
/* Port Pin Output Select Registers */
#define SWDCLK_PRTDSI__OUT_SEL0       (* (reg8 *) SWDCLK__PRTDSI__OUT_SEL0)
#define SWDCLK_PRTDSI__OUT_SEL1       (* (reg8 *) SWDCLK__PRTDSI__OUT_SEL1)
/* Sync Output Enable Registers */
#define SWDCLK_PRTDSI__SYNC_OUT       (* (reg8 *) SWDCLK__PRTDSI__SYNC_OUT)

/* SIO registers */
#if defined(SWDCLK__SIO_CFG)
    #define SWDCLK_SIO_HYST_EN        (* (reg8 *) SWDCLK__SIO_HYST_EN)
    #define SWDCLK_SIO_REG_HIFREQ     (* (reg8 *) SWDCLK__SIO_REG_HIFREQ)
    #define SWDCLK_SIO_CFG            (* (reg8 *) SWDCLK__SIO_CFG)
    #define SWDCLK_SIO_DIFF           (* (reg8 *) SWDCLK__SIO_DIFF)
#endif /* (SWDCLK__SIO_CFG) */

/* Interrupt Registers */
#if defined(SWDCLK__INTSTAT)
    #define SWDCLK_INTSTAT            (* (reg8 *) SWDCLK__INTSTAT)
    #define SWDCLK_SNAP               (* (reg8 *) SWDCLK__SNAP)

	#define SWDCLK_0_INTTYPE_REG 		(* (reg8 *) SWDCLK__0__INTTYPE)
#endif /* (SWDCLK__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_SWDCLK_H */


/* [] END OF FILE */
