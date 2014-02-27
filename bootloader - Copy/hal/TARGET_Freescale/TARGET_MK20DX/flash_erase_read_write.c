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
 
#include "flash_erase_read_write.h"
#include "flash.h"
#include "retarget.h"

uint8_t _flash_init(uint32_t clk)
{
	return flash_init(clk);
}

uint8_t _flash_uninit(void)
{
	return flash_uninit(0);
}

uint8_t _flash_erase_chip(void)
{
	int i = START_APP_ADDRESS;
    for(; i < END_FLASH; i += SECTOR_SIZE) {
        dbg_message("break %x", i);
        flash_erase_sector(i);
    }
    return 0;
}

uint8_t _flash_erase_sector(uint32_t adr)
{
	return flash_erase_sector_svc(adr);
}

uint8_t _flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size)
{
	return flash_program_page_svc(adr, size, buf);
}

uint32_t _read_memory(uint32_t address, uint8_t *data, uint32_t size)
{
	char *start_address = (char *)address;
    while(size--) {
        *data++ = *(char *)address++;
    }
    return address - *(char *)start_address;
}
