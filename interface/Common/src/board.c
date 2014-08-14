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

/* Each board should have a unique ID and secret. For information
 *	about obtaining a secret contact support@mbed.org
 */
#if defined (BOARD_FRDM_KL25Z) || defined (BOARD_TWR_KL25Z48M)
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

#elif defined (BOARD_FRDM_K22F)
#define BOARD_ID        "0231"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_FRDM_K64F)
#define BOARD_ID        "0240"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_FRDM_KL02Z)
#define BOARD_ID        "0250"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_FRDM_KL26Z)
#define BOARD_ID        "0260"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_FRDM_KE02Z)
#define BOARD_ID        "0270"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_TWR_K24F)
#define BOARD_ID        "0280"
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

#elif defined (BOARD_BAMBINO_210)
#define BOARD_ID        "1600"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_BAMBINO_210E)
#define BOARD_ID        "1605"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_LPC1549)
#define BOARD_ID        "1549"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_LPC11U68)
#define BOARD_ID        "1168"
#define BOARD_SECRET    "xxxxxxxx"

#elif defined (BOARD_LPC4337)
#define BOARD_ID        "1062"
#define BOARD_SECRET    "xxxxxxxx"

#endif

#if !defined(BOARD_SECRET) || !defined(BOARD_ID)
// TODO: how to check a string value using the preprocessor...
#error contact support@mbed.org to obtain a secret and board ID
#endif

BOARD board = {
    BOARD_ID,      // board id
    BOARD_SECRET   // board secret
};

/* Weak declaration of the board_init function for boards that don't use it */
__weak void board_init(void) {
}
