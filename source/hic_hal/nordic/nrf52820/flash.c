/**
 * @file    flash.c
 * @brief   Flash access functions for nrf52820 HIC
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

#include "target_config.h"
#include "nrf_nvmc.h"

uint32_t Init(uint32_t adr, uint32_t clk, uint32_t fnc)
{
    return 0; // Not needed
}

uint32_t UnInit(uint32_t fnc)
{
    return 0; // Not needed
}

uint32_t EraseChip(void)
{
    return 0; // Not used in DAPLink
}

uint32_t EraseSector(uint32_t adr)
{
    nrf_nvmc_mode_set(NRF_NVMC, NRF_NVMC_MODE_ERASE);
    nrf_nvmc_page_erase_start(NRF_NVMC, adr);
    while (!nrf_nvmc_ready_check(NRF_NVMC)) {
        // Wait for controller to be ready
    }
    nrf_nvmc_mode_set(NRF_NVMC, NRF_NVMC_MODE_READONLY);

    return 0;
}

uint32_t ProgramPage(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    nrf_nvmc_mode_set(NRF_NVMC, NRF_NVMC_MODE_WRITE);
    for (uint32_t i = 0; i < sz / 4; i++) {
        ((volatile uint32_t *)adr)[i] = buf[i];
        while (!nrf_nvmc_ready_check(NRF_NVMC)) {
            // Wait for controller to be ready
        }
    }
    nrf_nvmc_mode_set(NRF_NVMC, NRF_NVMC_MODE_READONLY);

    return 0;
}
