/**
 * @file    gpio.c
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
#include "DAP_config.h"
#include "gpio.h"
#include "daplink.h"
#include "hic_init.h"
#include "fsl_clock.h"
#include "fsl_iocon.h"
#include "fsl_reset.h"

void gpio_init(void)
{
    // Enable hardfault on unaligned access for the interface only.
    // If this is done in the bootloader than then it might (will) break
    // older application firmware or firmware from 3rd party vendors.
#if defined(DAPLINK_IF)
    SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif

    // Ensure clocks are enabled.
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL0_IOCON_MASK
                                | SYSCON_AHBCLKCTRL0_GPIO0_MASK;
    SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL1_FC0_MASK
                                | SYSCON_AHBCLKCTRL1_FC3_MASK;

    // Reset peripherals.
    RESET_PeripheralReset(kIOCON_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kFC0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kFC3_RST_SHIFT_RSTn);

    // Configure pins.
    IOCON->PIO[PIN_PIO_PORT][LED_CONNECTED] = IOCON_FUNC0 | IOCON_DIGITAL_EN;
    IOCON->PIO[PIN_PIO_PORT][PIN_HW_VERS_6] = IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN | IOCON_OPENDRAIN_EN;
    IOCON->PIO[PIN_PIO_PORT][PIN_HW_VERS_7] = IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN | IOCON_OPENDRAIN_EN;

    // Turn off LED.
    GPIO->B[PIN_PIO_PORT][LED_CONNECTED] = 1;
    // Set LED to output.
    GPIO->DIRSET[PIN_PIO_PORT] = LED_CONNECTED_MASK;
    // Turn on LED.
    GPIO->B[PIN_PIO_PORT][LED_CONNECTED] = 1;

    GPIO->DIRCLR[PIN_PIO_PORT] = PIN_HW_VERS_6_MASK;
    GPIO->DIRCLR[PIN_PIO_PORT] = PIN_HW_VERS_7_MASK;
}

void gpio_set_board_power(bool powerEnabled)
{
    // No target power control in this circuit.
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    // LED is active low, so set to inverse of the enum value.
    GPIO->B[PIN_PIO_PORT][LED_CONNECTED] = !(uint8_t)state;
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    gpio_set_hid_led(state);
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    gpio_set_hid_led(state);
}

uint8_t gpio_get_reset_btn_no_fwrd(void)
{
    return GPIO->B[PIN_PIO_PORT][PIN_HW_VERS_6] ? 0 : 1;
}

uint8_t gpio_get_reset_btn_fwrd(void)
{
    return GPIO->B[PIN_PIO_PORT][PIN_HW_VERS_7] ? 0 : 1;
}
