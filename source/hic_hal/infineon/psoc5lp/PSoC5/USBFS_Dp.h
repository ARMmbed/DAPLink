/*******************************************************************************
* File Name: USBFS_Dp.h
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

#if !defined(CY_PINS_USBFS_Dp_H) /* Pins USBFS_Dp_H */
#define CY_PINS_USBFS_Dp_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "USBFS_Dp_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 USBFS_Dp__PORT == 15 && ((USBFS_Dp__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes
***************************************/

/**
* \addtogroup group_general
* @{
*/
void    USBFS_Dp_Write(uint8 value);
void    USBFS_Dp_SetDriveMode(uint8 mode);
uint8   USBFS_Dp_ReadDataReg(void);
uint8   USBFS_Dp_Read(void);
void    USBFS_Dp_SetInterruptMode(uint16 position, uint16 mode);
uint8   USBFS_Dp_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the USBFS_Dp_SetDriveMode() function.
     *  @{
     */
        #define USBFS_Dp_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define USBFS_Dp_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define USBFS_Dp_DM_RES_UP          PIN_DM_RES_UP
        #define USBFS_Dp_DM_RES_DWN         PIN_DM_RES_DWN
        #define USBFS_Dp_DM_OD_LO           PIN_DM_OD_LO
        #define USBFS_Dp_DM_OD_HI           PIN_DM_OD_HI
        #define USBFS_Dp_DM_STRONG          PIN_DM_STRONG
        #define USBFS_Dp_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */

/* Digital Port Constants */
#define USBFS_Dp_MASK               USBFS_Dp__MASK
#define USBFS_Dp_SHIFT              USBFS_Dp__SHIFT
#define USBFS_Dp_WIDTH              1u

/* Interrupt constants */
#if defined(USBFS_Dp__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in USBFS_Dp_SetInterruptMode() function.
     *  @{
     */
        #define USBFS_Dp_INTR_NONE      (uint16)(0x0000u)
        #define USBFS_Dp_INTR_RISING    (uint16)(0x0001u)
        #define USBFS_Dp_INTR_FALLING   (uint16)(0x0002u)
        #define USBFS_Dp_INTR_BOTH      (uint16)(0x0003u)
    /** @} intrMode */
/** @} group_constants */

    #define USBFS_Dp_INTR_MASK      (0x01u)
#endif /* (USBFS_Dp__INTSTAT) */


/***************************************
*             Registers
***************************************/

/* Main Port Registers */
/* Pin State */
#define USBFS_Dp_PS                     (* (reg8 *) USBFS_Dp__PS)
/* Data Register */
#define USBFS_Dp_DR                     (* (reg8 *) USBFS_Dp__DR)
/* Port Number */
#define USBFS_Dp_PRT_NUM                (* (reg8 *) USBFS_Dp__PRT)
/* Connect to Analog Globals */
#define USBFS_Dp_AG                     (* (reg8 *) USBFS_Dp__AG)
/* Analog MUX bux enable */
#define USBFS_Dp_AMUX                   (* (reg8 *) USBFS_Dp__AMUX)
/* Bidirectional Enable */
#define USBFS_Dp_BIE                    (* (reg8 *) USBFS_Dp__BIE)
/* Bit-mask for Aliased Register Access */
#define USBFS_Dp_BIT_MASK               (* (reg8 *) USBFS_Dp__BIT_MASK)
/* Bypass Enable */
#define USBFS_Dp_BYP                    (* (reg8 *) USBFS_Dp__BYP)
/* Port wide control signals */
#define USBFS_Dp_CTL                    (* (reg8 *) USBFS_Dp__CTL)
/* Drive Modes */
#define USBFS_Dp_DM0                    (* (reg8 *) USBFS_Dp__DM0)
#define USBFS_Dp_DM1                    (* (reg8 *) USBFS_Dp__DM1)
#define USBFS_Dp_DM2                    (* (reg8 *) USBFS_Dp__DM2)
/* Input Buffer Disable Override */
#define USBFS_Dp_INP_DIS                (* (reg8 *) USBFS_Dp__INP_DIS)
/* LCD Common or Segment Drive */
#define USBFS_Dp_LCD_COM_SEG            (* (reg8 *) USBFS_Dp__LCD_COM_SEG)
/* Enable Segment LCD */
#define USBFS_Dp_LCD_EN                 (* (reg8 *) USBFS_Dp__LCD_EN)
/* Slew Rate Control */
#define USBFS_Dp_SLW                    (* (reg8 *) USBFS_Dp__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define USBFS_Dp_PRTDSI__CAPS_SEL       (* (reg8 *) USBFS_Dp__PRTDSI__CAPS_SEL)
/* Double Sync Enable */
#define USBFS_Dp_PRTDSI__DBL_SYNC_IN    (* (reg8 *) USBFS_Dp__PRTDSI__DBL_SYNC_IN)
/* Output Enable Select Drive Strength */
#define USBFS_Dp_PRTDSI__OE_SEL0        (* (reg8 *) USBFS_Dp__PRTDSI__OE_SEL0)
#define USBFS_Dp_PRTDSI__OE_SEL1        (* (reg8 *) USBFS_Dp__PRTDSI__OE_SEL1)
/* Port Pin Output Select Registers */
#define USBFS_Dp_PRTDSI__OUT_SEL0       (* (reg8 *) USBFS_Dp__PRTDSI__OUT_SEL0)
#define USBFS_Dp_PRTDSI__OUT_SEL1       (* (reg8 *) USBFS_Dp__PRTDSI__OUT_SEL1)
/* Sync Output Enable Registers */
#define USBFS_Dp_PRTDSI__SYNC_OUT       (* (reg8 *) USBFS_Dp__PRTDSI__SYNC_OUT)

/* SIO registers */
#if defined(USBFS_Dp__SIO_CFG)
    #define USBFS_Dp_SIO_HYST_EN        (* (reg8 *) USBFS_Dp__SIO_HYST_EN)
    #define USBFS_Dp_SIO_REG_HIFREQ     (* (reg8 *) USBFS_Dp__SIO_REG_HIFREQ)
    #define USBFS_Dp_SIO_CFG            (* (reg8 *) USBFS_Dp__SIO_CFG)
    #define USBFS_Dp_SIO_DIFF           (* (reg8 *) USBFS_Dp__SIO_DIFF)
#endif /* (USBFS_Dp__SIO_CFG) */

/* Interrupt Registers */
#if defined(USBFS_Dp__INTSTAT)
    #define USBFS_Dp_INTSTAT            (* (reg8 *) USBFS_Dp__INTSTAT)
    #define USBFS_Dp_SNAP               (* (reg8 *) USBFS_Dp__SNAP)

	#define USBFS_Dp_0_INTTYPE_REG 		(* (reg8 *) USBFS_Dp__0__INTTYPE)
#endif /* (USBFS_Dp__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_USBFS_Dp_H */


/* [] END OF FILE */
