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

/* UART port assignments */
#if defined(MBED_EXT) || defined (UART_TEST)
#define UART_TARGET    (0)     /* UART connected to the external target via JTAG connector */
                         /* No debug UART on external build */
#else
#define UART_TARGET    (1)     /* UART connected to the on-board target */
#define UART_DEBUG     (0)     /* UART used for debug output */
#endif

typedef __packed struct {
  U32 dwDTERate;    /* Data terminal rate in bits per second */
  U8  bCharFormat;  /* Number of stop bits */
  U8  bParityType;  /* Parity bit type */
  U8  bDataBits;    /* Number of data bits */
} UART_LINE_CODING;

BOOL InitUART(int port);
BOOL SetLineCoding(int port, UART_LINE_CODING l);
void flushUartFIFO(int port);

void mputch(int port, char data);
char getch(int port);
char RxEmpty(int port);

int serialFifoRead(U16 timeout);
void setLoopbackMode(int port);

#endif
