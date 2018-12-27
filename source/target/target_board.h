/**
 * @file    target_board.h
 * @brief   
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2018, ARM Limited, All Rights Reserved
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

#ifndef TARGET_BOARD_H
#define TARGET_BOARD_H

#include "stdint.h"
#include "target_config.h"
#include "target_reset.h"
#include "virtual_fs.h" 

// Flags for board_info 
enum { 
	kEnablePageErase = 1, // only applies when MSD is enabled 
};

typedef struct board_info { 
	uint32_t infoVersion; // == kCurrentBoardInfoVersion 
	uint32_t family_id; 
	char board_id[5]; // 4-char board ID plus null terminator. 
	uint8_t _padding[3]; 
	uint32_t flags; 
	target_cfg_t *target_cfg; // enables MSD when non-NULL 
	 
	// fields used by MSD 
	vfs_filename_t daplink_url_name; 
	vfs_filename_t daplink_drive_name; 
	char daplink_target_url[64]; 
	
	// some specific board initilization
	void (*prerun_board_config)(void);
    void (*swd_set_target_reset)(uint8_t asserted);
    uint8_t (*target_set_state)(TARGET_RESET_STATE state);
    uint8_t (*validate_bin_nvic)(const uint8_t *buf);
    uint8_t (*validate_hexfile)(const uint8_t *buf);
    uint32_t soft_reset_type; 
} board_info_t;

extern const board_info_t g_board_info;
const char * get_board_id(void);
uint8_t flash_algo_valid(void);
uint8_t validate_bin_nvic(const uint8_t *buf);
uint8_t validate_hexfile(const uint8_t *buf);

inline const char * get_daplink_url_name ( void ) { return ((g_board_info.daplink_url_name[0] != 0) ? g_board_info.daplink_url_name : "MBED    HTM"); }
inline const char * get_daplink_drive_name ( void ) { return ((g_board_info.daplink_drive_name[0] != 0) ? g_board_info.daplink_drive_name : "DAPLINK    "); }
inline const char * get_daplink_target_url ( void ) { return ((g_board_info.daplink_target_url[0] != 0) ? g_board_info.daplink_target_url : "https://mbed.org/device/?code=@U?version=@V?target_id=@T"); }

#endif
