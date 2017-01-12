/**
 * @file    usbd_user_dfu.c
 * @brief   DFU driver for target flashing
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

#include <stdint.h>
#include <stdbool.h>
#include "string.h"
#include "RTL.h"
#include "rl_usb.h"
#include "usb.h"

#include "usbd_lib_dfu.h"

#include "flash_manager.h"
#include "flash_intf.h"
#include "target_config.h"
#include "settings.h"

#include "main.h"
#include "target_reset.h"

static uint32_t current_write_addr;
static bool initialized = false;

// USB DFU Callback: when system initializes
void usbd_dfu_init(void)
{
    DFU_Reset();
    current_write_addr = 0;
}

BOOL USBD_DFU_StartUpgrade(void) {
    error_t err = flash_manager_init(flash_intf_target);
    current_write_addr = target_device.flash_start;
    switch (err) {
        case ERROR_SUCCESS:
            initialized = true;
            break;
        case ERROR_RESET:
        case ERROR_ALGO_DL:
        case ERROR_ALGO_DATA_SEQ:
        case ERROR_INIT:
        case ERROR_SECURITY_BITS:
        case ERROR_UNLOCK:
            DFU_SetStatus(DFU_STATUS_ERR_PROG);
            break;
        case ERROR_ERASE_SECTOR:
        case ERROR_ERASE_ALL:
            DFU_SetStatus(DFU_STATUS_ERR_ERASE);
            break;
        case ERROR_WRITE:
            DFU_SetStatus(DFU_STATUS_ERR_WRITE);
            break;
        case ERROR_FAILURE:
        case ERROR_INTERNAL:
        default:
            DFU_SetStatus(DFU_STATUS_ERR_UNKNOWN);
            break;
    }
    
    return (err == ERROR_SUCCESS) ? (__TRUE) : (__FALSE);
}

static bool reset_target(bool error_condition) {
    current_write_addr = 0;
    if (initialized) {
        error_t err = flash_manager_uninit();
        switch (err) {
            case ERROR_SUCCESS:
                if (config_get_auto_rst()) {
                    // Target is reset and run by the uninit
                } else if (!error_condition) {
                    // Reset and run the target at the end of a successful upgrade
                    target_set_state(RESET_RUN);
                }
                break;
            case ERROR_RESET:
            case ERROR_ALGO_DL:
            case ERROR_ALGO_DATA_SEQ:
            case ERROR_INIT:
            case ERROR_SECURITY_BITS:
            case ERROR_UNLOCK:
                DFU_SetStatus(DFU_STATUS_ERR_PROG);
                break;
            case ERROR_ERASE_SECTOR:
            case ERROR_ERASE_ALL:
                DFU_SetStatus(DFU_STATUS_ERR_ERASE);
                break;
            case ERROR_WRITE:
                DFU_SetStatus(DFU_STATUS_ERR_WRITE);
                break;
            case ERROR_FAILURE:
            case ERROR_INTERNAL:
            default:
                DFU_SetStatus(DFU_STATUS_ERR_UNKNOWN);
                break;
        }
        initialized = false;
        return (err == ERROR_SUCCESS);
    }
    
    return true;
}

BOOL USBD_DFU_FinishUpgrade(void) {
    return reset_target(false) ? (__TRUE) : (__FALSE);
}

BOOL USBD_DFU_WriteBlock(const U8 *buffer, U16 blockSize) {
    error_t err = flash_manager_data(current_write_addr, (U8*)buffer, blockSize);
    switch (err) {
        case ERROR_SUCCESS:
            current_write_addr += blockSize;
            break;
        case ERROR_RESET:
        case ERROR_ALGO_DL:
        case ERROR_ALGO_DATA_SEQ:
        case ERROR_INIT:
        case ERROR_SECURITY_BITS:
        case ERROR_UNLOCK:
            DFU_SetStatus(DFU_STATUS_ERR_PROG);
            break;
        case ERROR_ERASE_SECTOR:
        case ERROR_ERASE_ALL:
            DFU_SetStatus(DFU_STATUS_ERR_ERASE);
            break;
        case ERROR_WRITE:
            DFU_SetStatus(DFU_STATUS_ERR_WRITE);
            break;
        case ERROR_FAILURE:
        case ERROR_INTERNAL:
        default:
            DFU_SetStatus(DFU_STATUS_ERR_UNKNOWN);
            break;
    }
    
    return (err == ERROR_SUCCESS);
}

BOOL USBD_DFU_HandleAbort(BOOL error) {
    if (error) {
        return reset_target(true) ? (__TRUE) : (__FALSE);
    } else {
        return reset_target(false) ? (__TRUE) : (__FALSE);
    }
}

__weak uint32_t target_chip_erase_time(uint32_t chipSize) {
    // Allow 80ms per KiB to erase the chip as a conservative estimate
    return (chipSize + 1023) / 1024 * 80;
}

__weak uint32_t target_chip_program_time(uint16_t blockSize) {
    // Allow for 80ms per KiB for programming as a conservative estimate
    return (blockSize + 1023) / 1024 * 80;
}

U32 USBD_DFU_GetInitTimeout(void) {
    // Allow for 200ms to initialize SWD
    U32 chipSize = target_device.sector_size * target_device.sector_cnt;
    return 200 + target_chip_erase_time(chipSize);
}

U32 USBD_DFU_GetDownloadTimeout(U16 blockSize) {
    return target_chip_program_time(blockSize);
}

U32 USBD_DFU_GetManifestTimeout(void) {
    return 200;
}

