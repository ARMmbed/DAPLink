/**
 * @file    IO_Config.h
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

#ifndef __IO_CONFIG_H__
#define __IO_CONFIG_H__

#include "stm32h7xx.h"
#include "compiler.h"
#include "daplink.h"

COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_STM32H743II);

//Connected LED
#define CONNECTED_LED_PORT              GPIOG
#define CONNECTED_LED_PIN               GPIO_PIN_0
#define CONNECTED_LED_PIN_Bit           0

#define POWER_EN_PIN_PORT               UDC_DUT_USB_EN_L_PORT
#define POWER_EN_PIN                    UDC_DUT_USB_EN_L_PIN
#define POWER_EN_Bit                    UDC_DUT_USB_EN_L_PIN_Bit

// nRESET OUT Pin
// JTAG0_MCU_UDC_RESET_L, UDC0_RST_L
#define nRESET_PIN_PORT                 GPIOI
#define nRESET_PIN                      GPIO_PIN_4
#define nRESET_PIN_Bit                  4

// nRESET DIR Pin
// JTAG0_NRESET_DIR
#define nRESET_DIR_PIN_PORT             GPIOF
#define nRESET_DIR_PIN                  GPIO_PIN_8
#define nRESET_DIR_PIN_Bit              8

//SWD
// JTAG0_MCU_UDC_TCK_SWDCLK
#define SWCLK_TCK_PIN_PORT              GPIOD
#define SWCLK_TCK_PIN                   GPIO_PIN_7
#define SWCLK_TCK_PIN_Bit               7

// JTAG0_MCU_UDC_TMS_SWDIO
#define SWDIO_PIN_PORT                  GPIOD
#define SWDIO_PIN                       GPIO_PIN_2
#define SWDIO_PIN_Bit                   2

#define JTAG1_MCU_UDC_TCK_SWDCLK_PORT       GPIOI
#define JTAG1_MCU_UDC_TCK_SWDCLK_PIN        GPIO_PIN_5
#define JTAG1_MCU_UDC_TCK_SWDCLK_PIN_Bit    5

#define JTAG1_MCU_UDC_TMS_SWDIO_PORT        GPIOI
#define JTAG1_MCU_UDC_TMS_SWDIO_PIN         GPIO_PIN_6
#define JTAG1_MCU_UDC_TMS_SWDIO_PIN_Bit     6

#define OE_L_CTRL1_PORT                 GPIOE
#define OE_L_CTRL1_PIN                  GPIO_PIN_9
#define OE_L_CTRL1_PIN_Bit              9

// SWD bidir buffer enable, OE_L_CTRL0
#define SWD_BUFFER_EN_PORT              GPIOC
#define SWD_BUFFER_EN_PIN               GPIO_PIN_8
#define SWD_BUFFER_EN_PIN_Bit           8

#define TDI_IN_PIN_PORT                 GPIOD
#define TDI_IN_PIN                      GPIO_PIN_10
#define TDI_IN_PIN_Bit                  10

//LEDs
//USB status LED
#define RUNNING_LED_PORT                GPIOG
#define RUNNING_LED_PIN                 GPIO_PIN_1
#define RUNNING_LED_Bit                 1

#define PIN_HID_LED_PORT                GPIOG
#define PIN_HID_LED                     GPIO_PIN_2
#define PIN_HID_LED_Bit                 2

#define PIN_CDC_LED_PORT                GPIOG
#define PIN_CDC_LED                     GPIO_PIN_3
#define PIN_CDC_LED_Bit                 3

#define PIN_MSC_LED_PORT                GPIOG
#define PIN_MSC_LED                     GPIO_PIN_4
#define PIN_MSC_LED_Bit                 4

#define USBHUB_RESET_L_PORT             GPIOH
#define USBHUB_RESET_L_PIN              GPIO_PIN_13
#define USBHUB_RESET_L_PIN_Bit          13

//UDB specific signals
#define USBHUB_SELFPWR_PORT             GPIOH
#define USBHUB_SELFPWR_PIN              GPIO_PIN_14
#define USBHUB_SELFPWR_PIN_Bit          14

// USB external PHY reset
#define USBPHY_RESET_L_PORT             GPIOH
#define USBPHY_RESET_L_PIN              GPIO_PIN_15
#define USBPHY_RESET_L_PIN_Bit          15

//DUT USB port power switch
//VBUS_DUT_EN_L
#define UDC_DUT_USB_EN_L_PORT           GPIOH
#define UDC_DUT_USB_EN_L_PIN            GPIO_PIN_10
#define UDC_DUT_USB_EN_L_PIN_Bit        10

//External power relay control
//EXT_RELAY_EN
#define UDC_EXT_RELAY_PORT              GPIOE
#define UDC_EXT_RELAY_PIN               GPIO_PIN_11
#define UDC_EXT_RELAY_PIN_Bit           11

//GPIO LINES
//Open Drain/Collector usage: MCU IO has pulldown/up enabled.
//External buffer set to OUT to pull low/high, and IN to be high-z
//JTAG0_MCU_UDC_RESET_L, JTAG0_NRESET_DIR
#define UDC0_RST_L_PORT                 nRESET_PIN_PORT
#define UDC0_RST_L_PIN                  nRESET_PIN
#define UDC0_RST_L_PIN_Bit              nRESET_PIN_Bit
#define UDC0_RST_L_DIR_PORT             nRESET_DIR_PIN_PORT
#define UDC0_RST_L_DIR_PIN              nRESET_DIR_PIN
#define UDC0_RST_L_DIR_PIN_Bit          nRESET_DIR_PIN_Bit

//MCU_BOOT0_UDC_L, BOOT0_DIR
#define UDC0_BOOT_L_PORT                GPIOE
#define UDC0_BOOT_L_PIN                 GPIO_PIN_7
#define UDC0_BOOT_L_PIN_Bit             7
#define UDC0_BOOT_L_DIR_PORT            GPIOE
#define UDC0_BOOT_L_DIR_PIN             GPIO_PIN_15
#define UDC0_BOOT_L_DIR_PIN_Bit         15

//MCU_BUT_USR0_UDC_L, BUT_USR0_DIR
#define UDC0_BUTTON_L_PORT              GPIOE
#define UDC0_BUTTON_L_PIN               GPIO_PIN_8
#define UDC0_BUTTON_L_PIN_Bit           8
#define UDC0_BUTTON_L_DIR_PORT          GPIOF
#define UDC0_BUTTON_L_DIR_PIN           GPIO_PIN_2
#define UDC0_BUTTON_L_DIR_PIN_Bit       2

//JTAG1_MCU_UDC_RESET, JTAG1_RESET_DIR
#define UDC1_RST_PORT                   GPIOI
#define UDC1_RST_PIN                    GPIO_PIN_9
#define UDC1_RST_PIN_Bit                9
#define UDC1_RST_DIR_PORT               GPIOB
#define UDC1_RST_DIR_PIN                GPIO_PIN_4
#define UDC1_RST_DIR_PIN_Bit            4

//MCU_BOOT1_UDC, BOOT1_DIR
#define UDC1_BOOT_PORT                  GPIOE
#define UDC1_BOOT_PIN                   GPIO_PIN_1
#define UDC1_BOOT_PIN_Bit               1
#define UDC1_BOOT_DIR_PORT              GPIOE
#define UDC1_BOOT_DIR_PIN               GPIO_PIN_13
#define UDC1_BOOT_DIR_PIN_Bit           13

//MCU_BUT_USR1_UDC, BUT_USR1_DIR
#define UDC1_BUTTON_PORT                GPIOE
#define UDC1_BUTTON_PIN                 GPIO_PIN_3
#define UDC1_BUTTON_PIN_Bit             3
#define UDC1_BUTTON_DIR_PORT            GPIOE
#define UDC1_BUTTON_DIR_PIN             GPIO_PIN_14
#define UDC1_BUTTON_DIR_PIN_Bit         14


/*
 * These MX_ flags are required to use ARM I2C driver. The I2C number is
 * a little messy now. The i2c labels on schematic are inconsistent with
 * the stm32h7xx datasheet:
 *    schematic I2C0 -> datasheet I2C1
 *    schematic I2C1 -> datasheet I2C3
 *    schematic I2C2 -> datasheet I2C2
 */
