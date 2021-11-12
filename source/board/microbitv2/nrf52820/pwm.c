/**
 * @file    pwm.c
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

#include "nrf.h"

#include "gpio.h"
#include "IO_Config.h"

#include "pwm.h"


static void pwm_stop(void);
static void pwm_start(void);
static void pwm_configure(void);

#define PWM_TIMER       NRF_TIMER3
#define PWM_IRQ         TIMER3_IRQn
#define PWM_CC_END      0
#define PWM_CC_MARK     1

#define PWM_GPIOTE_CH   0

#define PWM_PPI_CLR     0
#define PWM_PPI_SET     1

// Timer frequency = 16MHz / (2 ^ prescaler)
// prescaler 7 => frequency 125000Hz => 8us per tick
// 2550 ticks = 20,400us
#define PWM_PRESCALER           7
#define PWM_FREQUENCY           125000
#define PWM_USPERTICK           8
#define PWM_COUNT_MAX           2550
#define PWM_DUTY_TO_COUNT(duty) ((uint32_t)(duty) * 10)

static uint8_t pwm_duty = 0;


void pwm_init()
{
}

void pwm_init_pins()
{
    gpio_cfg_output(GPIO_REG(LED_PWR), GPIO_IDX(LED_PWR));
}

void pwm_deinit_pins()
{
    gpio_cfg(GPIO_REG(LED_PWR),
             GPIO_IDX(LED_PWR),
             NRF_GPIO_PIN_DIR_INPUT,
             NRF_GPIO_PIN_INPUT_DISCONNECT,
             NRF_GPIO_PIN_NOPULL,
             NRF_GPIO_PIN_S0S1,
             NRF_GPIO_PIN_NOSENSE);
}

void pwm_set_dutycycle(uint8_t duty)
{
    if (pwm_duty != duty) {
        if (duty == 0) {
            pwm_stop();
            gpio_clear(GPIO_REG(LED_PWR), GPIO_IDX(LED_PWR));
        } else if (duty == 255) {
            pwm_stop();
            gpio_set(GPIO_REG(LED_PWR), GPIO_IDX(LED_PWR));
        } else {
            pwm_configure();
            PWM_TIMER->CC[PWM_CC_MARK] = PWM_DUTY_TO_COUNT(duty);
            pwm_start();
        }
        pwm_duty = duty;
    }
}


static void pwm_stop()
{
    if (pwm_duty != 0 && pwm_duty != 255) {
        NVIC_DisableIRQ(PWM_IRQ);
        NRF_PPI->CHENCLR = NRF_PPI->CHENSET;
        PWM_TIMER->TASKS_STOP = 1;
        //https://infocenter.nordicsemi.com/topic/errata_nRF52820_Rev3/ERR/nRF52820/Rev3/latest/anomaly_820_78.html
        PWM_TIMER->TASKS_SHUTDOWN = 1;
        NRF_GPIOTE->CONFIG[PWM_GPIOTE_CH] = GPIOTE_CONFIG_MODE_Disabled << GPIOTE_CONFIG_MODE_Pos;
        NRF_PPI->CH[PWM_PPI_CLR].EEP = 0;
        NRF_PPI->CH[PWM_PPI_CLR].TEP = 0;
        NRF_PPI->CH[PWM_PPI_SET].EEP = 0;
        NRF_PPI->CH[PWM_PPI_SET].TEP = 0;
    }
}

static void pwm_start()
{
    if (pwm_duty == 0 || pwm_duty == 255) {
        NVIC_EnableIRQ(PWM_IRQ);
        NRF_PPI->CHENSET = (1 << PWM_PPI_CLR) | (1 << PWM_PPI_SET);
        PWM_TIMER->TASKS_CLEAR = 1;
        PWM_TIMER->TASKS_START = 1;
    }
}

static void pwm_configure()
{
    if (pwm_duty == 0 || pwm_duty == 255) {
        PWM_TIMER->BITMODE          = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos;
        PWM_TIMER->PRESCALER        = PWM_PRESCALER;
        PWM_TIMER->SHORTS           = TIMER_SHORTS_COMPARE0_CLEAR_Msk << PWM_CC_END;
        PWM_TIMER->MODE             = TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos;
        PWM_TIMER->CC[PWM_CC_MARK]  = 1;
        PWM_TIMER->CC[PWM_CC_END]   = PWM_COUNT_MAX;

        NRF_GPIOTE->CONFIG[PWM_GPIOTE_CH] = LED_PWR                         << GPIOTE_CONFIG_PSEL_Pos       |
                                            GPIOTE_CONFIG_MODE_Task         << GPIOTE_CONFIG_MODE_Pos       | 
                                            GPIOTE_CONFIG_POLARITY_Toggle   << GPIOTE_CONFIG_POLARITY_Pos   | 
                                            GPIOTE_CONFIG_OUTINIT_High << GPIOTE_CONFIG_OUTINIT_Pos;

        NRF_PPI->CH[PWM_PPI_CLR].EEP = (uint32_t)&PWM_TIMER->EVENTS_COMPARE[PWM_CC_MARK];
        NRF_PPI->CH[PWM_PPI_CLR].TEP = (uint32_t)&NRF_GPIOTE->TASKS_CLR[PWM_GPIOTE_CH];

        NRF_PPI->CH[PWM_PPI_SET].EEP = (uint32_t)&PWM_TIMER->EVENTS_COMPARE[PWM_CC_END];
        NRF_PPI->CH[PWM_PPI_SET].TEP = (uint32_t)&NRF_GPIOTE->TASKS_SET[PWM_GPIOTE_CH];
    }
}
