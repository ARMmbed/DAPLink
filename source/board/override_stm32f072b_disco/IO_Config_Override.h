/**
 * @file    IO_Config_Override.c
 * @brief   Alternative IO for STM32F02CB based Hardware Interface Circuit
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2021, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
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
 */

#ifndef __IO_CONFIG_H__
#define __IO_CONFIG_H__

#include "stm32f0xx.h"
#include "compiler.h"
#include "daplink.h"

// This GPIO configuration is only valid for the STM32F072CB HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_STM32F072CB);

#define __HAL_RCC_USB_GPIO_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE

/*
 * - #RESET (PA1  / Pin 11) - 
 * - SWCLK  (PA2  / Pin 12) = PA5
 * - SWO    (PA3  / Pin 13) = PA15
 * - SWDIO  (PA4  / Pin 14) -
 * - TXD    (PA9  / Pin 30) -
 * - RXD    (PA10 / Pin 31) -
 * - CTS    (PA7  / Pin 17) X
 * - RTS    (PA6  / Pin 16) X 
 */

// LED_RED    / LD3 : PC6
// LED_ORANGE / LD4 : PC8
// LED_GREEN  / LD5 : PC9
// LED_BLUE   / LD6 : PC7

#define USER_BUTTON_PIN                GPIO_PIN_0                       /* PA0 */
#define USER_BUTTON_GPIO_PORT          GPIOA
#define USER_BUTTON_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define USER_BUTTON_EXTI_IRQn          EXTI0_1_IRQn

#define LED3_PIN                         GPIO_PIN_6
#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOC_CLK_DISABLE()

#define LED4_PIN                         GPIO_PIN_8
#define LED4_GPIO_PORT                   GPIOC
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOC_CLK_DISABLE()

#define LED5_PIN                         GPIO_PIN_9
#define LED5_GPIO_PORT                   GPIOC
#define LED5_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED5_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOC_CLK_DISABLE()

#define LED6_PIN                         GPIO_PIN_7
#define LED6_GPIO_PORT                   GPIOC
#define LED6_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED6_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOC_CLK_DISABLE()

//==============================================================================
// Debug Port I/O Pins
//==============================================================================
/* led pin: PA0 */
#define HID_LED_PORT        GPIOA
#define HID_LED_PIN         GPIO_PIN_0

#define CDC_LED_PORT        GPIOA
#define CDC_LED_PIN         GPIO_PIN_0

#define MSC_LED_PORT        GPIOA
#define MSC_LED_PIN         GPIO_PIN_0

#define __HAL_RCC_LED_GPIO_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE

#define LED_CONNECTED_PORT  GPIOA
#define LED_CONNECTED_PIN   GPIO_PIN_0
#define LED_CONNECTED_OUT(__bit)                          \
    do                                                    \
    {                                                     \
        if (__bit)                                        \
            LED_CONNECTED_PORT->BRR = LED_CONNECTED_PIN;  \
        else                                              \
            LED_CONNECTED_PORT->BSRR = LED_CONNECTED_PIN; \
    } while (0)

#define LED_RUNNING_PORT    GPIOA
#define LED_RUNNING_PIN     GPIO_PIN_0
#define LED_RUNNING_OUT(__bit)                        \
    do                                                \
    {                                                 \
        if (__bit)                                    \
            LED_RUNNING_PORT->BRR = LED_RUNNING_PIN;  \
        else                                          \
            LED_RUNNING_PORT->BSRR = LED_RUNNING_PIN; \
    } while (0)

/* SWD_DAT pin: PA4 */
#define SWDIO_TMS_PORT      GPIOA
#define SWDIO_TMS_PIN       GPIO_PIN_4

/* SWD_CLK pin: PA2 */
#define SWCLK_TCK_PORT      GPIOA
#define SWCLK_TCK_PIN       GPIO_PIN_2

/* SWD_NRST pin: PA1 */
#define nRESET_PORT         GPIOA
#define nRESET_PIN          GPIO_PIN_1

/* SWD_SWO pin: PA3 */
#define SWSWO_PORT          GPIOA
#define SWSWO_PIN           GPIO_PIN_3

/* SWD_SWO pin: PA3 */
#define SWSWO_PORT          GPIOA
#define SWSWO_PIN           GPIO_PIN_3

#define __HAL_RCC_DAP_GPIO_CLK_ENABLE() \
    do                                  \
    {                                   \
        __HAL_RCC_GPIOA_CLK_ENABLE();   \
        __HAL_RCC_GPIOC_CLK_ENABLE();   \
    } while (0)

//==============================================================================
// GOBtl Button GPIO Pin
//==============================================================================
#define __HAL_RCC_BTN_BTL_CLK_ENABLE    __HAL_RCC_GPIOB_CLK_ENABLE
#define __HAL_RCC_BTN_BTL_CLK_DISABLE   __HAL_RCC_GPIOB_CLK_DISABLE
#define BTN_BTL_PORT        GPIOB
#define BTN_BTL_PIN         GPIO_PIN_6

//==============================================================================
// USART Port I/O Pins
//==============================================================================
#define USART_PORT			USART1
#define USART_GPIO          GPIOA
#define USART_RTS_PIN       GPIO_PIN_6
#define USART_CTS_PIN       GPIO_PIN_7
#define USART_DTR_PIN       GPIO_PIN_8
#define USART_TX_PIN		GPIO_PIN_9
#define USART_RX_PIN		GPIO_PIN_10
#define USART_IRQn			USART1_IRQn

#endif
