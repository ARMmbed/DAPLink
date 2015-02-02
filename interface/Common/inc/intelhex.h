#ifndef INTELHEX_H
#define INTELHEX_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    INTELHEX_LINE_TOO_LONG = -3,
    INTELHEX_CHECKSUM_MISMATCH = -2,
    INTELHEX_INVALID_CHAR = -1,
    INTELHEX_DONE = 0,
    INTELHEX_TO_CONTINUE = 1,
    INTELHEX_TO_WRITE = 2,
} intelhex_status_t;

/** Prepare any state that is maintained for the start of a file
 *   @return none
 */
void intelhex_reset(void);

/** Convert a blob of hex data into its binary equivelant
 *   @param pbuf The input and formatted output buffer for data
 *   @param start_address The address the data converted in the buffer starts at
 *   @param size The amount of data in the buffer
 *   @return >=0 - complete,
 */
int32_t intelhex_parse(uint8_t **p_hex_buf, uint8_t **p_bin_buf, uint32_t *start_address, uint32_t hex_buf_size, uint32_t bin_buf_size);

#ifdef __cplusplus
}
#endif
#endif
