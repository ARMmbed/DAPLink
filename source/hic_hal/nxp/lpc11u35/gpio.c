/**
 * @file    gpio.c
 * @brief   
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "LPC11Uxx.h"
#include "RTL.h"
#include "gpio.h"
#include "compiler.h"
#include "target_reset.h"
#include "IO_Config.h"

// taken code from the Nxp App Note AN11305
/* This data must be global so it is not read from the stack */
typedef void (*IAP)(uint32_t [], uint32_t []);
static IAP iap_entry = (IAP)0x1fff1ff1;
static uint32_t command[5], result[4];
#define init_msdstate() *((uint32_t *)(0x10000054)) = 0x0

/* This function resets some microcontroller peripherals to reset
 * hardware configuration to ensure that the USB In-System Programming module
 * will work properly. It is normally called from reset and assumes some reset
 * configuration settings for the MCU.
 * Some of the peripheral configurations may be redundant in your specific
 * project.
 */
void ReinvokeISP(void)
{
    /* make sure USB clock is turned on before calling ISP */
    LPC_SYSCON->SYSAHBCLKCTRL |= 0x04000;
    /* make sure 32-bit Timer 1 is turned on before calling ISP */
    LPC_SYSCON->SYSAHBCLKCTRL |= 0x00400;
    /* make sure GPIO clock is turned on before calling ISP */
    LPC_SYSCON->SYSAHBCLKCTRL |= 0x00040;
    /* make sure IO configuration clock is turned on before calling ISP */
    LPC_SYSCON->SYSAHBCLKCTRL |= 0x10000;
    /* make sure AHB clock divider is 1:1 */
    LPC_SYSCON->SYSAHBCLKDIV = 1;
    /* Send Reinvoke ISP command to ISP entry point*/
    command[0] = 57;
    init_msdstate();					 /* Initialize Storage state machine */
    /* Set stack pointer to ROM value (reset default) This must be the last
     * piece of code executed before calling ISP, because most C expressions
     * and function returns will fail after the stack pointer is changed.
     */
    __set_MSP(*((volatile uint32_t *)0x00000000));
    /* Enter ISP. We call "iap_entry" to enter ISP because the ISP entry is done
     * through the same command interface as IAP.
     */
    iap_entry(command, result);
    // Not supposed to come back!
}

static void busy_wait(uint32_t cycles)
{
    volatile uint32_t i;
    i = cycles;

    while (i > 0) {
        i--;
    }
}

void gpio_init(void)
{
    // enable clock for GPIO port 0
    LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 6);
#if defined(TARGET_POWER_HOLD)
    // Target PowerHOLD port
    PIN_PWH_IOCON = PIN_PWH_IOCON_INIT;
    LPC_GPIO->CLR[PIN_PWH_PORT] = PIN_PWH;
    LPC_GPIO->DIR[PIN_PWH_PORT] |= PIN_PWH;
#endif
    // configure GPIO-LED as output
#if defined(CONTROLLED_POWER_LED)
    // Power led (red)
    PIN_POW_LED_IOCON = PIN_POW_LED_IOCON_INIT;
    LPC_GPIO->CLR[PIN_POW_LED_PORT] = PIN_POW_LED;
    LPC_GPIO->DIR[PIN_POW_LED_PORT] |= PIN_POW_LED;
#endif
#if defined(INTEGRATED_STATUS_LED)
    // DAP/MSD/Serial led (green)
    PIN_DAP_LED_IOCON = PIN_DAP_LED_IOCON_INIT;
    LPC_GPIO->SET[PIN_DAP_LED_PORT] = PIN_DAP_LED;
    LPC_GPIO->DIR[PIN_DAP_LED_PORT] |= PIN_DAP_LED;
#else
    // DAP led (green)
    PIN_DAP_LED_IOCON = PIN_DAP_LED_IOCON_INIT;
    LPC_GPIO->SET[PIN_DAP_LED_PORT] = PIN_DAP_LED;
    LPC_GPIO->DIR[PIN_DAP_LED_PORT] |= PIN_DAP_LED;
    // MSD led (red)
    PIN_MSD_LED_IOCON = PIN_MSD_LED_IOCON_INIT;
    LPC_GPIO->SET[PIN_MSD_LED_PORT] = PIN_MSD_LED;
    LPC_GPIO->DIR[PIN_MSD_LED_PORT] |= PIN_MSD_LED;
    // Serial LED (blue)
    PIN_CDC_LED_IOCON = PIN_CDC_LED_IOCON_INIT;
    LPC_GPIO->SET[PIN_CDC_LED_PORT] = PIN_CDC_LED;
    LPC_GPIO->DIR[PIN_CDC_LED_PORT] |= PIN_CDC_LED;
