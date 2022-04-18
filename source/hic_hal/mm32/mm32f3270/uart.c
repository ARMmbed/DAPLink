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

#include "mm32_device.h"
#include "hal_gpio.h"
#include "hal_uart.h"
#include "hal_misc.h"
#include "uart.h"
#include "gpio.h"
#include "util.h"
#include "circ_buf.h"
#include "IO_Config.h"

// For uart
#define CDC_UART                     UART1
#define CDC_UART_ENABLE()            RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE)
#define CDC_UART_DISABLE()           RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE)
#define CDC_UART_IRQn                UART1_IRQn
#define CDC_UART_IRQn_Handler        UART1_IRQHandler

#define UART_PINS_PORT_ENABLE()      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)
#define UART_PINS_PORT_DISABLE()     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)

#define UART_TX_PORT                 GPIOB
#define UART_TX_PIN                  GPIO_Pin_6

#define UART_RX_PORT                 GPIOB
#define UART_RX_PIN                  GPIO_Pin_7

// #define UART_CTS_PORT                GPIOA
// #define UART_CTS_PIN                 GPIO_PIN_0

// #define UART_RTS_PORT                GPIOA
// #define UART_RTS_PIN                 GPIO_PIN_1


#define RX_OVRF_MSG         "<DAPLink:Overflow>\n"
#define RX_OVRF_MSG_SIZE    (sizeof(RX_OVRF_MSG) - 1)
#define BUFFER_SIZE         (512)

circ_buf_t write_buffer;
uint8_t write_buffer_data[BUFFER_SIZE];
circ_buf_t read_buffer;
uint8_t read_buffer_data[BUFFER_SIZE];

static UART_Configuration configuration = {
    .Baudrate = 9600,
    .DataBits = UART_DATA_BITS_8,
    .Parity = UART_PARITY_NONE,
    .StopBits = UART_STOP_BITS_1,
    .FlowControl = UART_FLOW_CONTROL_NONE,
};

extern uint32_t SystemCoreClock;



static void clear_buffers(void)
{
    circ_buf_init(&write_buffer, write_buffer_data, sizeof(write_buffer_data));
    circ_buf_init(&read_buffer, read_buffer_data, sizeof(read_buffer_data));
}

int32_t uart_initialize(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    UART_ITConfig(CDC_UART, UART_IT_RXIEN|UART_IT_TXIEN, DISABLE);
    clear_buffers();

    //TX pin
    GPIO_InitStructure.GPIO_Pin = UART_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(UART_TX_PORT, &GPIO_InitStructure);

    //RX pin
    GPIO_InitStructure.GPIO_Pin = UART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART_RX_PORT, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = CDC_UART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_ClearPendingIRQ(CDC_UART_IRQn);


    // CDC_UART->CR1 &= ~(USART_IT_TXE | USART_IT_RXNE);
    // clear_buffers();

    // CDC_UART_ENABLE();
    // UART_PINS_PORT_ENABLE();

    // //TX pin
    // GPIO_InitStructure.Pin = UART_TX_PIN;
    // GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    // GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    // HAL_GPIO_Init(UART_TX_PORT, &GPIO_InitStructure);
    // //RX pin
    // GPIO_InitStructure.Pin = UART_RX_PIN;
    // GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    // GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    // GPIO_InitStructure.Pull = GPIO_PULLUP;
    // HAL_GPIO_Init(UART_RX_PORT, &GPIO_InitStructure);
    // //CTS pin, input
    // GPIO_InitStructure.Pin = UART_CTS_PIN;
    // GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    // GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    // GPIO_InitStructure.Pull = GPIO_PULLUP;
    // HAL_GPIO_Init(UART_CTS_PORT, &GPIO_InitStructure);
    // //RTS pin, output low
    // HAL_GPIO_WritePin(UART_RTS_PORT, UART_RTS_PIN, GPIO_PIN_RESET);
    // GPIO_InitStructure.Pin = UART_RTS_PIN;
    // GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    // GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    // HAL_GPIO_Init(UART_RTS_PORT, &GPIO_InitStructure);

    // NVIC_EnableIRQ(CDC_UART_IRQn);

    return 1;
}

int32_t uart_uninitialize(void)
{
    UART_Cmd(CDC_UART, DISABLE);    // ??? ADD
    UART_ITConfig(CDC_UART, UART_IT_RXIEN|UART_IT_TXIEN, DISABLE);
    
    // CDC_UART->CR1 &= ~(USART_IT_TXE | USART_IT_RXNE);
    clear_buffers();
    return 1;
}

