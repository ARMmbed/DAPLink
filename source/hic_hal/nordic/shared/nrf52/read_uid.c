/**
 * @file    read_uid.c
 * @brief   UID extraction function for nrf52820 HIC
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

#include "read_uid.h"
#include "device.h"

void read_unique_id(uint32_t *id)
{
    id[0] = NRF_FICR->INFO.PART;
    id[1] = NRF_FICR->DEVICEID[0];
    id[2] = NRF_FICR->DEVICEID[1];
    id[3] = 0;
}