#endif
    // configure Button(s) as input
    PIN_RESET_IN_IOCON = PIN_RESET_IN_IOCON_INIT;
    LPC_GPIO->DIR[PIN_RESET_IN_PORT] &= ~PIN_RESET_IN;
    PIN_RESET_IN_FWRD_IOCON = PIN_RESET_IN_FWRD_IOCON_INIT;
    LPC_GPIO->DIR[PIN_RESET_IN_FWRD_PORT] &= ~PIN_RESET_IN_FWRD;
#if !defined(PIN_nRESET_FET_DRIVE)
    // open drain logic for reset button
    PIN_nRESET_IOCON = PIN_nRESET_IOCON_INIT;
    LPC_GPIO->CLR[PIN_nRESET_PORT] = PIN_nRESET;
    LPC_GPIO->DIR[PIN_nRESET_PORT] &= ~PIN_nRESET;
#else
    // FET drive logic for reset button
    PIN_nRESET_IOCON = PIN_nRESET_IOCON_INIT;
    LPC_GPIO->CLR[PIN_nRESET_PORT] = PIN_nRESET;
    LPC_GPIO->DIR[PIN_nRESET_PORT] |= PIN_nRESET;
#endif
    /* Enable AHB clock to the FlexInt, GroupedInt domain. */
    LPC_SYSCON->SYSAHBCLKCTRL |= ((1 << 19) | (1 << 23) | (1 << 24));
    // Give the cap on the reset button time to charge
    busy_wait(10000);

    if (gpio_get_sw_reset() == 0) {
        IRQn_Type irq;
        // Disable SYSTICK timer and interrupt before calling into ISP
        SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);

        // Disable all nvic interrupts
        for (irq = (IRQn_Type)0; irq < (IRQn_Type)32; irq++) {
            NVIC_DisableIRQ(irq);
            NVIC_ClearPendingIRQ(irq);
        }

        ReinvokeISP();
    }
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    if (state) {
        LPC_GPIO->SET[PIN_DAP_LED_PORT] = PIN_DAP_LED;
    } else {
        LPC_GPIO->CLR[PIN_DAP_LED_PORT] = PIN_DAP_LED;
    }
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    if (state) {
        LPC_GPIO->SET[PIN_CDC_LED_PORT] = PIN_CDC_LED;
    } else {
        LPC_GPIO->CLR[PIN_CDC_LED_PORT] = PIN_CDC_LED;
    }
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    if (state) {
        LPC_GPIO->SET[PIN_MSD_LED_PORT] = PIN_MSD_LED;
    } else {
        LPC_GPIO->CLR[PIN_MSD_LED_PORT] = PIN_MSD_LED;
    }
}

uint8_t gpio_get_sw_reset(void)
{
    static uint8_t last_reset_forward_pressed = 0;
    uint8_t reset_forward_pressed;
    uint8_t reset_pressed;
    reset_forward_pressed = LPC_GPIO->PIN[PIN_RESET_IN_FWRD_PORT] & PIN_RESET_IN_FWRD ? 0 : 1;

    // Forward reset if the state of the button has changed
    //    This must be done on button changes so it does not interfere
    //    with other reset sources such as programming or CDC Break
    if (last_reset_forward_pressed != reset_forward_pressed) {
        if (reset_forward_pressed) {
            target_set_state(RESET_HOLD);
        } else {
            target_set_state(RESET_RUN);
        }

        last_reset_forward_pressed = reset_forward_pressed;
    }

    reset_pressed = reset_forward_pressed || (LPC_GPIO->PIN[PIN_RESET_IN_PORT] & PIN_RESET_IN ? 0 : 1);
    return !reset_pressed;
}

void target_forward_reset(bool assert_reset)
{
    // Do nothing - reset is forwarded in gpio_get_sw_reset
}
