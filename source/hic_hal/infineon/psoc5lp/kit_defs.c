/*******************************************************************************
* @file    kit_defs.c
* @brief   Definitions of Cypress kits
*
* File Version : 1.0.60
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

#include "kit_defs.h"

/* Kit capabilities per HW ID */
const board_properties_t board_config[MAX_SUPPORTED_HWID + 1u] =
{
    [1u]  = { .kitHasThreeLeds = true,  .possibleDAPLink = false, .kitIsMiniProg = false }, /**< HWID = 0x01 */
    [2u]  = { .kitHasThreeLeds = true,  .possibleDAPLink = true,  .kitIsMiniProg = false }, /**< HWID = 0x02 */
    [3u]  = { .kitHasThreeLeds = false, .possibleDAPLink = true,  .kitIsMiniProg = false }, /**< HWID = 0x03 */
    [5u]  = { .kitHasThreeLeds = true,  .possibleDAPLink = true,  .kitIsMiniProg = true },  /**< HWID = 0x05 */
    [6u]  = { .kitHasThreeLeds = false, .possibleDAPLink = false, .kitIsMiniProg = false }, /**< HWID = 0x06 */
    [7u]  = { .kitHasThreeLeds = true,  .possibleDAPLink = false, .kitIsMiniProg = false }, /**< HWID = 0x07 */
    [8u]  = { .kitHasThreeLeds = false, .possibleDAPLink = false, .kitIsMiniProg = false }, /**< HWID = 0x08 */
    [10u] = { .kitHasThreeLeds = true,  .possibleDAPLink = false, .kitIsMiniProg = true },  /**< HWID = 0x0A */
    [11u] = { .kitHasThreeLeds = false, .possibleDAPLink = true,  .kitIsMiniProg = false }, /**< HWID = 0x0B */
    [12u] = { .kitHasThreeLeds = false, .possibleDAPLink = false, .kitIsMiniProg = false }, /**< HWID = 0x0C */
    [13u] = { .kitHasThreeLeds = false, .possibleDAPLink = true,  .kitIsMiniProg = false }, /**< HWID = 0x0D */
    [14u] = { .kitHasThreeLeds = false, .possibleDAPLink = false, .kitIsMiniProg = false }, /**< HWID = 0x0E */
};

/* MBed board ID per Cypress uinique ID */
const char * const board_ids[UNSPECIFIED_UID + 1u] = {
    [CY8CKIT_005          ] = "0000",
    [CY8CKIT_062_WIFI_BT  ] = "1900",
    [CY8CPROTO_062_4343W  ] = "1901",
    [CY8CKIT_062_BLE      ] = "1902",
    [CYW9P62S1_43012EVB_01] = "1903",
    [CY8CPROTO_063_BLE    ] = "1904",
    [CY8CKIT_062_4343W    ] = "1905",
    [CYW943012P6EVB_01    ] = "1906",
    [CY8CPROTO_064_SB     ] = "1907",
    [CYW9P62S1_43438EVB_01] = "1908",
    [CY8CPROTO_062S2_43012] = "1909",
    [CY8CKIT_064S2_4343W  ] = "190A",
    [CY8CKIT_062S2_43012  ] = "190B",
    [CY8CPROTO_064B0S3    ] = "190C",
    [AUGUST_CYW43012      ] = "190D",
    [CY8CPROTO_062S3_4343W] = "190E",
    [CY8CPROTO_064B0S1_BLE] = "190F",
    [CY8CKIT_064B0S2_4343W] = "1910",
    [CY8CKIT_064S0S2_4343W] = "1911",
    [CYSBSYSKIT_01        ] = "1912",
    [UNSPECIFIED_UID      ] = "19FF"
};

/* List of kits that can be detected
   Be careful: combination of hw_id and target_siid must be unique */
