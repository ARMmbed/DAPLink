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

#include "stm32f0xx.h"
#include "uart.h"
#include "gpio.h"
#include "util.h"
#include "circ_buf.h"
#include "IO_Config.h"

// For usart
#define CDC_UART                     USART2
#define CDC_UART_ENABLE()            __HAL_RCC_USART2_CLK_ENABLE()
#define CDC_UART_DISABLE()           __HAL_RCC_USART2_CLK_DISABLE()
#define CDC_UART_IRQn                USART2_IRQn
#define CDC_UART_IRQn_Handler        USART2_IRQHandler

#define UART_PINS_PORT               GPIOA
#define UART_TX_PIN                  GPIO_PIN_2
#define UART_RX_PIN                  GPIO_PIN_3

#define UART_PINS_PORT_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define UART_PINS_PORT_DISABLE()     __HAL_RCC_GPIOA_CLK_DISABLE()

#define RX_OVRF_MSG         "<DAPLink:Overflow>\n"
#define RX_OVRF_MSG_SIZE    (sizeof(RX_OVRF_MSG) - 1)
#define BUFFER_SIZE         (512)

circ_buf_t write_buffer;
uint8_t write_buffer_data[BUFFER_SIZE];
circ_buf_t read_buffer;
uint8_t read_buffer_data[BUFFER_SIZE];

static UART_Configuration configuration = {
    .Baudrate = 115200,
    .DataBits = UART_DATA_BITS_8,
    .Parity = UART_PARITY_NONE,
    .StopBits = UART_STOP_BITS_1,
    .FlowControl = UART_FLOW_CONTROL_NONE,
};

static UART_HandleTypeDef uart_handle;

static void clear_buffers(void)
{
    circ_buf_init(&write_buffer, write_buffer_data, sizeof(write_buffer_data));
    circ_buf_init(&read_buffer, read_buffer_data, sizeof(read_buffer_data));
}

int32_t uart_initialize(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

    clear_buffers();

    CDC_UART_ENABLE();
    UART_PINS_PORT_ENABLE();

    GPIO_InitStructure.Pin = (UART_TX_PIN | UART_RX_PIN);
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Alternate = GPIO_AF1_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_EnableIRQ(CDC_UART_IRQn);

    return 1;
}

int32_t uart_uninitialize(void)
{
    __HAL_UART_DISABLE_IT(&uart_handle, (USART_IT_TXE | USART_IT_RXNE));
    clear_buffers();
    CDC_UART_DISABLE();
    HAL_GPIO_DeInit(GPIOA, UART_TX_PIN|UART_RX_PIN);
    return 1;
}

int32_t uart_reset(void)
{
    uart_initialize();
    return 1;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    HAL_StatusTypeDef status;

    uart_handle.Instance = CDC_UART;

    // parity
    if(config->Parity == UART_PARITY_ODD) {
        uart_handle.Init.Parity = HAL_UART_PARITY_ODD;
    } else if(config->Parity == UART_PARITY_EVEN) {
        uart_handle.Init.Parity = HAL_UART_PARITY_EVEN;
    } else if(config->Parity == UART_PARITY_NONE) {
        uart_handle.Init.Parity = HAL_UART_PARITY_NONE;
    } else {   //Other not support
        uart_handle.Init.Parity = HAL_UART_PARITY_NONE;
    }

    // stop bits
    if(config->StopBits == UART_STOP_BITS_2) {
        uart_handle.Init.StopBits = UART_STOPBITS_2;
    } else if(config->StopBits == UART_STOP_BITS_1_5) {
        uart_handle.Init.StopBits = UART_STOPBITS_2;
    } else if(config->StopBits == UART_STOP_BITS_1) {
        uart_handle.Init.StopBits = UART_STOPBITS_1;
    } else {
        uart_handle.Init.StopBits = UART_STOPBITS_1;
    }

    //Only 8 bit support
    if (uart_handle.Init.Parity == HAL_UART_PARITY_ODD || uart_handle.Init.Parity == HAL_UART_PARITY_EVEN) {
        uart_handle.Init.WordLength = UART_WORDLENGTH_9B;
    } else {
        uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
    }

    // No flow control
    uart_handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;

    // Specified baudrate
    uart_handle.Init.BaudRate = config->Baudrate;

    // TX and RX
    uart_handle.Init.Mode = UART_MODE_TX_RX;

    uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;
    uart_handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    uart_handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    // Disable uart and tx/rx interrupt
    __HAL_UART_DISABLE_IT(&uart_handle, (USART_IT_TXE | USART_IT_RXNE));

    clear_buffers();

    status = HAL_UART_DeInit(&uart_handle);
    util_assert(HAL_OK == status);
    status = HAL_UART_Init(&uart_handle);
    util_assert(HAL_OK == status);
    (void)status;

    __HAL_UART_ENABLE_IT (&uart_handle, UART_IT_RXNE);

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
    uint32_t cnt = circ_buf_write(&write_buffer, data, size);
    __HAL_UART_ENABLE_IT (&uart_handle, UART_IT_TXE);

    return cnt;
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    return circ_buf_read(&read_buffer, data, size);
}

void CDC_UART_IRQn_Handler(void)
{
    if (__HAL_UART_GET_IT(&uart_handle, UART_IT_RXNE) != RESET) {
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

    if (__HAL_UART_GET_IT(&uart_handle, UART_IT_TXE) != RESET) {
        if (circ_buf_count_used(&write_buffer) > 0) {
            CDC_UART->RDR = circ_buf_pop(&write_buffer);
        } else {
            __HAL_UART_DISABLE_IT (&uart_handle, UART_IT_TXE);
        }
    }
}