// I2C1
#define MX_I2C1                         1
#define MX_HSI_VALUE                    64000000

/* Pin PB8 */
#define MX_I2C1_SCL_GPIO_Speed          GPIO_SPEED_FREQ_LOW
#define MX_I2C1_SCL_GPIO_FM6            __NULL
#define MX_I2C1_SCL_GPIOx               GPIOB
#define MX_I2C1_SCL_GPIO_PuPd           GPIO_NOPULL
#define MX_I2C1_SCL_GPIO_Pin            GPIO_PIN_8
#define MX_I2C1_SCL_GPIO_AF             GPIO_AF4_I2C1
#define MX_I2C1_SCL_GPIO_Mode           GPIO_MODE_AF_OD

/* Pin PB9 */
#define MX_I2C1_SDA_GPIO_Speed          GPIO_SPEED_FREQ_LOW
#define MX_I2C1_SDA_GPIOx               GPIOB
#define MX_I2C1_SDA_GPIO_PuPd           GPIO_NOPULL
#define MX_I2C1_SDA_GPIO_Pin            GPIO_PIN_9
#define MX_I2C1_SDA_GPIO_AF             GPIO_AF4_I2C1
#define MX_I2C1_SDA_GPIO_FM7            __NULL
#define MX_I2C1_SDA_GPIO_Mode           GPIO_MODE_AF_OD

#define MX_I2C3                         1

/* Pin PA8 */
#define MX_I2C3_SCL_GPIO_Speed          GPIO_SPEED_FREQ_LOW
#define MX_I2C3_SCL_GPIO_FM6            __NULL
#define MX_I2C3_SCL_GPIOx               GPIOA
#define MX_I2C3_SCL_GPIO_PuPd           GPIO_NOPULL
#define MX_I2C3_SCL_GPIO_Pin            GPIO_PIN_8
#define MX_I2C3_SCL_GPIO_AF             GPIO_AF4_I2C3
#define MX_I2C3_SCL_GPIO_Mode           GPIO_MODE_AF_OD

