/**
 * @file    blueninja_sb.c
 * @brief   board ID for the Cerevo BlueNinja sub board & specific functions.
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
#include <RTL.h>
#include "IO_Config.h"
#include "swd_host.h"

const char *board_id = "0000";


void target_set_state_by_board(TARGET_RESET_STATE state)
{
    
    if (RESET_PROGRAM == state) {
        LPC_GPIO->SET[PIN_PWH_PORT] = PIN_PWH;
        os_dly_wait(10);
    }
}
