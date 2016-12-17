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

#include "main.h"
#include "target_reset.h"

static volatile uint32_t current_write_addr;
static volatile U8* buffer_addr;
static volatile U16 transfer_size;

// USB DFU Callback: when system initializes
void usbd_dfu_init(void)
{
    DFU_Reset();
    current_write_addr = 0;
}

/* DFU Messages */
typedef enum {
    DFU_MSG_INIT_TARGET,
    DFU_MSG_READ_BLOCK,
    DFU_MSG_WRITE_BLOCK,
    DFU_MSG_RESET_TARGET,
    DFU_MSG_ABORT,
    DFU_MSG_ERROR,
} DFU_MSG;

os_mbx_declare(dfu_mailbox, 4);

BOOL USBD_DFU_StartUpgrade(void)
{
    os_mbx_send(&dfu_mailbox, (void *)DFU_MSG_INIT_TARGET, 0);
    return (__TRUE);
}

BOOL USBD_DFU_FinishUpgrade(void) {
    os_mbx_send(&dfu_mailbox, (void *)DFU_MSG_RESET_TARGET, 0);
    return (__TRUE);
}

BOOL USBD_DFU_WriteBlock(const U8 *buffer, U16 blockSize) {
    buffer_addr = (U8*)buffer;
    transfer_size = blockSize;
    os_mbx_send(&dfu_mailbox, (void *)DFU_MSG_WRITE_BLOCK, 0);
    return (__TRUE);
}

void USBD_DFU_SignalAbort(BOOL error) {
    if (error) {
        os_mbx_send(&dfu_mailbox, (void *)DFU_MSG_ERROR, 0);
    } else {
        os_mbx_send(&dfu_mailbox, (void *)DFU_MSG_ABORT, 0);
    }
}

__task void dfu_process(void)
{
    void *msg;
    static BOOL initialized = __FALSE;
    while (1) {
        // Wait for a request
        if (os_mbx_wait(&dfu_mailbox, &msg, 0xFFFF) != OS_R_TMO) {
            error_t err;
            BOOL error_condition = __FALSE;
            switch ((DFU_MSG)(unsigned)msg) {
                case DFU_MSG_INIT_TARGET:
                    err = flash_manager_init(flash_intf_target);
                    current_write_addr = target_device.flash_start;
                    switch (err) {
                        case ERROR_SUCCESS:
                            initialized = __TRUE;
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

                    break;

                case DFU_MSG_WRITE_BLOCK:
                    err = flash_manager_data(current_write_addr, (U8*)buffer_addr, transfer_size);
                    switch (err) {
                        case ERROR_SUCCESS:
                            current_write_addr += transfer_size;
                            DFU_SetState(DFU_STATE_DFU_DNLOAD_IDLE);
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
                    break;

                case DFU_MSG_ERROR:
                    error_condition = __TRUE;
                    // Fall through
                case DFU_MSG_ABORT:
                    if (!initialized) {
                        break;
                    } else {
                        // Fall through
                    }
                case DFU_MSG_RESET_TARGET:
                    initialized = __FALSE;
                    current_write_addr = 0;
                    err = flash_manager_uninit();
                    switch (err) {
                        case ERROR_SUCCESS:
                            if (!error_condition) {
                                DFU_SetState(DFU_STATE_DFU_IDLE);
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
                    break;

                default:
                    break;
            }
        }
    }
}
