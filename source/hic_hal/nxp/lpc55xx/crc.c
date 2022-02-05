/*
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

#include "crc.h"
#include "fsl_crc.h"
#include "flash_hal.h"
#include "daplink_addr.h"
#include "util.h"

#define TEST_CHECK_VALUE (0)
#define CHECK_VALUE     (0xCBF43926)

#define INITIAL_SEED    (0xFFFFFFFF)

static void crc_checked_write_data(const void *data, int count);
static void crc_write_data_safely(const void *data, int count);

//! Initialize the CRC engine peripheral for CRC-32.
void hic_crc_init(void)
{
    crc_config_t config = {
            .polynomial = kCRC_Polynomial_CRC_32,
            .reverseIn = true,
            .complementIn = false,
            .reverseOut = true,
            .complementOut = true,
            .seed = INITIAL_SEED,
        };
    CRC_Init(CRC_ENGINE, &config);

#if TEST_CHECK_VALUE
    uint32_t sum = crc32("123456789", 9);
    if (sum != CHECK_VALUE) {
        __BKPT(0);
    }

    sum = crc32("1234", 4);
    sum = crc32_continue(sum, "56789", 5);
    if (sum != CHECK_VALUE) {
        __BKPT(0);
    }
#endif
}

//! Test readability of the given buffer. Then either write actual data to the CRC engine, or
//! write fake erased flash data.
void crc_checked_write_data(const void *data, int count)
{
    if (flash_is_readable((uint32_t)data, count)) {
        CRC_WriteData(CRC_ENGINE, data, count);
    }
    else {
        // Write full words.
        while (count >= sizeof(uint32_t)) {
            CRC_ENGINE->WR_DATA = 0xFFFFFFFF;
            count -= sizeof(uint32_t);
        }

        // Write any trailing bytes.
        while (count) {
            *((__O uint8_t *)&(CRC_ENGINE->WR_DATA)) = 0xFF;
            --count;
        }
    }
}

//! Pass data to the CRC engine while ensuring that we don't attempt to read from an erased
//! flash sector.
void crc_write_data_safely(const void *data, int count)
{
    // Skip readability checks if the data is not in flash.
    if (!((uint32_t)data >= DAPLINK_ROM_START && (uint32_t)data < (DAPLINK_ROM_START + DAPLINK_ROM_SIZE))) {
        CRC_WriteData(CRC_ENGINE, data, count);
        return;
    }

    // Check leading ragged edge.
    uint32_t n = ROUND_UP((uint32_t)data, DAPLINK_SECTOR_SIZE) - (uint32_t)data;
    if (n) {
        crc_checked_write_data(data, n);
        data += n;
        count -= n;
    }

    // Write sector-size chuncks as they are checked.
    while (count >= DAPLINK_SECTOR_SIZE) {
        n = MIN(count, DAPLINK_SECTOR_SIZE);
        crc_checked_write_data(data, n);
        data += n;
        count -= n;
    }

    // Check trailing ragged edge.
    if (count) {
        crc_checked_write_data(data, count);
        data += count;
        count -= count;
    }
}

uint32_t crc32(const void *data, int nBytes)
{
    // Start new CRC with the initial seed, then write the data through the engine.
    CRC_ENGINE->SEED = INITIAL_SEED;
    crc_write_data_safely(data, nBytes);

    return CRC_ENGINE->SUM;
}

uint32_t crc32_continue(uint32_t prev_crc, const void *data, int nBytes)
{
    // The previous CRC has been reversed and complemented when read. When writing the SEED register, the
    // value will be reversed/complemented according to the MODE input settings. We need to temporarily modify
    // the MODE to convert the CRC into a seed. CRC-32 input reverses the data, which the previous CRC has
    // already been, but does not complement, though the previous CRC has been. So we need to disable reverse
    // and enable complement.
    uint32_t mode = CRC_ENGINE->MODE;
    CRC_ENGINE->MODE = mode & ~(CRC_MODE_BIT_RVS_WR_MASK | CRC_MODE_CMPL_WR_MASK);

    // Undo post-processing.
    prev_crc = __RBIT(prev_crc ^ 0xFFFFFFFF);

    // Continue with the previous CRC, then write the data through the engine.
    CRC_ENGINE->SEED = prev_crc;

    // Restore original CRC sum bit reverse and 1's complement setting.
    CRC_ENGINE->MODE = mode;

    crc_write_data_safely(data, nBytes);
    return CRC_ENGINE->SUM;
}


