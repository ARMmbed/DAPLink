/*******************************************************************************
* File Name: SWDIO.h
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

#if !defined(CY_PINS_SWDIO_H) /* Pins SWDIO_H */
#define CY_PINS_SWDIO_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SWDIO_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 SWDIO__PORT == 15 && ((SWDIO__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes
***************************************/

/**
* \addtogroup group_general
* @{
*/
void    SWDIO_Write(uint8 value);
void    SWDIO_SetDriveMode(uint8 mode);
uint8   SWDIO_ReadDataReg(void);
uint8   SWDIO_Read(void);
void    SWDIO_SetInterruptMode(uint16 position, uint16 mode);
uint8   SWDIO_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the SWDIO_SetDriveMode() function.
     *  @{
     */
        #define SWDIO_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define SWDIO_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define SWDIO_DM_RES_UP          PIN_DM_RES_UP
        #define SWDIO_DM_RES_DWN         PIN_DM_RES_DWN
        #define SWDIO_DM_OD_LO           PIN_DM_OD_LO
        #define SWDIO_DM_OD_HI           PIN_DM_OD_HI
        #define SWDIO_DM_STRONG          PIN_DM_STRONG
        #define SWDIO_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */

/* Digital Port Constants */
#define SWDIO_MASK               SWDIO__MASK
#define SWDIO_SHIFT              SWDIO__SHIFT
#define SWDIO_WIDTH              1u

/* Interrupt constants */
#if defined(SWDIO__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SWDIO_SetInterruptMode() function.
     *  @{
     */
        #define SWDIO_INTR_NONE      (uint16)(0x0000u)
        #define SWDIO_INTR_RISING    (uint16)(0x0001u)
        #define SWDIO_INTR_FALLING   (uint16)(0x0002u)
        #define SWDIO_INTR_BOTH      (uint16)(0x0003u)
    /** @} intrMode */
/** @} group_constants */

    #define SWDIO_INTR_MASK      (0x01u)
#endif /* (SWDIO__INTSTAT) */


/***************************************
*             Registers
***************************************/

/* Main Port Registers */
/* Pin State */
#define SWDIO_PS                     (* (reg8 *) SWDIO__PS)
/* Data Register */
#define SWDIO_DR                     (* (reg8 *) SWDIO__DR)
/* Port Number */
#define SWDIO_PRT_NUM                (* (reg8 *) SWDIO__PRT)
/* Connect to Analog Globals */
#define SWDIO_AG                     (* (reg8 *) SWDIO__AG)
/* Analog MUX bux enable */
#define SWDIO_AMUX                   (* (reg8 *) SWDIO__AMUX)
/* Bidirectional Enable */
#define SWDIO_BIE                    (* (reg8 *) SWDIO__BIE)
/* Bit-mask for Aliased Register Access */
#define SWDIO_BIT_MASK               (* (reg8 *) SWDIO__BIT_MASK)
/* Bypass Enable */
#define SWDIO_BYP                    (* (reg8 *) SWDIO__BYP)
/* Port wide control signals */
#define SWDIO_CTL                    (* (reg8 *) SWDIO__CTL)
/* Drive Modes */
#define SWDIO_DM0                    (* (reg8 *) SWDIO__DM0)
#define SWDIO_DM1                    (* (reg8 *) SWDIO__DM1)
#define SWDIO_DM2                    (* (reg8 *) SWDIO__DM2)
/* Input Buffer Disable Override */
#define SWDIO_INP_DIS                (* (reg8 *) SWDIO__INP_DIS)
/* LCD Common or Segment Drive */
#define SWDIO_LCD_COM_SEG            (* (reg8 *) SWDIO__LCD_COM_SEG)
/* Enable Segment LCD */
#define SWDIO_LCD_EN                 (* (reg8 *) SWDIO__LCD_EN)
/* Slew Rate Control */
#define SWDIO_SLW                    (* (reg8 *) SWDIO__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SWDIO_PRTDSI__CAPS_SEL       (* (reg8 *) SWDIO__PRTDSI__CAPS_SEL)
/* Double Sync Enable */
#define SWDIO_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SWDIO__PRTDSI__DBL_SYNC_IN)
/* Output Enable Select Drive Strength */
#define SWDIO_PRTDSI__OE_SEL0        (* (reg8 *) SWDIO__PRTDSI__OE_SEL0)
#define SWDIO_PRTDSI__OE_SEL1        (* (reg8 *) SWDIO__PRTDSI__OE_SEL1)
/* Port Pin Output Select Registers */
#define SWDIO_PRTDSI__OUT_SEL0       (* (reg8 *) SWDIO__PRTDSI__OUT_SEL0)
#define SWDIO_PRTDSI__OUT_SEL1       (* (reg8 *) SWDIO__PRTDSI__OUT_SEL1)
/* Sync Output Enable Registers */
#define SWDIO_PRTDSI__SYNC_OUT       (* (reg8 *) SWDIO__PRTDSI__SYNC_OUT)

/* SIO registers */
#if defined(SWDIO__SIO_CFG)
    #define SWDIO_SIO_HYST_EN        (* (reg8 *) SWDIO__SIO_HYST_EN)
    #define SWDIO_SIO_REG_HIFREQ     (* (reg8 *) SWDIO__SIO_REG_HIFREQ)
    #define SWDIO_SIO_CFG            (* (reg8 *) SWDIO__SIO_CFG)
    #define SWDIO_SIO_DIFF           (* (reg8 *) SWDIO__SIO_DIFF)
#endif /* (SWDIO__SIO_CFG) */

/* Interrupt Registers */
#if defined(SWDIO__INTSTAT)
    #define SWDIO_INTSTAT            (* (reg8 *) SWDIO__INTSTAT)
    #define SWDIO_SNAP               (* (reg8 *) SWDIO__SNAP)

	#define SWDIO_0_INTTYPE_REG 		(* (reg8 *) SWDIO__0__INTTYPE)
#endif /* (SWDIO__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_SWDIO_H */


/* [] END OF FILE */
