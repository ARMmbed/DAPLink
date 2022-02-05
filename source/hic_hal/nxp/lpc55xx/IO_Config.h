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
#define LED_A_PORT              (0U)
#define LED_A_PIN               (5U)
#define LED_A_MASK              (1U << LED_A_PIN)

//////////////////////////////////////////////////////////////////////////////////////////////////
// Additional configuration for MCU-LINK-PRO support
//////////////////////////////////////////////////////////////////////////////////////////////////

//// MCU-LINK / OB (for reference)
//
// HW_VER_6                  PIO0_18  GPI (pullup required)  VCOM disabled when low (J4)
// HW_VER_7                  PIO0_27  GPI (pullup required)  SWD debug disabled when low (J5)
// DBGIF_RESET               PIO0_19  GPIO
//
// LED_A: ISP_EN-LED_SWD_ACT PIO0_5   GPO (LED1 Red)         Status
//

//// MCU-LINK-PRO
//
// HW_VER_0                  PIO1_27  GPI (pullup required)  USB power negotiation when low
// HW_VER_1                  PIO1_28  GPI (pullup required)  1-bit board id code
// HW_VER_2                  PIO1_30  GPI (pullup required)  Disable USB-SIO bridge when low (J15)
// HW_VER_3                  PIO1_4   GPI (pullup required)  OB(0) or Pro (1) id
// HW_VER_4                  PIO1_5   GPI (pullup required)  Board id code is valid when low
// HW_VER_5                  PIO1_6   GPI (pullup required)  Power measurement enabled when low
// HW_VER_6                  PIO0_18  GPI (pullup required)  VCOM disabled when low (J14)
// HW_VER_7                  PIO0_27  GPI (pullup required)  SWD debug disabled when low (J13)
// DBGIF_RESET               PIO0_19  GPIO                   (SW1)
//
// LED_A: ISP_EN-LED_SWD_ACT PIO0_5   GPO (LED3 Red)         Status
// LED_B: LED_OPT1           PIO1_17  GPO (LED6 Green)       SIO activity
// LED_C: LED_SWO_ACT        PIO1_13  GPO (LED2 Green)       SWO activity
// LED_D: LED_OPT2           PIO1_18  GPO (LED7 Green)       NRG (Energy)
// LED_E: LED_VCOM_ACT       PIO1_15  GPO (LED4 Green)       VCOM activity
// LED_F: LED_USB_COMM       PIO1_12  GPO (LED1 Green)       USB communication
// LED_G: LED_HEARTBEAT      PIO1_16  GPO (LED5 Green)       FUNC

#define LED_B_PORT              (1U)
#define LED_B_PIN               (17U)
#define LED_B_MASK              (1U << LED_B_PIN)

#define LED_C_PORT              (1U)
#define LED_C_PIN               (13U)
#define LED_C_MASK              (1U << LED_C_PIN)

#define LED_D_PORT              (1U)
#define LED_D_PIN               (18U)
#define LED_D_MASK              (1U << LED_D_PIN)

#define LED_E_PORT              (1U)
#define LED_E_PIN               (15U)
#define LED_E_MASK              (1U << LED_E_PIN)

#define LED_F_PORT              (1U)
#define LED_F_PIN               (12U)
#define LED_F_MASK              (1U << LED_F_PIN)

#define LED_G_PORT              (1U)
#define LED_G_PIN               (16U)
#define LED_G_MASK              (1U << LED_G_PIN)

// HW Version 3 Pin             PIO1_4 (I, pullup)
// (HW_VERS_3)
#define PIN_HW_VERS_3_PORT      (1U)
#define PIN_HW_VERS_3           (4U)
#define PIN_HW_VERS_3_MASK      (1U << PIN_HW_VERS_3)

// HW Version 4 Pin             PIO1_5 (I, pullup)
// (HW_VERS_4)
#define PIN_HW_VERS_4_PORT      (1U)
#define PIN_HW_VERS_4           (5U)
#define PIN_HW_VERS_4_MASK      (1U << PIN_HW_VERS_4)

typedef enum led_types {
    LED_T_CONNECTED = 1 << 0,
    LED_T_RUNNING = 1 << 1,
    LED_T_HID = 1 << 2,
    LED_T_CDC = 1 << 3,
    LED_T_MSC = 1 << 4,
    LED_T_EXTRA1 = 1 << 5,
    LED_T_EXTRA2 = 1 << 6,
} led_types_t;

typedef enum led_state gpio_led_state_t;
void gpio_set_leds(uint32_t leds, gpio_led_state_t state);

#endif
