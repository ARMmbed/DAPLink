/**
 * @file    dap_lock.c
 * @brief   Implementation of swd_host lock in multithreaded environment.
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
 *
 *
 * Locking DAP prevents concurrent thread access to SWD/JTAG DAP operations.
 * Lock may be assigned to Task (prevents concurrency) and to mark Operation.
 * Task Lock is tied to OS and has precedence over user set Operation Lock.
 * All underlying DAP/SWD/JTAG operations should be protected internally with
 * provided lock verify mechanisms (TID based) that tells if DAP is available.
 * By setting Operation Lock user also locks DAP for a given TID. No other TID
 * can access DAP in that time, nor other Operation Lock can be set or clear,
 * until ongoing Operation is unlocked from an originating owner thread/task. 
 * Unlocking the Operation makes DAP again available for use by anyone.
 * Functions return 1 for OK/GO/READY, 0 for ERROR/BUSY/FAIL.
 * You need to initialize mutex by hand before any function can be called!
 *  
 */

#include "swd_host.h"

static OS_MUT _dap_lock_mutex;
static OS_TID _dap_lock_tid;
static dap_lock_operation_t _dap_lock_operation;

/**
 * You need to call this function first to initialize mutex.
 */
uint8_t dap_lock_mutex_init(void)
{
	rt_mut_init(&_dap_lock_mutex);
	return 1;
}

/**
 * Lock DAP for a given TID, with prior verification if applicable.
 * @tid is the thread id that will get exclusive DAP access.
 * @return 1 when TID has/gets the lock, 0 otherwise.
 */
uint8_t dap_lock_tid(OS_TID tid)
{
	uint8_t locked = 0;
	os_mut_wait(&_dap_lock_mutex, 0xFFFF);
	if (dap_lock_verify_tid(tid))
	{
		_dap_lock_tid = tid;
		locked = 1;
	}
	util_assert(locked);
	os_mut_release(&_dap_lock_mutex);
	return locked;
}

/**
 * Lock DAP for a caller's thread. TID is obtained from the OS.
 * @return 1 when TID has/gets the lock, 0 otherwise.
 */
uint8_t dap_lock_tid_self(void)
{
	uint8_t locked = 0;
	os_mut_wait(&_dap_lock_mutex, 0xFFFF);
	locked = dap_lock_tid(os_tsk_self());
	os_mut_release(&_dap_lock_mutex);
	return locked;
}

/**
 * Verify if given TID can access the DAP (free or already owned).
 * @tid is the thread id that asks for exclusive DAP access.
 * @return 1 when TID can access DAP, 0 otherwise.
 */
uint8_t dap_lock_verify_tid(OS_TID tid)
{
	uint8_t ready = 0;
	os_mut_wait(&_dap_lock_mutex, 0xFFFF);
	// Check if given TID has already the port lock.
	ready = (tid == _dap_lock_tid ? 1 : 0);
	// Or port is free to use.
	ready |= (0 == _dap_lock_tid ? 1 : 0);
	os_mut_release(&_dap_lock_mutex);
	return ready;
}

/**
 * Verify if caller's thread can access the DAP (free or already owned).
 * TID is obtained from the OS.
 * @return 1 when caller's thread can access DAP, 0 otherwise.
 */
uint8_t dap_lock_verify_tid_self(void)
{
	uint8_t ready = 0;
	os_mut_wait(&_dap_lock_mutex, 0xFFFF);
	ready = (dap_lock_verify_tid(os_tsk_self()));
	os_mut_release(&_dap_lock_mutex);
	return ready;
}

/**
 * Mark DAP lock for a given Operation under caller's thread.
 * Verification is first performed if lock can take place.
 * @operation is the operation code to assign to DAP.
 * @return 1 when operation is/can be assigned, 0 otherwise.
 */
uint8_t dap_lock_operation(dap_lock_operation_t operation)
{
	uint8_t locked = 0;
	os_mut_wait(&_dap_lock_mutex, 0xFFFF);
	if (dap_lock_verify_operation(operation))
	{
		_dap_lock_operation = operation;
		locked = 1;
	}
	util_assert(locked);
	os_mut_release(&_dap_lock_mutex);
	return locked;
}

/**
 * Verify DAP against existing/possible operation assignment.
 * @operation is the operation code to verify on DAP.
 * @return 1 when operation is/can be assigned, 0 otherwise.
 */
uint8_t dap_lock_verify_operation(dap_lock_operation_t operation)
{
	uint8_t ready = 0;
	os_mut_wait(&_dap_lock_mutex, 0xFFFF);
	// Check if operation already ongoing or port free.
	ready = (operation == _dap_lock_operation ? 1 : 0);
	ready |= ( DAP_LOCK_OPERATION_NONE == _dap_lock_operation ? 1 : 0);
	// Also verify if TID can use the port.
	ready &= dap_lock_verify_tid_self();
	os_mut_release(&_dap_lock_mutex);
	return ready;
}

/**
 * Mark DAP as free to use, if called from thread that locked the port.
 * @return 1 when DAP gets unlocked for use by anyone, 0 otherwise.
 */
uint8_t dap_unlock(void)
{
	uint8_t unlocked = 0;
	os_mut_wait(&_dap_lock_mutex, 0xFFFF);
	if (dap_lock_verify_tid_self())
	{
		dap_unlock_force();
		unlocked = 1;
	}
	os_mut_release(&_dap_lock_mutex);
	util_assert(unlocked);
	return unlocked;
}

/**
 * Clear all DAP Locks with no verification.
 */
uint8_t dap_unlock_force(void)
{
	os_mut_wait(&_dap_lock_mutex, 0xFFFF);
	_dap_lock_tid = 0;
	_dap_lock_operation = DAP_LOCK_OPERATION_NONE;
	os_mut_release(&_dap_lock_mutex);
	return 1;
}

/**
 * Check and mark finished Operation on DAP.
 * @operation is the finished Operation code.
 * @return 1 when DAP gets unlocked, 0 otherwise.
 */
uint8_t dap_unlock_operation(dap_lock_operation_t operation)
{
	uint8_t unlocked = 0;
	os_mut_wait(&_dap_lock_mutex, 0xFFFF);
	if (dap_lock_verify_operation(operation))
	{
		unlocked = dap_unlock();
	}
	os_mut_release(&_dap_lock_mutex);
	return unlocked;
}

//
// Below are CMSIS-DAP handler wrappers specific to DAPLink.
//
uint8_t DAP_Lock_Verify (void *param)
{
	return dap_lock_verify_tid_self();
}

uint8_t DAP_Lock (void *param)
{
	return dap_lock_operation(DAP_LOCK_OPERATION_HID_DEBUG);
}

uint8_t DAP_Unlock (void *param)
{
	return dap_unlock_operation(DAP_LOCK_OPERATION_HID_DEBUG);
}
