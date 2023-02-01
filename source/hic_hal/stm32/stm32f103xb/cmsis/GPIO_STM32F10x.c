/* -----------------------------------------------------------------------------
 * Copyright (c) 2016 Arm Limited (or its affiliates). All 
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
 * $Date:        26. April 2016
 * $Revision:    V1.3
 *
 * Project:      GPIO Driver for ST STM32F10x
 * -------------------------------------------------------------------- */

#include "GPIO_STM32F10x.h"

/* History:
 *  Version 1.3
 *    Corrected corruption of Serial wire JTAG pins alternate function configuration.
 *  Version 1.2
 *    Added GPIO_GetPortClockState function
 *    GPIO_PinConfigure function enables GPIO peripheral clock, if not already enabled
 *  Version 1.1
 *    GPIO_PortClock and GPIO_PinConfigure functions rewritten
 *    GPIO_STM32F1xx header file cleaned up and simplified
 *    GPIO_AFConfigure function and accompanying definitions added
 *  Version 1.0
 *    Initial release
 */

/* Serial wire JTAG pins alternate function Configuration
 * Can be user defined by C preprocessor:
 *   AFIO_SWJ_FULL, AFIO_SWJ_FULL_NO_NJTRST, AFIO_SWJ_JTAG_NO_SW, AFIO_SWJ_NO_JTAG_NO_SW
 */
#ifndef AFIO_MAPR_SWJ_CFG_VAL
#define AFIO_MAPR_SWJ_CFG_VAL   (AFIO_SWJ_FULL)    // Full SWJ (JTAG-DP + SW-DP)
#endif

/**
  \fn          void GPIO_PortClock (GPIO_TypeDef *GPIOx, bool en)
  \brief       Port Clock Control
  \param[in]   GPIOx  Pointer to GPIO peripheral
  \param[in]   enable Enable or disable clock
*/
void GPIO_PortClock (GPIO_TypeDef *GPIOx, bool enable) {

  if (enable) {
    if      (GPIOx == GPIOA) RCC->APB2ENR |=  (1U << 2);
    else if (GPIOx == GPIOB) RCC->APB2ENR |=  (1U << 3);
    else if (GPIOx == GPIOC) RCC->APB2ENR |=  (1U << 4);
    else if (GPIOx == GPIOD) RCC->APB2ENR |=  (1U << 5);
    else if (GPIOx == GPIOE) RCC->APB2ENR |=  (1U << 6);
    else if (GPIOx == GPIOF) RCC->APB2ENR |=  (1U << 7);
    else if (GPIOx == GPIOG) RCC->APB2ENR |=  (1U << 8);
  } else {
    if      (GPIOx == GPIOA) RCC->APB2ENR &= ~(1U << 2);
    else if (GPIOx == GPIOB) RCC->APB2ENR &= ~(1U << 3);
    else if (GPIOx == GPIOC) RCC->APB2ENR &= ~(1U << 4);
    else if (GPIOx == GPIOD) RCC->APB2ENR &= ~(1U << 5);
    else if (GPIOx == GPIOE) RCC->APB2ENR &= ~(1U << 6);
    else if (GPIOx == GPIOF) RCC->APB2ENR &= ~(1U << 7);
    else if (GPIOx == GPIOG) RCC->APB2ENR &= ~(1U << 8);
  }
}

/**
  \fn          bool GPIO_GetPortClockState (GPIO_TypeDef *GPIOx)
  \brief       Get GPIO port clock state
  \param[in]   GPIOx  Pointer to GPIO peripheral
  \return      true  - enabled
               false - disabled
*/
bool GPIO_GetPortClockState (GPIO_TypeDef *GPIOx) {

  if      (GPIOx == GPIOA) { return ((RCC->APB2ENR &  (1U << 2)) != 0U); }
  else if (GPIOx == GPIOB) { return ((RCC->APB2ENR &  (1U << 3)) != 0U); }
  else if (GPIOx == GPIOC) { return ((RCC->APB2ENR &  (1U << 4)) != 0U); }
  else if (GPIOx == GPIOD) { return ((RCC->APB2ENR &  (1U << 5)) != 0U); }
  else if (GPIOx == GPIOE) { return ((RCC->APB2ENR &  (1U << 6)) != 0U); }
  else if (GPIOx == GPIOF) { return ((RCC->APB2ENR &  (1U << 7)) != 0U); }
  else if (GPIOx == GPIOG) { return ((RCC->APB2ENR &  (1U << 8)) != 0U); }

  return false; 
}


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
bool GPIO_PinConfigure(GPIO_TypeDef *GPIOx, uint32_t num, GPIO_CONF conf,
                                                          GPIO_MODE mode) {
  __IO uint32_t *reg;
  if (num > 15) return false;

  if (GPIO_GetPortClockState(GPIOx) == false) {
    /* Enable GPIOx peripheral clock */
    GPIO_PortClock (GPIOx, true);
  } 

  if (mode == GPIO_MODE_INPUT) {
    if      (conf == GPIO_IN_PULL_DOWN) {
      /* Enable pull down on selected input */
      GPIOx->ODR &= ~(1 << num);
    }
    else if (conf == GPIO_IN_PULL_UP) {
      /* Enable pull up on selected input */
      conf &= ~1;
      GPIOx->ODR |=  (1 << num);
    }
  }

  if (num < 8) {
    reg = &GPIOx->CRL;
  }
  else {
    num -= 8;
    reg = &GPIOx->CRH;
  }
  *reg &= ~(0xF << (num << 2));
  *reg |= ((conf << 2) | mode) << (num << 2);
  return true;
}


/**
  \fn          void GPIO_AFConfigure (AFIO_REMAP af_type)
  \brief       Configure alternate functions
  \param[in]   af_type Alternate function remap type
*/
void GPIO_AFConfigure (AFIO_REMAP af_type) {
           uint32_t msk, val;
  volatile uint32_t mapr;

  if (af_type != AFIO_UNAVAILABLE_REMAP) {
    msk = (af_type >> 5) & 0x07;
    val = (af_type >> 8) & 0x0F;

    if (!(RCC->APB2ENR & RCC_APB2ENR_AFIOEN)) {
      /* Enable AFIO peripheral clock */
      RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    }
    if (af_type & (1 << 12)) {
      /* AF remap and debug I/O config register 2 */
      mapr  = AFIO->MAPR2;
      mapr &= ~(msk << (af_type & 0x1F));
      mapr |=  (val << (af_type & 0x1F));
      AFIO->MAPR2 = mapr;
    }
    else {
      /* AF remap and debug I/O config register */
      mapr  = AFIO->MAPR;
      mapr &= ~(msk << (af_type & 0x1F));
      mapr |=  (val << (af_type & 0x1F));

      /* Serial wire JTAG configuration */
      msk  =   (AFIO_MAPR_SWJ_CFG_VAL >> 5) & 0x07;
      val  =   (AFIO_MAPR_SWJ_CFG_VAL >> 8) & 0x0F;
      mapr &= ~(msk << (AFIO_MAPR_SWJ_CFG_VAL & 0x1F));
      mapr |=  (val << (AFIO_MAPR_SWJ_CFG_VAL & 0x1F));

      AFIO->MAPR = mapr;
    }
  }
}
