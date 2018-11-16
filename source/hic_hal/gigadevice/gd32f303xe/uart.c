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

#include "gd32f30x.h"
#include "uart.h"
#include "util.h"
#include "circ_buf.h"
#include "settings.h" // for config_get_overflow_detect

static uint32_t tx_in_progress;

extern uint32_t SystemCoreClock;

#define RX_OVRF_MSG         "<DAPLink:Overflow>\n"
#define RX_OVRF_MSG_SIZE    (sizeof(RX_OVRF_MSG) - 1)
#define BUFFER_SIZE         (64)

#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK                    RCU_USART1
#define EVAL_COM1_TX_PIN                 GPIO_PIN_2
#define EVAL_COM1_RX_PIN                 GPIO_PIN_3
#define EVAL_COM1_GPIO_PORT              GPIOA
#define EVAL_COM1_GPIO_CLK               RCU_GPIOA

circ_buf_t write_buffer;
uint8_t write_buffer_data[BUFFER_SIZE];
circ_buf_t read_buffer;
uint8_t read_buffer_data[BUFFER_SIZE];

static int32_t reset(void);

int32_t uart_initialize(void)
{
    nvic_irq_disable(USART1_IRQn);

    /* enable GPIO clock */
    rcu_periph_clock_enable(EVAL_COM1_GPIO_CLK);

    rcu_periph_clock_enable(RCU_AF);

    /* enable USART clock */
    rcu_periph_clock_enable(EVAL_COM1_CLK);

    /* connect port to USARTx_Tx */
    gpio_initial(EVAL_COM1_GPIO_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, EVAL_COM1_TX_PIN);

    /* connect port to USARTx_Rx */
    gpio_initial(EVAL_COM1_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, EVAL_COM1_RX_PIN);

    usart_deinit(EVAL_COM1);

    reset();

    usart_baudrate_set(EVAL_COM1, 115200U);
    usart_receive_config(EVAL_COM1, USART_RECEIVE_ENABLE);
    usart_transmit_config(EVAL_COM1, USART_TRANSMIT_ENABLE);
    usart_enable(EVAL_COM1);

    usart_interrupt_enable(EVAL_COM1, USART_INT_RBNE);

    /* USART interrupt configuration */
    nvic_irq_enable(USART1_IRQn, 0, 0);

    return 1;
}

int32_t uart_uninitialize(void)
{
    nvic_irq_disable(USART1_IRQn);

    usart_receive_config(EVAL_COM1, USART_RECEIVE_DISABLE);
    usart_transmit_config(EVAL_COM1, USART_TRANSMIT_DISABLE);

    usart_interrupt_disable(EVAL_COM1, USART_INT_TBE);
    usart_interrupt_disable(EVAL_COM1, USART_INT_RBNE);

    reset();

    return 1;
}

