/*******************************************************************************
* @file    kit_defs.h
* @brief   This file provides definitions of Cypress kits
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

#if !defined(KIT_DEFS_H)
#define KIT_DEFS_H /**< Symbol preventing repeated inclusion */

#include <stdbool.h>
#include <stdint.h>

/* Maximum count of supported  HW ids */
#define MAX_SUPPORTED_HWID          (31u)

/* Silicon ids by mpn without revision bits (mask is 0xFFFF00FF) */
#define CY8C6245LQI_S3D72   (0xE7010005u)
#define CY8C6247BZI_D54     (0xE2060000u)
#define CY8C6247FDI_D32     (0xE2330000u)
#define CY8C6247FDI_D52     (0xE2340000u)
#define CY8C624ABZI_S2D44   (0xE4530002u)
#define CY8C624ABZI_S2D44A0 (0xE4020002u)
#define CY8C624AFNI_D43     (0xE4140002u)
#define CY8C624AFNI_S2D43   (0xE4550002u)
#define CY8C6347BZI_BLD53   (0xE2070000u)
#define CYB06445LQI_S3D42   (0xE70D0005u)
#define CYB06447BZI_BLD53   (0xE2610000u)
#define CYB06447BZI_D54     (0xE2620000u)
#define CYB0644ABZI_S2D44   (0xE4700002u)
#define CYBLE_416045_02     (0xE2F00000u)
#define CYS0644ABZI_S2D44   (0xE4A00002u)
#define UNSPECIFIED_SIID    (0xFFFFFFFFu)

/* Board properties struct definition */
typedef struct
{
    bool kitHasThreeLeds;  /**< Kit has three lEDs, assuming in false case kit has single LED */
    bool possibleDAPLink;  /**< HW of the board allows to DAPLink work */
    bool kitIsMiniProg;    /**< Kit is MiniProg, a standalone programmer */
} board_properties_t;

/* Kit detection record struct */
typedef struct
{
    uint8_t hw_id;          // Kit HW id - 1 byte
    uint32_t target_siid;   // Target silicon id - 4 bytes
    uint16_t uid;           // Unique kit ID - 2 bytes
} kit_id_struct_t;

/* Kits unique ID enums */
enum
{
    CY8CKIT_041_40XX      = 0x0001u,
    CY8CKIT_041_41XX      = 0x0002u,
    CY8CKIT_048           = 0x0003u,
    CY8CKIT_145_40XX      = 0x0004u,
    CY8CKIT_062_BLE       = 0x0005u,
    CY8CKIT_062_WIFI_BT   = 0x0006u,
    CY8CKIT_146           = 0x0007u,
    CY8CKIT_147           = 0x0008u,
    CY8CKIT_149           = 0x0009u,
    CY8CKIT_148           = 0x000Au,
    CY8CPROTO_063_BLE     = 0x000Bu,
    CY8CKIT_062_AUDIO     = 0x000Cu,
    CY8CKIT_005           = 0x000Du,
    CY8CKIT_005_CN        = 0x000Fu,
    CY8CPROTO_062_4343W   = 0x0010u,
    CY8CPROTO_064_SB      = 0x0012u,
    CYW943012P6EVB_01     = 0x0015u,
    CY8CKIT_062_4343W     = 0x0016u,
    CY8CPROTO_062S2_43012 = 0x0017u,
    CY8CKIT_062S2_43012   = 0x0018u,
    CY8CPROTO_062S3_4343W = 0x0019u,
    AUGUST_CYW43012       = 0x001Au,
    CYW9P62S1_43438EVB_01 = 0x001Bu,
    CYW9P62S1_43012EVB_01 = 0x001Cu,
    CY8CKIT_064S2_4343W   = 0x001Du,
    CY8CPROTO_064B0S3     = 0x001Eu,
    CY8CPROTO_064B0S1_BLE = 0x001Fu,
    CY8CKIT_064B0S2_4343W = 0x0020u,
    CY8CKIT_064S0S2_4343W = 0x0021u,
    CYSBSYSKIT_01         = 0x0022u,
    CY8CKIT_062S4         = 0x0023u,
    CY8CKIT_045S          = 0x0024u,
    CY8CKIT_041S_MAX      = 0x0025u,
    CYHVPA_128K_32_001    = 0x0026u,
    CY7110                = 0x0027u,
    CY7111                = 0x0028u,
    CY7112                = 0x0029u,
    CY7113                = 0x002Au,
    CY8CEVAL_062S2        = 0x002Bu,
    CY8CEVAL_064SXS2      = 0x002Cu,
    UNSPECIFIED_UID
};

extern const board_properties_t board_config[];
extern const char * const board_ids[];
extern const kit_id_struct_t kit_ids[];
extern const uint16_t uns_kit_ids[];
extern const uint32_t uns_kit_count;
extern const uint32_t kit_id_count;
extern const uint16_t secure_kits[];
extern const uint32_t secure_kit_count;

#endif
