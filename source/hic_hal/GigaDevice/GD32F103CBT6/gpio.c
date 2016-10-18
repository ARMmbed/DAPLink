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

#include "gd32f10x.h"
#include "RTL.h"
#include "DAP_config.h"
#include "gpio.h"
#include "target_reset.h"
#include "daplink.h"


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
    GPIO_InitPara  GPIO_InitStructure;
    // Enable ports clock
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOA|RCC_APB2PERIPH_GPIOB|RCC_APB2PERIPH_GPIOC, ENABLE);
    // Config usb_control_pin
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_AF, ENABLE);
    GPIO_PinRemapConfig(GPIO_REMAP_SWJ_JTAGDISABLE, ENABLE);

    USB_CONNECT_PORT_ENABLE();
    USB_CONNECT_OFF();
    GPIO_InitStructure.GPIO_Pin   = USB_CONNECT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_Init(USB_CONNECT_PORT, &GPIO_InitStructure);
    //
    GPIO_InitStructure.GPIO_Pin   = RUNNING_LED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_Init(RUNNING_LED_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(RUNNING_LED_PORT, RUNNING_LED_PIN);

    GPIO_InitStructure.GPIO_Pin   = CONNECTED_LED_PIN;
    GPIO_Init(CONNECTED_LED_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(CONNECTED_LED_PORT, CONNECTED_LED_PIN);

    // reset button configured as gpio input_pullup
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IPU;
    GPIO_InitStructure.GPIO_Pin  = nRESET_PIN;
    GPIO_Init(nRESET_PIN_PORT, &GPIO_InitStructure);

    // Keep powered off in bootloader mode
    // to prevent the target from effecting the state
    // of the reset line / reset button
    if (!daplink_is_bootloader()) {
        // configure pin as GPIO
        // force always on logic 1
        GPIO_InitStructure.GPIO_Pin = POWER_EN_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
        GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
        GPIO_Init(POWER_EN_PIN_PORT, &GPIO_InitStructure);
        GPIO_SetBits(POWER_EN_PIN_PORT, POWER_EN_PIN);
    }
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
    if(state) {
        GPIO_ResetBits(CONNECTED_LED_PORT, CONNECTED_LED_PIN); // LED on
    } else {
        GPIO_SetBits(CONNECTED_LED_PORT, CONNECTED_LED_PIN);   // LED off
    }
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    gpio_set_hid_led(state);
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    gpio_set_hid_led(state);
}

uint8_t gpio_get_sw_reset(void)
{
    static uint8_t last_reset_forward_pressed = 0;
    uint8_t reset_forward_pressed;
    uint8_t reset_pressed;

    // Set nRESET_PIN to input pull-up, then read status
    pin_in(nRESET_PIN_PORT, nRESET_PIN_Bit, 2);
    busy_wait(5);
    reset_forward_pressed = (nRESET_PIN_PORT->DIR & nRESET_PIN) ? 0 : 1;
    // Forward reset if the state of the button has changed
    //    This must be done on button changes so it does not interfere
    //    with other reset sources such as programming or CDC Break
    if(last_reset_forward_pressed != reset_forward_pressed) {
#if defined(DAPLINK_IF)
        if(reset_forward_pressed) {
            target_set_state(RESET_HOLD);
        }
        else {
            target_set_state(RESET_RUN);
        }
#endif
        last_reset_forward_pressed = reset_forward_pressed;
    }
    reset_pressed = reset_forward_pressed || ((nRESET_PIN_PORT->DIR & nRESET_PIN) ? 0 : 1);
    // Config nRESET_PIN to output
    pin_out(nRESET_PIN_PORT, nRESET_PIN_Bit);
    nRESET_PIN_PORT->BOR = nRESET_PIN;

    return !reset_pressed;
}

uint8_t GPIOGetButtonState(void)
{
    return 0;
}

void target_forward_reset(bool assert_reset)
{
    // Do nothing
}
