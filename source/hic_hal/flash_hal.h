/**
 * @file    flash_hal.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2020, ARM Limited, All Rights Reserved
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

#ifndef FLASH_HAL_H
#define FLASH_HAL_H

#include <stdbool.h>
#include "FlashPrg.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t  flash_program_page(uint32_t adr, uint32_t sz, uint8_t *buf);
uint32_t  flash_erase_sector(uint32_t addr);

/*!
 * @brief Test whether the specified flash address range can be read from.
 *
 * The default implementation returns true if the given address range is within the bounds
 * of the HIC's internal flash memory (defined by DAPLINK_ROM_START and DAPLINK_ROM_SIZE).
 *
 * @retval true All pages within the specified range are readble.
 * @retval false At least one page within the specified range cannot be read.
 */
bool flash_is_readable(uint32_t addr, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif
