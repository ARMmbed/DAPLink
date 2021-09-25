/**
 * @file    gpio.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

#include "MK22F51212.h"
#include "RTL.h"
#include "DAP_config.h"
#include "gpio.h"
#include "daplink.h"

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
    // Enable hardfault on unaligned access for the interface only.
    // If this is done in the bootloader than then it might (will) break
    // older application firmware or firmware from 3rd party vendors.
#if  defined(DAPLINK_IF)
    SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif
    // enable clock to ports
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTB_MASK;

    #if defined(INTERFACE_K22F)
        SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    #endif

    // configure pin as GPIO
    LED_CONNECTED_PORT->PCR[LED_CONNECTED_BIT] = PORT_PCR_MUX(1);
    // led off - enable output
    LED_CONNECTED_GPIO->PDOR = 1UL << LED_CONNECTED_BIT;
    LED_CONNECTED_GPIO->PDDR = 1UL << LED_CONNECTED_BIT;
    // led on
    LED_CONNECTED_GPIO->PCOR  |= 1UL << LED_CONNECTED_BIT;
        #if defined(INTERFACE_K22F)
        //TODO: Use ADC for reading nRESET due to issue with 'blank' target parts
        PIN_nRESET_PORT->PCR[PIN_nRESET_BIT] = PORT_PCR_MUX(0);

        //Init ADC 8-bit single ended on channel 13
        SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;

        ADC0->CFG1 = (  ADC_CFG1_ADIV(2)      //Input clock division (1, 2, 4, 8)
                      | ADC_CFG1_MODE(0)      //Resolution (8, 12, 10-bit)
                      | ADC_CFG1_ADLSMP_MASK  //Sample time (Short, Long)
                      | ADC_CFG1_ADICLK(0)    //Input clock source (Bus, Bus/2, ALTCLK, ADACK)
                     );

        uint16_t calVar;

        ADC0->SC2 &= ADC_SC2_ADTRG_MASK;

        ADC0->SC3 &= (~ADC_SC3_ADCO_MASK & ~ADC_SC3_AVGS_MASK);

        ADC0->SC3 |= ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3);

        ADC0->SC3 |= ADC_SC3_CAL_MASK;

        while((ADC0->SC1[0] & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);

        if((ADC0->SC3 & ADC_SC3_CALF_MASK) == ADC_SC3_CALF_MASK)
        {
            return;
        }

        /* Plus side calibration. */
        calVar = 0x0000U;

        calVar  = ADC0->CLP0;
        calVar += ADC0->CLP1;
        calVar += ADC0->CLP2;
        calVar += ADC0->CLP3;
        calVar += ADC0->CLP4;
        calVar += ADC0->CLPS;

        calVar = (calVar >> 1U);
        calVar |= 0x8000U;

        ADC0->PG = ADC_PG_PG(calVar);

        ADC0->SC3 &= ~ADC_SC3_CAL_MASK;

    #else
        // reset button configured as gpio input
        PIN_nRESET_GPIO->PDDR &= ~PIN_nRESET;
        PIN_nRESET_PORT->PCR[PIN_nRESET_BIT] = PORT_PCR_MUX(1);
    #endif

    // Keep powered off in bootloader mode
    // to prevent the target from effecting the state
    // of the reset line / reset button
    if (!daplink_is_bootloader()) {
            //TODO: Add USB board (K22F) support
            #if !defined(INTERFACE_K22F)
        // configure pin as GPIO
        PIN_POWER_EN_PORT->PCR[PIN_POWER_EN_BIT] = PORT_PCR_MUX(1);
        // force always on logic 1
        PIN_POWER_EN_GPIO->PDOR |= 1UL << PIN_POWER_EN_BIT;
        PIN_POWER_EN_GPIO->PDDR |= 1UL << PIN_POWER_EN_BIT;
            #endif
    }

    // Let the voltage rails stabilize.  This is especailly important
    // during software resets, since the target's 3.3v rail can take
    // 20-50ms to drain.  During this time the target could be driving
    // the reset pin low, causing the bootloader to think the reset
    // button is pressed.
    // Note: With optimization set to -O2 the value 1000000 delays for ~85ms
    busy_wait(1000000);
}

void gpio_set_board_power(bool powerEnabled)
{

}

void gpio_set_hid_led(gpio_led_state_t state)
{
    if (state) {
        LED_CONNECTED_GPIO->PCOR = 1UL << LED_CONNECTED_BIT; // LED on
    } else {
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
    #if defined(INTERFACE_K22F)
        ADC0->SC1[0] = (ADC_SC1_AIEN(0) |ADC_SC1_DIFF(0)| ADC_SC1_ADCH(13));

        while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));

        return ((uint8_t)ADC0->R[0] > 10) ? 1 : 0;
    #else
        return (PIN_nRESET_GPIO->PDIR & PIN_nRESET) ? 1 : 0;
    #endif
}

uint8_t gpio_get_reset_btn_no_fwrd(void)
{
    return (PIN_nRESET_GPIO->PDIR & PIN_nRESET) ? 0 : 1;
}

uint8_t gpio_get_reset_btn_fwrd(void)
{
    return 0;
}

uint8_t GPIOGetButtonState(void)
{
    return 0;
}

void target_forward_reset(bool assert_reset)
{
    // Do nothing - reset button is already tied to the target
    //              reset pin on k20dx interface hardware
}
