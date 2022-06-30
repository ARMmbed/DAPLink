#ifndef UDB_UART_B_H
#define UDB_UART_B_H

#include <stdint.h>
#include <stdbool.h>

int32_t udb_uart_b_initialize(void);
int32_t udb_uart_b_uninitialize(void);
int32_t udb_uart_b_reset(void);
int32_t udb_uart_b_set_configuration(UART_Configuration *config);
int32_t udb_uart_b_get_configuration(UART_Configuration *config);
void    udb_uart_b_set_control_line_state(uint16_t ctrl_bmp);
int32_t udb_uart_b_write_free(void);
int32_t udb_uart_b_write_data(uint8_t *data, uint16_t size);
int32_t udb_uart_b_read_data(uint8_t *data, uint16_t size);
void    UART_B_IRQn_Handler(void);

#endif // UDB_UART_B_H