/* Pin PH8 */
#define MX_I2C3_SDA_GPIO_Speed          GPIO_SPEED_FREQ_LOW
#define MX_I2C3_SDA_GPIOx               GPIOH
#define MX_I2C3_SDA_GPIO_PuPd           GPIO_NOPULL
#define MX_I2C3_SDA_GPIO_Pin            GPIO_PIN_8
#define MX_I2C3_SDA_GPIO_AF             GPIO_AF4_I2C3
#define MX_I2C3_SDA_GPIO_FM7            __NULL
#define MX_I2C3_SDA_GPIO_Mode           GPIO_MODE_AF_OD

#define MX_I2C2                         1

/* Pin PF1 */
#define MX_I2C2_SCL_GPIO_Speed          GPIO_SPEED_FREQ_LOW
#define MX_I2C2_SCL_GPIO_FM6            __NULL
#define MX_I2C2_SCL_GPIOx               GPIOF
#define MX_I2C2_SCL_GPIO_PuPd           GPIO_NOPULL
#define MX_I2C2_SCL_GPIO_Pin            GPIO_PIN_1
#define MX_I2C2_SCL_GPIO_AF             GPIO_AF4_I2C2
#define MX_I2C2_SCL_GPIO_Mode           GPIO_MODE_AF_OD

/* Pin PF0 */
#define MX_I2C2_SDA_GPIO_Speed          GPIO_SPEED_FREQ_LOW
#define MX_I2C2_SDA_GPIOx               GPIOF
#define MX_I2C2_SDA_GPIO_PuPd           GPIO_NOPULL
#define MX_I2C2_SDA_GPIO_Pin            GPIO_PIN_0
#define MX_I2C2_SDA_GPIO_AF             GPIO_AF4_I2C2
#define MX_I2C2_SDA_GPIO_FM7            __NULL
#define MX_I2C2_SDA_GPIO_Mode           GPIO_MODE_AF_OD

typedef enum
{
    DUT_PIN_GROUP_ID_UDC0_RST_L,
    DUT_PIN_GROUP_ID_UDC0_BOOT_L,
    DUT_PIN_GROUP_ID_UDC0_BUTTON_L,
    DUT_PIN_GROUP_ID_UDC1_RST,
    DUT_PIN_GROUP_ID_UDC1_BOOT,
    DUT_PIN_GROUP_ID_UDC1_BUTTON,
    DUT_PIN_GROUP_ID_COUNT,
} dut_pin_group_id_t;

typedef enum
{
    DUT_PIN_GROUP_STATE_INPUT,
    DUT_PIN_GROUP_STATE_OUTPUT,
    DUT_PIN_GROUP_STATE_COUNT,
} dut_pin_group_state_t;

#define IS_DUT_PIN_GROUP_ID(id)           ((id) < DUT_PIN_GROUP_ID_COUNT)
#define IS_DUT_PIN_GROUP_STATE(state)     ((state) < DUT_PIN_GROUP_STATE_COUNT)
#define GPIO_CONFIG_DUT_DELAY_MS          (100)
#define GPIO_WRITE_DUT_DELAY_MS           (50)

typedef struct
{
    // io pin
    GPIO_TypeDef* io_port;
    uint16_t io_pin;
    uint16_t io_pin_bit;

    // direction pin
    GPIO_TypeDef* dir_port;
    uint16_t dir_pin;
    uint16_t dir_pin_bit;

    bool active_high;
} dut_pin_group_t;

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    uint16_t pin_bit;
} swd_dut_gpio_pin_t;

typedef struct
{
    swd_dut_gpio_pin_t swclk;
    swd_dut_gpio_pin_t swdio;
    swd_dut_gpio_pin_t nreset;
    swd_dut_gpio_pin_t nreset_dir;
    swd_dut_gpio_pin_t swd_en_buf;
} swd_dut_config_t;

typedef enum
{
    SWD_DUT0,
    SWD_DUT1,
    SWD_DUT_COUNT,
} swd_dut_t;

extern swd_dut_config_t g_swd_dut_configs[SWD_DUT_COUNT];
extern swd_dut_t g_cur_swd_dut;

void gpio_config_dut_pin_group(dut_pin_group_id_t dut_pin_group_id, dut_pin_group_state_t dut_pin_group_state, uint16_t pull);

dut_pin_group_t gpio_get_dut_pin_group(dut_pin_group_id_t dut_pin_group_id);

GPIO_PinState gpio_read_dut_dir_pin(dut_pin_group_id_t dut_pin_group_id);

GPIO_PinState gpio_read_dut_io_pin(dut_pin_group_id_t dut_pin_group_id);

void gpio_write_dut_pin(dut_pin_group_id_t dut_pin_group_id, GPIO_PinState pin_state);

bool gpio_dut_pin_group_is_active_high(dut_pin_group_id_t dut_pin_group_id);

#endif // __IO_CONFIG__
