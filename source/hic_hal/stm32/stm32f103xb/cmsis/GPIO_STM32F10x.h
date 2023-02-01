/* -----------------------------------------------------------------------------
 * Copyright (c) 2013 Arm Limited (or its affiliates). All 
 * rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        26. August 2013
 * $Revision:    V1.2
 *
 * Project:      GPIO Driver definitions for ST STM32F10x
 * -------------------------------------------------------------------- */

#ifndef __GPIO_STM32F10X_H
#define __GPIO_STM32F10X_H

#include <stdbool.h>
#include "stm32f10x.h"


#if defined (__CC_ARM)
  #define __FORCE_INLINE  static __forceinline
#else
  #define __FORCE_INLINE  __STATIC_INLINE
#endif

/// GPIO Pin identifier
typedef struct _GPIO_PIN_ID {
  GPIO_TypeDef *port;
  uint8_t       num;
} GPIO_PIN_ID;

/// Port Mode
typedef enum {
  GPIO_MODE_INPUT     = 0x00,              /// GPIO is input
  GPIO_MODE_OUT10MHZ  = 0x01,              /// Max output Speed 10MHz
  GPIO_MODE_OUT2MHZ   = 0x02,              /// Max output Speed  2MHz
  GPIO_MODE_OUT50MHZ  = 0x03               /// Max output Speed 50MHz
} GPIO_MODE;

/// Port Conf
typedef enum {
  GPIO_OUT_PUSH_PULL  = 0x00,               /// general purpose output push-pull
  GPIO_OUT_OPENDRAIN  = 0x01,               /// general purpose output open-drain
  GPIO_AF_PUSHPULL    = 0x02,               /// alternate function push-pull
  GPIO_AF_OPENDRAIN   = 0x03,               /// alternate function open-drain
  GPIO_IN_ANALOG      = 0x00,               /// input analog
  GPIO_IN_FLOATING    = 0x01,               /// input floating
  GPIO_IN_PULL_DOWN   = 0x02,               /// alternate function push-pull
  GPIO_IN_PULL_UP     = 0x03                /// alternate function pull up
} GPIO_CONF;

/* Alternate function definition macro */
#define AFIO_FUNC_DEF(bit, mask, val, reg) ((bit) | (mask << 5) | (val << 8) | (reg << 12))

