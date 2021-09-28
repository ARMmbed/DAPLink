/**
 * @file    pwr_mon.c
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

#include "pwr_mon.h"

void pwr_mon_init()
{
    // TODO
}

power_source_t pwr_mon_get_power_source()
{
    // TODO
    return PWR_SOURCE_NONE;
}

uint32_t pwr_mon_get_vin_mv()
{
    // TODO
    return 0;
}

uint32_t pwr_mon_get_vbat_mv()
{
    // TODO
    return 0;
}
