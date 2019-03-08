/*******************************************************************************
* @file    kit_utils.c
* @brief   Utilities for Cypress kits
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
#include "IO_Config.h"
#include "CyLib.h"
#include "Pin_VoltageEn.h"
#include "EEPROM_ModeStorage.h"
#include "PSOC6xxx.h"
#include "psoc5lp.h"
#include "kit_defs.h"
#include "kit_utils.h"

// Minimum flashboot version with sflash restriction
#define MIN_FB_SR_VER               (0x24000000u)

// Successful acquiring response addr.
#define READY_ADDR_512K             (0x0803E004u)
#define READY_ADDR_1M               (0x08044804u)
#define READY_ADDR_2M               (0x080FE004u)

// Unique ID record header
#define UID_PSOC5_HEADER            (0x6970122Eu)

#define P5LP_EEPROM_ROW_SIZE        (16u)
#define CRC8_2S_COMP_BASE           (0x0100u)
#define UID_PRE_VERSION             (0x01u)
#define UID_CUR_VERSION             (0x02u)
#define TARGET_INFO_BYTE            (3u)
#define TARGET_SECURE_BIT_MASK      (0x80u)

// Unique ID record struct
// struct should be packed
// coverity[misra_c_2012_rule_1_2_violation]
typedef struct __attribute__((packed))
{
    uint32_t signature;     // Unique id header (PSoC5 silicon ID) - 4 bytes
    char mbed_board_id[4];  // mbed ID - 4 bytes
    uint16_t uid;           // Unique kit ID - 2 bytes
    uint8_t feature_list[8];// Kit feature list - 8 bytes
    char name[32];          // Full kit name - 32 bytes
    uint8_t hw_id;          // Kit HW id - 1 byte
    uint32_t target_siid;   // Target silicon id - 4 bytes
    uint8_t ver_minor;      // Record version(1 byte for major part,
    uint8_t ver_major;      // 1 byte for minor part) - 2 bytes
    uint8_t reserved[6];    // Reserved for future use - 6 bytes
    uint8_t checksum;       // Record checksum - 1 byte
} unique_id_struct_t;

static bool sflash_restriction = false;

static kit_id_struct_t cy_id =
{
    .uid = (uint16_t)UNSPECIFIED_UID
};

// Calculate checksum of uiniqe id record
static uint8_t calculate_uniq_id_checksum(const unique_id_struct_t *uid_record)
{
    const uint8_t *uid_record_arr = (const uint8_t *) uid_record;
    uint8_t checksum = 0u;
    const size_t checksum_pos = sizeof(unique_id_struct_t) - sizeof(checksum);

    for (uint8_t i = 0u; i < checksum_pos; i++)
    {
        checksum += uid_record_arr[i];
    }

    return (uint8_t)(CRC8_2S_COMP_BASE - checksum);
}

static bool kit_is_miniprog(void)
{
    return board_config[cy_id.hw_id].kitIsMiniProg;
}

static void kp3_power_on(void)
{
    if (!kit_is_miniprog())
    {
        // Powering up target for further recognition
        Pin_VoltageEn_Write(1u);

        // Wait for target initialization
        CyDelay(300u);
    }
}

// Define board id based on HW id and silicon ID
void define_board_id(void)
{

    bool DAPLinkIsSupported = true;
    cy_id.target_siid = UNSPECIFIED_SIID;

    // Start EEPROM block
    EEPROM_ModeStorage_Start();

    // coverity[misra_c_2012_rule_11_4_violation]
    const unique_id_struct_t *uid_record = (const unique_id_struct_t *)(CYDEV_EE_BASE + P5LP_EEPROM_ROW_SIZE);
    // Calculate unique id record checksum
    uint8_t calculated_checksum = calculate_uniq_id_checksum(uid_record);

    // 4-char board ID plus null terminator
    static char board_id[5] = "";

    bool uid_is_valid = (UID_PSOC5_HEADER == uid_record->signature) && (calculated_checksum == uid_record->checksum);

    if (uid_is_valid)
    {
        // Populate HW ID, Silicon ID and mbed board ID from unique id record
        cy_id.uid = uid_record->uid;
        cy_id.target_siid = uid_record->target_siid;
        (void)memcpy(board_id, uid_record->mbed_board_id, sizeof(uid_record->mbed_board_id));
        // String terminator
        board_id[4] = (char)0;
        familyID = ((uint16_t)PSOC6_FAMILY_ID_HI << SHIFT_8) | ((uint16_t)cy_id.target_siid & PSOC6_FAMILY_ID_LO_MSK);

        if (cy_id.uid >= (uint16_t)UNSPECIFIED_UID)
        {
            cy_id.uid = UNSPECIFIED_UID;
            DAPLinkIsSupported = false;
        }
        else
        {
            // Go through the list of known pairs HW_ID/Silicon_ID until the first match
            for( uint8_t i = 0u; i < (uns_kit_count); i++)
            {
                if (cy_id.uid == uns_kit_ids[i] )
                {
                    cy_id.uid = UNSPECIFIED_UID;
                    DAPLinkIsSupported = false;
                    break;
                }
            }
        }

        if (DAPLinkIsSupported)
        {
            switch (uid_record->ver_major) 
            {
                case UID_CUR_VERSION:
                {
                    sflash_restriction = ((uid_record->feature_list[TARGET_INFO_BYTE] & TARGET_SECURE_BIT_MASK) == TARGET_SECURE_BIT_MASK);
                    break;
                }

                case UID_PRE_VERSION:
                default:
                {
                    // Go through the list of known secure kits until the first match
                    for (uint32_t i = 0u; i < secure_kit_count; i++)
                    {
                        if (uid_record->uid == secure_kits[i])
                        {
                            sflash_restriction = true;
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    // Stop EEPROM block
    EEPROM_ModeStorage_Stop();

    if (DAPLinkIsSupported) 
    {
        // Validate HW ID
        bool possibleDAPLink = false;
        if (cy_id.uid == (uint16_t)UNSPECIFIED_UID)
        {
            // Read the real HW ID
            cy_id.hw_id = interrogate_kit_hw_id();
            if ((cy_id.hw_id <= MAX_SUPPORTED_HWID) && (board_config[cy_id.hw_id].possibleDAPLink))
            {
                possibleDAPLink = true;
                kp3_power_on();
            }
        }
        else
        {
            kp3_power_on();
        }

        // Analyze HW ID and Silicon ID in case
        // when unique id record is empty or corrupted
        if (possibleDAPLink)
        {
            if (kit_is_miniprog())
            {
                //if Miniprog do not attempt to read SIID and FB Version
                cy_id.uid = (uint16_t)CY8CKIT_005;
            }
            else
            {
                // Read silicon ID
                cy_id.target_siid = get_silcon_id();

                // Read flashboot version
                uint32_t fb_ver = get_fb_version();
                if (fb_ver >= MIN_FB_SR_VER)
                {
                    sflash_restriction = true;
                }
                else
                {
                    sflash_restriction = false;
                }

                // Go through the list of known pairs HW_ID/Silicon_ID until the first match
                for( uint32_t i = 0u; i < kit_id_count; i++)
                {
                    if ( (cy_id.hw_id == kit_ids[i].hw_id) && (cy_id.target_siid == kit_ids[i].target_siid) )
                    {
                        cy_id.uid = kit_ids[i].uid;
                        break;
                    }
                }
            }
        }
    }

    if ((cy_id.uid == (uint16_t)UNSPECIFIED_UID) || !DAPLinkIsSupported)
    {
        // Switch back to KP3 if the kit isn't supported by DAPLink
        SetKitProgActiveApp(KP3_MODE_BULK);
    }
    else
    {
        // Use board_id as rt_board_id if it isn't empty or value from board_ids array otherwise
        if ( board_id[0] == (char)0 )
        {
            g_board_info.target_cfg->rt_board_id = board_ids[cy_id.uid];
        }
        else
        {
            g_board_info.target_cfg->rt_board_id = board_id;
        }
    }
}

// Called in main_task() to init before USB and files are configured
void prerun_target_config(void)
{
    // Initialize main flash,WFlash, SFlash and SMIF programming algorithms
    // and set proper flash geometry based on family type/silicon ID
    init_flash_algo(cy_id.target_siid);
}

// Get .kitHasThreeLeds board property
bool kit_has_three_led(void)
{
    return board_config[cy_id.hw_id].kitHasThreeLeds;
}

// Get SFLASH restriction to write
bool kit_has_sflash_restriction(void)
{
    return sflash_restriction;
}

// Calculate ready value address
uint32_t get_readyval_addr(void)
{
    uint32_t readyval_addr;
    switch (cy_id.target_siid & PSOC6_FAMILY_ID_LO_MSK)
    {
            // PSoC6A-2M family
        case PSOC6A_2M_FAMILY_ID_LO:
            readyval_addr = READY_ADDR_2M;
            break;

            // PSoC6A-512K family
        case PSOC6A_512K_FAMILY_ID_LO:
            readyval_addr = READY_ADDR_512K;
            break;

            // PSoC6-BLE family by default
        default:
            readyval_addr = READY_ADDR_1M;
            break;
    }
    return readyval_addr;
}

// Get Kit HW id
uint8_t get_kit_hw_id(void)
{
    return cy_id.hw_id;
}

// Get Kit unique product ID
uint16_t get_kit_uid(void)
{
    return cy_id.uid;
}
