/**
 * @file    flash_hal_tmpm366.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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
#include "target_config.h"    // target_device
#include "cortex_m.h"

#define M32(adr)            (*((volatile unsigned long  *)(adr)))
#define PAGE_SIZE           512      // Page Size
// Watchdog Timer Registers
#define WDMOD               (*((volatile unsigned char  *)0x400F2000))
#define WDCR                (*((volatile unsigned char  *)0x400F2004))
// Flash Control/Status Register
#define FLCSF0              (*((volatile unsigned long  *)0x41FFF020))
#define FLCS_RDY            0x00000001
#define FLASH_START_ADDR    ((uint32_t)0x00000000) /* User Boot Mode As Default */

__attribute__((section("ram_func")))
uint32_t Init(uint32_t adr, uint32_t clk, uint32_t fnc)
{
    // Disable Watchdog
    WDMOD = 0x00;
    WDCR  = 0xB1;

    return (0);
}

__attribute__((section("ram_func")))
uint32_t UnInit(uint32_t fnc)
{
    return (0);
}


__attribute__((section("ram_func")))
uint32_t EraseChip(void)
{
    cortex_int_state_t state = cortex_int_get_and_disable();
    // Erase the contents of the entire chip
    // Start Chip Erase Command
    M32(FLASH_START_ADDR + 0x5400) = 0xAA;
    M32(FLASH_START_ADDR + 0xAA00) = 0x55;
    M32(FLASH_START_ADDR + 0x5400) = 0x80;
    M32(FLASH_START_ADDR + 0x5400) = 0xAA;
    M32(FLASH_START_ADDR + 0xAA00) = 0x55;
    M32(FLASH_START_ADDR + 0x5400) = 0x10;
    __DSB();

    while ((FLCSF0 & FLCS_RDY) != FLCS_RDY);     // Wait until completed

    cortex_int_restore(state);
    
    return (0);
}

__attribute__((section("ram_func")))
uint32_t EraseSector(uint32_t adr)
{
    cortex_int_state_t state = cortex_int_get_and_disable();

    // Start Block Erase Command
    M32(FLASH_START_ADDR + 0x5400) = 0xAA;
    M32(FLASH_START_ADDR + 0xAA00) = 0x55;
    M32(FLASH_START_ADDR + 0x5400) = 0x80;
    M32(FLASH_START_ADDR + 0x5400) = 0xAA;
    M32(FLASH_START_ADDR + 0xAA00) = 0x55;

    M32(adr) = 0x30;
    __DSB();

    while ((FLCSF0 & FLCS_RDY) != FLCS_RDY);     // Wait until completed

    cortex_int_restore(state);
    
    return (0);  // O.K.
}

__attribute__((section("ram_func")))
uint32_t ProgramPage(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    uint32_t *source;
    uint32_t *temp;
    uint32_t *dest;
    volatile uint32_t *addr = (uint32_t *) dest;
    int i = 0;
    dest   = (uint32_t *)adr;
    source = buf;

    cortex_int_state_t state = cortex_int_get_and_disable();
    while (sz > PAGE_SIZE) {
        addr = (uint32_t *) dest;
        temp = source;   
        // Start Page Programming Command
        M32(FLASH_START_ADDR + 0x5400) = 0xAA;
        M32(FLASH_START_ADDR + 0xAA00) = 0x55;
        M32(FLASH_START_ADDR + 0x5400) = 0xA0;

        // Write Page Data
        for (i = 0; i < (PAGE_SIZE / 4); i++) {
            *addr = *temp;
            temp++;
        }
        __DSB();
        while ((FLCSF0 & FLCS_RDY) != FLCS_RDY);     // Wait until completed      
        
        sz = sz - PAGE_SIZE;
        dest = dest + PAGE_SIZE / 4U;
        source = source + PAGE_SIZE / 4U;
    }
    addr = (uint32_t *) dest;
    temp = source;   
    // Start Page Programming Command
    M32(FLASH_START_ADDR + 0x5400) = 0xAA;
    M32(FLASH_START_ADDR + 0xAA00) = 0x55;
    M32(FLASH_START_ADDR + 0x5400) = 0xA0;

    // Write Page Data
    for (i = 0; i < (PAGE_SIZE/4); i++) {
        *addr = *temp;
        temp++;
    }
    __DSB();

    while ((FLCSF0 & FLCS_RDY) != FLCS_RDY);     // Wait until completed
    
    cortex_int_restore(state);

    return 0;
}
