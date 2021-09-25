/**
 * @file    uart.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

#include "MK22F51212.h"
#include "uart.h"

extern uint32_t SystemCoreClock;

static void clear_buffers(void);

// Size must be 2^n for using quick wrap around
#define  BUFFER_SIZE (512)

struct {
    uint8_t  data[BUFFER_SIZE];
    volatile uint32_t idx_in;
    volatile uint32_t idx_out;
    volatile uint32_t cnt_in;
    volatile uint32_t cnt_out;
} write_buffer, read_buffer;

uint32_t tx_in_progress = 0;

void clear_buffers(void)
{
    memset((void *)&read_buffer, 0xBB, sizeof(read_buffer.data));
    read_buffer.idx_in = 0;
    read_buffer.idx_out = 0;
    read_buffer.cnt_in = 0;
    read_buffer.cnt_out = 0;
    memset((void *)&write_buffer, 0xBB, sizeof(read_buffer.data));
    write_buffer.idx_in = 0;
    write_buffer.idx_out = 0;
    write_buffer.cnt_in = 0;
    write_buffer.cnt_out = 0;
}

int32_t uart_initialize(void)
{
    NVIC_DisableIRQ(UART1_RX_TX_IRQn);
    clear_buffers();
    // enable clk PORTC
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    // enable clk uart
    SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
    // disable interrupt
    NVIC_DisableIRQ(UART1_RX_TX_IRQn);
    // Enable receiver and transmitter
    UART1->C2 |= UART_C2_RE_MASK | UART_C2_TE_MASK;
    // alternate 3: UART1
    PORTC->PCR[3] = (3 << 8);
    PORTC->PCR[4] = (3 << 8);
    // Enable receive interrupt
    UART1->C2 |= UART_C2_RIE_MASK;
    NVIC_ClearPendingIRQ(UART1_RX_TX_IRQn);
    NVIC_EnableIRQ(UART1_RX_TX_IRQn);
    return 1;
}

int32_t uart_uninitialize(void)
{
    // transmitter and receiver disabled
    UART1->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);
    // disable interrupt
    UART1->C2 &= ~(UART_C2_RIE_MASK | UART_C2_TIE_MASK);
    clear_buffers();
    return 1;
}

int32_t uart_reset(void)
{
    // disable interrupt
    NVIC_DisableIRQ(UART1_RX_TX_IRQn);
    clear_buffers();
    // disable TIE interrupt
    UART1->C2 &= ~(UART_C2_TIE_MASK);
    tx_in_progress = 0;
    // enable interrupt
    NVIC_EnableIRQ(UART1_RX_TX_IRQn);
    return 1;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    uint8_t data_bits = 8;
    uint8_t parity_enable = 0;
    uint8_t parity_type = 0;
    uint32_t dll;
    // disable interrupt
    NVIC_DisableIRQ(UART1_RX_TX_IRQn);
    // Disable receiver and transmitter while updating
    UART1->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);
    clear_buffers();

    // set data bits, stop bits, parity
    if ((config->DataBits < 8) || (config->DataBits > 9)) {
        data_bits = 8;
    }

    data_bits -= 8;

    if (config->Parity == 1) {
        parity_enable = 1;
        parity_type = 1;
        data_bits++;
    } else if (config->Parity == 2) {
        parity_enable = 1;
        parity_type = 0;
        data_bits++;
    }

    // does not support 10 bit data comm
    if (data_bits == 2) {
        data_bits = 0;
        parity_enable = 0;
        parity_type = 0;
    }

    // data bits, parity and parity mode
    UART1->C1 = data_bits << UART_C1_M_SHIFT
                | parity_enable << UART_C1_PE_SHIFT
                | parity_type << UART_C1_PT_SHIFT;
    dll = SystemCoreClock / (16 * config->Baudrate);
    // set baudrate
    UART1->BDH = (UART1->BDH & ~(UART_BDH_SBR_MASK)) | ((dll >> 8) & UART_BDH_SBR_MASK);
    UART1->BDL = (UART1->BDL & ~(UART_BDL_SBR_MASK)) | (dll & UART_BDL_SBR_MASK);
    // Enable transmitter and receiver
    UART1->C2 |= UART_C2_RE_MASK | UART_C2_TE_MASK;
    // Enable UART interrupt
    NVIC_ClearPendingIRQ(UART1_RX_TX_IRQn);
    NVIC_EnableIRQ(UART1_RX_TX_IRQn);
    return 1;
}

int32_t uart_get_configuration(UART_Configuration *config)
{
    return 1;
}

int32_t uart_write_free(void)
{
    return BUFFER_SIZE - (write_buffer.cnt_in - write_buffer.cnt_out);
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    uint32_t cnt;
    int16_t  len_in_buf;

    if (size == 0) {
        return 0;
    }

    cnt = 0;

    while (size--) {
        len_in_buf = write_buffer.cnt_in - write_buffer.cnt_out;

        if (len_in_buf < BUFFER_SIZE) {
            write_buffer.data[write_buffer.idx_in++] = *data++;
            write_buffer.idx_in &= (BUFFER_SIZE - 1);
            write_buffer.cnt_in++;
            cnt++;
        }
    }

    if (!tx_in_progress) {
        // Wait for D register to be free
        while (!(UART1->S1 & UART_S1_TDRE_MASK)) { }

        tx_in_progress = 1;
        // Write the first byte into D
        UART1->D = write_buffer.data[write_buffer.idx_out++];
        write_buffer.idx_out &= (BUFFER_SIZE - 1);
        write_buffer.cnt_out++;
        // enable TX interrupt
        UART1->C2 |= UART_C2_TIE_MASK;
    }

    return cnt;
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    uint32_t cnt;

    if (size == 0) {
        return 0;
    }

    cnt = 0;

    while (size--) {
        if (read_buffer.cnt_in != read_buffer.cnt_out) {
            *data++ = read_buffer.data[read_buffer.idx_out++];
            read_buffer.idx_out &= (BUFFER_SIZE - 1);
            read_buffer.cnt_out++;
            cnt++;
        } else {
            break;
        }
    }

    return cnt;
}

void UART1_RX_TX_IRQHandler(void)
{
    uint32_t s1;
    volatile uint8_t errorData;
    // read interrupt status
    s1 = UART1->S1;

    // handle character to transmit
    if (write_buffer.cnt_in != write_buffer.cnt_out) {
        // if TDRE is empty
        if (s1 & UART_S1_TDRE_MASK) {
            UART1->D = write_buffer.data[write_buffer.idx_out++];
            write_buffer.idx_out &= (BUFFER_SIZE - 1);
            write_buffer.cnt_out++;
            tx_in_progress = 1;
        }
    } else {
        // disable TIE interrupt
        UART1->C2 &= ~(UART_C2_TIE_MASK);
        tx_in_progress = 0;
    }

    // handle received character
    if (s1 & UART_S1_RDRF_MASK) {
        if ((s1 & UART_S1_NF_MASK) || (s1 & UART_S1_FE_MASK)) {
            errorData = UART1->D;
        } else {
            read_buffer.data[read_buffer.idx_in++] = UART1->D;
            read_buffer.idx_in &= (BUFFER_SIZE - 1);
            read_buffer.cnt_in++;
        }
    }
}
