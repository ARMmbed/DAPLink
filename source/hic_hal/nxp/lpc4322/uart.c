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

#include "LPC43xx.h"
#include "uart.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_scu.h"
#include "util.h"
#include "circ_buf.h"
#include "settings.h" // for config_get_overflow_detect

static uint32_t baudrate;
static uint32_t dll;
static uint32_t tx_in_progress;

extern uint32_t SystemCoreClock;

#define RX_OVRF_MSG         "<DAPLink:Overflow>\n"
#define RX_OVRF_MSG_SIZE    (sizeof(RX_OVRF_MSG) - 1)
#define  BUFFER_SIZE    (512)

circ_buf_t write_buffer;
uint8_t write_buffer_data[BUFFER_SIZE];
circ_buf_t read_buffer;
uint8_t read_buffer_data[BUFFER_SIZE];

static int32_t reset(void);

#define UART_IRQn  USART0_IRQn
#define LPC_USART  LPC_USART0
#define UART_IRQHandler  UART0_IRQHandler

// UART Control Pin           P2_2:  GPIO5[2]
#define PORT_UARTCTRL         5
#define PIN_UARTCTRL_IN_BIT   2
#define PIN_UARTCTRL          (1<<PIN_UARTCTRL_IN_BIT)


int32_t uart_initialize(void)
{
    NVIC_DisableIRQ(UART_IRQn);

    // The baudrate calculations require the UART to be clocked as SystemCoreClock
    CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_UART0);
    CGU_EnableEntity(CGU_BASE_UART0, ENABLE);
    scu_pinmux(2, 0, UART_RX_TX, FUNC1);   /* P2_0: U0_TXD */
    scu_pinmux(2, 1, UART_RX_TX, FUNC1);   /* P2_1: U0_RXD */

    scu_pinmux(2,  2, GPIO_NOPULL, FUNC4);  /* UARTCTRL:  GPIO5[2]  */
    // Control target's UART RX:
    //   UARTCTRL high:  The LPC1549 gets uart input from the LPC4322
    //   UARTCTRL low:   The LPC1549 gets uart input from the ISP_RX on the pinlist
    LPC_GPIO_PORT->CLR[PORT_UARTCTRL] = PIN_UARTCTRL;
    LPC_GPIO_PORT->DIR[PORT_UARTCTRL] |= (PIN_UARTCTRL);
    // enable FIFOs (trigger level 1) and clear them
    LPC_USART->FCR = 0x87;
    // Transmit Enable
    LPC_USART->TER     = 0x01;
    // reset uart
    reset();
    // enable rx and tx interrupt
    LPC_USART->IER |= (1 << 0) | (1 << 1);
    NVIC_EnableIRQ(UART_IRQn);
    return 1;
}

int32_t uart_uninitialize(void)
{
    // disable interrupt
    LPC_USART->IER &= ~(0x7);
    NVIC_DisableIRQ(UART_IRQn);
    // reset uart
    reset();
    return 1;
}

int32_t uart_reset(void)
{
    // disable interrupt
    NVIC_DisableIRQ(UART_IRQn);
    // reset uart
    reset();
    // enable interrupt
    NVIC_EnableIRQ(UART_IRQn);
    return 1;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    uint8_t DivAddVal = 0;
    uint8_t MulVal = 1;
    uint8_t mv, data_bits = 8, parity, stop_bits = 0;
    // disable interrupt
    NVIC_DisableIRQ(UART_IRQn);
    // reset uart
    reset();
    baudrate = config->Baudrate;
    // Compute baud rate dividers
    mv = 15;
    dll = util_div_round_down(SystemCoreClock, 16 * config->Baudrate);
    DivAddVal = util_div_round(SystemCoreClock * mv, dll * config->Baudrate * 16) - mv;
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
        case UART_PARITY_ODD:
            parity = 0x01;
            break;     // Parity Odd

        case UART_PARITY_EVEN:
            parity = 0x03;
            break;    // Parity Even

        case UART_PARITY_MARK:
            parity = 0x05;
            break;    // Parity Mark

        case UART_PARITY_SPACE:
            parity = 0x07;
            break;   // Parity Space

        case UART_PARITY_NONE:                          // Parity None
        default:
            parity = 0x00;
            break;
    }

    LPC_USART->LCR = (data_bits << 0)
                     | (stop_bits << 2)
                     | (parity << 3);
    // Enable UART interrupt
    NVIC_EnableIRQ(UART_IRQn);
    return 1;
}

int32_t uart_get_configuration(UART_Configuration *config)
{
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

int32_t uart_write_free(void)
{
    return circ_buf_count_free(&write_buffer);
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    uint32_t cnt;

    cnt = circ_buf_write(&write_buffer, data, size);

    // Make sure that the target LPC1549 can receive the output
    LPC_GPIO_PORT->SET[PORT_UARTCTRL] = PIN_UARTCTRL;

    // enable THRE interrupt
    LPC_USART->IER |= (1 << 1);

    if (!tx_in_progress) {
        // force THRE interrupt to start
        NVIC_SetPendingIRQ(UART_IRQn);
    }

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

void UART_IRQHandler(void)
{
    uint32_t iir;
    // read interrupt status
    iir = LPC_USART->IIR;

    // handle character to transmit
    if (circ_buf_count_used(&write_buffer) > 0) {
        // if THR is empty
        if (LPC_USART->LSR & (1 << 5)) {
            LPC_USART->THR = circ_buf_pop(&write_buffer);
            tx_in_progress = 1;
        }

    } else if (tx_in_progress) {
        tx_in_progress = 0;
        // Turn back input for the target LPC1549 to it's pinlist
        LPC_GPIO_PORT->CLR[PORT_UARTCTRL] = PIN_UARTCTRL;
        // disable THRE interrupt
        LPC_USART->IER &= ~(1 << 1);
    }

    // handle received character
    if (((iir & 0x0E) == 0x04)  ||        // Rx interrupt (RDA)
            ((iir & 0x0E) == 0x0C))  {        // Rx interrupt (CTI)
        while (LPC_USART->LSR & 0x01) {
            uint32_t free;
            uint8_t data;

            data = LPC_USART->RBR;
            free = circ_buf_count_free(&read_buffer);
            if (free > RX_OVRF_MSG_SIZE) {
                circ_buf_push(&read_buffer, data);
            } else if (config_get_overflow_detect()) {
                if (RX_OVRF_MSG_SIZE == free) {
                    circ_buf_write(&read_buffer, (uint8_t*)RX_OVRF_MSG, RX_OVRF_MSG_SIZE);
                } else {
                    // Drop newest
                }
            } else {
                // Drop oldest
                circ_buf_pop(&read_buffer);
                circ_buf_push(&read_buffer, data);
            }
        }
    }

    LPC_USART->LSR;
}

static int32_t reset(void)
{
    // Reset FIFOs
    LPC_USART->FCR = 0x06;
    baudrate  = 0;
    dll       = 0;
    tx_in_progress = 0;

    circ_buf_init(&write_buffer, write_buffer_data, sizeof(write_buffer_data));
    circ_buf_init(&read_buffer, read_buffer_data, sizeof(read_buffer_data));

    // Ensure a clean start, no data in either TX or RX FIFO
    while ((LPC_USART->LSR & ((1 << 5) | (1 << 6))) != ((1 << 5) | (1 << 6)));

    while (LPC_USART->LSR & 0x01) {
        LPC_USART->RBR;    // Dump data from RX FIFO
    }

    return 1;
}
