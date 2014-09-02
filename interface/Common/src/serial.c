/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <RTL.h>
#include "serial.h"

extern OS_ID serial_mailbox;

UART_Configuration uart_config;

int32_t serial_initialize(void)
{
    os_mbx_send(&serial_mailbox, (void*)SERIAL_INITIALIZE, 0);
    return 1;
}

int32_t serial_uninitialize(void)
{
    os_mbx_send(&serial_mailbox, (void*)SERIAL_UNINITIALIZE, 0);
    return 1;
}

int32_t serial_reset(void)
{
    os_mbx_send(&serial_mailbox, (void*)SERIAL_RESET, 0);
    return 1;
}

int32_t serial_set_configuration(UART_Configuration *config)
{
    uart_config = *config;
    os_mbx_send(&serial_mailbox, (void*)SERIAL_SET_CONFIGURATION, 0);
    return 1;
}

int32_t serial_get_configuration(UART_Configuration *config)
{
    *config = uart_config;
    return 1;
}

