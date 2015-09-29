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

#include "uart.h"
#include "IO_Config.h"
#include "string.h"
#include "em_leuart.h"
#include "em_cmu.h"
#include "em_gpio.h"

// Size must be 2^n for using quick wrap around
#define UART_BUFFER_SIZE    (32)

struct {
    uint8_t  data[UART_BUFFER_SIZE];
    volatile uint32_t idx_in;
    volatile uint32_t idx_out;
    volatile uint32_t cnt_in;
    volatile uint32_t cnt_out;
} write_buffer, read_buffer;

uint32_t tx_in_progress = 0;

void clear_buffers(void)
{
    memset((void*)&read_buffer, 0xBB, sizeof(read_buffer.data));
    read_buffer.idx_in = 0;
    read_buffer.idx_out = 0;
    read_buffer.cnt_in = 0;
    read_buffer.cnt_out = 0;
    memset((void*)&write_buffer, 0xBB, sizeof(read_buffer.data));
    write_buffer.idx_in = 0;
    write_buffer.idx_out = 0;
    write_buffer.cnt_in = 0;
    write_buffer.cnt_out = 0;
}

void serial_baud(uint32_t baudrate);

int32_t uart_initialize(void)
{
    NVIC_DisableIRQ(UART_RX_TX_IRQn);
    clear_buffers();
    
    // disable GPIO pins used by leuart
    CMU_ClockEnable(cmuClock_GPIO, true);
		GPIO_PinModeSet(PIN_UART_RX_PORT, PIN_UART_RX_BIT, gpioModeDisabled, 1);	
		GPIO_PinModeSet(PIN_UART_TX_PORT, PIN_UART_TX_BIT, gpioModeDisabled, 1);
    
    // enable clock to uart
    CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_CORELEDIV2);
    CMU_ClockEnable(cmuClock_LFB, true);
    CMU_ClockEnable(UART_CLK, true);
    
    // set output location
    UART->ROUTE = (UART->ROUTE & _LEUART_ROUTE_LOCATION_MASK) | UART_LOC;
    
    NVIC_ClearPendingIRQ(UART_RX_TX_IRQn);
    NVIC_EnableIRQ(UART_RX_TX_IRQn);
    return 1;
}

int32_t uart_uninitialize(void)
{
    LEUART_IntDisable(UART, LEUART_IF_TXBL | LEUART_IF_RXDATAV);
    LEUART_Enable(UART, leuartDisable);
    
    clear_buffers();
    return 1;
}

int32_t uart_reset(void)
{
    // disable interrupt
    NVIC_DisableIRQ(UART_RX_TX_IRQn);
    clear_buffers();
    LEUART_IntDisable(UART, LEUART_IF_TXBL);
    UART->CMD = LEUART_CMD_CLEARTX | LEUART_CMD_CLEARRX;
    tx_in_progress = 0;
    // enable interrupt
    NVIC_EnableIRQ(UART_RX_TX_IRQn);
    return 1;
}

