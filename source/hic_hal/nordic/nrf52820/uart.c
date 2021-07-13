/**
 * @file    uart.c
 * @brief   UART Function for nrf52820 HIC
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2021, Arm Limited, All Rights Reserved
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

#include "uart.h"

int32_t uart_initialize(void)
{
}

int32_t uart_uninitialize(void)
{
}

int32_t uart_reset(void)
{
}

int32_t uart_set_configuration(UART_Configuration *config)
{
}

int32_t uart_get_configuration(UART_Configuration *config)
{
}

int32_t uart_write_free(void)
{
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
}

void CDC_UART_IRQn_Handler(void)
{
}
