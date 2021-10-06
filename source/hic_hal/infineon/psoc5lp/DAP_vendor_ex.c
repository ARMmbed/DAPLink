/*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
 * Copyright (2019-2021) Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ----------------------------------------------------------------------
 *
 * $Date:        20. May 2015
 * $Revision:    V1.10
 *
 * Project:      CMSIS-DAP Source
 * Title:        DAP_vendor_ex.c CMSIS-DAP Extended Vendor Commands
 *
 *---------------------------------------------------------------------------*/

#include "DAP_config.h"
#include "DAP.h"
#include "psoc5lp.h"

/* DAP Extended range of Vendor Command IDs */
#define ID_DAP_CypressModeSwitch        (ID_DAP_VendorExFirst)
#define ID_DAP_CypressGetKP3FWHW        (ID_DAP_VendorExFirst + 1u)
#define ID_DAP_ResetKP3                 (ID_DAP_VendorExFirst + 2u)

#define MODE_BOOTLOADER                 (0x00u)
#define MODE_CMSIS_DAP2X                (0x01u)
#define MODE_CMSIS_DAP1X                (0x02u)
#define MODE_CMSIS_DAP2X_DOUBLEUARTS    (0x03u)

#define KP3_METADATA_VERSION_INFO_ADDR (0x00040000u - Bootloadable_META_DATA_SIZE + Bootloadable_META_APP_VER_OFFSET)
#define KP3_METADATA_VERSION_INFO_SIZE (4u)

/* Enum for Basic DAP Vendor Response */
enum
{
    generalResponse_COMMAND = 0u,
    generalResponse_STATUS = 1u,
    generalResponse_RESULT = 2u
};

/* Enum for Basic DAP Vendor Request */
enum
{
    generalRequest_COMMAND = 0u,
    generalRequest_SUBCOMMAND = 1u
};
/* Enumeration for Response bytes of CMSIS-DAP Extended Vendor Command 1 (get verrsion of kp3 from bootloadeble metadata + kit HW ID) */
enum {
    vA1r_KP3_MAJOR_VERSION = 1u,
    vA1r_KP3_MINOR_VERSION,
    vA1r_BUILD_LO,
    vA1r_BUILD_HI,
    vA1r_HWID
};


//**************************************************************************************************
/**
\defgroup DAP_Vendor_Adapt_gr Adapt Vendor Commands
\ingroup DAP_Vendor_gr
@{

The file DAP_vendor.c provides template source code for extension of a Debug Unit with
Vendor Commands. Copy this file to the project folder of the Debug Unit and add the
file to the MDK-ARM project under the file group Configuration.
*/

/** Process DAP Vendor Command and prepare Response Data
\param request   pointer to request data
\param response  pointer to response data
\return          number of bytes in response (lower 16 bits)
                 number of bytes in request (upper 16 bits)
*/
uint32_t DAP_ProcessVendorCommandEx(const uint8_t *request, uint8_t *response) {
  uint32_t num = ((uint32_t)1u << 16u) | 1u;

  response[generalResponse_COMMAND] = request[generalRequest_COMMAND];        /* copy Command ID */

  switch (request[generalRequest_COMMAND]) {          /* first byte in request is Command ID */
    case ID_DAP_CypressModeSwitch: {
        /* reload to kitprog3 mode or kitprog bootloader
                        COMMAND(OUT Packet)
                        BYTE 0 1010 0000 0xA0
                        BYTE 1 Desired Mode:
                                0x00 – Bootloader
                                0x01 – KitProg3 CMSIS-DAP v.2.xx (USB Bulk)
                                0x02 – KitProg3 CMSIS-DAP v.1.xx (USB HID)
                                0x03 - KitProg3 CMSIS-DAP v.2.xx  with two UARTs
                        If KitProg3 CMSIS-DAP v.2.xx  with two UARTs mode is not supported, kit switches to KitProg3 CMSIS-DAP v.2.xx (USB Bulk)
                        Response shall be issued only if invalid parameters (Byte 1) is received.
        */
        switch (request[generalRequest_SUBCOMMAND])
        {
            case MODE_BOOTLOADER:
                Bootloadable_Load();
                break;
            case MODE_CMSIS_DAP1X:
                SetKitProgActiveApp(KP3_MODE_HID);
                break;
            case MODE_CMSIS_DAP2X:
                SetKitProgActiveApp(KP3_MODE_BULK);
                break;
            case MODE_CMSIS_DAP2X_DOUBLEUARTS:
                SetKitProgActiveApp(KP3_MODE_BULK2UARTS);
                break;
            default:
                response[generalResponse_COMMAND] = DAP_ERROR;
                num = (((uint32_t)2u << 16u) | 1u);
                break;
        }
        break;
    }
    case ID_DAP_CypressGetKP3FWHW: {
        /* get verrsion of kp3 from bootloadeble metadata + kit HW ID
                        COMMAND(OUT Packet)
                        BYTE 0 1010 0001 0xA1
                        RESPONSE(IN Packet)
                        BYTE 0 1010 0001 0xA1
                        BYTE 1 - Major version of KitProg FW
                        BYTE 2 - Minor version of KitProg FW
                        BYTE 3 - Firmware build number LSB
                        BYTE 4 - Firmware build number MSB
                        BYTE 5 - Hardware ID
        */
        const uint8_t *kp3_ver_ptr = (uint8_t *)KP3_METADATA_VERSION_INFO_ADDR;
        // Version of KitProg FW
        (void)memcpy(&response[vA1r_KP3_MAJOR_VERSION], kp3_ver_ptr, KP3_METADATA_VERSION_INFO_SIZE);
        // Hardware ID
        response[vA1r_HWID] = get_kit_hw_id();
        num += (KP3_METADATA_VERSION_INFO_SIZE + 1u); /* increment response count by Version length + Hardware ID byte */
        break;
    }
    case ID_DAP_ResetKP3:
        /* issues software reset for psoc5 device
                        COMMAND(OUT Packet)
                        BYTE 0 1010 0010 0xA2
                        RESPONSE(IN Packet)
                        No response
        */
                        
        CySoftwareReset();
        break;
    default:
        // Unsupported command
        break;
  }

  return (num);
}

///@}
