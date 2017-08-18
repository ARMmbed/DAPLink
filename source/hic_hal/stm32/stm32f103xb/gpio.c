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

#include "stm32f10x.h"
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
    GPIO_InitTypeDef GPIO_InitStructure;
    // enable clock to ports
    RCC->APB2ENR |= RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC;
    // Enable USB connect pin
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

#ifdef USB_CONNECT_PIN
    USB_CONNECT_PORT_ENABLE();
    USB_CONNECT_OFF();
    GPIO_InitStructure.GPIO_Pin = USB_CONNECT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(USB_CONNECT_PORT, &GPIO_InitStructure);
#endif
#ifdef USB_PULLUP_PIN
    GPIO_InitStructure.GPIO_Pin = USB_PULLUP_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(USB_PULLUP_PORT, &GPIO_InitStructure);
    GPIO_SetBits(USB_PULLUP_PORT, USB_PULLUP_PIN);
#endif

    // Enable MCO output
#ifdef MCO_PIN
    RCC_MCOConfig(RCC_MCO_HSE);
    GPIO_InitStructure.GPIO_Pin = MCO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(MCO_PIN_PORT, &GPIO_InitStructure);
#endif

    // configure LEDs
#ifdef RUNNING_LED_PIN
    GPIO_InitStructure.GPIO_Pin = RUNNING_LED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RUNNING_LED_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(RUNNING_LED_PORT, RUNNING_LED_PIN);
#endif
#ifdef CONNECTED_LED_PIN
    GPIO_InitStructure.GPIO_Pin = CONNECTED_LED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(CONNECTED_LED_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(CONNECTED_LED_PORT, CONNECTED_LED_PIN);
#endif
#ifdef PIN_CDC_LED
    GPIO_InitStructure.GPIO_Pin = PIN_CDC_LED;
    GPIO_Init(PIN_CDC_LED_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(PIN_CDC_LED_PORT, PIN_CDC_LED);
#endif
#ifdef PIN_HID_LED
    GPIO_InitStructure.GPIO_Pin = PIN_HID_LED;
    GPIO_Init(PIN_HID_LED_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(PIN_HID_LED_PORT, PIN_HID_LED);
#endif
#ifdef PIN_MSC_LED
    GPIO_InitStructure.GPIO_Pin = PIN_MSC_LED;
    GPIO_Init(PIN_MSC_LED_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(PIN_MSC_LED_PORT, PIN_MSC_LED);
#endif
    // reset button configured as gpio input_pullup
    GPIO_InitStructure.GPIO_Pin = nRESET_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(nRESET_PIN_PORT, &GPIO_InitStructure);

#ifdef POWER_EN_PIN
    // Keep powered off in bootloader mode
    // to prevent the target from effecting the state
    // of the reset line / reset button
    if (!daplink_is_bootloader() || !POWER_DOWN_DURING_BOOTLOADER) {
        // configure pin as GPIO
        // force always on logic 1
        GPIO_InitStructure.GPIO_Pin = POWER_EN_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(POWER_EN_PIN_PORT, &GPIO_InitStructure);
#if (POWER_EN_PIN_ACTIVE_HIGH)
        GPIO_SetBits(POWER_EN_PIN_PORT, POWER_EN_PIN);
#else
        GPIO_ResetBits(POWER_EN_PIN_PORT, POWER_EN_PIN);
#endif
    }
#endif
    // Let the voltage rails stabilize.  This is especailly important
    // during software resets, since the target's 3.3v rail can take
    // 20-50ms to drain.  During this time the target could be driving
    // the reset pin low, causing the bootloader to think the reset
    // button is pressed.
    // Note: With optimization set to -O2 the value 1000000 delays for ~85ms
    busy_wait(1000000);

#ifdef STLINK_V2_1_BOOTLOADER
    if (gpio_get_sw_reset() == 0) {
        // Re-invoke the STLink/v2-1 bootloader
        NVIC_SystemReset();
    }
#endif
}

void gpio_set_hid_led(gpio_led_state_t state)
{
#ifdef PIN_HID_LED
    if (state) {
        GPIO_ResetBits(PIN_HID_LED_PORT, PIN_HID_LED); // LED on
    } else {
        GPIO_SetBits(PIN_HID_LED_PORT, PIN_HID_LED);   // LED off
    }
#else
    (void)state;
#endif
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
#ifdef PIN_CDC_LED
    if (state) {
        GPIO_ResetBits(PIN_CDC_LED_PORT, PIN_CDC_LED); // LED on
    } else {
        GPIO_SetBits(PIN_CDC_LED_PORT, PIN_CDC_LED);   // LED off
    }
#else
    (void)state;
#endif
}

void gpio_set_msc_led(gpio_led_state_t state)
{
#ifdef PIN_MSC_LED
    if (state) {
        GPIO_ResetBits(PIN_MSC_LED_PORT, PIN_MSC_LED); // LED on
    } else {
        GPIO_SetBits(PIN_MSC_LED_PORT, PIN_MSC_LED);   // LED off
    }
#else
    (void)state;
#endif
}

uint8_t gpio_get_sw_reset(void)
{
    static uint8_t last_reset_forward_pressed = 0;
    uint8_t reset_forward_pressed;
    uint8_t reset_pressed;

    // Set nRESET_PIN to input pull-up, then read status
    pin_in_init(nRESET_PIN_PORT, nRESET_PIN_Bit, 1);
    busy_wait(5);
    reset_forward_pressed = (nRESET_PIN_PORT->IDR & nRESET_PIN) ? 0 : 1;
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
    reset_pressed = reset_forward_pressed || ((nRESET_PIN_PORT->IDR & nRESET_PIN) ? 0 : 1);
    // Config nRESET_PIN to an open drain output
    pin_out_init_od(nRESET_PIN_PORT, nRESET_PIN_Bit);
    nRESET_PIN_PORT->BSRR = nRESET_PIN;

    return !reset_pressed;
}

uint8_t GPIOGetButtonState(void)
{
    return 0;
}

void target_forward_reset(bool assert_reset)
{
    // Do nothing - reset is forwarded in gpio_get_sw_reset
}
