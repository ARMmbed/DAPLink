/*******************************************************************************
* @file PSOC6xxx.h
*
* @brief
*  This file provides constants and function declaration for PSoC6xxx family
*  device acquiring, silicon id reading.
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

#if !defined(PSOC6XXX_H)
#define PSOC6XXX_H

#include "CyLib.h"
#include "target_family.h"

// Acquire timeout in Timer_CSTick ticks (8Hz rate, 8 ticks == 1000ms)
#define TIMER_CSTICK_PER_SECOND                 (8u)
#define DETECTION_AQUIRE_TIMEOUT_SECONDS        (30u)
#define DETECTION_AQUIRE_TIMEOUT_TICKS          (DETECTION_AQUIRE_TIMEOUT_SECONDS*TIMER_CSTICK_PER_SECOND)
#define PROGRAM_AQUIRE_TIMEOUT_SECONDS          (15u)
#define PROGRAM_AQUIRE_TIMEOUT_TICKS            (PROGRAM_AQUIRE_TIMEOUT_SECONDS*TIMER_CSTICK_PER_SECOND)

// Data shift
#define SHIFT_24                                 (24u)
#define SHIFT_16                                 (16u)
#define SHIFT_8                                  (8u)

// CY8C6xx6, CY8C6XX7 devices
#define MEM_BASE_IPC_1M                          (0x40230000u)
#define IPC_STRUCT0_1M                            MEM_BASE_IPC_1M
#define IPC_STRUCT1_1M                           (IPC_STRUCT0_1M + IPC_STRUCT_SIZE)
#define IPC_STRUCT2_1M                           (IPC_STRUCT1_1M + IPC_STRUCT_SIZE)
#define IPC_INTR_STRUCT_1M                       (0x40231000u)
#define IPC_STRUCT_LOCK_STATUS_OFFSET_1M         (0x10u)
#define IPC_STRUCT_DATA_1M                       (IPC_STRUCT2_1M + IPC_STRUCT_DATA_OFFSET)
#define IPC_STRUCT2_NOTIFY_DATA_1M               (IPC_STRUCT2_1M + IPC_STRUCT_NOTIFY_OFFSET)
#define IPC_INTR_STRUCT_DATA_1M                  (IPC_INTR_STRUCT_1M + IPC_INTR_STRUCT_INTR_IPC_MASK_OFFSET)

// CY8C6xx8, CY8C6XXA devices
#define MEM_BASE_IPC_2M                          (0x40220000u)
#define IPC_STRUCT0_2M                            MEM_BASE_IPC_2M
#define IPC_STRUCT1_2M                           (IPC_STRUCT0_2M + IPC_STRUCT_SIZE)
#define IPC_STRUCT2_2M                           (IPC_STRUCT1_2M + IPC_STRUCT_SIZE)
#define IPC_INTR_STRUCT_2M                       (0x40221000u)
#define IPC_STRUCT_LOCK_STATUS_OFFSET_2M         (0x1Cu)
#define IPC_STRUCT_DATA_2M                       (IPC_STRUCT2_2M + IPC_STRUCT_DATA_OFFSET)
#define IPC_STRUCT2_NOTIFY_DATA_2M               (IPC_STRUCT2_2M + IPC_STRUCT_NOTIFY_OFFSET)
#define IPC_INTR_STRUCT_DATA_2M                  (IPC_INTR_STRUCT_2M + IPC_INTR_STRUCT_INTR_IPC_MASK_OFFSET)

// IPC strustures definitions fro PSoC6 SROM API usage
#define MXS40_SROMAPI_SILID_CODE                 (0x00000001u)
#define PSOC6_SROM_SILID_CODE_LO                 (0x00000000u)
#define PSOC6_SROM_SILID_CODE_HI                 (0x00000100u)
#define IPC_2                                    (2u)
#define IPC_STRUCT_SIZE                          (0x20u)
#define IPC_STRUCT_ACQUIRE_TIMEOUT               (5000u)
#define MXS40_SROMAPI_DATA_LOCATION_MSK          (0x00000001u)
#define SRAM_SCRATCH_ADDR                        (0x08000400u)
#define IPC_INTR_STRUCT_INTR_IPC_MASK_OFFSET     (0x08u)
#define IPC_INTR_STRUCT_DATA_VALUE               (0x00040000u)
#define IPC_STRUCT_DATA_OFFSET                   (0x0Cu)
#define IPC_STRUCT_NOTIFY_OFFSET                 (0x08u)
#define IPC_STRUCT_DATA_TIMEOUT                  (9000u)
#define IPC_STRUCT_ACQUIRE_OFFSET                (0x00u)
#define IPC_STRUCT_ACQUIRE_SUCCESS_MSK           (0x80000000u)
#define IPC_STRUCT_LOCK_STATUS_ACQUIRED_MSK      (0x80000000u)
#define SROM_STATUS_SUCCESS_MASK                 (0xF0000000u)
#define MXS40_SROMAPI_STAT_SUCCESS               (0xA0000000u)

// Masks for Silicond ID parts for PSoC6
#define PSOC6_FAMILY_ID_HI_MSK                   (0x0000FF00u)
#define PSOC6_FAMILY_ID_LO_MSK                   (0x000000FFu)
#define PSOC6_REV_ID_MAJ_MSK                     (0x00F00000u)
#define PSOC6_REV_ID_MIN_MSK                     (0x000F0000u)
#define PSOC6_SILICON_ID_LO_MSK                  (0x000000FFu)
#define PSOC6_SILICON_ID_HI_MSK                  (0x0000FF00u)
#define PSOC6_SILICON_ID_MASK                    (0xFFFF00FFu)

// Family ID for PSoC6xxx targets
#define PSOC6A_BLE2_FAMILY_ID                    (0x100u)
#define PSOC6A_2M_FAMILY_ID                      (0x102u)
#define PSoC6A_512K_FAMILY_ID                    (0x105u)
#define MXS28PLAYER_FAMILY_ID                    (0x10Au)

#define PSOC6_FAMILY_ID_HI                       (0x01u)

#define PSOC6A_BLE2_FAMILY_ID_LO                 (PSOC6A_BLE2_FAMILY_ID & PSOC6_FAMILY_ID_LO_MSK)
#define PSOC6A_2M_FAMILY_ID_LO                   (PSOC6A_2M_FAMILY_ID   & PSOC6_FAMILY_ID_LO_MSK)
#define PSOC6A_512K_FAMILY_ID_LO                 (PSoC6A_512K_FAMILY_ID & PSOC6_FAMILY_ID_LO_MSK)
#define MXS28PLAYER_FAMILY_ID_LO                 (MXS28PLAYER_FAMILY_ID & PSOC6_FAMILY_ID_LO_MSK)

// Acquire modes
#define RESET_ACQUIRE       (0x00u)
#define POWER_ACQUIRE       (0x01u)
#define NOTESTBIT_ACQUIRE   (0x02u)

// Test mode defines
#define TEST_MODE_REGISTER  (0x40260100u)
#define TEST_MODE_VALUE     (0x80000000u)

// Flash related difines
#define PSOC6_SROM_START_ADDR                    (0x00000000u)
#define PSOC6_MAIN_FLASH_START_ADDR              (0x10000000u)
#define PSOC6_WFLASH_START_ADDR                  (0x14000000u)
#define PSOC6_SFLASH_START_ADDR                  (0x16000000u)
#define PSOC6_XIP_START_ADDR                     (0x18000000u)

#define PSOC6_SRAM_START_ADDR                    (0x08000000u)

extern target_family_descriptor_t g_target_family_psoc6;
extern uint16_t familyID;
extern uint16_t acquireTimeout;
extern uint16_t SWDDelayedAppearance;
extern uint8_t lastDetectionStep;
extern uint8_t highestDetectionStep;

void DetectionStepClear(void);
uint32_t get_silcon_id(void);
void  init_flash_algo(uint32_t si_id);
uint32_t get_fb_version(void);
uint8_t SWDAcquire(uint8_t acquireMode);
void prerun_target_config(void);
uint32_t get_readyval_addr(void);
uint16_t get_kit_uid(void);

#endif /* PSOC6XXX_H */
