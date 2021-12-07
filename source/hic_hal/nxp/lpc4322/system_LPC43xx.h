/**********************************************************************
* $Id$		system_LPC43xx.h			2011-06-02
*//**
* @file		system_LPC43xx.h
* @brief	Cortex-M3 Device System Header File for NXP lpc43xx Series.
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team, modified by KEIL
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/

#ifndef __SYSTEM_lpc43xx_H
#define __SYSTEM_lpc43xx_H

#ifdef __cplusplus
extern "C" {
#endif

/**
  \brief  System clock frequency (core clock).
    The system clock frequency supplied to the SysTick timer and the 
    processor core clock.
 */
extern uint32_t SystemCoreClock;

/**
  \brief  Setup the microcontroller system.

    Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);

/**
  \brief  Update SystemCoreClock variable.

    Updates the SystemCoreClock with current core Clock
    retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_lpc43xx_H */
