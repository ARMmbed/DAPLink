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

#include "dl_nrf_gpio.h"

// This GPIO configuration is only valid for the nrf52820 HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_NRF52820);

#define LED_1          NRF_GPIO_PIN_MAP(0, 13)
#define LED_2          NRF_GPIO_PIN_MAP(0, 14)
#define LED_3          NRF_GPIO_PIN_MAP(0, 15)
#define LED_4          NRF_GPIO_PIN_MAP(0, 16)
#define BUTTON_1       NRF_GPIO_PIN_MAP(0, 11)

// SWDCLK (Output)
#define PIN_SWCLK      NRF_GPIO_PIN_MAP(0, 4)
// #define PIN_SWCLK      NRF_GPIO_PIN_MAP(0, 30)

// SWDIO (Input/Output)
#define PIN_SWDIO      NRF_GPIO_PIN_MAP(0, 5)
// #define PIN_SWDIO      NRF_GPIO_PIN_MAP(0, 31)

// nRESET Pin
#undef PIN_nRESET


#ifndef NRF528XX_DYNAMIC_PIN

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

// I2C
#define I2C_SCL_PIN  NRF_GPIO_PIN_MAP(0, 27)
#define I2C_SDA_PIN  NRF_GPIO_PIN_MAP(0, 26)

#else

/* Use dynamic pin-mapping */

extern uint32_t uart_tx_pin;
extern uint32_t uart_rx_pin;
extern uint32_t sw_reset_pin;
extern uint32_t led_usb_pin;
extern uint32_t led_pwr_pin;
extern uint32_t i2c_scl_pin;
extern uint32_t i2c_sda_pin;

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

/* Use defaults for nRF52820 */
#define NRF52820_RESET_PIN   NRF_GPIO_PIN_MAP(0, 6)
#define NRF52820_USB_LED_PIN NRF_GPIO_PIN_MAP(0, 14)
#define NRF52820_PWR_LED_PIN NRF_GPIO_PIN_MAP(0, 15)
#define NRF52820_UART_TX_PIN NRF_GPIO_PIN_MAP(0, 8)  // From IMCU to target
#define NRF52820_UART_RX_PIN NRF_GPIO_PIN_MAP(0, 29) // From target to IMCU

#define NRF52833_RESET_PIN   BUTTON_1
#define NRF52833_USB_LED_PIN LED_4
#define NRF52833_PWR_LED_PIN LED_3
#ifndef UART_EXTERNAL
// This is connected to J-Link
#define NRF52833_UART_TX_PIN NRF_GPIO_PIN_MAP(0, 8) // RxD P0.08
#define NRF52833_UART_RX_PIN NRF_GPIO_PIN_MAP(0, 6) // TxD P0.06
#else
// Alternative pin out
#define NRF52833_UART_TX_PIN NRF_GPIO_PIN_MAP(0, 29)  // From IMCU to target
#define NRF52833_UART_RX_PIN NRF_GPIO_PIN_MAP(0, 30)  // From target to IMCU
#endif

#define NRF52833_I2C_SCL_PIN NRF_GPIO_PIN_MAP(0, 27)
#define NRF52833_I2C_SDA_PIN NRF_GPIO_PIN_MAP(0, 26)

#endif

#endif
