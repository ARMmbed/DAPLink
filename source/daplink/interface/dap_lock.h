/**
 * @file    swd_host.h
 * @brief   Host driver for accessing the DAP
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
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

#ifndef DAPLOCK_H
#define DAPLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum dap_lock_operation_t {
	DAP_LOCK_OPERATION_NONE = 0,
	DAP_LOCK_OPERATION_HIC_INIT,
	DAP_LOCK_OPERATION_CDC_BREAK,
	DAP_LOCK_OPERATION_HID_DEBUG,
	DAP_LOCK_OPERATION_UMS_FLASH,
} dap_lock_operation_t;

uint8_t dap_lock_mutex_init(void);
uint8_t dap_lock_tid(OS_TID tid);
uint8_t dap_lock_tid_self(void);
uint8_t dap_lock_verify_tid(OS_TID tid);
uint8_t dap_lock_verify_tid_self(void);
uint8_t dap_lock_operation(dap_lock_operation_t operation);
uint8_t dap_lock_verify_operation(dap_lock_operation_t operation);
uint8_t dap_unlock(void);
uint8_t dap_unlock_force(void);
uint8_t dap_unlock_operation(dap_lock_operation_t operation);

#ifdef __cplusplus
}
#endif



#endif
