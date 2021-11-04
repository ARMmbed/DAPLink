/**
 * @file    gpio.c
 * @brief   GPIO handling for LPC55xx HIC, MCU-LINK and MCU-LINK-PRO
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

static bool mcu_link_pro = false;

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
                                | SYSCON_AHBCLKCTRL0_GPIO0_MASK
                                | SYSCON_AHBCLKCTRL0_GPIO1_MASK;
    SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL1_FC0_MASK
                                | SYSCON_AHBCLKCTRL1_FC3_MASK;

    // Reset peripherals.
    RESET_PeripheralReset(kIOCON_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kFC0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kFC3_RST_SHIFT_RSTn);

    // Configure pins for board detection
    IOCON->PIO[PIN_HW_VERS_3_PORT][PIN_HW_VERS_3] = IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN | IOCON_OPENDRAIN_EN;
    IOCON->PIO[PIN_HW_VERS_4_PORT][PIN_HW_VERS_4] = IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN | IOCON_OPENDRAIN_EN;
    GPIO->DIRCLR[PIN_HW_VERS_3_PORT] = PIN_HW_VERS_3_MASK;
    GPIO->DIRCLR[PIN_HW_VERS_4_PORT] = PIN_HW_VERS_4_MASK;
    if (GPIO->B[PIN_HW_VERS_4_PORT][PIN_HW_VERS_4] == 0 && GPIO->B[PIN_HW_VERS_3_PORT][PIN_HW_VERS_3] != 0) {
        mcu_link_pro = true;
    }

    // Configure pins.
    IOCON->PIO[LED_A_PORT][LED_A_PIN] = IOCON_FUNC0 | IOCON_DIGITAL_EN;
    IOCON->PIO[PIN_PIO_PORT][PIN_RESET] = IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN | IOCON_OPENDRAIN_EN;
    IOCON->PIO[PIN_PIO_PORT][PIN_HW_VERS_6] = IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN | IOCON_OPENDRAIN_EN;
    IOCON->PIO[PIN_PIO_PORT][PIN_HW_VERS_7] = IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN | IOCON_OPENDRAIN_EN;
    if (mcu_link_pro) {
        IOCON->PIO[LED_B_PORT][LED_B_PIN] = IOCON_FUNC0 | IOCON_DIGITAL_EN;
        IOCON->PIO[LED_C_PORT][LED_C_PIN] = IOCON_FUNC0 | IOCON_DIGITAL_EN;
        IOCON->PIO[LED_D_PORT][LED_D_PIN] = IOCON_FUNC0 | IOCON_DIGITAL_EN;
        IOCON->PIO[LED_E_PORT][LED_E_PIN] = IOCON_FUNC0 | IOCON_DIGITAL_EN;
        IOCON->PIO[LED_F_PORT][LED_F_PIN] = IOCON_FUNC0 | IOCON_DIGITAL_EN;
        IOCON->PIO[LED_G_PORT][LED_G_PIN] = IOCON_FUNC0 | IOCON_DIGITAL_EN;
    }

    // Turn off LED.
    GPIO->B[LED_A_PORT][LED_A_PIN] = 1;
    if (mcu_link_pro) {
        GPIO->B[LED_B_PORT][LED_B_PIN] = 1;
        GPIO->B[LED_C_PORT][LED_C_PIN] = 1;
        GPIO->B[LED_D_PORT][LED_D_PIN] = 1;
        GPIO->B[LED_E_PORT][LED_E_PIN] = 1;
        GPIO->B[LED_F_PORT][LED_F_PIN] = 1;
        GPIO->B[LED_G_PORT][LED_G_PIN] = 1;
    }

    // Set LED to output.
    GPIO->DIRSET[LED_A_PORT] = LED_A_MASK;
    if (mcu_link_pro) {
        GPIO->DIRSET[LED_B_PORT] = LED_B_MASK;
        GPIO->DIRSET[LED_C_PORT] = LED_C_MASK;
        GPIO->DIRSET[LED_D_PORT] = LED_D_MASK;
        GPIO->DIRSET[LED_E_PORT] = LED_E_MASK;
        GPIO->DIRSET[LED_F_PORT] = LED_F_MASK;
        GPIO->DIRSET[LED_G_PORT] = LED_G_MASK;
    }

    // Turn on LED.
    GPIO->B[LED_A_PORT][LED_A_PIN] = 0;

    GPIO->DIRCLR[PIN_PIO_PORT] = PIN_RESET_MASK;
    GPIO->DIRCLR[PIN_PIO_PORT] = PIN_HW_VERS_6_MASK;
    GPIO->DIRCLR[PIN_PIO_PORT] = PIN_HW_VERS_7_MASK;
}

void gpio_set_board_power(bool powerEnabled)
{
    // No target power control in this circuit.
}

void gpio_set_leds(uint32_t leds, gpio_led_state_t state)
{
    // LED is active low, so set to inverse of the enum value.
    if(mcu_link_pro) {
        if(leds & LED_T_CONNECTED) {
            GPIO->B[LED_A_PORT][LED_A_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
        }
        if(leds & LED_T_RUNNING) {
            GPIO->B[LED_B_PORT][LED_B_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
        }
        if(leds & LED_T_HID) {
            GPIO->B[LED_C_PORT][LED_C_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
        }
        if(leds & LED_T_CDC) {
            GPIO->B[LED_D_PORT][LED_D_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
        }
        if(leds & LED_T_MSC) {
            GPIO->B[LED_E_PORT][LED_E_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
        }
        if(leds & LED_T_EXTRA1) {
            GPIO->B[LED_F_PORT][LED_F_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
        }
        if(leds & LED_T_EXTRA2) {
            GPIO->B[LED_G_PORT][LED_G_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
        }
    } else if(leds & (LED_T_CONNECTED | LED_T_HID | LED_T_CDC | LED_T_MSC)) {
        GPIO->B[LED_A_PORT][LED_A_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
    }
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    gpio_set_leds(LED_T_HID, state);
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    gpio_set_leds(LED_T_CDC, state);
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    gpio_set_leds(LED_T_MSC, state);
}

uint8_t gpio_get_reset_btn_no_fwrd(void)
{
    return (PIN_nRESET_IN() && GPIO->B[PIN_PIO_PORT][PIN_HW_VERS_6]) ? 0 : 1;
}

uint8_t gpio_get_reset_btn_fwrd(void)
{
    return GPIO->B[PIN_PIO_PORT][PIN_HW_VERS_7] ? 0 : 1;
}
