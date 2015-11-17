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
#include "version.h"
#include "target_config.h"
#include "board.h"
#include "read_uid.h"
#include "virtual_fs.h"

char mac_string[16] = {0};
char uuid_string[33] = {0};
char version_string[49+4] = {0};

// Pointers to substitution strings
const char * const fw_version = (const char *)FW_BUILD;

static uint32_t unique_id[4];
static uint32_t auth;
uint8_t string_auth[49 + 4]; //25 + 4
uint8_t string_auth_descriptor[2+49*2]; // 2+25*2
static const char nybble_chars[] = "0123456789abcdef";

static void setup_string_id_auth(void);

static int write_hex8(uint8_t value, uint8_t * str)
{
    *(str + 0) = nybble_chars[ (value >> 4) & 0x0F ];
    *(str + 1) = nybble_chars[ (value >> 0) & 0x0F ];
    return 2;
}

static int write_hex16(uint16_t value, uint8_t * str)
{
    int pos = 0;
    pos += write_hex8((value >> 8) & 0xFF, str + pos);
    pos += write_hex8((value >> 0) & 0xFF, str + pos);
    return pos;
}

static int write_hex32(uint32_t value, uint8_t * str)
{
    int pos = 0;
    pos += write_hex8((value >> 0x18) & 0xFF, str + pos);
    pos += write_hex8((value >> 0x10) & 0xFF, str + pos);
    pos += write_hex8((value >> 0x08) & 0xFF, str + pos);
    pos += write_hex8((value >> 0x00) & 0xFF, str + pos);
    return pos;
}

void build_uuid_string(uint32_t *uuid_data)
{
    int i;
    uint32_t idx = 0;
    for (i = 0; i < 4; i++) {
        idx += write_hex32(uuid_data[i], (uint8_t*)uuid_string + idx);
    }
    uuid_string[idx++] = '\0';
}

void build_mac_string(uint32_t *uuid_data)
{
    uint32_t idx = 0;

    // patch for MAC use. Make sure MSB bits are set correctly
    uuid_data[2] |=  (0x2 << 8);
    uuid_data[2] &= ~(0x1 << 8);

    idx += write_hex16(uuid_data[2] & 0xFFFF, (uint8_t*)mac_string + idx);
    idx += write_hex32(uuid_data[3], (uint8_t*)mac_string + idx);
    mac_string[idx++] = 0;
}

static uint32_t atoi(uint8_t * str, uint8_t size, uint8_t base)
{
    uint32_t k = 0;
    uint8_t i = 0, idx = 0;
    for (i = 0; i < size; i++) {
        if ((uint8_t)(*str) >= '0' && (uint8_t)(*str) <= '9') {
            idx = '0';
        }
        else if ((uint8_t)(*str) >= 'A' && (uint8_t)(*str) <= 'F') {
            idx = 'A' - 10;
        }
        else if ((uint8_t)(*str) >= 'a' && (uint8_t)(*str) <= 'f') {
            idx = 'a' - 10;
        }
        k = k*base + (uint8_t)(*str) - idx;
        str++;
    }
    return k;
}

static void setup_string_version()
{
    uint8_t i = 0, idx = 0;

        /* XXXXYYYY: <board id><fw version> */
    for (i = 0; i < 4; i++) {
        version_string[idx++] = target_device.board_id[i];
    }
    for (i = 0; i < 4; i++) {
        version_string[idx++] = fw_version[i];
    }

        /* Null terminate */
    version_string[idx] = '\0';
}

static void setup_string_id_auth()
{
    uint8_t i = 0, idx = 0;

    string_auth[0] = '$';
    string_auth[1] = '$';
    string_auth[2] = '$';
    string_auth[3] = 24;
    idx += 4;
    // string id
    for (i = 0; i < 4; i++) {
        string_auth[idx++] = target_device.board_id[i];
    }
    for (i = 0; i < 4; i++) {
        string_auth[idx++] = fw_version[i];
    }
    for (i = 0; i < 4; i++) {
        idx += write_hex32(unique_id[i], string_auth + idx);
    }

    //string auth
    idx += write_hex32(auth, string_auth + idx);

    // Null terminate
    string_auth[idx++] = 0;
}

static void setup_string_descriptor()
{
    uint8_t i = 0, idx = 0, len = 0;
    len = strlen((const char *)(string_auth+4));
    string_auth_descriptor[0] = len*2 + 2;
    string_auth_descriptor[1] = 3;
    idx += 2;

    for (i = 0; i < len*2; i++) {
        if ((i % 2) == 0) {
            string_auth_descriptor[idx + i] = string_auth[4 + i/2];
        }
        else {
            string_auth_descriptor[idx + i] = 0;
        }
    }
    idx += len*2;

    string_auth_descriptor[idx] = 0;
}


uint8_t *get_uid_string(void)
{
    return string_auth;
}

uint8_t get_len_string_interface(void)
{
    return 2 + strlen((const char *)(string_auth+4))*2;
}

uint8_t *get_uid_string_interface(void)
{
    return string_auth_descriptor;
}

static void compute_auth()
{
    uint32_t id = 0, fw = 0, sec = 0;
    id = atoi((uint8_t *)target_device.board_id  , 4, 16);
    fw = atoi((uint8_t *)fw_version, 4, 16);
    auth = (id) | (fw << 16);
    auth ^= unique_id[0];
    sec = atoi((uint8_t *)(target_device.secret), 8, 16);
    auth ^= sec;
}

void init_auth_config(void)
{
    read_unique_id(unique_id);
    compute_auth();
    setup_string_id_auth();
    setup_string_descriptor();
    setup_string_version();
}
