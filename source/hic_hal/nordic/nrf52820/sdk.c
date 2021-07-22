/**
 * @file    sdk.c
 * @brief   HIC Initialization
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

#include "device.h"
#include "IO_config.h"
#include "daplink.h"
#include "util.h"

#ifdef NRF52820_DYNAMIC_PIN
uint32_t uart_tx_pin;
uint32_t uart_rx_pin;
uint32_t sw_reset_pin;
uint32_t led_usb_pin;
#endif

void sdk_init()
{
#ifdef NRF52820_DYNAMIC_PIN
    if (NRF_FICR->INFO.PART == 0x52833) {
        // nRF52833
        sw_reset_pin = NRF52833_RESET_PIN;
        led_usb_pin = NRF52833_LED_PIN;
        uart_tx_pin = NRF52833_UART_TX_PIN;
        uart_rx_pin = NRF52833_UART_RX_PIN;
    } else {
        // nRF52820
        sw_reset_pin = NRF52820_RESET_PIN;
        led_usb_pin = NRF52820_LED_PIN;
        uart_tx_pin = NRF52820_UART_TX_PIN;
        uart_rx_pin = NRF52820_UART_RX_PIN;
    }
#endif
}
