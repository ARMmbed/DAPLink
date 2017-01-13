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

#include "stm32f10x.h"
#include "uart.h"
#include "gpio.h"
#include "circ_buf.h"
#include "settings.h" // for config_get_overflow_detect

#include "IO_Config.h"

#define RX_OVRF_MSG         "<DAPLink:Overflow>\n"
#define RX_OVRF_MSG_SIZE    (sizeof(RX_OVRF_MSG) - 1)
#define  BUFFER_SIZE (512)

#ifdef UART_TX_PIN
circ_buf_t write_buffer;
uint8_t write_buffer_data[BUFFER_SIZE];
static uint32_t tx_in_progress = 0;
#endif
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
#ifdef UART_TX_PIN
    circ_buf_init(&write_buffer, write_buffer_data, sizeof(write_buffer_data));
#endif
    circ_buf_init(&read_buffer, read_buffer_data, sizeof(read_buffer_data));
}

int32_t uart_initialize(void)
{
    uint16_t data_bits;
    uint16_t parity;
    uint16_t stop_bits;

    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    USART_ITConfig(CDC_UART, USART_IT_RXNE|USART_IT_TXE, DISABLE);
    clear_buffers();

    CDC_UART_ENABLE();
    UART_PINS_PORT_ENABLE();

#ifdef UART_TX_PIN
    //TX pin
    GPIO_InitStructure.GPIO_Pin = UART_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(UART_TX_PORT, &GPIO_InitStructure);
#endif
    //RX pin
    GPIO_InitStructure.GPIO_Pin = UART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART_RX_PORT, &GPIO_InitStructure);

#ifdef UART_CTS_PIN
    //CTS pin, input
    GPIO_InitStructure.GPIO_Pin = UART_CTS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(UART_CTS_PORT, &GPIO_InitStructure);
#endif
#ifdef UART_RTS_PIN
    //RTS pin, output low
    GPIO_InitStructure.GPIO_Pin = UART_RTS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(UART_RTS_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(UART_RTS_PORT, UART_RTS_PIN);
#endif
    //Only 8 bit support
    data_bits = USART_WordLength_8b;
    // parity
    if(configuration.Parity == UART_PARITY_ODD)
        parity = USART_Parity_Odd;
    else if(configuration.Parity == UART_PARITY_EVEN)
        parity = USART_Parity_Even;
    else
        parity = USART_Parity_No;
    // stop bits
    if(configuration.StopBits == UART_STOP_BITS_2)
        stop_bits = USART_StopBits_2;
    else if(configuration.StopBits == UART_STOP_BITS_1_5)
        stop_bits = USART_StopBits_1_5;
    else if(configuration.StopBits == UART_STOP_BITS_1)
        stop_bits = USART_StopBits_1;
    else
        stop_bits = USART_StopBits_1;
    //
    USART_InitStructure.USART_BaudRate = configuration.Baudrate;
    USART_InitStructure.USART_WordLength = data_bits;
    USART_InitStructure.USART_StopBits = stop_bits;
    USART_InitStructure.USART_Parity = parity;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
#ifdef UART_TX_PIN
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
#else
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
#endif
    USART_Init(CDC_UART, &USART_InitStructure);
    //
    NVIC_InitStructure.NVIC_IRQChannel = CDC_UART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // Enable RX interrupt
    USART_ITConfig(CDC_UART, USART_IT_RXNE, ENABLE);
    // Initially disable TxEmpty Interrupt
    USART_ITConfig(CDC_UART, USART_IT_TXE, DISABLE);

    NVIC_ClearPendingIRQ(CDC_UART_IRQn);

    USART_Cmd(CDC_UART, ENABLE);

    return 1;
}

int32_t uart_uninitialize(void)
{
    USART_Cmd(CDC_UART, DISABLE);
    USART_ITConfig(CDC_UART, USART_IT_RXNE|USART_IT_TXE, DISABLE);
    clear_buffers();
    return 1;
}