/// Alternate function I/O remap
typedef enum {
  /* AF remap and debug I/O config register */
  AFIO_SPI1_NO_REMAP         = AFIO_FUNC_DEF (0,  1, 0, 0),
  AFIO_SPI1_REMAP            = AFIO_FUNC_DEF (0,  1, 1, 0),
  AFIO_I2C1_NO_REMAP         = AFIO_FUNC_DEF (1,  1, 0, 0),
  AFIO_I2C1_REMAP            = AFIO_FUNC_DEF (1,  1, 1, 0),
  AFIO_USART1_NO_REMAP       = AFIO_FUNC_DEF (2,  1, 0, 0),
  AFIO_USART1_REMAP          = AFIO_FUNC_DEF (2,  1, 1, 0),
  AFIO_USART2_NO_REMAP       = AFIO_FUNC_DEF (3,  1, 0, 0),
  AFIO_USART2_REMAP          = AFIO_FUNC_DEF (3,  1, 1, 0),
  AFIO_USART3_NO_REMAP       = AFIO_FUNC_DEF (4,  3, 0, 0),
  AFIO_USART3_REMAP_PARTIAL  = AFIO_FUNC_DEF (4,  3, 1, 0),
  AFIO_USART3_REMAP_FULL     = AFIO_FUNC_DEF (4,  3, 3, 0),
  AFIO_TIM1_NO_REMAP         = AFIO_FUNC_DEF (6,  3, 0, 0),
  AFIO_TIM1_REMAP_PARTIAL    = AFIO_FUNC_DEF (6,  3, 1, 0),
  AFIO_TIM1_REMAP_FULL       = AFIO_FUNC_DEF (6,  3, 3, 0),
  AFIO_TIM2_NO_REMAP         = AFIO_FUNC_DEF (8,  3, 0, 0),
  AFIO_TIM2_REMAP_PARTIAL_1  = AFIO_FUNC_DEF (8,  3, 1, 0),
  AFIO_TIM2_REMAP_PARTIAL_2  = AFIO_FUNC_DEF (8,  3, 2, 0),
  AFIO_TIM2_REMAP_FULL       = AFIO_FUNC_DEF (8,  3, 3, 0),
  AFIO_TIM3_NO_REMAP         = AFIO_FUNC_DEF (10, 3, 0, 0),
  AFIO_TIM3_REMAP_PARTIAL    = AFIO_FUNC_DEF (10, 3, 2, 0),
  AFIO_TIM3_REMAP_FULL       = AFIO_FUNC_DEF (10, 3, 3, 0),
  AFIO_TIM4_NO_REMAP         = AFIO_FUNC_DEF (12, 1, 0, 0),
  AFIO_TIM4_REMAP            = AFIO_FUNC_DEF (12, 1, 1, 0),
  AFIO_CAN_PA11_PA12         = AFIO_FUNC_DEF (13, 3, 0, 0),
  AFIO_CAN_PB8_PB9           = AFIO_FUNC_DEF (13, 3, 2, 0),
  AFIO_CAN_PD0_PD1           = AFIO_FUNC_DEF (13, 3, 3, 0),
  AFIO_PD01_NO_REMAP         = AFIO_FUNC_DEF (15, 1, 0, 0),
  AFIO_PD01_REMAP            = AFIO_FUNC_DEF (15, 1, 1, 0),
  AFIO_TIM5CH4_NO_REMAP      = AFIO_FUNC_DEF (16, 1, 0, 0),
  AFIO_TIM5CH4_REMAP         = AFIO_FUNC_DEF (16, 1, 1, 0),
  AFIO_ADC1_ETRGINJ_NO_REMAP = AFIO_FUNC_DEF (17, 1, 0, 0),
  AFIO_ADC1_ETRGINJ_REMAP    = AFIO_FUNC_DEF (17, 1, 1, 0),
  AFIO_ADC1_ETRGREG_NO_REMAP = AFIO_FUNC_DEF (18, 1, 0, 0),
  AFIO_ADC1_ETRGREG_REMAP    = AFIO_FUNC_DEF (18, 1, 1, 0),
  AFIO_ADC2_ETRGINJ_NO_REMAP = AFIO_FUNC_DEF (19, 1, 0, 0),
  AFIO_ADC2_ETRGINJ_REMAP    = AFIO_FUNC_DEF (19, 1, 1, 0),
  AFIO_ADC2_ETRGREG_NO_REMAP = AFIO_FUNC_DEF (20, 1, 0, 0),
  AFIO_ADC2_ETRGREG_REMAP    = AFIO_FUNC_DEF (20, 1, 1, 0),
#if defined(STM32F10X_CL)
  AFIO_ETH_NO_REMAP          = AFIO_FUNC_DEF (21, 1, 0, 0),
  AFIO_ETH_REMAP             = AFIO_FUNC_DEF (21, 1, 1, 0),
  AFIO_CAN2_NO_REMAP         = AFIO_FUNC_DEF (22, 1, 0, 0),
  AFIO_CAN2_REMAP            = AFIO_FUNC_DEF (22, 1, 1, 0),
  AFIO_ETH_MII_SEL           = AFIO_FUNC_DEF (23, 1, 0, 0),
  AFIO_ETH_RMII_SEL          = AFIO_FUNC_DEF (23, 1, 1, 0),
#endif
  AFIO_SWJ_FULL              = AFIO_FUNC_DEF (24, 7, 0, 0),
  AFIO_SWJ_FULL_NO_NJTRST    = AFIO_FUNC_DEF (24, 7, 1, 0),
  AFIO_SWJ_JTAG_NO_SW        = AFIO_FUNC_DEF (24, 7, 2, 0),
  AFIO_SWJ_NO_JTAG_NO_SW     = AFIO_FUNC_DEF (24, 7, 4, 0),
#if defined(STM32F10X_CL)
  AFIO_SPI3_NO_REMAP         = AFIO_FUNC_DEF (28, 1, 0, 0),
  AFIO_SPI3_REMAP            = AFIO_FUNC_DEF (28, 1, 1, 0),
  AFIO_TIM2ITR_NO_REMAP      = AFIO_FUNC_DEF (29, 1, 0, 0),
  AFIO_TIM2ITR_REMAP         = AFIO_FUNC_DEF (29, 1, 1, 0),
  AFIO_PTP_PPS_NO_REMAP      = AFIO_FUNC_DEF (30, 1, 0, 0),
  AFIO_PTP_PPS_REMAP         = AFIO_FUNC_DEF (30, 1, 1, 0),
#endif
  
  /* AF remap and debug I/O config register 2 */
  AFIO_TIM15_NO_REMAP        = AFIO_FUNC_DEF (0,  1, 0, 1),
  AFIO_TIM15_REMAP           = AFIO_FUNC_DEF (0,  1, 1, 1),
  AFIO_TIM16_NO_REMAP        = AFIO_FUNC_DEF (1,  1, 0, 1),
  AFIO_TIM16_REMAP           = AFIO_FUNC_DEF (1,  1, 1, 1),
  AFIO_TIM17_NO_REMAP        = AFIO_FUNC_DEF (2,  1, 0, 1),
  AFIO_TIM17_REMAP           = AFIO_FUNC_DEF (2,  1, 1, 1),
  AFIO_CEC_NO_REMAP          = AFIO_FUNC_DEF (3,  1, 0, 1),
  AFIO_CEC_REMAP             = AFIO_FUNC_DEF (3,  1, 1, 1),
  AFIO_TIM1_DMA_NO_REMAP     = AFIO_FUNC_DEF (4,  1, 0, 1),
  AFIO_TIM1_DMA_REMAP        = AFIO_FUNC_DEF (4,  1, 1, 1),
  
  AFIO_TIM9_NO_REMAP         = AFIO_FUNC_DEF (5,  1, 0, 1),
  AFIO_TIM9_REMAP            = AFIO_FUNC_DEF (5,  1, 1, 1),
  AFIO_TIM10_NO_REMAP        = AFIO_FUNC_DEF (6,  1, 0, 1),
  AFIO_TIM10_REMAP           = AFIO_FUNC_DEF (6,  1, 1, 1),
  AFIO_TIM11_NO_REMAP        = AFIO_FUNC_DEF (7,  1, 0, 1),
  AFIO_TIM11_REMAP           = AFIO_FUNC_DEF (7,  1, 1, 1),
  AFIO_TIM13_NO_REMAP        = AFIO_FUNC_DEF (8,  1, 0, 1),
  AFIO_TIM13_REMAP           = AFIO_FUNC_DEF (8,  1, 0, 1),
  AFIO_TIM14_NO_REMAP        = AFIO_FUNC_DEF (9,  1, 0, 1),
  AFIO_TIM14_REMAP           = AFIO_FUNC_DEF (9,  1, 1, 1),
  AFIO_FSMC_NADV_NO_REMAP    = AFIO_FUNC_DEF (10, 1, 0, 1),
  AFIO_FSMC_NADV_REMAP       = AFIO_FUNC_DEF (10, 1, 1, 1),

  AFIO_TIM67_DAC_DMA_NO_REMAP = AFIO_FUNC_DEF(11, 1, 0, 1),
  AFIO_TIM67_DAC_DMA_REMAP   = AFIO_FUNC_DEF (11, 1, 1, 1),
  AFIO_TIM12_NO_REMAP        = AFIO_FUNC_DEF (12, 1, 0, 1),
  AFIO_TIM12_REMAP           = AFIO_FUNC_DEF (12, 1, 1, 1),
  AFIO_MISC_NO_REMAP         = AFIO_FUNC_DEF (13, 1, 0, 1),
  AFIO_MISC_REMAP            = AFIO_FUNC_DEF (13, 1, 1, 1),

  /* Reserved value */
  AFIO_UNAVAILABLE_REMAP     = AFIO_FUNC_DEF (0,  0, 0, 0)
} AFIO_REMAP;


