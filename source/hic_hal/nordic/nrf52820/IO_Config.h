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

#include "nrf_gpio.h"

// This GPIO configuration is only valid for the nrf52820 HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_NRF52820);

// SWDCLK (Output)
#define PIN_SWDCLK     NRF_GPIO_PIN_MAP(0, 1)

// SWDIO (Input/Output)
#define PIN_SWDIO      NRF_GPIO_PIN_MAP(0, 0)

// nRESET Pin
#undef PIN_nRESET

// Target Running LED (Output)
#undef LED_RUNNING

// Connected LED (Output)
#undef LED_CONNECTED

// HID LED
#define LED_HID        NRF_GPIO_PIN_MAP(0, 7)

// MSC LED
#define LED_MSC        NRF_GPIO_PIN_MAP(0, 7)

// CDC LED
#define LED_CDC        NRF_GPIO_PIN_MAP(0, 7)

// Reset button (SW_RESET)
#define RESET_BUTTON      NRF_GPIO_PIN_MAP(0, 6)
#define RESET_BUTTON_PULL NRF_GPIO_PIN_PULLUP

// UART
#define UART_TX NRF_GPIO_PIN_MAP(0, 8)  // From IMCU to target
#define UART_RX NRF_GPIO_PIN_MAP(0, 29) // From target to IMCU

#endif
