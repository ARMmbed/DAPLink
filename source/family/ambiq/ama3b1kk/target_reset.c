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

#include "target_reset.h"
#include "swd_host.h"
#include "target_family.h"

static void target_before_init_debug(void)
{
    // any target specific sequences needed before attaching
    //	to the DAP across JTAG or SWD
    return;
}

static uint8_t target_unlock_sequence(void)
{
    // if the device can secure the flash and there is a way to
    //	erase all it should be implemented here.
    return 1;
}

static uint8_t target_set_state(TARGET_RESET_STATE state)
{
    // if a custom state machine is needed to set the TARGET_RESET_STATE state
    return 1;
}

static uint8_t security_bits_set(uint32_t addr, uint8_t *data, uint32_t size)
{
    // if there are security bits in the programmable flash region
    //	a check should be performed. This method is used when programming
    //	by drag-n-drop and should refuse to program an image requesting
    //	to set the device security. This can be performed with the debug channel
    //	if needed.
    return 0;
}

const target_family_descriptor_t _g_target_family = {
    .family_id = kAmbiq_ama3b1kk_FamilyID,
    .default_reset_type = kHardwareReset,
    .target_before_init_debug = target_before_init_debug,
    .target_unlock_sequence = target_unlock_sequence,
    .target_set_state = target_set_state,
    .security_bits_set = security_bits_set,
};

const target_family_descriptor_t* g_target_family = &_g_target_family;
