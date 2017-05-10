/**
 * @file    beetle.c
 * @brief   board ID for the beetle
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2017-2017, ARM Limited, All Rights Reserved
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

#include "flash_intf.h"
#include "target_reset.h"
#include "settings.h"
#include "swd_host.h"
#include "target_config.h"
#include "util.h"

const char *board_id = "5002";

static error_t init(void);
static error_t uninit(void);
static error_t program_page(uint32_t adr, const uint8_t *buf, uint32_t size);
static error_t erase_sector(uint32_t addr);
static error_t erase_chip(void);
static uint32_t program_page_min_size(uint32_t addr);
static uint32_t erase_sector_size(uint32_t addr);

static const flash_intf_t flash_intf = {
    init,
    uninit,
    program_page,
    erase_sector,
    erase_chip,
    program_page_min_size,
    erase_sector_size,
};

const flash_intf_t *const flash_intf_target_custom = &flash_intf;

static error_t init()
{
    if (0 == target_set_state(RESET_PROGRAM)) {
        return ERROR_RESET;
    }

    return ERROR_SUCCESS;
}

static error_t uninit(void)
{
    // Resume the target if configured to do so
    if (config_get_auto_rst()) {
        target_set_state(RESET_RUN);
    }

    swd_off();
    return ERROR_SUCCESS;
}

static error_t program_page(uint32_t addr, const uint8_t *buf, uint32_t size)
{
    // check if security bits were set
    if (1 == security_bits_set(addr, (uint8_t *)buf, size)) {
        return ERROR_SECURITY_BITS;
    }

    // Write data directly to device
    if (!swd_write_memory(addr, (uint8_t *)buf, size)) {
        return ERROR_ALGO_DATA_SEQ;
    }

    return ERROR_SUCCESS;
}

static error_t erase_sector(uint32_t addr)
{
    return ERROR_SUCCESS;
}

static error_t erase_chip(void)
{
    return ERROR_SUCCESS;
}

static uint32_t program_page_min_size(uint32_t addr)
{
    uint32_t size = 256;
    util_assert(target_device.sector_size >= size);
    return size;
}

static uint32_t erase_sector_size(uint32_t addr)
{
    return target_device.sector_size;
}
