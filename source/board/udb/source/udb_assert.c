#ifdef BUILD_FEATURE_UDB_ASSERT

#include "stm32h743xx.h"
#include "cmsis_os2.h"
#include "rtx_os.h"
#include "core_cm7.h"
#include "cmsis_gcc.h"
#include "udb_reset.h"
#include "stm32h7xx_hal.h"
#include "udb_fault_info.h"
#include "udb_watchdog.h"

// Flash memory bank 1 of stm32h743x
#define UDB_RAM_BASE_ADDRESS    FLASH_BANK1_BASE
#define UDB_RAM_TOP_ADDRESS     (FLASH_BANK2_BASE-1)

#define DEBUGGER_ATTACHED       ((CoreDebug->DHCSR & CoreDebug_DHCSR_S_RESET_ST_Msk) == 0)

static uint32_t get_PC()
{
    register uint32_t result;
    __asm volatile ("mov %0, pc\n"  : "=r" (result));

    return result;
}

static uint32_t get_SP()
{
    register uint32_t result;
    __asm volatile ("mov %0, sp\n"  : "=r" (result) );

    return result;
}

// overwrite the functions in backtrace.c

bool isAddressValidForCode(uint32_t addr)
{
    return (addr >= UDB_RAM_BASE_ADDRESS && addr <= UDB_RAM_TOP_ADDRESS);
}

bool isAddressValidForStack(uint32_t addr)
{
    // only work on RTX, but there's no better way to get the stack of the running task
    osRtxThread_t *thread = (osRtxThread_t*)osThreadGetId();
    uint32_t base = (uint32_t)thread->stack_mem;
    uint32_t top = ((uint32_t)thread->stack_mem) + thread->stack_size - 1;
    return (addr >= base && addr <= top);
}

void _util_assert(bool expression, const char *filename, uint16_t line)
{
    if (!expression)
    {
        udb_watchdog_refresh();
        __disable_irq();
        udb_write_backtrace_info(filename, line, get_PC(), get_SP());

        udb_watchdog_refresh();
        HAL_Delay(UDB_WRITE_BACKTRACE_INFO_DELAY_MS);
        if (DEBUGGER_ATTACHED)
        {
            __BKPT(0);
        }
        else
        {
            udb_reset();
        }
    }
}

#endif // BUILD_FEATURE_UDB_ASSERT
