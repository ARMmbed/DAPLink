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

#include "stdint.h"

#define FW_BUILD "0231"
// URL_NAME and DRIVE_NAME must be 11 characters excluding
// the null terminated character
#if defined(DAPLINK_BL)
    #define DAPLINK_URL_NAME        "HELP_FAQHTM"
    #define DAPLINK_DRIVE_NAME      "MAINTENANCE"
    #define DAPLINK_TARGET_URL      "https://mbed.com/daplink"
#else
    #define DAPLINK_URL_NAME        "MBED    HTM"
    #define DAPLINK_DRIVE_NAME      "DAPLINK    "
    #define DAPLINK_TARGET_URL      "https://mbed.org/device/?code=@A"
#endif

extern const char daplink_drive_name[11];
extern const char daplink_url_name[11];
extern const char * const daplink_target_url;

void update_html_file(uint8_t *buf, uint32_t bufsize);
uint8_t * get_uid_string          (void);
uint8_t   get_len_string_interface(void);
uint8_t * get_uid_string_interface(void);
void      init_auth_config        (void);
void build_mac_string(uint32_t *uuid_data);

#endif
