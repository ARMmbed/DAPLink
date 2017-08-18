/**
 * @file    IO_Config.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

// Override all defines if IO_CONFIG_OVERRIDE is defined
#ifdef IO_CONFIG_OVERRIDE
#include "IO_Config_Override.h"
#ifndef __IO_CONFIG_H__
#define __IO_CONFIG_H__
#endif
#endif


#ifndef __IO_CONFIG_H__
#define __IO_CONFIG_H__

#include "stm32f10x.h"
#include "compiler.h"
#include "daplink.h"

// This GPIO configuration is only valid for the STLINK_V2_1 or STLINK_V2_1_NATIVE HIC
COMPILER_ASSERT((DAPLINK_HIC_ID == DAPLINK_HIC_ID_STLINK_V2_1) ||
                (DAPLINK_HIC_ID == DAPLINK_HIC_ID_STLINK_V2_1_NATIVE));

// USB pullup control
#define USB_CONNECT_PORT_ENABLE()    (RCC->APB2ENR |= RCC_APB2Periph_GPIOA)
#define USB_CONNECT_PORT_DISABLE()   (RCC->APB2ENR &= ~RCC_APB2Periph_GPIOA)
#define USB_CONNECT_PORT             GPIOA
#define USB_CONNECT_PIN              GPIO_Pin_15
#define USB_CONNECT_ON()             (USB_CONNECT_PORT->BSRR = USB_CONNECT_PIN)
#define USB_CONNECT_OFF()            (USB_CONNECT_PORT->BRR  = USB_CONNECT_PIN)

// Output clock for the target microcontroller
#define MCO_PIN_PORT                 GPIOA
#define MCO_PIN                      GPIO_Pin_8

// Target power control pin
#define POWER_EN_PIN_PORT            GPIOB
#define POWER_EN_PIN                 GPIO_Pin_15
#define POWER_EN_Bit                 15
#define POWER_EN_PIN_ACTIVE_HIGH     0
#define POWER_DOWN_DURING_BOOTLOADER 0

// Reset pin - enters the bootloader if held at power up
// Resets the target when the interface is running
#define nRESET_PIN_PORT              GPIOB
#define nRESET_PIN                   GPIO_Pin_0
#define nRESET_PIN_Bit               0

// SWD
#define SWCLK_TCK_PIN_PORT           GPIOB
#define SWCLK_TCK_PIN                GPIO_Pin_13
#define SWCLK_TCK_PIN_Bit            13

#define SWDIO_OUT_PIN_PORT           GPIOB
#define SWDIO_OUT_PIN                GPIO_Pin_14
#define SWDIO_OUT_PIN_Bit            14

#define SWDIO_IN_PIN_PORT            GPIOB
#define SWDIO_IN_PIN                 GPIO_Pin_12
#define SWDIO_IN_PIN_Bit             12

//LEDs
//USB status LED
#define RUNNING_LED_PORT             GPIOA
#define RUNNING_LED_PIN              GPIO_Pin_9
#define RUNNING_LED_Bit              9

// For usart
#define CDC_UART                     USART2
#define CDC_UART_ENABLE()            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE)
#define CDC_UART_DISABLE()           RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE)
#define CDC_UART_IRQn                USART2_IRQn
#define CDC_UART_IRQn_Handler        USART2_IRQHandler

#define UART_PINS_PORT_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE)
#define UART_PINS_PORT_DISABLE()     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , DISABLE)

#define UART_TX_PORT                 GPIOA
#define UART_TX_PIN                  GPIO_Pin_2
#define UART_TX_PIN_SOURCE           GPIO_PinSource2

#define UART_RX_PORT                 GPIOA
#define UART_RX_PIN                  GPIO_Pin_3
#define UART_RX_PIN_SOURCE           GPIO_PinSource3

#define UART_CTS_PORT                GPIOA
#define UART_CTS_PIN                 GPIO_Pin_0
#define UART_CTS_PIN_SOURCE          GPIO_PinSource0

#define UART_RTS_PORT                GPIOA
#define UART_RTS_PIN                 GPIO_Pin_1
#define UART_RTS_PIN_SOURCE          GPIO_PinSource1

#endif
