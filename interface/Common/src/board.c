/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "board.h"

#if defined (BOARD_FRDM_KL25Z)
#define BOARD_ID        "0200"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_FRDM_KL05Z)
#define BOARD_ID        "0210"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_FRDM_KL46Z)
#define BOARD_ID        "0220"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_FRDM_K20D50M)
#define BOARD_ID        "0230"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_LPC812_MAX)
#define BOARD_ID        "1050"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_SEEED_ARCH_PRO)
#define BOARD_ID        "9004"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_UBLOX_C027)
#define BOARD_ID        "1234"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_LPC1114)
#define BOARD_ID        "1114"
#define BOARD_SECRET    "xxxxxxxx"

#endif

BOARD board = {
    BOARD_ID,      // board id
    BOARD_SECRET   // board secret
};
