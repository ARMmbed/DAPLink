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
#include "RTL.h"
#include "debug_cm.h"
#include "target_reset.h"
#include "swd_host.h"
#include "DAP_Config.h"

void target_before_init_debug(void) {
    return;
}

uint8_t target_unlock_sequence(void) {
    return 1;
}

uint8_t target_set_state(TARGET_RESET_STATE state) {
	/* sw reset preferred because hardware (pin) reset also resets debug logic */
  return swd_set_target_state_sw(state); 
}

uint8_t security_bits_set(uint32_t addr, uint8_t *data, uint32_t size) {
    return 0;
}
