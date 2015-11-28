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

#include "main.h"
#include "info.h"
#include "target_config.h"
#include "board.h"
#include "read_uid.h"
#include "virtual_fs.h"
#include "util.h"

// Raw variables
static uint32_t host_id[4];
static uint32_t target_id[4];
static uint32_t hdk_id = DAPLINK_HDK_ID;

// Strings
static char string_unique_id[48 + 1];
static char string_mac[12 + 1];
static char string_board_id[4 + 1];
static char string_host_id[32 + 1];
static char string_target_id[32 + 1];
static char string_hdk_id[8 + 1];
static char string_version[4 + 1];

static char usb_desc_unique_id[2 + sizeof(string_unique_id) * 2];


const char * info_get_unique_id(void)
{
    return string_unique_id;
}

const char * info_get_board_id(void)
{
    return string_board_id;
}

const char * info_get_host_id(void)
{
    return string_host_id;
}

const char * info_get_target_id(void)
{
    return string_target_id;
}

const char * info_get_hdk_id(void)
{
    return string_hdk_id;
}
const char * info_get_version(void)
{
    return string_version;
}
const char * info_get_mac(void)
{
    return string_mac;
}

const char * info_get_unique_id_string_descriptor(void)
{
    return usb_desc_unique_id;
}

static void setup_basics()
{
    uint8_t i = 0, idx = 0;
    memset(string_board_id, 0, sizeof(string_board_id));
    memset(string_host_id, 0, sizeof(string_host_id));
    memset(string_target_id, 0, sizeof(string_target_id));
    memset(string_hdk_id, 0, sizeof(string_hdk_id));
    memset(string_board_id, 0, sizeof(string_board_id));

    // Host ID
    idx = 0;
    for (i = 0; i < 4; i++) {
        idx += util_write_hex32(string_host_id + idx, host_id[i]);
    }
    string_host_id[idx++] = 0;

    // Target ID
    idx = 0;
    for (i = 0; i < 4; i++) {
        idx += util_write_hex32(string_target_id + idx, target_id[i]);
    }
    string_target_id[idx++] = 0;

    // HDK ID
    idx = 0;
    idx += util_write_hex32(string_hdk_id + idx, hdk_id);
    string_hdk_id[idx++] = 0;

    // Board ID
    memcpy(string_board_id, target_device.board_id, 4);
    string_board_id[4] = 0;

    // Version
    idx = 0;
    string_version[idx++] = '0' + (DAPLINK_VERSION / 1000) % 10;
    string_version[idx++] = '0' + (DAPLINK_VERSION / 100 ) % 10;
    string_version[idx++] = '0' + (DAPLINK_VERSION / 10  ) % 10;
    string_version[idx++] = '0' + (DAPLINK_VERSION / 1   ) % 10;
    string_version[idx++] = 0;
}

static void setup_unique_id()
{
    memset(string_unique_id, 0, sizeof(string_unique_id));

    strcat(string_unique_id, string_board_id);
    strcat(string_unique_id, "0000");           // Reserved - was version number
    strcat(string_unique_id, string_host_id);
    strcat(string_unique_id, string_hdk_id);
}

static void setup_string_descriptor()
{
    uint8_t i = 0, idx = 0, len = 0;
    len = strlen((const char *)string_unique_id);
    
    // bLength
    usb_desc_unique_id[idx++] = len*2 + 2;
    
    // bDescriptorType
    usb_desc_unique_id[idx++] = 3;

    // bString
    for (i = 0; i < len; i++) {
        usb_desc_unique_id[idx++] = string_unique_id[i];
        usb_desc_unique_id[idx++] = 0;
    }
}

void info_init(void)
{
    read_unique_id(host_id);
    setup_basics();
    setup_unique_id();
    setup_string_descriptor();
}

void info_set_uuid_target(uint32_t *uuid_data)
{
    uint32_t idx = 0;

    // Save the target ID
    memcpy(target_id, uuid_data, 16);
    
    // patch for MAC use. Make sure MSB bits are set correctly
    uuid_data[2] |=  (0x2 << 8);
    uuid_data[2] &= ~(0x1 << 8);

    idx += util_write_hex16(string_mac + idx, uuid_data[2] & 0xFFFF);
    idx += util_write_hex32(string_mac + idx, uuid_data[3]);
    string_mac[idx++] = 0;
}
