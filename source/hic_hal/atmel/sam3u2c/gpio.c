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

#include "sam3u.h"
#include "RTL.h"
#include "gpio.h"
#include "uart.h"
#include "compiler.h"
#include "daplink.h"

// This GPIO configuration is only valid for the SAM3U2C
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_SAM3U2C);

#define _BIT_LED_BLUE       (31)      // PA31
#define _BIT_LED_RED       (29)      // PA29
#define _BIT_LED_GREEN       (28)      // PA28
#define _BIT_BOOT_MODE_PIN   (25)      // PA25

void gpio_init(void)
{
    volatile int Cnt;
    //
    // Initially enable clock for GPIOA and initialize LED port as output with LED == off
    //
    PMC->PMC_PCER0 = (1 << 10);  // Enable clock for PIOA
    PIOA->PIO_PER  = (1 << _BIT_LED_GREEN) | (1 << _BIT_LED_RED) | (1 << _BIT_LED_BLUE) | (1 << _BIT_BOOT_MODE_PIN);  // Pin == GPIO control
    PIOA->PIO_SODR = (1 << _BIT_LED_GREEN) | (1 << _BIT_LED_RED) | (1 << _BIT_LED_BLUE); // Green LED == off
    PIOA->PIO_OER  = (1 << _BIT_LED_GREEN) | (1 << _BIT_LED_RED) | (1 << _BIT_LED_BLUE); // Pin == output
    PIOA->PIO_ODR  = (1 << _BIT_BOOT_MODE_PIN); // Disable output
    PIOA->PIO_PUER = (1 << _BIT_BOOT_MODE_PIN); // Enable pull-up
    Cnt = 1000000;
    do {} while (--Cnt);    // Give pull-up some time to become active

    // Enable port A interrupts in the NVIC
    NVIC_EnableIRQ(PIOA_IRQn);
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    if (GPIO_LED_ON == state) {
        PIOA->PIO_CODR = (1 << _BIT_LED_GREEN); // Green LED == on
    } else {
        PIOA->PIO_SODR = (1 << _BIT_LED_GREEN); // Green LED == off
    }
}

void gpio_set_cdc_led(gpio_led_state_t state)
  if (GPIO_LED_ON == state) {
    PIOA->PIO_CODR = (1 << _BIT_LED_BLUE); //  LED == on
  } else {
    PIOA->PIO_SODR = (1 << _BIT_LED_BLUE); //  LED == off
  }
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    if (GPIO_LED_ON == state) {
    PIOA->PIO_CODR = (1 << _BIT_LED_RED); //  LED == on
    } else {
    PIOA->PIO_SODR = (1 << _BIT_LED_RED); //  LED == off
    }
}

void PIOA_IRQHandler(void)
{
    //
    // ISR is called when flow control is de-asserted
    //
    uint32_t interrupts = PIOA->PIO_ISR;

    if ((interrupts >> 9) & 1) { //CTS
        uart_software_flow_control();
    }
}

uint8_t gpio_get_sw_reset()
{
    return (PIOA->PIO_PDSR & (1 << _BIT_BOOT_MODE_PIN)) != 0;
}
