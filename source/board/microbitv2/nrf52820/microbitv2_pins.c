/**
 * @file    pwr_mon.c
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

#include "nrf.h"

#include "microbitv2_pins.h"


#ifdef NRF528XX_DYNAMIC_PIN

uint32_t pin_run_vbat_sense;
uint32_t pin_vbat_sense;
uint32_t pin_vbat_sense_comp;
uint32_t pin_vin_comp;
uint32_t pin_vbus_absent;

#endif


void microbitv2_pins_init()
{
#ifdef NRF528XX_DYNAMIC_PIN
    if (NRF_FICR->INFO.PART == 0x52833) {
        // nRF52833
        pin_run_vbat_sense  = NRF52833_PIN_RUN_VBAT_SENSE;
        pin_vbat_sense      = NRF52833_PIN_VBAT_SENSE;
        pin_vbat_sense_comp = NRF52833_PIN_VBAT_SENSE_COMP;
        pin_vin_comp        = NRF52833_PIN_VIN_COMP;
        pin_vbus_absent     = NRF52833_PIN_VBUS_ABSENT;
    } else {
        // nRF52820
        pin_run_vbat_sense  = NRF52820_PIN_RUN_VBAT_SENSE;
        pin_vbat_sense      = NRF52820_PIN_VBAT_SENSE;
        pin_vbat_sense_comp = NRF52820_PIN_VBAT_SENSE_COMP;
        pin_vin_comp        = NRF52820_PIN_VIN_COMP;
        pin_vbus_absent     = NRF52820_PIN_VBUS_ABSENT;
    }
#endif
}
