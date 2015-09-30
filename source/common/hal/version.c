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
__attribute__((weak))
//Drive and URL must be 11 characters to follow the 8.3 convention
const char daplink_drive_name[11] = DAPLINK_DRIVE_NAME;
__attribute__((weak))
const char daplink_url_name[11] = DAPLINK_URL_NAME;
__attribute__((weak))
const char * const daplink_target_url = DAPLINK_TARGET_URL;

static uint32_t unique_id[4];
static uint8_t already_unique_id = 0;
static uint32_t auth;
uint8_t string_auth[49 + 4]; //25 + 4
uint8_t string_auth_descriptor[2+49*2]; // 2+25*2
static const char nybble_chars[] = "0123456789abcdef";

static void setup_string_id_auth(void);

static void get_byte_hex( uint8_t b, uint8_t *ch1, uint8_t *ch2 ) {
    *ch1 = nybble_chars[ ( b >> 4 ) & 0x0F ];
    *ch2 = nybble_chars[ b & 0x0F ];
}

void build_uuid_string(uint32_t *uuid_data)
{
    uint32_t i = 0, j = 0, k = 0;
    uint8_t b = 0;
    // format UUID
    for (; j<32; j+=8) {
        for (; i<4; i++) {
            b = uuid_data[k] >> (24-(i*8));
            get_byte_hex(b, (uint8_t *)&uuid_string[j+2*i], (uint8_t *)&uuid_string[j+2*i+1]);
        }
        i = 0;
        k++;
    }
    uuid_string[32] = '\0';
}

void build_mac_string(uint32_t *uuid_data)
{
    uint32_t i = 0;
    uint8_t b = 0;
    // patch for MAC use. Make sure MSB bits are set correctly
    uuid_data[2] |=  (0x2 << 8);
    uuid_data[2] &= ~(0x1 << 8);
    for (; i<2; i++) {
        b = uuid_data[2] >> (8-(i*8));
        get_byte_hex(b, (uint8_t *)&mac_string[i*2], (uint8_t *)&mac_string[i*2+1]);
    }
    for (; i<6; i++) {
        b = uuid_data[3] >> (24-((i-2)*8));
        get_byte_hex(b, (uint8_t *)&mac_string[i*2], (uint8_t *)&mac_string[i*2+1]);
    }
    mac_string[15] = '\0';
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
    uint8_t i = 0, j = 0, idx = 0;

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
        for(j = 0; j < 4; j++) {
            get_byte_hex((unique_id[j] >> 8*(3 - i)) & 0xff, &string_auth[idx + 2*i + 8*j], &string_auth[idx + 2*i + 1] + 8*j);
        }
    }
    idx+=32; //8

    //string auth
    for (i = 0; i < 4; i++) {
        get_byte_hex((auth >> 8*(3 - i)) & 0xff, &string_auth[idx + 2*i], &string_auth[idx + 2*i + 1]);
    }
    idx+=8;
    string_auth[idx] = 0;
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
static void insert(uint8_t *buf, uint8_t* new_str, uint32_t strip_count)
{
    uint32_t buf_len = strlen((const char *)buf);
    uint32_t str_len = strlen((const char *)new_str);
    // push out string
    memmove(buf + str_len, buf + strip_count, buf_len - strip_count);
    // insert
    memcpy(buf, new_str, str_len);
}

void update_html_file(uint8_t *buf, uint32_t bufsize)
{
    uint32_t size_left;
    uint8_t *orig_buf = buf;
    uint8_t *insert_string;

    if (0 == already_unique_id) {
        init_auth_config();
        already_unique_id = 1;
    }

    // Zero out buffer so strlen operations don't go out of bounds
    memset(buf, 0, bufsize);
    memcpy(buf, mbed_redirect_file, strlen((const char *)mbed_redirect_file));
    do {
        // Look for key or the end of the string
        while ((*buf != '@') && (*buf != 0)) buf++;

        // If key was found then replace it
        if ('@' == *buf) {
            switch(*(buf+1)) {
                case 'a':
                case 'A':   // platform ID string
                    insert_string = string_auth + 4;
                    break;

                case 'm':
                case 'M':   // MAC address
                    insert_string = (uint8_t *)mac_string;
                    break;

                case 'u':
                case 'U':   // UUID
                    insert_string = (uint8_t *)uuid_string;
                    break;

                case 'v':
                case 'V':   // Firmware version
                    insert_string = (uint8_t *)version_string;
                    break;

                case 'r':
                case 'R':   // URL replacement
                    insert_string = (uint8_t *)daplink_target_url;
                    break;

                default:
                    insert_string = (uint8_t *)"ERROR";
                    break;
            }
            insert(buf, insert_string, 2);
        }
    } while(*buf != '\0');
    size_left = buf - orig_buf;
    memset(buf, ' ', bufsize - size_left);
}
