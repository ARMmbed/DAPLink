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

#include "file_stream.h"
#include "util.h"
#include "validation.h"
#include "macro.h"
#include "intelhex.h"
#include "flash_decoder.h"
#include "error.h"
#include "RTL.h"
#include "compiler.h"

typedef enum {
    STREAM_STATE_CLOSED,
    STREAM_STATE_OPEN,
    STREAM_STATE_END,
    STREAM_STATE_ERROR
} stream_state_t;

typedef error_t (*stream_open_cb_t)(void * state);
typedef error_t (*stream_write_cb_t)(void * state, const uint8_t * data, uint32_t size);
typedef error_t (*stream_close_cb_t)(void * state);

typedef struct {
    stream_open_cb_t open;
    stream_write_cb_t write;
    stream_close_cb_t close;
} stream_t;

typedef struct {
    uint8_t dummy;
} bin_state_t;

typedef struct {
    bool parsing_complete;
    uint8_t bin_buffer[256];
} hex_state_t;

typedef union {
     bin_state_t bin;
     hex_state_t hex;
} shared_state_t;

static error_t open_bin(void * state);
static error_t write_bin(void * state, const uint8_t * data, uint32_t size);
static error_t close_bin(void * state);

static error_t open_hex(void * state);
static error_t write_hex(void * state, const uint8_t * data, uint32_t size);
static error_t close_hex(void * state);

stream_t stream[] = {
    {open_bin, write_bin, close_bin},   // STREAM_TYPE_BIN
    {open_hex, write_hex, close_hex},   // STREAM_TYPE_HEX
};
COMPILER_ASSERT(ELEMENTS_IN_ARRAY(stream) == STREAM_TYPE_COUNT);
// STREAM_TYPE_NONE must not be included in count
COMPILER_ASSERT(STREAM_TYPE_NONE > STREAM_TYPE_COUNT);

static shared_state_t shared_state;
static stream_state_t state = STREAM_STATE_CLOSED;
static stream_t * current_stream = 0;

// Thread variables (STUB these if RTX is not used)
static OS_TID stream_thread_tid = 0;
static void stream_thread_set(void)     {stream_thread_tid =  os_tsk_self();}
static void stream_thread_assert(void)  {util_assert(os_tsk_self() == stream_thread_tid);}

stream_type_t stream_start_identify(const uint8_t * data, uint32_t size)
{
    if (FLASH_DECODER_TYPE_UNKNOWN != flash_decoder_detect_type(data, size, 0, false)) {
        return STREAM_TYPE_BIN;
    }

    else if (1 == validate_hexfile(data)) {
        return STREAM_TYPE_HEX;
    }
    return STREAM_TYPE_NONE;
}

// Identify the file type from its extension
stream_type_t stream_type_from_name(const vfs_filename_t filename)
{
    // 8.3 file names must be in upper case
    if (0 == strncmp("BIN", &filename[8], 3)) {
        return STREAM_TYPE_BIN;
    } else if (0 == strncmp("HEX", &filename[8], 3)) {
        return STREAM_TYPE_HEX;
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

error_t stream_write(const uint8_t * data, uint32_t size)
{
    error_t status;

    stream_thread_assert();

    // Stream must be open already
    if (state != STREAM_STATE_OPEN) {
        util_assert(0);
        return ERROR_INTERNAL;
    }

    // Write to stream
    status = current_stream->write(&shared_state, data, size);
    if ((ERROR_SUCCESS != status) && (ERROR_SUCCESS_DONE_OR_CONTINUE != status)) {
        state = STREAM_STATE_ERROR;
    }
    return status;
}

error_t stream_close(void)
{
    error_t status;

    stream_thread_assert();

    // Stream must not be closed already
    if (STREAM_STATE_CLOSED == state) {
        util_assert(0);
        return ERROR_INTERNAL;
    }

    // Close stream
    status = current_stream->close(&shared_state);
    state = STREAM_STATE_CLOSED;
    return status;
}

/* Binary file processing */

static error_t open_bin(void * state)
{
    error_t status;
    status = flash_decoder_open();
    return status;
}

static error_t write_bin(void * state, const uint8_t * data, uint32_t size)
{
    error_t status;
    status = flash_decoder_write(data, size, 0, false);
    if (ERROR_SUCCESS == status) {
        // There is no way to determine the end of a binary
        // file so any point could be the end
        status = ERROR_SUCCESS_DONE_OR_CONTINUE;
    }
    return status;
}

static error_t close_bin(void * state)
{
    error_t status;
    status = flash_decoder_close();
    return status;
}

/* Hex file processing */

static error_t open_hex(void * state)
{
    error_t status;
    hex_state_t * hex_state = (hex_state_t *)state;
    memset(hex_state, 0, sizeof(*hex_state));

    reset_hex_parser();
    hex_state->parsing_complete = false;

    status = flash_decoder_open();
    return status;
}

static error_t write_hex(void * state, const uint8_t * data, uint32_t size)
{
    error_t status = ERROR_SUCCESS;
    hex_state_t * hex_state = (hex_state_t *)state;

    hexfile_parse_status_t parse_status = HEX_PARSE_UNINIT;
    uint32_t bin_start_address = 0; // Decoded from the hex file, the binary buffer data starts at this address
    uint32_t bin_buf_written = 0;   // The amount of data in the binary buffer starting at address above
    uint32_t block_amt_parsed = 0;  // amount of data parsed in the block on the last call
    
    while (1) {
        // try to decode a block of hex data into bin data
        parse_status = parse_hex_blob(data, size, &block_amt_parsed, hex_state->bin_buffer, sizeof(hex_state->bin_buffer), &bin_start_address, &bin_buf_written);
        // the entire block of hex was decoded. This is a simple state
        if (HEX_PARSE_OK == parse_status) {
            if (bin_buf_written > 0) {
                status = flash_decoder_write(hex_state->bin_buffer, bin_buf_written, bin_start_address, true);
            }
            break;
        }
        else if (HEX_PARSE_UNALIGNED == parse_status) {
            if (bin_buf_written > 0) {
                status = flash_decoder_write(hex_state->bin_buffer, bin_buf_written, bin_start_address, true);
                if (ERROR_SUCCESS != status) {
                    break;
                }
            }

            // incrememntal offset to finish the block
            size -= block_amt_parsed;
            data += block_amt_parsed;
        }
        else if (HEX_PARSE_EOF == parse_status) {
            if (bin_buf_written > 0) {
                status = flash_decoder_write(hex_state->bin_buffer, bin_buf_written, bin_start_address, true);
                if (ERROR_SUCCESS == status) {
                    status = ERROR_SUCCESS_DONE;
                }
            }
            break;
        }
        else if (HEX_PARSE_CKSUM_FAIL == parse_status) {
            status = ERROR_HEX_CKSUM;
            break;
        }
        else if ((HEX_PARSE_UNINIT == parse_status) || (HEX_PARSE_FAILURE == parse_status)) {
            util_assert(HEX_PARSE_UNINIT != parse_status);
            status = ERROR_HEX_PARSER;
            break;
        }
    }
    return status;
}

static error_t close_hex(void * state)
{
    error_t status;
    status = flash_decoder_close();
    return status;
}
