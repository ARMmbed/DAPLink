/**
 * @file    uart.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2018 dotnfc, All Rights Reserved
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

#include "uart.h"
#include <stm32f0xx.h>
#include "DAP_config.h"

#define UART_BUFFER_SIZE	(256)	/*	Size of Receive and Transmit buffers MUST BE 2^n */

static struct {
	volatile uint16_t	idx_in;
	volatile uint16_t	idx_out;
	volatile int16_t	cnt_in;
	volatile int16_t	cnt_out;
			uint8_t		data[UART_BUFFER_SIZE];
} WrBuffer, RdBuffer;

static uint32_t SendBreakFlag;
UART_HandleTypeDef hal_config;

// #define __wait_todo() util_assert(0)

#if DAP_CDC_USART

int32_t uart_initialize(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;
    
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
      
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	return 1;
}

int32_t uart_uninitialize(void)
{
    __HAL_RCC_USART1_CLK_DISABLE();
    
    HAL_GPIO_DeInit(GPIOA, USART_RX_PIN | USART_TX_PIN);
    return 1;
}

/* UART_Reset:  Reset the Serial module variables */
int32_t uart_reset(void)
{
    uart_initialize();
	return (1);
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    NVIC_DisableIRQ (USART_IRQn);

    hal_config.Instance = USART1;
    
    /* Data bits */
	if (config->DataBits != UART_DATA_BITS_8)
		return(0);

    hal_config.Init.WordLength = UART_WORDLENGTH_8B;
    
	/* Parity */
	switch (config->Parity)
	{
		case UART_PARITY_NONE:	
            hal_config.Init.Parity = HAL_UART_PARITY_NONE;		
            break;
		case UART_PARITY_EVEN:	
            hal_config.Init.Parity = HAL_UART_PARITY_EVEN;	
            hal_config.Init.WordLength = UART_WORDLENGTH_9B;
            break;
		case UART_PARITY_ODD:	
            hal_config.Init.Parity = HAL_UART_PARITY_ODD;	
            break;
		default: 
            return (0);
	}

	/* Stop bits */
	switch (config->StopBits)
	{
		case UART_STOP_BITS_1:	
            hal_config.Init.StopBits = UART_STOPBITS_1;		
            break;
		case UART_STOP_BITS_2:	
            hal_config.Init.StopBits = UART_STOPBITS_2;
            break;
		case UART_STOP_BITS_1_5:
            hal_config.Init.StopBits = UART_STOPBITS_1_5;	
            break;
		default: 
            return (0);
	}
    hal_config.Init.BaudRate		= config->Baudrate;
	hal_config.Init.Mode			= UART_MODE_TX_RX;
	hal_config.Init.HwFlowCtl	= UART_HWCONTROL_NONE;
	hal_config.Init.OverSampling = UART_OVERSAMPLING_16;
    hal_config.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    hal_config.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    
	HAL_UART_Init(&hal_config);
    __HAL_UART_ENABLE_IT (&hal_config, UART_IT_RXNE);
    
    NVIC_EnableIRQ (USART_IRQn);
    
    return 1;
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
	int32_t cnt = 0;
	int16_t len_in_buf;
	uint16_t next;

	while (size != 0) {
		--size;
		len_in_buf = WrBuffer.cnt_in - WrBuffer.cnt_out;
		if (len_in_buf < UART_BUFFER_SIZE) {
			next = WrBuffer.idx_in;
			WrBuffer.data[next] = *data++;
			next = (next + 1) & (UART_BUFFER_SIZE - 1);
			WrBuffer.idx_in = next;
			WrBuffer.cnt_in++;
			cnt++;
		}
	}

	if (cnt != 0) {
        __HAL_UART_ENABLE_IT (&hal_config, UART_IT_TXE);
    }

	return cnt;
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
	int32_t cnt = 0;

	while (size != 0) {
		--size;
		if (RdBuffer.cnt_in != RdBuffer.cnt_out) {
			*data++ = RdBuffer.data[RdBuffer.idx_out++];
			RdBuffer.idx_out &= (UART_BUFFER_SIZE - 1);
			RdBuffer.cnt_out++;
			cnt++;
		}
	}

	return (cnt);
}

int32_t uart_write_free(void)
{
    int32_t v;
    v = (int32_t)(WrBuffer.idx_in - WrBuffer.idx_out - 1);

    if (v < 0) {
        v += sizeof(WrBuffer.data);
    }

    // Return free space in buffer
    return v;
}

void USART1_IRQHandler (void)
{
	uint8_t  ch;
	int16_t  len_in_buf;

	/* Read data register not empty interrupt */
	if (__HAL_UART_GET_IT(&hal_config, UART_IT_RXNE) != RESET)
	{
		len_in_buf = RdBuffer.cnt_in - RdBuffer.cnt_out;
		if (len_in_buf < UART_BUFFER_SIZE)
		{
			//ch = (uint8_t)USART_ReceiveData(USART_PORT);
            ch = (USART_PORT->RDR & 0xff);
			RdBuffer.data[RdBuffer.idx_in++] = ch;
			if ((ch == 0)
			&&	(__HAL_UART_GET_IT(&hal_config, UART_IT_FE)  != RESET)	/* framing error */
				)
			{	/* break character */
				//BreakFlag = 1;
			}
			else
            {
				//BreakFlag = 0;
            }
			RdBuffer.idx_in &= (UART_BUFFER_SIZE - 1);
			RdBuffer.cnt_in++;
		}
	}

	/* Transmit data register empty interrupt */
	if (__HAL_UART_GET_IT(&hal_config, UART_IT_TXE) != RESET)
	{
		if (SendBreakFlag == 1)
		{
			//USART_SendBreak(USART_PORT);
		}
		else if (WrBuffer.cnt_in != WrBuffer.cnt_out)
		{
			//USART_SendData(USART_PORT, WrBuffer.data[WrBuffer.idx_out++]);
            USART_PORT->TDR = (WrBuffer.data[WrBuffer.idx_out++] & 0xff);
			WrBuffer.idx_out &= (UART_BUFFER_SIZE - 1);
			WrBuffer.cnt_out++;
		}
		else
		{
            __HAL_UART_DISABLE_IT (&hal_config, UART_IT_TXE);
		}
	}
}

#else //  ---------------------- !DAP_CDC_USART ----------------------

int32_t uart_initialize(void)
{
    return 0;
}

int32_t uart_uninitialize(void)
{
    return 0;
}

int32_t uart_reset(void)
{
    return 0;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    return 0;
}

int32_t uart_write_free(void)
{
    return 0;
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    return 0;
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    return 0;
}
#endif // DAP_CDC_USART
