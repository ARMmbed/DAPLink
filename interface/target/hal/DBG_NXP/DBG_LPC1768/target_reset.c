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

void target_before_init_debug(void) {
    return;
}

uint8_t target_unlock_sequence(void) {
    return 1;
}

uint8_t target_set_state(TARGET_RESET_STATE state) {
    return swd_set_target_state(state);
}


//
// UBLOX code for power detection

// #ifdef BOARD_UBLOX_C027
//     PORT_SWD_SETUP();
//     // wait until reset output to the target is pulled high
//     while (!PIN_nRESET_IN()) {
//         /* wait doing nothing */
//     }
//     os_dly_wait(4);
//     // if the reset input from button is low then enter isp programming mode
//     if (!(LPC_GPIO->B[19/*RESET_PIN*/ + (1/*RESET_PORT*/ << 5)] & 1)) {
//         enter_isp();
//     }
// #endif

//
