/**
 * @file    target_reset.h
 * @brief   
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

#ifndef TARGET_RESET_H
#define TARGET_RESET_H

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RESET_HOLD,              // Hold target in reset
    RESET_PROGRAM,           // Reset target and setup for flash programming.
    RESET_RUN,               // Reset target and run normally
    NO_DEBUG,                // Disable debug on running target
    DEBUG                    // Enable debug on running target
} TARGET_RESET_STATE;

void target_before_init_debug(void);
uint8_t target_unlock_sequence(void);
uint8_t target_set_state(TARGET_RESET_STATE state);
uint8_t security_bits_set(uint32_t addr, uint8_t *data, uint32_t size);
void target_forward_reset(bool assert_reset);

#ifdef __cplusplus
}
#endif

#endif
