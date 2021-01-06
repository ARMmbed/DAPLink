/**
 * @file    adc.h
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

#ifndef ADC_H
#define ADC_H

#include "IO_Config.h"

#ifdef __cplusplus
extern "C" {
#endif

void adc_init(void);
void adc_init_pins(void);
uint32_t adc_read_channel(uint32_t channelGroup, uint32_t channelNumber, uint32_t channelMux);

#ifdef __cplusplus
}
#endif

#endif
