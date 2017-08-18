/**
 * @file    usbd_core_dfu.c
 * @brief   Device Firmware Upgrade class driver
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

#include "string.h"

#include "RTL.h"
#include "rl_usb.h"
#include "usb_for_lib.h"

/*
 *  USB Device Endpoint 0 Event Callback - DFU specific handling (Setup Request To Interface)
 *    Parameters:      none
 *    Return Value:    TRUE - Setup class request ok, FALSE - Setup class request not supported
 */

__weak BOOL USBD_EndPoint0_Setup_DFU_ReqToIF(void)
{
    if (USBD_SetupPacket.wIndexL == usbd_dfu_if_num) {               /* IF number correct? */
        switch (USBD_SetupPacket.bRequest) {
            case DFU_REQUEST_UPLOAD:
                /* Not implemented */
                return (__FALSE);

            case DFU_REQUEST_GETSTATUS:
                if (USBD_DFU_GetStatus()) {
                    USBD_EP0Data.pData = USBD_EP0Buf;
                    USBD_EP0Data.Count = sizeof(DFU_GETSTATUS_RESPONSE);
                    USBD_DataInStage();
                    USBD_DFU_GetStatusAction();
                    return (__TRUE);
                }

                break;

            case DFU_REQUEST_GETSTATE:
                if (USBD_DFU_GetState()) {
                    USBD_EP0Data.pData = USBD_EP0Buf;
                    USBD_EP0Data.Count = sizeof(DFU_GETSTATE_RESPONSE);
                    USBD_DataInStage();
                    return (__TRUE);
                }

                break;

            case DFU_REQUEST_DNLOAD:
                if (USBD_DFU_StartDnload(USBD_SetupPacket.wLength)) {
                    if (USBD_SetupPacket.wLength > 0) {
                        USBD_EP0Data.pData = USBD_DFU_TransferBuf;
                    } else {
                        // Ack the ZLP immediately
                        USBD_StatusInStage();
                    }
                    return (__TRUE);
                }

                break;

            case DFU_REQUEST_ABORT:
                if (USBD_DFU_Abort()) {
                    USBD_StatusInStage();
                    return (__TRUE);
                }

                break;

            case DFU_REQUEST_CLRSTATUS:
                if (USBD_DFU_ClrStatus()) {
                    USBD_StatusInStage();
                    return (__TRUE);
                }

                break;

        }
    }

    return (__FALSE);
}


/*
 *  USB Device Endpoint 0 Event Callback - DFU specific handling (Out Request To Interface)
 *    Parameters:      none
 *    Return Value:    TRUE - Out class request ok, FALSE - Out class request not supported
 */

__weak BOOL USBD_EndPoint0_Out_DFU_ReqToIF(void)
{
    if (USBD_SetupPacket.wIndexL == usbd_dfu_if_num) {           /* IF number correct? */
        switch (USBD_SetupPacket.bRequest) {
            case DFU_REQUEST_DNLOAD:
                if (USBD_DFU_SyncDnload()) {
                    USBD_StatusInStage();
                    return (__TRUE);
                }

                break;
        }
    }

    return (__FALSE);
}
