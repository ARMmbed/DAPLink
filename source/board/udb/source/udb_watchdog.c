#include "udb_watchdog.h"

#include <stm32h7xx_hal.h>
#include <rtx_os.h>
#include "udb_fault_info.h"
#include "udb_reset.h"
#include "util.h"

#define WWDG_PRESCALER_1     0x00000000U                              // WWDG counter clock = (PCLK1/4096)/1
#define WWDG_PRESCALER_2     WWDG_CFR_WDGTB_0                         // WWDG counter clock = (PCLK1/4096)/2
#define WWDG_PRESCALER_4     WWDG_CFR_WDGTB_1                         // WWDG counter clock = (PCLK1/4096)/4
#define WWDG_PRESCALER_8     (WWDG_CFR_WDGTB_1 | WWDG_CFR_WDGTB_0)    // WWDG counter clock = (PCLK1/4096)/8
#define WWDG_PRESCALER_16    WWDG_CFR_WDGTB_2                         // WWDG counter clock = (PCLK1/4096)/16
#define WWDG_PRESCALER_32    (WWDG_CFR_WDGTB_2 | WWDG_CFR_WDGTB_0)    // WWDG counter clock = (PCLK1/4096)/32
#define WWDG_PRESCALER_64    (WWDG_CFR_WDGTB_2 | WWDG_CFR_WDGTB_1)    // WWDG counter clock = (PCLK1/4096)/64
#define WWDG_PRESCALER_128   WWDG_CFR_WDGTB                           // WWDG counter clock = (PCLK1/4096)/128

#define WWDG_MAX_COUNTER_VALUE        (0x7FU)
#define WWDG_MIN_COUNTER_VALUE        (0x40U)
#define WWDG_MAX_WINDOW_VALUE         (0x7FU)
#define WWDG_INTERNAL_CLOCK_DIVIDER   (4096U)

#define UDB_WDG              WWDG1
#define UDB_WDG_IRQn         WWDG_IRQn
#define UDB_WDG_PRESCALER    WWDG_PRESCALER_128
#define UDB_WDG_WINDOW       WWDG_MAX_WINDOW_VALUE
#define UDB_WDG_COUNTER      WWDG_MAX_COUNTER_VALUE

uint32_t s_udb_wdg_timeout_limit;
uint32_t s_udb_wdg_last_refresh_count;
uint32_t s_udb_wdg_timeout_count = 0;

static unsigned get_wwdg_prescaler(uint32_t prescaler_macro)
{
    unsigned prescaler;

    switch (prescaler_macro)
    {
    case WWDG_PRESCALER_1:
        prescaler = 1;
        break;
    case WWDG_PRESCALER_2:
        prescaler = 2;
        break;
    case WWDG_PRESCALER_4:
        prescaler = 4;
        break;
    case WWDG_PRESCALER_8:
        prescaler = 8;
        break;
    case WWDG_PRESCALER_16:
        prescaler = 16;
        break;
    case WWDG_PRESCALER_32:
        prescaler = 32;
        break;
    case WWDG_PRESCALER_64:
        prescaler = 64;
        break;
    case WWDG_PRESCALER_128:
        prescaler = 128;
        break;
    default:
        util_assert(false);
    }

    return prescaler;
}

/*
 * WWDG depends on the 120MHz PCLK1, and can only support 555ms timeout at max.
 * To support larger timeout, we keep track of the counter ourselves.
 * When early wakeup interrupt fires, we can pet the watchdog if it hasn't
 * reached the timeout. Otherwise, we save the backtrace info and reset.
 */
void udb_watchdog_init(uint32_t timeout_s)
{
    uint32_t wwdg_clock_hz = HAL_RCC_GetPCLK1Freq() / (WWDG_INTERNAL_CLOCK_DIVIDER * get_wwdg_prescaler(UDB_WDG_PRESCALER)) + 1U;

    s_udb_wdg_timeout_limit = wwdg_clock_hz * timeout_s;

    __HAL_RCC_WWDG_CLK_ENABLE();

    // Enable the watchdog and set the down counter
    WRITE_REG(UDB_WDG->CR, (WWDG_CR_WDGA | UDB_WDG_COUNTER));
    s_udb_wdg_last_refresh_count = UDB_WDG_COUNTER;

    // enable early wakeup interrupt, set the prescaler and window
    WRITE_REG(UDB_WDG->CFR, (WWDG_CFR_EWI | UDB_WDG_PRESCALER | UDB_WDG_WINDOW));

    NVIC_SetPriority(UDB_WDG_IRQn, 1);
    NVIC_EnableIRQ(UDB_WDG_IRQn);

    udb_watchdog_refresh();
}

void udb_watchdog_refresh(void)
{
    WRITE_REG(UDB_WDG->CR, (UDB_WDG_COUNTER));
    s_udb_wdg_timeout_count = 0;
    s_udb_wdg_last_refresh_count = UDB_WDG_COUNTER;
}

void WWDG_IRQHandler(void)
{
    uint32_t pc;
    uint32_t sp;

    // clear early wakeup interrupt
    UDB_WDG->SR = ~WWDG_SR_EWIF;

    s_udb_wdg_timeout_count += (s_udb_wdg_last_refresh_count - ((UDB_WDG->CR & WWDG_CR_T_Msk)));

    if (s_udb_wdg_timeout_count >= s_udb_wdg_timeout_limit)
    {
        WRITE_REG(UDB_WDG->CR, WWDG_MAX_COUNTER_VALUE);
        // save info in ASSERT.TXT and reset
        // Use the context before entering interrupt handler,
        // refer to rtos2/RTX/Source/GCC/irq_cm3.S SVC_Handler and SVC_ContextSave to
        // understand how RTX save pc and sp
        pc = *(uint32_t*)(__get_PSP() + 24);
        sp = (*(uint32_t*)((uint32_t)osRtxInfo.thread.run.curr + 56)) + 0x40;
        udb_write_backtrace_info(__FILE__, __LINE__, pc, sp);

        WRITE_REG(UDB_WDG->CR, WWDG_MAX_COUNTER_VALUE);
        HAL_Delay(UDB_WRITE_BACKTRACE_INFO_DELAY_MS);
        udb_reset();
    }

    s_udb_wdg_last_refresh_count = MIN(UDB_WDG_COUNTER - WWDG_MIN_COUNTER_VALUE, s_udb_wdg_timeout_limit - s_udb_wdg_timeout_count) +
                                   WWDG_MIN_COUNTER_VALUE;
    WRITE_REG(UDB_WDG->CR, s_udb_wdg_last_refresh_count);
}
