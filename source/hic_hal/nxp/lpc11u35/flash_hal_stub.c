/**
 * @file    flash_hal_stub.c
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

#include "flash_hal.h"

uint32_t Init(uint32_t adr, uint32_t clk, uint32_t fnc)
{
    return (1); // IAP not supported
}

uint32_t UnInit(uint32_t fnc)
{
    return (1); // IAP not supported
}

uint32_t EraseChip(void)
{
    return (1); // IAP not supported
}

uint32_t EraseSector(uint32_t adr)
{
    return (1); // IAP not supported
}

uint32_t ProgramPage(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    return (1); // IAP not supported
}
