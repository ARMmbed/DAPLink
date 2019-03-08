/*******************************************************************************
* File Name: ModeButton.h
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

#if !defined(CY_PINS_ModeButton_H) /* Pins ModeButton_H */
#define CY_PINS_ModeButton_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "ModeButton_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 ModeButton__PORT == 15 && ((ModeButton__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes
***************************************/

/**
* \addtogroup group_general
* @{
*/
void    ModeButton_Write(uint8 value);
void    ModeButton_SetDriveMode(uint8 mode);
uint8   ModeButton_ReadDataReg(void);
uint8   ModeButton_Read(void);
void    ModeButton_SetInterruptMode(uint16 position, uint16 mode);
uint8   ModeButton_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the ModeButton_SetDriveMode() function.
     *  @{
     */
        #define ModeButton_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define ModeButton_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define ModeButton_DM_RES_UP          PIN_DM_RES_UP
        #define ModeButton_DM_RES_DWN         PIN_DM_RES_DWN
        #define ModeButton_DM_OD_LO           PIN_DM_OD_LO
        #define ModeButton_DM_OD_HI           PIN_DM_OD_HI
        #define ModeButton_DM_STRONG          PIN_DM_STRONG
        #define ModeButton_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */

/* Digital Port Constants */
#define ModeButton_MASK               ModeButton__MASK
#define ModeButton_SHIFT              ModeButton__SHIFT
#define ModeButton_WIDTH              1u

/* Interrupt constants */
#if defined(ModeButton__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ModeButton_SetInterruptMode() function.
     *  @{
     */
        #define ModeButton_INTR_NONE      (uint16)(0x0000u)
        #define ModeButton_INTR_RISING    (uint16)(0x0001u)
        #define ModeButton_INTR_FALLING   (uint16)(0x0002u)
        #define ModeButton_INTR_BOTH      (uint16)(0x0003u)
    /** @} intrMode */
/** @} group_constants */

    #define ModeButton_INTR_MASK      (0x01u)
#endif /* (ModeButton__INTSTAT) */


/***************************************
*             Registers
***************************************/

/* Main Port Registers */
/* Pin State */
#define ModeButton_PS                     (* (reg8 *) ModeButton__PS)
/* Data Register */
#define ModeButton_DR                     (* (reg8 *) ModeButton__DR)
/* Port Number */
#define ModeButton_PRT_NUM                (* (reg8 *) ModeButton__PRT)
/* Connect to Analog Globals */
#define ModeButton_AG                     (* (reg8 *) ModeButton__AG)
/* Analog MUX bux enable */
#define ModeButton_AMUX                   (* (reg8 *) ModeButton__AMUX)
/* Bidirectional Enable */
#define ModeButton_BIE                    (* (reg8 *) ModeButton__BIE)
/* Bit-mask for Aliased Register Access */
#define ModeButton_BIT_MASK               (* (reg8 *) ModeButton__BIT_MASK)
/* Bypass Enable */
#define ModeButton_BYP                    (* (reg8 *) ModeButton__BYP)
/* Port wide control signals */
#define ModeButton_CTL                    (* (reg8 *) ModeButton__CTL)
/* Drive Modes */
#define ModeButton_DM0                    (* (reg8 *) ModeButton__DM0)
#define ModeButton_DM1                    (* (reg8 *) ModeButton__DM1)
#define ModeButton_DM2                    (* (reg8 *) ModeButton__DM2)
/* Input Buffer Disable Override */
#define ModeButton_INP_DIS                (* (reg8 *) ModeButton__INP_DIS)
/* LCD Common or Segment Drive */
#define ModeButton_LCD_COM_SEG            (* (reg8 *) ModeButton__LCD_COM_SEG)
/* Enable Segment LCD */
#define ModeButton_LCD_EN                 (* (reg8 *) ModeButton__LCD_EN)
/* Slew Rate Control */
#define ModeButton_SLW                    (* (reg8 *) ModeButton__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define ModeButton_PRTDSI__CAPS_SEL       (* (reg8 *) ModeButton__PRTDSI__CAPS_SEL)
/* Double Sync Enable */
#define ModeButton_PRTDSI__DBL_SYNC_IN    (* (reg8 *) ModeButton__PRTDSI__DBL_SYNC_IN)
/* Output Enable Select Drive Strength */
#define ModeButton_PRTDSI__OE_SEL0        (* (reg8 *) ModeButton__PRTDSI__OE_SEL0)
#define ModeButton_PRTDSI__OE_SEL1        (* (reg8 *) ModeButton__PRTDSI__OE_SEL1)
/* Port Pin Output Select Registers */
#define ModeButton_PRTDSI__OUT_SEL0       (* (reg8 *) ModeButton__PRTDSI__OUT_SEL0)
#define ModeButton_PRTDSI__OUT_SEL1       (* (reg8 *) ModeButton__PRTDSI__OUT_SEL1)
/* Sync Output Enable Registers */
#define ModeButton_PRTDSI__SYNC_OUT       (* (reg8 *) ModeButton__PRTDSI__SYNC_OUT)

/* SIO registers */
#if defined(ModeButton__SIO_CFG)
    #define ModeButton_SIO_HYST_EN        (* (reg8 *) ModeButton__SIO_HYST_EN)
    #define ModeButton_SIO_REG_HIFREQ     (* (reg8 *) ModeButton__SIO_REG_HIFREQ)
    #define ModeButton_SIO_CFG            (* (reg8 *) ModeButton__SIO_CFG)
    #define ModeButton_SIO_DIFF           (* (reg8 *) ModeButton__SIO_DIFF)
#endif /* (ModeButton__SIO_CFG) */

/* Interrupt Registers */
#if defined(ModeButton__INTSTAT)
    #define ModeButton_INTSTAT            (* (reg8 *) ModeButton__INTSTAT)
    #define ModeButton_SNAP               (* (reg8 *) ModeButton__SNAP)

	#define ModeButton_0_INTTYPE_REG 		(* (reg8 *) ModeButton__0__INTTYPE)
#endif /* (ModeButton__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_ModeButton_H */


/* [] END OF FILE */
