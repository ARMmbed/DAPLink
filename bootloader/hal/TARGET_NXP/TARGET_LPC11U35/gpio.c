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
#include <LPC11Uxx.h>
#include "gpio.h"

void gpio_init(void) {
    // enable clock for GPIO port 0
    LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 6);

    // configure GPIO-LED as output
    // DAP led (green)
    LPC_GPIO->DIR[0]  |= (1UL << 21);
    LPC_GPIO->SET[0]  |= (1UL << 21);

    // MSD led (red)
    LPC_GPIO->DIR[0]  |= (1UL << 20);
    LPC_GPIO->SET[0]  |= (1UL << 20);

    // Serial LED (blue)
    LPC_IOCON->TDI_PIO0_11 |= 0x01;
    LPC_GPIO->DIR[0]  |= (1UL << 11);
    LPC_GPIO->CLR[0]  |= (1UL << 11);

    // configure p1_19 pin as input
    LPC_GPIO->DIR[1]  &= ~(1UL << 19);
}

void gpio_set_dap_led(uint8_t state) {
    if (state) {
        LPC_GPIO->CLR[0] |= (1UL << 21);
    } else {
        LPC_GPIO->SET[0] |= (1UL << 21);
    }
}

void gpio_set_msd_led(uint8_t state) {
    if (state) {
        LPC_GPIO->CLR[0] |= (1UL << 20);
    } else {
        LPC_GPIO->SET[0] |= (1UL << 20);
    }
}

void gpio_set_cdc_led(uint8_t state) {
    if (state) {
        LPC_GPIO->CLR[0] |= (1UL << 11);
    } else {
        LPC_GPIO->SET[0] |= (1UL << 11);
    }
}

uint8_t gpio_get_pin_loader_state(void) {
    return LPC_GPIO->B1[19];
}


