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
#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// THIS file is common between bootloader and CMSIS-DAP

// CMSIS-DAP each target needs a name_gpio.c file with #ifdef for the target type
//  BOOTLOADER has a project file and one target so no problems
void gpio_init(void);
void gpio_set_dap_led(uint8_t state);
void gpio_set_cdc_led(uint8_t state);
void gpio_set_msd_led(uint8_t state);
uint8_t gpio_get_rst_pin_state(void);

#endif
