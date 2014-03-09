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
#include <LPC11Uxx.h>
#include "vector_table.h"
#include "flash_hal.h"

#define NVIC_NUM_VECTORS (16 + 32)            // CORE + MCU Peripherals
#define NVIC_RAM_VECTOR_ADDRESS (0x10000000)  // Vectors positioned at start of RAM

void relocate_vector_table() {
    int i;
    // Space for dynamic vectors, initialised to allocate in R/W
    static volatile uint32_t * vectors = (uint32_t*)NVIC_RAM_VECTOR_ADDRESS;

    // Copy and switch to dynamic vectors if first time called
    if((LPC_SYSCON->SYSMEMREMAP & 0x3) != 0x1) {
        uint32_t *old_vectors = (uint32_t *)START_APP_ADDRESS;
        for(i = 0; i < NVIC_NUM_VECTORS; i++) {
            vectors[i] = old_vectors[i];
        }
        LPC_SYSCON->SYSMEMREMAP = 0x1;
    }
}
