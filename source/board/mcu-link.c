/**
 * @file    mcu-link.c
 * @brief   MCU-LINK and MCU-LINK-PRO support
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2021, Arm Limited, All Rights Reserved
 * Copyright (c) 2021 Mathias Brossard <mathias@brossard.org>
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
#include "cmsis_os2.h"

static bool s_mcu_link_pro = false;

void board_gpio_init()
{
    // Ensure GPIO1 clocks are enabled.
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL0_IOCON_MASK
                                | SYSCON_AHBCLKCTRL0_GPIO0_MASK
                                | SYSCON_AHBCLKCTRL0_GPIO1_MASK;
    RESET_PeripheralReset(kGPIO1_RST_SHIFT_RSTn);

    // Configure additonal reset pins
    IOCON->PIO[PIN_PIO_PORT][PIN_HW_VERS_6] = IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN | IOCON_OPENDRAIN_EN;
    IOCON->PIO[PIN_PIO_PORT][PIN_HW_VERS_7] = IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN | IOCON_OPENDRAIN_EN;
    GPIO->DIRCLR[PIN_PIO_PORT] = PIN_HW_VERS_6_MASK;
    GPIO->DIRCLR[PIN_PIO_PORT] = PIN_HW_VERS_7_MASK;

    // Configure pins for board detection
    IOCON->PIO[PIN_HW_VERS_3_PORT][PIN_HW_VERS_3] = IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN | IOCON_OPENDRAIN_EN;
    IOCON->PIO[PIN_HW_VERS_4_PORT][PIN_HW_VERS_4] = IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN | IOCON_OPENDRAIN_EN;
    GPIO->DIRCLR[PIN_HW_VERS_3_PORT] = PIN_HW_VERS_3_MASK;
    GPIO->DIRCLR[PIN_HW_VERS_4_PORT] = PIN_HW_VERS_4_MASK;

    if (GPIO->B[PIN_HW_VERS_4_PORT][PIN_HW_VERS_4] == 0 && GPIO->B[PIN_HW_VERS_3_PORT][PIN_HW_VERS_3] != 0) {
        s_mcu_link_pro = true;

        GPIO->B[LED_B_PORT][LED_B_PIN] = 1;
        GPIO->B[LED_C_PORT][LED_C_PIN] = 1;
        GPIO->B[LED_D_PORT][LED_D_PIN] = 1;
        GPIO->B[LED_E_PORT][LED_E_PIN] = 1;
        GPIO->B[LED_F_PORT][LED_F_PIN] = 1;
        GPIO->B[LED_G_PORT][LED_G_PIN] = 1;

        IOCON->PIO[LED_B_PORT][LED_B_PIN] = IOCON_FUNC0 | IOCON_DIGITAL_EN;
        IOCON->PIO[LED_C_PORT][LED_C_PIN] = IOCON_FUNC0 | IOCON_DIGITAL_EN;
        IOCON->PIO[LED_D_PORT][LED_D_PIN] = IOCON_FUNC0 | IOCON_DIGITAL_EN;
        IOCON->PIO[LED_E_PORT][LED_E_PIN] = IOCON_FUNC0 | IOCON_DIGITAL_EN;
        IOCON->PIO[LED_F_PORT][LED_F_PIN] = IOCON_FUNC0 | IOCON_DIGITAL_EN;
        IOCON->PIO[LED_G_PORT][LED_G_PIN] = IOCON_FUNC0 | IOCON_DIGITAL_EN;

        GPIO->DIRSET[LED_B_PORT] = LED_B_MASK;
        GPIO->DIRSET[LED_C_PORT] = LED_C_MASK;
        GPIO->DIRSET[LED_D_PORT] = LED_D_MASK;
        GPIO->DIRSET[LED_E_PORT] = LED_E_MASK;
        GPIO->DIRSET[LED_F_PORT] = LED_F_MASK;
        GPIO->DIRSET[LED_G_PORT] = LED_G_MASK;

#if defined(DAPLINK_IF) && defined(MCU_LINK_PRO_GREETING)
        uint8_t j = 0;
        for (uint8_t i = 0; i < 7; i++) {
            osDelay(15);
            j = (j << 1 | 1);
            gpio_set_leds(j, GPIO_LED_ON);
        }
        osDelay(15);
        gpio_set_leds(j, GPIO_LED_OFF);
#endif
    }
}

__WEAK void gpio_set_leds(uint32_t leds, gpio_led_state_t state)
{
    // LED is active low, so set to inverse of the enum value.
    if (s_mcu_link_pro) {
        if (leds & LED_T_CONNECTED) {
            GPIO->B[LED_A_PORT][LED_A_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
        }
        if (leds & LED_T_RUNNING) {
            GPIO->B[LED_B_PORT][LED_B_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
        }
        if (leds & LED_T_HID) {
            GPIO->B[LED_C_PORT][LED_C_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
        }
        if (leds & LED_T_CDC) {
            GPIO->B[LED_D_PORT][LED_D_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
        }
        if (leds & LED_T_MSC) {
            GPIO->B[LED_E_PORT][LED_E_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
        }
        if (leds & LED_T_EXTRA1) {
            GPIO->B[LED_F_PORT][LED_F_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
        }
        if (leds & LED_T_EXTRA2) {
            GPIO->B[LED_G_PORT][LED_G_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
        }
    } else if (leds & (LED_T_CONNECTED | LED_T_HID | LED_T_CDC | LED_T_MSC)) {
        GPIO->B[LED_A_PORT][LED_A_PIN] = (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
    }
}


uint8_t gpio_get_reset_btn_no_fwrd(void)
{
    return (PIN_nRESET_IN() && GPIO->B[PIN_PIO_PORT][PIN_HW_VERS_6]) ? 0 : 1;
}

uint8_t gpio_get_reset_btn_fwrd(void)
{
    return GPIO->B[PIN_PIO_PORT][PIN_HW_VERS_7] ? 0 : 1;
}
