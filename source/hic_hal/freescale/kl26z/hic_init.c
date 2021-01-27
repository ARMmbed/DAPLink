/**
 * @file    hic_init.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * Copyright (c) 2016-2017 NXP
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

#include "fsl_device_registers.h"

/* Enable all clocks needed for USB to function */
void hic_enable_usb_clocks(void)
{
    /* Set USB clock to 48 MHz                                                  */
        SIM->SOPT2   |=   SIM_SOPT2_USBSRC_MASK     | /* MCGPLLCLK used as src      */
                          SIM_SOPT2_PLLFLLSEL_MASK  ; /* Select MCGPLLCLK as clock  */
        SIM->SCGC4   |=   SIM_SCGC4_USBOTG_MASK;      /* Enable USBOTG clock        */
}

