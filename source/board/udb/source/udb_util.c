#include "udb_util.h"
#include <stdio.h>

#define MS_PER_SECOND   (1000U)
#define MS_PER_MINUTE   (MS_PER_SECOND * 60U)
#define MS_PER_HOUR     (MS_PER_MINUTE * 60U)
#define MS_PER_DAY      (MS_PER_HOUR * 24U)

void print_time_from_ms(uint64_t time_ms)
{
    uint32_t days;
    uint32_t hours;
    uint32_t mins;
    uint32_t secs;

    days = time_ms / MS_PER_DAY;
    time_ms %= MS_PER_DAY;

    hours = time_ms / MS_PER_HOUR;
    time_ms %= MS_PER_HOUR;

    mins = time_ms / MS_PER_MINUTE;
    time_ms %= MS_PER_MINUTE;

    secs = time_ms / MS_PER_SECOND;

    if (days > 0)
    {
        printf(" %u days", days);
    }

    if (days > 0 || hours > 0)
    {
        printf(" %02u hours", hours);
    }

    printf(" %02u mins", mins);
    printf(" %02u secs\n", secs);
}
