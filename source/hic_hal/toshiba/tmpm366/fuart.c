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
#include <TMPM366.h>
#include <string.h>
#include "uart.h"
#include "tmpm366_fuart.h"
#include "circ_buf.h"
#include "settings.h" // for config_get_overflow_detect

FUART_InitTypeDef myUART;
static uint32_t tx_in_progress = 0;

#define RX_OVRF_MSG         "<DAPLink:Overflow>\n"
#define RX_OVRF_MSG_SIZE    (sizeof(RX_OVRF_MSG) - 1)
#define BUFFER_SIZE         (64)

circ_buf_t write_buffer;
uint8_t write_buffer_data[BUFFER_SIZE];
circ_buf_t read_buffer;
uint8_t read_buffer_data[BUFFER_SIZE];

// Initialize UART
int32_t uart_initialize(void)
{
    // Configure UART
    myUART.BaudRate = (uint32_t)9600;
    myUART.DataBits = FUART_DATA_BITS_8;
    myUART.StopBits = FUART_STOP_BITS_1;
    myUART.Parity   = FUART_NO_PARITY;
    myUART.Mode     = FUART_ENABLE_TX | FUART_ENABLE_RX;
    myUART.FlowCtrl = FUART_NONE_FLOW_CTRL;

    // UART pin configuration
    TSB_PG->CR  |= 0x01;
    TSB_PG->FR4 |= 0x01;
    TSB_PG->FR4 |= 0x02;
    TSB_PG->IE  |= 0x02;

    NVIC_DisableIRQ(INTUART_IRQn);
    // Initialize UART
    FUART_Init(FUART0, &myUART);
    FUART_SetINTMask(FUART0, FUART_TX_FIFO_INT_MASK | FUART_RX_FIFO_INT_MASK);
    uart_reset();
    FUART_Enable(FUART0);
    NVIC_EnableIRQ(INTUART_IRQn);
    return 1;
}

// Uninitialized FUART0
int32_t uart_uninitialize(void)
{
    // Disable  UART
    FUART_Disable(FUART0);
    uart_reset();
    NVIC_DisableIRQ(INTUART_IRQn);
    return 1;
}

// Reset FUART0
int32_t uart_reset(void)
{
    FUART_INTStatus clearFlag;
    // Disable Interrupt
    NVIC_DisableIRQ(INTUART_IRQn);

    // Reset buffer
    circ_buf_init(&write_buffer, write_buffer_data, sizeof(write_buffer_data));
    circ_buf_init(&read_buffer, read_buffer_data, sizeof(read_buffer_data));

    clearFlag.All = 0x30;
    FUART_ClearINT(FUART0, clearFlag);

    while (FUART_STORAGE_EMPTY != FUART_GetStorageStatus(FUART0, FUART_TX));

    while (FUART_STORAGE_EMPTY != FUART_GetStorageStatus(FUART0, FUART_RX)) {
        FUART_GetRxData(FUART0);
    }
    // Re enable UART
    NVIC_EnableIRQ(INTUART_IRQn);
    return 1;
}

// Set UART configuration
int32_t uart_set_configuration(UART_Configuration *config)
{
    myUART.BaudRate = config->Baudrate;
    myUART.FlowCtrl = FUART_NONE_FLOW_CTRL; // no support flow control
    myUART.Mode = FUART_ENABLE_TX | FUART_ENABLE_RX;

    switch (config->DataBits) {
        case UART_DATA_BITS_7:
            myUART.DataBits = FUART_DATA_BITS_7;
            break;
        case UART_DATA_BITS_8:
            myUART.DataBits = FUART_DATA_BITS_8;
            break;
        default:
            break;
    }
    switch (config->Parity) {
        case UART_PARITY_NONE:
            myUART.Parity = FUART_NO_PARITY;
            break;
        case UART_PARITY_EVEN:
            myUART.Parity = FUART_EVEN_PARITY;
            break;
        case UART_PARITY_ODD:
            myUART.Parity = FUART_ODD_PARITY;
            break;
        default:
            break;
    }
    switch (config->StopBits) {
        case UART_STOP_BITS_1:
            myUART.StopBits = FUART_STOP_BITS_1;
            break;
        case UART_STOP_BITS_2:
            myUART.StopBits = FUART_STOP_BITS_2;
            break;
        default:
            break;
    }

    uart_uninitialize();
    FUART_Init(FUART0, &myUART);
    FUART_Enable(FUART0);
    NVIC_EnableIRQ(INTUART_IRQn);
    return 1;
}

