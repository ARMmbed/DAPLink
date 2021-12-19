/**
 * @file    IO_Config_Override.c
 * @brief   Alternative IO for LPC11U35 based Hardware Interface Circuit
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

// This GPIO configuration is only valid for the SAM3U2C HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_SAM3U2C);

// DAP LED
#define PIN_DAP_LED_PORT        PIOA
#define PIN_DAP_LED_BIT         29
#define PIN_DAP_LED             (1UL << PIN_DAP_LED_BIT)

// MSD LED
#define PIN_MSD_LED_PORT        PIOA
#define PIN_MSD_LED_BIT         29
#define PIN_MSD_LED             (1UL << PIN_MSD_LED_BIT)

// CDC LED
#define PIN_CDC_LED_PORT        PIOA
#define PIN_CDC_LED_BIT         29
#define PIN_CDC_LED             (1UL << PIN_CDC_LED_BIT)

// external nRESET OUT Pin
#ifndef PIN_EXT_nRESET
#define PIN_EXT_nRESET_PORT         PIOB
#define PIN_EXT_nRESET_BIT          17
#define PIN_EXT_nRESET              (1UL << PIN_EXT_nRESET_BIT)
#endif

// external SWCLK/TCK Pin
#ifndef PIN_EXT_SWCLK
#define PIN_EXT_SWCLK_PORT          PIOB
#define PIN_EXT_SWCLK_BIT           10
#define PIN_EXT_SWCLK               (1UL << PIN_EXT_SWCLK_BIT)
#endif

// external SWDIO/TMS In/Out Pin
#ifndef PIN_EXT_SWDIO
#define PIN_EXT_SWDIO_PORT          PIOB
#define PIN_EXT_SWDIO_BIT           9
#define PIN_EXT_SWDIO               (1UL << PIN_EXT_SWDIO_BIT)
#endif

// shield nRESET OUT Pin
#ifndef PIN_SH_nRESET
#define PIN_SH_nRESET_PORT          PIOB
#define PIN_SH_nRESET_BIT           22
#define PIN_SH_nRESET               (1UL << PIN_SH_nRESET_BIT)
#endif

// shield SWCLK/TCK Pin
#ifndef PIN_SH_SWCLK
#define PIN_SH_SWCLK_PORT           PIOB
#define PIN_SH_SWCLK_BIT            20
#define PIN_SH_SWCLK                (1UL << PIN_SH_SWCLK_BIT)
#endif

// shield SWDIO/TMS In/Out Pin
#ifndef PIN_SH_SWDIO
#define PIN_SH_SWDIO_PORT           PIOB
#define PIN_SH_SWDIO_BIT            19
#define PIN_SH_SWDIO                (1UL << PIN_SH_SWDIO_BIT)
#endif

#endif
