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
#include <RTL.h>
#include "DAP_config.h"
#include "gpio.h"

#if defined(INTERFACE_GEN_32KHZ) && defined(INTERFACE_POWER_EN)
#error "Cannot use 32kHz clock gen and power enable simultaneously."
#endif

// Pin definitions
#define CLK_32K_PIN (6)     // PTD6
#define POWER_EN_PIN (6)    // PTD6
#define VTRG_FAULT_B (7)    // PTD7

#if defined(BOARD_FRDM_K64F)
    #define SDA_USB_P5V_SENSE (5)   // PTD5
#elif defined(BOARD_FRDM_KL25Z)
    #define SDA_USB_P5V_SENSE (7)   // PTD7
#endif

//static U16 isr_flags;
//static OS_TID isr_notify;

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

#if defined(INTERFACE_GEN_32KHZ)
    // we use PTD6 to generate 32kHz clk
    // ftm0_ch6 (alternate function 4)
    PORTD->PCR[CLK_32K_PIN] = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;
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
    PORTD->PCR[POWER_EN_PIN] = PORT_PCR_MUX(1);
	// force always on logic 1
    PTD->PDOR |= 1UL << POWER_EN_PIN;
    PTD->PDDR |= 1UL << POWER_EN_PIN;

#endif
}

void gpio_set_dap_led(uint8_t state)
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

void gpio_set_cdc_led(uint8_t state)
{
    gpio_set_dap_led(state);
}

void gpio_set_msd_led(uint8_t state)
{
    gpio_set_dap_led(state);
}

uint8_t GPIOGetButtonState(void)
{
    return 0;
}

void gpio_enable_button_flag(OS_TID task, U16 flags)
{
    // When the "reset" button is pressed the ISR will set the
    // event flags "flags" for task "task"

    // Keep a local copy of task & flags
    //isr_notify=task;
    //isr_flags=flags;

    PIN_nRESET_PORT->PCR[PIN_nRESET_BIT] |= PORT_PCR_ISF_MASK;
    //sw2 - interrupt on falling edge
    PIN_nRESET_PORT->PCR[PIN_nRESET_BIT] = PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_PFE_MASK|PORT_PCR_IRQC(10)|PORT_PCR_MUX(1);

    NVIC_ClearPendingIRQ(PORTB_IRQn);
    NVIC_EnableIRQ(PORTB_IRQn);
}

void PORTB_IRQHandler(void)
{
    if (PIN_nRESET_PORT->ISFR == (1<<PIN_nRESET_BIT))
    {
        PIN_nRESET_PORT->PCR[PIN_nRESET_BIT] |= PORT_PCR_ISF_MASK;
        // Notify a task that the button has been pressed
        // disable interrupt
        PIN_nRESET_PORT->PCR[PIN_nRESET_BIT] = PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_PFE_MASK|PORT_PCR_IRQC(00)|PORT_PCR_MUX(1); // IRQ Falling edge

        //isr_evt_set(isr_flags, isr_notify);
    }
}

