/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2021 Arm Limited (or its affiliates). All 
 * rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        23. July 2021
 * $Revision:    V1.0
 *
 * Project:      I2C Slave Driver Definitions for Nordic Semiconductor nRF52820
 * -------------------------------------------------------------------------- */

#ifndef __I2C_SLAVE_nRF52820_H
#define __I2C_SLAVE_nRF52820_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "Driver_I2C.h"

//#include "config.h"
#include "nrfx_twis.h"
//#include "nrfx_gpio.h"

#include "RTE_Device.h"

// I2C Driver states
#define I2C_INITIALIZED    (1U)
#define I2C_POWERED        (1U << 1)

// I2C Direction
#define I2C_DIR_TX         (0U)
#define I2C_DIR_RX         (1U)

/* I2C Information (Run-Time) */
typedef struct {
  ARM_I2C_SignalEvent_t cb_event;             // Event Callback
  nrfx_twis_config_t    config;
  uint32_t              state;
  uint32_t              tx_num;
  uint32_t              rx_num;
  uint8_t               dir;
  uint8_t               err;
} I2C_INFO;

/* I2C Resources definition */
typedef struct {
  nrfx_twis_t                nrfx_twis;
  nrfx_twis_event_handler_t  twis_event_handler;
  I2C_INFO                   *info;                 // Run-Time information
} const I2C_RESOURCES;


#endif /* __I2C_SLAVE_nRF52820_H */
