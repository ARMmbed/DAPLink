#include "udb_uptime.h"

#include <stdio.h>
#include <stm32h7xx.h>
#include <stm32h743xx.h>
#include "udb_util.h"

// use 64-bit to allow track more than 50 days.
static uint64_t s_udb_uptime_ms;
static uint32_t s_last_ticks;

void udb_uptime_init(void)
{
    s_udb_uptime_ms = 0;
    s_last_ticks = HAL_GetTick();
}

void udb_uptime_update(void)
{
    uint32_t tick = HAL_GetTick();
    s_udb_uptime_ms += (tick - s_last_ticks);
    s_last_ticks = tick;
}

void udb_uptime_print(void)
{
    printf("uptime: ");
    print_time_from_ms(s_udb_uptime_ms);
}

uint64_t udb_uptime_now_ms(void)
{
    return s_udb_uptime_ms;
}
