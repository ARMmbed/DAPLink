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
#include "MK20D5.h"
#include "RTL.h"
#include "DAP_config.h"
#include "gpio.h"

// Power target in all configurations
// aside from bootloader
#define INTERFACE_POWER_EN  !defined(DAPLINK_BL)

void gpio_init(void)
{
    // enable clock to ports
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTB_MASK;
    
    // configure pin as GPIO
    LED_CONNECTED_PORT->PCR[LED_CONNECTED_BIT] = PORT_PCR_MUX(1);

    // led off - enable output
    LED_CONNECTED_GPIO->PDOR = 1UL << LED_CONNECTED_BIT;
    LED_CONNECTED_GPIO->PDDR = 1UL << LED_CONNECTED_BIT;

    // led on
    LED_CONNECTED_GPIO->PCOR  |= 1UL << LED_CONNECTED_BIT;

    // reset button configured as gpio input
    PIN_nRESET_GPIO->PDDR &= ~PIN_nRESET;
    PIN_nRESET_PORT->PCR[PIN_nRESET_BIT] = PORT_PCR_MUX(1);

#if INTERFACE_POWER_EN
    // configure pin as GPIO
    PIN_POWER_EN_PORT->PCR[PIN_POWER_EN_BIT] = PORT_PCR_MUX(1);
	// force always on logic 1
    PIN_POWER_EN_GPIO->PDOR |= 1UL << PIN_POWER_EN_BIT;
    PIN_POWER_EN_GPIO->PDDR |= 1UL << PIN_POWER_EN_BIT;
#endif
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    if (state)
    {
        LED_CONNECTED_GPIO->PCOR = 1UL << LED_CONNECTED_BIT; // LED on
    }
    else
    {
        LED_CONNECTED_GPIO->PSOR = 1UL << LED_CONNECTED_BIT; // LED off
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
    //TODO - Handle reset button on k20dx.  The logic below does not work
    //       correctly because as soon as the reset button is pressed
    //       reset is reconfigured as an output.  That causes this
    //       this function to return that the reset button is not
    //       pressed.  For now just return that the button is not
    //       pressed.
    uint8_t reset_driven;
    uint8_t not_pressed;

    // Normally there is a dedicated pin to set reset and a
    // dedicated pin to read the reset button.  This is not the
    // case for boards using the K20DX which uses the same pin
    // to read the reset button and to trigger a reset.  Because
    // of this button reads must be masked off when the reset
    // button is being driven.
    
    // Pin is open drian so it is only driven when it is an output and set low
    reset_driven = !(PIN_nRESET_GPIO->PDOR & PIN_nRESET) && (PIN_nRESET_GPIO->PDDR & PIN_nRESET);
    not_pressed = PIN_nRESET_GPIO->PDIR & PIN_nRESET;

    return not_pressed || reset_driven;
}

uint8_t GPIOGetButtonState(void)
{
    return 0;
}
