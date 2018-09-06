/**
 * @file    usbd_user_hid.c
 * @brief   HID driver for CMSIS-DAP packet processing
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
#include "usb.h"
#define __NO_USB_LIB_C
#include "usb_config.c"
#include "DAP_config.h"
#include "DAP.h"
#include "util.h"

#include "main.h"

#if (USBD_HID_OUTREPORT_MAX_SZ != DAP_PACKET_SIZE)
#error "USB HID Output Report Size must match DAP Packet Size"
#endif
#if (USBD_HID_INREPORT_MAX_SZ != DAP_PACKET_SIZE)
#error "USB HID Input Report Size must match DAP Packet Size"
#endif

#define FREE_COUNT_INIT          (DAP_PACKET_COUNT)
#define SEND_COUNT_INIT          0

static uint8_t USB_Request [DAP_PACKET_COUNT][DAP_PACKET_SIZE];  // Request  Buffer

static uint32_t free_count;
static uint32_t send_count;

// Only used on USB thread
static uint32_t recv_idx;
static uint32_t send_idx;
static volatile uint8_t  USB_ResponseIdle;

void hid_send_packet()
{
    if (send_count) {
        send_count--;
        usbd_hid_get_report_trigger(0, USB_Request[send_idx], DAP_PACKET_SIZE);
        send_idx = (send_idx + 1) % DAP_PACKET_COUNT;
        free_count++;
    }
}

// USB HID Callback: when system initializes
void usbd_hid_init(void)
{
    recv_idx = 0;
    send_idx = 0;
    USB_ResponseIdle = 1;
    free_count = FREE_COUNT_INIT;
    send_count = SEND_COUNT_INIT;
}

// USB HID Callback: when data needs to be prepared for the host
int usbd_hid_get_report(U8 rtype, U8 rid, U8 *buf, U8 req)
{
    switch (rtype) {
        case HID_REPORT_INPUT:
            switch (req) {
                case USBD_HID_REQ_PERIOD_UPDATE:
                    break;

                case USBD_HID_REQ_EP_CTRL:
                case USBD_HID_REQ_EP_INT:
                    if (send_count > 0) {
                        send_count--;
                        memcpy(buf, USB_Request[send_idx], DAP_PACKET_SIZE);
                        send_idx = (send_idx + 1) % DAP_PACKET_COUNT;
                        free_count++;
                        return (DAP_PACKET_SIZE);
                    } else if (req == USBD_HID_REQ_EP_INT) {
                        USB_ResponseIdle = 1;
                    }
                    break;
            }

            break;

        case HID_REPORT_FEATURE:
            break;
    }

    return (0);
}

// USB HID override function return 1 if the activity is trivial or response is null 
__attribute__((weak))
uint8_t usbd_hid_no_activity(U8 *buf)
{
    return 0;
}

// USB HID Callback: when data is received from the host
void usbd_hid_set_report(U8 rtype, U8 rid, U8 *buf, int len, U8 req)
{
    main_led_state_t led_next_state = MAIN_LED_FLASH;
    switch (rtype) {
        case HID_REPORT_OUTPUT:
            if (len == 0) {
                break;
            }

            if (buf[0] == ID_DAP_TransferAbort) {
                DAP_TransferAbort = 1;
                break;
            }

            // Store data into request packet buffer
            // If there are no free buffers discard the data
            if (free_count > 0) {
                free_count--;
                memcpy(USB_Request[recv_idx], buf, len);
                DAP_ExecuteCommand(buf, USB_Request[recv_idx]);
                if(usbd_hid_no_activity(USB_Request[recv_idx]) == 1){
                    //revert HID LED to default if the response is null
                    led_next_state = MAIN_LED_DEF;
                }
                recv_idx = (recv_idx + 1) % DAP_PACKET_COUNT;
                send_count++;
                if (USB_ResponseIdle) {
                    hid_send_packet();
                    USB_ResponseIdle = 0;
                }
            } else {
                util_assert(0);
            }
            
            main_blink_hid_led(led_next_state);

            break;

        case HID_REPORT_FEATURE:
            break;
    }
}


