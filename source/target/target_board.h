/**
 * @file    target_board.h
 * @brief   
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2018-2019, ARM Limited, All Rights Reserved
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

#include <stdint.h>
#include "target_config.h"
#include "target_reset.h"
#include "virtual_fs.h" 

// Flags for board_info 
enum { 
	kEnablePageErase = 1,               /*!< Enable page programming and sector erase for drag and drop */
    kEnableUnderResetConnect = 1<<1,    /*!< Enable under reset connection when enabling debug mode */
};

typedef struct __attribute__((__packed__)) board_info { 
    uint16_t infoVersion;               /*!< Version number of the board */ 
    uint16_t family_id;                 /*!< Use to select or identify target family from defined target family or custom ones */ 
    char board_id[5];                   /*!< 4-char board ID plus null terminator */
    uint8_t _padding[3]; 
    uint32_t flags;                     /*!< Combination of kEnablePageErase and kEnableUnderResetConnect */
    target_cfg_t *target_cfg;           /*!< Specific chip configuration for the target and enables MSD when non-NULL */ 
     
    // fields used by MSD 
    vfs_filename_t daplink_url_name;    /*!< Customize the URL file name */
    vfs_filename_t daplink_drive_name;  /*!< Customize the MSD DAPLink drive name */
    char daplink_target_url[64];        /*!< Customize the target url in DETAILS.TXT */
    
    // some specific board initilization
    void (*prerun_board_config)(void);                      /*!< Specific board debug/ID related initialization */
    void (*swd_set_target_reset)(uint8_t asserted);         /*!< Boards can customize how to send reset to the target precedence over target family */
    uint8_t (*target_set_state)(TARGET_RESET_STATE state);  /*!< Boards can customize target debug states in target_reset.h precedence over target family */
    uint32_t soft_reset_type;                               /*!< Boards can override software reset type to VECTRESET or SYSRESETREQ */
} board_info_t;

extern const board_info_t g_board_info;

#ifdef __cplusplus
extern "C" {
#endif

const char * get_board_id(void);
uint16_t get_family_id(void);
uint8_t flash_algo_valid(void);

static inline const char * get_daplink_url_name ( void ) { return ((g_board_info.daplink_url_name[0] != 0) ? g_board_info.daplink_url_name : "MBED    HTM"); }
static inline const char * get_daplink_drive_name ( void ) { return ((g_board_info.daplink_drive_name[0] != 0) ? g_board_info.daplink_drive_name : "DAPLINK    "); }
static inline const char * get_daplink_target_url ( void ) { return ((g_board_info.daplink_target_url[0] != 0) ? g_board_info.daplink_target_url : "https://mbed.org/device/?code=@U?version=@V?target_id=@T"); }

#ifdef __cplusplus
}
#endif

#endif
