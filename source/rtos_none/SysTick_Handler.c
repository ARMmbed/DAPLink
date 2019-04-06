/**
 * @file    SysTick_Handler.c
  * @brief  No RTOS port only needs a systick handler
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
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
 
#include "Systick_Handler.h"

#if defined (INTERFACE_K20D5)
#include "MK20D5.h"
#elif defined (INTERFACE_K26F)
#include "MK26F18.h"
#elif defined (INTERFACE_KL26Z)
#include "MKL26Z4.h"
#elif defined (INTERFACE_LPC11U35)
#include "LPC11Uxx.h"
#elif defined (INTERFACE_LPC4322)
#include "LPC43xx.h"
#elif defined (INTERFACE_MAX32620)
#include "max32620.h"
#elif defined (INTERFACE_MAX32625)
#include "max32625.h"
#elif defined (INTERFACE_SAM3U2C)
#include "sam3u2c.h"
#elif defined (INTERFACE_STM32F103XB)
#include "stm32f103xb.h"
#else
#error "CMSIS core headers needed"
#endif


//SysTick Timer Configuration
#ifndef OS_CLOCK
#error "OS_CLOCK should be defined by HIC configuration"
#endif

//Set the timer tick value for selected timer.
#ifndef OS_TICK
#error "OS_TICK should be defined by RTOS configuration"
#endif

#define OS_TRV          ((uint32_t)(((double)OS_CLOCK*(double)OS_TICK)/1E6)-1)


static volatile uint32_t tick_counter = 0;
static volatile uint32_t event_flags =0;
static volatile uint32_t wait_counter = 0;

static osTimerFunc_t sysTickCb = NULL;
static uint32_t tickFreq = 0;
static osThreadFunc_t mainFuncCb = NULL;

void sysTickInit(void)
{
    tick_counter = 0;
    event_flags=0;
    wait_counter = 0;
    sysTickCb = NULL;
    tickFreq = 0;
    mainFuncCb = NULL;
    SysTick_Config(OS_TRV);
}


void SysTick_Handler(void)
{
    if (sysTickCb && tickFreq && (tick_counter % tickFreq)==0) {
        sysTickCb(NULL);
    }
    tick_counter++;
    if(wait_counter){
        --wait_counter;
    }
}

void sysTickRegCallback(osTimerFunc_t callback)
{
    sysTickCb = callback;
}

void sysTickFreqSet(uint32_t ticks)
{
    tickFreq = ticks;
}

void sysTickWait(uint32_t wait)
{
    wait_counter = wait;
    while(wait_counter);
}

void sysTickEvtSet(uint32_t flag)
{
    __disable_irq();
    event_flags |= flag;
    __enable_irq();
}

uint32_t sysTickEvtWaitOr(uint32_t flag)
{
    uint32_t return_flags=0;
    while ((flag&event_flags)==0);
    return_flags = event_flags;
    __disable_irq();
    event_flags &= ~return_flags;
    __enable_irq();
    return return_flags;
}

uint32_t sysTickTime(void)
{
    return tick_counter;
}

void sysTickRegMainFunc(osThreadFunc_t func)
{
    mainFuncCb = func;
}

void sysTickStartMain(void)
{
    if(mainFuncCb){
        mainFuncCb(NULL);
    }
}

