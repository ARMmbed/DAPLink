/**
 * @file    target_default.c
 * @brief   Default target board info
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2021, Arm Limited, All Rights Reserved
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

// g_board_info needed to be moved away from target_board.c to prevent
// compilers from make optimization decisions based on it.

// Default empty board info.
__WEAK
const board_info_t g_board_info = {
		.info_version = kBoardInfoVersion,
		.board_id = "0000",
		.daplink_url_name =       "MBED    HTM",
		.daplink_drive_name = 		"DAPLINK    ",
		.daplink_target_url = "https://mbed.org/device/?code=@U?version=@V?target_id=@T",
};
