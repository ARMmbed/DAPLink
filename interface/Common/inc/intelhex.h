#ifndef INTELHEX_H
#define INTELHEX_H

#include "stdint.h"

#ifdef __cplusplus
  extern "C" {
#endif
      
typedef enum hex_parse_status_t hex_parse_status_t;
enum hex_parse_status_t {
    HEX_PARSE_OK = 0,
    HEX_PARSE_EOF,
    HEX_PARSE_UNALIGNED,
    HEX_PARSE_LINE_OVERRUN
};

/** Prepare any state that is maintained for the start of a file
 *   @return none
 */
void reset_hex_parser(void);

/** Convert a blob of hex data into its binary equivelant
 *   @param buf The input and formatted output buffer for data
 *   @param start_address The address the data converted in the buffer starts at
 *   @param size The amount of data in the buffer
 *   @return 1 if we have valid data to write in buf, 0 otherwise
 */
uint32_t parse_hex_blob(uint8_t *buf, uint32_t *start_address, uint32_t *size);
      
#ifdef __cplusplus
  }
#endif

#endif
