/**
 * @file    target_reset.c
 * @brief   Target reset for the MAX32620
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
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
#include "target_family.h"
#include "swd_host.h"
#include "DAP_Config.h"
#include "target_board.h"

#define DBG_Addr     (0xe000edf0)   // Default Core debug base addresses

#define TARGET_MXC_BASE_FLC         ((uint32_t)0x40002000UL)    // Flash controller base address
#define TARGET_MXC_R_FLC_PERFORM    (TARGET_MXC_BASE_FLC + (uint32_t)0x00000050UL)  // Flash controller perform register

#define INCREASE_DELAY  10
#define MAX_DELAY       30000


/* RESET_PROGRAM state halts the processor on reset by default but the SWD
 * is unavailable for 1024 clocks after reset.
 * This code will reset and halt after the SWD lockout period.
 */
static uint8_t target_set_state_max32620(target_state_t state)
{
    unsigned int halt_timeout, halt_retries = 10;
    unsigned int lockout_delay, i;
    uint32_t val;

    if (state != RESET_PROGRAM) {
        return swd_set_target_state_hw(state);
    }

    /* Reset the target and halt it when SWD is available */
    while (halt_retries--) {
        halt_timeout = 10;
        lockout_delay = 0;

        /* Wait for SWD lockout period after reset */
        do {
            swd_init();

            PIN_nRESET_OUT(0);
            for (i = 0; i < 1000; i++);
            PIN_nRESET_OUT(1);
            for (i = 0; i < lockout_delay; i++);

            lockout_delay += INCREASE_DELAY;

        } while ((JTAG2SWD() == 0) && (lockout_delay < MAX_DELAY));

        if (lockout_delay >= MAX_DELAY) {
            continue;
        }

        if (!swd_write_dp(DP_CTRL_STAT, CSYSPWRUPREQ | CDBGPWRUPREQ | TRNNORMAL | MASKLANE)) {
            continue;
        }

        /* Dummy read here forces the line buffer to actually read flash the
         * next time the flash memory base address has to be read.
         * Generally, address read here should be an offset by atleast the size
         * of line buffer (128 bytes) from the base address.
         */
        uint8_t tmp;
        swd_read_byte(g_board_info.target_cfg->flash_regions[0].start + 0x2000, &tmp);

        /* Set the Debug, and Halt bits and wait until core is halted or timeout */
        if (swd_write_word(DBG_HCSR, DBGKEY | C_DEBUGEN | C_HALT)) {
            do {
                if (!swd_read_word(DBG_HCSR, &val)) {
                    break;
                }
            } while (((val & S_HALT) == 0) && (halt_timeout--));
        }

        if (val & S_HALT) {
            /* Setting the Flash Controller Perform register to 0 allows
             * the Flash Controller pending bit to clear, otherwise the
             * bit remains set which could fail flash operation.
             */
            val = 0;
            swd_write_word(TARGET_MXC_R_FLC_PERFORM, val);

            return 1;
        }
    }

    return 0;
}

const target_family_descriptor_t g_maxim_max32620 = {
    .family_id = kMaxim_MAX32620_FamilyID,
    .target_set_state = target_set_state_max32620,
    .default_reset_type = kHardwareReset,
};
