/**
 * @file    target_board.c
 * @brief   Implementation of target_board.h
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
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

#include <string.h>
#include "target_board.h"
#include "compiler.h"

// Disable optimization of these functions.
//
// This is required because for the "no target" builds, the compiler sees g_board_info fields as
// defined above and will elide entire expressions. However, the board and target info may be
// modified using the post processor script, changing what the code sees at runtime.

NO_OPTIMIZE_PRE
__WEAK const char * NO_OPTIMIZE_INLINE get_board_id(void)
{
    if (g_board_info.target_cfg && g_board_info.target_cfg->rt_board_id) {
        return g_board_info.target_cfg->rt_board_id; //flexible board id
    } else {
        return g_board_info.board_id;
    }
}
NO_OPTIMIZE_POST

NO_OPTIMIZE_PRE
__WEAK uint16_t NO_OPTIMIZE_INLINE get_family_id(void)
{
    if (g_board_info.target_cfg && g_board_info.target_cfg->rt_family_id) {
        return g_board_info.target_cfg->rt_family_id; //flexible family id
    } else {
        return g_board_info.family_id;
    }
}
NO_OPTIMIZE_POST

// Disable optimization of this function.
//
// This is required because for the "no target" builds, the compiler sees g_board_info.target_cfg as
// NULL and will elide the entire expression. However, the board and target info may be modified
// using the post processor script, changing what the code sees at runtime.
NO_OPTIMIZE_PRE
__WEAK uint8_t NO_OPTIMIZE_INLINE flash_algo_valid(void)
{
    return (g_board_info.target_cfg != 0);
}
NO_OPTIMIZE_POST
