/**
 * @file    gpio_extra.h
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

#ifndef GPIO_EXTRA_H
#define GPIO_EXTRA_H

#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

void gpio_enable_hid_led(void);
void gpio_disable_hid_led(void);

void gpio_init_combined_int(void);
void gpio_disable_combined_int(void);
void gpio_assert_combined_int(void);

#ifdef __cplusplus
}
#endif

#endif
