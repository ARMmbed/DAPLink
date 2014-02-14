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
#include "mbed_htm.h"
#include "read_uid.h"

extern uint8_t BlockBuf[];

// Pointers to substitution strings
const char *fw_version = (const char *)FW_BUILD;

static uint32_t unique_id;
static uint8_t already_unique_id = 0;
static uint32_t auth;
uint8_t string_auth[25 + 4];
uint8_t string_auth_descriptor[2+25*2];
static const char nybble_chars[] = "0123456789ABCDEF";

static void setup_string_id_auth(void);

static void get_byte_hex( uint8_t b, uint8_t *ch1, uint8_t *ch2 ) {
    *ch1 = nybble_chars[ ( b >> 4 ) & 0x0F ];
    *ch2 = nybble_chars[ b & 0x0F ];
}

static uint32_t atoi(uint8_t * str, uint8_t size, uint8_t base) {
    uint32_t k = 0;
    uint8_t i, idx = 0;
    for (i = 0; i < size; i++) {
        if (*str >= '0' && *str <= '9')
            idx = '0';
        else if (*str >= 'A' && *str <= 'F')
            idx = 'A' - 10;
        else if (*str >= 'a' && *str <= 'f')
            idx = 'a' - 10;
        k = k*base + (*str) - idx;
        str++;
    }
    return k;
}

static void setup_string_id_auth() {
    uint8_t i;
    uint8_t idx = 0;

    string_auth[0] = '$';
    string_auth[1] = '$';
    string_auth[2] = '$';
    string_auth[3] = 24;
    idx += 4;

    // string id
    for (i = 0; i < 4; i++)
        string_auth[idx++] = fw_version[i];
    for (i = 0; i < 4; i++)
        get_byte_hex((unique_id >> 8*(3 - i)) & 0xff, &string_auth[idx + 2*i], &string_auth[idx + 2*i + 1]);
    idx+=8;

    //string auth
    for (i = 0; i < 4; i++)
        get_byte_hex((auth >> 8*(3 - i)) & 0xff, &string_auth[idx + 2*i], &string_auth[idx + 2*i + 1]);
    idx+=8;

    string_auth[idx] = 0;
}

static void setup_string_descriptor() {
    uint8_t i, idx = 0, len;
    len = strlen((const char *)(string_auth+4));
    string_auth_descriptor[0] = len*2 + 2;
    string_auth_descriptor[1] = 3;
    idx += 2;

    for (i = 0; i < len*2; i++) {
        if ((i % 2) == 0)
            string_auth_descriptor[idx + i] = string_auth[4 + i/2];
        else
            string_auth_descriptor[idx + i] = 0;
    }
    idx += len*2;

    string_auth_descriptor[idx] = 0;
}

uint8_t get_len_string_interface(void) {
    return 2 + strlen((const char *)(string_auth+4))*2;
}

uint8_t * get_uid_string_interface(void) {
    return string_auth_descriptor;
}

const uint8_t board_secret[9] = "k4flu5fs";

static void compute_auth() {
    auth = atoi((uint8_t *)fw_version, 4, 16);
    auth ^= unique_id;
    auth ^= atoi((uint8_t *)(board_secret), 8, 16);
}

// HTML character reader context
typedef struct {
    uint8_t *phtml;        // Pointer to current position in HTML data in flash
    uint8_t substitute;  // TRUE if characters should be read from a substitution string, otherwise read from HTML data.
} HTMLCTX;


static void init_html(HTMLCTX *h, uint8_t *ptr) {
    h->substitute = 0;
    h->phtml = ptr;
    return;
}

static uint8_t get_html_character(HTMLCTX *h) {
    // Returns the next character from the HTML data at h->phtml.
    // Substitutes special sequences @V etc. with variable text.

    uint8_t c=0, s;           // Character from HTML data
    static uint8_t *sptr;     // Pointer to substitution string data
    uint8_t valid;          // Set to false if we need to read an additional character

    do {
        valid = 1;

        if (h->substitute) {
            // Check next substitution character
            if (*sptr=='\0') {
                // End of substituted string
                h->substitute = 0;
            }
        }

        if (!h->substitute) {
            // Get next HTML character
            c = *h->phtml++;

            // Indicates substitution
            if (c == '@') {
                // Check next HTML character
                s = *h->phtml;

                switch (s) {
                    case 'A':
                        sptr = (uint8_t *)(string_auth+4);             // auth string
                        h->substitute = 1;
                        break;

                    // Add any additional substitutions here
                    default:
                        break;
                }

                if (h->substitute) {
                    // If a vaild substitution sequence was found then discard this character
                    // Increment HTML pointer
                    h->phtml++;

                    // Check for the case of the substitution string being zero characters length
                    if (*sptr=='\0') {
                        // Effectively the substitution is already completed
                        h->substitute = 0;

                        // Must read another character
                        valid = 0;
                    }
                }
            }
        }

        if (h->substitute) {
            // Get next substitution character
            c = *sptr++;
        }
    } while (!valid);

    return c;
}

uint8_t update_html_file(void) {
    // Update a file containing the version information for this firmware
    // This assumes exclusive access to the file system (i.e. USB not enabled at this time)

    HTMLCTX html;                 // HTML reader context
    uint8_t c;                    // Current character from HTML reader

    uint32_t i = 0;

    if (already_unique_id == 0) {
        read_unique_id(&unique_id);
        compute_auth();
        setup_string_id_auth();
        setup_string_descriptor();
        already_unique_id = 1;
    }

    // Write file
    init_html(&html, (uint8_t *)WebSide);

    do {
        c=get_html_character(&html);
        if (c != '\0') {
            BlockBuf[i++] = c;
        }
    } while (c != '\0');

    return 1;  // Success
}