int32_t uart_reset(void)
{
    uart_initialize();
#ifdef TX_UART_PIN
    tx_in_progress = 0;
#endif
    clear_buffers();
    return 1;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    uint16_t data_bits;
    uint16_t parity;
    uint16_t stop_bits;

    USART_InitTypeDef USART_InitStructure;

    // Disable uart and tx/rx interrupter
    USART_Cmd(CDC_UART, DISABLE);
    USART_ITConfig(CDC_UART, USART_IT_RXNE|USART_IT_TXE, DISABLE);

    clear_buffers();
    //Only 8 bit support
    data_bits = USART_WordLength_8b;
    configuration.DataBits = UART_DATA_BITS_8;
    // parity
    configuration.Parity = config->Parity;
    if(config->Parity == UART_PARITY_ODD)
        parity = USART_Parity_Odd;
    else if(config->Parity == UART_PARITY_EVEN)
        parity = USART_Parity_Even;
    else if(config->Parity == UART_PARITY_NONE)
        parity = USART_Parity_No;
    else {   //Other not support
        parity = USART_Parity_No;
        configuration.Parity = UART_PARITY_NONE;
    }
    // stop bits
    configuration.StopBits = config->StopBits;
    if(config->StopBits == UART_STOP_BITS_2)
        stop_bits = USART_StopBits_2;
    else if(config->StopBits == UART_STOP_BITS_1_5)
        stop_bits = USART_StopBits_1_5;
    else if(config->StopBits == UART_STOP_BITS_1)
        stop_bits = USART_StopBits_1;
    else {
        stop_bits = USART_StopBits_1;
        configuration.StopBits = UART_STOP_BITS_1;
    }
    configuration.Baudrate = config->Baudrate;
    configuration.FlowControl = UART_FLOW_CONTROL_NONE;
    //
    USART_InitStructure.USART_BaudRate = config->Baudrate;
    USART_InitStructure.USART_WordLength = data_bits;
    USART_InitStructure.USART_StopBits = stop_bits;
    USART_InitStructure.USART_Parity = parity;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
#ifdef UART_TX_PIN
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
#else
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
#endif
    USART_Init(CDC_UART, &USART_InitStructure);
    // Enable RX interrupt
    USART_ITConfig(CDC_UART, USART_IT_RXNE, ENABLE);
    // Initially disable TxEmpty Interrupt
    USART_ITConfig(CDC_UART, USART_IT_TXE, DISABLE);

    USART_Cmd(CDC_UART, ENABLE);

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
#ifdef UART_TX_PIN
    return circ_buf_count_free(&write_buffer);
#else
	return 0;
#endif
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
#ifdef UART_TX_PIN
    uint32_t cnt = circ_buf_write(&write_buffer, data, size);

    if(!tx_in_progress) {
        // Wait for tx is free
        //while(USART_GetITStatus(CDC_UART, USART_IT_TXE) == RESET);

        tx_in_progress = 1;
        USART_SendData(CDC_UART, circ_buf_pop(&write_buffer));
        // Enale tx interrupt
        USART_ITConfig(CDC_UART, USART_IT_TXE, ENABLE);
    }
	return cnt;
#else
  	(void)data;
    return size;
#endif
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    return circ_buf_read(&read_buffer, data, size);
}

void CDC_UART_IRQn_Handler(void)
{
    if(USART_GetITStatus(CDC_UART, USART_IT_ERR) != RESET){
        USART_ClearITPendingBit(CDC_UART, USART_IT_ERR|USART_IT_RXNE|USART_IT_TXE);
    }

    if(USART_GetITStatus(CDC_UART, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(CDC_UART, USART_IT_RXNE);

        uint8_t  data = USART_ReceiveData(CDC_UART);
        uint32_t free = circ_buf_count_free(&read_buffer);

        if (free > RX_OVRF_MSG_SIZE) {
            circ_buf_push(&read_buffer, data);
        } else if ((RX_OVRF_MSG_SIZE == free) && config_get_overflow_detect()) {
            circ_buf_write(&read_buffer, (uint8_t*)RX_OVRF_MSG, RX_OVRF_MSG_SIZE);
        } else {
            // Drop character
        }
    }

#ifdef UART_TX_PIN
    if(USART_GetITStatus(CDC_UART, USART_IT_TXE) != RESET) {
        USART_ClearITPendingBit(CDC_UART, USART_IT_TXE);
        if (circ_buf_count_used(&write_buffer) > 0) {
            USART_SendData(CDC_UART, circ_buf_pop(&write_buffer));
            tx_in_progress = 1;
        } else {
            USART_ITConfig(CDC_UART, USART_IT_TXE, DISABLE);
            tx_in_progress = 0;
        }
    }
#endif
}
