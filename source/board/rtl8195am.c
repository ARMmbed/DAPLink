/**
 * @file    rtl8195am.c
 * @brief   board ID for the Realtek RTL8195AM board
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2014-2017, Realtek Semiconductor Corp., All Rights Reserved
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

#include "stdbool.h"
#include "flash_manager.h"

const char *board_id = "4600";

void prerun_board_config(void)
{
    flash_manager_set_page_erase(true);
}
