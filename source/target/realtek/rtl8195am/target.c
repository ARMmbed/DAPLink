/**
 * @file    target.c
 * @brief   Target information for Realtek RTL8195AM
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2016-2017, Realtek Semiconductor Corp., All Rights Reserved
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

#include "target_config.h"

// The file flash_blob.c must only be included in target.c
#include "flash_blob.c"

// target information
target_cfg_t target_device = {
    .sector_size    = KB(4),
    .sector_cnt     = MB(2) / KB(4),
    .flash_start    = 0x00000000,
    .flash_end      = 0x00000000 + MB(2),
    .ram_start      = 0x10007000,
    .ram_end        = 0x10070000,
    .flash_algo     = (program_target_t *) &flash,
    .erase_reset    = 0,
};

// RTL8195AM's main cpu can only talk 38400 with DAP UART
//#include "string.h"
#include "RTL.h"
#include "rl_usb.h"
#include "usb_for_lib.h"

extern int32_t data_send_access;
extern int32_t data_send_active;
extern int32_t data_send_zlp;
extern int32_t data_to_send_wr;
extern int32_t data_to_send_rd;
extern uint8_t *ptr_data_to_send;
extern uint8_t *ptr_data_sent;
extern int32_t data_read_access;
extern int32_t data_receive_int_access;
extern int32_t data_received_pending_pckts;
extern int32_t data_no_space_for_receive;
extern uint8_t *ptr_data_received;
extern uint8_t *ptr_data_read;
extern uint16_t control_line_state;
extern CDC_LINE_CODING line_coding;

extern int32_t USBD_CDC_ACM_PortReset(void);
extern int32_t USBD_CDC_ACM_PortSetLineCoding(CDC_LINE_CODING *line_coding);

int32_t USBD_CDC_ACM_Reset(void)
{
    data_send_access            = 0;
    data_send_active            = 0;
    data_send_zlp               = 0;
    data_to_send_wr             = 0;
    data_to_send_rd             = 0;
    ptr_data_to_send            = USBD_CDC_ACM_SendBuf;
    ptr_data_sent               = USBD_CDC_ACM_SendBuf;
    data_read_access            = 0;
    data_receive_int_access     = 0;
    data_received_pending_pckts = 0;
    data_no_space_for_receive   = 0;
    ptr_data_received           = USBD_CDC_ACM_ReceiveBuf;
    ptr_data_read               = USBD_CDC_ACM_ReceiveBuf;
    control_line_state          = 0;
    USBD_CDC_ACM_PortReset();
    line_coding.dwDTERate       = 38400;
    line_coding.bCharFormat     = 0;
    line_coding.bParityType     = 0;
    line_coding.bDataBits       = 8;

    return (USBD_CDC_ACM_PortSetLineCoding(&line_coding));
}

int32_t USBD_CDC_ACM_SetLineCoding(void)
{
    line_coding.dwDTERate = 38400;
    line_coding.bCharFormat =  USBD_EP0Buf[4];
    line_coding.bParityType =  USBD_EP0Buf[5];
    line_coding.bDataBits   =  USBD_EP0Buf[6];

    return (USBD_CDC_ACM_PortSetLineCoding(&line_coding));
}
