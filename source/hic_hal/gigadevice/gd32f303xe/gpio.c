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

#include "gd32f30x.h"
#include "RTL.h"
#include "gpio.h"
#include "compiler.h"
#include "target_reset.h"
#include "IO_Config.h"
#include "settings.h"

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
    /* configure GPIO-LED as output */
    RCU_APB2EN = LED_RCU;

    gpio_initial(LED_DAP_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_DAP_PIN);
    gpio_initial(LED_MSC_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_MSC_PIN);
    gpio_initial(LED_CDC_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_CDC_PIN);

    GPIO_BC(LED_DAP_PORT) = LED_DAP_PIN;
    GPIO_BC(LED_MSC_PORT) = LED_MSC_PIN;
    GPIO_BC(LED_CDC_PORT) = LED_CDC_PIN;

    /* configure USB-pullup pin */
    RCU_APB2EN = PIN_USB_CONNECT_RCU;

    gpio_initial(PIN_USB_CONNECT_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, PIN_USB_CONNECT);

    GPIO_BC(PIN_USB_CONNECT_PORT) = PIN_USB_CONNECT;

    /* Give the cap on the reset button time to charge */
    busy_wait(10000);

    if (gpio_get_reset_btn() || config_ram_get_initial_hold_in_bl()) {
        IRQn_Type irq;
        // Disable SYSTICK timer and interrupt before calling into ISP
        SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);

        // Disable all nvic interrupts
        for (irq = (IRQn_Type)0; irq < (IRQn_Type)32; irq++) {
            NVIC_DisableIRQ(irq);
            NVIC_ClearPendingIRQ(irq);
        }

        // If switching to "bootloader" mode then setup the watchdog
        // so it will exit CRP mode after ~30 seconds
        if (config_ram_get_initial_hold_in_bl()) {
//            LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 15); // Enable watchdog module
//            LPC_SYSCON->PDRUNCFG &= ~(1 << 6);      // Enable watchdog clock (WDOSC)
//            LPC_SYSCON->WDTOSCCTRL = (0xF << 5);    // Set max frequency - 2.3MHz
//            LPC_WWDT->CLKSEL = (1 << 0);            // Select watchdog clock
//            LPC_WWDT->TC = 0x00FFFFFF;              // Set time to reset to ~29s
//            LPC_WWDT->MOD = (1 << 0) | (1 << 1);    // Enable watchdog and set reset
//            LPC_WWDT->FEED = 0xAA;                  // Enable watchdog
//            LPC_WWDT->FEED = 0x55;
        }

//        iap_reinvoke();
    }
}

void gpio_set_board_power(bool powerEnabled)
{

}

void gpio_set_hid_led(gpio_led_state_t state)
{
    if (state) {
        GPIO_BOP(LED_DAP_PORT) = LED_DAP_PIN;
    } else {
        GPIO_BC(LED_DAP_PORT) = LED_DAP_PIN;
    }
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    if (state) {
        GPIO_BOP(LED_CDC_PORT) = LED_CDC_PIN;
    } else {
        GPIO_BC(LED_CDC_PORT) = LED_CDC_PIN;
    }
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    if (state) {
        GPIO_BOP(LED_MSC_PORT) = LED_MSC_PIN;
    } else {
        GPIO_BC(LED_MSC_PORT) = LED_MSC_PIN;
    }
}

uint8_t gpio_get_reset_btn_no_fwrd()
{
    if (SET == gpio_input_bit_get(PIN_nRESET_PORT, PIN_nRESET)) {
        return 0;
    } else {
        return 1;
    }
}

uint8_t gpio_get_reset_btn_fwrd()
{
    return 0;
}

void target_forward_reset(bool assert_reset)
{
    // Do nothing - reset is forwarded in gpio_get_sw_reset
}
