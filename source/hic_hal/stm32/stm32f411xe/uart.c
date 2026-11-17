#include "string.h"
#include "stm32f4xx.h"
#include "uart.h"
#include "gpio.h"
#include "circ_buf.h"
#include "IO_Config.h"
#include "settings.h"
#include "util.h"

#define CDC_UART                    USART2
#define CDC_UART_ENABLE()           __HAL_RCC_USART2_CLK_ENABLE()
#define CDC_UART_DISABLE()          __HAL_RCC_USART2_CLK_DISABLE()
#define CDC_UART_IRQn               USART2_IRQn
#define CDC_UART_IRQn_Handler       USART2_IRQHandler

#define UART_ALTFUNC                GPIO_AF7_USART2
#define UART_TX_PORT                GPIOA
#define UART_TX_PIN                 GPIO_PIN_2
#define UART_RX_PORT                GPIOA
#define UART_RX_PIN                 GPIO_PIN_3

#define CDC_UART_BAUDRATE_DEFAULT   (115200)
#define RX_OVRF_MSG                 "<DAPLink:Overflow>\n"
#define RX_OVRF_MSG_SIZE            (sizeof(RX_OVRF_MSG) - 1)
#define BUFFER_SIZE                 (512)

static circ_buf_t s_write_buffer;
static uint8_t s_write_buffer_data[BUFFER_SIZE];
static circ_buf_t s_read_buffer;
static uint8_t s_read_buffer_data[BUFFER_SIZE];

static UART_Configuration s_configuration = {
    .Baudrate = CDC_UART_BAUDRATE_DEFAULT,
    .DataBits = UART_DATA_BITS_8,
    .Parity = STM32_UART_PARITY_NONE,
    .StopBits = UART_STOP_BITS_1,
    .FlowControl = UART_FLOW_CONTROL_NONE,
};

static UART_HandleTypeDef s_uart_handle;

static void clear_buffers(void)
{
    circ_buf_init(&s_write_buffer, s_write_buffer_data, sizeof(s_write_buffer_data));
    circ_buf_init(&s_read_buffer, s_read_buffer_data, sizeof(s_read_buffer_data));
}

int32_t uart_initialize(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    CDC_UART_ENABLE();

    // TX (PA2)
    GPIO_InitStructure.Pin = UART_TX_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Alternate = UART_ALTFUNC;
    HAL_GPIO_Init(UART_TX_PORT, &GPIO_InitStructure);

    // RX (PA3)
    GPIO_InitStructure.Pin = UART_RX_PIN;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(UART_RX_PORT, &GPIO_InitStructure);

    clear_buffers();

    NVIC_ClearPendingIRQ(CDC_UART_IRQn);
    NVIC_EnableIRQ(CDC_UART_IRQn);

    return 1;
}

int32_t uart_uninitialize(void)
{
    NVIC_DisableIRQ(CDC_UART_IRQn);
    clear_buffers();
    HAL_GPIO_DeInit(UART_RX_PORT, UART_RX_PIN);
    HAL_GPIO_DeInit(UART_TX_PORT, UART_TX_PIN);
    CDC_UART_DISABLE();
    return 1;
}

int32_t uart_reset(void)
{
    NVIC_DisableIRQ(CDC_UART_IRQn);
    clear_buffers();
    NVIC_ClearPendingIRQ(CDC_UART_IRQn);
    NVIC_EnableIRQ(CDC_UART_IRQn);
    return 1;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    s_uart_handle.Instance = CDC_UART;
    HAL_UART_DeInit(&s_uart_handle);

    s_configuration.Parity = config->Parity;
    if (config->Parity == STM32_UART_PARITY_ODD) {
        s_uart_handle.Init.Parity = STM32_UART_PARITY_ODD;
    } else if (config->Parity == STM32_UART_PARITY_EVEN) {
        s_uart_handle.Init.Parity = STM32_UART_PARITY_EVEN;
    } else {
        s_uart_handle.Init.Parity = STM32_UART_PARITY_NONE;
    }

    s_configuration.StopBits = config->StopBits;
    if (config->StopBits == UART_STOP_BITS_2 || config->StopBits == UART_STOP_BITS_1_5) {
        s_uart_handle.Init.StopBits = UART_STOPBITS_2;
    } else {
        s_uart_handle.Init.StopBits = UART_STOPBITS_1;
    }

    s_configuration.DataBits = config->DataBits;
    s_uart_handle.Init.WordLength = (s_uart_handle.Init.Parity == STM32_UART_PARITY_NONE) ? UART_WORDLENGTH_8B : UART_WORDLENGTH_9B;
    s_configuration.FlowControl = UART_FLOW_CONTROL_NONE;
    s_uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;

    s_configuration.Baudrate = config->Baudrate;
    s_uart_handle.Init.BaudRate = config->Baudrate;
    s_uart_handle.Init.Mode = UART_MODE_TX_RX;
    s_uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&s_uart_handle);
    clear_buffers();

    CDC_UART->CR1 |= USART_CR1_RXNEIE;

    return 1;
}

int32_t uart_get_configuration(UART_Configuration *config)
{
    *config = s_configuration;
    return 1;
}

void uart_set_control_line_state(uint16_t ctrl_bmp) {}

int32_t uart_write_free(void)
{
    return circ_buf_count_free(&s_write_buffer);
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    uint32_t cnt = circ_buf_write(&s_write_buffer, data, size);
    if (cnt > 0) {
        CDC_UART->CR1 |= USART_CR1_TXEIE;
    }
    return cnt;
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    return circ_buf_read(&s_read_buffer, data, size);
}

void CDC_UART_IRQn_Handler(void)
{
    uint32_t sr = CDC_UART->SR;
    uint32_t cr1 = CDC_UART->CR1;

    // RX
    if ((sr & USART_SR_RXNE) && (cr1 & USART_CR1_RXNEIE)) {
        uint8_t data = (uint8_t)(CDC_UART->DR & 0xFF);
        uint16_t free = circ_buf_count_free(&s_read_buffer);
        if (free > RX_OVRF_MSG_SIZE) {
            circ_buf_push(&s_read_buffer, data);
        } else if (config_get_overflow_detect() && (free == RX_OVRF_MSG_SIZE)) {
            circ_buf_write(&s_read_buffer, (uint8_t *)RX_OVRF_MSG, RX_OVRF_MSG_SIZE);
        }
    }

    // TX
    if ((sr & USART_SR_TXE) && (cr1 & USART_CR1_TXEIE)) {
        if (circ_buf_count_used(&s_write_buffer) > 0) {
            CDC_UART->DR = circ_buf_pop(&s_write_buffer);
        } else {
            CDC_UART->CR1 &= ~USART_CR1_TXEIE;
        }
    }
}
