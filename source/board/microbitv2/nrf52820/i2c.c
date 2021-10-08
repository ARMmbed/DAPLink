/**
 * @file    i2c.c
 * @brief
 *
 * DAPLink Interface Firmware
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

#include "i2c.h"

void i2c_initialize()
{
    // TODO
}

void i2c_deinitialize()
{
    // TODO
}

i2c_status_t i2c_registerWriteCallback(i2cWriteCallback_t writeCallback, uint8_t slaveAddress)
{
    // TODO
}

i2c_status_t i2c_registerReadCallback(i2cReadCallback_t readCallback, uint8_t slaveAddress)
{
    // TODO
}

void i2c_clearBuffer()
{
    // TODO
}

void i2c_fillBuffer(uint8_t* data, uint32_t position, uint32_t size)
{
    // TODO
}

bool i2c_isBusy()
{
    // TODO
    return false;
}
