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

//#include "target_flash.h"	//move to flashing files
//#include "target_reset.h"	//move to flashing files
//#include "DAP_config.h"		//move to flashing files
//#include "dap.h"					//move to flashing files
//#include "semihost.h"			//move to flashing files
//#include "swd_host.h"			//move to flashing files


// This file is common to bootloading and to CMSIS-DAP (at ./shared/common/inc)

// each Device should implement a C file with these modules that are specific (Bootloader)
// CMSIS-DAP will have a common file with DAP commands
int  _flash_init(uint32_t clk);
int  _flash_uninit(void);
int  _flash_erase_chip(void);
int  _flash_erase_sector(uint32_t adr);
int  _flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size);
uint32_t _read_memory(uint32_t address, uint8_t *data, uint32_t size);


// pulled over from original bootloader. Not sure if needed because with the copy of CMSIS-DAP 
//  code structure
int  __swi(3) flash_program_page_svc(uint32_t adr, uint8_t * buf, uint32_t size);
int  __swi(2) flash_erase_sector_svc(uint32_t adr);

#endif

