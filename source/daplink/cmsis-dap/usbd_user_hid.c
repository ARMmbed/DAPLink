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

#include "main.h"

#if (USBD_HID_OUTREPORT_MAX_SZ != DAP_PACKET_SIZE)
#error "USB HID Output Report Size must match DAP Packet Size"
#endif
#if (USBD_HID_INREPORT_MAX_SZ != DAP_PACKET_SIZE)
#error "USB HID Input Report Size must match DAP Packet Size"
#endif

#define FREE_SEM_INIT_COUNT          (DAP_PACKET_COUNT)
#define PROC_SEM_INIT_COUNT          0
#define SEND_SEM_INIT_COUNT          0

static uint8_t temp_buf                      [DAP_PACKET_SIZE];
static uint8_t USB_Request [DAP_PACKET_COUNT][DAP_PACKET_SIZE];  // Request  Buffer

static OS_SEM free_sem;
static OS_SEM proc_sem;
static OS_SEM send_sem;

static OS_MUT hid_mutex;

// Only used by HID out thread
static uint32_t recv_idx;

// Only used by hid_process
static uint32_t proc_idx;

// Used by hid_process and HID out thread
// so must be synchronized to HID lock
static uint32_t send_idx;
static volatile uint8_t  USB_ResponseIdle;

// USB HID Callback: when system initializes
void usbd_hid_init(void)
{
    recv_idx = 0;
    proc_idx = 0;
    send_idx = 0;
    USB_ResponseIdle = 1;
    os_sem_init(&free_sem, FREE_SEM_INIT_COUNT);
    os_sem_init(&proc_sem, PROC_SEM_INIT_COUNT);
    os_sem_init(&send_sem, SEND_SEM_INIT_COUNT);
    os_mut_init(&hid_mutex);
}

// USB HID Callback: when data needs to be prepared for the host
int usbd_hid_get_report(U8 rtype, U8 rid, U8 *buf, U8 req)
{
    switch (rtype) {
        case HID_REPORT_INPUT:
            switch (req) {
                case USBD_HID_REQ_EP_CTRL:
                case USBD_HID_REQ_PERIOD_UPDATE:
                    break;

                case USBD_HID_REQ_EP_INT:
                    os_mut_wait(&hid_mutex, 0xFFFF);

                    if (os_sem_wait(&send_sem, 0) == OS_R_OK) {
                        memcpy(buf, USB_Request[send_idx], DAP_PACKET_SIZE);
                        send_idx = (send_idx + 1) % DAP_PACKET_COUNT;
                        os_sem_send(&free_sem);
                        os_mut_release(&hid_mutex);
                        return (DAP_PACKET_SIZE);
                    } else {
                        USB_ResponseIdle = 1;
                    }

                    os_mut_release(&hid_mutex);
                    break;
            }

            break;

        case HID_REPORT_FEATURE:
            break;
    }

    return (0);
}

// USB HID Callback: when data is received from the host
void usbd_hid_set_report(U8 rtype, U8 rid, U8 *buf, int len, U8 req)
{
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
            if (os_sem_wait(&free_sem, 0) == OS_R_OK) {
                memcpy(USB_Request[recv_idx], buf, len);
                recv_idx = (recv_idx + 1) % DAP_PACKET_COUNT;
                os_sem_send(&proc_sem);
            }

            break;

        case HID_REPORT_FEATURE:
            break;
    }
}

// CMSIS-DAP task
__task void hid_process(void *argv)
{
    while (1) {
        // Process DAP Command
        os_sem_wait(&proc_sem, 0xFFFF);
        DAP_ProcessCommand(USB_Request[proc_idx], temp_buf);
        memcpy(USB_Request[proc_idx], temp_buf, DAP_PACKET_SIZE);
        proc_idx = (proc_idx + 1) % DAP_PACKET_COUNT;
        os_sem_send(&send_sem);
        // Send input report if USB is idle
        os_mut_wait(&hid_mutex, 0xFFFF);

        if (USB_ResponseIdle) {
            USB_ResponseIdle = 0;
            os_sem_wait(&send_sem, 0xFFFF);
            usbd_hid_get_report_trigger(0, USB_Request[send_idx], DAP_PACKET_SIZE);
            send_idx = (send_idx + 1) % DAP_PACKET_COUNT;
            os_sem_send(&free_sem);
        }

        os_mut_release(&hid_mutex);
        main_blink_hid_led(MAIN_LED_OFF);
    }
}
