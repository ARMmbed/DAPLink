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

void gpio_set_isp_pin(uint8_t state);

void target_before_init_debug(void) {
    return;
}

uint8_t target_unlock_sequence(void) {
    return 1;
}

uint8_t target_set_state(TARGET_RESET_STATE state) {
    volatile uint8_t res;
    if (state == RESET_PROGRAM)
    {
        gpio_set_isp_pin(0);
        res = swd_set_target_state(state);
        gpio_set_isp_pin(1);
    }
    else
    {
        gpio_set_isp_pin(1);
        res = swd_set_target_state(state);
    }
    return res;
}
