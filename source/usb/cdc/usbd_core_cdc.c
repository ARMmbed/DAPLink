/**
 * @file    usbd_core_cdc.c
 * @brief   Communication Device Class driver
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

#include <string.h>

#include "rl_usb.h"
#include "usb_for_lib.h"


/*
 *  USB Device Endpoint 0 Event Callback - CDC specific handling (Setup Request To Interface)
 *    Parameters:      none
 *    Return Value:    TRUE - Setup class request ok, FALSE - Setup class request not supported
 */

__WEAK BOOL USBD_EndPoint0_Setup_CDC_ReqToIF(void)
{
    for (usbd_cdc_num_t cdc_num = 0; cdc_num < USB_CDC_ACM_EP_COUNT; cdc_num++)
    {
        if ((USBD_SetupPacket.wIndexL == usbd_cdc_acm_cif_num[cdc_num])  || /* IF number correct? */
            (USBD_SetupPacket.wIndexL == usbd_cdc_acm_dif_num[cdc_num])) {
            switch (USBD_SetupPacket.bRequest) {
                case CDC_SEND_ENCAPSULATED_COMMAND:
                    USBD_EP0Data.pData = USBD_EP0Buf;                    /* data to be received, see USBD_EVT_OUT */
                    return (__TRUE);

                case CDC_GET_ENCAPSULATED_RESPONSE:
                    if (USBD_CDC_ACM_GetEncapsulatedResponse(cdc_num)) {
                        USBD_EP0Data.pData = USBD_EP0Buf;                  /* point to data to be sent */
                        USBD_DataInStage();                                /* send requested data */
                        return (__TRUE);
                    }

                    break;

                case CDC_SET_COMM_FEATURE:
                    USBD_EP0Data.pData = USBD_EP0Buf;                    /* data to be received, see USBD_EVT_OUT */
                    return (__TRUE);

                case CDC_GET_COMM_FEATURE:
                    if (USBD_CDC_ACM_GetCommFeature(cdc_num, USBD_SetupPacket.wValue)) {
                        USBD_EP0Data.pData = USBD_EP0Buf;                  /* point to data to be sent */
                        USBD_DataInStage();                                /* send requested data */
                        return (__TRUE);
                    }

                    break;

                case CDC_CLEAR_COMM_FEATURE:
                    if (USBD_CDC_ACM_ClearCommFeature(cdc_num, USBD_SetupPacket.wValue)) {
                        USBD_StatusInStage();                              /* send Acknowledge */
                        return (__TRUE);
                    }

                    break;

                case CDC_SET_LINE_CODING:
                    USBD_EP0Data.pData = USBD_EP0Buf;                    /* data to be received, see USBD_EVT_OUT */
                    return (__TRUE);

                case CDC_GET_LINE_CODING:
                    if (USBD_CDC_ACM_GetLineCoding(cdc_num)) {
                        USBD_EP0Data.pData = USBD_EP0Buf;                  /* point to data to be sent */
                        USBD_DataInStage();                                /* send requested data */
                        return (__TRUE);
                    }

                    break;

                case CDC_SET_CONTROL_LINE_STATE:
                    if (USBD_CDC_ACM_SetControlLineState(cdc_num, USBD_SetupPacket.wValue)) {
                        USBD_StatusInStage();                              /* send Acknowledge */
                        return (__TRUE);
                    }

                    break;

                case CDC_SEND_BREAK:
                    if (USBD_CDC_ACM_SendBreak(cdc_num, USBD_SetupPacket.wValue)) {
                        USBD_StatusInStage();                              /* send Acknowledge */
                        return (__TRUE);
                    }
                    break;
            }
        }

    }

    return (__FALSE);
}


/*
 *  USB Device Endpoint 0 Event Callback - CDC specific handling (Out Request To Interface)
 *    Parameters:      none
 *    Return Value:    TRUE - Out class request ok, FALSE - Out class request not supported
 */

__WEAK BOOL USBD_EndPoint0_Out_CDC_ReqToIF(void)
{
    for (usbd_cdc_num_t cdc_num = 0; cdc_num < USB_CDC_ACM_EP_COUNT; cdc_num++)
    {
        if ((USBD_SetupPacket.wIndexL == usbd_cdc_acm_cif_num[cdc_num])  || /* IF number correct? */
            (USBD_SetupPacket.wIndexL == usbd_cdc_acm_dif_num[cdc_num])) {
            switch (USBD_SetupPacket.bRequest) {
                case CDC_SEND_ENCAPSULATED_COMMAND:
                    if (USBD_CDC_ACM_SendEncapsulatedCommand(cdc_num)) {
                        USBD_StatusInStage();                        /* send Acknowledge */
                        return (__TRUE);
                    }

                    break;

                case CDC_SET_COMM_FEATURE:
                    if (USBD_CDC_ACM_SetCommFeature(cdc_num, USBD_SetupPacket.wValue)) {
                        USBD_StatusInStage();                        /* send Acknowledge */
                        return (__TRUE);
                    }

                    break;

                case CDC_SET_LINE_CODING:
                    if (USBD_CDC_ACM_SetLineCoding(cdc_num)) {
                        USBD_StatusInStage();                        /* send Acknowledge */
                        return (__TRUE);
                    }

                    break;
            }
        }
    }

    return (__FALSE);
}
