/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "max32625.h"
#include "clkman_regs.h"
#include "adc_regs.h"
#include "pwrseq_regs.h"
#include "pwrman_regs.h"
#include "icc_regs.h"
#include "flc_regs.h"
#include "rtc_regs.h"
#include "trim_regs.h"

#ifndef RO_FREQ
#define RO_FREQ     96000000
#endif

#define __SYSTEM_CLOCK   RO_FREQ

#ifndef LP0_POST_HOOK
#define LP0_POST_HOOK
#endif

extern void (* const __Vectors[])(void);
uint32_t SystemCoreClock = RO_FREQ;

void NMI_Handler(void)
{
#ifdef DEBUG_MODE
    printf("NMI\n");
#endif
    while(1)
        __WFI();
}

void SystemCoreClockUpdate(void)
{
    if (MXC_PWRSEQ->reg0 & MXC_F_PWRSEQ_REG0_PWR_RCEN_RUN) {
        /* 4 MHz source */
        if (MXC_PWRSEQ->reg3 & MXC_F_PWRSEQ_REG3_PWR_RC_DIV) {
            SystemCoreClock = (4000000 / (0x1 << ((MXC_PWRSEQ->reg3 & MXC_F_PWRSEQ_REG3_PWR_RC_DIV) >>
                MXC_F_PWRSEQ_REG3_PWR_RC_DIV_POS)));
        } else {
            SystemCoreClock = 4000000;
        }
    } else {
        /* 96 MHz source */
        if (MXC_PWRSEQ->reg3 & MXC_F_PWRSEQ_REG3_PWR_RO_DIV) {
            SystemCoreClock = (RO_FREQ / (0x1 << ((MXC_PWRSEQ->reg3 & MXC_F_PWRSEQ_REG3_PWR_RO_DIV) >>
                MXC_F_PWRSEQ_REG3_PWR_RO_DIV_POS)));
        } else {
            SystemCoreClock = RO_FREQ;
        }
    }
}

