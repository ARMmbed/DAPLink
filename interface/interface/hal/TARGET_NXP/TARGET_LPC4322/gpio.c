/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <LPC43xx.h>
#include <RTL.h>
#include "lpc43xx_scu.h"

#include "gpio.h"
#include "DAP_config.h" // For the nRESET and RESET_TXE port/pin info

static uint16_t isr_flags;
static OS_TID isr_notify;
BOOL gpio_reset_pin_is_input  = __TRUE;


// Connected LED                P1_1: GPIO0[8]
#define LED_CONNECTED_PORT      0
#define LED_CONNECTED_BIT       8

#ifdef BOARD_LPC4337
  // ISP Control Pin            P2_11:  GPIO1[11]
  #define ISPCTRL_PORT          1
  #define ISPCTRL_BIT           11
#else  
  // ISP Control Pin            P2_3:  GPIO5[3]
  #define ISPCTRL_PORT          5
  #define ISPCTRL_BIT           3
#endif

// LPC43xx peripheral register bit masks (used by macros)
#define CCU_CLK_CFG_RUN         (1UL << 0)
#define CCU_CLK_CFG_AUTO        (1UL << 1)
#define CCU_CLK_STAT_RUN        (1UL << 0)

void gpio_init(void) {

    /* Enable clock and init GPIO outputs */
    LPC_CCU1->CLK_M4_GPIO_CFG = CCU_CLK_CFG_AUTO | CCU_CLK_CFG_RUN;
    while (!(LPC_CCU1->CLK_M4_GPIO_STAT & CCU_CLK_STAT_RUN));

    /* Configure I/O pins: function number, input buffer enabled,  */
    /*                     no pull-up/down                         */
    scu_pinmux(1, 1, GPIO_NOPULL, FUNC0);   /* LED:      GPIO0[8]  */
#ifdef BOARD_LPC4337
    scu_pinmux(2, 11, GPIO_NOPULL, FUNC0);  /* ISPCTRL:  GPIO1[11]  */
#else  
    scu_pinmux(2, 3, GPIO_NOPULL, FUNC4);   /* ISPCTRL:  GPIO5[3]  */
#endif
    scu_pinmux(2, 5, GPIO_PUP,    FUNC4);   /* nRESET:    GPIO5[5] */
    scu_pinmux(2, 6, GPIO_NOPULL, FUNC4);   /* nRESET_OE: GPIO5[6] */

    /* Configure: LED as output (turned off) */
    LPC_GPIO_PORT->CLR[LED_CONNECTED_PORT]  =  (1 << LED_CONNECTED_BIT);
    LPC_GPIO_PORT->DIR[LED_CONNECTED_PORT] |=  (1 << LED_CONNECTED_BIT);

    /* Configure: ISPCTRL as output and high */
    LPC_GPIO_PORT->SET[ISPCTRL_PORT]  = (1 << ISPCTRL_BIT);
    LPC_GPIO_PORT->DIR[ISPCTRL_PORT] |= (1 << ISPCTRL_BIT);

    /* configure Reset Button as input, Reset Output Enable as output LOW */
    LPC_GPIO_PORT->DIR[PORT_nRESET]    &= ~(1 << PIN_nRESET_IN_BIT);
    LPC_GPIO_PORT->CLR[PORT_RESET_TXE]  = (1 << PIN_RESET_TXE_IN_BIT);
    LPC_GPIO_PORT->DIR[PORT_RESET_TXE] |= (1 << PIN_RESET_TXE_IN_BIT);

    /* Use Pin Interrupt 0 */
    LPC_SCU->PINTSEL0 &= ~0xff;
    LPC_SCU->PINTSEL0 |= (PORT_nRESET << 5) | (PIN_nRESET_IN_BIT);
}

void gpio_set_dap_led(uint8_t state) {
    if (state) {
        LPC_GPIO_PORT->SET[LED_CONNECTED_PORT] = (1 << LED_CONNECTED_BIT);
    } else {
        LPC_GPIO_PORT->CLR[LED_CONNECTED_PORT] = (1 << LED_CONNECTED_BIT);
    }
}

void gpio_set_cdc_led(uint8_t state) {
    if (state) {
        LPC_GPIO_PORT->SET[LED_CONNECTED_PORT] = (1 << LED_CONNECTED_BIT);
    } else {
        LPC_GPIO_PORT->CLR[LED_CONNECTED_PORT] = (1 << LED_CONNECTED_BIT);
    }
}

void gpio_set_msd_led(uint8_t state) {
    if (state) {
        LPC_GPIO_PORT->SET[LED_CONNECTED_PORT] = (1 << LED_CONNECTED_BIT);
    } else {
        LPC_GPIO_PORT->CLR[LED_CONNECTED_PORT] = (1 << LED_CONNECTED_BIT);
    }
}

void gpio_set_isp_pin(uint8_t state) {
    if (state) {
        LPC_GPIO_PORT->SET[ISPCTRL_PORT] = (1 << ISPCTRL_BIT);
    } else {
        LPC_GPIO_PORT->CLR[ISPCTRL_PORT] = (1 << ISPCTRL_BIT);
    }
}

void gpio_enable_button_flag(OS_TID task, uint16_t flags) {
    /* When the "reset" button is pressed the ISR will set the */
    /* event flags "flags" for task "task" */

    /* Keep a local copy of task & flags */
    isr_notify=task;
    isr_flags=flags;

    LPC_GPIO_PIN_INT->IST = 0x01;   // Clear pending interrupts
    LPC_GPIO_PIN_INT->ISEL &= ~0x1; // Edge sensitive
    LPC_GPIO_PIN_INT->SIENF |= 0x1; // Enable falling edge interrupt

    NVIC_EnableIRQ(PIN_INT0_IRQn);
}

void GPIO0_IRQHandler() {
    isr_evt_set(isr_flags, isr_notify);
    NVIC_DisableIRQ(PIN_INT0_IRQn);

    // ACK interrupt
    LPC_GPIO_PIN_INT->IST = 0x01;
}
