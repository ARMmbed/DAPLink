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

#include "TMPM366.h"
#include "daplink.h"

COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_TMPM366);

#define LED_CONNECTED_IN_BIT        0    // Connect status led
#define LED_RUNNING_IN_BIT          1    // Running status led
#define LED_CONNECTED               (1 << LED_CONNECTED_IN_BIT)
#define LED_RUNNING                 (1 << LED_RUNNING_IN_BIT)
#define PIN_SWDIO_IN_BIT            0    // SWDIO/TMS In/Out Pin
#define PIN_SWCLK_IN_BIT            1    // SWCLK/TCK Pin
// This pin not connected to Port H Toshiba STK
#define PIN_nRESET_IN_BIT           3    // nRESET Pin
// This pin not connected to Port H in  Toshiba STK
#define PIN_TDO_IN_BIT              4    // SWO/TDO Pin
#define PIN_SWCLK                   (1 << PIN_SWCLK_IN_BIT)
#define PIN_SWDIO                   (1 << PIN_SWDIO_IN_BIT)
#define PIN_nRESET                  (1 << PIN_nRESET_IN_BIT)

// All used pins
#define PIN_ALL                     (PIN_SWCLK | PIN_SWDIO | PIN_nRESET)

// For uart
#define UART_ENABLE_RX              ((uint32_t)0x00000200)
#define UART_ENABLE_TX              ((uint32_t)0x00000100)
#define RX_OVRF_MSG                 "<DAPLink:Overflow>\n"
#define RX_OVRF_MSG_SIZE            (sizeof(RX_OVRF_MSG) - 1)
#define BUFFER_SIZE                 (512)
#define UART_RXMIS_INT              (0x10)
#define UART_TXMIS_INT              (0x20)
#define UART_IMSC_TXIM              (5)
#define UART_IMSC_RXIM              (4)


#endif
