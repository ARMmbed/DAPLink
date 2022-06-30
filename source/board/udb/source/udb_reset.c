#include "udb_reset.h"
#include "cortex_m.h"
#include "rtx_os.h"
#include "util.h"

static uint32_t s_reset_timer_cb[WORDS(sizeof(osRtxTimer_t))];
static const osTimerAttr_t k_reset_timer_attr =
{
    .name = "reset_timer",
    .cb_mem = s_reset_timer_cb,
    .cb_size = sizeof(s_reset_timer_cb),
};

static void udb_reset_callback(void *argument)
{
    SystemReset();
}

void udb_reset(void)
{
    SystemReset();
}

void udb_reset_async(uint32_t delay_ms)
{
    osTimerId_t timer_id = osTimerNew(udb_reset_callback, osTimerOnce, NULL, &k_reset_timer_attr);
    osTimerStart(timer_id, osKernelGetTickFreq() * delay_ms / 1000);
}
