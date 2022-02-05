/**
 * @file    IO_Config_Override.c
 * @brief   Alternative IO for nRF5x-DK boards
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

#ifndef __IO_CONFIG_OVERRIDE_H__
#define __IO_CONFIG_OVERRIDE_H__

#include "sam3u2c.h"
#include "daplink.h"

// This GPIO configuration is only valid for the SAM3U2C HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_SAM3U2C);

// DAP LED
#define PIN_DAP_LED_PORT           PIOA
#define PIN_DAP_LED_BIT            29
#define PIN_DAP_LED                (1UL << PIN_DAP_LED_BIT)

// MSD LED
#define PIN_MSD_LED_PORT           PIOA
#define PIN_MSD_LED_BIT            29
#define PIN_MSD_LED                (1UL << PIN_MSD_LED_BIT)

// CDC LED
#define PIN_CDC_LED_PORT           PIOA
#define PIN_CDC_LED_BIT            29
#define PIN_CDC_LED                (1UL << PIN_CDC_LED_BIT)

// Non-Forwarded Reset in PIN - Not used

// Forwarded Reset in PIN
#define PIN_RESET_IN_FWRD_PORT     PIOA
#define PIN_RESET_IN_FWRD_BIT      25
#define PIN_RESET_IN_FWRD          (1UL << PIN_RESET_IN_FWRD_BIT)

// support for dynamic assignment of SWD signals
extern Pio *pin_nreset_port;            // GPIO port for nRESET signal
extern Pio *pin_swclk_port;             // GPIO port for SWCLK signal
extern Pio *pin_swdio_port;             // GPIO port for SWDIO signal
extern unsigned long pin_nreset_bit;    // GPIO pin for nRESET signal
extern unsigned long pin_swclk_bit;     // GPIO pin for SWCLK signal
extern unsigned long pin_swdio_bit;     // GPIO pin for SWDIO signal
extern unsigned long pin_nreset;        // GPIO mask for nRESET signal
extern unsigned long pin_swclk;         // GPIO mask for SWCLK signal
extern unsigned long pin_swdio;         // GPIO msak for SWDIO signal

// dynamic nRESET OUT Pin
#define PIN_nRESET_PORT            pin_nreset_port
#define PIN_nRESET_BIT             pin_nreset_bit
#define PIN_nRESET                 pin_nreset

// dynamic SWCLK/TCK Pin
#define PIN_SWCLK_PORT             pin_swclk_port
#define PIN_SWCLK_BIT              pin_swclk_bit
#define PIN_SWCLK                  pin_swclk

// dynamic SWDIO/TMS In/Out Pin
#define PIN_SWDIO_PORT             pin_swdio_port
#define PIN_SWDIO_BIT              pin_swdio_bit
#define PIN_SWDIO                  pin_swdio

// internal nRESET OUT Pin
#define PIN_OB_nRESET_PORT          PIOA
#define PIN_OB_nRESET_BIT           4
#define PIN_OB_nRESET               (1UL << PIN_OB_nRESET_BIT)

// internal SWCLK/TCK Pin
#define PIN_OB_SWCLK_PORT           PIOA
#define PIN_OB_SWCLK_BIT            17
#define PIN_OB_SWCLK                (1UL << PIN_OB_SWCLK_BIT)

// internal SWDIO/TMS In/Out Pin
#define PIN_OB_SWDIO_PORT           PIOA
#define PIN_OB_SWDIO_BIT            18
#define PIN_OB_SWDIO                (1UL << PIN_OB_SWDIO_BIT)

// external nRESET OUT Pin
#define PIN_EXT_nRESET_PORT         PIOB
#define PIN_EXT_nRESET_BIT          17
#define PIN_EXT_nRESET              (1UL << PIN_EXT_nRESET_BIT)

// external SWCLK/TCK Pin
#define PIN_EXT_SWCLK_PORT          PIOB
#define PIN_EXT_SWCLK_BIT           10
#define PIN_EXT_SWCLK               (1UL << PIN_EXT_SWCLK_BIT)

// external SWDIO/TMS In/Out Pin
#define PIN_EXT_SWDIO_PORT          PIOB
#define PIN_EXT_SWDIO_BIT           9
#define PIN_EXT_SWDIO               (1UL << PIN_EXT_SWDIO_BIT)

// shield nRESET OUT Pin
#define PIN_SH_nRESET_PORT          PIOB
#define PIN_SH_nRESET_BIT           22
#define PIN_SH_nRESET               (1UL << PIN_SH_nRESET_BIT)

// shield SWCLK/TCK Pin
#define PIN_SH_SWCLK_PORT           PIOB
#define PIN_SH_SWCLK_BIT            20
#define PIN_SH_SWCLK                (1UL << PIN_SH_SWCLK_BIT)

// shield SWDIO/TMS In/Out Pin
#define PIN_SH_SWDIO_PORT           PIOB
#define PIN_SH_SWDIO_BIT            19
#define PIN_SH_SWDIO                (1UL << PIN_SH_SWDIO_BIT)

// TDI Pin - Not used

// SWO/TDO Pin - Not used

#endif
