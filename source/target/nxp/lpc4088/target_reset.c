/**
 * @file    target_reset.c
 * @brief   Target reset for the lpc4088
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
#include "gpio.h"

#define PIN_ISPCTRL1      (1<<12)
#define PIN_ISPCTRL2      (1<<15)

static void gpio_set_isp_pin(uint8_t state) {
    if (state) {
        // High => Both pins are inputs
        LPC_GPIO->DIR[0] &= ~(PIN_ISPCTRL1);
        LPC_GPIO->DIR[1] &= ~(PIN_ISPCTRL2);
    } else {
        // Low => Both pins are outputs with 0
        LPC_GPIO->CLR[0] = (PIN_ISPCTRL1);
        LPC_GPIO->CLR[1] = (PIN_ISPCTRL2);
        LPC_GPIO->DIR[0] |= (PIN_ISPCTRL1);
        LPC_GPIO->DIR[1] |= (PIN_ISPCTRL2);
    }
}

void prerun_target_config(void)
{
    // Configure ISPCTRL as output and high
    // We use two ISP pins to cover different hardware versions
    LPC_IOCON->TMS_PIO0_12 |= 0x01;
    LPC_IOCON->PIO1_15 &= ~0x07;
    gpio_set_isp_pin(1);
}

void target_before_init_debug(void)
{
    return;
}

uint8_t target_unlock_sequence(void)
{
    return 1;
}

uint8_t target_set_state(TARGET_RESET_STATE state)
{
    //return swd_set_target_state_hw(state);
    uint8_t res;
    if (state == RESET_PROGRAM)
    {
        gpio_set_isp_pin(0);
        res = swd_set_target_state_hw(state);
        gpio_set_isp_pin(1);
    }
    else
    {
        gpio_set_isp_pin(1);
        res = swd_set_target_state_hw(state);
    }
    return res;
}

uint8_t security_bits_set(uint32_t addr, uint8_t *data, uint32_t size)
{
    return 0;
}

