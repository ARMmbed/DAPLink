/**
 * @file    gpio.c
 * @brief   GPIO access functions for nrf52820 HIC
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2021, Arm Limited, All Rights Reserved
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

#include "IO_config.h"

#if (defined(GPIO_LED_ACTIVE_STATE) && (GPIO_LED_ACTIVE_STATE == 1))
#define GPIO_LED_STATE(state) (state ? 1 : 0)
#else
#define GPIO_LED_STATE(state) (state ? 0 : 1)
#endif

void gpio_init(void)
{
#ifdef LED_PWR
    nrf_gpio_cfg_output(LED_PWR);
    nrf_gpio_pin_write(LED_PWR, GPIO_LED_STATE(1));
#endif
#ifdef LED_CONNECTED
    nrf_gpio_cfg_output(LED_CONNECTED);
#endif
#ifdef LED_RUNNING
    nrf_gpio_cfg_output(LED_RUNNING);
#endif
    nrf_gpio_cfg_output(LED_HID);
    nrf_gpio_cfg_output(LED_MSC);
    nrf_gpio_cfg_output(LED_CDC);

    nrf_gpio_cfg_input(RESET_BUTTON, RESET_BUTTON_PULL);
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    nrf_gpio_pin_write(LED_HID, GPIO_LED_STATE(state));
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    nrf_gpio_pin_write(LED_CDC, GPIO_LED_STATE(state));
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    nrf_gpio_pin_write(LED_MSC, GPIO_LED_STATE(state));
}

uint8_t gpio_get_reset_btn_no_fwrd(void)
{
    return 0;
}

uint8_t gpio_get_reset_btn_fwrd(void)
{
    return nrf_gpio_pin_read(RESET_BUTTON) ? 0 : 1;
}

void gpio_set_board_power(bool powerEnabled)
{
}
