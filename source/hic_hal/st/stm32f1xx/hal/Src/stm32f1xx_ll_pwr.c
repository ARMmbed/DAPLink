/**
  ******************************************************************************
  * @file    stm32f1xx_ll_pwr.c
  * @author  MCD Application Team
  * @brief   PWR LL module driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#if defined(USE_FULL_LL_DRIVER)

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_bus.h"

/** @addtogroup STM32F1xx_LL_Driver
  * @{
  */

#if defined(PWR)

/** @defgroup PWR_LL PWR
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup PWR_LL_Exported_Functions
  * @{
  */

/** @addtogroup PWR_LL_EF_Init
  * @{
  */

/**
  * @brief  De-initialize the PWR registers to their default reset values.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: PWR registers are de-initialized
  *          - ERROR: not applicable
  */
ErrorStatus LL_PWR_DeInit(void)
{
  /* Force reset of PWR clock */
  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_PWR);

  /* Release reset of PWR clock */
  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_PWR);

  return SUCCESS;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* defined(PWR) */
/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
