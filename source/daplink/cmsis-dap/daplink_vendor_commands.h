/*
 * DAPLink Interface Firmware
 * Copyright (c) 2021 Chris Reed
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
/**
 * @file
 * @brief Vendor-specific CMSIS-DAP command constants.
 */

#include "DAP.h"

//! @name DAPLink vendor-specific CMSIS-DAP command IDs
//@{
#define ID_DAP_GetUniqueID              ID_DAP_Vendor0
#define ID_DAP_UART_GetLineCoding       ID_DAP_Vendor1
#define ID_DAP_UART_SetConfiguration    ID_DAP_Vendor2
#define ID_DAP_UART_Read                ID_DAP_Vendor3
#define ID_DAP_UART_Write               ID_DAP_Vendor4
#define ID_DAP_SetUSBTestMode           ID_DAP_Vendor8
#define ID_DAP_ResetTargetIfNoAutoReset ID_DAP_Vendor9
#define ID_DAP_MSD_Open                 ID_DAP_Vendor10
#define ID_DAP_MSD_Close                ID_DAP_Vendor11
#define ID_DAP_MSD_Write                ID_DAP_Vendor12
#define ID_DAP_SelectEraseMode          ID_DAP_Vendor13
//@}

