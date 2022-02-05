/**
 * @file    xDot.c
 * @brief   board ID for the MultiTech xDot
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
#include "target_board.h"
#define DBG_Addr     (0xe000edf0)
#include "swd_host.h"
#include "cmsis_os2.h"
#include "debug_cm.h"

static uint8_t xdot_set_state(target_state_t state)
{
    uint32_t val;

    if (state == RESET_PROGRAM) {
        swd_init();
        swd_set_target_reset(1);
        osDelay(2);

        if (!swd_init_debug()) {
            return 0;
        }

        // Enable debug
        if (!swd_write_word(DBG_HCSR, DBGKEY | C_HALT | C_DEBUGEN)) {
            return 0;
        }

        // Enable halt on reset
        if (!swd_write_word(DBG_EMCR, VC_CORERESET)) {
            return 0;
        }

        // Deassert reset
        swd_set_target_reset(0);
        osDelay(2);

        do {
            if (!swd_read_word(DBG_HCSR, &val)) {
                return 0;
            }
        } while ((val & S_HALT) == 0);

        // Disable halt on reset
        if (!swd_write_word(DBG_EMCR, 0)) {
            return 0;
        }
    } else {
        return swd_set_target_state_hw(state);
    }

    return 1;
}

const board_info_t g_board_info = {
    .info_version = kBoardInfoVersion,
    .board_id = "0350",
    .family_id = kStub_HWReset_FamilyID,
    .flags = kEnableUnderResetConnect,
    .daplink_url_name = "XDOT    HTM",
    .daplink_drive_name =  "XDOT       ",
    .target_cfg = &target_device,
    .target_set_state = xdot_set_state,
    .board_vendor = "MultiTech",
    .board_name = "xDot",
};
