/**
 * @file    read_uid.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2018, ARM Limited, All Rights Reserved
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

#include "read_uid.h"
#include "util.h"
#include "stm32f0xx.h"

typedef struct
{
    uint16_t fwid;
    uint16_t hwid;
    uint32_t devid_l;
    uint32_t devid_h;
} UID_TypeDef;

#define UID ((UID_TypeDef *) UID_BASE)

void read_unique_id(uint32_t *id)
{
    util_assert(id != NULL);

    id[0] = UID->fwid;
    id[1] = UID->hwid;
    id[2] = UID->devid_l;
    id[3] = UID->devid_h;
}

void create_unique_id(void)
{
    util_assert(false);
}
