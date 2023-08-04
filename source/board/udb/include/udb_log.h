#ifndef _UDB_LOG_H_INCLUDED_
#define _UDB_LOG_H_INCLUDED_

#include <stdbool.h>
#include <stdint.h>

bool udb_log_cdc_ready();
void udb_log_set_cdc_ready(bool ready);
void udb_log_push(char *ptr, uint16_t size);
/*
 *  Flush the logs to CDC_3 endpoint
 */
void udb_log_flush(void);

#endif // _UDB_LOG_H_INCLUDED_
