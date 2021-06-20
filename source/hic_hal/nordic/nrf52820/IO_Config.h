/**
 * @file    IO_Config.h
 * @brief   IO Configuration for nrf52820 HIC
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2021 Arm Limited, All Rights Reserved
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

// This GPIO configuration is only valid for the nrf52820 HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_NRF52820);

// SWCLK (Output)
#define PIN_SWCDLK_PORT
#define PIN_SWCDLK_BIT

// SWDIO (Input/Output)
#define PIN_SWDIO_PORT
#define PIN_SWDIO_BIT

// nRESET Pin
#define PIN_nRESET_PORT
#define PIN_nRESET_BIT

// Connected LED (Output)
#define LED_CONNECTED_PORT
#define LED_CONNECTED_BIT

// Target Running LED (Output)
#define LED_RUNNING_PORT
#define LED_RUNNING_BIT

#endif
