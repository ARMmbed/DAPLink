#ifndef __DAP_CONFIG_H__
#define __DAP_CONFIG_H__

#include "IO_Config.h"

#define CPU_CLOCK               SystemCoreClock
#define IO_PORT_WRITE_CYCLES    2U
#define DAP_SWD                 1
#define DAP_JTAG                0
#define DAP_JTAG_DEV_CNT        0
#define DAP_DEFAULT_PORT        1
#define DAP_DEFAULT_SWJ_CLOCK   5000000

// Для Full-Speed USB размер пакета равен 64 байтам
#define DAP_PACKET_SIZE         64
#define DAP_PACKET_COUNT        4

#define SWO_UART                0
#define SWO_UART_DRIVER         0
#define SWO_UART_MAX_BAUDRATE   10000000U
#define SWO_MANCHESTER          0
#define SWO_BUFFER_SIZE         4096U
#define SWO_STREAM              0

#define TIMESTAMP_CLOCK         1000000U

#define DAP_UART                0
#define DAP_UART_DRIVER         1
#define DAP_UART_RX_BUFFER_SIZE 1024U
#define DAP_UART_TX_BUFFER_SIZE 1024U
#define DAP_UART_USB_COM_PORT   1

#define TARGET_FIXED            0

__STATIC_INLINE void PORT_JTAG_SETUP(void) {}

__STATIC_INLINE void PORT_SWD_SETUP(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    // SWCLK -> Output Push-Pull HIGH
    HAL_GPIO_WritePin(SWCLK_TCK_PIN_PORT, SWCLK_TCK_PIN, GPIO_PIN_SET);
    gpio_init.Pin = SWCLK_TCK_PIN;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(SWCLK_TCK_PIN_PORT, &gpio_init);

    // SWDIO -> Output Push-Pull HIGH
    HAL_GPIO_WritePin(SWDIO_OUT_PIN_PORT, SWDIO_OUT_PIN, GPIO_PIN_SET);
    gpio_init.Pin = SWDIO_OUT_PIN;
    HAL_GPIO_Init(SWDIO_OUT_PIN_PORT, &gpio_init);

    // nRESET -> Output Open-Drain HIGH (с внутренней подтяжкой)
    HAL_GPIO_WritePin(nRESET_PIN_PORT, nRESET_PIN, GPIO_PIN_SET);
    gpio_init.Pin = nRESET_PIN;
    gpio_init.Mode = GPIO_MODE_OUTPUT_OD;
    gpio_init.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(nRESET_PIN_PORT, &gpio_init);
}

__STATIC_INLINE void PORT_OFF(void)
{
    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_NOPULL;

    gpio_init.Pin = SWCLK_TCK_PIN;
    HAL_GPIO_Init(SWCLK_TCK_PIN_PORT, &gpio_init);

    gpio_init.Pin = SWDIO_OUT_PIN;
    HAL_GPIO_Init(SWDIO_OUT_PIN_PORT, &gpio_init);

    gpio_init.Pin = nRESET_PIN;
    HAL_GPIO_Init(nRESET_PIN_PORT, &gpio_init);
}

// SWCLK
__STATIC_FORCEINLINE uint32_t PIN_SWCLK_TCK_IN(void)
{
    return (SWCLK_TCK_PIN_PORT->ODR >> SWCLK_TCK_PIN_Bit) & 1U;
}

__STATIC_FORCEINLINE void PIN_SWCLK_TCK_SET(void)
{
    SWCLK_TCK_PIN_PORT->BSRR = SWCLK_TCK_PIN;
}

__STATIC_FORCEINLINE void PIN_SWCLK_TCK_CLR(void)
{
    SWCLK_TCK_PIN_PORT->BSRR = (uint32_t)SWCLK_TCK_PIN << 16;
}

// SWDIO
__STATIC_FORCEINLINE uint32_t PIN_SWDIO_TMS_IN(void)
{
    return (SWDIO_IN_PIN_PORT->IDR >> SWDIO_IN_PIN_Bit) & 1U;
}

__STATIC_FORCEINLINE void PIN_SWDIO_TMS_SET(void)
{
    SWDIO_OUT_PIN_PORT->BSRR = SWDIO_OUT_PIN;
}

