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
#include "target_reset.h"
#include "swd_host.h"
#include "DAP_config.h"
#include "RTL.h"

void target_before_init_debug(void) {
    return;
}

uint8_t target_unlock_sequence(void) {
    return 1;
}

uint8_t target_set_state(TARGET_RESET_STATE state) {
    uint8_t retval = swd_set_target_state(state);
    
    if (RESET_RUN == state) {
        const uint32_t POWER_RESET_ADDR = (0x40000000 + 0x544);
        const uint32_t POWER_RESET_DATA = 0x01;
                
        swd_read_memory(POWER_RESET_ADDR, (uint8_t *)&POWER_RESET_DATA, 4);
        
        PIN_SWDIO_OUT_ENABLE();
        PIN_SWDIO_OUT(0);
        PIN_SWCLK_TCK_CLR();
        
        os_dly_wait(1);
        
        PIN_SWDIO_OUT(1);
    }
    
    return retval;
}
