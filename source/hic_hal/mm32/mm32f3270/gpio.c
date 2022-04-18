/**
 * @file    gpio.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

#include "mm32_device.h"
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "DAP_config.h"
#include "gpio.h"
#include "daplink.h"
#include "util.h"


static void busy_wait(uint32_t cycles)
{
    volatile uint32_t i;
    i = cycles;

    while (i > 0) {
        i--;
    }
}


void gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // enable clock to ports
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);

    /* Enable USB connect pin */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    /* Disable JTAG to free pins for other uses. Note - SWD is still enabled */
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    USB_CONNECT_PORT_ENABLE();
    USB_CONNECT_OFF();
    GPIO_InitStructure.GPIO_Pin = USB_CONNECT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(USB_CONNECT_PORT, &GPIO_InitStructure);

    // configure LEDs
    GPIO_ResetBits(RUNNING_LED_PORT, RUNNING_LED_PIN);
    GPIO_InitStructure.GPIO_Pin = RUNNING_LED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RUNNING_LED_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(CONNECTED_LED_PORT, CONNECTED_LED_PIN);
    GPIO_InitStructure.GPIO_Pin = CONNECTED_LED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(CONNECTED_LED_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(PIN_CDC_LED_PORT, PIN_CDC_LED);
    GPIO_InitStructure.GPIO_Pin = PIN_CDC_LED;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(PIN_CDC_LED_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(PIN_MSC_LED_PORT, PIN_MSC_LED);
    GPIO_InitStructure.GPIO_Pin = PIN_MSC_LED;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(PIN_MSC_LED_PORT, &GPIO_InitStructure);

    // reset button configured as gpio open drain output with a pullup
    GPIO_SetBits(nRESET_PIN_PORT, nRESET_PIN);
    GPIO_InitStructure.GPIO_Pin = nRESET_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(nRESET_PIN_PORT, &GPIO_InitStructure);

    // Turn on power to the board. When the target is unpowered
    // it holds the reset line low.
    GPIO_ResetBits(POWER_EN_PIN_PORT, POWER_EN_PIN);
    GPIO_InitStructure.GPIO_Pin = POWER_EN_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(POWER_EN_PIN_PORT, &GPIO_InitStructure);

    // Setup the 8MHz MCO
    // GPIO_InitStructure.GPIO_Pin = GPIO_PIN_8;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    // GPIO_Init(GPIOA, &GPIO_InitStructure);
    // output_clock_enable();

    // Let the voltage rails stabilize.  This is especailly important
    // during software resets, since the target's 3.3v rail can take
    // 20-50ms to drain.  During this time the target could be driving
    // the reset pin low, causing the bootloader to think the reset
    // button is pressed.
    // Note: With optimization set to -O2 the value 1000000 delays for ~85ms
    busy_wait(1000000);
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    if (state) {
        GPIO_ResetBits(PIN_HID_LED_PORT, PIN_HID_LED); // LED on
    } else {
        GPIO_SetBits(PIN_HID_LED_PORT, PIN_HID_LED);   // LED off
    }
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    if (state) {
        GPIO_ResetBits(PIN_CDC_LED_PORT, PIN_CDC_LED); // LED on
    } else {
        GPIO_SetBits(PIN_CDC_LED_PORT, PIN_CDC_LED);   // LED off
    }
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    if (state) {
        GPIO_ResetBits(PIN_MSC_LED_PORT, PIN_MSC_LED); // LED on
    } else {
        GPIO_SetBits(PIN_MSC_LED_PORT, PIN_MSC_LED);   // LED off
    }
}

uint8_t gpio_get_reset_btn_no_fwrd(void)
{
    return (nRESET_PIN_PORT->IDR & nRESET_PIN) ? 0 : 1;
}

uint8_t gpio_get_reset_btn_fwrd(void)
{
    return 0;
}


uint8_t GPIOGetButtonState(void)
{
    return 0;
}

void target_forward_reset(bool assert_reset)
{
    // Do nothing - reset is forwarded in gpio_get_sw_reset
}

void gpio_set_board_power(bool powerEnabled)
{
}
