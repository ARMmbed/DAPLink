/**
 * @file    gpio_weak.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2018, Findlay Feng, All Rights Reserved
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

#include "gpio.h"
#include "stm32f1xx_hal.h"

__weak void gpio_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;

    GPIO_InitStructure.Pin = GPIO_PIN_11;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_12;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}

__weak void gpio_set_board_power(bool powerEnabled)
{
}

__weak void gpio_set_hid_led(gpio_led_state_t state)
{
}

__weak void gpio_set_cdc_led(gpio_led_state_t state)
{
}

__weak void gpio_set_msc_led(gpio_led_state_t state)
{
}

__weak uint8_t gpio_get_reset_btn_no_fwrd(void)
{
    return 0;
}

__weak uint8_t gpio_get_reset_btn_fwrd(void)
{
    return 0;
}
