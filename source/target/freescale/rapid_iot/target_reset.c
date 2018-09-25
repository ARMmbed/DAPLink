/**
 * @file    target_reset.c
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
#include "info.h"
#include "target_config.h"

#define MDM_STATUS  0x01000000
#define MDM_CTRL    0x01000004
#define MDM_IDR     0x010000fc
#define MDM_ID_K64  0x001c0000 // K64
#define MDM_ID_KW4  0x001c0020 // KW4

/* Kinetis series ID */
#define K_SERIES     0
#define KW_SERIES    5

/* KW4 subfamily defines */
#define KW40         0
#define KW41         1

static uint32_t mdm_id;
extern char *board_id;
char *board_id_kw41z = "0234";

void target_before_init_debug(void)
{
    return;
}

void prerun_target_config(void)
{
    // SIM peripheral   0x40047000
    // address offset   0x    1024
    uint32_t SDID = 0x40048024;
    uint32_t sdid;

    mdm_id = MDM_ID_K64;

    // get a hold of the target
    if (target_set_state(HALT) == 0) {
        /*
         * When the Kinetis flash is empty the reset line toggles. This causes failures
         * when trying to halt the target. Use the reset halt method in this case.
         */
        target_set_state(RESET_PROGRAM);
    }

    // Read the system identification register
    swd_read_memory(SDID, (uint8_t *)&sdid, 4);

    // Set the target flash algorithm
    if (((sdid >> 20) & 0xF) == KW_SERIES) {
        mdm_id = MDM_ID_KW4;

        if (((sdid >> 24) & 0x3) == KW40) {
            // Program to the KW40 flash
            extern target_cfg_t target_device_kw40;
            target_device = target_device_kw40;
        } else {
            // Program to the KW41 flash
            extern target_cfg_t target_device_kw41;
            target_device = target_device_kw41;
            board_id = board_id_kw41z;
        }
    }

    // Let the target run
    target_set_state(RUN);
}

void board_init(void)
{
}

uint8_t target_unlock_sequence(void)
{
    uint32_t val;

    // read the device ID
    if (!swd_read_ap(MDM_IDR, &val)) {
        return 0;
    }

    // verify the result
    if (val != mdm_id) {
        return 0;
    }

    if (!swd_read_ap(MDM_STATUS, &val)) {
        return 0;
    }

    // flash in secured mode
    if (val & (1 << 2)) {
        // hold the device in reset
        swd_set_target_reset(1);

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

// Check Flash Configuration Field bytes at address 0x400-0x40f to ensure that flash security
// won't be enabled.
//
// FCF bytes:
// [0x0-0x7]=backdoor key
// [0x8-0xb]=flash protection bytes
// [0xc]=FSEC:
//      [7:6]=KEYEN (2'b10 is backdoor key enabled, all others backdoor key disabled)
//      [5:4]=MEEN (2'b10 mass erase disabled, all other mass erase enabled)
//      [3:2]=FSLACC (2'b00 and 2'b11 factory access enabled, 2'b01 and 2'b10 factory access disabled)
//      [1:0]=SEC (2'b10 flash security disabled, all other flash security enabled)
// [0xd]=FOPT
// [0xe]=EEPROM protection bytes (FlexNVM devices only)
// [0xf]=data flash protection bytes (FlexNVM devices only)
//
// This function checks that:
// - FSEC does not disable mass erase or secure the device.
//
uint8_t security_bits_set(uint32_t addr, uint8_t *data, uint32_t size)
{
    const uint32_t fsec_addr = 0x40C;

    if ((addr <= fsec_addr) && (addr + size) > fsec_addr) {
        uint8_t fsec = data[fsec_addr - addr];

        // make sure we can unsecure the device or dont program at all
        if ((fsec & 0x30) == 0x20) {
            // Dont allow programming mass-erase disabled state
            return 1;
        }

        // Security is OK long as we can mass-erase (comment the following out to enable target security)
        if ((fsec & 0x03) != 0x02) {
            return 1;
        }
    }

    return 0;
}

uint8_t target_set_state(TARGET_RESET_STATE state)
{
    return swd_set_target_state_hw(state);
}
