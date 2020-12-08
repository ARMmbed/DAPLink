/**
 * @file    uart.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2020 Arm Limited, All Rights Reserved
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

#include "string.h"
#include "fsl_device_registers.h"
#include "fsl_usart_cmsis.h"
#include "uart.h"
#include "util.h"
#include "cortex_m.h"
#include "circ_buf.h"
#include "settings.h" // for config_get_overflow_detect

#define USART_INSTANCE (Driver_USART0)
#define USART_IRQ      (FLEXCOMM0_IRQn)

extern uint32_t SystemCoreClock;

static void clear_buffers(void);

#define RX_OVRF_MSG         "<DAPLink:Overflow>\n"
#define RX_OVRF_MSG_SIZE    (sizeof(RX_OVRF_MSG) - 1)
#define BUFFER_SIZE         (512)

circ_buf_t write_buffer;
uint8_t write_buffer_data[BUFFER_SIZE];
circ_buf_t read_buffer;
uint8_t read_buffer_data[BUFFER_SIZE];

struct {
    uint8_t rx;
    uint8_t tx;
} cb_buf;

void uart_handler(uint32_t event);

void clear_buffers(void)
{
    circ_buf_init(&write_buffer, write_buffer_data, sizeof(write_buffer_data));
    circ_buf_init(&read_buffer, read_buffer_data, sizeof(read_buffer_data));
}

int32_t uart_initialize(void)
{
    clear_buffers();
    Driver_USART0.Initialize(uart_handler);

    return 1;
}

int32_t uart_uninitialize(void)
{
    USART_INSTANCE.Control(ARM_USART_CONTROL_RX, 0);
    Driver_USART0.Uninitialize();
    clear_buffers();

    return 1;
}

int32_t uart_reset(void)
{
    // disable interrupt
    NVIC_DisableIRQ(USART_IRQ);
    clear_buffers();
    // enable interrupt
    NVIC_EnableIRQ(USART_IRQ);

    return 1;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    uint32_t control = ARM_USART_MODE_ASYNCHRONOUS;

    switch (config->DataBits) {
        case UART_DATA_BITS_5:
            control |= UART_DATA_BITS_5;
            break;

        case UART_DATA_BITS_6:
            control |= ARM_USART_DATA_BITS_6;
            break;

        case UART_DATA_BITS_7:
            control |= ARM_USART_DATA_BITS_6;
            break;

        case UART_DATA_BITS_8: /* fallthrough */
        default:
            control |= ARM_USART_DATA_BITS_8;
            break;
    }

    switch (config->Parity) {
        case UART_PARITY_EVEN:
            control |= ARM_USART_PARITY_EVEN;
            break;

        case UART_PARITY_ODD:
            control |= ARM_USART_PARITY_ODD;
            break;

        case UART_PARITY_NONE: /* fallthrough */
        default:
            control |= ARM_USART_PARITY_NONE;
            break;
    }

    switch (config->StopBits) {
        case UART_STOP_BITS_1: /* fallthrough */
        default:
            control |= ARM_USART_STOP_BITS_1;
            break;

        case UART_STOP_BITS_1_5:
            control |= ARM_USART_STOP_BITS_1_5;
            break;

        case UART_STOP_BITS_2:
            control |= ARM_USART_STOP_BITS_2;
            break;
    }

    switch (config->FlowControl) {
        case UART_FLOW_CONTROL_NONE: /* fallthrough */
        default:
            control |= ARM_USART_FLOW_CONTROL_NONE;
            break;

        case UART_FLOW_CONTROL_RTS_CTS:
            control |= ARM_USART_FLOW_CONTROL_RTS_CTS;
            break;
    }

    NVIC_DisableIRQ(USART_IRQ);
    clear_buffers();
    uint32_t r = USART_INSTANCE.Control(control, config->Baudrate);
    if (r != ARM_DRIVER_OK) {
        return 0;
    }
    USART_INSTANCE.Control(ARM_USART_CONTROL_TX, 1);
    USART_INSTANCE.Control(ARM_USART_CONTROL_RX, 1);
    USART_INSTANCE.Receive(&(cb_buf.rx), 1);

    NVIC_ClearPendingIRQ(USART_IRQ);
    NVIC_EnableIRQ(USART_IRQ);

    return 1;
}

int32_t uart_get_configuration(UART_Configuration *config)
{
    return 1;
}

int32_t uart_write_free(void)
{
    return circ_buf_count_free(&write_buffer);
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    if (size == 0) {
        return 0;
    }

    uint32_t cnt = 0;
    if (circ_buf_count_used(&write_buffer) > 0) {
        cb_buf.tx = circ_buf_pop(&write_buffer);
        cnt = circ_buf_write(&write_buffer, data, size);
    } else {
        cb_buf.tx = data[0];
        cnt = circ_buf_write(&write_buffer, data + 1, size - 1) + 1;
    }
    USART_INSTANCE.Send(&(cb_buf.tx), 1);

    return cnt;
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    return circ_buf_read(&read_buffer, data, size);
}

void uart_handler(uint32_t event) {
   if (event & ARM_USART_EVENT_RECEIVE_COMPLETE) {
        uint32_t free = circ_buf_count_free(&read_buffer);
        if (free > RX_OVRF_MSG_SIZE) {
            circ_buf_push(&read_buffer, cb_buf.rx);
        } else if ((RX_OVRF_MSG_SIZE == free) && config_get_overflow_detect()) {
            circ_buf_write(&read_buffer, (uint8_t*)RX_OVRF_MSG, RX_OVRF_MSG_SIZE);
        } else {
            // Drop character
        }
        USART_INSTANCE.Receive(&(cb_buf.rx), 1);
    }

    if (event & ARM_USART_EVENT_SEND_COMPLETE) {
        if (circ_buf_count_used(&write_buffer) > 0) {
            cb_buf.tx = circ_buf_pop(&write_buffer);
            USART_INSTANCE.Send(&(cb_buf.tx), 1);
        }
    }
}

