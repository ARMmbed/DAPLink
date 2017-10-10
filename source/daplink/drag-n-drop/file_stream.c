/**
 * @file    file_stream.c
 * @brief   Implementation of file_stream.h
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

#include "string.h"

#include "file_stream.h"
#include "util.h"
#include "validation.h"
#include "macro.h"
#include "intelhex.h"
#include "flash_decoder.h"
#include "error.h"
#include "RTL.h"
#include "compiler.h"
#include "flash_manager.h"
#include "target_config.h"

typedef enum {
    STREAM_STATE_CLOSED,
    STREAM_STATE_OPEN,
    STREAM_STATE_END,
    STREAM_STATE_ERROR
} stream_state_t;

typedef bool (*stream_detect_cb_t)(const uint8_t *data, uint32_t size);
typedef error_t (*stream_open_cb_t)(void *state);
typedef error_t (*stream_write_cb_t)(void *state, const uint8_t *data, uint32_t size);
typedef error_t (*stream_close_cb_t)(void *state);

typedef struct {
    stream_detect_cb_t detect;
    stream_open_cb_t open;
    stream_write_cb_t write;
    stream_close_cb_t close;
} stream_t;

typedef struct {
    uint32_t flash_addr;
} bin_state_t;

typedef struct {
    bool parsing_complete;
    uint8_t bin_buffer[256];
} hex_state_t;

typedef union {
    bin_state_t bin;
    hex_state_t hex;
} shared_state_t;

static bool detect_bin(const uint8_t *data, uint32_t size);
static error_t open_bin(void *state);
static error_t write_bin(void *state, const uint8_t *data, uint32_t size);
static error_t close_bin(void *state);

stream_t stream[] = {
    {detect_bin, open_bin, write_bin, close_bin},   // STREAM_TYPE_BIN
};
COMPILER_ASSERT(ELEMENTS_IN_ARRAY(stream) == STREAM_TYPE_COUNT);
// STREAM_TYPE_NONE must not be included in count
COMPILER_ASSERT(STREAM_TYPE_NONE > STREAM_TYPE_COUNT);

static shared_state_t shared_state;
static stream_state_t state = STREAM_STATE_CLOSED;
static stream_t *current_stream = 0;

// Thread variables (STUB these if RTX is not used)
static OS_TID stream_thread_tid = 0;
static void stream_thread_set(void)
{
    stream_thread_tid =  os_tsk_self();
}
static void stream_thread_assert(void)
{
    util_assert(os_tsk_self() == stream_thread_tid);
}

stream_type_t stream_start_identify(const uint8_t *data, uint32_t size)
{
    stream_type_t i;

    for (i = STREAM_TYPE_START; i < STREAM_TYPE_COUNT; i++) {
        if (stream[i].detect(data, size)) {
            return i;
        }
    }

    return STREAM_TYPE_NONE;
}

// Identify the file type from its extension
stream_type_t stream_type_from_name(const vfs_filename_t filename)
{
    // 8.3 file names must be in upper case
    if (0 == strncmp("BIN", &filename[8], 3)) {
        return STREAM_TYPE_BIN;
    } else {
        return STREAM_TYPE_NONE;
    }
}

error_t stream_open(stream_type_t stream_type)
{
    error_t status;

    // Stream must not be open already
    if (state != STREAM_STATE_CLOSED) {
        util_assert(0);
        return ERROR_INTERNAL;
    }

    // Stream must be of a supported type
    if (stream_type >= STREAM_TYPE_COUNT) {
        util_assert(0);
        return ERROR_INTERNAL;
    }

    stream_thread_set();
    // Initialize all variables
    memset(&shared_state, 0, sizeof(shared_state));
    state = STREAM_STATE_OPEN;
    current_stream = &stream[stream_type];
    // Initialize the specified stream
    status = current_stream->open(&shared_state);

    if (ERROR_SUCCESS != status) {
        state = STREAM_STATE_ERROR;
    }

    return status;
}

error_t stream_write(const uint8_t *data, uint32_t size)
{
    error_t status;

    // Stream must be open already
    if (state != STREAM_STATE_OPEN) {
        util_assert(0);
        return ERROR_INTERNAL;
    }

    // Check thread after checking state since the stream thread is
    // set only if stream_open has been called
    stream_thread_assert();
    // Write to stream
    status = current_stream->write(&shared_state, data, size);

    if (ERROR_SUCCESS_DONE == status) {
        state = STREAM_STATE_END;
    } else if ((ERROR_SUCCESS_DONE_OR_CONTINUE == status) || (ERROR_SUCCESS == status)) {
        // Stream should remain in the open state
        util_assert(STREAM_STATE_OPEN == state);
    } else {
        state = STREAM_STATE_ERROR;
    }

    return status;
}

error_t stream_close(void)
{
    error_t status;

    // Stream must not be closed already
    if (STREAM_STATE_CLOSED == state) {
        util_assert(0);
        return ERROR_INTERNAL;
    }

    // Check thread after checking state since the stream thread is
    // set only if stream_open has been called
    stream_thread_assert();
    // Close stream
    status = current_stream->close(&shared_state);
    state = STREAM_STATE_CLOSED;
    return status;
}

/* Binary file processing */

static bool detect_bin(const uint8_t *data, uint32_t size)
{
    return FLASH_DECODER_TYPE_UNKNOWN != flash_decoder_detect_type(data, size, 0, false);
}

static error_t open_bin(void *state)
{
    bin_state_t *bin_state = (bin_state_t *)state;
    bin_state->flash_addr = target_device.flash_start;
    return flash_manager_init(flash_intf_iap_protected);
}

static error_t write_bin(void *state, const uint8_t *data, uint32_t size)
{
    error_t status;
    bin_state_t *bin_state = (bin_state_t *)state;

    // Write data
    status = flash_manager_data(bin_state->flash_addr, data, size);

    if (ERROR_SUCCESS != status) {
        return status;
    }

    bin_state->flash_addr += size;
    // There is no way to determine the end of a binary
    // file so any point could be the end
    return ERROR_SUCCESS_DONE_OR_CONTINUE;
}

static error_t close_bin(void *state)
{
    error_t status;
    status = flash_manager_uninit();
    return status;
}
