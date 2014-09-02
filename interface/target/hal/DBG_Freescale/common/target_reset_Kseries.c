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

#define MDM_STATUS 0x01000000
#define MDM_CTRL   0x01000004
#define MDM_IDR    0x010000fc

#define MCU_ID     0x001c0000

void target_before_init_debug(void) {
    swd_set_target_reset(1);
}

uint8_t target_unlock_sequence(void) {
    uint32_t val;

    // read the device ID
    if (!swd_read_ap(MDM_IDR, &val)) {
        return 0;
    }
    // verify the result
    if (val != MCU_ID) {
        return 0;
    }

    if (!swd_read_ap(MDM_STATUS, &val)) {
        return 0;
    }

    // flash in secured mode
    if (val & (1 << 2)) {
        // hold the device in reset
        target_set_state(RESET_HOLD);
        // write the mass-erase enable bit
        if (!swd_write_ap(MDM_CTRL, 1)) {
            return 0;
        }
        while (1) {
            // wait until mass erase is started
            if (!swd_read_ap(MDM_STATUS, &val)) {
                return 0;
            }

            if (val & 1) {
                break;
            }
        }
        // mass erase in progress
        while (1) {            
            // keep reading until procedure is complete
            if (!swd_read_ap(MDM_CTRL, &val)) {
                return 0;
            }

            if (val == 0) {
                break;
            }
        }
    }

    return 1;
}

uint8_t target_set_state(TARGET_RESET_STATE state) {
    return swd_set_target_state(state);
}

