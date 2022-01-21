/**
 * @file    timer_wait.c
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

#include "timer_wait.h"

#include "fsl_device_registers.h"
#include "fsl_tpm.h"


static volatile bool tpmIsrFlag = false;
static uint32_t tpm_source_clock;


void TPM0_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    TPM_ClearStatusFlags(TPM0, kTPM_TimeOverflowFlag);
    tpmIsrFlag = true;
    TPM_StopTimer(TPM0);
    __DSB();
}

void timer_wait_init(void)
{
    /* Select the clock source for the TPM counter as kCLOCK_McgIrc48MClk */
    CLOCK_SetTpmClock(1U);

    tpm_config_t tpmInfo;
    TPM_GetDefaultConfig(&tpmInfo);
    /* TPM clock divide by TPM_PRESCALER */
    tpmInfo.prescale = kTPM_Prescale_Divide_4;
    /* Initialize TPM module */
    TPM_Init(TPM0, &tpmInfo);

    TPM_EnableInterrupts(TPM0, kTPM_TimeOverflowInterruptEnable);
    EnableIRQ(TPM0_IRQn);

    tpm_source_clock = (CLOCK_GetFreq(kCLOCK_McgIrc48MClk) / 4);
    tpmIsrFlag = false;
}

void timer_wait_set_period(uint32_t microsec)
{
    TPM_SetTimerPeriod(TPM0, USEC_TO_COUNT(microsec, tpm_source_clock));
}

void timer_wait(void)
{
    TPM_StartTimer(TPM0, kTPM_SystemClock);
    while (false == tpmIsrFlag);
    tpmIsrFlag = false;
}
