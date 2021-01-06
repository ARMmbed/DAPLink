/**
 * @file    power.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

#ifndef POWER_H_
#define POWER_H_

#include "fsl_common.h"
#include "fsl_smc.h"

/* Power mode definition used in application. */
typedef enum _app_power_mode
{
    kAPP_PowerModeMin = 0,
    kAPP_PowerModeRun,   /* Normal RUN mode */
    kAPP_PowerModeWait,  /* WAIT mode. */
    kAPP_PowerModeStop,  /* STOP mode. */
    kAPP_PowerModeVlpr,  /* VLPR mode. */
    kAPP_PowerModeVlpw,  /* VLPW mode. */
    kAPP_PowerModeVlps,  /* VLPS mode. */
    kAPP_PowerModeLls,   /* LLS mode. */
    kAPP_PowerModeVlls0, /* VLLS0 mode. */
    kAPP_PowerModeVlls1, /* VLLS1 mode. */
    kAPP_PowerModeVlls3, /* VLLS3 mode. */
    kAPP_PowerModeMax
} app_power_mode_t;

void power_init(void);

/* Lowest power mode available in KL27*/
void power_enter_VLLS0(void);

/* Lowest power mode that allows I2C operation with address match wakeup */
void power_enter_VLPS(void);

#endif /* POWER_H_ */