void CLKMAN_TrimRO(void)
{
    uint32_t running;
    uint32_t trim;

    // Reset ADC
    MXC_PWRMAN->peripheral_reset = MXC_F_PWRMAN_PERIPHERAL_RESET_ADC;
    MXC_PWRMAN->peripheral_reset = 0;

    /* Step 1: enable 32KHz RTC */
    running = MXC_PWRSEQ->reg0 & MXC_F_PWRSEQ_REG0_PWR_RTCEN_RUN;
    MXC_PWRSEQ->reg0 |= MXC_F_PWRSEQ_REG0_PWR_RTCEN_RUN;

    /* Wait for RTC warm-up */
    while(MXC_RTCCFG->osc_ctrl & MXC_F_RTC_OSC_CTRL_OSC_WARMUP_ENABLE) {}

    /* Step 2: enable RO calibration complete interrupt */
    MXC_ADC->intr |= MXC_F_ADC_INTR_RO_CAL_DONE_IE;

    /* Step 3: clear RO calibration complete interrupt */
    MXC_ADC->intr |= MXC_F_ADC_INTR_RO_CAL_DONE_IF;

    // Step 4: read RO flash trim shadow register*/
    // needed if parts are untrimmed
#ifdef UNTRIM
    uint32_t reg;
    reg = MXC_PWRSEQ->reg6;
    reg &= ~MXC_F_PWRSEQ_REG6_PWR_TRIM_OSC_VREF;
    reg |= (480 << MXC_F_PWRSEQ_REG6_PWR_TRIM_OSC_VREF_POS);
    MXC_PWRSEQ->reg6 = reg;
#endif

    /* Step 5: write initial trim to frequency calibration initial condition register */
    trim = (MXC_PWRSEQ->reg6 & MXC_F_PWRSEQ_REG6_PWR_TRIM_OSC_VREF) >> MXC_F_PWRSEQ_REG6_PWR_TRIM_OSC_VREF_POS;
    MXC_ADC->ro_cal1 = (MXC_ADC->ro_cal1 & ~MXC_F_ADC_RO_CAL1_TRM_INIT) |
                       ((trim << MXC_F_ADC_RO_CAL1_TRM_INIT_POS) & MXC_F_ADC_RO_CAL1_TRM_INIT);

    /* Step 6: load initial trim to active frequency trim register */
    MXC_ADC->ro_cal0 |= MXC_F_ADC_RO_CAL0_RO_CAL_LOAD;

    /* Step 7: enable frequency loop to control RO trim */
    MXC_ADC->ro_cal0 |= MXC_F_ADC_RO_CAL0_RO_CAL_EN;

    /* Step 8: run frequency calibration in atomic mode */
    MXC_ADC->ro_cal0 |= MXC_F_ADC_RO_CAL0_RO_CAL_ATOMIC;

    /* Step 9: waiting for ro_cal_done flag */
    while(!(MXC_ADC->intr & MXC_F_ADC_INTR_RO_CAL_DONE_IF));

    /* Step 10: stop frequency calibration */
    MXC_ADC->ro_cal0 &= ~MXC_F_ADC_RO_CAL0_RO_CAL_RUN;

    /* Step 11: disable RO calibration complete interrupt */
    MXC_ADC->intr &= ~MXC_F_ADC_INTR_RO_CAL_DONE_IE;

    /* Step 12: read final frequency trim value */
    trim = (MXC_ADC->ro_cal0 & MXC_F_ADC_RO_CAL0_RO_TRM) >> MXC_F_ADC_RO_CAL0_RO_TRM_POS;

    /* Step 13: write final trim to RO flash trim shadow register */
    MXC_PWRSEQ->reg6 = (MXC_PWRSEQ->reg6 & ~MXC_F_PWRSEQ_REG6_PWR_TRIM_OSC_VREF) |
                       ((trim << MXC_F_PWRSEQ_REG6_PWR_TRIM_OSC_VREF_POS) & MXC_F_PWRSEQ_REG6_PWR_TRIM_OSC_VREF);

    // Step 14: restore RTC status
    if (running) {
        MXC_PWRSEQ->reg0 |= MXC_F_PWRSEQ_REG0_PWR_RTCEN_RUN;
    } else {
        MXC_PWRSEQ->reg0 &= ~MXC_F_PWRSEQ_REG0_PWR_RTCEN_RUN;
    }

    /* Step 15: disable frequency loop to control RO trim */
    MXC_ADC->ro_cal0 &= ~MXC_F_ADC_RO_CAL0_RO_CAL_EN;
}

static void ICC_Enable(void)
{
    /* Invalidate cache and wait until ready */
    MXC_ICC->invdt_all = 1;
    while (!(MXC_ICC->ctrl_stat & MXC_F_ICC_CTRL_STAT_READY));

    /* Enable cache */
    MXC_ICC->ctrl_stat |= MXC_F_ICC_CTRL_STAT_ENABLE;

    /* Must invalidate a second time for proper use */
    MXC_ICC->invdt_all = 1;
}

/* This function is called before C runtime initialization and can be
 * implemented by the application for early initializations. If a value other
 * than '0' is returned, the C runtime initialization will be skipped.
 *
 * You may over-ride this function in your program by defining a custom
 *  PreInit(), but care should be taken to reproduce the initilization steps
 *  or a non-functional system may result.
 */
__weak int PreInit(void)
{
    /* Increase system clock to 96 MHz */
    MXC_CLKMAN->clk_ctrl = MXC_V_CLKMAN_CLK_CTRL_SYSTEM_SOURCE_SELECT_96MHZ_RO;

    /* Performance-measurement hook, may be defined as nothing */
    LP0_POST_HOOK;

    /* Enable cache here to reduce boot time */
    ICC_Enable();

    return 0;
}

/* This function can be implemented by the application to initialize the board */
__weak int Board_Init(void)
{
    /* Do nothing */
    return 0;
}

// This function to be implemented by the hal
extern void low_level_init(void);

