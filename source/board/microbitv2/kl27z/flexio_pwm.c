/**
 * @file    flexio_pwm.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2020 NXP
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

#include "fsl_flexio.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pwm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_TIME_DELAY_FOR_DUTY_CYCLE_UPDATE 100000U
#define DEMO_FLEXIO_BASEADDR FLEXIO
#define DEMO_FLEXIO_OUTPUTPIN 7U /* Select FXIO_D7 as PWM output */
#define DEMO_FLEXIO_TIMER_CH 0U  /* Flexio timer0 used */

#define DEMO_FLEXIO_CLOCK_FREQUENCY CLOCK_GetFreq(kCLOCK_McgInternalRefClk)
#define DEMO_FLEXIO_FREQUENCY 31250U
#define FLEXIO_MAX_FREQUENCY (DEMO_FLEXIO_CLOCK_FREQUENCY / 2U)
#define FLEXIO_MIN_FREQUENCY (DEMO_FLEXIO_CLOCK_FREQUENCY / 256U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/
void pwm_init(void)
{
    flexio_config_t fxioUserConfig;
    
    /* Enable the LIRC clock in the MCG */
    MCG->C1 |= MCG_C1_IRCLKEN_MASK | MCG_C1_IREFSTEN_MASK;
    
    /* Select the clock source for the FlexIO
        00 - Clock disabled
        01 - MCGPCLK clock
        10 - OSCERCLK clock
        11 - MCGIRCLK clock
    */
    CLOCK_SetFlexio0Clock(3U);

    /* Init flexio, use default configure
     * Enable in Doze and fast access mode
     * Enable in debug mode
     */
    FLEXIO_GetDefaultConfig(&fxioUserConfig);
    fxioUserConfig.enableInDoze = true;
    FLEXIO_Init(DEMO_FLEXIO_BASEADDR, &fxioUserConfig);
}

void pwm_set_dutycycle(uint8_t duty)
{
    uint32_t freq_Hz = DEMO_FLEXIO_FREQUENCY;
    
    assert((freq_Hz <= FLEXIO_MAX_FREQUENCY) && (freq_Hz >= FLEXIO_MIN_FREQUENCY));

    uint32_t lowerValue = 0; /* Number of clock cycles in high logic state in one period */
    uint32_t upperValue = 0; /* Number of clock cycles in low logic state in one period */
    uint32_t sum        = 0; /* Number of clock cycles in one period */
    flexio_timer_config_t fxioTimerConfig;

    /* Check parameter */
    if (duty >= 255)
    {
        duty = 254;
        PIN_RED_LED_GPIO->PDDR |= PIN_RED_LED;
        PIN_RED_LED_GPIO->PSOR = PIN_RED_LED;
        PORT_SetPinMux(PIN_RED_LED_PORT, PIN_RED_LED_BIT, kPORT_MuxAsGpio);
    }
    else if (duty == 0)
    {
        duty = 1;
        PIN_RED_LED_GPIO->PDDR |= PIN_RED_LED;
        PIN_RED_LED_GPIO->PCOR = PIN_RED_LED;
        PORT_SetPinMux(PIN_RED_LED_PORT, PIN_RED_LED_BIT, kPORT_MuxAsGpio);
    }
    else {
        pwm_init_pins();
    }

    /* Configure the timer DEMO_FLEXIO_TIMER_CH for generating PWM */
    fxioTimerConfig.triggerSelect   = FLEXIO_TIMER_TRIGGER_SEL_SHIFTnSTAT(0U);
    fxioTimerConfig.triggerSource   = kFLEXIO_TimerTriggerSourceInternal;
    fxioTimerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
    fxioTimerConfig.pinConfig       = kFLEXIO_PinConfigOutput;
    fxioTimerConfig.pinPolarity     = kFLEXIO_PinActiveHigh;
    fxioTimerConfig.pinSelect       = DEMO_FLEXIO_OUTPUTPIN; /* Set pwm output */
    fxioTimerConfig.timerMode       = kFLEXIO_TimerModeDisabled;
    fxioTimerConfig.timerOutput     = kFLEXIO_TimerOutputOneNotAffectedByReset;
    fxioTimerConfig.timerDecrement  = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    fxioTimerConfig.timerDisable    = kFLEXIO_TimerDisableNever;
    fxioTimerConfig.timerEnable     = kFLEXIO_TimerEnabledAlways;
    fxioTimerConfig.timerReset      = kFLEXIO_TimerResetNever;
    fxioTimerConfig.timerStart      = kFLEXIO_TimerStartBitDisabled;
    fxioTimerConfig.timerStop       = kFLEXIO_TimerStopBitDisabled;

    /* Calculate timer lower and upper values of TIMCMP */
    /* Calculate the nearest integer value for sum, using formula round(x) = (2 * floor(x) + 1) / 2 */
    /* sum = DEMO_FLEXIO_CLOCK_FREQUENCY / freq_H */
    sum = (DEMO_FLEXIO_CLOCK_FREQUENCY * 2 / freq_Hz + 1) / 2;
    /* Calculate the nearest integer value for lowerValue, the high period of the pwm output */
    /* lowerValue = sum * duty / 255 */
    lowerValue = (sum * duty / 127 + 1) / 2;
    /* Calculate upper value, the low period of the pwm output */
    upperValue                   = sum - lowerValue;
    fxioTimerConfig.timerCompare = ((upperValue - 1) << 8U) | (lowerValue - 1);

    FLEXIO_SetTimerConfig(DEMO_FLEXIO_BASEADDR, DEMO_FLEXIO_TIMER_CH, &fxioTimerConfig);
    
    /* Set Timer mode to kFLEXIO_TimerModeDual8BitPWM to start timer */
    DEMO_FLEXIO_BASEADDR->TIMCTL[DEMO_FLEXIO_TIMER_CH] |= FLEXIO_TIMCTL_TIMOD(kFLEXIO_TimerModeDual8BitPWM);
}

void pwm_init_pins(void)
{
    PORT_SetPinMux(PIN_RED_LED_PORT, PIN_RED_LED_BIT, kPORT_MuxAlt6);
}

void pwm_deinit_pins(void)
{
    PORT_SetPinMux(PIN_RED_LED_PORT, PIN_RED_LED_BIT, kPORT_PinDisabledOrAnalog);
}
