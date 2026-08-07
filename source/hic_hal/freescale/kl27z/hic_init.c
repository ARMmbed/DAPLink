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
#include "fsl_clock.h"
#include "fsl_port.h"
#include "IO_Config.h"

/* Enable all clocks needed for USB to function */
void hic_enable_usb_clocks(void)
{
    /* Set USB clock to 48 MHz                                                  */
    MCG->MC                  |=   MCG_MC_HIRCEN_MASK;         /* Enable the HIRC clock in the MCG */
    SIM->SCGC4               |=   SIM_SCGC4_USBFS_MASK;       /* Enable the USB clock       */
    USB0->CLK_RECOVER_INT_EN |= USB_CLK_RECOVER_IRC_EN_IRC_EN_MASK;
    USB0->CLK_RECOVER_CTRL   |= USB_CLK_RECOVER_CTRL_CLOCK_RECOVER_EN_MASK;
    SIM->SOPT2               |=   SIM_SOPT2_USBSRC_MASK;      /* MCGPLLCLK used as src      */
}

void sdk_init(void) {
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortD);
    CLOCK_EnableClock(kCLOCK_PortE);

    NVIC_SetPriority(CMSIS_UART_IRQ, (1UL << __NVIC_PRIO_BITS) - 1UL);
}

uint32_t LPUART1_GetFreq(void)
{
    CLOCK_SetLpuart1Clock(1);
    return CLOCK_GetPeriphClkFreq();
}

void LPUART1_InitPins(void)
{
    CLOCK_SetLpuart1Clock(1);
    CLOCK_EnableClock(kCLOCK_Lpuart1);

    PORT_SetPinMux(PIN_UART_RX_PORT, PIN_UART_RX_BIT, PIN_UART_RX_MUX_ALT);
    PORT_SetPinMux(PIN_UART_TX_PORT, PIN_UART_TX_BIT, PIN_UART_TX_MUX_ALT);
}

void LPUART1_DeinitPins(void)
{
    PORT_SetPinMux(PIN_UART_RX_PORT, PIN_UART_RX_BIT, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(PIN_UART_TX_PORT, PIN_UART_TX_BIT, kPORT_PinDisabledOrAnalog);

    CLOCK_DisableClock(kCLOCK_Lpuart1);
}
