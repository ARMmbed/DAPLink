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
#ifndef TARGET_FLASH_H
#define TARGET_FLASH_H

#include <stdbool.h>
#include "stdint.h"
#include "flash_blob.h"
#include "compiler.h"
#include "error.h"

#ifdef __cplusplus
  extern "C" {
#endif

//! @brief Verify that security will not be enabled.
error_t check_security_bits(uint32_t flashAddr, uint8_t *data);

//! @name Flash programming operations
//@{
error_t target_flash_init(void);
error_t target_flash_uninit(void);
error_t target_flash_program_page(uint32_t adr, const uint8_t * buf, uint32_t size);
error_t target_flash_erase_sector(uint32_t sector);
error_t target_flash_erase_chip(void);
uint32_t target_flash_program_page_min_size(uint32_t addr);
uint32_t target_flash_erase_sector_size(uint32_t addr);
//@}

#ifdef __cplusplus
  }
#endif

#endif // TARGET_FLASH_H
