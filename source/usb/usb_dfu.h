/**
 * @file    usb_dfu.h
 * @brief   USB Device Firmware Upgrade driver header
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

#ifndef __USB_DFU_H__
#define __USB_DFU_H__

/* DFU Descriptor Types */
#define DFU_FUNCTIONAL_DESCRIPTOR_TYPE          0x21

/* DFU Subclass Codes */
#define DFU_SUBCLASS_DFU                        0x01

/* DFU Protocol Codes */
#define DFU_PROTOCOL_RUNTIME_MODE               0x01
#define DFU_PROTOCOL_DFU_MODE                   0x02

/* DFU Functional Descriptor */
typedef __packed struct _DFU_FUNCTIONAL_DESCRIPTOR {
    U8  bLength;
    U8  bDescriptorType;
    U8  bmAttributes;
    U16 wDetachTimeout;
    U16 wTransferSize;
    U16 bcdDFUVersion;
} DFU_FUNCTIONAL_DESCRIPTOR;

#define USB_DFU_FUNCTIONAL_DESCRIPTOR_SIZE      (sizeof(DFU_FUNCTIONAL_DESCRIPTOR))

/* DFU Request Codes */
#define DFU_REQUEST_DETACH                      0x00
#define DFU_REQUEST_DNLOAD                      0x01
#define DFU_REQUEST_UPLOAD                      0x02
#define DFU_REQUEST_GETSTATUS                   0x03
#define DFU_REQUEST_CLRSTATUS                   0x04
#define DFU_REQUEST_GETSTATE                    0x05
#define DFU_REQUEST_ABORT                       0x06

/* bmAttributes in the DFU functional descriptor */
#define DFU_ATTR_WILLDETACH                     0x08
#define DFU_ATTR_MANIFESTATIONTOLERANT          0x04
#define DFU_ATTR_CANUPLOAD                      0x02
#define DFU_ATTR_CANDNLOAD                      0x01

/* DFU status codes */
#define DFU_STATUS_OK                           0x00
#define DFU_STATUS_ERR_TARGET                   0x01
#define DFU_STATUS_ERR_FILE                     0x02
#define DFU_STATUS_ERR_WRITE                    0x03
#define DFU_STATUS_ERR_ERASE                    0x04
#define DFU_STATUS_ERR_CHECK_ERASED             0x05
#define DFU_STATUS_ERR_PROG                     0x06
#define DFU_STATUS_ERR_VERIFY                   0x07
#define DFU_STATUS_ERR_ADDRESS                  0x08
#define DFU_STATUS_ERR_NOTDONE                  0x09
#define DFU_STATUS_ERR_FIRMWARE                 0x0A
#define DFU_STATUS_ERR_VENDOR                   0x0B
#define DFU_STATUS_ERR_USBR                     0x0C
#define DFU_STATUS_ERR_POR                      0x0D
#define DFU_STATUS_ERR_UNKNOWN                  0x0E
#define DFU_STATUS_ERR_STALLEDPKT               0x0F

/* DFU GETSTATUS response */
typedef __packed struct _DFU_GETSTATUS_RESPONSE {
    U8  bStatus;
    U8  bwPollTimeout[3];
    U8  bState;
    U8  iString;
} DFU_GETSTATUS_RESPONSE;

/* DFU states */
#define DFU_STATE_APP_IDLE                      0x00
#define DFU_STATE_APP_DETACH                    0x01
#define DFU_STATE_DFU_IDLE                      0x02
#define DFU_STATE_DFU_DNLOAD_SYNC               0x03
#define DFU_STATE_DFU_DNBUSY                    0x04
#define DFU_STATE_DFU_DNLOAD_IDLE               0x05
#define DFU_STATE_DFU_MANIFEST_SYNC             0x06
#define DFU_STATE_DFU_MANIFEST                  0x07
#define DFU_STATE_DFU_MANIFEST_WAIT_RESET       0x08
#define DFU_STATE_DFU_UPLOAD_IDLE               0x09
#define DFU_STATE_DFU_ERROR                     0x0A

/* DFU GETSTATE response */
typedef __packed struct _DFU_GETSTATE_RESPONSE {
    U8  bState;
} DFU_GETSTATE_RESPONSE;

#endif  /* __USB_DFU_H__ */
