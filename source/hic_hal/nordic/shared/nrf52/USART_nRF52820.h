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
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        16. July 2021
 * $Revision:    V1.0
 *
 * Project:      USART Driver definitions for Nordic Semiconductor nRF52820
 * -------------------------------------------------------------------------- */

#ifndef __USART_nRF52820_H
#define __USART_nRF52820_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "Driver_USART.h"

#include "nrf.h"                        // Device header
#include "nrf52_erratas.h"

#include "RTE_Device.h"

// USART flags
#define USART_FLAG_INITIALIZED         (1U)
#define USART_FLAG_POWERED             (1U << 1)
#define USART_FLAG_CONFIGURED          (1U << 2)

// USART direction
#define RX_DIR                         (0U)
#define TX_DIR                         (1U)

#endif /* __USART_nRF52820_H */
