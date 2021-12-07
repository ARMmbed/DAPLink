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


// Override all defines if IO_CONFIG_OVERRIDE is defined
#ifndef __IO_CONFIG_H__
#define __IO_CONFIG_H__

#include "daplink.h"
#include "device.h"

// This GPIO configuration is only valid for the LPC4322 HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_LPC4322);

// Debug Port I/O Pins

// SWCLK Pin                  P1_17: GPIO0[12]
#define PORT_SWCLK            0
#define PIN_SWCLK_IN_BIT      12
#define PIN_SWCLK             (1<<PIN_SWCLK_IN_BIT)

// SWDIO Pin                  P1_6:  GPIO1[9]
#define PORT_SWDIO            1
#define PIN_SWDIO_IN_BIT      9
#define PIN_SWDIO             (1<<PIN_SWDIO_IN_BIT)

// SWO Pin                    P1_14:  GPIO1[7]
#define PORT_SWO              1
#define PIN_SWO_IN_BIT        7
#define PIN_SWO               (1<<PIN_SWO_IN_BIT)

// SWDIO Output Enable Pin    P1_5:  GPIO1[8]
#define PORT_SWDIO_TXE        1
#define PIN_SWDIO_TXE_IN_BIT  8
#define PIN_SWDIO_TXE         (1<<PIN_SWDIO_TXE_IN_BIT)

// nRESET Pin                 P2_5:  GPIO5[5]
#define PORT_nRESET           5
#define PIN_nRESET_IN_BIT     5
#define PIN_nRESET            (1<<PIN_nRESET_IN_BIT)

// nRESET Output Enable Pin   P2_6:  GPIO5[6]
#define PORT_RESET_TXE        5
#define PIN_RESET_TXE_IN_BIT  6
#define PIN_RESET_TXE         (1<<PIN_RESET_TXE_IN_BIT)

// ISP Control Pin            P2_11:  GPIO1[11]
#define PORT_ISPCTRL          1
#define PIN_ISPCTRL_IN_BIT    11
#define PIN_ISPCTRL           (1<<PIN_ISPCTRL_IN_BIT)

// Connected LED              P1_1: GPIO0[8]
#define PORT_LED_CONNECTED    0
#define PIN_LED_CONNECTED_IN_BIT   8
#define PIN_LED_CONNECTED     (1<<PIN_LED_CONNECTED_IN_BIT)

// Power Enable               P3_1: GPIO5[8]
#define PORT_POWER_EN         5
#define PIN_POWER_EN_BIT      8
#define PIN_POWER_EN          (1<<PIN_POWER_EN_BIT)

#define X_SET(str)     LPC_GPIO_PORT->SET[PORT_##str] = PIN_##str
#define X_CLR(str)     LPC_GPIO_PORT->CLR[PORT_##str] = PIN_##str
#define X_DIR_OUT(str) LPC_GPIO_PORT->DIR[PORT_##str] |= (PIN_##str)
#define X_DIR_IN(str)  LPC_GPIO_PORT->DIR[PORT_##str] &= ~(PIN_##str)
#define X_BYTE(str)    LPC_GPIO_PORT->B[(PORT_##str << 5) + PIN_##str##_IN_BIT]
#define X_WORD(str)    LPC_GPIO_PORT->W[(PORT_##str << 5) + PIN_##str##_IN_BIT]


#endif
