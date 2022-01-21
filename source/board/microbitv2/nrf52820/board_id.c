/**
 * @file    board_id.c
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

#include "board_id.h"

#include "nrf.h"


mb_version_t board_id_detect(void)
{
    switch (NRF_FICR->INFO.PART) {
        case 0x52833: return BOARD_VERSION_2_2_833;
        case 0x52820: return BOARD_VERSION_2_2_820;
        default: return BOARD_VERSION_2_DEF;
    }
}
