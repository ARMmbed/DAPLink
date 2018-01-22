/**
 * @file    target_reset_nrf5x_sam3u2c.c
 * @brief   Target reset for the nrf51 and nrf52
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
#include "DAP_config.h"
#include "info.h"

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
    const char *board_id;
    board_id = info_get_board_id();
    uint32_t ap_index_return;
    uint8_t nrf52_dk_is_used;
    uint8_t nrf52840_dk_is_used;
    uint8_t ublox_evk_nina_b1_used;

    nrf52_dk_is_used =      (   board_id[0] == '1'
                                && board_id[1] == '1'
                                && board_id[2] == '0'
                                && board_id[3] == '1') ? 1 : 0;  // ID 1101 is the nrf52-dk

    nrf52840_dk_is_used =   (   board_id[0] == '1'
                                && board_id[1] == '1'
                                && board_id[2] == '0'
                                && board_id[3] == '2') ? 1 : 0;  // ID 1102 is the nrf52840-dk

    ublox_evk_nina_b1_used = (  board_id[0] == '1'
                                && board_id[1] == '2'
                                && board_id[2] == '3'
                                && board_id[3] == '7') ? 1 : 0;  // ID 1237 is the UBLOX_EVK_NINA_B1 which is based on NRF52

    if (nrf52_dk_is_used || nrf52840_dk_is_used || ublox_evk_nina_b1_used) {
        if (asserted) {
            swd_init_debug();
            
            swd_read_ap(0x010000FC, &ap_index_return);
            if (ap_index_return == 0x02880000) {
                // Device has CTRL-AP
                swd_write_ap(0x01000000, 1);  // CTRL-AP reset hold
            }
            else {
                // No CTRL-AP - Perform a soft reset
                uint32_t swd_mem_write_data = 0x05FA0000 /* VECTKEY */ | 0x4 /* SYSRESETREQ */;
                swd_write_memory(0xE000ED0C, (uint8_t *)&swd_mem_write_data, 4);
                //os_dly_wait(1);
            }
        }
        else {           
            swd_read_ap(0x010000FC, &ap_index_return);
            if (ap_index_return == 0x02880000) {
                // Device has CTRL-AP
                swd_write_ap(0x01000000, 0);  // CTRL-AP reset release
            }
            else {
                // No CTRL-AP - Soft reset has been performed
            }
            PIOA->PIO_MDER = PIN_SWDIO | PIN_SWCLK | PIN_nRESET;
        }
    }
    else {
        if (asserted) {
            swd_init_debug();
            
            /*  There is no reset pin on the nRF51822, so we need to use special reset routine, 
                SWDCLK and SWDIO/nRESET needs to be kept low for a minimum of 100uS. Called of this func needs to make sure of this. */
            //Set POWER->RESET on NRF to 1
            if (!swd_write_ap(AP_TAR, 0x40000000 + 0x544)) {
                return;
            }

            if (!swd_write_ap(AP_DRW, 1)) {
                return;
            }

            //Hold RESET and SWCLK low for a minimum of 100us
            PIOA->PIO_OER = PIN_SWDIO;
            PIOA->PIO_OER = PIN_SWCLK;
            PIN_SWCLK_TCK_CLR();
            PIN_SWDIO_TMS_CLR();
            //os_dly_wait(1);
        }
        else {
            PIN_SWCLK_TCK_SET();
            PIN_SWDIO_TMS_SET();
            PIOA->PIO_MDER = PIN_SWDIO | PIN_SWCLK | PIN_nRESET;
        }
    }
}
