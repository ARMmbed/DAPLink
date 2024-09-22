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

uint8_t swd_set_state_ama3bx(target_state_t state)
{
    uint32_t val, scratch0, secure, bootldr;
    int8_t ap_retries = 2;

    if (state == RESET_PROGRAM) {
        swd_init();

        if (!swd_init_debug()) {
            return 0;
        }

        // Enable debug and halt the core (DHCSR <- 0xA05F0003)
        while (swd_write_word(DBG_HCSR, DBGKEY | C_DEBUGEN ) == 0) {
            if ( --ap_retries <=0 ) {
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

        if (!swd_read_word(MCUCTRL_BOOTLDR, &bootldr)) {
            return 0;
        }
        secure = ((bootldr & 0xC000000) == 0x04000000);

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

        if (secure) {
            if (!swd_write_word(MCUCTRL_SCRATCH0, scratch0)) {
                return 0;
            }
        } else {
            if (!swd_write_word(DBG_EMCR, 0)) {
                return 0;
            }
        }

    }

    return swd_set_target_state_sw(state);
}

const target_family_descriptor_t g_ambiq_ama3bx_family = {
    .family_id = kAmbiq_ama3bxkk_FamilyID,
    .default_reset_type = kSoftwareReset,
    .soft_reset_type = SYSRESETREQ,
    .target_set_state = swd_set_state_ama3bx,
};
