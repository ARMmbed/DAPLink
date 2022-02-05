/**
 * @file    pwr_mon_adc.c
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

#include "nrf52.h"

#include "pwr_mon_adc.h"


static uint32_t pwr_mon_saadc(uint32_t pselp);

#ifndef __NRF52833_BITS_H

#define SAADC_CH_PSELP_PSELP_VDDHDIV5   (0x0DUL)
#define SAADC_CH_CONFIG_TACQ_3us        (0UL)
#define SAADC_CH_CONFIG_TACQ_10us       (2UL)
#define SAADC_RESOLUTION_VAL_14bit      (3UL)
#define SAADC_RESOLUTION_VAL_Pos        (0UL)
#define SAADC_SAMPLERATE_MODE_Task      (0UL)
#define SAADC_SAMPLERATE_MODE_Pos       (12UL)
#define SAADC_CH_CONFIG_RESP_Bypass     (0UL)
#define SAADC_CH_CONFIG_RESP_Pos        (0UL)
#define SAADC_CH_CONFIG_RESN_Bypass     (0UL)
#define SAADC_CH_CONFIG_RESN_Pos        (4UL)
#define SAADC_CH_CONFIG_GAIN_Gain1_6    (0UL)
#define SAADC_CH_CONFIG_GAIN_Pos        (8UL)
#define SAADC_CH_CONFIG_REFSEL_Internal (0UL)
#define SAADC_CH_CONFIG_REFSEL_Pos      (12UL)
#define SAADC_CH_CONFIG_TACQ_Pos        (16UL)
#define SAADC_CH_CONFIG_MODE_SE         (0UL)
#define SAADC_CH_CONFIG_MODE_Pos        (20UL)
#define SAADC_CH_CONFIG_BURST_Disabled  (0UL)
#define SAADC_CH_CONFIG_BURST_Pos       (24UL)
#define SAADC_CH_PSELP_PSELP_Pos        (0UL)
#define SAADC_CH_PSELN_PSELN_NC         (0UL)
#define SAADC_CH_PSELN_PSELN_Pos        (0UL)
#define SAADC_CH_PSELP_PSELP_NC         (0UL)

#endif // __NRF52833_BITS_H



uint32_t pwr_mon_adc_vin() {
  return pwr_mon_saadc(SAADC_CH_PSELP_PSELP_VDDHDIV5);
}


static uint32_t pwr_mon_saadc(uint32_t pselp) {
  volatile int16_t buffer[1];
  uint32_t tacq = pselp == SAADC_CH_PSELP_PSELP_VDDHDIV5 ? SAADC_CH_CONFIG_TACQ_10us : SAADC_CH_CONFIG_TACQ_3us;

  NRF_SAADC->ENABLE = 0;

  NRF_SAADC->RESOLUTION = SAADC_RESOLUTION_VAL_14bit << SAADC_RESOLUTION_VAL_Pos;
  NRF_SAADC->SAMPLERATE = SAADC_SAMPLERATE_MODE_Task << SAADC_SAMPLERATE_MODE_Pos;
  NRF_SAADC->OVERSAMPLE = 0;

  NRF_SAADC->RESULT.MAXCNT = 1;
  NRF_SAADC->RESULT.PTR = (uint32_t) buffer;

  // Input range = REFERENCE/GAIN = 0.6/(1/6) = 3.6V
  NRF_SAADC->CH[0].CONFIG = (SAADC_CH_CONFIG_RESP_Bypass     << SAADC_CH_CONFIG_RESP_Pos)   |
                            (SAADC_CH_CONFIG_RESN_Bypass     << SAADC_CH_CONFIG_RESN_Pos)   |
                            (SAADC_CH_CONFIG_GAIN_Gain1_6    << SAADC_CH_CONFIG_GAIN_Pos)   |
                            (SAADC_CH_CONFIG_REFSEL_Internal << SAADC_CH_CONFIG_REFSEL_Pos) |
                            (tacq                            << SAADC_CH_CONFIG_TACQ_Pos)   |
                            (SAADC_CH_CONFIG_MODE_SE         << SAADC_CH_CONFIG_MODE_Pos)   |
                            (SAADC_CH_CONFIG_BURST_Disabled  << SAADC_CH_CONFIG_BURST_Pos);

  NRF_SAADC->CH[0].PSELP = pselp                    << SAADC_CH_PSELP_PSELP_Pos;
  NRF_SAADC->CH[0].PSELN = SAADC_CH_PSELN_PSELN_NC  << SAADC_CH_PSELN_PSELN_Pos;

  for ( int i = 1; i < 8; i++) {
      NRF_SAADC->CH[i].CONFIG = NRF_SAADC->CH[0].CONFIG;
      NRF_SAADC->CH[i].PSELP = SAADC_CH_PSELP_PSELP_NC << SAADC_CH_PSELP_PSELP_Pos;
      NRF_SAADC->CH[i].PSELN = SAADC_CH_PSELN_PSELN_NC << SAADC_CH_PSELN_PSELN_Pos;
  }

  NRF_SAADC->ENABLE = 1;
  NRF_SAADC->TASKS_START = 1;
  while (!NRF_SAADC->EVENTS_STARTED) /*wait*/;
  NRF_SAADC->EVENTS_STARTED = 0;

  NRF_SAADC->TASKS_SAMPLE = 1;
  while (!NRF_SAADC->EVENTS_END) /*wait*/;
  NRF_SAADC->EVENTS_END = 0;

  NRF_SAADC->TASKS_STOP = 1;
  while (!NRF_SAADC->EVENTS_STOPPED) /*wait*/;
  NRF_SAADC->EVENTS_STOPPED = 0;

  int16_t adc = buffer[0];
  if ( adc < 0) {
      adc = 0;
  }

  // adc = V * (GAIN/REFERENCE) * (2^RESOLUTION)
  // V = (REFERENCE/GAIN) * adc / (2^RESOLUTION)
  // V = 3.6 * adc / 16384
  // mV = 3600 * adc / 16384
  // mV = 225 * adc / 1024
  uint32_t mv = 225 * (uint32_t) adc / 1024;
  if ( pselp == SAADC_CH_PSELP_PSELP_VDDHDIV5) {
      mv *= 5;
  }

  return mv;
}