__STATIC_FORCEINLINE void PIN_SWDIO_TMS_CLR(void)
{
    SWDIO_OUT_PIN_PORT->BSRR = (uint32_t)SWDIO_OUT_PIN << 16;
}

__STATIC_FORCEINLINE uint32_t PIN_SWDIO_IN(void)
{
    return (SWDIO_IN_PIN_PORT->IDR >> SWDIO_IN_PIN_Bit) & 1U;
}

__STATIC_FORCEINLINE void PIN_SWDIO_OUT(uint32_t bit)
{
    if (bit & 1U) {
        SWDIO_OUT_PIN_PORT->BSRR = SWDIO_OUT_PIN;
    } else {
        SWDIO_OUT_PIN_PORT->BSRR = (uint32_t)SWDIO_OUT_PIN << 16;
    }
}

__STATIC_FORCEINLINE void PIN_SWDIO_OUT_ENABLE(void)
{
    SWDIO_OUT_PIN_PORT->BSRR = (uint32_t)SWDIO_OUT_PIN << 16;
    SWDIO_OUT_PIN_PORT->MODER = (SWDIO_OUT_PIN_PORT->MODER & ~(3UL << (SWDIO_OUT_PIN_Bit * 2))) | (1UL << (SWDIO_OUT_PIN_Bit * 2));
}

__STATIC_FORCEINLINE void PIN_SWDIO_OUT_DISABLE(void)
{
    SWDIO_IN_PIN_PORT->MODER &= ~(3UL << (SWDIO_IN_PIN_Bit * 2));
}

// JTAG заглушки
__STATIC_FORCEINLINE uint32_t PIN_TDI_IN(void) { return 0; }
__STATIC_FORCEINLINE void PIN_TDI_OUT(uint32_t bit) {}
__STATIC_FORCEINLINE uint32_t PIN_TDO_IN(void) { return 0; }
__STATIC_FORCEINLINE uint32_t PIN_nTRST_IN(void) { return 0; }
__STATIC_FORCEINLINE void PIN_nTRST_OUT(uint32_t bit) {}

// nRESET
__STATIC_FORCEINLINE uint32_t PIN_nRESET_IN(void)
{
    return (nRESET_PIN_PORT->IDR >> nRESET_PIN_Bit) & 1U;
}

__STATIC_FORCEINLINE void PIN_nRESET_OUT(uint32_t bit)
{
    if (bit & 1U) {
        nRESET_PIN_PORT->BSRR = nRESET_PIN;
    } else {
        nRESET_PIN_PORT->BSRR = (uint32_t)nRESET_PIN << 16;
    }
}

// Светодиод на PC13 (активный уровень 0)
__STATIC_INLINE void LED_CONNECTED_OUT(uint32_t bit)
{
    if (bit & 1U) {
        CONNECTED_LED_PORT->BSRR = (uint32_t)CONNECTED_LED_PIN << 16; // ON
    } else {
        CONNECTED_LED_PORT->BSRR = CONNECTED_LED_PIN; // OFF
    }
}

__STATIC_INLINE void LED_RUNNING_OUT(uint32_t bit)
{
    if (bit & 1U) {
        RUNNING_LED_PORT->BSRR = (uint32_t)RUNNING_LED_PIN << 16; // ON
    } else {
        RUNNING_LED_PORT->BSRR = RUNNING_LED_PIN; // OFF
    }
}

__STATIC_INLINE uint32_t TIMESTAMP_GET(void)
{
    return (DWT->CYCCNT) / (CPU_CLOCK / TIMESTAMP_CLOCK);
}

__STATIC_INLINE void DAP_SETUP(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    PORT_OFF();

    // Счётчик циклов для TIMESTAMP_GET
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    HAL_GPIO_WritePin(CONNECTED_LED_PORT, CONNECTED_LED_PIN, GPIO_PIN_SET);
    gpio_init.Pin = CONNECTED_LED_PIN;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CONNECTED_LED_PORT, &gpio_init);
}

__STATIC_INLINE uint32_t RESET_TARGET(void)
{
    return 0;
}

#endif /* __DAP_CONFIG_H__ */
