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
#ifndef CONFIG_SETTINGS_H
#define CONFIG_SETTINGS_H

#include<stdint.h>
#include<stdbool.h>

void config_init(void);

// Get/set settings residing in flash
void config_set_auto_rst(bool on);
bool config_get_auto_rst(void);

// Get/set settings residing in shared ram
void config_ram_set_hold_in_bl(bool hold);
void config_ram_set_assert(const char * file, uint16_t line);
void config_ram_clear_assert(void);
bool config_ram_get_hold_in_bl(void);
bool config_ram_get_initial_hold_in_bl(void);
bool config_ram_get_assert(char * buf, uint16_t buf_size, uint16_t * line);

#endif
