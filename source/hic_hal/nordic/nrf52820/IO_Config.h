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

// This GPIO configuration is only valid for the nrf52820 HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_NRF52820);

#define NRF528XX_DYNAMIC_PIN
#define GPIO_LED_ACTIVE_STATE 1

extern uint32_t uart_tx_pin;
extern uint32_t uart_rx_pin;
extern uint32_t sw_reset_pin;
extern uint32_t led_usb_pin;
extern uint32_t led_pwr_pin;
extern uint32_t i2c_scl_pin;
extern uint32_t i2c_sda_pin;

// SWDCLK (Output)
#define PIN_SWCLK         NRF_GPIO_PIN_MAP(0, 1)

// SWDIO (Input/Output)
#define PIN_SWDIO         NRF_GPIO_PIN_MAP(0, 0)

// nRESET Pin
#undef PIN_nRESET

// Target Running LED (Output)
#undef LED_RUNNING

// Connected LED (Output)
#undef LED_CONNECTED

// HID LED
#define LED_HID           led_usb_pin

// MSC LED
#define LED_MSC           led_usb_pin

// CDC LED
#define LED_CDC           led_usb_pin

// POWER LED
#define LED_PWR           led_pwr_pin

// Reset button (SW_RESET)
#define RESET_BUTTON      sw_reset_pin
#define RESET_BUTTON_PULL NRF_GPIO_PIN_PULLUP

// UART
#define UART_TX_PIN       uart_tx_pin
#define UART_RX_PIN       uart_rx_pin

// I2C
#define I2C_SCL_PIN       i2c_scl_pin
#define I2C_SDA_PIN       i2c_sda_pin

#define NRF52820_RESET_PIN   NRF_GPIO_PIN_MAP(0, 6)
#define NRF52820_USB_LED_PIN NRF_GPIO_PIN_MAP(0, 14)
#define NRF52820_PWR_LED_PIN NRF_GPIO_PIN_MAP(0, 15)
#define NRF52820_UART_TX_PIN NRF_GPIO_PIN_MAP(0, 29) // From IMCU to target
#define NRF52820_UART_RX_PIN NRF_GPIO_PIN_MAP(0, 8)  // From target to IMCU
#define NRF52820_I2C_SCL_PIN NRF_GPIO_PIN_MAP(0, 28)
#define NRF52820_I2C_SDA_PIN NRF_GPIO_PIN_MAP(0, 30)

#define NRF52833_RESET_PIN   NRF_GPIO_PIN_MAP(1, 9)
#define NRF52833_USB_LED_PIN NRF_GPIO_PIN_MAP(0, 15)
#define NRF52833_PWR_LED_PIN NRF_GPIO_PIN_MAP(0, 17)
#define NRF52833_UART_TX_PIN NRF_GPIO_PIN_MAP(0, 2)  // From IMCU to target
#define NRF52833_UART_RX_PIN NRF_GPIO_PIN_MAP(0, 3)  // From target to IMCU
#define NRF52833_I2C_SCL_PIN NRF_GPIO_PIN_MAP(0, 29)
#define NRF52833_I2C_SDA_PIN NRF_GPIO_PIN_MAP(0, 28)

#endif
