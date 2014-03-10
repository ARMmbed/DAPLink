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
 
// common API for MSC to work from (CMSIS-DAP or BOOTLOADER)
#include "flash_erase_read_write.h" 

// Specific resources for device FLASH read/write


int flash_init(uint32_t clk)
{
    return 0;
}

int flash_uninit(void)
{
    return 0;
}

int flash_erase_chip(void)
{
    return 0;
}

int flash_erase_sector(uint32_t adr)
{
    return 0;
}

int flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size)
{
    return 0;
}
uint32_t read_memory(uint32_t adr, uint8_t *buf, uint32_t size)
{
	char *start_address = (char *)adr;
    while(size--) {
        *buf++ = *(char *)adr++;
    }
    return adr - *(char *)start_address;
}
