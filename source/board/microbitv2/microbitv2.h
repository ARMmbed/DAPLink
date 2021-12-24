/**
 * @file    microbitv2.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2020 NXP
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

#ifndef MICROBITV2_H_
#define MICROBITV2_H_

#define M0_RESERVED_VECT_OFFSET     (4 * 4)
#define M0_RESERVED_VECT_SIZE       (3 * 4) // Size for mem fault, bus fault and usage fault vectors

#define PWR_LED_ON_MAX_BRIGHTNESS       255 // Max LED Brightness (PC Connected)
#define PWR_LED_INIT_FADE_BRIGHTNESS    200 // Initial fade LED Brightness
#define PWR_LED_ON_BATT_BRIGHTNESS      100 // LED Brightness while being powered by battery
#define PWR_LED_FADEOUT_MIN_BRIGHTNESS  80  // Minimum LED brightness when fading out
#define PWR_LED_SLEEP_STATE_DEFAULT     true
#define AUTOMATIC_SLEEP_DEFAULT         true

// define the reset button presses
#define RESET_SHORT_PRESS       10   // x 30ms debounce time = 300ms
#define RESET_MID_PRESS         80   // x 30ms debounce time = 2400ms
#define RESET_LONG_PRESS        120  // x 30ms debounce time = 3600ms
#define RESET_MAX_LENGTH_PRESS  RESET_LONG_PRESS

typedef enum main_shutdown_state {
    MAIN_SHUTDOWN_WAITING = 0,
    MAIN_SHUTDOWN_WAITING_OFF,
    MAIN_SHUTDOWN_PENDING,
    MAIN_SHUTDOWN_REACHED,
    MAIN_SHUTDOWN_REACHED_FADE,
    MAIN_SHUTDOWN_REQUESTED,
    MAIN_USER_EVENT,
    MAIN_LED_BLINK_ONCE,
    MAIN_LED_BLINKING,
    MAIN_LED_FULL_BRIGHTNESS,
    MAIN_SHUTDOWN_CANCEL
} main_shutdown_state_t;

#endif /* MICROBITV2_H_ */
