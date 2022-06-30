#ifndef UDB_WATCHDOG_H
#define UDB_WATCHDOG_H

#include <stdint.h>

void udb_watchdog_init(uint32_t timeout_s);

void udb_watchdog_refresh(void);

#endif  // UDB_WATCHDOG_H