// Get UART configuration
int32_t uart_get_configuration(UART_Configuration *config)
{
    // Get configuration from UART
    config->Baudrate = (uint32_t) myUART.BaudRate;
    config->FlowControl = UART_FLOW_CONTROL_NONE; // no support flow control

    switch (myUART.DataBits) {
        case 4:
            config->DataBits = UART_DATA_BITS_7;
            break;
        case 8:
            config->DataBits = UART_DATA_BITS_8;
            break;
        default:
            break;
    }

    config->StopBits = (UART_StopBits) myUART.StopBits;
    switch (myUART.StopBits) {
        case 0:
            config->StopBits = UART_STOP_BITS_1;
            break;
        case 16:
            config->StopBits = UART_STOP_BITS_2;
            break;
        default:
            break;
    }

    config->Parity = (UART_Parity) myUART.Parity;
    switch (myUART.Parity) {
        case FUART_NO_PARITY:
            config->Parity = UART_PARITY_NONE;
            break;
        case FUART_EVEN_PARITY:
            config->Parity = UART_PARITY_EVEN;
            break;
        case FUART_ODD_PARITY:
            config->Parity = UART_PARITY_ODD;
            break;
        default:
            break;
    }

    return 1;
}

// Get transmit buffer empty length
int32_t uart_write_free(void)
{
    return circ_buf_count_free(&write_buffer);
}

// Write data to transmit buffer
int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    uint32_t cnt;

    cnt = circ_buf_write(&write_buffer, data, size);

    // enable TX interrupt
    TSB_UART_CR_TXE = 1;

    if (!tx_in_progress) {
        // force TX interrupt to start
        NVIC_SetPendingIRQ(INTUART_IRQn);
    }
    return cnt;
}

// Read data for receive buffer
int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    return circ_buf_read(&read_buffer, data, size);
}

// Receive Interrupt
void INTUART_IRQHandler(void)
{
    FUART_StorageStatus FIFOStatus;
    FUART_INTStatus INTStatus_, clearFlag;
    INTStatus_ = FUART_GetMaskedINTStatus(FUART0);
    if ((INTStatus_.All & 0x10) && (!tx_in_progress)) {
        while (FUART_STORAGE_EMPTY != FUART_GetStorageStatus(FUART0, FUART_RX)) {
            uint32_t free;
            uint8_t data;
            free = circ_buf_count_free(&read_buffer);
            data = FUART_GetRxData(FUART0);

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
        clearFlag.All |= 0x10;
        FUART_ClearINT(FUART0, clearFlag);
    }  else if (!(INTStatus_.All & 0x20)) {
        if (circ_buf_count_used(&write_buffer) > 0) {
            // if Transmit buffer is empty
            FIFOStatus = FUART_GetStorageStatus(FUART0, FUART_TX);
            if ((FIFOStatus == FUART_STORAGE_EMPTY)
                    || (FIFOStatus == FUART_STORAGE_NORMAL)) {
                while(TSB_UART_FR_BUSY);
                FUART_SetTxData(FUART0,circ_buf_pop(&write_buffer));
                tx_in_progress = 1;
                clearFlag.All |= 0x20;
                FUART_ClearINT(FUART0, clearFlag);
            }
        } else if (tx_in_progress) {
            tx_in_progress = 0;
            TSB_UART_CR_TXE = 0;
        }
    }
}
