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

#include "gd32f30x.h"
#include "daplink.h"

// This GPIO configuration is only valid for the GD32F303 HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_GD32F303);

#define PIN_MODE_MASK(pin)      (((uint32_t)0x0F) << ((pin) * 4))
#define PIN_MODE(mode, pin)     (((uint32_t)mode) << ((pin) * 4))
#define PIN_MASK(pin)           (((uint16_t)1) << (pin))

#define LED_RCU                 RCU_APB2EN_PBEN

/* DAP LED (Green) */
#define LED_DAP_PORT            GPIOB
#define LED_DAP_PIN             GPIO_PIN_12

/* MSD LED (Yellow) */
#define LED_MSC_PORT            GPIOB
#define LED_MSC_PIN             GPIO_PIN_13

/* CDC LED (Red) */
#define LED_CDC_PORT            GPIOB
#define LED_CDC_PIN             GPIO_PIN_14

/* USB Connect Pull-Up */
#define PIN_USB_CONNECT_RCU     RCU_APB2EN_PAEN
#define PIN_USB_CONNECT_PORT    GPIOA
#define PIN_USB_CONNECT_PIN     8
#define PIN_USB_CONNECT         PIN_MASK(PIN_USB_CONNECT_PIN)
#define PIN_USB_MODE            GPIO_MODE_OUT_PP
#define PIN_USB_CONNECT_ON()    GPIO_BOP(PIN_USB_CONNECT_PORT) = PIN_USB_CONNECT
#define PIN_USB_CONNECT_OFF()   GPIO_BC(PIN_USB_CONNECT_PORT) = PIN_USB_CONNECT

/* SWDIO/TMS Pin clock enable */
#define PIN_SWDIO_TMS_RCU       RCU_APB2EN_PAEN
#define PIN_SWCLK_TCK_RCU       RCU_APB2EN_PAEN

/* SWDIO/TMS Pin */
#define PIN_SWDIO_TMS_PORT      GPIOA
#define PIN_SWDIO_TMS_PIN       4

/* SWCLK/TCK Pin */
#define PIN_SWCLK_TCK_PORT      GPIOA
#define PIN_SWCLK_TCK_PIN       5

/* TDO/SWO Pin (input) */
#define PIN_TDO_PORT            GPIOA
#define PIN_TDO                 GPIO_PIN_6

/* TDI Pin (output) */
#define PIN_TDI_PORT            GPIOA
#define PIN_TDI                 GPIO_PIN_7

/* nRESET Pin clock enable */
#define PIN_nRESET_RCU          RCU_APB2EN_PBEN

/* nRESET Pin */
#define PIN_nRESET_PORT         GPIOB
#define PIN_nRESET_PIN          9

#define PIN_nRESET              PIN_MASK(PIN_nRESET_PIN)
#define PIN_SWDIO_TMS           PIN_MASK(PIN_SWDIO_TMS_PIN)
#define PIN_SWCLK_TCK           PIN_MASK(PIN_SWCLK_TCK_PIN)

#if (PIN_nRESET_PIN >= 8)
    #define PIN_nRESET_LOW() \
        do {    \
            /* GPIO_Mode_Out_OD | GPIO_Speed_50MHz */   \
            GPIO_CTL1(PIN_nRESET_PORT) = (GPIO_CTL1(PIN_nRESET_PORT) & ~PIN_MODE_MASK(PIN_nRESET_PIN - 8))  \
                                    | PIN_MODE(((GPIO_MODE_OUT_OD | GPIO_OSPEED_50MHZ) & 0x0F), PIN_nRESET_PIN - 8);	\
           } while (0)

    #define PIN_nRESET_HIGH()   \
        do {    \
            GPIO_CTL1(PIN_nRESET_PORT) = (GPIO_CTL1(PIN_nRESET_PORT) & ~PIN_MODE_MASK(PIN_nRESET_PIN - 8))  \
                                    | PIN_MODE(GPIO_MODE_IN_FLOATING, PIN_nRESET_PIN - 8);  \
           } while (0)
#else
    #define PIN_nRESET_LOW()    \
        do {    \
            /* GPIO_Mode_Out_OD | GPIO_Speed_50MHz */  \
            GPIO_CTL0(PIN_nRESET_PORT) = (GPIO_CTL0(PIN_nRESET_PORT) & ~PIN_MODE_MASK(PIN_nRESET_PIN)) \
                                    | PIN_MODE(((GPIO_MODE_OUT_OD | GPIO_OSPEED_50MHZ) & 0x0F), PIN_nRESET_PIN); \
           } while (0)

    #define PIN_nRESET_HIGH()   \
        do {    \
            GPIO_CTL0(PIN_nRESET_PORT) = (GPIO_CTL0(PIN_nRESET_PORT) & ~PIN_MODE_MASK(PIN_nRESET_PIN))  \
                                    | PIN_MODE(GPIO_MODE_IN_FLOATING, PIN_nRESET_PIN);  \
           } while (0)
#endif

/* For fast switch between input and output mode without GPIO_Init call */
#if (PIN_SWDIO_TMS_PIN >= 8)
    #define PIN_SWDIO_TMS_OUT_DISABLE() \
        do {    \
                GPIO_CTL1(PIN_SWDIO_TMS_PORT) = (GPIO_CTL1(PIN_SWDIO_TMS_PORT) & ~PIN_MODE_MASK(PIN_SWDIO_TMS_PIN - 8)) | PIN_MODE(0x8, PIN_SWDIO_TMS_PIN - 8); \
                GPIO_BOP(PIN_SWDIO_TMS_PORT) = PIN_SWDIO_TMS; \
           } while (0)

    #define PIN_SWDIO_TMS_OUT_ENABLE()  \
        do {    \
                GPIO_CTL1(PIN_SWDIO_TMS_PORT) = (GPIO_CTL1(PIN_SWDIO_TMS_PORT) & ~PIN_MODE_MASK(PIN_SWDIO_TMS_PIN - 8)) | PIN_MODE(0x3, PIN_SWDIO_TMS_PIN - 8); \
                GPIO_BC(PIN_SWDIO_TMS_PORT) = PIN_SWDIO_TMS; \
           } while (0)
#else
    #define PIN_SWDIO_TMS_OUT_DISABLE() \
        do {    \
                GPIO_CTL0(PIN_SWDIO_TMS_PORT) = (GPIO_CTL0(PIN_SWDIO_TMS_PORT) & ~PIN_MODE_MASK(PIN_SWDIO_TMS_PIN)) | PIN_MODE(0x8, PIN_SWDIO_TMS_PIN);	\
                GPIO_BOP(PIN_SWDIO_TMS_PORT) = PIN_SWDIO_TMS; \
           } while (0)

    #define PIN_SWDIO_TMS_OUT_ENABLE() \
        do {    \
                GPIO_CTL0(PIN_SWDIO_TMS_PORT) = (GPIO_CTL0(PIN_SWDIO_TMS_PORT) & ~PIN_MODE_MASK(PIN_SWDIO_TMS_PIN)) | PIN_MODE(0x3, PIN_SWDIO_TMS_PIN);	\
                GPIO_BC(PIN_SWDIO_TMS_PORT) = PIN_SWDIO_TMS; \
           } while (0)

#endif

#endif
