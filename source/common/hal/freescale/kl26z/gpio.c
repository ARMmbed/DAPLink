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
#if defined(TARGET_MKL26Z)
#include "MKL26Z4.h"
#else
#error "Unknown target type"
#endif
#include "RTL.h"
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

// DAP_LED PTB18
#define PIN_DAP_LED_PORT  PORTB
#define PIN_DAP_LED_GPIO  PTB
#define PIN_DAP_LED_BIT   18
#define PIN_DAP_LED       (1<<PIN_DAP_LED_BIT)

// MSD_LED PTB19
#define PIN_MSD_LED_PORT  PORTB
#define PIN_MSD_LED_GPIO  PTB
#define PIN_MSD_LED_BIT   19
#define PIN_MSD_LED       (1<<PIN_MSD_LED_BIT)

// CDC_LED PTD1
#define PIN_CDC_LED_PORT  PORTD
#define PIN_CDC_LED_GPIO  PTD
#define PIN_CDC_LED_BIT   1
#define PIN_CDC_LED       (1<<PIN_CDC_LED_BIT)

// SW RESET BUTTON PTD3
#define PIN_SW_RESET_PORT PORTD
#define PIN_SW_RESET_GPIO PTD
#define PIN_SW_RESET_BIT  3
#define PIN_SW_RESET      (1<<PIN_SW_RESET_BIT)
#define PIN_SW_RESET_PORT_IRQn PORTD_IRQn
#define PIN_SW_RESET_PORT_IRQHandler PORTD_IRQHandler

#define PWM_FREQUENCY    ((float)(32768.0))
#define FTM0_MOD_VALUE    (int)((float)(SystemCoreClock)/PWM_FREQUENCY)

void gpio_init(void)
{
    // enable clock to ports
    if (PIN_DAP_LED_PORT == PORTA || PIN_MSD_LED_PORT == PORTA || PIN_CDC_LED_PORT == PORTA) {
        SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    }
    if (PIN_DAP_LED_PORT == PORTB || PIN_MSD_LED_PORT == PORTB || PIN_CDC_LED_PORT == PORTB) {
        SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    }
    if (PIN_DAP_LED_PORT == PORTC || PIN_MSD_LED_PORT == PORTC || PIN_CDC_LED_PORT == PORTC) {
        SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    }
    if (PIN_DAP_LED_PORT == PORTD || PIN_MSD_LED_PORT == PORTD || PIN_CDC_LED_PORT == PORTD) {
        SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    }
    if (PIN_DAP_LED_PORT == PORTE || PIN_MSD_LED_PORT == PORTE || PIN_CDC_LED_PORT == PORTE) {
        SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    }
    // configure pin as GPIO
    PIN_DAP_LED_PORT->PCR[PIN_DAP_LED_BIT] = PORT_PCR_MUX(1);
    PIN_MSD_LED_PORT->PCR[PIN_MSD_LED_BIT] = PORT_PCR_MUX(1);
    PIN_CDC_LED_PORT->PCR[PIN_CDC_LED_BIT] = PORT_PCR_MUX(1);

    // led off - enable output
    PIN_DAP_LED_GPIO->PSOR = PIN_DAP_LED;
    PIN_MSD_LED_GPIO->PSOR = PIN_MSD_LED;
    PIN_CDC_LED_GPIO->PSOR = PIN_CDC_LED;

    PIN_DAP_LED_GPIO->PDDR |= PIN_DAP_LED;
    PIN_MSD_LED_GPIO->PDDR |= PIN_MSD_LED;
    PIN_CDC_LED_GPIO->PDDR |= PIN_CDC_LED;

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
    if (!state)
    {
        PIN_DAP_LED_GPIO->PCOR = PIN_DAP_LED; // LED on
    }
    else
    {
        PIN_DAP_LED_GPIO->PSOR = PIN_DAP_LED; // LED off
    }
}

void gpio_set_cdc_led(uint8_t state)
{
    if (!state) {
        PIN_CDC_LED_GPIO->PCOR = PIN_CDC_LED; // LED on
    } else {
        PIN_CDC_LED_GPIO->PSOR = PIN_CDC_LED; // LED off
    }
}

void gpio_set_msd_led(uint8_t state)
{
    if (!state) {
        PIN_MSD_LED_GPIO->PCOR = PIN_MSD_LED; // LED on
    } else {
        PIN_MSD_LED_GPIO->PSOR = PIN_MSD_LED; // LED off
    }
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

    // enable clock PORT
    if (PIN_SW_RESET_PORT == PORTA) {
        SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    }
    if (PIN_SW_RESET_PORT == PORTD) {
        SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    }

    PIN_SW_RESET_PORT->PCR[PIN_SW_RESET_BIT] |= PORT_PCR_ISF_MASK;
    //sw2 - interrupt on falling edge
    PIN_SW_RESET_PORT->PCR[PIN_SW_RESET_BIT] = PORT_PCR_PS_MASK /* Pull-up */
                                              |PORT_PCR_PE_MASK /* Pull enable */
                                              |PORT_PCR_PFE_MASK
                                              |PORT_PCR_IRQC(10) /* IRQ_FALLING_EDGE */
                                              |PORT_PCR_MUX(1); /* GPIO */

    NVIC_ClearPendingIRQ(PIN_SW_RESET_PORT_IRQn);
    NVIC_EnableIRQ(PIN_SW_RESET_PORT_IRQn);
}

void PIN_SW_RESET_PORT_IRQHandler(void)
{
    if(PIN_SW_RESET_PORT->ISFR & PIN_SW_RESET) {
        PIN_SW_RESET_PORT->PCR[PIN_SW_RESET_BIT] |= PORT_PCR_ISF_MASK;
        // Notify a task that the button has been pressed
        // disable interrupt
        PIN_SW_RESET_PORT->PCR[PIN_SW_RESET_BIT] = PORT_PCR_PS_MASK  /* Pull-up */
                                                  |PORT_PCR_PE_MASK  /* Pull enable */
                                                  |PORT_PCR_PFE_MASK /* IRQ Falling edge */
                                                  |PORT_PCR_IRQC(00) /* IRQ_DISABLED */
                                                  |PORT_PCR_MUX(1);  /* GPIO */

        //isr_evt_set(isr_flags, isr_notify);
    }
}

