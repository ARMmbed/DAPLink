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
#ifndef SERIAL_H
#define SERIAL_H

#include "uart.h"

/* UART port assignments */
#if defined(MBED_EXT) || defined (UART_TEST)
#define UART_TARGET    (0)     /* UART connected to the external target via JTAG connector */
                         /* No debug UART on external build */
#else
#define UART_TARGET    (1)     /* UART connected to the on-board target */
#define UART_DEBUG     (0)     /* UART used for debug output */
#endif

/* Serial Messages */
typedef enum {
    SERIAL_INITIALIZE,
    SERIAL_UNINITIALIZE,
    SERIAL_RESET,
    SERIAL_SET_CONFIGURATION,
    SERIAL_GET_CONFIGURATION
} SERIAL_MSG;

/* The purpose of these functions is to serialize access to the serial (currently just UART)
 * driver. This keeps multiple threads from calling the UART init/config functions while the serial
 * task is doing reads and writes. */

int32_t  serial_initialize                  (void);
int32_t  serial_uninitialize                (void);
int32_t  serial_reset                       (void);
int32_t  serial_set_configuration           (UART_Configuration *config);
int32_t  serial_get_configuration           (UART_Configuration *config);

#endif
