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

#include "gd32f10x.h"
#include "uart.h"
#include "gpio.h"

#define USARTx_BASE                USART2
#define USARTx_CLOCK_ENABLE()      RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_USART2, ENABLE)
#define USARTx_IRQn                USART2_IRQn
#define USARTx_IRQn_Handler        USART2_IRQHandler

#define USARTx_PINS_PORT_ENABLE()  RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOA, ENABLE);
#define USARTx_TX_PORT             GPIOA
#define USARTx_TX_PIN              GPIO_PIN_2
#define USARTx_TX_PIN_SOURCE       GPIO_PINSOURCE2

#define USARTx_RX_PORT             GPIOA
#define USARTx_RX_PIN              GPIO_PIN_3
#define USARTx_RX_PIN_SOURCE       GPIO_PINSOURCE3

#define USARTx_CTS_PORT            GPIOA
#define USARTx_CTS_PIN             GPIO_PIN_0
#define USARTx_CTS_PIN_SOURCE      GPIO_PINSOURCE0

#define USARTx_RTS_PORT            GPIOA
#define USARTx_RTS_PIN             GPIO_PIN_1
#define USARTx_RTS_PIN_SOURCE      GPIO_PINSOURCE1

// Size must be 2^n for using quick wrap around
#define  BUFFER_SIZE (512)

typedef struct {
    volatile uint8_t  data[BUFFER_SIZE];
    volatile uint32_t head;
    volatile uint32_t tail;
}ring_buf_t;


extern uint32_t SystemCoreClock;

static ring_buf_t tx_buffer, rx_buffer;

static uint32_t tx_in_progress = 0;

static UART_Configuration configuration = {
    .Baudrate = 9600,
    .DataBits = UART_DATA_BITS_8,
    .Parity = UART_PARITY_NONE,
    .StopBits = UART_STOP_BITS_1,
    .FlowControl = UART_FLOW_CONTROL_NONE,
};


static int16_t read_available(ring_buf_t *buffer)
{
    return ((BUFFER_SIZE + buffer->head - buffer->tail) % BUFFER_SIZE);
}

static int16_t write_available(ring_buf_t *buffer)
{
    int16_t cnt;

    cnt = (buffer->tail - buffer->head - 1);
    if(cnt < 0)
        cnt += BUFFER_SIZE;

    return cnt;
}

static void clear_buffers(void)
{
    memset((void *)&rx_buffer, 0xBB, sizeof(ring_buf_t));
    rx_buffer.head = 0;
    rx_buffer.tail = 0;
    memset((void *)&tx_buffer, 0xBB, sizeof(ring_buf_t));
    tx_buffer.head = 0;
    tx_buffer.tail = 0;
}

