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
#ifndef VERSION_H
#define VERSION_H

#include <stdint.h>

#define FW_BUILD "0201"

/**
 Prepare UID for USB and also web auth token. Done before USB connects
 @param  none
 @return none
*/
void unique_string_auth_config(void);

/**
 Create the htm file and pass it into a buffer (done during USB MSC request)
 and there is a note about USB not being enabled...
 @param  buffer - a buffer to write the htm file contents into
 @param  size - the maximum size of the parameter buffer
 @return 1 on success and 0 otherwise
*/
uint8_t update_html_file(uint8_t *buffer, uint32_t size);

/**
 Get the auth string. This is sent to CDC and the contents are the same 
 auth string used in mbed.htm for mbed.org redirection
 @param  none
 @return a pointer to the auth string
*/
uint8_t *get_web_auth_string(void);

/**
 Get the usb interface descriptor string. Used in USB descriptor to identify 1 of 
 many mbeds that can be connected to a single computer
 @param  none
 @return a pointer to the UID string
*/
uint8_t *get_uid_string(void);

/**
 Get the usb interface descriptor string length.
 @param  none
 @return the length of the interface descriptor string
*/
uint8_t get_uid_string_len(void);

#endif
