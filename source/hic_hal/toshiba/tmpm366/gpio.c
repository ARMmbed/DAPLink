/**
 * @file    gpio.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <TMPM366.h>
#include "DAP_config.h"
#include "gpio.h"
#include "target_reset.h"
#include "daplink.h"
#include "util.h"

COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_TMPM366);

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
    // Set PC0, PC1 to output, M066 STK
    TSB_PC->CR |= 0x03;
    // Set PE7 to input
    TSB_PE->IE |= 0x80;
    // led off
    gpio_set_hid_led(GPIO_LED_OFF);
    gpio_set_cdc_led(GPIO_LED_OFF);
    gpio_set_msc_led(GPIO_LED_OFF);

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
    if (state == GPIO_LED_OFF) {
        TSB_PC_DATA_PC0 = 1;
    } else {
        TSB_PC_DATA_PC0 = 0;
    }
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    if (state == GPIO_LED_OFF) {
        TSB_PC_DATA_PC1 = 1;
    } else {
        TSB_PC_DATA_PC1 = 0;
    }
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    // Only 2 LED on hardware which is used for DAP/MSD
}

uint8_t gpio_get_reset_btn_no_fwrd()
{
    return TSB_PE_DATA_PE7;
}

uint8_t gpio_get_reset_btn_fwrd()
{
    return 0;//TSB_PE_DATA_PE7;
}

void gpio_set_board_power(bool powerEnabled)
{

}
