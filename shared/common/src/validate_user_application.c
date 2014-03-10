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
#include "device_cfg.h"

#define CHECK_RANGE(i, min, max) (i < min) || (i > max) ? 0 : 1
#define CHECK_VALUE(i, val) 	 (i == val) ? 1 : 0

//The verification algorithm will simply check that vectors 0-6, 11-12, 
//	and 14-15 all point to a valid memory region and are non-zero.
int validate_user_application(int app_address)
{
    uint32_t i = 0;
    uint32_t mem[1];
    int test_val = 0;
    // Initial_SP
    for( ; i<(1*4); i+=4)
    {	
        read_memory(app_address+i, (uint8_t*)mem, 4);
        test_val = mem[0];
        // check for a valid ram address.
        if (!CHECK_RANGE(test_val, START_RAM, END_RAM)) {
            return 0;
        }
    }
    // Reset_Handler
    // NMI_Handler
    // HardFault_Handler
    // MemManage_Handler (Reserved on CM0+)
    // BusFault_Handler (Reserved on CM0+)
    // UsageFault_Handler (Reserved on CM0+)
    for( ; i<(7*4); i+=4)
    {	
        read_memory(app_address+i, (uint8_t*)mem, 4);
        test_val = mem[0];    
        // check for a valid flash address.
        if (!CHECK_RANGE(test_val, START_FLASH, END_FLASH)) {
            return 0;
        }
    }
    // RESERVED * 4
    for( ; i<(11*4); i+=4)
    {
        read_memory(app_address+i, (uint8_t*)mem, 4);
        test_val = mem[0];    
        // check for a known value.
        if (!CHECK_VALUE(test_val, 0)) {
            return 0;
        }
    }
    // SVC_Handler
    // DebugMon_Handler (Reserved on CM0+)
    for( ; i<(13*4); i+=4)
    {	
        read_memory(app_address+i, (uint8_t*)mem, 4);
        test_val = mem[0];    
        // check for a valid flash address.
        if (!CHECK_RANGE(test_val, START_FLASH, END_FLASH)) {
            return 0;
        }
    }
    // RESERVED * 1
    for( ; i<(14*4); i+=4)
    {
        read_memory(app_address+i, (uint8_t*)mem, 4);
        test_val = mem[0];    
        // check for a known value
        if (!CHECK_VALUE(test_val, 0)) {
            return 0;
        }
    }
    // PendSV_Handler
    // SysTick_Handler
    for( ; i<(16*4); i+=4)
    {	
        read_memory(app_address+i, (uint8_t*)mem, 4);
        test_val = mem[0];    
        // check for a valid flash address.
        if (!CHECK_RANGE(test_val, START_FLASH, END_FLASH)) {
            return 0;
        }
    }
    return 1;
}
