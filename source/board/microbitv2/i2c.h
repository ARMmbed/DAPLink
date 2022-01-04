/**
 * @file    i2c.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2020 NXP
 * Copyright 2021 Micro:bit Educational Foundation
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

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include <stdbool.h>

/*! i2c Read/Write Callback prototype */
typedef void (*i2cCallback_t)
(
    uint8_t*    pData,
    uint8_t     size
);

typedef enum {
    I2C_STATUS_SUCCESS = 0,
    I2C_STATUS_FAIL
} i2c_status_t;

/* TX and RX buffer size */
#define I2C_DATA_LENGTH             (1024U + 8U)

void i2c_initialize(void);
void i2c_deinitialize(void);
i2c_status_t i2c_registerWriteCallback(i2cCallback_t writeCallback, uint8_t slaveAddress);
i2c_status_t i2c_registerReadCallback(i2cCallback_t readCallback, uint8_t slaveAddress);
void i2c_clearState(void);
void i2c_fillBuffer(uint8_t* data, uint32_t position, uint32_t size);
/* A more efficient way to fill the first byte for simple transactions. */
void i2c_fillBufferHead(uint8_t data);
/* Returns true if I2C activity is idle and device can go too sleep. */
bool i2c_canSleep(void);
/* This functions needs to be called at a 30ms interval. */
void i2c_30ms_tick(void);

#endif /* I2C_H_ */
