/**
 * @file    pwr_mon.h
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

#ifndef PWR_MON_H
#define PWR_MON_H

#include "IO_Config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PWR_SOURCE_NONE = 0,
    PWR_USB_ONLY,
    PWR_BATT_ONLY,
    PWR_USB_AND_BATT
} power_source_t;

void pwr_mon_init(void);

/**
 * Based on the volatages in VBAT_SENSE and the MCU Vin it determines what
 * power sources are active.
 * @return Active power source via power_source_t enum.
 */
power_source_t pwr_mon_get_power_source(void);

/**
 * Measures the Vin voltage.
 * @return Interface MCU Vin voltage in millivolts.
 */
uint32_t pwr_mon_get_vin_mv(void);

/**
 * Measures the VBAT_SENSE voltage.
 * @return VBAT_SENSE voltage in millivolts.
 */
uint32_t pwr_mon_get_vbat_mv(void);


#ifdef __cplusplus
}
#endif

#endif
