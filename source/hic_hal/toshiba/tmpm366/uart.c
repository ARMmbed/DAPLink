/**
 * @file    fuart.c
 * @brief   
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
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

#include "string.h"
#include "TMPM366.h"
#include "uart.h"
#include "circ_buf.h"
#include "cortex_m.h"
#include "IO_Config.h"

circ_buf_t write_buffer;
uint8_t write_buffer_data[BUFFER_SIZE];
circ_buf_t read_buffer;
uint8_t read_buffer_data[BUFFER_SIZE];

static UART_Configuration configuration = {
    .Baudrate    = 9600,
    .DataBits    = UART_DATA_BITS_8,
    .Parity      = UART_PARITY_NONE,
    .StopBits    = UART_STOP_BITS_1,
    .FlowControl = UART_FLOW_CONTROL_NONE,
};

extern uint32_t SystemCoreClock;

static void set_baudrate(uint32_t br)
{
    uint32_t fuartclk = 0U;
    uint32_t ibd = 0U; // Interger part of baud rate divisor
    uint32_t fbd = 0U; // Franctional part of baud rate divisor

    SystemCoreClockUpdate();
    fuartclk = SystemCoreClock; // UARTCLK = fsys

    ibd = fuartclk / (16U * br);
    fbd = (8U * fuartclk + br - 128U * ibd * br) / (2U * br);

    if (fbd == 0U) {
        fbd = 1U; // Franctional part of baud rate divisor can not be 0x00
    } else {
        // Do nothing
    }

    TSB_UART->IBDR = ibd; // Set interger part of baud rate divisor
    TSB_UART->FBDR = fbd; // Set franctional part of baud rate divisor
}

static void uart_init(void)
{
    uint32_t dsp = 0U;
    uint32_t cm  = 0U;

    cm  = UART_FLOW_CONTROL_NONE; // No support flow control
    cm |= (UART_ENABLE_RX | UART_ENABLE_TX);

    dsp  = ((UART_DATA_BITS_8 << 2) | (1 << 6));
    
    dsp |= (UART_PARITY_NONE << 2); 
    dsp |= (UART_STOP_BITS_1 << 3);

    NVIC_DisableIRQ(INTUART_IRQn);

    TSB_UART->LCR_H |= dsp;  // Set DataBits, StopBits, Parity
    TSB_UART->CR    |= cm;   // Set Flow Control, Mode

    TSB_UART_LCR_H_FEN = 0;
    TSB_UART_CR_UARTEN = 1;
    NVIC_EnableIRQ(INTUART_IRQn);
}

static void clear_buffers(void)
{
    circ_buf_init(&write_buffer, write_buffer_data, sizeof(write_buffer_data));
    circ_buf_init(&read_buffer,  read_buffer_data,  sizeof(read_buffer_data));
}

int32_t uart_initialize(void)
{
    // UART pin configuration
    TSB_PG->CR  |= 0x01;
    TSB_PG->FR4 |= 0x01;
    TSB_PG->FR4 |= 0x02;
    TSB_PG->IE  |= 0x02;

    uart_reset();
    TSB_UART->IMSC |= ((1 << UART_IMSC_TXIM) | (1 << UART_IMSC_RXIM));
    set_baudrate(9600);
    uart_init();

    return 1;
}

int32_t uart_uninitialize(void)
{
    uart_reset();
    TSB_UART_CR_UARTEN = 0;
    clear_buffers();

    return 1;
}

int32_t uart_reset(void)
{
    cortex_int_state_t state;

    state = cortex_int_get_and_disable();

    clear_buffers();
    TSB_UART->ICR = 0x30;
    cortex_int_restore(state);

    return 1;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    uart_uninitialize();
    set_baudrate(config->Baudrate);
    uart_init();

    return 1;
}

int32_t uart_get_configuration(UART_Configuration *config)
{
    config->Baudrate = configuration.Baudrate;
    config->DataBits = configuration.DataBits;
    config->Parity   = configuration.Parity;
    config->StopBits = configuration.StopBits;
    config->FlowControl = UART_FLOW_CONTROL_NONE;

    return 1;
}

int32_t uart_write_free(void)
{
    return circ_buf_count_free(&write_buffer);
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    cortex_int_state_t state;

    uint32_t cnt = circ_buf_write(&write_buffer, data, size);

    TSB_UART_CR_TXE = 1;

    state = cortex_int_get_and_disable();
    NVIC_SetPendingIRQ(INTUART_IRQn);
    cortex_int_restore(state);

    return cnt;
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    return circ_buf_read(&read_buffer, data, size);
}

void uart_enable_flow_control(bool enabled)
{
    // Flow control not implemented for this platform
}

static bool SerialReadable(void)
{
    bool ret = false;
    uint32_t src = 0U;

    src = TSB_UART->FR & 0x50;
    if(src == 0x40) { // FR_RX_STORAGE_FULL
        ret = true;
    }

    return ret;
}

static bool SerialWritable(void)
{
    bool ret = false;
    uint32_t src = 0U;

    src = TSB_UART->FR & 0xA0;
    if(src == 0x80) { // FR_TX_STORAGE_EMPTY
        ret = true;
    }

    return ret;
}

static uint8_t SerialGetc(void)
{
    uint8_t data = 0;

    while (!SerialReadable()) {
        // Do nothing
    }
    data = (uint8_t)TSB_UART->DR;

    return data;
}

static void SerialPutc(uint8_t dat)
{
    while (!SerialWritable()) {
        // Do nothing
    }
    TSB_UART->DR = dat;
}

void INTUART_IRQHandler(void)
{
    const uint32_t mis = TSB_UART->MIS;

    if (mis & UART_RXMIS_INT) {
        uint8_t dat   = SerialGetc();
        uint32_t free = circ_buf_count_free(&read_buffer);
        if (free > RX_OVRF_MSG_SIZE) {
            circ_buf_push(&read_buffer, dat);
        } else if (RX_OVRF_MSG_SIZE == free) {
            circ_buf_write(&read_buffer, (uint8_t*)RX_OVRF_MSG, RX_OVRF_MSG_SIZE);
        } else {
            // Drop oldest
            circ_buf_pop(&read_buffer);
            circ_buf_push(&read_buffer, dat);
        }
        TSB_UART->ICR = UART_RXMIS_INT;
    } else if (!(mis & UART_TXMIS_INT)) {
        if (circ_buf_count_used(&write_buffer) > 0) {
            while(TSB_UART_FR_BUSY);
            SerialPutc(circ_buf_pop(&write_buffer));
            TSB_UART->ICR = UART_TXMIS_INT;
        } else {
            TSB_UART_CR_TXE = 0;
        }
    }
}
