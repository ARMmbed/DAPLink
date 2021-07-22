/**
 * @file    IO_Config_Override.c
 * @brief   Alternative IO Configuration for nrf52820 HIC
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

#ifndef __IO_CONFIG_H__
#define __IO_CONFIG_H__

#include "device.h"
#include "compiler.h"
#include "daplink.h"

#include "nrf_gpio.h"

// This GPIO configuration is only valid for the nrf52820 HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_NRF52820);

#define LED_1          NRF_GPIO_PIN_MAP(0, 13)
#define LED_2          NRF_GPIO_PIN_MAP(0, 14)
#define LED_3          NRF_GPIO_PIN_MAP(0, 15)
#define LED_4          NRF_GPIO_PIN_MAP(0, 16)
#define BUTTON_1       NRF_GPIO_PIN_MAP(0, 11)

// SWDCLK (Output)
#define PIN_SWDCLK     NRF_GPIO_PIN_MAP(0, 4)

// SWDIO (Input/Output)
#define PIN_SWDIO      NRF_GPIO_PIN_MAP(0, 5)

// nRESET Pin
#undef PIN_nRESET
// #define PIN_nRESET     NRF_GPIO_PIN_MAP(0, 7)

// Target Running LED (Output)
#define LED_RUNNING    LED_1

// Connected LED (Output)
#define LED_CONNECTED  LED_2

// HID LED
#define LED_HID        LED_2

// MSC LED
#define LED_MSC        LED_3

// CDC LED
#define LED_CDC        LED_4

// Reset button
#define RESET_BUTTON      BUTTON_1
#define RESET_BUTTON_PULL NRF_GPIO_PIN_PULLUP

// UART
#ifndef UART_EXTERNAL
// This is connected to J-Link
#define UART_RX_PIN NRF_GPIO_PIN_MAP(0, 8) // RxD P0.08
#define UART_TX_PIN NRF_GPIO_PIN_MAP(0, 6) // TxD P0.06
#else
// Alternative pin out
#define UART_RX_PIN NRF_GPIO_PIN_MAP(0, 29)
#define UART_TX_PIN NRF_GPIO_PIN_MAP(0, 30)
#endif

#endif
