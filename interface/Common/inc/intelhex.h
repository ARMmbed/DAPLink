#ifndef INTELHEX_H
#define INTELHEX_H

#include "stdint.h"

#ifdef __cplusplus
  extern "C" {
#endif
      
/** Type of states that the parser can return
    @enum hex_parse_status_t
 */
typedef enum hex_parse_status_t hex_parse_status_t;
enum hex_parse_status_t {
    HEX_PARSE_OK = 0,       /*!< The input buffer was complete parsed and converted into the output buffer */
    HEX_PARSE_EOF,          /*!< EOF line found in the hex file */
    HEX_PARSE_UNALIGNED,    /*!< The address of decoded data isnt consecutive. Need to program what was returned and continue to parse the input buffer */
    HEX_PARSE_LINE_OVERRUN, /*!< Error state when the record length is longer than the record structure */
    HEX_PARSE_CKSUM_FAIL,   /*!< Error state when the record checksum doesnt properly compute */
    HEX_PARSE_UNINIT,       /*!< Default state. Return of this type is unrecoverable logic error */
    HEX_PARSE_FAILURE       /*!< Amount of hex data to decode didnt match the parsing logics count of decoded bytes */
};

/** Prepare any state that is maintained for the start of a file
    @param none
    @return none
 */
void reset_hex_parser(void);

/** Convert a blob of hex data into its binary equivelant
    @param hex_blob A block of ascii encoded hex file data
    @param hex_blob_size The amount of valid data in the hex_blob
    @param hex_parse_cnt The amount of hex_blob data from the call that was parsed
    @param bin_buf Buffer the decoded hex file contents goes into
    @param bin_buf_size max size of the buffer
    @param bin_buf_address The start address for data in the bin_buf as decoded from the hex file
    @param bin_buf_cnt The amount of data in the bin_buf
    @return A member of hex_parse_status_t that describes the state of decoding
 */
hex_parse_status_t parse_hex_blob(uint8_t *hex_blob, uint32_t hex_blob_size, uint32_t *hex_parse_cnt, uint8_t *bin_buf, uint32_t bin_buf_size, uint32_t *bin_buf_address, uint32_t *bin_buf_cnt);
      
#ifdef __cplusplus
  }
#endif

#endif
