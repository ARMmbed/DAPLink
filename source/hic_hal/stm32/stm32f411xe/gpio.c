#include "stm32f4xx.h"
#include "DAP_config.h"
#include "gpio.h"
#include "daplink.h"

void gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Синий светодиод PC13 (выключен при высоком уровне)
    HAL_GPIO_WritePin(CONNECTED_LED_PORT, CONNECTED_LED_PIN, GPIO_PIN_SET);
    GPIO_InitStructure.Pin = CONNECTED_LED_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(CONNECTED_LED_PORT, &GPIO_InitStructure);

    // Встроенная кнопка KEY на Black Pill (PA0, подтянута к VCC)
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    HAL_GPIO_WritePin(PIN_HID_LED_PORT, PIN_HID_LED, state ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    HAL_GPIO_WritePin(PIN_CDC_LED_PORT, PIN_CDC_LED, state ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    HAL_GPIO_WritePin(PIN_MSC_LED_PORT, PIN_MSC_LED, state ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

// Зажатие кнопки KEY (PA0) при подаче питания переводит плату в режим Bootloader (MAINTENANCE)
bool reset_button_pressed(void)
{
    return (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET);
}
 
uint8_t gpio_get_reset_btn_no_fwrd(void)
{
    // Кнопка KEY (PA0, активный уровень 0). nRESET цели (PB15) здесь читать нельзя:
    // при выключенном SWD пин плавает и даёт ложные срабатывания.
    return (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) ? 1 : 0;
}

uint8_t gpio_get_reset_btn_fwrd(void)
{
    return 0;
}

uint8_t GPIOGetButtonState(void)
{
    return 0;
}

void target_forward_reset(bool assert_reset) {}
void gpio_set_board_power(bool powerEnabled) {}