const kit_id_struct_t kit_ids[] =
{
    { .hw_id = 0x02u, .target_siid = CY8C6247BZI_D54,     .uid = (uint16_t)CY8CKIT_062_WIFI_BT },
    { .hw_id = 0x02u, .target_siid = CY8C624ABZI_S2D44,   .uid = (uint16_t)CY8CKIT_062_4343W },
    { .hw_id = 0x02u, .target_siid = CY8C624ABZI_S2D44A0, .uid = (uint16_t)CY8CKIT_062_4343W },
    { .hw_id = 0x02u, .target_siid = CY8C6347BZI_BLD53,   .uid = (uint16_t)CY8CKIT_062_BLE },
    { .hw_id = 0x02u, .target_siid = CYB0644ABZI_S2D44,   .uid = (uint16_t)CY8CKIT_064S2_4343W },
    { .hw_id = 0x03u, .target_siid = CYB06445LQI_S3D42,   .uid = (uint16_t)CY8CPROTO_064B0S3 },
    { .hw_id = 0x03u, .target_siid = CYB06447BZI_BLD53,   .uid = (uint16_t)CY8CPROTO_064B0S1_BLE },
    { .hw_id = 0x03u, .target_siid = CYB06447BZI_D54,     .uid = (uint16_t)CY8CPROTO_064_SB },
    { .hw_id = 0x03u, .target_siid = CYBLE_416045_02,     .uid = (uint16_t)CY8CPROTO_063_BLE },
    { .hw_id = 0x0Bu, .target_siid = CY8C6245LQI_S3D72,   .uid = (uint16_t)CY8CPROTO_062S3_4343W },
    { .hw_id = 0x0Bu, .target_siid = CY8C624ABZI_S2D44,   .uid = (uint16_t)CY8CPROTO_062_4343W },
    { .hw_id = 0x0Bu, .target_siid = CY8C624ABZI_S2D44A0, .uid = (uint16_t)CY8CPROTO_062_4343W },
    { .hw_id = 0x0Bu, .target_siid = CY8C624AFNI_D43,     .uid = (uint16_t)CYSBSYSKIT_01 },
    { .hw_id = 0x0Bu, .target_siid = CY8C624AFNI_S2D43,   .uid = (uint16_t)CYSBSYSKIT_01 },
    { .hw_id = 0x0Du, .target_siid = CY8C6247BZI_D54,     .uid = (uint16_t)CYW9P62S1_43438EVB_01 },
    { .hw_id = 0x0Du, .target_siid = CY8C6247FDI_D32,     .uid = (uint16_t)CYW9P62S1_43012EVB_01 },
    { .hw_id = 0x0Du, .target_siid = CY8C6247FDI_D52,     .uid = (uint16_t)CYW9P62S1_43012EVB_01 },
    { .hw_id = 0x0Du, .target_siid = CY8C624ABZI_S2D44,   .uid = (uint16_t)CY8CKIT_062S2_43012 },
    { .hw_id = 0x0Du, .target_siid = CY8C624ABZI_S2D44A0, .uid = (uint16_t)CY8CKIT_062S2_43012 },
    { .hw_id = 0x0Du, .target_siid = CYB0644ABZI_S2D44,   .uid = (uint16_t)CY8CKIT_064B0S2_4343W },
    { .hw_id = 0x0Du, .target_siid = CYS0644ABZI_S2D44,   .uid = (uint16_t)CY8CKIT_064S0S2_4343W }
};

const uint32_t kit_id_count = sizeof(kit_ids)/sizeof(kit_id_struct_t);

/* List of kits with secure target */
const uint16_t secure_kits[] = {
    AUGUST_CYW43012,
    CY8CKIT_064B0S2_4343W,
    CY8CKIT_064S0S2_4343W,
    CY8CKIT_064S2_4343W,
    CY8CPROTO_064_SB,
    CY8CPROTO_064B0S1_BLE,
    CY8CPROTO_064B0S3
};
 
const uint32_t secure_kit_count = sizeof(secure_kits)/sizeof(uint16_t);

/* List of kits with unsupported uids */
const uint16_t uns_kit_ids[] = {
    CY8CKIT_041_40XX,
    CY8CKIT_041_41XX,
    CY8CKIT_048,
    CY8CKIT_145_40XX,
    CY8CKIT_146,
    CY8CKIT_147,
    CY8CKIT_149,
    CY8CKIT_148,
    CY8CKIT_062_AUDIO,
    CY8CKIT_005_CN,
    CY8CKIT_062S4,
    CY8CKIT_045S,
    CY8CKIT_041S_MAX,
    CYHVPA_128K_32_001,
    CY7110,
    CY7111,
    CY7112,
    CY7113,
    CY8CEVAL_062S2,
    CY8CEVAL_064SXS2
};

const uint32_t uns_kit_count = sizeof(uns_kit_ids)/sizeof(uint16_t);
