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
#include <MK20D5.h>
#include "uart.h"

static uint32_t dll;
static uint32_t tx_in_progress;

extern uint32_t SystemCoreClock;

// Size must be 2^n
#define  BUFFER_SIZE          (64)


static struct {
    uint8_t  data[BUFFER_SIZE];
    volatile uint16_t idx_in;
    volatile uint16_t idx_out;
    volatile  int16_t cnt_in;
    volatile  int16_t cnt_out;
} write_buffer, read_buffer;


int32_t uart_initialize (void) {
    NVIC_DisableIRQ(UART1_RX_TX_IRQn);

    // enable clk PORTC
    SIM->SCGC5 |= (1UL << 11);
    // enable clk uart
    SIM->SCGC4 |= (1 << 11);

    // alternate 3: UART1
    PORTC->PCR[3] = (3 << 8);
    PORTC->PCR[4] = (3 << 8);

    // activate fifo (64 datawords)
    UART1->PFIFO = (5 << 0) | (1 << 3) | (5 << 4) | (1 << 7);

    // transmitter and receiver enabled
    UART1->C2 |= (1 << 2) | (1 << 3);

    // disable interrupt
    UART1->C2 |= (1 << 5);

    NVIC_EnableIRQ(UART1_RX_TX_IRQn);

    return 1;
}


int32_t uart_uninitialize (void) {
    // transmitter and receiver disabled
    UART1->C2 &= ~((1 << 2) | (1 << 3));

    // disable interrupt
    UART1->C2 &= ~((1 << 5) | (1 << 6));

    // reset uart
    uart_reset();

    return 1;
}


int32_t uart_reset (void) {
    uint8_t *ptr;
    int32_t  i;

    // disable interrupt
    NVIC_DisableIRQ (UART1_RX_TX_IRQn);

    dll       = 0;
    tx_in_progress = 0;

    ptr = (uint8_t *)&write_buffer;
    for (i = 0; i < sizeof(write_buffer); i++) {
        *ptr++ = 0;
    }

    ptr = (uint8_t *)&read_buffer;
    for (i = 0; i < sizeof(read_buffer); i++) {
        *ptr++ = 0;
    }

    // enable interrupt
    NVIC_EnableIRQ (UART1_RX_TX_IRQn);

    return 1;
}


int32_t uart_set_configuration (UART_Configuration *config) {

    uint8_t data_bits = 8;
    uint8_t parity_enable = 0;
    uint8_t parity_type = 0;

    // disable interrupt
    NVIC_DisableIRQ (UART1_RX_TX_IRQn);

    // reset uart
    uart_reset();

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
    UART1->C1 = data_bits << 4
              | parity_enable << 1
              | parity_type << 0;

    dll =  SystemCoreClock / (16 * config->Baudrate);

    // set baudrate
    UART1->BDH = (UART1->BDH & ~(0x1f)) | ((dll >> 8) & 0x1f);
    UART1->BDL = (UART1->BDL & ~(0xff)) | dll & 0xff;

    // Enable UART interrupt
    NVIC_EnableIRQ (UART1_RX_TX_IRQn);

    return 1;
}


int32_t uart_get_configuration (UART_Configuration *config) {
    return 1;
}

int32_t uart_write_free(void) {
    return BUFFER_SIZE - (write_buffer.cnt_in - write_buffer.cnt_out);
}

int32_t uart_write_data (uint8_t *data, uint16_t size) {
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

    // enable TX interrupt
    UART1->C2 |= (1 << 6);

    if (!tx_in_progress) {
        // force THRE interrupt to start
        NVIC_SetPendingIRQ(UART1_RX_TX_IRQn);
    }

    return cnt;
}


int32_t uart_read_data (uint8_t *data, uint16_t size) {
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
        }
    }

    return cnt;
}


void UART1_RX_TX_IRQHandler (void) {
    uint32_t s1;
    int16_t  len_in_buf;

    // read interrupt status
    s1 = UART1->S1;

    // handle character to transmit
    if (write_buffer.cnt_in != write_buffer.cnt_out) {
        // if THR is empty
        if (s1 & UART_S1_TC_MASK) {
            UART1->D = write_buffer.data[write_buffer.idx_out++];
            write_buffer.idx_out &= (BUFFER_SIZE - 1);
            write_buffer.cnt_out++;
            tx_in_progress = 1;
        }
    } else if (tx_in_progress) {
        tx_in_progress = 0;
        // disable THRE interrupt
        UART1->C2 &= ~(1 << 6);
    }

    // handle received character
    if (s1 & UART_S1_RDRF_MASK) {
        while (!(UART1->SFIFO & (1 << 6))) {
            len_in_buf = read_buffer.cnt_in - read_buffer.cnt_out;
            read_buffer.data[read_buffer.idx_in++] = UART1->D;
            read_buffer.idx_in &= (BUFFER_SIZE - 1);
            read_buffer.cnt_in++;
            // if buffer full: write by dropping oldest characters
            if (len_in_buf == BUFFER_SIZE) {
                read_buffer.idx_out++;
                read_buffer.idx_out &= (BUFFER_SIZE - 1);
                read_buffer.cnt_out++;
            }
        }
    }
}

/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/

