/**
 * @file    board_id.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2020 NXP
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

#include "IO_Config.h"

#include "board_id.h"
#include "timer_wait.h"
#include "adc.h"

#include "fsl_port.h"
#include "fsl_gpio.h"

#define BRD_ID_1_UPPER_THR_V        935   // Upper threshold in mV for 100nF and 4700R
#define BRD_ID_1_LOWER_THR_V        268   // Lower threshold in mV for 100nF and 4700R

// Board Rev ID detection. Reads BRD_REV_ID voltage
// Depends on gpio_init() to have been executed already
mb_version_t board_id_detect()
{
    gpio_pin_config_t pin_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    uint32_t board_rev_id_adc = 0;
    uint32_t board_rev_id_mv = 0;

    // Set Board Rev ID pin as output but pin disabled
    PORT_SetPinMux(PIN_BOARD_REV_ID_PORT , PIN_BOARD_REV_ID_BIT,  kPORT_PinDisabledOrAnalog);
    PORT_SetPinDriveStrength(PIN_BOARD_REV_ID_PORT, PIN_BOARD_REV_ID_BIT, kPORT_HighDriveStrength);
    GPIO_PinInit(PIN_BOARD_REV_ID_GPIO, PIN_BOARD_REV_ID_BIT, &pin_config);

    adc_init();
    timer_wait_init();

    // 1. Discharge capacitor
    /* Set timer period 3ms*/
    timer_wait_set_period(3000);
    /* Drive BRD_REV_ID pin to low */
    GPIO_PortClear(PIN_BOARD_REV_ID_GPIO, PIN_BOARD_REV_ID);
    PORT_SetPinMux(PIN_BOARD_REV_ID_PORT , PIN_BOARD_REV_ID_BIT,  kPORT_MuxAsGpio);
    /* Wait 3ms to allow the 100nF Cap to discharge at least 5*RC with 4700R */
    timer_wait();

    // 2. Charge capacitor for 100us
    /* Set timer period slightly below 100us to account for overheads */
    timer_wait_set_period(98);
    /* Drive BRD_REV_ID pin to high */
    GPIO_PortSet(PIN_BOARD_REV_ID_GPIO, PIN_BOARD_REV_ID);
    /* Wait for ~100us */
    timer_wait();
    /* Change pin to ADC (High-Z). Capacitor will stop charging */
    PORT_SetPinMux(PIN_BOARD_REV_ID_PORT , PIN_BOARD_REV_ID_BIT,  kPORT_PinDisabledOrAnalog);

    // 3. Take ADC measurement
    board_rev_id_adc = adc_read_channel(0, PIN_BOARD_REV_ID_ADC_CH, PIN_BOARD_REV_ID_ADC_MUX);
    board_rev_id_mv = board_rev_id_adc * 3300 / 0xFFF;  // Convert ADC 12-bit value to mV with 3.3V reference

    // 4. Discharge capacitor
    /* Set timer period 3ms */
    timer_wait_set_period(3000);
    /* Drive BRD_REV_ID pin to low */
    GPIO_PortClear(PIN_BOARD_REV_ID_GPIO, PIN_BOARD_REV_ID);
    PORT_SetPinMux(PIN_BOARD_REV_ID_PORT , PIN_BOARD_REV_ID_BIT,  kPORT_MuxAsGpio);
    /* Wait 3ms to allow the 100nF Cap to discharge at least 5*RC with 4700R */
    timer_wait();

    // 5. Identify board ID depending on voltage
    mb_version_t board_version;
    if (board_rev_id_mv > BRD_ID_1_LOWER_THR_V && board_rev_id_mv < BRD_ID_1_UPPER_THR_V) {
        board_version = BOARD_VERSION_2_0;
    } else {
        board_version = BOARD_VERSION_2_DEF;
    }

    return board_version;
}
