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

#ifndef __IO_CONFIG_H__
#define __IO_CONFIG_H__

#include "stm32wbxx.h"
#include "compiler.h"
#include "daplink.h"


//COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_STM32F373XC);
COMPILER_ASSERT(DAPLINK_HIC_ID == 0x9796990E); //Ashley

//USB control pin
#define USB_CONNECT_PORT_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define USB_CONNECT_PORT_DISABLE()   __HAL_RCC_GPIOA_CLK_DISABLE()
#define USB_CONNECT_PORT             GPIOC
#define USB_CONNECT_PIN              GPIO_PIN_13
#define USB_CONNECT_ON()             (USB_CONNECT_PORT->BRR = USB_CONNECT_PIN)
#define USB_CONNECT_OFF()            (USB_CONNECT_PORT->BSRR  = USB_CONNECT_PIN)

//USB GPIO Configuration
//biby
#define USB_DM                       GPIOA
#define USB_DM_PIN                   GPIO_PIN_11
#define USB_DM_PIN_Bit               11

#define USB_DP                       GPIOA
#define USB_DP_PIN                   GPIO_PIN_12
#define USB_DP_PIN_Bit               12

//Connected LED
#define CONNECTED_LED_PORT           GPIOE
#define CONNECTED_LED_PIN            GPIO_PIN_4
#define CONNECTED_LED_PIN_Bit        4

//When bootloader, disable the target port(not used)
#define POWER_EN_PIN_PORT            GPIOC
#define POWER_EN_PIN                 GPIO_PIN_10
#define POWER_EN_Bit                 10

// nRESET OUT Pin
#define nRESET_PIN_PORT              GPIOC
#define nRESET_PIN                   GPIO_PIN_11
#define nRESET_PIN_Bit               11

//SWD
#define SWCLK_TCK_PIN_PORT           GPIOA
#define SWCLK_TCK_PIN                GPIO_PIN_8	
#define SWCLK_TCK_PIN_Bit            8

#define SWDIO_OUT_PIN_PORT           GPIOB
#define SWDIO_OUT_PIN                GPIO_PIN_15
#define SWDIO_OUT_PIN_Bit            15

#define SWDIO_IN_PIN_PORT            GPIOA
#define SWDIO_IN_PIN                 GPIO_PIN_9
#define SWDIO_IN_PIN_Bit             9

//LEDs
//USB status LED
//Blue
#define RUNNING_LED_PORT             GPIOC
#define RUNNING_LED_PIN              GPIO_PIN_15
#define RUNNING_LED_Bit              15
//Green
#define PIN_HID_LED_PORT             GPIOA
#define PIN_HID_LED                  GPIO_PIN_1
#define PIN_HID_LED_Bit              1
//Red
#define PIN_CDC_LED_PORT             GPIOA
#define PIN_CDC_LED                  GPIO_PIN_0
#define PIN_CDC_LED_Bit              0

#define PIN_MSC_LED_PORT             GPIOB
#define PIN_MSC_LED                  GPIO_PIN_1
#define PIN_MSC_LED_Bit              1

//JTAG Pins

#define OE_PORT                     GPIOB
#define OE                          GPIO_PIN_5
#define OE_Bit                      5

#define JTAG_TRST_PORT              GPIOA
#define JTAG_TRST                   GPIO_PIN_2
#define JTAG_TRST_Bit               2

#define JTAG_TCK_PORT               GPIOA
#define JTAG_TCK                    GPIO_PIN_9
#define JTAG_TCK_Bit                9

#define JTAG_TDI_PORT               GPIOB
#define JTAG_TDI                    GPIO_PIN_14
#define JTAG_TDI_Bit                14

#define JTAG_TDO_PORT               GPIOB
#define JTAG_TDO                    GPIO_PIN_15
#define JTAG_TDO_Bit                15

#define JTAG_TMS_PORT               GPIOA
#define JTAG_TMS                    GPIO_PIN_10
#define JTAG_TMS_Bit                10

#endif
