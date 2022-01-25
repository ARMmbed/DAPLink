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
    // Number of bytes pending to be transferred. This is 0 if there is no
    // ongoing transfer and the uart_handler processed the last transfer.
    volatile uint32_t tx_size;

    uint8_t rx;
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
    cb_buf.tx_size = 0;
    USART_INSTANCE.Initialize(uart_handler);
    USART_INSTANCE.PowerControl(ARM_POWER_FULL);

    return 1;
}

int32_t uart_uninitialize(void)
{
    USART_INSTANCE.Control(ARM_USART_CONTROL_RX, 0);
    USART_INSTANCE.Control(ARM_USART_ABORT_RECEIVE, 0U);
    USART_INSTANCE.PowerControl(ARM_POWER_OFF);
    USART_INSTANCE.Uninitialize();
    clear_buffers();
    cb_buf.tx_size = 0;

    return 1;
}

int32_t uart_reset(void)
{
    // disable interrupt
    NVIC_DisableIRQ(USART_IRQ);
    clear_buffers();
    if (cb_buf.tx_size != 0) {
        USART_INSTANCE.Control(ARM_USART_ABORT_SEND, 0U);
        cb_buf.tx_size = 0;
    }
    // enable interrupt
    NVIC_EnableIRQ(USART_IRQ);

    return 1;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    uint32_t control = ARM_USART_MODE_ASYNCHRONOUS;

    switch (config->DataBits) {
        case UART_DATA_BITS_5:
            control |= ARM_USART_DATA_BITS_5;
            break;

        case UART_DATA_BITS_6:
            control |= ARM_USART_DATA_BITS_6;
            break;

        case UART_DATA_BITS_7:
            control |= ARM_USART_DATA_BITS_7;
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
    if (cb_buf.tx_size != 0) {
        USART_INSTANCE.Control(ARM_USART_ABORT_SEND, 0U);
        cb_buf.tx_size = 0;
    }

    // If there was no Receive() call in progress aborting it is harmless.
    USART_INSTANCE.Control(ARM_USART_CONTROL_RX, 0U);
    USART_INSTANCE.Control(ARM_USART_ABORT_RECEIVE, 0U);

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

void uart_set_control_line_state(uint16_t ctrl_bmp)
{
}

int32_t uart_write_free(void)
{
    return circ_buf_count_free(&write_buffer);
}

// Start a new TX transfer if there are bytes pending to be transferred on the
// write_buffer buffer. The transferred bytes are not removed from the circular
// by this function, only the event handler will remove them once the transfer
// is done.
static void uart_start_tx_transfer() {
    uint32_t tx_size = 0;
    const uint8_t* buf = circ_buf_peek(&write_buffer, &tx_size);
    if (tx_size > BUFFER_SIZE / 4) {
        // The bytes being transferred remain on the circular buffer memory
        // until the transfer is done. Limiting the UART transfer size
        // allows the uart_handler to clear those bytes earlier.
        tx_size = BUFFER_SIZE / 4;
    }
    cb_buf.tx_size = tx_size;
    if (tx_size) {
        USART_INSTANCE.Send(buf, tx_size);
    }
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    if (size == 0) {
        return 0;
    }

    uint32_t cnt = circ_buf_write(&write_buffer, data, size);
    if (cb_buf.tx_size == 0) {
        // There's no pending transfer and the value of cb_buf.tx_size will not
        // change to non-zero by the event handler once it is zero. Note that it
        // is entirely possible that we transferred all the bytes we added to
        // the circular buffer in this function by the time we are in this
        // branch, in that case uart_start_tx_transfer() would not schedule any
        // transfer.
        uart_start_tx_transfer();
    }

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
        circ_buf_pop_n(&write_buffer, cb_buf.tx_size);
        uart_start_tx_transfer();
    }
}
