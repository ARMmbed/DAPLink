/**
 * @file    circ_buf.h
 * @brief   Implementation of a circular buffer
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2016-2016, ARM Limited, All Rights Reserved
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

#ifndef CIRC_BUF_H
#define CIRC_BUF_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t head;
    uint32_t tail;
    uint32_t size;
    uint8_t *buf;
} circ_buf_t;

// Initialize or reinitialize a circular buffer
void circ_buf_init(circ_buf_t *circ_buf, uint8_t *buffer, uint32_t size);

// Push a byte into the circular buffer
void circ_buf_push(circ_buf_t *circ_buf, uint8_t data);

// Return a byte from the circular buffer
uint8_t circ_buf_pop(circ_buf_t *circ_buf);

// Get the number of bytes in the circular buffer
uint32_t circ_buf_count_used(circ_buf_t *circ_buf);

// Get the number of free spots left in the circular buffer
uint32_t circ_buf_count_free(circ_buf_t *circ_buf);

// Attempt to read size bytes from the buffer. Return the number of bytes read
uint32_t circ_buf_read(circ_buf_t *circ_buf, uint8_t *data, uint32_t size);

// Attempt to write size bytes to the buffer. Return the number of bytes written
uint32_t circ_buf_write(circ_buf_t *circ_buf, const uint8_t *data, uint32_t size);

// Returns a pointer to the next byte on the circular buffer and stores in the
// value pointed by "size" the number of bytes available in that region, which
// may be less than the total number of bytes in the circular buffer.
const uint8_t* circ_buf_peek(circ_buf_t *circ_buf, uint32_t* size);

// Remove n bytes from the front of the circular buffer. The values are
// discarded.
void circ_buf_pop_n(circ_buf_t *circ_buf, uint32_t n);

#ifdef __cplusplus
}
#endif

#endif
