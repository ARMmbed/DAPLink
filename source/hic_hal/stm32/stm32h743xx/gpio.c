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

#include "stm32h7xx.h"
#include "DAP_config.h"
#include "gpio.h"
#include "daplink.h"
#include "util.h"

#define GPIO_INIT_STABILIZATION_DELAY_MS    (100)

static const dut_pin_group_t s_dut_pin_group[DUT_PIN_GROUP_ID_COUNT] =
{
    [DUT_PIN_GROUP_ID_UDC0_RST_L] =
    {
        .io_port = UDC0_RST_L_PORT,
        .io_pin = UDC0_RST_L_PIN,
        .io_pin_bit = UDC0_RST_L_PIN_Bit,
        .dir_port = UDC0_RST_L_DIR_PORT,
        .dir_pin = UDC0_RST_L_DIR_PIN,
        .dir_pin_bit = UDC0_RST_L_DIR_PIN_Bit,
        .active_high = false,
    },
    [DUT_PIN_GROUP_ID_UDC0_BOOT_L] =
    {
        .io_port = UDC0_BOOT_L_PORT,
        .io_pin = UDC0_BOOT_L_PIN,
        .io_pin_bit = UDC0_BOOT_L_PIN_Bit,
        .dir_port = UDC0_BOOT_L_DIR_PORT,
        .dir_pin = UDC0_BOOT_L_DIR_PIN,
        .dir_pin_bit = UDC0_BOOT_L_DIR_PIN_Bit,
        .active_high = false,
    },
    [DUT_PIN_GROUP_ID_UDC0_BUTTON_L] =
    {
        .io_port = UDC0_BUTTON_L_PORT,
        .io_pin = UDC0_BUTTON_L_PIN,
        .io_pin_bit = UDC0_BUTTON_L_PIN_Bit,
        .dir_port = UDC0_BUTTON_L_DIR_PORT,
        .dir_pin = UDC0_BUTTON_L_DIR_PIN,
        .dir_pin_bit = UDC0_BUTTON_L_DIR_PIN_Bit,
        .active_high = false,
    },
    [DUT_PIN_GROUP_ID_UDC1_RST] =
    {
        .io_port = UDC1_RST_PORT,
        .io_pin = UDC1_RST_PIN,
        .io_pin_bit = UDC1_RST_PIN_Bit,
        .dir_port = UDC1_RST_DIR_PORT,
        .dir_pin = UDC1_RST_DIR_PIN,
        .dir_pin_bit = UDC1_RST_DIR_PIN_Bit,
        .active_high = true,
    },
    [DUT_PIN_GROUP_ID_UDC1_BOOT] =
    {
        .io_port = UDC1_BOOT_PORT,
        .io_pin = UDC1_BOOT_PIN,
        .io_pin_bit = UDC1_BOOT_PIN_Bit,
        .dir_port = UDC1_BOOT_DIR_PORT,
        .dir_pin = UDC1_BOOT_DIR_PIN,
        .dir_pin_bit = UDC1_BOOT_DIR_PIN_Bit,
        .active_high = true,
    },
    [DUT_PIN_GROUP_ID_UDC1_BUTTON] =
    {
        .io_port = UDC1_BUTTON_PORT,
        .io_pin = UDC1_BUTTON_PIN,
        .io_pin_bit = UDC1_BUTTON_PIN_Bit,
        .dir_port = UDC1_BUTTON_DIR_PORT,
        .dir_pin = UDC1_BUTTON_DIR_PIN,
        .dir_pin_bit = UDC1_BUTTON_DIR_PIN_Bit,
        .active_high = true,
    },
};

swd_dut_t g_cur_swd_dut = SWD_DUT0;

