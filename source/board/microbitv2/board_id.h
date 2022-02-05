/**
 * @file    board_id.h
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

#ifndef BOARD_ID_H_
#define BOARD_ID_H_

#ifdef __cplusplus
extern "C" {
#endif

#define BOARD_ID_MB_2_DEFAULT       "9903"
#define BOARD_ID_MB_2_0             "9904"
#define BOARD_ID_MB_2_2_833         "9905"
#define BOARD_ID_MB_2_2_820         "9906"

typedef enum {
    BOARD_VERSION_2_DEF = 0x9903,
    BOARD_VERSION_2_0 = 0x9904,
    BOARD_VERSION_2_2_833 = 0x9905,
    BOARD_VERSION_2_2_820 = 0x9906,
} mb_version_t;


/**
 * Determines the micro:bit board ID based on on-board components configured
 * differently between versions.
 */
mb_version_t board_id_detect(void);


#ifdef __cplusplus
}
#endif

#endif /* BOARD_ID_H_ */
