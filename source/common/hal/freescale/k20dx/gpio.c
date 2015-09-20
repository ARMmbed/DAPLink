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

#if defined(BOARD_FRDM_K64F)
    #define SDA_USB_P5V_SENSE (5)   // PTD5
#elif defined(BOARD_FRDM_KL25Z)
    #define SDA_USB_P5V_SENSE (7)   // PTD7
#endif

#define PWM_FREQUENCY    ((float)(32768.0))
#define FTM0_MOD_VALUE    (int)((float)(SystemCoreClock)/PWM_FREQUENCY)

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
    
#if defined(INTERFACE_GEN_32KHZ)
    #if (PIN_CLK_32K_BIT != 6) || (PIN_CLK_32K_PORT != PORTD)
        #error "Clock pin changed - FTM code below must be updated"
    #endif

    // we use PTD6 to generate 32kHz clk
    // ftm0_ch6 (alternate function 4)
    PIN_CLK_32K_PORT->PCR[PIN_CLK_32K_BIT] = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;
    // enable clk for ftm0
    SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;

    // configure PWM to generate a 32kHz clock used
    // by the RTC module of the target.
    // Choose EDGE-Aligned PWM:  selected when QUADEN=0, DECAPEN=0, COMBINE=0, CPWMS=0, and MSnB=1  (page 964)

    //disable write protection
    FTM0->MODE |= FTM_MODE_WPDIS_MASK;

    //FTMEN is bit 0, need to set to zero so DECAPEN can be set to 0
    FTM0->MODE &= ~FTM_MODE_FTMEN_MASK;

    //QUADEN = 0; DECAPEN = 0; COMBINE = 0; CPWMS = 0; MSnB = 1
    FTM0->QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;
    FTM0->COMBINE &= ~FTM_COMBINE_DECAPEN3_MASK;
    FTM0->COMBINE &= ~FTM_COMBINE_COMBINE3_MASK;
    FTM0->SC &= ~FTM_SC_CPWMS_MASK;
    FTM0->CONTROLS[6].CnSC |= FTM_CnSC_MSB_MASK;

    // ELSnB:ELSnA = 1:0
    FTM0->CONTROLS[6].CnSC |= FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[6].CnSC &= ~FTM_CnSC_ELSA_MASK;

    // set CNT, MOD (period) and CNTIN
    FTM0->CNT = 0x0;
    FTM0->MOD = FTM0_MOD_VALUE;
    FTM0->CNTIN = 0;

    // set pulsewidth to period/2
    FTM0->CONTROLS[6].CnV = FTM0_MOD_VALUE/2;

    // select clock (system core clock)
    FTM0->SC = FTM_SC_PS(0) | FTM_SC_CLKS(1);

    //enable write protection
    FTM0->MODE &= ~FTM_MODE_WPDIS_MASK;
	
#elif defined(INTERFACE_POWER_EN)
    // POWER_EN (PTD5) and VTRG_FAULT_B (PTD7)
    //	VTRG_FAULT_B not currently implemented. Just power the target ;)
	
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
