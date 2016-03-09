/**
 * @file    validation.c
 * @brief   Implementation of validation.h
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

#include "validation.h"
#include "string.h"
#include "target_config.h"

static inline uint32_t test_range(const uint32_t test, const uint32_t min, const uint32_t max)
{
    return ((test < min) || (test > max)) ? 0 : 1;
}

uint8_t validate_bin_nvic(const uint8_t *buf)
{
    // test for known required NVIC entries
    //  00 is stack pointer (RAM address)
    //  04 is Reset vector  (FLASH address)
    //  08 NMI_Handler      (FLASH address)
    //  12 HardFault_Handler(FLASH address)
    uint32_t i = 4, nvic_val = 0;
    // test the initial SP value
    memcpy(&nvic_val, buf + 0, sizeof(nvic_val));

    if (0 == test_range(nvic_val, target_device.ram_start, target_device.ram_end)) {
        return 0;
    }

    // Reset_Handler
    // NMI_Handler
    // HardFault_Handler
    for (; i <= 12; i += 4) {
        memcpy(&nvic_val, buf + i, sizeof(nvic_val));

        if (0 == test_range(nvic_val, target_device.flash_start, target_device.flash_end)) {
            return 0;
        }
    }

    return 1;
}

uint8_t validate_hexfile(const uint8_t *buf)
{
    // look here for known hex records
    // add hex identifier b[0] == ':' && b[8] == {'0', '2', '3', '4', '5'}
    return ((buf[0] == ':') && ((buf[8] == '0') || (buf[8] == '2') || (buf[8] == '3') || (buf[8] == '4') || (buf[8] == '5'))) ? 1 : 0;
}
