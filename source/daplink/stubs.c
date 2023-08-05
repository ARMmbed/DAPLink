/**
 * @file    stubs.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2017, 2023, ARM Limited, All Rights Reserved
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

#include "device.h"

__WEAK void sdk_init()
{
    // Do nothing
}

/*
 * Work-around for a link-time issue with Arm GNU Toolchain
 * in version 11.3 and later.
 */
#ifdef __GNUC__
#if ((__GNUC__ > 11) || \
     ((__GNUC__ == 11) && (__GNUC_MINOR__ >= 3)))
__WEAK void _close() {}
__WEAK void _fstat() {}
__WEAK void _getpid() {}
__WEAK void _isatty() {}
__WEAK void _kill() {}
__WEAK void _lseek() {}
__WEAK void _read() {}
__WEAK void _write() {}
#endif
#endif
