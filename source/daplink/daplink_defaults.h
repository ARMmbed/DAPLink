/**
 * @file    daplink_defaults.h
 * @brief   Default settings
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2021, Arm Limited, All Rights Reserved
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

#ifndef DAPLINK_DEFAULTS_H
#define DAPLINK_DEFAULTS_H

#ifndef DAPLINK_HEAP_SIZE
#define DAPLINK_HEAP_SIZE 0x0
#endif

#ifndef DAPLINK_STACK_SIZE
#if defined(DAPLINK_BL)
#define DAPLINK_STACK_SIZE 0x800
#elif defined(DAPLINK_IF)
#define DAPLINK_STACK_SIZE 0x200
#endif
#endif

#endif
