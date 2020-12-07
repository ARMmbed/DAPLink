/**
 * @file    IO_Config.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2020 Arm Limited, All Rights Reserved
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

// This GPIO configuration is only valid for the LPC55xx HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_LPC55XX);

// All pins are PIO0.
#define PIN_PIO_PORT            (0)

// Debug Port I/O Pins

// SWCLK Pin                    PIO_0 (O)
// (DBGIF_TCK_SWCLK)
#define PIN_SWCLK_BIT           (0)

// SWDIO I/O Pin                PIO0_2 (IO)
// (DBGIF_TMS_SWDIO)
#define PIN_SWDIO_OUT_BIT       (2)

// SWDIO Output Enable Pin      PIO0_28 (O)
// (DBGIF_TMS_SWDIO_TXEN)
#define PIN_SWDIO_OE_BIT        (28)

// TDI Output Pin               PIO0_1 (O)
// (DBGIF_TDI)
#define PIN_TDI_BIT             (1)

// TDO/SWO Input Pin            PIO0_3 (I)
// (DBGIF_TDO_SWO)
// SWO = function 1 (FC3_RXD_SDA_MOSI_DATA)
#define PIN_SWO_RX_BIT          (3)

// nRESET Pin                   PIO0_19 (O)
// (DBGIF_RESET)
#define PIN_RESET_BIT           (19)
#define PIN_RESET               (1 << PIN_RESET_BIT)

// nRESET Pin Output Enable     PIO0_13 (O)
// (DBGIF_RESET_TXEN)
#define PIN_RESET_EN_BIT       (13)
#define PIN_RESET_EN           (1 << PIN_RESET_EN_BIT)

// SWD Detect Pin               PIO0_22 (I, pullup)
// (DBGIF_DETECT)
#define PIN_SWD_DETECT_BIT      (22)
#define PIN_SWD_DETECT          (1 << PIN_SWD_DETECT_BIT)


// UART
// RX = PIO0_24, function 1 (FC0_RXD_SDA_MOSI_DATA)
// TX = PIO0_25, function 1 (FC0_TXD_SCL_MISO_WS)

// DBGIF_VREF = PIO0_31 (Analog input, 1/2 target VREF)


// Debug Unit LEDs

// Connected/Activity LED       PIO0_5
// (PIO0_5-ISP_EN-LED1_CTRL)
#define LED_CONNECTED_BIT       (5)
#define LED_CONNECTED           (1 << LED_CONNECTED_BIT)


#endif
