/*******************************************************************************
* @file    cy8ckit.c
* @brief   Board ID definitions for KitProg3 devices.
*
********************************************************************************
* Copyright (2019-2021) Cypress Semiconductor Corporation (an Infineon company)
* or an affiliate of Cypress Semiconductor Corporation.
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
*****************************************************************************/

#include "target_board.h"
#include "target_family.h"
#include "kit_utils.h"

// Called in main_task() to init before USB and files are configured
static void prerun_board_config(void)
{
    // Define board ID
    define_board_id();

    //Reset target
    (void)target_set_state(RESET_RUN);
}


// Default Daplink board_info
// The g_board_info also defined in target_board.c but with __attribute__((weak))
// coverity[misra_c_2012_rule_8_6_violation]
const board_info_t g_board_info =
{
    .info_version = (uint16_t)kBoardInfoVersion,
    .board_id = "19FF",
    .family_id = (uint16_t)kCypress_psoc6_FamilyID,
    .target_cfg = &target_device,
    .prerun_board_config = prerun_board_config,
    .flags = (uint32_t)kEnablePageErase
};

