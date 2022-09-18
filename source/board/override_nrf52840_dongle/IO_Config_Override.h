/**
 * @file    IO_Config_Override.c
 * @brief   Alternative IO for nRF52840 based Hardware Interface Circuit
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

#include "device.h"
#include "compiler.h"
#include "daplink.h"

#include "dl_nrf_gpio.h"

// This GPIO configuration is only valid for the nrf52840 HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_NRF52840);

#define GPIO_LED_ACTIVE_STATE 0

// SWDCLK (Output)
#define PIN_SWCLK         NRF_GPIO_PIN_MAP(0, 9)

// SWDIO (Input/Output)
#define PIN_SWDIO         NRF_GPIO_PIN_MAP(0, 10)

// nRESET Pin
#undef PIN_nRESET

// Target Running LED (Output)
#undef LED_RUNNING

// Connected LED (Output)
#undef LED_CONNECTED

// HID LED
#define LED_HID           NRF_GPIO_PIN_MAP(0, 6)

// MSC LED
#define LED_MSC           LED_HID

// CDC LED
#define LED_CDC           LED_HID

// POWER LED
#define LED_PWR           NRF_GPIO_PIN_MAP(0, 8)

// Reset button (SW_RESET)
#define RESET_BUTTON      NRF_GPIO_PIN_MAP(1, 6)
#define RESET_BUTTON_PULL NRF_GPIO_PIN_PULLUP

// UART
#define UART_TX_PIN       NRF_GPIO_PIN_MAP(1, 10)
#define UART_RX_PIN       NRF_GPIO_PIN_MAP(1, 13)

// I2C
#define I2C_SCL_PIN       NRF_GPIO_PIN_MAP(1, 25)
#define I2C_SDA_PIN       NRF_GPIO_PIN_MAP(0, 2)
#endif
