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
#include "LPC11Uxx.h"

#include "gpio.h"
#include "compiler.h"
#include "daplink.h"

// This GPIO configuration is only valid for the LPC11U35 HDK
COMPILER_ASSERT(DAPLINK_HDK_ID == DAPLINK_HDK_ID_LPC11U35);

#ifdef DBG_LPC812
#define SW_RESET_BUTTON    0
#else
#define SW_RESET_BUTTON    1
#endif

#ifdef SW_RESET_BUTTON
#define RESET_PORT        (1)
#define RESET_PIN         (19)
#define RESET_INT_CH      (0)
#define RESET_INT_MASK    (1 << RESET_INT_CH)
#endif

#define PIN_DAP_LED       (1<<21)
#define PIN_MSD_LED       (1<<20)
#define PIN_CDC_LED       (1<<11)

void gpio_init(void)
{
    // enable clock for GPIO port 0
    LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 6);

    // configure GPIO-LED as output
    // DAP led (green)
    LPC_GPIO->DIR[0]  |= (PIN_DAP_LED);
    LPC_GPIO->CLR[0]  |= (PIN_DAP_LED);

    // MSD led (red)
    LPC_GPIO->DIR[0]  |= (PIN_MSD_LED);
    LPC_GPIO->CLR[0]  |= (PIN_MSD_LED);

    // Serial LED (blue)
      LPC_IOCON->TDI_PIO0_11 |= 0x01;
    LPC_GPIO->DIR[0]  |= (PIN_CDC_LED);
    LPC_GPIO->CLR[0]  |= (PIN_CDC_LED);

    // configure Button as input
#if SW_RESET_BUTTON
    LPC_GPIO->DIR[RESET_PORT]  &= ~(1 << RESET_PIN);
#endif

    /* Enable AHB clock to the FlexInt, GroupedInt domain. */
    LPC_SYSCON->SYSAHBCLKCTRL |= ((1<<19) | (1<<23) | (1<<24));
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    if (state) {
        LPC_GPIO->SET[0] |= (PIN_DAP_LED);
    } else {
        LPC_GPIO->CLR[0] |= (PIN_DAP_LED);
    }
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    if (state) {
      LPC_GPIO->SET[0] |= (PIN_CDC_LED);
    } else {
      LPC_GPIO->CLR[0] |= (PIN_CDC_LED);
    }
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    if (state) {
        LPC_GPIO->SET[0] |= (PIN_MSD_LED);
    } else {
        LPC_GPIO->CLR[0] |= (PIN_MSD_LED);
    }
}

uint8_t gpio_get_sw_reset(void)
{
    return (LPC_GPIO->PIN[RESET_PORT] & (1 << RESET_PIN)) ? 1 : 0;
}
