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
 
#include <MK20D5.h>
#include "gpio.h"

void time_delay_ms(uint32_t delay)
{
    SIM->SCGC5 |= SIM_SCGC5_LPTIMER_MASK;
    LPTMR0->CMR = delay;
    LPTMR0->PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PBYP_MASK;
    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;
    while (!(LPTMR0->CSR & LPTMR_CSR_TCF_MASK));
    LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;
		return;
}

void gpio_init(void) 
{
    // clock for LED and button
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTB_MASK;

    // Green LED, only one LED available
    PORTD->PCR[4] = PORT_PCR_MUX(1);
    PTD->PDOR = 1UL << 4;
    PTD->PDDR = 1UL << 4;

    // RST button
    PORTB->PCR[1] = PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
	
		// delay 2ms for pull-up enable
		time_delay_ms(2);
		return;
}

void gpio_set_dap_led(uint8_t state)
{
    if (state) {
        PTD->PCOR  |= 1UL << 4; // LED on
    } else {
        PTD->PSOR  |= 1UL << 4; // LED off
    }
		return;
}

void gpio_set_msd_led(uint8_t state)
{
    gpio_set_dap_led(state);
		return;
}

void gpio_set_cdc_led(uint8_t state)
{
    gpio_set_dap_led(state);
		return;
}

uint8_t gpio_get_pin_loader_state(void)
{
    return (PTB->PDIR & (1UL << 1));
}

void mcu_reboot(void)
{
		NVIC_SystemReset();
}
