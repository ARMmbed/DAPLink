/*
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

#include <stdint.h>

//! @name IOCON pin config constants
//@{
#define IOCON_PIO_FUNC0 0x00u         /*!<@brief Selects pin function 0 */
#define IOCON_PIO_FUNC1 0x01u         /*!<@brief Selects pin function 1 */
#define IOCON_PIO_FUNC7 0x07u         /*!<@brief Selects pin function 7 */
#define IOCON_PIO_INV_DI 0x00u        /*!<@brief Input function is not inverted */
#define IOCON_PIO_MODE_INACT 0x00u    /*!<@brief No addition pin function */
#define IOCON_PIO_OPENDRAIN_DI 0x00u  /*!<@brief Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD 0x00u /*!<@brief Standard mode, output slew rate control is enabled */
#define IOCON_PIO_DIGITAL_EN 0x0100u  /*!<@brief Enables digital function */
//@}

#if defined(__cplusplus)
extern "C" {
#endif

//! @name USART0 CMSIS-Driver callbacks
//@{
uint32_t USART0_GetFreq(void);
void USART0_InitPins(void);
void USART0_DeinitPins(void);
//@}

//! @name USART3 CMSIS-Driver callbacks
//@{
uint32_t USART3_GetFreq(void);
void USART3_InitPins(void);
void USART3_DeinitPins(void);
//@}

//! @brief Configure VBUS pin.
void init_vbus_pin(void);

#if defined(__cplusplus)
}
#endif
