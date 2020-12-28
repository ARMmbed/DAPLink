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

#include "LPC43xx.h"
#include "lpc43xx_scu.h"
#include "gpio.h"
#include "compiler.h"
#include "DAP_config.h" // For the nRESET and RESET_TXE port/pin info
#include "IO_Config.h"

BOOL gpio_reset_pin_is_input  = __TRUE;


// LPC43xx peripheral register bit masks (used by macros)
#define CCU_CLK_CFG_RUN     (1UL << 0)
#define CCU_CLK_CFG_AUTO    (1UL << 1)
#define CCU_CLK_STAT_RUN    (1UL << 0)

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
    /* Enable clock and init GPIO outputs */
    LPC_CCU1->CLK_M4_GPIO_CFG = CCU_CLK_CFG_AUTO | CCU_CLK_CFG_RUN;

    while (!(LPC_CCU1->CLK_M4_GPIO_STAT & CCU_CLK_STAT_RUN));

    /* Configure I/O pins: function number, input buffer enabled,  */
    /*                     no pull-up/down                         */
    scu_pinmux(1, 1, GPIO_NOPULL, FUNC0);   /* P1_1  LED:       GPIO0[8]  */
    scu_pinmux(2, 11, GPIO_NOPULL, FUNC0);  /* P2_11 ISPCTRL:   GPIO1[11] */
    scu_pinmux(2, 5, GPIO_PUP,    FUNC4);   /* P2_5  nRESET:    GPIO5[5]  */
    scu_pinmux(2, 6, GPIO_NOPULL, FUNC4);   /* P2_6  nRESET_OE: GPIO5[6]  */
    scu_pinmux(3, 1, GPIO_NOPULL, FUNC4);   /* P3_1  POWER_EN:  GPIO5[8]  */

    /* Configure: LED as output LOW (turned off)*/
    X_DIR_OUT(LED_CONNECTED);
    X_CLR(LED_CONNECTED);

    /* Configure: ISPCTRL as output HIGH */
    X_DIR_OUT(ISPCTRL);
    X_SET(ISPCTRL);

    /* Configure: Reset Button */
    X_DIR_IN(nRESET);
    X_CLR(nRESET);

    /* Reset Output Enable as output LOW */
    X_DIR_OUT(RESET_TXE);
    X_CLR(RESET_TXE);

    /* Use Pin Interrupt 0 */
    LPC_SCU->PINTSEL0 &= ~0xff;
    LPC_SCU->PINTSEL0 |= (PORT_nRESET << 5) | (PIN_nRESET_IN_BIT);

    /* Configure: POWER_EN as output LOW */
    X_DIR_OUT(POWER_EN);
    X_CLR(POWER_EN);

#if (SWO_UART != 0)
    /* Configure: SWO as input */
    X_DIR_IN(SWO);
#endif

    busy_wait(10000);
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    if (state) {
        X_SET(LED_CONNECTED);
    } else {
        X_CLR(LED_CONNECTED);
    }
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    if (state) {
        X_SET(LED_CONNECTED);
    } else {
        X_CLR(LED_CONNECTED);
    }
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    if (state) {
        X_SET(LED_CONNECTED);
    } else {
        X_CLR(LED_CONNECTED);
    }
}

void gpio_set_isp_pin(uint8_t state)
{
    if (state) {
        X_SET(ISPCTRL);
    } else {
        X_CLR(ISPCTRL);
    }
}

uint8_t gpio_get_reset_btn_no_fwrd(void)
{
    return X_WORD(nRESET) ? 0 : 1;
}

uint8_t gpio_get_reset_btn_fwrd(void)
{
    return 0;
}

void gpio_set_board_power(bool powerEnabled)
{
    if (powerEnabled) {
        X_SET(POWER_EN);
    } else {
        X_CLR(POWER_EN);
    }
}
