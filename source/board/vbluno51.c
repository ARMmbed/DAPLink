/**
 * @file    vbluno51.c
 * @brief   Board ID for the VBLUno51 board (VNG Bluetooth Low Energy UNO nRF51822 board)
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2017, ARM Limited, All Rights Reserved
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

#include "virtual_fs.h"
#include "uart.h"

const char *board_id = "C006";

// Override default behavior
//
// URL_NAME and DRIVE_NAME must be 11 characters excluding
// the null terminated character
// Note - 4 byte alignemnt required as workaround for ARMCC compiler bug with weak references
__attribute__((aligned(4)))
const vfs_filename_t daplink_drive_name =     "DAPLINK    ";
__attribute__((aligned(4)))
const char *const daplink_target_url = "https://os.mbed.com/platforms/VBLUNO51/";

void prerun_board_config()
{
    uart_enable_flow_control(false);
}
