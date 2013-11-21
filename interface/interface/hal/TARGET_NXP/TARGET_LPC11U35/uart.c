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
#include <LPC11Uxx.h>
#include "uart.h"

static uint32_t baudrate;
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
    NVIC_DisableIRQ(UART_IRQn);

    LPC_SYSCON->SYSAHBCLKCTRL |= ((1UL <<  6) |   // enable clock for GPIO
                                 (1UL << 16) );   // enable clock for IOCON

    // enable clk for usart
    LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 12);

    // usart clk divider = 1
    LPC_SYSCON->UARTCLKDIV = (1UL << 0);

    // alternate function USART and PullNone
    LPC_IOCON->PIO0_18 |= 0x01;
    LPC_IOCON->PIO0_19 |= 0x01;

    // enable FIFOs (trigger level 1) and clear them
    LPC_USART->FCR = 0x87;

    // Transmit Enable
    LPC_USART->TER     = 0x80;

    // reset uart
    uart_reset();

    // enable rx and tx interrupt
    LPC_USART->IER |= (1 << 0) | (1 << 1);

    NVIC_EnableIRQ(UART_IRQn);

    return 1;
}


int32_t uart_uninitialize (void) {
    // disable interrupt
    LPC_USART->IER &= ~(0x7);
    NVIC_DisableIRQ(UART_IRQn);

    // reset uart
    uart_reset();

    return 1;
}


int32_t uart_reset (void) {

    uint8_t *ptr;
    int32_t  i;

    // disable interrupt
    NVIC_DisableIRQ (UART_IRQn);

    // Reset FIFOs
    LPC_USART->FCR = 0x06;

    baudrate  = 0;
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

    // Ensure a clean start, no data in either TX or RX FIFO
    while (( LPC_USART->LSR & ( (1 << 5) | (1 << 6) ) ) != ( (1 << 5) | (1 << 6) ) );
    while ( LPC_USART->LSR & 0x01 ) {
        LPC_USART->RBR;    // Dump data from RX FIFO
    }

    // enable interrupt
    NVIC_EnableIRQ (UART_IRQn);

    return 1;
}


int32_t uart_set_configuration (UART_Configuration *config) {

    uint8_t DivAddVal = 0;
    uint8_t MulVal = 1;
    uint16_t dlv;
    uint8_t mv, dav, hit = 0, data_bits = 8, parity, stop_bits = 0;
    float ratio, err, calcbaud;

    // disable interrupt
    NVIC_DisableIRQ (UART_IRQn);

    // reset uart
    uart_reset();

    dll =  SystemCoreClock / (16 * config->Baudrate);
    baudrate = config->Baudrate;

    // First we check to see if the basic divide with no DivAddVal/MulVal
    // ratio gives us an integer result. If it does, we set DivAddVal = 0,
    // MulVal = 1. Otherwise, we search the valid ratio value range to find
    // the closest match. This could be more elegant, using search methods
    // and/or lookup tables, but the brute force method is not that much
    // slower, and is more maintainable.
    if ((SystemCoreClock % (16 * config->Baudrate)) != 0) {     // Checking for zero remainder
        float err_best = (float) config->Baudrate;
        unsigned short dlmax = dll;
        for (dlv = dlmax/2; (dlv <= dlmax) && !hit; dlv++) {
            for ( mv = 1; mv <= 15; mv++) {
                for ( dav = 1; dav < mv; dav++) {
                    ratio = 1.0 + ((float) dav / (float) mv);
                    calcbaud = (float)SystemCoreClock / (16.0 * (float) dlv * ratio);
                    err = ((config->Baudrate - calcbaud) > 0) ? (config->Baudrate - calcbaud) : -(config->Baudrate - calcbaud);
                    if (err < err_best) {
                        dll = dlv;
                        DivAddVal = dav;
                        MulVal = mv;
                        err_best = err;
                        if (err < 10) {
                            hit = 1;
                        }
                    }
                }
            }
        }
    }


    // set LCR[DLAB] to enable writing to divider registers
    LPC_USART->LCR |= (1 << 7);

    // set divider values
    LPC_USART->DLM = (dll >> 8) & 0xFF;
    LPC_USART->DLL = (dll >> 0) & 0xFF;
    LPC_USART->FDR = (uint32_t) DivAddVal << 0
                   | (uint32_t) MulVal    << 4;

    // clear LCR[DLAB]
    LPC_USART->LCR &= ~(1 << 7);

    // set data bits, stop bits, parity
    if ((config->DataBits < 5) || (config->DataBits > 8)) {
        data_bits = 8;
    }
    data_bits -= 5;

    if (config->StopBits != 1 && config->StopBits != 2) {
        stop_bits = 1;
    }
    stop_bits -= 1;

    switch (config->Parity) {
        case UART_PARITY_ODD: parity = 0x01; break;     // Parity Odd
        case UART_PARITY_EVEN: parity = 0x03; break;    // Parity Even
        case UART_PARITY_MARK: parity = 0x05; break;    // Parity Mark
        case UART_PARITY_SPACE: parity = 0x07; break;   // Parity Space

        case UART_PARITY_NONE:                          // Parity None
        default:
            parity = 0x00;
        break;
    }

    LPC_USART->LCR = (data_bits << 0)
                   | (stop_bits << 2)
                   | (parity << 3);

    // Enable UART interrupt
    NVIC_EnableIRQ (UART_IRQn);


    return 1;
}


