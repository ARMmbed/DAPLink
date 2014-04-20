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
#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

#define START_APP_ADDRESS (0x5000)

#if defined(TARGET_LPC11U35)
#define SECTOR_SIZE       (0x1000)
#define NB_SECTOR         (15)

#elif defined(TARGET_MK20DX)
#define SECTOR_SIZE       (0x400)
#define NB_SECTOR         (128)
#endif

#define END_FLASH         (NB_SECTOR*SECTOR_SIZE)

int  flash_hal_init         (uint32_t clk);
int  flash_hal_uninit       (uint32_t fnc);
int  flash_hal_erase_sector (uint32_t adr);
int  flash_hal_program_page (uint32_t adr, uint32_t sz, unsigned char *buf);

int  __swi(3) flash_program_page_svc (uint32_t adr, uint32_t sz, uint8_t *buf);
int  __swi(2) flash_erase_sector_svc (uint32_t addr);

#endif
