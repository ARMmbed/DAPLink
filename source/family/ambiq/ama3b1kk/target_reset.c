/**
 * @file    target_reset.c
 * @brief   Target reset for the new target
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

#include "util.h"
#include "cmsis_os2.h"
#include "target_config.h"
#include "swd_host.h"
#include "debug_cm.h"
#include "DAP_config.h"
#include "DAP.h"
#include "target_family.h"
#include "device.h"
#include "daplink_debug.h"

#define NVIC_Addr (0xe000e000)
#define DBG_Addr (0xe000edf0)

#define MCUCTRL_SCRATCH0 0x400401B0
#define MCUCTRL_BOOTLDR 0x400401A0
#define JDEC_PID 0xF0000FE0

#if !defined(SCB_AIRCR_PRIGROUP_Pos)
#define SCB_AIRCR_PRIGROUP_Pos 8U                              /*!< SCB AIRCR: PRIGROUP Position */
#define SCB_AIRCR_PRIGROUP_Msk (7UL << SCB_AIRCR_PRIGROUP_Pos) /*!< SCB AIRCR: PRIGROUP Mask */
#endif

uint8_t swd_set_state_ama3b1kk(target_state_t state)
{
    uint32_t val, scratch0, secure, jdecpid, bootldr;
    int8_t ap_retries = 2;
    swd_init();
    switch (state) {
        case RESET_HOLD:
            swd_set_target_reset(1);
            break;

        case RESET_RUN:
            swd_set_target_reset(1);
            osDelay(2);
            swd_set_target_reset(0);
            osDelay(2);
            swd_off();
            break;

        case RESET_PROGRAM:
            if (!swd_init_debug()) {
                return 0;
            }

            // Enable debug and halt the core (DHCSR <- 0xA05F0003)
            while (swd_write_word(DBG_HCSR, DBGKEY | C_DEBUGEN | C_HALT) == 0) {
                if (--ap_retries <= 0) {
                    return 0;
                }
                // Target is in invalid state?
                swd_set_target_reset(1);
                osDelay(2);
                swd_set_target_reset(0);
                osDelay(2);
            }

            // Wait until core is halted
            do {
                if (!swd_read_word(DBG_HCSR, &val)) {
                    return 0;
                }
            } while ((val & S_HALT) == 0);

            // Enable halt on reset
            //Confirm it is an apollo3
            if (!swd_read_word(JDEC_PID, &jdecpid)) {
                return 0;
            }
            if ((jdecpid & 0xf0) == 0xc0) {
                if (!swd_read_word(MCUCTRL_BOOTLDR, &bootldr)) {
                    return 0;
                }
                secure = ((bootldr & 0x0C000000) == 0x04000000);
            }

            if (secure) {
                if (!swd_read_word(MCUCTRL_SCRATCH0, &scratch0)) {
                    return 0;
                }
                if (!swd_write_word(MCUCTRL_SCRATCH0, (scratch0 | 0x1))) {
                    return 0;
                }
            } else {
                if (!swd_write_word(DBG_EMCR, VC_CORERESET)) {
                    return 0;
                }
            }

            // Perform a soft reset
            if (!swd_read_word(NVIC_AIRCR, &val)) {
                return 0;
            }

            if (!swd_write_word(NVIC_AIRCR, VECTKEY | (val & SCB_AIRCR_PRIGROUP_Msk) | SYSRESETREQ)) {
                return 0;
            }

            osDelay(10);

            do {
                if (!swd_read_word(DBG_HCSR, &val)) {
                    return 0;
                }
            } while ((val & S_HALT) == 0);

            // Disable halt on reset
            if (secure) {
                if (!swd_write_word(MCUCTRL_SCRATCH0, scratch0)) {
                    return 0;
                }
            } else {
                if (!swd_write_word(DBG_EMCR, 0)) {
                    return 0;
                }
            }

            break;

        case NO_DEBUG:
            if (!swd_write_word(DBG_HCSR, DBGKEY)) {
                return 0;
            }

            break;

        case DEBUG:
            if (!JTAG2SWD()) {
                return 0;
            }

            if (!swd_write_dp(DP_ABORT, STKCMPCLR | STKERRCLR | WDERRCLR | ORUNERRCLR)) {
                return 0;
            }

            // Ensure CTRL/STAT register selected in DPBANKSEL
            if (!swd_write_dp(DP_SELECT, 0)) {
                return 0;
            }

            // Power up
            if (!swd_write_dp(DP_CTRL_STAT, CSYSPWRUPREQ | CDBGPWRUPREQ)) {
                return 0;
            }

            // Enable debug
            if (!swd_write_word(DBG_HCSR, DBGKEY | C_DEBUGEN)) {
                return 0;
            }
            break;

        default:
            return 0;
    }

    return 1;
}

const target_family_descriptor_t g_ambiq_ama3b1kk = {
    .family_id = kAmbiq_ama3b1kk_FamilyID,
    .default_reset_type = kSoftwareReset,
    .soft_reset_type = SYSRESETREQ,
    .target_set_state = swd_set_state_ama3b1kk,
};


