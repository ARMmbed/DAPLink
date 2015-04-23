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
#include "read_uid.h"

#define IAP_LOCATION 0x1fff1ff1
static uint32_t command[5];
static uint32_t result[5];
typedef void (*IAP)(uint32_t[], uint32_t[]);
static IAP iap_entry;


void read_unique_id(uint32_t * id) {
    // readUID IAP call
    iap_entry = (IAP) IAP_LOCATION;
    command[0] = 58;
    iap_entry (command, result);
    *id = result[1] ^ result[2] ^ result[3] ^ result[4];
}
