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
#include "sam3u.h"
#include "RTL.h"
#include "gpio.h"
#include "uart.h"

#define _BIT_LED_GREEN       (29)      // PA29
#define _BIT_BOOT_MODE_PIN   (25)      // PA25

static uint16_t isr_flags;
static OS_TID isr_notify;

void gpio_init(void) {
  volatile int Cnt;
  //
	// Initially enable clock for GPIOA and initialize LED port as output with LED == off
	//
    PMC->PMC_PCER0 = (1 << 10);  // Enable clock for PIOA
    PIOA->PIO_PER  = (1 << _BIT_LED_GREEN);  // Pin == GPIO control
    PIOA->PIO_SODR = (1 << _BIT_LED_GREEN); // Green LED == off
    PIOA->PIO_OER  = (1 << _BIT_LED_GREEN); // Pin == output
    PIOA->PIO_PER  = (1 << _BIT_BOOT_MODE_PIN);  // Enable GPIO functionality and disable peripheral function of pin
	PIOA->PIO_ODR  = (1 << _BIT_BOOT_MODE_PIN); // Disable output
	PIOA->PIO_PUER = (1 << _BIT_BOOT_MODE_PIN); // Enable pull-up	
    Cnt = 1000000;
	do {} while (--Cnt);    // Give pull-up some time to become active
}

void gpio_set_dap_led(uint8_t state) {
  if (state) {
    PIOA->PIO_CODR = (1 << _BIT_LED_GREEN); // Green LED == on
  } else {
    PIOA->PIO_SODR = (1 << _BIT_LED_GREEN); // Green LED == off
  }
}

void gpio_set_cdc_led(uint8_t state) {
// Only 1 LED on hardware which is used for DAP/MSD
}

void gpio_set_msd_led(uint8_t state) {
  if (state) {
    PIOA->PIO_CODR = (1 << _BIT_LED_GREEN); // Green LED == on
  } else {
    PIOA->PIO_SODR = (1 << _BIT_LED_GREEN); // Green LED == off
  }
}

void gpio_enable_button_flag(OS_TID task, uint16_t flags) {
	/* Keep a local copy of task & flags */
	isr_notify=task;
	isr_flags=flags;
	//
	// Enable interrupt on falling edge of "reset" button
  //
  PIOA->PIO_AIMER  = (1 << _BIT_BOOT_MODE_PIN);  // Enable additional interrupt mode
  PIOA->PIO_ESR    = (1 << _BIT_BOOT_MODE_PIN);  // Interrupt shall trigger on edges
  PIOA->PIO_FELLSR = (1 << _BIT_BOOT_MODE_PIN);  // Interrupt shall trigger on falling edges only.
  PIOA->PIO_IER    = (1 << _BIT_BOOT_MODE_PIN);  // Enable interrupt for "reset" pin
  PIOA->PIO_IFER   = (1 << _BIT_BOOT_MODE_PIN);  // Enable input filter
  PIOA->PIO_SCIFSR = (1 << _BIT_BOOT_MODE_PIN);  // Filter glitches
  PIOA->PIO_ISR;   // Clear all PIO interrupt status flags.
 // NVIC_SetPriority(PIOA_IRQn, 2);
  NVIC_EnableIRQ(PIOA_IRQn);
}

void PIOA_IRQHandler(void) {
  //
	// ISR is called when "reset" button on board is pressed
	//	
  uint32_t interrupts = PIOA->PIO_ISR;  
  if((interrupts >> 9) & 1){//CTS
      uart_software_flow_control();    
  }
  
  if ((interrupts >> _BIT_BOOT_MODE_PIN) & 1) { // Check if interrupt was caused by I/O change on "reset" pin. This also clears interrupt status flags		
    isr_evt_set(isr_flags, isr_notify);
  }
  
}
