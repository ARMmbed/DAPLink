/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef DAPLINK_H
#define DAPLINK_H

#include <stdbool.h>

#define DAPLINK_BUILD_KEY_IF        0x9B939699
#define DAPLINK_BUILD_KEY_BL        0x9B939D93

#define DAPLINK_HDK_ID_K20DX        0x646c0000
#define DAPLINK_HDK_ID_KL26         0x646c0001
#define DAPLINK_HDK_ID_LPC11U35     0x646c0002

bool daplink_is_bootloader(void);
bool daplink_is_interface(void);

#endif
