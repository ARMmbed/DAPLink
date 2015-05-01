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
#include "sam3u2c.h"

// AG xxxxxxxxxxxxxxxxxx

#define _BIT_LED_GREEN       (29)      // PA29
#define _BIT_LED_RED         (28)      // PA28
#define _BIT_BOOT_MODE_PIN   (25)      // PA25


void gpio_init(void) {
  volatile int Cnt;
  //
	// Configure PA25 as input with pull-up
	// Used to detect if FW shall be started or if we should stay in BTL
	//
	PMC->PMC_PCER0 = (1 << 10);  // Enable clock for PIOA
  PIOA->PIO_PER  = (1 << _BIT_BOOT_MODE_PIN);  // Enable GPIO functionality and disable peripheral function of pin
	PIOA->PIO_ODR  = (1 << _BIT_BOOT_MODE_PIN); // Disable output
	PIOA->PIO_PUER = (1 << _BIT_BOOT_MODE_PIN); // Enable pull-up	
  Cnt = 1000000;
	do {} while (--Cnt);    // Give pull-up some time to become active
  //
	// Configure LEDs
	//
  PIOA->PIO_SODR = (1 << _BIT_LED_GREEN) | (1 << _BIT_LED_RED); // Initially off
  PIOA->PIO_OER = (1 << _BIT_LED_GREEN) | (1 << _BIT_LED_RED);
	PIOA->PIO_PER = (1 << _BIT_LED_GREEN) | (1 << _BIT_LED_RED);
}

void gpio_set_dap_led(uint8_t state) {
  if (state) {
    PIOA->PIO_CODR = (1 << _BIT_LED_GREEN);
  } else {
    PIOA->PIO_SODR = (1 << _BIT_LED_GREEN);
  }
}

void gpio_set_msd_led(uint8_t state) {
  gpio_set_dap_led(state);
}

void gpio_set_cdc_led(uint8_t state) {
  gpio_set_dap_led(state);
}

uint8_t gpio_get_pin_loader_state(void) {
	return (PIOA->PIO_PDSR >> _BIT_BOOT_MODE_PIN) & 1;
}
