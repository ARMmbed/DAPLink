/**
 * @file    mkit_dk_dongle_nrf5x_ext.c
 * @brief   board ID for the Nordic nRF5x developments boards
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2021, Arm Limited, All Rights Reserved
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
#include "DAP_config.h"
#include "target_config.h"
#include "util.h"
#include "flash_manager.h"
#include "target_family.h"
#include "target_board.h"

// BOARD_ID pointer will be set during run time to point to one of these
const char *board_id_nrf51_dk = "1100";
const char *board_id_nrf52_dk = "1101";
const char *board_id_nrf52840_dk = "1102";

const char *board_name_nrf51_dk = "nRF51-DK";
const char *board_name_nrf52_dk = "nRF52-DK";
const char *board_name_nrf52840_dk = "nRF52840-DK";
static char board_name[24] = "UNKNOWN"; // needs to be >= longest board name + longest suffix + 1

const char *suffix_ext_str = " (ext)";
const char *suffix_shield_str = " (shield)";

extern target_cfg_t target_device_invalid;
extern target_cfg_t target_device_nrf51822_32;
extern target_cfg_t target_device_nrf52_64;
extern target_cfg_t target_device_nrf52840;

target_cfg_t target_device = { // invalid by default
    .version        = kTargetConfigVersion,
    .rt_family_id   = kStub_HWReset_FamilyID,
    .rt_board_id    = "0000",
};

static uint8_t board_supported = 0;
static uint8_t target_ext;
static uint8_t target_shield;

// support for dynamic assignment of SWD signals
Pio *pin_nreset_port = PIN_OB_nRESET_PORT;        // GPIO port for nRESET signal
Pio *pin_swclk_port =  PIN_OB_SWCLK_PORT;         // GPIO port for SWCLK signal
Pio *pin_swdio_port =  PIN_OB_SWDIO_PORT;         // GPIO port for SWDIO signal
unsigned long pin_nreset_bit = PIN_OB_nRESET_BIT; // GPIO pin for nRESET signal
unsigned long pin_swclk_bit  = PIN_OB_SWCLK_BIT;  // GPIO pin for SWCLK signal
unsigned long pin_swdio_bit  = PIN_OB_SWDIO_BIT;  // GPIO pin for SWDIO signal
unsigned long pin_nreset = PIN_OB_nRESET;         // GPIO mask for nRESET signal
unsigned long pin_swclk =  PIN_OB_SWCLK;          // GPIO mask for SWCLK signal
unsigned long pin_swdio =  PIN_OB_SWDIO;          // GPIO msak for SWDIO signal

static void nrf_prerun_board_config(void)
{
    // Work around for setting the correct board id based on GPIOs
    uint8_t bit1, bit2, bit3, gpio_id;

    PIOB->PIO_PER = (1 << 1); // Enable PIO pin PB1
    PIOB->PIO_PER = (1 << 2); // Enable PIO pin PB2
    PIOB->PIO_PER = (1 << 3); // Enable PIO pin PB3
    PIOB->PIO_ODR = (1 << 1); // Disabe output
    PIOB->PIO_ODR = (1 << 2); // Disabe output
    PIOB->PIO_ODR = (1 << 3); // Disabe output
    PIOB->PIO_PUER = (1 << 1); // Enable pull-up
    PIOB->PIO_PUER = (1 << 2); // Enable pull-up
    PIOB->PIO_PUER = (1 << 3); // Enable pull-up

    bit1 = (PIOB->PIO_PDSR >> 1) & 1; // Read PB1
    bit2 = (PIOB->PIO_PDSR >> 2) & 1; // Read PB2
    bit3 = (PIOB->PIO_PDSR >> 3) & 1; // Read PB3

    PIOB->PIO_PUDR = (1 << 1); // Disable pull-up
    PIOB->PIO_PUDR = (1 << 2); // Disable pull-up
    PIOB->PIO_PUDR = (1 << 3); // Disable pull-up

    gpio_id = (bit3 << 2) | (bit2 << 1) | bit1;

    /* Pins translate to board-ids as follow.
     * Starred IDs are those supported here.
     *
     *  PB3|PB2|PB1|BOARD ID| BOARD
     *  ----------------------------------
     *   0 | 0 | 0 |  1120  | nRF51-Dongle
     *   0 | 0 | 1 |  1100  | nRF51-DK*
     *   0 | 1 | 0 |  1101  | nRF52-DK*
     *   0 | 1 | 1 |  1102  | nRF52840-DK*
     *   1 | 1 | 1 |  1070  | nRF51822-mKIT
     *   1 | 0 | 0 |    undefined
     *   1 | 0 | 1 |    undefined
     *   1 | 1 | 0 |    undefined
     */

    // if board is supported, set board name
    switch (gpio_id) {
        case 0x01: // nRF-51 DK
            strcpy(board_name, board_name_nrf51_dk);
            board_supported = 1;
            break;

        case 0x02: // nRF52-DK
            strcpy(board_name, board_name_nrf52_dk);
            board_supported = 1;
            break;

        case 0x03: // nRF52840-DK
            strcpy(board_name, board_name_nrf52840_dk);
            board_supported = 1;
            break;

        default:
            board_supported = 0;
    }

    // if board is unsupported, we are done.
    if (!board_supported)
        return;

    // External target detection:
    // supports nRF51-DK, nRF52-DK, nRF52840-DK

    // Configure GPIO for detection
    // - EXT_VTG (high if external target is powered)
    PIOB->PIO_PUDR = (1 << 6); // pull-up disable
    PIOB->PIO_ODR  = (1 << 6); // input
    PIOB->PIO_PER  = (1 << 6); // GPIO control
    // - EXT_GND_DETECT (low if external target is connected)
    PIOB->PIO_PUER = (1 << 18); // pull-up enable
    PIOB->PIO_ODR  = (1 << 18); // input
    PIOB->PIO_PER  = (1 << 18); // GPIO control
    // - SH_VTG (high if shield-mounted target is powered)
    PIOB->PIO_PUDR = (1 << 5); // pull-up disable
    PIOB->PIO_ODR  = (1 << 5); // input
    PIOB->PIO_PER  = (1 << 5); // GPIO control
    // - SH_GND_DETECT (low if shield-mounted target is connected)
    PIOB->PIO_PUER = (1 << 23); // pull-up enable
    PIOB->PIO_ODR  = (1 << 23); // input
    PIOB->PIO_PER  = (1 << 23); // GPIO control

    // nonzero if external target is detected
    bit1 = (PIOB->PIO_PDSR >> 6) & 1;  // Read PB6
    bit2 = (PIOB->PIO_PDSR >> 18) & 1; // Read PB18
    target_ext = bit1 | !bit2;

    // nonzero if shield-mounted target is detected
    bit1 = (PIOB->PIO_PDSR >> 5) & 1;  // Read PB5
    bit2 = (PIOB->PIO_PDSR >> 23) & 1; // Read PB23
    target_shield = bit1 | !bit2;

    // Disable pull-ups for detection
    PIOB->PIO_PUDR = (1 << 18); // pull-up disable
    PIOB->PIO_PUDR = (1 << 23); // pull-up disable

    // if external/shield target is detected, re-route SWD signals
    if (target_ext) {
        pin_nreset_port = PIN_EXT_nRESET_PORT;
        pin_nreset_bit  = PIN_EXT_nRESET_BIT;
        pin_nreset      = PIN_EXT_nRESET;

        pin_swclk_port  = PIN_EXT_SWCLK_PORT;
        pin_swclk_bit   = PIN_EXT_SWCLK_BIT;
        pin_swclk       = PIN_EXT_SWCLK;

        pin_swdio_port  = PIN_EXT_SWDIO_PORT;
        pin_swdio_bit   = PIN_EXT_SWDIO_BIT;
        pin_swdio       = PIN_EXT_SWDIO;

        // append suffix to board name
        strcpy(board_name + strlen(board_name), suffix_ext_str);

    } else if (target_shield) {
        pin_nreset_port = PIN_SH_nRESET_PORT;
        pin_nreset_bit  = PIN_SH_nRESET_BIT;
        pin_nreset      = PIN_SH_nRESET;

        pin_swclk_port  = PIN_SH_SWCLK_PORT;
        pin_swclk_bit   = PIN_SH_SWCLK_BIT;
        pin_swclk       = PIN_SH_SWCLK;

        pin_swdio_port  = PIN_SH_SWDIO_PORT;
        pin_swdio_bit   = PIN_SH_SWDIO_BIT;
        pin_swdio       = PIN_SH_SWDIO;

        // append suffix to board name
        strcpy(board_name + strlen(board_name), suffix_shield_str);

    } else { // OB target
        switch (gpio_id) {
            case 0x01: // nRF-51 DK
                target_device = target_device_nrf51822_32;
                target_device.rt_family_id = kNordic_Nrf51_FamilyID;
                target_device.rt_board_id = board_id_nrf51_dk;
                break;

            case 0x02: // nRF52-DK
                target_device = target_device_nrf52_64;
                target_device.rt_family_id = kNordic_Nrf52_FamilyID;
                target_device.rt_board_id = board_id_nrf52_dk;
                break;

            case 0x03: // nRF52840-DK
                target_device = target_device_nrf52840;
                target_device.rt_family_id = kNordic_Nrf52_FamilyID;
                target_device.rt_board_id = board_id_nrf52840_dk;
                break;

            default: // never reached
                ;
        }
    }

    // switch on red LED if external/shield target is in use
    PIOA->PIO_PUDR = (1 << 28); // pull-up disable
    PIOA->PIO_OER  = (1 << 28); // output
    PIOA->PIO_PER  = (1 << 28); // GPIO control
    if (target_ext || target_shield)
        PIOA->PIO_CODR = (1 << 28); // set low
    else
        PIOA->PIO_SODR = (1 << 28); // set high
}

// Overrides flash_algo_valid() in source/target/target_board.c .
// Only enables MSD when a supported board is detected and the on-board target is enabled.
uint8_t flash_algo_valid(void)
{
    return (board_supported && !target_ext && !target_shield);
}

static void nrf_swd_set_target_reset(uint8_t asserted){
    if(!asserted) {
        PIOA->PIO_MDER = PIN_SWDIO | PIN_SWCLK | PIN_nRESET;
    }
}

const board_info_t g_board_info = {
    .info_version = kBoardInfoVersion,
    .flags = kEnablePageErase,
    .prerun_board_config = nrf_prerun_board_config,
    .swd_set_target_reset = nrf_swd_set_target_reset,
    .target_cfg = &target_device,
    .board_vendor = "Nordic Semiconductor",
    .board_name = board_name,
};