int32_t uart_initialize(void)
{
    uint16_t  data_bits;
    uint16_t  parity;
    uint16_t  stop_bits;

    GPIO_InitPara    GPIO_InitStructure;
    USART_InitPara   USART_InitStructure;
    NVIC_InitPara    NVIC_InitStructure;

    USARTx_CLOCK_ENABLE();
    USARTx_PINS_PORT_ENABLE();

    USART_INT_Set(USARTx_BASE, USART_INT_RBNE|USART_INT_TBE, DISABLE);
    clear_buffers();

    // TX pin
    GPIO_InitStructure.GPIO_Pin   = USARTx_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_Init(USARTx_TX_PORT, &GPIO_InitStructure);
    // RX pin
    GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(USARTx_RX_PORT, &GPIO_InitStructure);
    // CTS pin, input
    GPIO_InitStructure.GPIO_Pin = USARTx_CTS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IPU;
    GPIO_Init(USARTx_CTS_PORT, &GPIO_InitStructure);
    // RTS pin, output low
    GPIO_InitStructure.GPIO_Pin = USARTx_RTS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_Init(USARTx_RTS_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(USARTx_RTS_PORT, USARTx_RTS_PIN);

    // Only support 8bits
    data_bits = USART_WL_8B;
    // Parity
    if(configuration.Parity == UART_PARITY_ODD)
        parity = USART_PARITY_SETODD;
    else if(configuration.Parity == UART_PARITY_EVEN)
        parity = USART_PARITY_SETEVEN;
    else
        parity = USART_PARITY_RESET;
    // stop bits
    if(configuration.StopBits == UART_STOP_BITS_2)
        stop_bits = USART_STBITS_2;
    else if(configuration.StopBits == UART_STOP_BITS_1_5)
        stop_bits = USART_STBITS_1_5;
    else
        stop_bits = USART_STBITS_1;

    // Configurate USARTx
    USART_InitStructure.USART_BRR = configuration.Baudrate;
    USART_InitStructure.USART_WL = data_bits;
    USART_InitStructure.USART_STBits = stop_bits;
    USART_InitStructure.USART_Parity = parity;
    USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
    USART_InitStructure.USART_RxorTx = USART_RXORTX_TX | USART_RXORTX_RX;
    USART_Init(USARTx_BASE, &USART_InitStructure);
    // Config NVIC
    NVIC_InitStructure.NVIC_IRQ = USARTx_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // Enable Rx interrupt
    USART_INT_Set(USARTx_BASE, USART_INT_RBNE, ENABLE);
    // Disable Tx interrupt
    USART_INT_Set(USARTx_BASE, USART_INT_TBE, DISABLE);
    // Clean all IT flag
    NVIC_ClearPendingIRQ(USARTx_IRQn);
    // Enable USARTx
    USART_Enable(USARTx_BASE, ENABLE);

    return 1;
}

int32_t uart_uninitialize(void)
{
    // Disable USARTx
    USART_Enable(USARTx_BASE, DISABLE);
    // Disable Tx interrupt
    USART_INT_Set(USARTx_BASE, USART_INT_TBE|USART_INT_RBNE, DISABLE);
    // Clean buffers
    clear_buffers();

    return 1;
}

int32_t uart_reset(void)
{
    uart_initialize();
    tx_in_progress = 0;
    return 1;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    uint16_t data_bits;
    uint16_t parity;
    uint16_t stop_bits;

    USART_InitPara   USART_InitStructure;

    // Disable USARTx
    USART_Enable(USARTx_BASE, DISABLE);
    USART_INT_Set(USARTx_BASE, USART_INT_RBNE|USART_INT_TBE, DISABLE);

    clear_buffers();
    // Only support 8bits
    data_bits = USART_WL_8B;
    configuration.DataBits = UART_DATA_BITS_8;
    // Parity
    configuration.Parity = config->Parity;
    if(configuration.Parity == UART_PARITY_ODD)
        parity = USART_PARITY_SETODD;
    else if(configuration.Parity == UART_PARITY_EVEN)
        parity = USART_PARITY_SETEVEN;
    else {
        parity = USART_PARITY_RESET;
        configuration.Parity = UART_PARITY_NONE;
    }
    // stop bits
    configuration.StopBits = config->StopBits;
    if(configuration.StopBits == UART_STOP_BITS_2)
        stop_bits = USART_STBITS_2;
    else if(configuration.StopBits == UART_STOP_BITS_1_5)
        stop_bits = USART_STBITS_1_5;
    else {
        stop_bits = USART_STBITS_1;
        configuration.StopBits = UART_STOP_BITS_1;
    }
    configuration.Baudrate = config->Baudrate;
    configuration.FlowControl = UART_FLOW_CONTROL_NONE;

    // Configurate USARTx
    USART_InitStructure.USART_BRR = configuration.Baudrate;
    USART_InitStructure.USART_WL = data_bits;
    USART_InitStructure.USART_STBits = stop_bits;
    USART_InitStructure.USART_Parity = parity;
    USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
    USART_InitStructure.USART_RxorTx = USART_RXORTX_TX | USART_RXORTX_RX;
    USART_Init(USARTx_BASE, &USART_InitStructure);

    // Enable Rx interrupt
    USART_INT_Set(USARTx_BASE, USART_INT_RBNE, ENABLE);
    // Disable Tx interrupt
    USART_INT_Set(USARTx_BASE, USART_INT_TBE, DISABLE);
    // Enable USARTx
    USART_Enable(USARTx_BASE, ENABLE);

    return 1;
}

int32_t uart_get_configuration(UART_Configuration *config)
{
    *config = configuration;

    return 1;
}

int32_t uart_write_free(void)
{
    return write_available(&tx_buffer);
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    uint32_t cnt, len;

    if(size == 0)
        return 0;

    len = write_available(&tx_buffer);
    if(len > size)
        len = size;

    cnt = len;
    while(len--) {
        tx_buffer.data[tx_buffer.head++] = *data++;
        if(tx_buffer.head >= BUFFER_SIZE)
            tx_buffer.head = 0;
    }

    if((!tx_in_progress)) {
        // Wait for tx is free
        //while(USART_GetITStatus(CDC_UART, USART_IT_TXE) == RESET);

        tx_in_progress = 1;
        USART_DataSend(USARTx_BASE, tx_buffer.data[tx_buffer.tail++]);
        if(tx_buffer.tail >= BUFFER_SIZE)
            tx_buffer.tail = 0;
        // Enale tx interrupt
        USART_INT_Set(USARTx_BASE, USART_INT_TBE, ENABLE);
    }

    return cnt;
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    uint32_t cnt, len;

    if(size == 0)
        return 0;

    len = read_available(&rx_buffer);
    if(len > size)
        len = size;

    cnt = len;
    while(len--) {
        *data++ = rx_buffer.data[rx_buffer.tail++];
        if(rx_buffer.tail >= BUFFER_SIZE)
            rx_buffer.tail = 0;
    }

    return cnt;
}

void USARTx_IRQn_Handler(void)
{
    uint8_t  dat;
    uint32_t cnt;

    if(USART_GetIntBitState(USARTx_BASE, USART_INT_ERIE) != RESET){
        USART_ClearIntBitState(USARTx_BASE, USART_INT_ERIE|USART_INT_TBE|USART_INT_RBNE);
    }

    if(USART_GetIntBitState(USARTx_BASE, USART_INT_RBNE) != RESET) {
        USART_ClearIntBitState(USARTx_BASE, USART_INT_RBNE);
        cnt = write_available(&rx_buffer);
        dat = USART_DataReceive(USARTx_BASE);
        if(cnt) {
            rx_buffer.data[rx_buffer.head++] = dat;
            if(rx_buffer.head >= BUFFER_SIZE)
                rx_buffer.head = 0;

            if(cnt <= 2) {
                // for flow control, need to set RTS = 1
            }
        }
    }

    if(USART_GetIntBitState(USARTx_BASE, USART_INT_TBE) != RESET) {
        USART_ClearIntBitState(USARTx_BASE, USART_INT_TBE);
        cnt = read_available(&tx_buffer);

        if(cnt == 0) {
            USART_INT_Set(USARTx_BASE, USART_INT_TBE, DISABLE);
            tx_in_progress = 0;
        }
        else {
            USART_DataSend(USARTx_BASE, tx_buffer.data[tx_buffer.tail++]);
            if(tx_buffer.tail >= BUFFER_SIZE)
                tx_buffer.tail = 0;
            tx_in_progress = 1;
        }
    }
}
