#ifndef UDB_UPTIME_H
#define UDB_UPTIME_H

#include <stdint.h>

void udb_uptime_init(void);
void udb_uptime_update(void);
void udb_uptime_print(void);
uint64_t udb_uptime_now_ms(void);

#endif // UDB_UPTIME_H
