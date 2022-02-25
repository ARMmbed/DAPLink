/**
 * @file    gpio.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2018, ARM Limited, All Rights Reserved
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
#include "IO_Config.h"
#include "stm32f0xx_hal.h"

#define GET_PIN_STATE(__state) ((__state) == GPIO_LED_OFF ? GPIO_PIN_RESET : GPIO_PIN_SET)

void gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    __HAL_RCC_USB_GPIO_CLK_ENABLE();
    __HAL_RCC_LED_GPIO_CLK_ENABLE();
    __HAL_RCC_BTN_BTL_CLK_ENABLE();
    
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;

    // LED
    HAL_GPIO_WritePin(HID_LED_PORT, HID_LED_PIN, GPIO_PIN_SET);
    GPIO_InitStructure.Pin = HID_LED_PIN;
    HAL_GPIO_Init(HID_LED_PORT, &GPIO_InitStructure);

    //HAL_GPIO_WritePin(CDC_LED_PORT, CDC_LED_PIN, GPIO_PIN_SET);
    //GPIO_InitStructure.Pin = CDC_LED_PIN;
    //HAL_GPIO_Init(CDC_LED_PORT, &GPIO_InitStructure);

    //HAL_GPIO_WritePin(MSC_LED_PORT, MSC_LED_PIN, GPIO_PIN_SET);
    //GPIO_InitStructure.Pin = MSC_LED_PIN;
    //HAL_GPIO_Init(MSC_LED_PORT, &GPIO_InitStructure);

    // USB
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;

    GPIO_InitStructure.Pin = GPIO_PIN_11;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_12;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Button BTL
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;

    HAL_GPIO_WritePin(BTN_BTL_PORT, BTN_BTL_PIN, GPIO_PIN_SET);
    GPIO_InitStructure.Pin = BTN_BTL_PIN;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BTN_BTL_PORT, &GPIO_InitStructure);    
}

void gpio_set_board_power(bool powerEnabled)
{
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    HAL_GPIO_WritePin(HID_LED_PORT, HID_LED_PIN, GET_PIN_STATE(!state));
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    HAL_GPIO_WritePin(CDC_LED_PORT, CDC_LED_PIN, GET_PIN_STATE(!state));
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    HAL_GPIO_WritePin(MSC_LED_PORT, MSC_LED_PIN, GET_PIN_STATE(!state));
}

uint8_t gpio_get_reset_btn_no_fwrd(void)
{
    return 0;
}

uint8_t gpio_get_reset_btn_fwrd(void)
{
    uint8_t ret = 0;
    GPIO_PinState pinStat;
    
    
    pinStat = HAL_GPIO_ReadPin(BTN_BTL_PORT, BTN_BTL_PIN);
    if (pinStat == GPIO_PIN_RESET) {
        ret = 1;
    }

    return ret;
}
