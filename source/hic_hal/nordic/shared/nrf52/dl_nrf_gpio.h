/**
 * @file    dl_nrf_gpio.h
 * @brief   DAPLink GPIO access functions for nrf52820/nrf52833
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

#ifndef __DAPLINK_NRF_GPIO_H__
#define __DAPLINK_NRF_GPIO_H__

/* This file provides alternate functions to nrf_gpio.h to support
 * dynamically nRF52820 and nRF52833.
 * For nRF52820 (modules/nrfx/mdk/nrf52820_peripherals.h):
 *   #define GPIO_COUNT 1
 *   #define P0_FEATURE_PINS_PRESENT \
 *     (nrf52_errata_230() ? 0xF0168E3Ful : 0x7017C1FFul)
 * For nRF52833 (modules/nrfx/mdk/nrf52833_peripherals.h):
 *   #define GPIO_COUNT 2
 *   #define P0_FEATURE_PINS_PRESENT 0xFFFFFFFFUL
 *   #define P1_FEATURE_PINS_PRESENT 0x000003FFUL
 */

#include "nrf_gpio.h"

#ifndef NRF_P1
#define NRF_P1_BASE                 0x50000300UL
#define NRF_P1                      ((NRF_GPIO_Type*)          NRF_P1_BASE)
#endif

#define GPIO_REG(pin)  (((pin) < 32) ? NRF_P0 : NRF_P1)
#define GPIO_IDX(pin)  (((pin) < 32) ? (pin) : ((pin) & 0x1F))

/*
 * 0xF0168E3FUL // 11110000000101101000111000111111
 * 0x7017C1FFUL // 01110000000101111100000111111111
 */

/* Assuming nrf52_errata_230() returns false */
#define NRF52820_P0_PINS (0x7017C1FFUL)

#if defined(DEVELOP_IN_NRF52833)
#define NRF52820_P0_MASK (NRF52820_P0_PINS | 0x03003800)
#else
#define NRF52820_P0_MASK (NRF52820_P0_PINS)
#endif

#define GPIO_CHECK_PRESENT_NRF52820(pin) \
  ((((pin) >> 5) == 0) ? ((NRF52820_P0_MASK & (1 << ((pin) & 0x1F))) ? 1 : 0) : 0)

#define NRF52833_P0_PINS 0xFFFFFFFFUL
#define NRF52833_P1_PINS 0x000003FFUL

#define GPIO_CHECK_PRESENT_NRF52833(pin) \
  (((pin) >> 5) == 0) ? ((NRF52833_P0_PINS & (1 << ((pin) & 0x1F))) ? 1 : 0) : \
  ((((pin) >> 5) == 1) ? ((NRF52833_P1_PINS & (1 << ((pin) & 0x1F))) ? 1 : 0) : 0)

/** @brief Pin configuration function */
__STATIC_INLINE \
void gpio_cfg(NRF_GPIO_Type *reg, uint32_t idx,
                 nrf_gpio_pin_dir_t dir, nrf_gpio_pin_input_t input,
                 nrf_gpio_pin_pull_t  pull, nrf_gpio_pin_drive_t drive,
                 nrf_gpio_pin_sense_t sense)
{
    reg->PIN_CNF[idx] = ((uint32_t)dir << GPIO_PIN_CNF_DIR_Pos)
                        | ((uint32_t)input << GPIO_PIN_CNF_INPUT_Pos)
                        | ((uint32_t)pull << GPIO_PIN_CNF_PULL_Pos)
                        | ((uint32_t)drive << GPIO_PIN_CNF_DRIVE_Pos)
                        | ((uint32_t)sense << GPIO_PIN_CNF_SENSE_Pos);
}

/** @brief Pin configuration as output */
__STATIC_INLINE void gpio_cfg_output(NRF_GPIO_Type *reg, uint32_t idx)
{
    gpio_cfg(reg, idx, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT,
             NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
}

/** @brief Pin configuration as input */
__STATIC_INLINE void gpio_cfg_input(NRF_GPIO_Type *reg, uint32_t idx,
                                    nrf_gpio_pin_pull_t pull_config)
{
    gpio_cfg(reg, idx, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_CONNECT,
             pull_config, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
}

/** @brief Pin configuration as input */
__STATIC_INLINE void gpio_set(NRF_GPIO_Type *reg, uint32_t idx)
{
    reg->OUTSET = (1UL << idx);
}

__STATIC_INLINE void gpio_clear(NRF_GPIO_Type *reg, uint32_t idx)
{
    reg->OUTCLR = (1UL << idx);
}

__STATIC_INLINE uint32_t gpio_read(NRF_GPIO_Type *reg, uint32_t idx)
{
    return ((reg->IN >> idx) & 1UL);
}

__STATIC_INLINE uint32_t gpio_out_read(NRF_GPIO_Type *reg, uint32_t idx)
{
    return ((reg->OUT >> idx) & 1UL);
}

__STATIC_INLINE void gpio_set_output_dir(NRF_GPIO_Type *reg, uint32_t idx)
{
    reg->DIRSET = (1UL << idx);
}

__STATIC_INLINE void gpio_write(NRF_GPIO_Type *reg, uint32_t idx, uint32_t value)
{
    if (value == 0) {
        gpio_clear(reg, idx);
    } else {
        gpio_set(reg, idx);
    }
}

#endif
