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
#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

typedef enum {
    USB_IDLE,
    USB_ACTIVE
} USB_BUSY;

typedef enum {
    USB_DISCONNECTED,
    USB_CONNECTING,
    USB_CONNECTED,
    USB_CHECK_CONNECTED,
    USB_CONFIGURED,
    USB_DISCONNECTING,
    USB_DISCONNECT_CONNECT
} USB_CONNECT;

void main_reset_target(uint8_t send_unique_id);
void main_usb_configure_event(void);
void main_usb_busy_event(void);
void main_powerdown_event(void);
void main_disable_debug_event(void);
void main_usb_disconnect_event(void);
void main_blink_dap_led(uint8_t permanent);
void main_blink_msd_led(uint8_t permanent);
void main_blink_cdc_led(uint8_t permanent);

#endif