/**
  \fn          void GPIO_PortClock (GPIO_TypeDef *GPIOx, bool en)
  \brief       Port Clock Control
  \param[in]   GPIOx  Pointer to GPIO peripheral
  \param[in]   enable Enable or disable clock
*/
extern void GPIO_PortClock (GPIO_TypeDef *GPIOx, bool enable);

/**
  \fn          bool GPIO_GetPortClockState (GPIO_TypeDef *GPIOx)
  \brief       Get GPIO port clock state
  \param[in]   GPIOx  Pointer to GPIO peripheral
  \return      true  - enabled
               false - disabled
*/
extern bool GPIO_GetPortClockState (GPIO_TypeDef *GPIOx);

/**
  \fn          bool GPIO_PinConfigure (GPIO_TypeDef      *GPIOx,
                                       uint32_t           num,
                                       GPIO_CONF          conf,
                                       GPIO_MODE          mode)
  \brief       Configure port pin
  \param[in]   GPIOx         Pointer to GPIO peripheral
  \param[in]   num           Port pin number
  \param[in]   mode          \ref GPIO_MODE
  \param[in]   conf          \ref GPIO_CONF
  \return      true  - success
               false - error
*/
bool GPIO_PinConfigure(GPIO_TypeDef      *GPIOx,
                       uint32_t           num,
                       GPIO_CONF          conf,
                       GPIO_MODE          mode);

