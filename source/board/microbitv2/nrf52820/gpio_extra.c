/**
 * @file    gpio_extra.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2021 Micro:bit Educational Foundation
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

#include "gpio_extra.h"
#include "compiler.h"
#include "nrf_nvmc.h"
#include "dl_nrf_gpio.h"

#define NRF52833_COMBINED_SENSOR_INT_PIN    NRF_GPIO_PIN_MAP(0, 9)
#define NRF52820_COMBINED_SENSOR_INT_PIN    NRF_GPIO_PIN_MAP(0, 16)

#ifdef NRF528XX_DYNAMIC_PIN
static uint32_t COMBINED_SENSOR_INT_PIN;
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52833(NRF52833_COMBINED_SENSOR_INT_PIN));
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52820(NRF52820_COMBINED_SENSOR_INT_PIN));
#else
static const uint32_t COMBINED_SENSOR_INT_PIN = NRF52820_COMBINED_SENSOR_INT_PIN;
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52820(NRF52820_COMBINED_SENSOR_INT_PIN));
#endif

/* NFC configuration in the User UICR area, needed to configure NFC pins as GPIO */
#define NRF52833_UICR_NFCPINS_OFFSET            (0x20CUL)
#define NRF52833_UCIR_NFCPINS_ADDRESS           (NRF_UICR_BASE + NRF52833_UICR_NFCPINS_OFFSET)
#define NRF52833_UICR_NFCPINS_PROTECT_Pos       (0UL) /*!< Position of PROTECT field. */
#define NRF52833_UICR_NFCPINS_PROTECT_Msk       (0x1UL << NRF52833_UICR_NFCPINS_PROTECT_Pos) /*!< Bit mask of PROTECT field. */
#define NRF52833_UICR_NFCPINS_PROTECT_Disabled  (0UL) /*!< Operation as GPIO pins. Same protection as normal GPIO pins. */
#define NRF52833_UICR_NFCPINS_PROTECT_NFC       (1UL) /*!< Operation as NFC antenna pins. Configures the protection for NFC operation. */


void gpio_enable_hid_led()
{
    gpio_cfg_output(GPIO_REG(LED_HID), GPIO_IDX(LED_HID));
}

void gpio_disable_hid_led()
{
    gpio_cfg(GPIO_REG(LED_HID),
             GPIO_IDX(LED_HID),
             NRF_GPIO_PIN_DIR_INPUT,
             NRF_GPIO_PIN_INPUT_DISCONNECT,
             NRF_GPIO_PIN_NOPULL,
             NRF_GPIO_PIN_S0S1,
             NRF_GPIO_PIN_NOSENSE);
}

void gpio_init_combined_int()
{
#ifdef NRF528XX_DYNAMIC_PIN
    if (NRF_FICR->INFO.PART == 0x52833) {
        // nRF52833
        COMBINED_SENSOR_INT_PIN = NRF52833_COMBINED_SENSOR_INT_PIN;

        // Configure the NFC pins as GPIO in the UICR if not done already
        volatile uint32_t* const nrf_uicr_nfcpins = (uint32_t *) NRF52833_UCIR_NFCPINS_ADDRESS;
        if ((*nrf_uicr_nfcpins & NRF52833_UICR_NFCPINS_PROTECT_Msk) ==
                (NRF52833_UICR_NFCPINS_PROTECT_NFC << NRF52833_UICR_NFCPINS_PROTECT_Pos)) {
            nrf_nvmc_mode_set(NRF_NVMC, NRF_NVMC_MODE_WRITE);
            *nrf_uicr_nfcpins &= ~NRF52833_UICR_NFCPINS_PROTECT_Msk;
            while (!nrf_nvmc_ready_check(NRF_NVMC));
            nrf_nvmc_mode_set(NRF_NVMC, NRF_NVMC_MODE_READONLY);
            // Changes only take effect after a system reset
            // TODO: This does not seem to reset the system correctly, it hangs somewhere
            NVIC_SystemReset();
        }
    } else {
        // nRF52820
        COMBINED_SENSOR_INT_PIN = NRF52820_COMBINED_SENSOR_INT_PIN;
    }
#endif
    gpio_disable_combined_int();
}

void gpio_disable_combined_int()
{
    gpio_cfg(GPIO_REG(COMBINED_SENSOR_INT_PIN),
             GPIO_IDX(COMBINED_SENSOR_INT_PIN),
             NRF_GPIO_PIN_DIR_INPUT,
             NRF_GPIO_PIN_INPUT_DISCONNECT,
             NRF_GPIO_PIN_NOPULL,
             NRF_GPIO_PIN_S0S1,
             NRF_GPIO_PIN_NOSENSE);
}

void gpio_assert_combined_int()
{
    gpio_cfg_output(GPIO_REG(COMBINED_SENSOR_INT_PIN),
                    GPIO_IDX(COMBINED_SENSOR_INT_PIN));
    gpio_clear(GPIO_REG(COMBINED_SENSOR_INT_PIN),
               GPIO_IDX(COMBINED_SENSOR_INT_PIN));
}
