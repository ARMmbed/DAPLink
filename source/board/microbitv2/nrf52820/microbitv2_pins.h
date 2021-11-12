/**
 * @file    nrf52_pins.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2021 Micro:bit Educational foundation
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
 
#ifndef MICROBITV2_PINS_H_
#define MICROBITV2_PINS_H_

#include "IO_Config.h"


// Volatge monitor pins
// RUN_VBAT_SENSE   Enable Battery Voltage monitor
// VBAT_SENSE       Battery Voltage monitor
// VBUS_ABSENT      High when USB power is not connected (WAKE_ON_EDGE)

#define NRF52833_PIN_RUN_VBAT_SENSE         NRF_GPIO_PIN_MAP(0, 31)
#define NRF52833_PIN_VBAT_SENSE             NRF_GPIO_PIN_MAP(0, 30)
#define NRF52833_PIN_VBAT_SENSE_COMP        6 //COMP_PSEL_PSEL_AnalogInput6
#define NRF52833_PIN_VIN_COMP               0 //Not available
#define NRF52833_PIN_VBUS_ABSENT            NRF_GPIO_PIN_MAP(0, 11)

#define NRF52820_PIN_RUN_VBAT_SENSE         NRF_GPIO_PIN_MAP(0, 2)
#define NRF52820_PIN_VBAT_SENSE             NRF_GPIO_PIN_MAP(0, 3)
#define NRF52820_PIN_VBAT_SENSE_COMP        COMP_PSEL_PSEL_AnalogInput1
#define NRF52820_PIN_VIN_COMP               COMP_PSEL_PSEL_VddhDiv5
#define NRF52820_PIN_VBUS_ABSENT            NRF_GPIO_PIN_MAP(0, 7)

#ifdef NRF528XX_DYNAMIC_PIN

extern uint32_t pin_run_vbat_sense;
extern uint32_t pin_vbat_sense;
extern uint32_t pin_vbat_sense_comp;
extern uint32_t pin_vin_comp;
extern uint32_t pin_vbus_absent;

COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52833(NRF52833_PIN_RUN_VBAT_SENSE));
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52833(NRF52833_PIN_VBAT_SENSE));
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52833(NRF52833_PIN_VBUS_ABSENT));

COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52820(NRF52820_PIN_RUN_VBAT_SENSE));
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52820(NRF52820_PIN_VBAT_SENSE));
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52820(NRF52820_PIN_VBUS_ABSENT));
#else
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52820(NRF52820_PIN_RUN_VBAT_SENSE));
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52820(NRF52820_PIN_VBAT_SENSE));
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52820(NRF52820_PIN_VBUS_ABSENT));
#endif

#ifdef NRF528XX_DYNAMIC_PIN
#define PIN_RUN_VBAT_SENSE      pin_run_vbat_sense
#define PIN_VBAT_SENSE          pin_vbat_sense
#define PIN_VBAT_SENSE_COMP     pin_vbat_sense_comp
#define PIN_VIN_COMP            pin_vin_comp
#define PIN_VBUS_ABSENT         pin_vbus_absent
#else
#define PIN_RUN_VBAT_SENSE      NRF52820_PIN_RUN_VBAT_SENSE
#define PIN_VBAT_SENSE          NRF52820_PIN_VBAT_SENSE
#define PIN_VBAT_SENSE_COMP     NRF52820_PIN_VBAT_SENSE_COMP
#define PIN_VIN_COMP            NRF52820_PIN_VIN_COMP
#define PIN_VBUS_ABSENT         NRF52820_PIN_VBUS_ABSENT
#endif


void microbitv2_pins_init(void);

#endif /* MICROBITV2_PINS_H_ */
