/**
 * @file    rl_usb.h
 * @brief   USB Device Library header
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

#ifndef __RL_USB_H__
#define __RL_USB_H__

#ifdef __cplusplus
extern "C"  {
#endif

#include <stdint.h>
#include "usb.h"

/*****************  Functions *************************************************/

/* USB Device functions exported from USB Device Core module                  */
extern void  usbd_init(void);
extern void  usbd_connect(BOOL con);
extern void  usbd_reset_core(void);
extern BOOL  usbd_configured(void);

/* USB Device user functions imported to USB HID Class module                 */
extern void  usbd_hid_init(void);
extern BOOL  usbd_hid_get_report_trigger(U8 rid,   U8 *buf, int len);
extern int   usbd_hid_get_report(U8 rtype, U8 rid, U8 *buf, U8  req);
extern void  usbd_hid_set_report(U8 rtype, U8 rid, U8 *buf, int len, U8 req);
extern U8    usbd_hid_get_protocol(void);
extern void  usbd_hid_set_protocol(U8 protocol);

/* USB Device user functions imported to USB Mass Storage Class module        */
extern void  usbd_msc_init(void);
extern void  usbd_msc_read_sect(U32 block, U8 *buf, U32 num_of_blocks);
extern void  usbd_msc_write_sect(U32 block, U8 *buf, U32 num_of_blocks);
extern void  usbd_msc_start_stop(BOOL start);

/* USB Device user functions imported to USB Audio Class module               */
extern void  usbd_adc_init(void);

#ifdef CDC_ENDPOINT
#ifndef USB_CDC_ACM_EP_COUNT
#define USB_CDC_ACM_EP_COUNT 1
#endif
#endif

/* The USB CDC driver can support multiple CDC endpoints. EP 1 is reserved for
 * standard USB to UART communication with the DAPLink target. EP 2 and EP 3
 * functionality is HIC specific and user defined. */
typedef enum {
    USB_CDC_ACM_NUM_1_USB2UART = 0,
#if (USB_CDC_ACM_EP_COUNT > 1)
    USB_CDC_ACM_NUM_2_USER,
#if (USB_CDC_ACM_EP_COUNT > 2)
    USB_CDC_ACM_NUM_3_USER,
#endif
#endif
} usbd_cdc_num_t;

/* USB Device CDC ACM class functions called automatically by USBD Core module*/
extern int32_t USBD_CDC_ACM_Initialize(usbd_cdc_num_t cdc_num);
extern int32_t USBD_CDC_ACM_Uninitialize(usbd_cdc_num_t cdc_num);
extern int32_t USBD_CDC_ACM_Reset(void);
/* USB Device CDC ACM class user functions                                    */
extern int32_t USBD_CDC_ACM_PortInitialize(usbd_cdc_num_t cdc_num);
extern int32_t USBD_CDC_ACM_PortUninitialize(usbd_cdc_num_t cdc_num);
extern int32_t USBD_CDC_ACM_PortReset(usbd_cdc_num_t cdc_num);
extern int32_t USBD_CDC_ACM_PortSetLineCoding(usbd_cdc_num_t cdc_num, CDC_LINE_CODING *line_coding);
extern int32_t USBD_CDC_ACM_PortGetLineCoding(usbd_cdc_num_t cdc_num, CDC_LINE_CODING *line_coding);
extern int32_t USBD_CDC_ACM_PortSetControlLineState(usbd_cdc_num_t cdc_num, uint16_t ctrl_bmp);
extern int32_t USBD_CDC_ACM_DataSend(usbd_cdc_num_t cdc_num, const uint8_t *buf, int32_t len);
extern int32_t USBD_CDC_ACM_DataFree(usbd_cdc_num_t cdc_num);
extern int32_t USBD_CDC_ACM_PutChar(usbd_cdc_num_t cdc_num, const uint8_t  ch);
extern int32_t USBD_CDC_ACM_DataRead(usbd_cdc_num_t cdc_num, uint8_t *buf, int32_t len);
extern int32_t USBD_CDC_ACM_GetChar(usbd_cdc_num_t cdc_num);
extern int32_t USBD_CDC_ACM_DataAvailable(usbd_cdc_num_t cdc_num);
extern int32_t USBD_CDC_ACM_Notify(usbd_cdc_num_t cdc_num, uint16_t stat);
/* USB Device CDC ACM class overridable functions                             */
extern int32_t USBD_CDC_ACM_SendEncapsulatedCommand(usbd_cdc_num_t cdc_num);
extern int32_t USBD_CDC_ACM_GetEncapsulatedResponse(usbd_cdc_num_t cdc_num);
extern int32_t USBD_CDC_ACM_SetCommFeature(usbd_cdc_num_t cdc_num, uint16_t feat);
extern int32_t USBD_CDC_ACM_GetCommFeature(usbd_cdc_num_t cdc_num, uint16_t feat);
extern int32_t USBD_CDC_ACM_ClearCommFeature(usbd_cdc_num_t cdc_num, uint16_t feat);
extern int32_t USBD_CDC_ACM_SetLineCoding(usbd_cdc_num_t cdc_num);
extern int32_t USBD_CDC_ACM_GetLineCoding(usbd_cdc_num_t cdc_num);
extern int32_t USBD_CDC_ACM_SetControlLineState(usbd_cdc_num_t cdc_num, uint16_t ctrl_bmp);
extern int32_t USBD_CDC_ACM_SendBreak(usbd_cdc_num_t cdc_num, uint16_t dur);

/* USB Device user functions imported to USB Custom Class module              */
extern void  usbd_cls_init(void);
extern void  usbd_cls_sof(void);
extern BOOL  usbd_cls_dev_req(BOOL setup);
extern BOOL  usbd_cls_if_req(BOOL setup);
extern BOOL  usbd_cls_ep_req(BOOL setup);

extern void  usbd_bulk_init(void);

#ifdef __cplusplus
}
#endif

#endif  /* __RL_USB_H__ */
