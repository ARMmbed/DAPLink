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
 
#ifndef FLASH_ERASE_READ_WRITE_H
#define FLASH_ERASE_READ_WRITE_H

#include <stdint.h>

/**
 Initialize the flash programming algorithm
 @param  clk - parameter should be SystemCoreClock or the equivelant
 @return 1 on success and 0 otherwise
*/
uint32_t flash_init(uint32_t clk);

/**
 Un-initialize the flash
 @param  none
 @return 1 on success and 0 otherwise
*/
uint32_t flash_uninit(void);

/**
 Erase the entire contents of flash memory
 @param  none
 @return 1 on success and 0 otherwise
*/
uint32_t flash_erase_chip(void);

/**
 Erase a sector in flash memory
 @param  num - The sector number to erase
 @return 1 on success and 0 otherwise
*/
uint32_t flash_erase_sector(uint32_t num);

/**
 Program a page into flash memory
 @param  adr - The start address of a page in memory
 @param  buf - The contents to be flashed into memory
 @param  size - The amount of data in buf
 @return 1 on success and 0 otherwise
*/
uint32_t flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size);

/**
 Read the contents of memory
 @param  adr - The start address of a page in memory
 @param  buf - A buffer to read memory into. Must be as large or larger than size
 @param  size - The amount of data to read into buf
 @return The amount of data written into buf
*/
uint32_t read_memory(uint32_t adr, uint8_t *buf, uint32_t size);

// These are called in in software interrupt mode. Was done this way for the bootloader
// and just moved over. Not sure if this is entirly necessary
uint32_t __swi(2) erase_sector_svc(uint32_t num);
uint32_t __swi(3) program_page_svc(uint32_t adr, uint8_t * buf, uint32_t size);

#endif