swd_dut_config_t g_swd_dut_configs[SWD_DUT_COUNT] =
{
    [SWD_DUT0] =
    {
        .swclk =
        {
            .port = SWCLK_TCK_PIN_PORT,
            .pin = SWCLK_TCK_PIN,
            .pin_bit = SWCLK_TCK_PIN_Bit,
        },
        .swdio =
        {
            .port = SWDIO_PIN_PORT,
            .pin = SWDIO_PIN,
            .pin_bit = SWDIO_PIN_Bit,
        },
        .nreset =
        {
            .port = nRESET_PIN_PORT,
            .pin = nRESET_PIN,
            .pin_bit = nRESET_PIN_Bit
        },
        .nreset_dir =
        {
            .port = nRESET_DIR_PIN_PORT,
            .pin = nRESET_DIR_PIN,
            .pin_bit = nRESET_DIR_PIN_Bit,
        },
        .swd_en_buf =
        {
            .port = SWD_BUFFER_EN_PORT,
            .pin = SWD_BUFFER_EN_PIN,
            .pin_bit = SWD_BUFFER_EN_PIN_Bit,
        }
    },
    [SWD_DUT1] =
    {
        .swclk =
        {
            .port = JTAG1_MCU_UDC_TCK_SWDCLK_PORT,
            .pin = JTAG1_MCU_UDC_TCK_SWDCLK_PIN,
            .pin_bit = JTAG1_MCU_UDC_TCK_SWDCLK_PIN_Bit,
        },
        .swdio =
        {
            .port = JTAG1_MCU_UDC_TMS_SWDIO_PORT,
            .pin = JTAG1_MCU_UDC_TMS_SWDIO_PIN,
            .pin_bit = JTAG1_MCU_UDC_TMS_SWDIO_PIN_Bit,
        },
        .nreset =
        {
            .port = UDC1_RST_PORT,
            .pin = UDC1_RST_PIN,
            .pin_bit = UDC1_RST_PIN_Bit,
        },
        .nreset_dir =
        {
            .port = UDC1_RST_DIR_PORT,
            .pin = UDC1_RST_DIR_PIN,
            .pin_bit = UDC1_RST_DIR_PIN_Bit,
        },
        .swd_en_buf =
        {
            .port = OE_L_CTRL1_PORT,
            .pin = OE_L_CTRL1_PIN,
            .pin_bit = OE_L_CTRL1_PIN_Bit,
        }
    },
};

/*
 * Initialize GPIO signals to DUT
 *
 * These signals connect to bi-directional level shifters, with one GPIO controlling the direction.
 *
 * The level shifter direction is normally an input (to the MCU) so the DUT can pull high/low as needed and a button can
 * also pull hi/low manually.
 * To "activate" the signal the MCU pin has a weak IO pullup/down so when the buffer becomes an output (from the MCU)
 * the DUT line is driven high/low as needed.
 */
static void gpio_init_dut_pin_group(dut_pin_group_id_t dut_pin_group_id)
{
    util_assert(IS_DUT_PIN_GROUP_ID(dut_pin_group_id));

    GPIO_InitTypeDef gpio_init;

    HAL_GPIO_WritePin(s_dut_pin_group[dut_pin_group_id].dir_port, s_dut_pin_group[dut_pin_group_id].dir_pin, GPIO_PIN_RESET);  // input to MCU

    gpio_init.Pin = s_dut_pin_group[dut_pin_group_id].dir_pin;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(s_dut_pin_group[dut_pin_group_id].dir_port, &gpio_init);

    // MCU pin is initialized as an input with pulldown (active low) or pullup (active high)
    gpio_init.Pin = s_dut_pin_group[dut_pin_group_id].io_pin;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(s_dut_pin_group[dut_pin_group_id].io_port, &gpio_init);
}

void gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // enable clock to ports
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    // configure LEDs
    HAL_GPIO_WritePin(RUNNING_LED_PORT, RUNNING_LED_PIN, GPIO_PIN_SET);
    GPIO_InitStructure.Pin = RUNNING_LED_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(RUNNING_LED_PORT, &GPIO_InitStructure);

    if (daplink_is_interface())
    {
        HAL_GPIO_WritePin(CONNECTED_LED_PORT, CONNECTED_LED_PIN, GPIO_PIN_SET);
        GPIO_InitStructure.Pin = CONNECTED_LED_PIN;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
    }
    else
    {
        GPIO_InitStructure.Pin = CONNECTED_LED_PIN;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Pull = GPIO_PULLDOWN;
    }
    HAL_GPIO_Init(CONNECTED_LED_PORT, &GPIO_InitStructure);

    HAL_GPIO_WritePin(PIN_CDC_LED_PORT, PIN_CDC_LED, GPIO_PIN_SET);
    GPIO_InitStructure.Pin = PIN_CDC_LED;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(PIN_CDC_LED_PORT, &GPIO_InitStructure);

    HAL_GPIO_WritePin(PIN_MSC_LED_PORT, PIN_MSC_LED, GPIO_PIN_SET);
    GPIO_InitStructure.Pin = PIN_MSC_LED;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(PIN_MSC_LED_PORT, &GPIO_InitStructure);

    // Setup the USB Hub to be "self powered" (very common setting, even if not strictly compliant).
    GPIO_InitStructure.Pin = USBHUB_SELFPWR_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USBHUB_SELFPWR_PORT, &GPIO_InitStructure);
    HAL_GPIO_WritePin(USBHUB_SELFPWR_PORT, USBHUB_SELFPWR_PIN, GPIO_PIN_RESET);

    //Initialize bidir buffer control signals
    GPIO_InitStructure.Pin = SWD_BUFFER_EN_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_WritePin(SWD_BUFFER_EN_PORT, SWD_BUFFER_EN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_Init(SWD_BUFFER_EN_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = OE_L_CTRL1_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_WritePin(OE_L_CTRL1_PORT, OE_L_CTRL1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_Init(OE_L_CTRL1_PORT, &GPIO_InitStructure);

    // These are DUT pins connected level shifters.
    gpio_init_dut_pin_group(DUT_PIN_GROUP_ID_UDC0_RST_L);
    gpio_init_dut_pin_group(DUT_PIN_GROUP_ID_UDC0_BOOT_L);
    gpio_init_dut_pin_group(DUT_PIN_GROUP_ID_UDC0_BUTTON_L);
    gpio_init_dut_pin_group(DUT_PIN_GROUP_ID_UDC1_RST);
    gpio_init_dut_pin_group(DUT_PIN_GROUP_ID_UDC1_BOOT);
    gpio_init_dut_pin_group(DUT_PIN_GROUP_ID_UDC1_BUTTON);

    // Turn on power to the board. When the target is unpowered
    // it holds the reset line low.
    if (daplink_is_interface())
    {
        HAL_GPIO_WritePin(POWER_EN_PIN_PORT, POWER_EN_PIN, GPIO_PIN_RESET);
        GPIO_InitStructure.Pin = POWER_EN_PIN;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(POWER_EN_PIN_PORT, &GPIO_InitStructure);
    }

    //Initialize external relay (turned on)
    HAL_GPIO_WritePin(UDC_EXT_RELAY_PORT, UDC_EXT_RELAY_PIN, GPIO_PIN_RESET);
    GPIO_InitStructure.Pin = UDC_EXT_RELAY_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(UDC_EXT_RELAY_PORT, &GPIO_InitStructure);

    // Let the voltage rails stabilize.  This is especailly important
    // during software resets, since the target's 3.3v rail can take
    // 20-50ms to drain.  During this time the target could be driving
    // the reset pin low, causing the bootloader to think the reset
    // button is pressed.
    HAL_Delay(GPIO_INIT_STABILIZATION_DELAY_MS);
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    // LED is active low
    HAL_GPIO_WritePin(PIN_HID_LED_PORT, PIN_HID_LED, state ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    // LED is active low
    HAL_GPIO_WritePin(PIN_CDC_LED_PORT, PIN_CDC_LED, state ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    // LED is active low
    HAL_GPIO_WritePin(PIN_MSC_LED_PORT, PIN_MSC_LED, state ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

/*
 * We share CONNECTED_LED with the additional button on UDB P3.
 * Use the button as the new "reset_button" so that users on P3
 * can get into bootloader mode without debug console.
 */
bool reset_button_pressed(void)
{
    return (CONNECTED_LED_PORT->IDR & CONNECTED_LED_PIN) ? 1: 0;
}

uint8_t gpio_get_reset_btn_no_fwrd(void)
{
    return (nRESET_PIN_PORT->IDR & nRESET_PIN) ? 0 : 1;
}

uint8_t gpio_get_reset_btn_fwrd(void)
{
    return 0;
}


uint8_t GPIOGetButtonState(void)
{
    return 0;
}

void target_forward_reset(bool assert_reset)
{
    // Do nothing - reset is forwarded in gpio_get_sw_reset
}

void gpio_set_board_power(bool powerEnabled)
{
}

void gpio_config_dut_pin_group(dut_pin_group_id_t dut_pin_group_id, dut_pin_group_state_t dut_pin_group_state, uint16_t pull)
{
    util_assert(IS_DUT_PIN_GROUP_ID(dut_pin_group_id));
    util_assert(IS_DUT_PIN_GROUP_STATE(dut_pin_group_state));
    util_assert(IS_GPIO_PULL(pull));

    GPIO_InitTypeDef gpio_init =
    {
        .Pin = s_dut_pin_group[dut_pin_group_id].io_pin,
        .Mode = GPIO_MODE_INPUT,
        .Pull = pull,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };

    if (dut_pin_group_state == DUT_PIN_GROUP_STATE_INPUT)
    {
        HAL_GPIO_WritePin(s_dut_pin_group[dut_pin_group_id].dir_port, s_dut_pin_group[dut_pin_group_id].dir_pin, GPIO_PIN_RESET);
    }
    else if (dut_pin_group_state == DUT_PIN_GROUP_STATE_OUTPUT)
    {
        HAL_GPIO_WritePin(s_dut_pin_group[dut_pin_group_id].dir_port, s_dut_pin_group[dut_pin_group_id].dir_pin, GPIO_PIN_SET);
    }
    HAL_GPIO_Init(s_dut_pin_group[dut_pin_group_id].io_port, &gpio_init);
}

dut_pin_group_t gpio_get_dut_pin_group(dut_pin_group_id_t dut_pin_group_id)
{
    util_assert(IS_DUT_PIN_GROUP_ID(dut_pin_group_id));

    return s_dut_pin_group[dut_pin_group_id];
}

GPIO_PinState gpio_read_dut_dir_pin(dut_pin_group_id_t dut_pin_group_id)
{
    util_assert(IS_DUT_PIN_GROUP_ID(dut_pin_group_id));

    return HAL_GPIO_ReadPin(s_dut_pin_group[dut_pin_group_id].dir_port, s_dut_pin_group[dut_pin_group_id].dir_pin);
}

GPIO_PinState gpio_read_dut_io_pin(dut_pin_group_id_t dut_pin_group_id)
{
    util_assert(IS_DUT_PIN_GROUP_ID(dut_pin_group_id));

    return HAL_GPIO_ReadPin(s_dut_pin_group[dut_pin_group_id].io_port, s_dut_pin_group[dut_pin_group_id].io_pin);
}

// Configure the level-shifter direction to output before using this function
void gpio_write_dut_pin(dut_pin_group_id_t dut_pin_group_id, GPIO_PinState pin_state)
{
    util_assert(IS_DUT_PIN_GROUP_ID(dut_pin_group_id));
    util_assert(IS_GPIO_PIN_ACTION(pin_state));
    util_assert(gpio_read_dut_dir_pin(dut_pin_group_id) == GPIO_PIN_SET);

    uint32_t pupd_reg = s_dut_pin_group[dut_pin_group_id].io_port->PUPDR;
    pupd_reg &= ~(3UL << (s_dut_pin_group[dut_pin_group_id].io_pin_bit * 2U));

    pupd_reg |= (pin_state << (s_dut_pin_group[dut_pin_group_id].io_pin_bit * 2U));
    s_dut_pin_group[dut_pin_group_id].io_port->PUPDR = pupd_reg;
}

bool gpio_dut_pin_group_is_active_high(dut_pin_group_id_t dut_pin_group_id)
{
    return s_dut_pin_group[dut_pin_group_id].active_high;
}
