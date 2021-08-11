/**
 * @file    tasks.h
 * @brief   Macros for configuring the run time tasks
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2020, ARM Limited, All Rights Reserved
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

#ifndef TASK_H
#define TASK_H

// trouble here is that reset for different targets is implemented differently so all targets
//  have to use the largest stack or these have to be defined in multiple places... Not ideal
//  may want to move away from threads for some of these behaviours to optimize mempory usage (RAM)
#ifndef MAIN_TASK_STACK
#define MAIN_TASK_STACK     (864)
#endif
#define MAIN_TASK_PRIORITY  (osPriorityNormal)

#endif
