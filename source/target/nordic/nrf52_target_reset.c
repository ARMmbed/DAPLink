/**
 * @file    target_reset.c
 * @brief   Target reset for the nrf52
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

#include "target_reset.h"
#include "swd_host.h"
#include "DAP_Config.h"

void target_before_init_debug(void)
{
    return;
}

uint8_t target_unlock_sequence(void)
{
    return 1;
}

uint8_t security_bits_set(uint32_t addr, uint8_t *data, uint32_t size)
{
    return 0;
}

uint8_t target_set_state(TARGET_RESET_STATE state)
{
    return swd_set_target_state_sw(state);
}

void swd_set_target_reset(uint8_t asserted)
{
    uint32_t ap_index_return;
    
    if (asserted) {
        swd_init_debug();
        
        swd_read_ap(0x010000FC, &ap_index_return);
        if (ap_index_return == 0x02880000) {
            // Have CTRL-AP
            swd_write_ap(0x01000000, 1);  // CTRL-AP reset hold
        }
        else {
            // No CTRL-AP - Perform a soft reset
            uint32_t swd_mem_write_data = 0x05FA0000 /* VECTKEY */ | 0x4 /* SYSRESETREQ */;
            swd_write_memory(0xE000ED0C, (uint8_t *)&swd_mem_write_data, 4);
            //os_dly_wait(1);
        }

        //Hold RESET and SWCLK low for a minimum of 100us
        PIN_SWCLK_TCK_CLR();
        PIN_SWDIO_TMS_CLR();
        LED_CONNECTED_OUT(0);
        //os_dly_wait(1);
    } else {
        swd_read_ap(0x010000FC, &ap_index_return);
        if (ap_index_return == 0x02880000) {
            // Device has CTRL-AP
            swd_write_ap(0x01000000, 0);  // CTRL-AP reset release
        }
        else {
            // No CTRL-AP - Soft reset has been performed
        }
        
        PIN_SWCLK_TCK_SET();
        PIN_SWDIO_TMS_SET();
        LED_CONNECTED_OUT(1);
    }
}