/* Set baudrate to requested value and adjust clocks */
/* Assumes there's a 32.768kHz LFXO */
void serial_baud(uint32_t baudrate) {
  /* check if baudrate is within allowed range */
  EFM_ASSERT(baudrate >= (UART_LF_REF_FREQ >> 7));

  if(baudrate > (UART_LF_REF_FREQ >> 1)){
      /* check if baudrate is within allowed range */
      EFM_ASSERT((baudrate <= (UART_HF_REF_FREQ >> 1)) && (baudrate > (UART_HF_REF_FREQ >> 10)));

      CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_CORELEDIV2);
      CMU_ClockEnable(cmuClock_LFB, true);
      CMU_ClockSelectSet(UART_CLK, cmuSelect_CORELEDIV2);
      uint8_t divisor = 1;

      if(baudrate > (UART_HF_REF_FREQ >> 7)){
          divisor = 1;
      }else if(baudrate > (UART_HF_REF_FREQ >> 8)){
          divisor = 2;
      }else if(baudrate > (UART_HF_REF_FREQ >> 9)){
          divisor = 4;
      }else{
          divisor = 8;
      }
      CMU_ClockDivSet(UART_CLK, divisor);
      LEUART_BaudrateSet(UART, UART_HF_REF_FREQ/divisor, (uint32_t)baudrate);
  } else {
      CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
      CMU_ClockEnable(cmuClock_LFB, true);
      CMU_ClockDivSet(UART_CLK, 1);
      LEUART_BaudrateSet(UART, UART_LF_REF_FREQ, (uint32_t)baudrate);
  }
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    LEUART_Init_TypeDef internal_config = LEUART_INIT_DEFAULT;
    // disable interrupt
    NVIC_DisableIRQ(UART_RX_TX_IRQn);
    
    switch(config->DataBits) {
      case UART_DATA_BITS_8:
      default:
        internal_config.databits = leuartDatabits8;
        break;
    }
    
    switch(config->Parity) {
      case UART_PARITY_ODD:
        internal_config.parity = leuartOddParity;
        break;
      case UART_PARITY_EVEN:
        internal_config.parity = leuartEvenParity;
        break;
      case UART_PARITY_NONE:
      default:
        internal_config.parity = leuartNoParity;
        break;
    }
    
    switch(config->StopBits) {
      case UART_STOP_BITS_1_5:
        internal_config.stopbits = leuartStopbits1;
        break;
      case UART_STOP_BITS_2:
        internal_config.stopbits = leuartStopbits2;
        break;
      case UART_STOP_BITS_1:
      default:
        internal_config.stopbits = leuartStopbits1;
        break;
    }
    
    internal_config.baudrate = config->Baudrate;
    
    LEUART_Init(UART, &internal_config);
    serial_baud(internal_config.baudrate);
    
    clear_buffers();
    
    // Enable UART interrupt
    NVIC_ClearPendingIRQ(UART_RX_TX_IRQn);
    NVIC_EnableIRQ(UART_RX_TX_IRQn);
    LEUART_IntEnable(UART, LEUART_IEN_RXDATAV);
    return 1;
}

int32_t uart_get_configuration(UART_Configuration *config)
{
    return 1;
}

int32_t uart_write_free(void)
{
    return UART_BUFFER_SIZE - (write_buffer.cnt_in - write_buffer.cnt_out);
}

int32_t uart_write_data (uint8_t *data, uint16_t size)
{
    uint32_t cnt;
    int16_t  len_in_buf;
    if (size == 0) {
        return 0;
    }
    cnt = 0;
    while (size--) {
        len_in_buf = write_buffer.cnt_in - write_buffer.cnt_out;
        if (len_in_buf < UART_BUFFER_SIZE) {
            write_buffer.data[write_buffer.idx_in++] = *data++;
            write_buffer.idx_in &= (UART_BUFFER_SIZE - 1);
            write_buffer.cnt_in++;
            cnt++;
        }
    }
    if (!tx_in_progress) {
        // Wait for D register to be free
        while(!(LEUART_StatusGet(UART) & LEUART_STATUS_TXBL));
        tx_in_progress = 1;
        // Write the first byte into D
        LEUART_Tx(UART, write_buffer.data[write_buffer.idx_out++]);
        write_buffer.idx_out &= (UART_BUFFER_SIZE - 1);
        write_buffer.cnt_out++;
        // enable TX interrupt
        LEUART_IntEnable(UART, LEUART_IEN_TXBL);
    }
    return cnt;
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    uint32_t cnt;
    if (size == 0) {
        return 0;
    }
    cnt = 0;
    while (size--) {
        if (read_buffer.cnt_in != read_buffer.cnt_out) {
            *data++ = read_buffer.data[read_buffer.idx_out++];
            read_buffer.idx_out &= (UART_BUFFER_SIZE - 1);
            read_buffer.cnt_out++;
            cnt++;
        } else {
            break;
        }
    }
    return cnt;
}

void UART_RX_TX_IRQHandler(void)
{
    uint32_t status;
    // read interrupt status
    status = LEUART_IntGet(UART);
    // handle character to transmit
    if (write_buffer.cnt_in != write_buffer.cnt_out) {
        // if TDRE is empty
        if (status & LEUART_IF_TXBL) {
            LEUART_Tx(UART, write_buffer.data[write_buffer.idx_out++]);
            write_buffer.idx_out &= (UART_BUFFER_SIZE - 1);
            write_buffer.cnt_out++;
            tx_in_progress = 1;
        }
    } else {
        // disable TX interrupt
        LEUART_IntDisable(UART, LEUART_IEN_TXBL);
        tx_in_progress = 0;
    }
    // handle received character
    if (status & LEUART_IF_RXDATAV) {
        read_buffer.data[read_buffer.idx_in++] = LEUART_Rx(UART);
        read_buffer.idx_in &= (UART_BUFFER_SIZE - 1);
        read_buffer.cnt_in++;
    }
}
