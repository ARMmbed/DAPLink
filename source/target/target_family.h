/**
 * @file    target_family.h
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

#ifndef TARGET_FAMILY_H
#define TARGET_FAMILY_H

#include "stdint.h"
#include "swd_host.h"

typedef enum _reset_type { 
    kHardwareReset=1, 
    kSoftwareReset, 
} reset_type_t; 
 
typedef struct target_family_descriptor { 
    uint32_t family_id; 
    reset_type_t default_reset_type; 
    uint32_t soft_reset_type; 
    void (*target_before_init_debug)(void); 
    void (*prerun_target_config)(void); 
    uint8_t (*target_unlock_sequence)(void); 
    uint8_t (*security_bits_set)(uint32_t addr, uint8_t *data, uint32_t size); 
    uint8_t (*target_set_state)(TARGET_RESET_STATE state); 
    void (*swd_set_target_reset)(uint8_t asserted);
} target_family_descriptor_t;


#define STUB_HW_RESET_FAMILY_ID         0x01
#define STUB_SW_VECTRESET_FAMILY_ID     0x02
#define STUB_SW_SYSRESETREQ_FAMILY_ID   0x03
#define NXP_KINETIS_K_SERIES_FAMILY_ID  0x11
#define NXP_KINETIS_L_SERIES_FAMILY_ID  0x12
#define NXP_MIMXRT_FAMILY_ID            0x13
#define NXP_RAPID_IOT_FAMILY_ID         0x14
#define NORDIC_NRF51_FAMILY_ID          0x21
#define NORDIC_NRF52_FAMILY_ID          0x22
#define REALTEK_RTL8195AM_FAMILY_ID     0x31
#define TI_FAMILY_ID                    0x41
#define TOSHIBA_TZ_FAMILY_ID            0x51
#define WIZNET_FAMILY_ID                0x61

//extern const target_family_descriptor_t g_nxp_kinetis_kseries;

extern const target_family_descriptor_t *g_target_family;

void init_family(void);
uint8_t target_family_valid(void);
uint8_t target_set_state(TARGET_RESET_STATE state);
void swd_set_target_reset(uint8_t asserted);

#endif
