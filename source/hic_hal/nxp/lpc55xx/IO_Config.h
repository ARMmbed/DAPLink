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
#define PIN_PIO_PORT            (0U)

// Debug Port I/O Pins

// SWCLK Pin                    PIO_0 (O)
#define PIN_TCK_SWCLK           (0U)
#define PIN_TCK_SWCLK_MASK      (1U << PIN_TCK_SWCLK)

// SWDIO I/O Pin                PIO0_2 (IO)
// port always has pull-down and digital input (DIGIMODE) enabled (UM11126, section 15.5.1, table 312)
#define PIN_TMS_SWDIO           (2U)
#define PIN_TMS_SWDIO_MASK      (1U << PIN_TMS_SWDIO)

// SWDIO Output Enable Pin      PIO0_28 (O)
// (DBGIF_TMS_SWDIO_TXEN)
#define PIN_TMS_SWDIO_TXEN      (28U)
#define PIN_TMS_SWDIO_TXEN_MASK (1U << PIN_TMS_SWDIO_TXEN)

// TDI Output Pin               PIO0_1 (O)
// (DBGIF_TDI)
#define PIN_TDI                 (1U)
#define PIN_TDI_MASK            (1U << PIN_TDI)

// TDO/SWO Input Pin            PIO0_3 (I)
// (DBGIF_TDO_SWO)
// SWO = function 1 (FC3_RXD_SDA_MOSI_DATA)
#define PIN_TDO_SWO             (3U)
#define PIN_TDO_SWO_MASK        (1U << PIN_TDO_SWO)

// nRESET Pin                   PIO0_19 (O)
// (DBGIF_RESET)
#define PIN_RESET               (19U)
#define PIN_RESET_MASK          (1U << PIN_RESET)

// nRESET Pin Output Enable     PIO0_13 (O)
// (DBGIF_RESET_TXEN)
// type I pin, combo I2C/IO
// must set EGP to put pin in GPIO mode
#define PIN_RESET_TXEN          (13U)
#define PIN_RESET_TXEN_MASK     (1U << PIN_RESET_TXEN)

// SWD Detect Pin               PIO0_22 (I, pullup)
// (DBGIF_DETECT)
#define PIN_DETECT              (22U)
#define PIN_DETECT_MASK         (1U << PIN_DETECT)

// HW Version 6 Pin             PIO0_18 (I, pullup)
// (HW_VERS_6)
#define PIN_HW_VERS_6           (18U)
#define PIN_HW_VERS_6_MASK      (1U << PIN_HW_VERS_6)

// HW Version 7 Pin             PIO0_27 (I, pullup)
// (HW_VERS_7)
#define PIN_HW_VERS_7           (27U)
#define PIN_HW_VERS_7_MASK      (1U << PIN_HW_VERS_7)

// SWD Detect Pin               PIO0_31 (A)
// (DBGIF_VREF)
// analog input = 1/2 target VREF
#define PIN_VREF                (31U)
#define PIN_VREF_MASK           (1U << PIN_VREF)


// UART

// UART Rx Pin                  PIO0_24 (I)
// (FC0_TARGET_RXD)
// function 1 (FC0_RXD_SDA_MOSI_DATA)
#define PIN_UART_RX             (24U)
#define PIN_UART_RX_MASK        (1U << PIN_UART_RX)

// UART Tx Pin                  PIO0_25 (O)
// (FC0_TARGET_TXD)
// function 1 (FC0_TXD_SCL_MISO_WS)
#define PIN_UART_TX             (25U)
#define PIN_UART_TX_MASK        (1U << PIN_UART_TX)


// Debug Unit LEDs

// Connected/Activity LED       PIO0_5
// (PIO0_5-ISP_EN-LED1_CTRL)
// active low
// port always has pull-up and digital input (DIGIMODE) enabled (UM11126, section 15.5.1, table 312)
#define LED_CONNECTED           (5U)
#define LED_CONNECTED_MASK      (1U << LED_CONNECTED)


#endif
