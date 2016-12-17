/**
 * @file    nrf5x.c
 * @brief   board ID for the Nordic nRF5x developments boards
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
 
#include "sam3u2c.h"
#include "target_config.h"

const char *board_id = "";

// BOARD_ID pointer will be set during run time to point to one of these
const char *board_id_nrf51_mkit = "1070";
const char *board_id_nrf51_dk = "1100";
const char *board_id_nrf51_dongle = "1120";
const char *board_id_nrf52_dk = "1101";


extern target_cfg_t target_device_nrf52;
static void set_target_device(uint32_t device)
{
    if (device == 0) {
        // Target device already set to nRF51
    }
    else if (device == 1) {
        target_device = target_device_nrf52;
    }
}

void prerun_board_config(void)
{
    // Work around for setting the correct board id based on GPIOs
    uint8_t bit1;
    uint8_t bit2;
    
    PIOB->PIO_PER = (1 << 1); // Enable PIO pin PB1
    PIOB->PIO_PER = (1 << 2); // Enable PIO pin PB2
    PIOB->PIO_ODR = (1 << 1); // Disabe output
    PIOB->PIO_ODR = (1 << 2); // Disabe output
    PIOB->PIO_PUER = (1 << 1); // Enable pull-up 
    PIOB->PIO_PUER = (1 << 2); // Enable pull-up
    
    bit1 = (PIOB->PIO_PDSR >> 1) & 1; // Read PB1
    bit2 = (PIOB->PIO_PDSR >> 2) & 1; // Read PB2
    
    if (!bit2 && bit1) {
        board_id = board_id_nrf51_dk;  // 1100
        set_target_device(0);
    }
    else if (!bit2 && !bit1) {
        board_id = board_id_nrf51_dongle;  // 1120
        set_target_device(0);
    }
    else if (bit2 && !bit1) {
        board_id = board_id_nrf52_dk;  // 1101
        set_target_device(1);
    }
    else { //mkit
        board_id = board_id_nrf51_mkit;  // 1070
        set_target_device(0);
    }
    
    PIOB->PIO_PUDR = (1 << 1); // Disable pull-up 
    PIOB->PIO_PUDR = (1 << 2); // Disable pull-up
}