void SystemInit(void)
{
    uint32_t reg;
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    /* Enable FPU on Cortex-M4, which occupies coprocessor slots 10 & 11 */
    /* Grant full access, per "Table B3-24 CPACR bit assignments". DDI0403D "ARMv7-M Architecture Reference Manual" */
    SCB->CPACR |= SCB_CPACR_CP10_Msk | SCB_CPACR_CP11_Msk;
    __DSB();
    __ISB();
#endif

    ICC_Enable();

    // Enable real-time clock during sleep mode
    MXC_PWRSEQ->reg0 |= (MXC_F_PWRSEQ_REG0_PWR_RTCEN_RUN | MXC_F_PWRSEQ_REG0_PWR_RTCEN_SLP);
    MXC_CLKMAN->clk_ctrl |= MXC_F_CLKMAN_CLK_CTRL_RTOS_MODE;
    // Turn on retention regulator - this needs some time to turn on before entering sleep
    MXC_PWRSEQ->reg0 |= (MXC_F_PWRSEQ_REG0_PWR_RETREGEN_RUN | MXC_F_PWRSEQ_REG0_PWR_RETREGEN_SLP);

    // Trim ring oscillator
    CLKMAN_TrimRO();

    // Select 96MHz ring oscillator as clock source
    reg = MXC_CLKMAN->clk_ctrl;
    reg &= ~MXC_F_CLKMAN_CLK_CTRL_SYSTEM_SOURCE_SELECT;
    reg |= 1 << MXC_F_CLKMAN_CLK_CTRL_SYSTEM_SOURCE_SELECT_POS;
    MXC_CLKMAN->clk_ctrl = reg;

    // The MAX32625 will not enter LP1 or LP0 if any of the GPIO WUD latches
    // are set, instead it will just wake up.  These are not cleared by reset,
    // so lets clear them here.
    // But only if a first boot event is detected, to avoid clearing valid
    // events when returning from LP0
    if ( (MXC_PWRSEQ->flags & MXC_F_PWRSEQ_FLAGS_PWR_FIRST_BOOT) ||
            !(MXC_PWRMAN->pwr_rst_ctrl & MXC_F_PWRMAN_PWR_RST_CTRL_POR) ) {
        MXC_PWRSEQ->reg1 |= (MXC_F_PWRSEQ_REG1_PWR_CLR_IO_EVENT_LATCH | MXC_F_PWRSEQ_REG1_PWR_CLR_IO_CFG_LATCH);
        MXC_PWRSEQ->reg1 &= ~(MXC_F_PWRSEQ_REG1_PWR_CLR_IO_EVENT_LATCH | MXC_F_PWRSEQ_REG1_PWR_CLR_IO_CFG_LATCH);
    }

    // NOTE: These must be cleared before clearing IOWAKEUP
    MXC_PWRSEQ->reg1 |= MXC_F_PWRSEQ_REG1_PWR_CLR_IO_EVENT_LATCH;
    MXC_PWRSEQ->reg1 &= ~MXC_F_PWRSEQ_REG1_PWR_CLR_IO_EVENT_LATCH;

    MXC_PWRSEQ->flags |= MXC_F_PWRSEQ_FLAGS_PWR_IOWAKEUP;

    // Clear the firstboot bit, which is generated by a POR event and locks out LPx modes
    MXC_PWRSEQ->reg0 &= ~(MXC_F_PWRSEQ_REG0_PWR_FIRST_BOOT);

    // Clear all unused wakeup sources
    // Beware! Do not change any flag not mentioned here, as they will gate important power sequencer signals
    MXC_PWRSEQ->msk_flags &= ~(MXC_F_PWRSEQ_MSK_FLAGS_PWR_USB_PLUG_WAKEUP |
                               MXC_F_PWRSEQ_MSK_FLAGS_PWR_USB_REMOVE_WAKEUP);

    // RTC sources are inverted, so a 1 will disable them
    MXC_PWRSEQ->msk_flags |= (MXC_F_PWRSEQ_MSK_FLAGS_RTC_CMPR1 |
                              MXC_F_PWRSEQ_MSK_FLAGS_RTC_PRESCALE_CMP);

    // Unfreeze the GPIO by clearing MBUS_GATE
    // This is always safe to do, and restores our I/O when returning from LP0
    MXC_PWRSEQ->reg1 &= ~MXC_F_PWRSEQ_REG1_PWR_MBUS_GATE;

    SystemCoreClockUpdate();

    Board_Init();
}
