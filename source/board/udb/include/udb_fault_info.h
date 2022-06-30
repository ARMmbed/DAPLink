#ifndef UDB_FAULT_INFO_H
#define UDB_FAULT_INFO_H

#include <stdbool.h>
#include <stdint.h>

#define UDB_WRITE_BACKTRACE_INFO_DELAY_MS   (100U)

void udb_write_backtrace_info(const char *file, uint16_t line, uint32_t pc, uint32_t sp);
bool udb_is_fault_info_uncleared(void);
void udb_print_fault_info(void);
void udb_clear_fault_info(void);
void udb_check_unexpected_watchdog_reset(void);

#endif  // UDB_FAULT_INFO_H
