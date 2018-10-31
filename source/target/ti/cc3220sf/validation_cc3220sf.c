/**
 * @file    validation_cc3220sf.c
 * @brief   Validate cookie list located at the start of the flash
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2018, ARM Limited, All Rights Reserved
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

const uint32_t cookieList[]=
{
    0x5AA5A55A,
    0x000FF800,
    0xEFA3247D
};

// Override the weak validate_bin_nvic function. The weak function expects NVIC at the beginning of the flash.
// On CC3220SF, the beginning of the flash is the cookie list, which allows the boot ROM code to jump into onchip flash directly bypassing external flash.
uint8_t validate_bin_nvic(const uint8_t *buf)
{
    return (memcmp(buf, cookieList, sizeof(cookieList)) == 0);
}
