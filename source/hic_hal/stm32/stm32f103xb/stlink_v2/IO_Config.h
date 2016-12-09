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

// This GPIO configuration is only valid for the STLINK_V2 HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_STLINK_V2);

// USB pullup control
#define USB_PULLUP_PORT         GPIOA
#define USB_PULLUP_PIN          GPIO_Pin_12
#define USB_REENUMERATE()       do { \
                                    USB_PULLUP_PORT->BRR = USB_PULLUP_PIN; \
                                    volatile uint32_t i = 800000; \
                                    while (i > 0) { \
                                        i--; \
                                    } \
                                    USB_PULLUP_PORT->BSRR = USB_PULLUP_PIN; \
                                 } while(0)

// Output clock for the target microcontroller
#define MCO_PIN_PORT                 GPIOA
#define MCO_PIN                      GPIO_Pin_8

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

#endif
