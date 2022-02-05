/**
 * @file    power.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2021 Micro:bit Educational Foundation
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

#include "nrf.h"

#include "gpio.h"
#include "gpio_extra.h"
#include "main_interface.h"
#include "pwr_mon.h"
#include "IO_Config.h"
#include "DAP_config.h"
#include "uart.h"
#include "i2c.h"
#include "rl_usb.h"
#include "microbitv2_pins.h"

#include "power.h"


static void power_before(bool systemoff);
static void power_after(void);
static void power_systemoff(void);
static void power_wfi(void);

static void power_gpio_set_sense(NRF_GPIO_Type *reg, uint32_t idx, nrf_gpio_pin_sense_t sense);

static void power_gpio_set_sense_from_read(NRF_GPIO_Type *reg, uint32_t idx);

void GPIOTE_IRQHandler(void);


extern volatile uint8_t wake_from_reset;
extern volatile uint8_t wake_from_usb;
extern volatile bool usb_pc_connected;
extern main_usb_connect_t usb_state;
extern power_source_t power_source;


uint8_t  power_gpiote_enabled;
uint32_t power_gpiote_intenset;

// Define one of these
// Using USBDETECTED for wake-up from SYSTEMOFF
// results in different values in NRF_POWER->RESETREAS
// for reset button and USB wake-ups
#define POWER_IRQ_USBDETECTED 1
//#define POWER_IRQ_VBUS_ABSENT 1

void power_init()
{
    if (NRF_POWER->RESETREAS & POWER_RESETREAS_VBUS_Msk) {
        wake_from_usb = 1;
    }
    if (NRF_POWER->RESETREAS & POWER_RESETREAS_OFF_Msk) {
        wake_from_reset = 1;
    }

    microbitv2_pins_init();

    // Store NRF_GPIOTE state
    power_gpiote_intenset = NRF_GPIOTE->INTENSET;
    NRF_GPIOTE->INTENCLR = NRF_GPIOTE->INTENSET;

    // Override hic_hal gpio_init() RESET_BUTTON_PULL (=NRF_GPIO_PIN_PULLUP)
    gpio_cfg_input(GPIO_REG(RESET_BUTTON), GPIO_IDX(RESET_BUTTON), NRF_GPIO_PIN_NOPULL);
    
    // Configure VBUS_ABSENT (WAKE_ON_EDGE) pin as input */
    gpio_cfg_input(GPIO_REG(PIN_VBUS_ABSENT), GPIO_IDX(PIN_VBUS_ABSENT), NRF_GPIO_PIN_NOPULL);
    
#ifdef POWER_IRQ_VBUS_ABSENT
    // Configure VBUS_ABSENT (WAKE_ON_EDGE) pin to detect USB attach/detach */
    power_gpio_set_sense_from_read(GPIO_REG(PIN_VBUS_ABSENT), GPIO_IDX(PIN_VBUS_ABSENT)); 
#endif // POWER_IRQ_VBUS_ABSENT

    NRF_GPIOTE->EVENTS_PORT = 0;
    NRF_GPIOTE->INTENSET = power_gpiote_intenset | ( GPIOTE_INTENSET_PORT_Set << GPIOTE_INTENSET_PORT_Pos);
    NVIC_EnableIRQ(GPIOTE_IRQn);

#ifdef POWER_IRQ_USBDETECTED
    // Enable NRF_POWER interrupt for USB detected/removed
    NRF_POWER->INTENCLR = NRF_POWER->INTENSET;
    NRF_POWER->INTENSET = POWER_INTENSET_USBDETECTED_Msk | POWER_INTENSET_USBREMOVED_Msk;
    NVIC_EnableIRQ(POWER_CLOCK_IRQn);
#endif // POWER_IRQ_USBDETECTED
}

void power_down()
{
    power_systemoff();
}

void power_sleep()
{
    power_wfi();
}


