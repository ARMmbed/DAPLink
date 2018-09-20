/**
 * @file    target.c
 * @brief   Target information for Realtek RTL8195AM
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2016-2017, Realtek Semiconductor Corp., All Rights Reserved
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

#include "target_config.h"

// The file flash_blob.c must only be included in target.c
#include "flash_blob.c"

// target information
target_cfg_t target_device = {
    .sector_size    = KB(4),
    .sector_cnt     = MB(2) / KB(4),
    .flash_start    = 0x00000000,
    .flash_end      = 0x00000000 + MB(2),
    .ram_start      = 0x10007000,
    .ram_end        = 0x10070000,
    .flash_algo     = (program_target_t *) &flash,
    .erase_reset    = 0,
};

// RTL8195AM's main cpu can only talk 38400 with DAP UART
#include "uart.h"
static UART_Configuration UART_Config;

int32_t USBD_CDC_ACM_SetLineCoding(void)
{
    UART_Config.Baudrate    = 38400;
    UART_Config.DataBits    = UART_DATA_BITS_8;
    UART_Config.Parity      = UART_PARITY_NONE;
    UART_Config.StopBits    = UART_STOP_BITS_1;
    UART_Config.FlowControl = UART_FLOW_CONTROL_NONE;

    return uart_set_configuration(&UART_Config);
}
