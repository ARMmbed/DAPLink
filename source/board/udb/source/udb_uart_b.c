#include "string.h"
#include "stm32h7xx.h"
#include "uart.h"
#include "udb_uart_b.h"
#include "gpio.h"
#include "circ_buf.h"
#include "IO_Config.h"
#include "settings.h"
#include "stm32h7xx_hal_uart_ex.h"
#include "util.h"

#define UART_B                  USART2
#define UART_B_ENABLE()         __HAL_RCC_USART2_CLK_ENABLE()
#define UART_B_DISABLE()        __HAL_RCC_USART2_CLK_DISABLE()
#define UART_B_IRQn             USART2_IRQn
#define UART_B_IRQn_Handler     USART2_IRQHandler

#define UART_B_ALTFUNC          GPIO_AF7_USART2 //Select the correct alt func
#define UART_B_RXFIFO_THRESHOLD UART_RXFIFO_THRESHOLD_3_4

#define UART_B_TX_PORT          GPIOD
#define UART_B_TX_PIN           GPIO_PIN_5

#define UART_B_RX_PORT          GPIOD
#define UART_B_RX_PIN           GPIO_PIN_6

#define UART_B_BAUDRATE_DEFAULT (115200)

#define RX_OVRF_MSG             "<DAPLink:Overflow>\n"
#define RX_OVRF_MSG_SIZE        (sizeof(RX_OVRF_MSG) - 1)

// This buffer size should be power of two.
#define BUFFER_SIZE             (512)
COMPILER_ASSERT((BUFFER_SIZE > 0U) && ((BUFFER_SIZE & (BUFFER_SIZE - 1U)) == 0U));

static circ_buf_t s_write_buffer;
static uint8_t s_write_buffer_data[BUFFER_SIZE];
static circ_buf_t s_read_buffer;
static uint8_t s_read_buffer_data[BUFFER_SIZE];

static UART_Configuration s_configuration =
{
    .Baudrate = UART_B_BAUDRATE_DEFAULT,
    .DataBits = UART_DATA_BITS_8,
    .Parity = UART_PARITY_NONE,
    .StopBits = UART_STOP_BITS_1,
    .FlowControl = UART_FLOW_CONTROL_NONE,
};

static UART_HandleTypeDef s_uart_handle;

static void clear_buffers(void)
{
    circ_buf_init(&s_write_buffer, s_write_buffer_data, sizeof(s_write_buffer_data));
    circ_buf_init(&s_read_buffer, s_read_buffer_data, sizeof(s_read_buffer_data));
}

static void udb_uart_b_read_from_fifo_in_isr(void)
{
    uint16_t size = circ_buf_count_free(&s_read_buffer);
    uint8_t data;
    while (UART_B->ISR & USART_ISR_RXNE_RXFNE)
    {
        data = UART_B->RDR;
        if (size > RX_OVRF_MSG_SIZE)
        {
            circ_buf_push(&s_read_buffer, data);
        }
        else if (config_get_overflow_detect())
        {
            if (RX_OVRF_MSG_SIZE == size)
            {
                circ_buf_write(&s_read_buffer, (uint8_t*)RX_OVRF_MSG, RX_OVRF_MSG_SIZE);
            }
            else
            {
                // drop characters
            }
            break;
        }
        else
        {
            // drop characters
            break;
        }
        --size;
    }
}

int32_t udb_uart_b_initialize(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    UART_B_ENABLE();

    //TX pin
    GPIO_InitStructure.Pin = UART_B_TX_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Alternate = UART_B_ALTFUNC;
    HAL_GPIO_Init(UART_B_TX_PORT, &GPIO_InitStructure);

    //RX pin
    GPIO_InitStructure.Pin = UART_B_RX_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Alternate = UART_B_ALTFUNC;

    HAL_GPIO_Init(UART_B_RX_PORT, &GPIO_InitStructure);

    clear_buffers();

    NVIC_ClearPendingIRQ(UART_B_IRQn);
    NVIC_EnableIRQ(UART_B_IRQn);

    return 1;
}

int32_t udb_uart_b_uninitialize(void)
{
    NVIC_DisableIRQ(UART_B_IRQn);
    clear_buffers();

    HAL_GPIO_DeInit(UART_B_RX_PORT, UART_B_RX_PIN);
    HAL_GPIO_DeInit(UART_B_TX_PORT, UART_B_TX_PIN);

    UART_B_DISABLE();

    return 1;
}

int32_t udb_uart_b_reset(void)
{
    NVIC_DisableIRQ(UART_B_IRQn);

    clear_buffers();

    NVIC_ClearPendingIRQ(UART_B_IRQn);
    NVIC_EnableIRQ(UART_B_IRQn);

    return 1;
}

