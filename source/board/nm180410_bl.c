/**
 * @file    nm180410_bl.c
 * @brief   additional configuration for Northern Mechatronics NM180410
 *          LPC55S69 HIC 
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2023, ARM Limited, All Rights Reserved
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

#include "DAP_config.h"

uint8_t gpio_get_reset_btn_no_fwrd(void)
{
    // In the Apollo3, an integrated power-on detector monitors the
    // supply voltage and keeps SYSRESETn asserted while VDD rises
    // during power on.  SYSRESETn will de-assert once VDD rises above
    // V_POR.  This behaviour will cause DAPLink to randomly, albeit
    // rarely, boot into bootloader mode during power up depending
    // on the time it takes VDD to rise above V_POR.  The following
    // delay allows VDD to stabilize and ensures the boot behaviour is
    // deterministic.
    //
    // Factoring in the bypass capacitance on the board, the VDD rise
    // time is around 500us to 1ms.  The following value targets a delay
    // of at least 3ms at the highest HIC clock frequency of 150MHz.

    volatile uint32_t i = 0;

    for (i = 0; i < 500000; i++)
    {
        __asm volatile("nop");
    }

    return PIN_nRESET_IN() ? 0 : 1;
}