static void power_before(bool systemoff)
{
    // KL27 waits "for debug console output finished" by checking (LPUART_STAT_TC_MASK & UART->STAT),
    // but we never get here with USB connected, so there is no need to wait

    uart_uninitialize(); // disables RX and TX pins

    gpio_disable_hid_led();
    
    /* Disable I/O pin SWCLK, SWDIO */
    PORT_OFF();

    if (systemoff) {
        i2c_deinitialize(); // disables I2C SCL & SDA
    }

    // Store NRF_GPIOTE state
    power_gpiote_enabled = NVIC_GetEnableIRQ(GPIOTE_IRQn);
    power_gpiote_intenset = NRF_GPIOTE->INTENSET;
    NRF_GPIOTE->INTENCLR = NRF_GPIOTE->INTENSET;

#ifdef POWER_IRQ_VBUS_ABSENT
    // Configure VBUS_ABSENT (WAKE_ON_EDGE) pin to detect USB attach */
    power_gpio_set_sense(GPIO_REG(PIN_VBUS_ABSENT), GPIO_IDX(PIN_VBUS_ABSENT), NRF_GPIO_PIN_SENSE_LOW);
#endif // POWER_IRQ_VBUS_ABSENT

    // Enable IRQ from RESET_BUTTON 
    power_gpio_set_sense(GPIO_REG(RESET_BUTTON), GPIO_IDX(RESET_BUTTON), NRF_GPIO_PIN_SENSE_LOW);

    NRF_GPIOTE->EVENTS_PORT = 0;
    NRF_GPIOTE->INTENSET = power_gpiote_intenset | ( GPIOTE_INTENSET_PORT_Set << GPIOTE_INTENSET_PORT_Pos);
    NVIC_EnableIRQ(GPIOTE_IRQn);

#ifdef POWER_IRQ_USBDETECTED
    // Enable NRF_POWER interrupt for USB detected/removed
    NRF_POWER->INTENCLR = NRF_POWER->INTENSET;
    NRF_POWER->INTENSET = POWER_INTENSET_USBDETECTED_Msk;
    NVIC_EnableIRQ(POWER_CLOCK_IRQn);
#endif // POWER_IRQ_USBDETECTED
}

static void power_after()
{
#ifdef POWER_IRQ_USBDETECTED
    // Enable NRF_POWER interrupt for USB detected/removed
    NRF_POWER->INTENCLR = NRF_POWER->INTENSET;
    NRF_POWER->INTENSET = POWER_INTENSET_USBDETECTED_Msk | POWER_INTENSET_USBREMOVED_Msk;
    NVIC_EnableIRQ(POWER_CLOCK_IRQn);
#endif // POWER_IRQ_USBDETECTED

    // Restore GPIOTE state
    if (!power_gpiote_enabled) {
        NVIC_DisableIRQ(GPIOTE_IRQn);
    }
    NRF_GPIOTE->INTENCLR = NRF_GPIOTE->INTENSET;

#ifdef POWER_IRQ_VBUS_ABSENT
    // Configure VBUS_ABSENT (WAKE_ON_EDGE) pin to detect USB attach/detach */
    power_gpio_set_sense_from_read(GPIO_REG(PIN_VBUS_ABSENT), GPIO_IDX(PIN_VBUS_ABSENT));
#endif // POWER_IRQ_VBUS_ABSENT

    // Disable RESET_BUTTON edge events
    power_gpio_set_sense(GPIO_REG(RESET_BUTTON), GPIO_IDX(RESET_BUTTON), NRF_GPIO_PIN_NOSENSE);

    NRF_GPIOTE->EVENTS_PORT = 0;
    NRF_GPIOTE->INTENSET = power_gpiote_intenset;
    if (power_gpiote_enabled) {
        NVIC_EnableIRQ(GPIOTE_IRQn);
    }

    /* Configure I/O pin SWCLK, SWDIO */
    PORT_SWD_SETUP();
    
    uart_initialize();
    // The KL27 code calls i2c_deinitialize() and i2c_initialize()
    // but tests have indicated this is not necessary here
}