int32_t uart_reset(void)
{
    UART_ITConfig(CDC_UART, UART_IT_RXIEN|UART_IT_TXIEN, DISABLE);
    clear_buffers();
    UART_ITConfig(CDC_UART, UART_IT_TXIEN, DISABLE);

    // const uint32_t cr1 = CDC_UART->CR1;
    // CDC_UART->CR1 = cr1 & ~(USART_IT_TXE | USART_IT_RXNE);
    // clear_buffers();
    // CDC_UART->CR1 = cr1 & ~USART_IT_TXE;
    return 1;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    uint16_t data_bits;
    uint16_t parity;
    uint16_t stop_bits;
    UART_InitTypeDef UART_InitStructure;

    // Disable uart and tx/rx interrupter
    UART_Cmd(CDC_UART, DISABLE);
    UART_ITConfig(CDC_UART, UART_IT_RXIEN|UART_IT_TXIEN, DISABLE);
    clear_buffers();
    
    //Only 8 bit support
    data_bits = UART_WordLength_8b;
    configuration.DataBits = UART_DATA_BITS_8;
    // parity
    configuration.Parity = config->Parity;
    if(config->Parity == UART_PARITY_ODD)
        parity = UART_Parity_Odd;
    else if(config->Parity == UART_PARITY_EVEN)
        parity = UART_Parity_Even;
    else if(config->Parity == UART_PARITY_NONE)
        parity = UART_Parity_No;
    else {   //Other not support
        parity = UART_Parity_No;
        configuration.Parity = UART_PARITY_NONE;
    }
    // stop bits
    configuration.StopBits = config->StopBits;
    if(config->StopBits == UART_STOP_BITS_2)
        stop_bits = UART_StopBits_2;
    else if(config->StopBits == UART_STOP_BITS_1)
        stop_bits = UART_StopBits_1;
    else {
        stop_bits = UART_StopBits_1;
        configuration.StopBits = UART_STOP_BITS_1;
    }
    configuration.Baudrate = config->Baudrate;
    configuration.FlowControl = UART_FLOW_CONTROL_NONE;
    //
    UART_InitStructure.UART_BaudRate = config->Baudrate;
    UART_InitStructure.UART_WordLength = data_bits;
    UART_InitStructure.UART_StopBits = stop_bits;
    UART_InitStructure.UART_Parity = parity;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_Init(CDC_UART, &UART_InitStructure);

    // Enable RX interrupt
    UART_ITConfig(CDC_UART, UART_IT_RXIEN, ENABLE);
    // Initially disable TxEmpty Interrupt
    UART_ITConfig(CDC_UART, UART_IT_TXIEN, DISABLE);

    UART_Cmd(CDC_UART, ENABLE);

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

void uart_set_control_line_state(uint16_t ctrl_bmp)
{
}

int32_t uart_write_free(void)
{
    return circ_buf_count_free(&write_buffer);
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    uint32_t cnt = circ_buf_write(&write_buffer, data, size);
    //CDC_UART->CR1 |= USART_IT_TXE;
    UART_ITConfig(CDC_UART, UART_IT_TXIEN, ENABLE);

    return cnt;
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    return circ_buf_read(&read_buffer, data, size);
}

void CDC_UART_IRQn_Handler(void)
{
    const uint32_t sr = CDC_UART->ISR;

    if (sr & UART_ISR_RX) {
        uint8_t dat = CDC_UART->RDR;
        uint32_t free = circ_buf_count_free(&read_buffer);
        if (free > RX_OVRF_MSG_SIZE) {
            circ_buf_push(&read_buffer, dat);
        } else if (RX_OVRF_MSG_SIZE == free) {
            circ_buf_write(&read_buffer, (uint8_t*)RX_OVRF_MSG, RX_OVRF_MSG_SIZE);
        } else {
            // Drop character
        }
    }

    if (sr & UART_ISR_TX) {
        if (circ_buf_count_used(&write_buffer) > 0) {
            CDC_UART->TDR = circ_buf_pop(&write_buffer);
        } else {
            //CDC_UART->CR1 &= ~USART_IT_TXE;
            UART_ITConfig(CDC_UART, UART_IT_TXIEN, DISABLE);
        }
    }
}