int32_t uart_get_configuration (UART_Configuration *config) {
    float    br;
    uint32_t lcr;

    // line control parameter
    lcr = LPC_USART->LCR;

    // baudrate
    br  = SystemCoreClock / (dll * 16);

    // If inside +/- 2% tolerance
    if (((br * 100) <= (baudrate * 102)) && ((br * 100) >= (baudrate * 98))) {
        config->Baudrate = baudrate;
    } else {
        config->Baudrate = br;
    }

    // get data bits
    switch ((lcr >> 0) & 3) {
        case 0:
            config->DataBits = UART_DATA_BITS_5;
            break;
        case 1:
            config->DataBits = UART_DATA_BITS_6;
            break;
        case 2:
            config->DataBits = UART_DATA_BITS_7;
            break;
        case 3:
            config->DataBits = UART_DATA_BITS_8;
            break;
        default:
            return 0;
    }

    // get parity
    switch ((lcr >> 3) & 7) {
        case 0:
        case 2:
        case 4:
        case 6:
            config->Parity = UART_PARITY_NONE;
            break;
        case 1:
            config->Parity = UART_PARITY_ODD;
            break;
        case 3:
            config->Parity = UART_PARITY_MARK;
            break;
        case 5:
            config->Parity = UART_PARITY_EVEN;
            break;
        case 7:
            config->Parity = UART_PARITY_SPACE;
            break;
        default:
            return 0;
    }

    // get stop bits
    switch ((lcr >> 2) & 1) {
        case 0:
            config->StopBits = UART_STOP_BITS_1;
            break;
        case 1:
            config->StopBits = UART_STOP_BITS_2;
            break;
        default:
            return 0;
    }

    // get flow control
    config->FlowControl = UART_FLOW_CONTROL_NONE;

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

    // enable THRE interrupt
    LPC_USART->IER |= (1 << 1);

    if (!tx_in_progress) {
        // force THRE interrupt to start
        NVIC_SetPendingIRQ(UART_IRQn);
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


void UART_IRQHandler (void) {
    uint32_t iir;
    int16_t  len_in_buf;

    // read interrupt status
    iir = LPC_USART->IIR;

    // handle character to transmit
    if (write_buffer.cnt_in != write_buffer.cnt_out) {
        // if THR is empty
        if (LPC_USART->LSR & (1 << 5)) {
            LPC_USART->THR = write_buffer.data[write_buffer.idx_out++];
            write_buffer.idx_out &= (BUFFER_SIZE - 1);
            write_buffer.cnt_out++;
            tx_in_progress = 1;
        }
    } else if (tx_in_progress) {
        tx_in_progress = 0;
        // disable THRE interrupt
        LPC_USART->IER &= ~(1 << 1);
    }

    // handle received character
    if (((iir & 0x0E) == 0x04)  ||        // Rx interrupt (RDA)
        ((iir & 0x0E) == 0x0C))  {        // Rx interrupt (CTI)
        while (LPC_USART->LSR & 0x01) {
            len_in_buf = read_buffer.cnt_in - read_buffer.cnt_out;
            read_buffer.data[read_buffer.idx_in++] = LPC_USART->RBR;
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

    LPC_USART->LSR;
}

/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/