int32_t uart_reset(void)
{
    nvic_irq_disable(USART1_IRQn);

    usart_interrupt_disable(EVAL_COM1, USART_INT_TBE);

    reset();

    nvic_irq_enable(USART1_IRQn, 0, 0);

    return 1;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    uint32_t parity, word_len, stop_bits;

    nvic_irq_disable(USART1_IRQn);

    usart_disable(EVAL_COM1);

    usart_receive_config(EVAL_COM1, USART_RECEIVE_DISABLE);
    usart_transmit_config(EVAL_COM1, USART_TRANSMIT_DISABLE);

    usart_interrupt_disable(EVAL_COM1, USART_INT_TBE);
    usart_interrupt_disable(EVAL_COM1, USART_INT_RBNE);

    reset();

    usart_baudrate_set(EVAL_COM1, config->Baudrate);

    switch (config->Parity) {
    case UART_PARITY_ODD:
        parity = USART_PM_ODD;
        break;
    case UART_PARITY_EVEN:
        parity = USART_PM_EVEN;
        break;
    case UART_PARITY_MARK:
    case UART_PARITY_SPACE:
    case UART_PARITY_NONE:
    default:
        parity = USART_PM_NONE;
        break;
    }

    usart_parity_config(EVAL_COM1, parity);

    if (config->DataBits <= 8) {
        word_len = USART_WL_8BIT;
    } else {
        word_len = USART_WL_9BIT;
    }

    usart_word_length_set(EVAL_COM1, word_len);

    switch (config->StopBits) {
    case UART_STOP_BITS_1:
        stop_bits = USART_STB_1BIT;
        break;
    case UART_STOP_BITS_1_5:
        stop_bits = USART_STB_1_5BIT;
        break;
    case UART_STOP_BITS_2:
        stop_bits = USART_STB_2BIT;
        break;
    default:
        stop_bits = USART_STB_0_5BIT;
        break;
    }

    usart_stop_bit_set(EVAL_COM1, stop_bits);

    usart_receive_config(EVAL_COM1, USART_RECEIVE_ENABLE);
    usart_transmit_config(EVAL_COM1, USART_TRANSMIT_ENABLE);
    usart_enable(EVAL_COM1);

    usart_interrupt_enable(EVAL_COM1, USART_INT_RBNE);

    /* USART interrupt configuration */
    nvic_irq_enable(USART1_IRQn, 0, 0);

    return 1;
}

int32_t uart_get_configuration(UART_Configuration *config)
{
    uint32_t uclk, udiv, parity, word_len, stop_bits;

    uclk = rcu_clock_freq_get(CK_APB2);

    udiv = USART_BAUD(EVAL_COM1) & (USART_BAUD_FRADIV | USART_BAUD_INTDIV);

    config->Baudrate = (2 * uclk) / (2 * udiv - 1); 

    parity = (USART_CTL0(EVAL_COM1) & USART_CTL0_PM) >> 9;

    if (parity == 0) {
        config->Parity = UART_PARITY_NONE;
    } else if (parity == 2) {
        config->Parity = UART_PARITY_EVEN;
    } else if (parity == 3) {
        config->Parity = UART_PARITY_ODD;
    } else {
    }

    word_len = (USART_CTL0(EVAL_COM1) & USART_CTL0_WL) >> 12;

    if (word_len == 0) {
        config->DataBits = UART_DATA_BITS_8;
    } else {
        config->DataBits = UART_DATA_BITS_16;
    }

    stop_bits = (USART_CTL1(EVAL_COM1) & USART_CTL1_STB) >> 12;

    if (stop_bits == 0) {
        config->StopBits = UART_STOP_BITS_1;
    } else if (stop_bits == 1) {
        config->StopBits = UART_STOP_BITS_1;
    } else if (stop_bits == 2) {
        config->StopBits = UART_STOP_BITS_2;
    } else if (stop_bits == 3) {
        config->StopBits = UART_STOP_BITS_1_5;
    } else {
    }

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

    if (!tx_in_progress) {
        /* enable USART TBE interrupt */  
        usart_interrupt_enable(USART1, USART_INT_TBE);
    }

    return cnt;
}


int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    return circ_buf_read(&read_buffer, data, size);
}

void uart_enable_flow_control(bool enabled)
{
    /* Flow control not implemented for this platform */
}

void USART1_IRQHandler(void)
{
    // handle character to transmit
    if (circ_buf_count_used(&write_buffer) > 0) {
        if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_TBE)) {
            /* transmit data */
            usart_data_transmit(USART1, circ_buf_pop(&write_buffer));

            tx_in_progress = 1;
        }
    } else if (tx_in_progress) {
        tx_in_progress = 0;

        usart_interrupt_disable(USART1, USART_INT_TBE);
    }

    /* handle received character */
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE)) {
        uint32_t free;
        uint8_t data;

        data = usart_data_receive(USART1);
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

static int32_t reset(void)
{
    tx_in_progress = 0;

    circ_buf_init(&write_buffer, write_buffer_data, sizeof(write_buffer_data));
    circ_buf_init(&read_buffer, read_buffer_data, sizeof(read_buffer_data));

    return 1;
}
