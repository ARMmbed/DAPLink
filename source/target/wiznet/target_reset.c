/**
 * @file    target_reset.c
 * @brief   Target reset for the W7500 
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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
 
#include <RTL.h>
#include "target_reset.h"
#include "swd_host.h"

__attribute__((weak)) void target_set_state_by_board(TARGET_RESET_STATE state)
{
    if( state == RESET_PROGRAM )
    {
        do
        {
            swd_set_target_reset(1);
            os_dly_wait(2);

            swd_set_target_reset(0);
            os_dly_wait(2);
        } while(!swd_init_debug());
    }
}

void target_before_init_debug(void) {
    //swd_set_target_state_hw(RESET_HOLD);
    return;
}

uint8_t target_unlock_sequence(void) {
    return 1;
}

uint8_t target_set_state(TARGET_RESET_STATE state) {
    uint8_t status;
    
    target_set_state_by_board(state);
    
    if( state == RESET_RUN )
    {
        do
        {
            swd_set_target_reset(1);
            os_dly_wait(2);
            swd_set_target_reset(0);
            os_dly_wait(2);
        } while(!swd_init_debug());

        swd_off();
        status = 1;
    }
    else
    {
        status = swd_set_target_state_sw(state);
    }
    
    return status;
}

uint8_t security_bits_set(uint32_t addr, uint8_t *data, uint32_t size)
{
    return 0;
}