static void power_systemoff()
{
    power_before(true /*systemoff*/);
    NRF_POWER->SYSTEMOFF = 1;
    //never get here
}

static void power_wfi()
{
    power_before(false /*systemoff*/);
    __WFI();
    power_after();
}


static void power_gpio_set_sense(NRF_GPIO_Type *reg, uint32_t idx, nrf_gpio_pin_sense_t sense) {
    reg->PIN_CNF[idx] = ( reg->PIN_CNF[idx] & ~GPIO_PIN_CNF_SENSE_Msk) | ( sense << GPIO_PIN_CNF_SENSE_Pos);
    reg->LATCH = 1 << idx;
}

static void power_gpio_set_sense_from_read(NRF_GPIO_Type *reg, uint32_t idx) {
    uint32_t sense = gpio_read(reg, idx) ? NRF_GPIO_PIN_SENSE_LOW : NRF_GPIO_PIN_SENSE_HIGH;
    reg->PIN_CNF[idx] = ( reg->PIN_CNF[idx] & ~GPIO_PIN_CNF_SENSE_Msk) | ( sense << GPIO_PIN_CNF_SENSE_Pos);
    reg->LATCH = 1 << idx;
}

void GPIOTE_IRQHandler(void)
{
    if (NRF_GPIOTE->EVENTS_PORT) {
        NRF_GPIOTE->EVENTS_PORT = 0;

        NRF_GPIO_Type *reg = GPIO_REG(RESET_BUTTON);
        uint32_t idx = GPIO_IDX(RESET_BUTTON);
        if (reg->LATCH & (1 << idx)) {
            reg->LATCH = 1 << idx;
            // Disable RESET_BUTTON edge events
            power_gpio_set_sense(reg, idx, NRF_GPIO_PIN_NOSENSE);
            wake_from_reset = 1;
        }

#ifdef POWER_IRQ_VBUS_ABSENT
        reg = GPIO_REG(PIN_VBUS_ABSENT);
        idx = GPIO_IDX(PIN_VBUS_ABSENT);
        if (reg->LATCH & (1 << idx)) {
            reg->LATCH = 1 << idx;
            
            bool absent = NRF_GPIO_PIN_SENSE_HIGH == ((reg->PIN_CNF[idx] & GPIO_PIN_CNF_SENSE_Msk) >> GPIO_PIN_CNF_SENSE_Pos);

            power_gpio_set_sense(reg, idx, absent ? NRF_GPIO_PIN_SENSE_LOW : NRF_GPIO_PIN_SENSE_HIGH);

            power_source = pwr_mon_get_power_source();

            // Read VBUS_ABSENT (WAKE_ON_EDGE) pin for detecting if board is USB powered
            if (absent) {
                /* Reset USB on cable detach (VBUS falling edge) */
                USBD_Reset();
                usbd_reset_core();
                usb_pc_connected = false;
                usb_state = USB_DISCONNECTED;
            } else {
                // Cable inserted
                wake_from_usb = 1;
            }
        }
#endif // POWER_IRQ_VBUS_ABSENT
    }
}


#ifdef POWER_IRQ_USBDETECTED
void POWER_CLOCK_IRQHandler(void)
{
    if (NRF_POWER->EVENTS_USBDETECTED) {
        NRF_POWER->EVENTS_USBDETECTED = 0;
        power_source = pwr_mon_get_power_source();
        wake_from_usb = 1;
    }

    if (NRF_POWER->EVENTS_USBREMOVED) {
        NRF_POWER->EVENTS_USBREMOVED = 0;
        power_source = pwr_mon_get_power_source();
        /* Reset USB on cable detach (VBUS falling edge) */
        USBD_Reset();
        usbd_reset_core();
        usb_pc_connected = false;
        usb_state = USB_DISCONNECTED;
    }
}
#endif // POWER_IRQ_USBDETECTED
