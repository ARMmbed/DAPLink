/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    system_stm32f3xx.h
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M4 Device Peripheral Access Layer System Source File.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
 ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTEM_STM32F3XX_H
#define __SYSTEM_STM32F3XX_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup STM32F30x_System_Exported_types
  * @{
  */

extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */
extern const uint8_t  AHBPrescTable[16];  /*!< AHB prescalers table values */
extern const uint8_t  APBPrescTable[8];   /*!< APB prescalers table values */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/** @addtogroup STM32F3xx_System_Exported_Functions
  * @{
  */  
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
/**

/* Exported functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_STM32F3XX_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
