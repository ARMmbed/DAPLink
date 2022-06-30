#include "udb_extended_features_task.h"
#include "adc.h"
#include "udb_adapter_detector.h"
#include "rtx_os.h"
#include "util.h"

#define UDB_EXTENDED_FEATURES_TASK_STACK          (800)
#define UDB_EXTENDED_FEATURES_TASK_PRIORITY       (osPriorityNormal)
#define UDB_EXTENDED_FEATURES_TASK_FLAGS_3S       (1 << 0)
#define UDB_EXTENDED_FEATURES_TASK_TICKS_3S       (osKernelGetTickFreq() * 3)

static osThreadId_t s_udb_extended_features_task_id;
static uint32_t s_udb_extended_features_task_cb[WORDS(sizeof(osRtxThread_t))];
static uint64_t s_udb_extended_features_task_stack[UDB_EXTENDED_FEATURES_TASK_STACK / sizeof(uint64_t)];
static const osThreadAttr_t k_udb_extended_features_task_attr =
{
    .name = "aux",
    .cb_mem = s_udb_extended_features_task_cb,
    .cb_size = sizeof(s_udb_extended_features_task_cb),
    .stack_mem = s_udb_extended_features_task_stack,
    .stack_size = sizeof(s_udb_extended_features_task_stack),
    .priority = UDB_EXTENDED_FEATURES_TASK_PRIORITY,
};

static uint32_t s_timer_3s_cb[WORDS(sizeof(osRtxTimer_t))];
static const osTimerAttr_t k_timer_3s_attr = {
    .name = "3s",
    .cb_mem = s_timer_3s_cb,
    .cb_size = sizeof(s_timer_3s_cb),
};

void timer_task_3s(void * arg)
{
    osThreadFlagsSet(s_udb_extended_features_task_id, UDB_EXTENDED_FEATURES_TASK_FLAGS_3S);
}

static void udb_extended_features_task(void *arg)
{
    uint16_t flags = 0;

    adc_init();

    osTimerId_t tmr_id = osTimerNew(timer_task_3s, osTimerPeriodic, NULL, &k_timer_3s_attr);
    osTimerStart(tmr_id, UDB_EXTENDED_FEATURES_TASK_TICKS_3S);

    while (1)
    {
        flags = osThreadFlagsWait(UDB_EXTENDED_FEATURES_TASK_FLAGS_3S, osFlagsWaitAny, osWaitForever);

        if (flags & UDB_EXTENDED_FEATURES_TASK_FLAGS_3S)
        {
            udb_adapter_detector_update_adapter_type_adc();
        }

        /* WARNING - Don't add code unless it's mutually exclusive with the DAPLink task
         * THe STHAL drivers don't implement locking, so it could cause a number of issues.
         * Please refer to b/198547046 for more info.
         */
    }
}

void udb_extended_features_task_create(void)
{
    s_udb_extended_features_task_id = osThreadNew(udb_extended_features_task, NULL, &k_udb_extended_features_task_attr);
}
