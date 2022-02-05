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

#include "uart.h"
#include "util.h"
#include "cortex_m.h"
#include "IO_Config.h"
#include "circ_buf.h"
#include "settings.h" // for config_get_overflow_detect
#include "fsl_clock.h"

#define RX_OVRF_MSG         "<DAPLink:Overflow>\n"
#define RX_OVRF_MSG_SIZE    (sizeof(RX_OVRF_MSG) - 1)
#define BUFFER_SIZE         (512)

circ_buf_t write_buffer;
uint8_t write_buffer_data[BUFFER_SIZE];
circ_buf_t read_buffer;
uint8_t read_buffer_data[BUFFER_SIZE];

void clear_buffers(void)
{
	util_assert(!(UART->CTRL & LPUART_CTRL_TIE_MASK));
    circ_buf_init(&write_buffer, write_buffer_data, sizeof(write_buffer_data));
    circ_buf_init(&read_buffer, read_buffer_data, sizeof(read_buffer_data));
}

int32_t uart_initialize(void)
{
    NVIC_DisableIRQ(UART_RX_TX_IRQn);

    // enable clk port
    if (UART_PORT == PORTA) {
        SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    }

    if (UART_PORT == PORTC) {
        SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    }

    if (UART_PORT == PORTD) {
        SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    }

    if (UART_PORT == PORTE) {
        SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    }

    // enable clk uart
    if (0 == UART_NUM) {
        CLOCK_SetLpuart0Clock(1);
        CLOCK_EnableClock(kCLOCK_Lpuart0);
    }

    if (1 == UART_NUM) {
        CLOCK_SetLpuart1Clock(1);
        CLOCK_EnableClock(kCLOCK_Lpuart1);
    }


    // transmitter and receiver disabled
    UART->CTRL &= ~(LPUART_CTRL_RE_MASK | LPUART_CTRL_TE_MASK);
    // disable interrupt
    UART->CTRL &= ~(LPUART_CTRL_RIE_MASK | LPUART_CTRL_TIE_MASK);

    clear_buffers();

    // alternate setting
    UART_PORT->PCR[PIN_UART_RX_BIT] = PORT_PCR_MUX(PIN_UART_RX_MUX_ALT);
    UART_PORT->PCR[PIN_UART_TX_BIT] = PORT_PCR_MUX(PIN_UART_TX_MUX_ALT);
    // transmitter and receiver enabled
    UART->CTRL |= LPUART_CTRL_RE_MASK | LPUART_CTRL_TE_MASK;
    // Enable receiver interrupt and RX Overrun interrupt
    UART->CTRL |= LPUART_CTRL_RIE_MASK | LPUART_CTRL_ORIE_MASK;
    
    NVIC_SetPriority(UART_RX_TX_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
    NVIC_ClearPendingIRQ(UART_RX_TX_IRQn);
    NVIC_EnableIRQ(UART_RX_TX_IRQn);
    return 1;
}

int32_t uart_uninitialize(void)
{
    // transmitter and receiver disabled
    UART->CTRL &= ~(LPUART_CTRL_RE_MASK | LPUART_CTRL_TE_MASK);
    // disable interrupt
    UART->CTRL &= ~(LPUART_CTRL_RIE_MASK | LPUART_CTRL_TIE_MASK | LPUART_CTRL_ORIE_MASK);
    clear_buffers();

    // disable uart clock
    if (0 == UART_NUM) {
        CLOCK_DisableClock(kCLOCK_Lpuart0);
    }

    if (1 == UART_NUM) {
        CLOCK_DisableClock(kCLOCK_Lpuart1);
    }
    return 1;
}

int32_t uart_reset(void)
{
    // disable interrupt
    NVIC_DisableIRQ(UART_RX_TX_IRQn);
    // disable TIE interrupt
    UART->CTRL &= ~(LPUART_CTRL_TIE_MASK);
    clear_buffers();
    // enable interrupt
    NVIC_EnableIRQ(UART_RX_TX_IRQn);
    return 1;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    uint8_t data_bits = 8;
    uint8_t parity_enable = 0;
    uint8_t parity_type = 0;
    uint32_t dll;
    // disable interrupt
    NVIC_DisableIRQ(UART_RX_TX_IRQn);
    UART->CTRL &= ~(LPUART_CTRL_RIE_MASK | LPUART_CTRL_TIE_MASK);
    // Disable receiver and transmitter while updating
    UART->CTRL &= ~(LPUART_CTRL_RE_MASK | LPUART_CTRL_TE_MASK);
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
    UART->CTRL = data_bits << LPUART_CTRL_M_SHIFT |
               parity_enable << LPUART_CTRL_PE_SHIFT |
               parity_type << LPUART_CTRL_PT_SHIFT;
    dll =  SystemCoreClock / (16 * config->Baudrate);

    // set baudrate
    UART->BAUD = ((UART->BAUD & ~LPUART_BAUD_SBR_MASK) | LPUART_BAUD_SBR(dll));
    // Enable transmitter and receiver
    UART->CTRL |= LPUART_CTRL_RE_MASK | LPUART_CTRL_TE_MASK;
    // Enable UART interrupt
    NVIC_ClearPendingIRQ(UART_RX_TX_IRQn);
    NVIC_EnableIRQ(UART_RX_TX_IRQn);
    UART->CTRL |= LPUART_CTRL_RIE_MASK | LPUART_CTRL_ORIE_MASK;
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

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    cortex_int_state_t state;
    uint32_t cnt;

    cnt = circ_buf_write(&write_buffer, data, size);

    // Atomically enable TX
    state = cortex_int_get_and_disable();
    if (circ_buf_count_used(&write_buffer)) {
        UART->CTRL |= LPUART_CTRL_TIE_MASK;
    }
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

void UART_RX_TX_IRQHandler(void)
{
    uint32_t s1;
    volatile uint8_t errorData;
    // read interrupt status
    s1 = UART->STAT;
    // mask off interrupts that are not enabled
    if (!(UART->CTRL & LPUART_CTRL_RIE_MASK)) {
        s1 &= ~LPUART_STAT_RDRF_MASK;
    }
    if (!(UART->CTRL & LPUART_CTRL_TIE_MASK)) {
        s1 &= ~LPUART_STAT_TDRE_MASK;
    }

    // If RX overrun
    if (s1 & LPUART_STAT_OR_MASK)
    {
        // Clear overrun flag, otherwise the RX does not work.
        UART->STAT = ((UART->STAT & 0x3FE00000U) | LPUART_STAT_OR_MASK);

        if (config_get_overflow_detect()) {
            if (RX_OVRF_MSG_SIZE <= circ_buf_count_free(&read_buffer)) {
                circ_buf_write(&read_buffer, (uint8_t*)RX_OVRF_MSG, RX_OVRF_MSG_SIZE);
            }
        }
    }

    // handle character to transmit
    if (s1 & LPUART_STAT_TDRE_MASK) {
        // Assert that there is data in the buffer
        util_assert(circ_buf_count_used(&write_buffer) > 0);
        // Send out data
        UART->DATA = circ_buf_pop(&write_buffer);
        // Turn off the transmitter if that was the last byte
        if (circ_buf_count_used(&write_buffer) == 0) {
            // disable TIE interrupt
            UART->CTRL &= ~(LPUART_CTRL_TIE_MASK);
            // Clear any pending irq that could be triggered before disabling TIE
            NVIC_ClearPendingIRQ(UART_RX_TX_IRQn);
        }
    }

    // handle received character
    if (s1 & LPUART_STAT_RDRF_MASK) {
        if ((s1 & LPUART_STAT_NF_MASK) || (s1 & LPUART_STAT_FE_MASK)) {
            errorData = UART->DATA;
            // Clear frame error or Noise flags
            UART->STAT = ((UART->STAT & 0x3FE00000U) | LPUART_STAT_NF_MASK | LPUART_STAT_FE_MASK);
        } else {
            uint32_t free;
            uint8_t data;

            data = UART->DATA;
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
}
