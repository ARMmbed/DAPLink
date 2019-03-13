/**
 * @file    target_family.h
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

#ifndef TARGET_FAMILY_H
#define TARGET_FAMILY_H

#include <stdint.h>
#include <string.h>
#include "target_reset.h"

#define VENDOR_TO_FAMILY(x, y) (x<<8 | y)

typedef enum _reset_type { 
    kHardwareReset=1, 
    kSoftwareReset, 
} reset_type_t; 

enum _vendor_ids {
    kStub_VendorID = 0,
    kNXP_VendorID = 11,
    kTI_VendorID = 16,
    kNordic_VendorID = 54,
    kToshiba_VendorID = 92,
    kRenesas_VendorID = 117,
    kWiznet_VendorID = 122,
    kRealtek_VendorID = 124,
};

typedef enum _family_id {
    kStub_HWReset_FamilyID = VENDOR_TO_FAMILY(kStub_VendorID, 1),
    kStub_SWVectReset_FamilyID = VENDOR_TO_FAMILY(kStub_VendorID, 2),
    kStub_SWSysReset_FamilyID = VENDOR_TO_FAMILY(kStub_VendorID, 3),
    kNXP_KinetisK_FamilyID = VENDOR_TO_FAMILY(kNXP_VendorID, 1),
    kNXP_KinetisL_FamilyID = VENDOR_TO_FAMILY(kNXP_VendorID, 2),
    kNXP_Mimxrt_FamilyID = VENDOR_TO_FAMILY(kNXP_VendorID, 3),
    kNXP_RapidIot_FamilyID = VENDOR_TO_FAMILY(kNXP_VendorID, 4),
    kNXP_KinetisK32W_FamilyID = VENDOR_TO_FAMILY(kNXP_VendorID, 5),
    kNordic_Nrf51_FamilyID = VENDOR_TO_FAMILY(kNordic_VendorID, 1),
    kNordic_Nrf52_FamilyID = VENDOR_TO_FAMILY(kNordic_VendorID, 2),
    kRealtek_Rtl8195am_FamilyID = VENDOR_TO_FAMILY(kRealtek_VendorID, 1),
    kTI_Cc3220sf_FamilyID = VENDOR_TO_FAMILY(kTI_VendorID, 1),
    kToshiba_Tz_FamilyID = VENDOR_TO_FAMILY(kToshiba_VendorID, 1),
    kWiznet_W7500_FamilyID = VENDOR_TO_FAMILY(kWiznet_VendorID, 1),
    kRenesas_FamilyID = VENDOR_TO_FAMILY(kRenesas_VendorID, 1),
} family_id_t;
 
typedef struct target_family_descriptor { 
    uint16_t family_id;                         /*!< Use to select or identify target family from defined target family or custom ones */ 
    reset_type_t default_reset_type;            /*!< Target family can select predefined reset from  kHardwareReset and kSoftwareReset */ 
    uint32_t soft_reset_type;                   /*!< Families can override software reset type to VECTRESET or SYSRESETREQ */
    void (*target_before_init_debug)(void);     /*!< Target dependant function before debug initialization */
    void (*prerun_target_config)(void);         /*!< Target specific initialization */
    uint8_t (*target_unlock_sequence)(void);    /*!< Unlock targets that can enter lock state */
    uint8_t (*security_bits_set)(uint32_t addr, uint8_t *data, uint32_t size);  /*!< Check security bits in the programmable flash region */
    uint8_t (*target_set_state)(TARGET_RESET_STATE state);                      /*!< Families can customize target debug states in target_reset.h */
    void (*swd_set_target_reset)(uint8_t asserted);                             /*!< Families can customize how to send reset to the target */
    uint8_t (*validate_bin_nvic)(const uint8_t *buf);                           /*!< Validate a bin file to be flash by drag and drop */
    uint8_t (*validate_hexfile)(const uint8_t *buf);                            /*!< Validate a hex file to be flash by drag and drop */
} target_family_descriptor_t;

extern const target_family_descriptor_t *g_target_family;

#ifdef __cplusplus
extern "C" {
#endif

void init_family(void);
uint8_t target_family_valid(void);
uint8_t target_set_state(TARGET_RESET_STATE state);
void swd_set_target_reset(uint8_t asserted);

#ifdef __cplusplus
}
#endif

#endif
