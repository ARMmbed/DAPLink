/**
 * @file    gpio_extra.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2021 Micro:bit Educational Foundation
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

#include "gpio_extra.h"

#include "fsl_port.h"
#include "fsl_gpio.h"

// COMBINED_SENSOR_INT PTA1
#define COMBINED_SENSOR_INT_PORT        PORTA
#define COMBINED_SENSOR_INT_GPIO        GPIOA
#define COMBINED_SENSOR_INT_PIN         (1)

void gpio_enable_hid_led()
{
    PIN_HID_LED_PORT->PCR[PIN_HID_LED_BIT] = PORT_PCR_MUX(1);
}

void gpio_disable_hid_led()
{
   PIN_HID_LED_PORT->PCR[PIN_HID_LED_BIT] = PORT_PCR_MUX(0);
}

void gpio_init_combined_int()
{
    gpio_pin_config_t pin_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* COMBINED_SENSOR_INT pin mux ALT0 (Disabled High-Z) */
    PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_PinDisabledOrAnalog);
    /* COMBINED_SENSOR_INT as output default low when pin mux ALT1 */
    GPIO_PinInit(COMBINED_SENSOR_INT_GPIO, COMBINED_SENSOR_INT_PIN, &pin_config);
}

void gpio_disable_combined_int()
{
    PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_PinDisabledOrAnalog);
}

void gpio_assert_combined_int()
{
    PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_MuxAsGpio);
}
