#include "udb_log.h"
#include "rl_usb.h"
#include "usb.h"
#include "util.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define UDB_LOG_BUFFER_SIZE         (4096U)
#define UDB_LOG_MAX_SIZE            (512U)
#define UDB_LOG_FLUSH_SIZE          (512U)

typedef struct
{
    uint16_t in;
    uint16_t out;
} udb_log_fifo_t;

static char s_udb_log_fifo_buffer[UDB_LOG_BUFFER_SIZE];
static udb_log_fifo_t s_udb_log_fifo;
static bool s_cdc_ready = false;

static bool udb_log_fifo_is_empty(udb_log_fifo_t *fifo)
{
    return fifo->in == fifo->out;
}

static bool udb_log_fifo_is_full(udb_log_fifo_t *fifo)
{
    return ((fifo->in + 1) % UDB_LOG_BUFFER_SIZE) == fifo->out;
}

static char udb_log_fifo_pop(udb_log_fifo_t *fifo)
{
    util_assert(udb_log_fifo_is_empty(fifo) == false);
    char ch = s_udb_log_fifo_buffer[fifo->out];
    fifo->out = (fifo->out + 1) % UDB_LOG_BUFFER_SIZE;

    return ch;
}

static void udb_log_fifo_push(udb_log_fifo_t *fifo, char ch)
{
    if (udb_log_fifo_is_full(fifo))
    {
        udb_log_fifo_pop(fifo);
    }
    s_udb_log_fifo_buffer[fifo->in] = ch;
    fifo->in = (fifo->in + 1) % UDB_LOG_BUFFER_SIZE;
}

bool udb_log_cdc_ready()
{
    return s_cdc_ready;
}

void udb_log_set_cdc_ready(bool ready)
{
    s_cdc_ready = ready;
}

/*
 * Push log ptr with size into FIFO.
 */
void udb_log_push(char *ptr, uint16_t size)
{
    for (int i = 0; i < size; ++i)
    {
        udb_log_fifo_push(&s_udb_log_fifo, ptr[i]);
    }
}

void udb_log_flush(void)
{
    char ch;
    while (udb_log_fifo_is_empty(&s_udb_log_fifo) == false)
    {
        for (int sz = 0; (sz < UDB_LOG_FLUSH_SIZE) && (udb_log_fifo_is_empty(&s_udb_log_fifo) == false); ++sz)
        {
            putchar(udb_log_fifo_pop(&s_udb_log_fifo));
        }
    }
}
