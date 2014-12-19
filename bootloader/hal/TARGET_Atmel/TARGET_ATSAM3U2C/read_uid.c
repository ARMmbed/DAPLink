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
#include "sam3u.h"

void read_unique_id(uint32_t * id) {
    EFC0->EEFC_FMR |= (1UL << 16);
    EFC0->EEFC_FCR = 0x5A00000E;
    /*Monitor FRDY*/
    while ((EFC0->EEFC_FSR & EEFC_FSR_FRDY) == EEFC_FSR_FRDY);

    id[0] = *(uint32_t *)0x80000;
    // patch until uuid is properly implemented for all targets
    //  causes problems in common file version.c
    //  https://github.com/mbedmicro/CMSIS-DAP/commit/be926417b8502eee31d2ef0be6fd37f027f58924#diff-7
    //id[1] = *(uint32_t *)0x80004;
    //id[2] = *(uint32_t *)0x80008;
    //id[3] = *(uint32_t *)0x8000C;

    EFC0->EEFC_FCR = 0x5A00000F;
    /*Monitor FRDY*/
    while ((EFC0->EEFC_FSR & EEFC_FSR_FRDY) != EEFC_FSR_FRDY);
    EFC0->EEFC_FMR &= ~(1UL << 16);
}
