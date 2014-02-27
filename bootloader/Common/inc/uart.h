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
#ifndef UART_H
#define UART_H

#include <stdint.h>

/* Parity enumerator */
typedef enum {
  UART_PARITY_NONE    = 0,
  UART_PARITY_ODD     = 1,
  UART_PARITY_EVEN    = 2,
  UART_PARITY_MARK    = 3,
  UART_PARITY_SPACE   = 4
} UART_Parity;

/* Stop Bits enumerator */
typedef enum {
  UART_STOP_BITS_1    = 0,
  UART_STOP_BITS_1_5  = 1,
  UART_STOP_BITS_2    = 2
} UART_StopBits;

/* Data Bits enumerator */
typedef enum {
  UART_DATA_BITS_5    = 5,
  UART_DATA_BITS_6    = 6,
  UART_DATA_BITS_7    = 7,
  UART_DATA_BITS_8    = 8,
  UART_DATA_BITS_16   = 16
} UART_DataBits;

/* Flow control enumerator */
typedef enum {
  UART_FLOW_CONTROL_NONE     = 0,
  UART_FLOW_CONTROL_RTS_CTS  = 1,
  UART_FLOW_CONTROL_XON_XOFF = 2
} UART_FlowControl;

/* UART Port Properties structure */
typedef struct {
  uint32_t           Baudrate;
  UART_DataBits      DataBits;
  UART_Parity        Parity;
  UART_StopBits      StopBits;
  UART_FlowControl   FlowControl;
} UART_Configuration;

/*-----------------------------------------------------------------------------
 * FUNCTION PROTOTYPES
 *----------------------------------------------------------------------------*/

/* UART driver function prototypes */
extern int32_t  uart_initialize                  (void);
extern int32_t  uart_uninitialize                (void);
extern int32_t  uart_reset                       (void);
extern int32_t  uart_set_configuration           (UART_Configuration *config);
extern int32_t  uart_get_configuration           (UART_Configuration *config);
extern int32_t  uart_write_free                  (void);
extern int32_t  uart_write_data                  (uint8_t *data, uint16_t size);
extern int32_t  uart_read_data                   (uint8_t *data, uint16_t size);

#endif /* __UART_H */
