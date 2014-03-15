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

/**
 @enum USB_BUSY
 @brief Defines the state of the USB connection
 */ 
typedef enum {
    USB_IDLE,   /*!< USB_IDLE value means no bus activity */
    USB_ACTIVE  /*!< USB_ACTIVE value means MSC transfers are occuring */
} USB_BUSY;

/**
 @enum USB_CONNECT
 @brief Statest the USB connection can be in
 */ 
typedef enum {
    USB_DISCONNECTED,       /*!< USB_DISCONNECTED state for a USB */
    USB_CONNECTING,         /*!< USB_CONNECTING state for a USB */
    USB_CONNECTED,          /*!< USB_CONNECTED state for a USB */
    USB_CHECK_CONNECTED,    /*!< USB_CHECK_CONNECTED state for a USB */
    USB_DISCONNECTING,      /*!< USB_DISCONNECTING state for a USB */
    USB_DISCONNECT_CONNECT  /*!< USB_DISCONNECT_CONNECT state for a USB */
} USB_CONNECT;

/**
 Allows other parts of the program to request a reset the countdown to USB idle
 @param  none
 @return none
*/
void main_usb_busy_event(void);

/**
 Allows other parts of the program to request the device reset itself
 @param  none
 @return none
*/
void main_usb_disconnect_event(void);

/**
 Allows other parts of the program to request a LED to toggle state
 @param  permanent decides if the led should remain on or flash
 @return none
*/
void main_blink_msd_led(uint8_t permanent);

#endif
