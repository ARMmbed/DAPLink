/*
    Description:
      UDB's own system reset functions.
 */
#ifndef _UDB_RESET_H_INCLUDED_
#define _UDB_RESET_H_INCLUDED_

#include <stdint.h>

void udb_reset(void);

void udb_reset_async(uint32_t delay_ms);

#endif
