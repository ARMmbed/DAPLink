/**
 * @file    IO_Config.h
 * @brief   IO Configuration for nrf52820 HIC
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2021, Arm Limited, All Rights Reserved
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

#include "device.h"
#include "compiler.h"
#include "daplink.h"

#include "dl_nrf_gpio.h"

// This GPIO configuration is only valid for the nrf52840 HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_NRF52840);

#if defined(NRF52840_DONGLE)

#define GPIO_LED_ACTIVE_STATE 0
#define RESET_BUTTON_PULL NRF_GPIO_PIN_PULLUP
#define PIN_SWCLK         NRF_GPIO_PIN_MAP(0, 9)  // SWDCLK (Output)
#define PIN_SWDIO         NRF_GPIO_PIN_MAP(0, 10) // SWDIO (Input/Output)
#undef LED_RUNNING                                // Target Running LED (Output)
#undef LED_CONNECTED                              // Connected LED (Output)
#define LED_HID           NRF_GPIO_PIN_MAP(0, 6)  // HID LED
#define LED_MSC           NRF_GPIO_PIN_MAP(0, 8)  // MSC LED
#define LED_CDC           NRF_GPIO_PIN_MAP(0, 12) // CDC LED
#undef LED_PWR                                    // POWER LED
#define RESET_BUTTON      NRF_GPIO_PIN_MAP(1, 6)  // Reset button (SW_RESET)
#define UART_TX_PIN       NRF_GPIO_PIN_MAP(1, 10) // UART
#define UART_RX_PIN       NRF_GPIO_PIN_MAP(1, 13) // UART
#define I2C_SCL_PIN       NRF_GPIO_PIN_MAP(1, 25) // I2C
#define I2C_SDA_PIN       NRF_GPIO_PIN_MAP(0, 2)  // I2C

#elif defined(SEEEDSTUDIO_XIAO_BLE)

#define GPIO_LED_ACTIVE_STATE 0
#define RESET_BUTTON_PULL NRF_GPIO_PIN_PULLUP
#define PIN_SWCLK         NRF_GPIO_PIN_MAP(1, 13) // SWDCLK (Output)
#define PIN_SWDIO         NRF_GPIO_PIN_MAP(1, 14) // SWDIO (Input/Output)
#undef LED_RUNNING                                // Target Running LED (Output)
#undef LED_CONNECTED                              // Connected LED (Output)
#define LED_HID           NRF_GPIO_PIN_MAP(0, 26) // HID LED
#define LED_MSC           NRF_GPIO_PIN_MAP(0, 30) // MSC LED
#define LED_CDC           NRF_GPIO_PIN_MAP(0, 6)  // CDC LED
#undef LED_PWR                                    // POWER LED
#define RESET_BUTTON      NRF_GPIO_PIN_MAP(0, 3)  // Reset button (SW_RESET)
#define UART_TX_PIN       NRF_GPIO_PIN_MAP(1, 11) // UART TX (From IMCU to target)
#define UART_RX_PIN       NRF_GPIO_PIN_MAP(1, 12) // UART RX (From target to IMCU)
#define I2C_SCL_PIN       NRF_GPIO_PIN_MAP(0, 5)  // I2C SCL
#define I2C_SDA_PIN       NRF_GPIO_PIN_MAP(0, 4)  // I2C SDA

#else

// Default PIN assignment (not finalized based on nRF52833)
#define GPIO_LED_ACTIVE_STATE 1
#define RESET_BUTTON_PULL NRF_GPIO_PIN_PULLUP

#define PIN_SWCLK         NRF_GPIO_PIN_MAP(0, 1)  // SWDCLK (Output)
#define PIN_SWDIO         NRF_GPIO_PIN_MAP(0, 0)  // SWDIO (Input/Output)
#undef PIN_nRESET                                 // nRESET Pin
#undef LED_RUNNING                                // Target Running LED (Output)
#undef LED_CONNECTED                              // Connected LED (Output)
#define LED_HID           NRF_GPIO_PIN_MAP(0, 15) // HID LED
#define LED_MSC           NRF_GPIO_PIN_MAP(0, 15) // MSC LED
#define LED_CDC           NRF_GPIO_PIN_MAP(0, 15) // CDC LED
#define LED_PWR           NRF_GPIO_PIN_MAP(0, 17) // POWER LED
#define RESET_BUTTON      NRF_GPIO_PIN_MAP(1, 9)  // Reset button (SW_RESET)
#define UART_TX_PIN       NRF_GPIO_PIN_MAP(0, 2)  // UART From IMCU to target
#define UART_RX_PIN       NRF_GPIO_PIN_MAP(0, 3)  // UART From target to IMCU
#define I2C_SCL_PIN       NRF_GPIO_PIN_MAP(0, 29) // I2C SCL
#define I2C_SDA_PIN       NRF_GPIO_PIN_MAP(0, 28) // I2C SDA

#endif

#endif
