/**
 * @file    settings.h
 * @brief   Access to read user configuration settings
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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
#ifndef SETTINGS_H
#define SETTINGS_H

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ASSERT_SOURCE_NONE = 0,
    ASSERT_SOURCE_BL = 1,
    ASSERT_SOURCE_APP = 2
} assert_source_t;

void config_init(void);

// Get/set settings residing in flash
void config_set_auto_rst(bool on);
void config_set_automation_allowed(bool on);
bool config_get_auto_rst(void);
bool config_get_automation_allowed(void);

// Get/set settings residing in shared ram
void config_ram_set_hold_in_bl(bool hold);
void config_ram_set_assert(const char *file, uint16_t line);
void config_ram_clear_assert(void);
bool config_ram_get_hold_in_bl(void);
bool config_ram_get_initial_hold_in_bl(void);
bool config_ram_get_assert(char *buf, uint16_t buf_size, uint16_t *line, assert_source_t *source);

// Private - should only be called from settings.c
void config_rom_init(void);

#ifdef __cplusplus
}
#endif

#endif
