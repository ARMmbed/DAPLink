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

#include "validate_user_application.h"
#include "flash_erase_read_write.h"
#include "gpio.h"

#define CHECK_RANGE(i, min, max) 	(i < min) || (i > max) ? 1 : 0
#define CHECK_VALUE(i, val) 			(i == val) ? 1 : 0

//The verification algorithm will simply check that vectors 0-6, 11-12, 
//	and 14-15 all point to a valid memory region and are non-zero.
int validate_user_application(int app_address)
{
		uint32_t i = 0;
		uint8_t *mem = 0;
		// Initial_SP
		// Reset_Handler
		// NMI_Handler
		// HardFault_Handler
		// MemManage_Handler (Reserved on CM0+)
		// BusFault_Handler (Reserved on CM0+)
		// UsageFault_Handler (Reserved on CM0+)
		for( ; i<7; ++i)
		{	
				// check for a valid flash address.
				if (!CHECK_RANGE((uint32_t)(*mem), FLASH_VALID_LO, FLASH_VALID_HI) && (!_read_memory(app_address+1, mem, 1))) {
						return 0;
				}
		}
		// RESERVED
		for( ; i<11; ++i)
		{
				// test for a 0
				if (!CHECK_VALUE((uint32_t)(*mem), 0) && (!_read_memory(app_address+1, mem, 1))) {
						return 0;
				}
		}
		// SVC_Handler
		// DebugMon_Handler (Reserved on CM0+)
		for( ; i<13; ++i)
		{	
				// check for a valid flash address
				if (!CHECK_RANGE((uint32_t)(*mem), FLASH_VALID_LO, FLASH_VALID_HI) && (!_read_memory(app_address+1, mem, 1))) {
						return 0;
				}
		}
		// RESERVED
		for( ; i<14; ++i)
		{
				// test for a 0
				if (!CHECK_VALUE((uint32_t)(*mem), 0) && (!_read_memory(app_address+1, mem, 1))) {
						return 0;
				}
		}
		// PendSV_Handler
		// SysTick_Handler
		for( ; i<16; ++i)
		{	
				// check for a valid flash address
				if (!CHECK_RANGE((uint32_t)(*mem), FLASH_VALID_LO, FLASH_VALID_HI) && (!_read_memory(app_address+1, mem, 1))) {
						return 0;
				}
		}
		return 1;
}