/**
  \fn          void GPIO_PinWrite (GPIO_TypeDef *GPIOx, uint32_t num, uint32_t val)
  \brief       Write port pin
  \param[in]   GPIOx  Pointer to GPIO peripheral
  \param[in]   num    Port pin number
  \param[in]   val    Port pin value (0 or 1)
*/
__FORCE_INLINE void GPIO_PinWrite (GPIO_TypeDef *GPIOx, uint32_t num, uint32_t val) {
  if (val & 1) {
    GPIOx->BSRR = (1UL << num);         // set
  } else {
    GPIOx->BSRR = (1UL << (num + 16));  // clr
  }
}

/**
  \fn          uint32_t GPIO_PinRead (GPIO_TypeDef *GPIOx, uint32_t num)
  \brief       Read port pin
  \param[in]   GPIOx  Pointer to GPIO peripheral
  \param[in]   num    Port pin number
  \return      pin value (0 or 1)
*/
__FORCE_INLINE uint32_t GPIO_PinRead (GPIO_TypeDef *GPIOx, uint32_t num) {
  return ((GPIOx->IDR >> num) & 1);
}

/**
  \fn          void GPIO_PortWrite (GPIO_TypeDef *GPIOx, uint16_t mask, uint16_t val)
  \brief       Write port pins
  \param[in]   GPIOx  Pointer to GPIO peripheral
  \param[in]   mask   Selected pins
  \param[in]   val    Pin values
*/
__FORCE_INLINE void GPIO_PortWrite (GPIO_TypeDef *GPIOx, uint16_t mask, uint16_t val) {
  GPIOx->ODR = (GPIOx->ODR & ~mask) | val;
}

/**
  \fn          uint16_t GPIO_PortRead (GPIO_TypeDef *GPIOx)
  \brief       Read port pins
  \param[in]   GPIOx  Pointer to GPIO peripheral
  \return      port pin inputs
*/
__FORCE_INLINE uint16_t GPIO_PortRead (GPIO_TypeDef *GPIOx) {
  return (GPIOx->IDR);
}

/**
  \fn          void GPIO_AFConfigure (AFIO_REMAP af_type)
  \brief       Configure alternate functions
  \param[in]   af_type Alternate function remap type
*/
void GPIO_AFConfigure (AFIO_REMAP af_type);


#endif /* __GPIO_STM32F10X_H */
