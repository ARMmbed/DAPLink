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

#include "gpio.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"

void gpio_init(void)
{
    /* Enable GPIO clock */
    CMU_ClockEnable(cmuClock_GPIO, true);
    
    // led off
    gpio_set_hid_led(GPIO_LED_OFF);
    gpio_set_cdc_led(GPIO_LED_OFF);
    gpio_set_msc_led(GPIO_LED_OFF);
    
    // set as output
		GPIO_PinModeSet(PIN_HID_LED_PORT, PIN_HID_LED_BIT, gpioModePushPull, 1);
		GPIO_PinModeSet(PIN_MSC_LED_PORT, PIN_MSC_LED_BIT, gpioModePushPull, 1);
		GPIO_PinModeSet(PIN_CDC_LED_PORT, PIN_CDC_LED_BIT, gpioModePushPull, 1);
    
    // set as input
		GPIO_PinModeSet(PIN_SW_RESET_PORT, PIN_SW_RESET_BIT, gpioModeInput, 1);
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    (GPIO_LED_ON == state) ? (GPIO->P[PIN_HID_LED_GPIO].DOUTCLR = PIN_HID_LED) : (GPIO->P[PIN_HID_LED_GPIO].DOUTSET = PIN_HID_LED);
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    (GPIO_LED_ON == state) ? (GPIO->P[PIN_CDC_LED_GPIO].DOUTCLR = PIN_CDC_LED) : (GPIO->P[PIN_CDC_LED_GPIO].DOUTSET = PIN_CDC_LED);
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    (GPIO_LED_ON == state) ? (GPIO->P[PIN_MSC_LED_GPIO].DOUTCLR = PIN_MSC_LED) : (GPIO->P[PIN_MSC_LED_GPIO].DOUTSET = PIN_MSC_LED);
}

uint8_t gpio_get_sw_reset(void)
{
    return (GPIO->P[PIN_SW_RESET_GPIO].DIN & PIN_SW_RESET) ? 1 : 0;
}
