/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
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
 
#include "LPC11Uxx.h"

#include "gpio.h"

#define MSD_LED_BIT_NUMBER  (1UL << 4)
#define CDC_LED_BIT_NUMBER
#define HID_LED_BIT_NUMBER

void time_delay_ms(uint32_t delay)
{
	return;
}

void gpio_init(void)
{
    // clock for LED and button

    // LED outut config

    // RST button
	
	// delay 2ms for pull-up enable
	time_delay_ms(2);
	return;
}

void gpio_set_dap_led(uint8_t state)
{
    if (state) {
// LED on
    } else {
// LED off
    }
    return;
}

void gpio_set_cdc_led(uint8_t state)
{
    gpio_set_dap_led(state);
}

void gpio_set_msd_led(uint8_t state)
{
    gpio_set_dap_led(state);
}

uint8_t gpio_get_button_state(void)
{
    return 0;
}

uint8_t gpio_get_rst_pin_state(void)
{
    return 0;
}
