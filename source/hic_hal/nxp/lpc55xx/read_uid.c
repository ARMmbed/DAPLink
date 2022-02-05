/**
 * @file    read_uid.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2020 Arm Limited, All Rights Reserved
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

#include <string.h>
#include "read_uid.h"

//! The RFC4122-compliant UUID resides in the Protected Flash Region.
#define UUID_ADDR (0x0009FC70)

#define UUID_LENGTH (16)

void read_unique_id(uint32_t *id)
{
    memcpy(id, (void *)UUID_ADDR, UUID_LENGTH);
}