int32_t udb_uart_b_set_configuration(UART_Configuration *config)
{
    HAL_StatusTypeDef status;

    s_uart_handle.Instance = UART_B;
    status = HAL_UART_DeInit(&s_uart_handle);

    util_assert(status != HAL_ERROR);

    // parity
    s_configuration.Parity = config->Parity;
    if (config->Parity == UART_PARITY_ODD)
    {
        s_uart_handle.Init.Parity = STM32_UART_PARITY_ODD;
    }
    else if (config->Parity == UART_PARITY_EVEN)
    {
        s_uart_handle.Init.Parity = STM32_UART_PARITY_EVEN;
    }
    else if (config->Parity == UART_PARITY_NONE)
    {
        s_uart_handle.Init.Parity = STM32_UART_PARITY_NONE;
    }
    else
    {
        // not support other parity
        util_assert(false);
    }

    // stop bits
    s_configuration.StopBits = config->StopBits;
    if (config->StopBits == UART_STOP_BITS_2)
    {
        s_uart_handle.Init.StopBits = UART_STOPBITS_2;
    }
    else if (config->StopBits == UART_STOP_BITS_1_5)
    {
        s_uart_handle.Init.StopBits = UART_STOPBITS_2;
        s_configuration.StopBits = UART_STOP_BITS_2;
    }
    else if (config->StopBits == UART_STOP_BITS_1)
    {
        s_uart_handle.Init.StopBits = UART_STOPBITS_1;
    }
    else
    {
        s_uart_handle.Init.StopBits = UART_STOPBITS_1;
        s_configuration.StopBits = UART_STOP_BITS_1;
    }

    //Only 8 bit support
    util_assert(config->DataBits == UART_DATA_BITS_8);
    s_configuration.DataBits = config->DataBits;

    if (s_uart_handle.Init.Parity == STM32_UART_PARITY_ODD || s_uart_handle.Init.Parity == STM32_UART_PARITY_EVEN)
    {
        s_uart_handle.Init.WordLength = UART_WORDLENGTH_9B;
    }
    else
    {
        s_uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
    }

    // No flow control
    util_assert(config->FlowControl == UART_FLOW_CONTROL_NONE);
    s_configuration.FlowControl = config->FlowControl;

    // Specified baudrate
    s_configuration.Baudrate = config->Baudrate;
    s_uart_handle.Init.BaudRate = config->Baudrate;

    // TX and RX
    s_uart_handle.Init.Mode = UART_MODE_TX_RX;

    status = HAL_UART_Init(&s_uart_handle);
    util_assert(status != HAL_ERROR);

    status = HAL_UARTEx_EnableFifoMode(&s_uart_handle);
    util_assert(status != HAL_ERROR);

    status = HAL_UARTEx_SetRxFifoThreshold(&s_uart_handle, UART_B_RXFIFO_THRESHOLD);
    util_assert(status != HAL_ERROR);

    clear_buffers();

    UART_B->CR1 |= USART_CR1_IDLEIE;
    UART_B->CR3 |= USART_CR3_RXFTIE;

    return 1;
}

int32_t udb_uart_b_get_configuration(UART_Configuration *config)
{
    config->Baudrate    = s_configuration.Baudrate;
    config->DataBits    = s_configuration.DataBits;
    config->Parity      = s_configuration.Parity;
    config->StopBits    = s_configuration.StopBits;
    config->FlowControl = s_configuration.FlowControl;

    return 1;
}

void udb_uart_b_set_control_line_state(uint16_t ctrl_bmp)
{
}

int32_t udb_uart_b_write_free(void)
{
    return circ_buf_count_free(&s_write_buffer);
}

int32_t udb_uart_b_write_data(uint8_t *data, uint16_t size)
{
    uint32_t cnt = circ_buf_write(&s_write_buffer, data, size);
    if (cnt > 0)
    {
        UART_B->CR1 |= USART_CR1_TXEIE_TXFNFIE;
    }
    return cnt;
}

int32_t udb_uart_b_read_data(uint8_t *data, uint16_t size)
{
    return circ_buf_read(&s_read_buffer, data, size);
}

void UART_B_IRQn_Handler(void)
{
    const uint32_t isr_reg = UART_B->ISR;
    const uint32_t cr1 = UART_B->CR1;
    const uint32_t cr3 = UART_B->CR3;

    // UART Rx
    if ((isr_reg & USART_ISR_RXFT) && (cr3 & USART_CR3_RXFTIE))
    {
        udb_uart_b_read_from_fifo_in_isr();
    }
    else if((cr1 & USART_CR1_IDLEIE) && (isr_reg & USART_ISR_IDLE))
    {
        // Handle IDLE interrupt to deal with the data left in FIFO,
        // which is less than UART_B_FIFO_SIZE and cannot trigger
        // USART_ISR_RXFT
        UART_B->ICR |= USART_ICR_IDLECF;
        udb_uart_b_read_from_fifo_in_isr();
    }

    // UART Tx
    if ((isr_reg & USART_ISR_TXE_TXFNF) && (cr1 & USART_CR1_TXEIE_TXFNFIE))
    {
        uint16_t size = circ_buf_count_used(&s_write_buffer);
        if (size > 0)
        {
            while (size && (UART_B->ISR & USART_ISR_TXE_TXFNF))
            {
                UART_B->TDR = circ_buf_pop(&s_write_buffer);
                --size;
            }

            if (size == 0)
            {
                UART_B->CR1 &= ~USART_CR1_TXEIE_TXFNFIE;
            }
        }
        else
        {
            // transmission is done
            UART_B->CR1 &= ~USART_CR1_TXEIE_TXFNFIE;
        }
    }
}
