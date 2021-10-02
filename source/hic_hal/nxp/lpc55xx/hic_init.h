/**
 * @file    hic_init.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2020 Arm Limited, All Rights Reserved
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

#define BOARD_XTAL0_CLK_HZ 16000000U /*!< Board xtal frequency in Hz */
#define BOARD_XTAL32K_CLK_HZ 32768U  /*!< Board xtal32K frequency in Hz */

#define BOARD_BOOTCLOCKFROHF96M_CORE_CLOCK 96000000U /*!< Core clock frequency: 96000000Hz */
#define BOARD_BOOTCLOCKPLL150M_CORE_CLOCK 150000000U /*!< Core clock frequency: 150000000Hz */

#if defined(__cplusplus)
extern "C" {
#endif

//! @brief Set some system-wide hardware settings.
void hic_init(void);

//! @brief Init CRC peripheral.
void hic_crc_init(void);

//! @brief Enable clocks required for USB operation.
void hic_enable_usb_clocks(void);

#if defined(__cplusplus)
}
#endif
