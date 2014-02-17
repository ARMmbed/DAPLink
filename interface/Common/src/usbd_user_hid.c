/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <string.h>
#include <RTL.h>
#include <rl_usb.h>
#include <usb.h>
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

static volatile uint8_t  USB_RequestFlag;       // Request  Buffer Usage Flag
static volatile uint32_t USB_RequestIn;         // Request  Buffer In  Index
static volatile uint32_t USB_RequestOut;        // Request  Buffer Out Index

static volatile uint8_t  USB_ResponseIdle;      // Response Buffer Idle  Flag
static volatile uint8_t  USB_ResponseFlag;      // Response Buffer Usage Flag
static volatile uint32_t USB_ResponseIn;        // Response Buffer In  Index
static volatile uint32_t USB_ResponseOut;       // Response Buffer Out Index

static          uint8_t  USB_Request [DAP_PACKET_COUNT][DAP_PACKET_SIZE];  // Request  Buffer
static          uint8_t  USB_Response[DAP_PACKET_COUNT][DAP_PACKET_SIZE];  // Response Buffer

// Reference to the dap task
static OS_TID dapTask;

// dap paquet received event to release the dap task waiting for this event
#define DAP_PAQUET_RECEIVED        (0x0001)

// USB HID Callback: when system initializes
void usbd_hid_init (void) {
    USB_RequestFlag   = 0;
    USB_RequestIn     = 0;
    USB_RequestOut    = 0;
    USB_ResponseIdle  = 1;
    USB_ResponseFlag  = 0;
    USB_ResponseIn    = 0;
    USB_ResponseOut   = 0;
}

// USB HID Callback: when data needs to be prepared for the host
int usbd_hid_get_report (U8 rtype, U8 rid, U8 *buf, U8 req) {
    switch (rtype) {
        case HID_REPORT_INPUT:
            switch (req) {
                case USBD_HID_REQ_EP_CTRL:
                case USBD_HID_REQ_PERIOD_UPDATE:
                    break;
                case USBD_HID_REQ_EP_INT:
                    if ((USB_ResponseOut != USB_ResponseIn) || USB_ResponseFlag) {
                        memcpy(buf, USB_Response[USB_ResponseOut], DAP_PACKET_SIZE);
                        USB_ResponseOut++;
                        if (USB_ResponseOut == DAP_PACKET_COUNT) {
                            USB_ResponseOut = 0;
                        }
                        if (USB_ResponseOut == USB_ResponseIn) {
                            USB_ResponseFlag = 0;
                        }
                        return (DAP_PACKET_SIZE);
                    } else {
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

// USB HID Callback: when data is received from the host
void usbd_hid_set_report (U8 rtype, U8 rid, U8 *buf, int len, U8 req) {
    switch (rtype) {
        case HID_REPORT_OUTPUT:
            if (len == 0) break;
            if (buf[0] == ID_DAP_TransferAbort) {
                DAP_TransferAbort = 1;
                break;
            }
            if (USB_RequestFlag && (USB_RequestIn == USB_RequestOut)) {
                break;  // Discard packet when buffer is full
            }
            // Store data into request packet buffer
            memcpy(USB_Request[USB_RequestIn], buf, len);
            USB_RequestIn++;
            if (USB_RequestIn == DAP_PACKET_COUNT) {
                USB_RequestIn = 0;
            }
            if (USB_RequestIn == USB_RequestOut) {
                USB_RequestFlag = 1;
            }
            os_evt_set(DAP_PAQUET_RECEIVED, dapTask);
            break;
        case HID_REPORT_FEATURE:
            break;
    }
}


// Process USB HID Data
void usbd_hid_process (void) {
    uint32_t n;

    // Process pending requests
    while ((USB_RequestOut != USB_RequestIn) || USB_RequestFlag) {
        // Process DAP Command and prepare response
        DAP_ProcessCommand(USB_Request[USB_RequestOut], USB_Response[USB_ResponseIn]);

        // Update request index and flag
        USB_RequestOut = (USB_RequestOut +1) % DAP_PACKET_COUNT;
        if (USB_RequestOut == USB_RequestIn) {
            USB_RequestFlag = 0;
        }

        if (USB_ResponseIdle) {
            // Request that data is send back to host
            USB_ResponseIdle = 0;
            usbd_hid_get_report_trigger(0, USB_Response[USB_ResponseIn], DAP_PACKET_SIZE);
        } else {
            // Update response index and flag
            n = USB_ResponseIn + 1;
            if (n == DAP_PACKET_COUNT) {
                n = 0;
            }
            USB_ResponseIn = n;
            if (USB_ResponseIn == USB_ResponseOut) {
                USB_ResponseFlag = 1;
            }
        }
    }
}


// CMSIS-DAP task
__task void hid_process(void * argv) {
    dapTask = os_tsk_self();
    while (1) {
        os_evt_wait_or(DAP_PAQUET_RECEIVED, 0xffff);
        usbd_hid_process ();
        main_blink_dap_led(0);
    }
}

