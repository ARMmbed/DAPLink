/**
 * @file    power.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2020 NXP
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

#include "fsl_smc.h"
#include "fsl_rcm.h"
#include "fsl_llwu.h"
#include "fsl_pmc.h"
#include "fsl_port.h"
#include "fsl_clock.h"

#include "main_interface.h"
#include "power.h"
#include "pwr_mon.h"
#include "IO_Config.h"
#include "uart.h"
#include "i2c.h"
#include "rl_usb.h"

/*******************************************************************************
 * Enums and macros
 ******************************************************************************/
/* Power mode definition used in application. */
typedef enum _app_power_mode
{
    kAPP_PowerModeMin = 0,
    kAPP_PowerModeRun,   /* Normal RUN mode */
    kAPP_PowerModeWait,  /* WAIT mode. */
    kAPP_PowerModeStop,  /* STOP mode. */
    kAPP_PowerModeVlpr,  /* VLPR mode. */
    kAPP_PowerModeVlpw,  /* VLPW mode. */
    kAPP_PowerModeVlps,  /* VLPS mode. */
    kAPP_PowerModeLls,   /* LLS mode. */
    kAPP_PowerModeVlls0, /* VLLS0 mode. */
    kAPP_PowerModeVlls1, /* VLLS1 mode. */
    kAPP_PowerModeVlls3, /* VLLS3 mode. */
    kAPP_PowerModeMax
} app_power_mode_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void power_set_wakeup_config(app_power_mode_t targetMode);
static void power_mode_switch(smc_power_state_t curPowerState, app_power_mode_t targetPowerMode);
static void power_pre_switch_hook(smc_power_state_t originPowerState, app_power_mode_t targetMode);
static void power_post_switch_hook(smc_power_state_t originPowerState, app_power_mode_t targetMode);
static void power_enter_mode(app_power_mode_t targetPowerMode);

extern volatile uint8_t wake_from_reset;
extern volatile uint8_t wake_from_usb;
extern volatile bool usb_pc_connected;
extern main_usb_connect_t usb_state;
extern power_source_t power_source;

/*******************************************************************************
 * Code
 ******************************************************************************/
void LLWU_IRQHandler(void)
{
    /* If wakeup by external pin BTN_NOT_PRESSED. */
    if (LLWU_GetExternalWakeupPinFlag(LLWU, PIN_SW_RESET_LLWU_PIN))
    {
        LLWU_ClearExternalWakeupPinFlag(LLWU, PIN_SW_RESET_LLWU_PIN);
        wake_from_reset = 1;
    }
    /* If wakeup by external pin WAKE_ON_EDGE. */
    if (LLWU_GetExternalWakeupPinFlag(LLWU, PIN_WAKE_ON_EDGE_LLWU_PIN))
    {
        LLWU_ClearExternalWakeupPinFlag(LLWU, PIN_WAKE_ON_EDGE_LLWU_PIN);
        wake_from_usb = 1;
    }
}

void PORTCD_IRQHandler(void)
{
    if ((1U << PIN_SW_RESET_BIT) & PORT_GetPinsInterruptFlags(PIN_SW_RESET_PORT))
    {
        /* Disable interrupt. */
        PORT_SetPinInterruptConfig(PIN_SW_RESET_PORT, PIN_SW_RESET_BIT, kPORT_InterruptOrDMADisabled);
        PORT_ClearPinsInterruptFlags(PIN_SW_RESET_PORT, (1U << PIN_SW_RESET_BIT));
    }
    if ((1U << PIN_WAKE_ON_EDGE_BIT) & PORT_GetPinsInterruptFlags(PIN_WAKE_ON_EDGE_PORT))
    {
        PORT_ClearPinsInterruptFlags(PIN_WAKE_ON_EDGE_PORT, (1U << PIN_WAKE_ON_EDGE_BIT));
        
        power_source = pwr_mon_get_power_source();

        bool usb_on = (((PIN_WAKE_ON_EDGE_GPIO->PDIR) >> PIN_WAKE_ON_EDGE_BIT) & 0x01U) ? false : true;

        if (usb_on == false) {
            /* Reset USB on cable detach (VBUS falling edge) */
            USBD_Reset();
            usbd_reset_core();
            usb_pc_connected = false;
            usb_state = USB_DISCONNECTED;
        }
        else {
            // Cable inserted
            wake_from_usb = 1;
        }
    }
}

void power_init(void)
{
    // Configure pin as GPIO
    PORT_SetPinMux(PIN_WAKE_ON_EDGE_PORT, PIN_WAKE_ON_EDGE_BIT, kPORT_MuxAsGpio);
    
    /* Power related. */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    if (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM)) /* Wakeup from VLLS. */
    {
        PMC_ClearPeriphIOIsolationFlag(PMC);
    }

    /* Allow writes to USBREGEN */
    SIM->SOPT1CFG |= SIM_SOPT1CFG_URWE_MASK;
    /* Disable USB voltage regulator */
    SIM->SOPT1 &= ~SIM_SOPT1_USBREGEN_MASK;

    /* Enable either edge interrupt on WAKE_ON_EDGE pin to detect USB attach/detach */
    PORT_SetPinInterruptConfig(PIN_WAKE_ON_EDGE_PORT, PIN_WAKE_ON_EDGE_BIT, PIN_WAKE_ON_EDGE_PORT_WAKEUP_TYPE);

    NVIC_SetPriority(LLWU_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
    NVIC_EnableIRQ(LLWU_IRQn);
    NVIC_SetPriority(PORTC_PORTD_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
    NVIC_EnableIRQ(PORTC_PORTD_IRQn);
}

void power_down()
{
    power_enter_mode(kAPP_PowerModeVlls0);
}

void power_sleep()
{
    power_enter_mode(kAPP_PowerModeVlps);
}

static void power_enter_mode(app_power_mode_t targetPowerMode)
{
    smc_power_state_t curPowerState = (smc_power_state_t) 0;

    curPowerState = SMC_GetPowerModeState(SMC);

    if ((targetPowerMode > kAPP_PowerModeMin) && (targetPowerMode < kAPP_PowerModeMax))
    {
        power_pre_switch_hook(curPowerState, targetPowerMode);
        power_set_wakeup_config(targetPowerMode);
        power_mode_switch(curPowerState, targetPowerMode);
        power_post_switch_hook(curPowerState, targetPowerMode);
    }
}

static void power_set_wakeup_config(app_power_mode_t targetMode)
{
    PORT_SetPinInterruptConfig(PIN_SW_RESET_PORT, PIN_SW_RESET_BIT, PIN_SW_RESET_PORT_WAKEUP_TYPE);

    /* If targetMode is VLLS/LLS, setup LLWU. */
    if ((kAPP_PowerModeWait != targetMode) && (kAPP_PowerModeVlpw != targetMode) &&
        (kAPP_PowerModeVlps != targetMode) && (kAPP_PowerModeStop != targetMode))
    {
        LLWU_SetExternalWakeupPinMode(LLWU, PIN_SW_RESET_LLWU_PIN, PIN_SW_RESET_LLWU_WAKEUP_TYPE);
        LLWU_SetExternalWakeupPinMode(LLWU, PIN_WAKE_ON_EDGE_LLWU_PIN, PIN_WAKE_ON_EDGE_LLWU_WAKEUP_TYPE);
        NVIC_EnableIRQ(LLWU_IRQn);
    }
}

static void power_mode_switch(smc_power_state_t curPowerState, app_power_mode_t targetPowerMode)
{
    smc_power_mode_vlls_config_t vlls_config;
    vlls_config.enablePorDetectInVlls0 = true;

    switch (targetPowerMode)
    {
        case kAPP_PowerModeVlps:
            SMC_PreEnterStopModes();
            SMC_SetPowerModeVlps(SMC);
            SMC_PostExitStopModes();
            break;

        case kAPP_PowerModeVlls0:
            vlls_config.subMode = kSMC_StopSub0;
            SMC_PreEnterStopModes();
            SMC_SetPowerModeVlls(SMC, &vlls_config);
            SMC_PostExitStopModes();
            break;

        default:
            break;
    }
}

static void power_pre_switch_hook(smc_power_state_t originPowerState, app_power_mode_t targetMode)
{
    // TODO: Check if this is necessary, when we have time to test. This has always been in the V2 code.
    // The comment here is misleading. The code waits for completion of the current byte tranmission,
    // which is from interface to target. We could possibly move this into uart_uninitialize(),
    // and consider waiting for the write buffer to empty too, but as this is our only call to uart_uninitialize()
    // and we never get here with USB connected it may be unnecessary.
    /* Wait for debug console output finished. */
    while (!(LPUART_STAT_TC_MASK & UART->STAT))
    {
    }
    uart_uninitialize();

    /* Disable pins to lower current leakage */
    PORT_SetPinMux(UART_PORT, PIN_UART_RX_BIT, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(UART_PORT, PIN_UART_TX_BIT, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(PIN_HID_LED_PORT, PIN_HID_LED_BIT, kPORT_PinDisabledOrAnalog);
    
    /* Disable I/O pin SWCLK */
    PIN_SWCLK_PORT->PCR[PIN_SWCLK_BIT] = 0;
    
    /* Disable I/O pin SWDIO */
    PIN_SWDIO_PORT->PCR[PIN_SWDIO_BIT] = 0;
    
    /* If targetMode is VLLS0, disable I2C pins */
    if (kAPP_PowerModeVlls0 == targetMode)
    {
        /* PORTC1 is configured as I2C1_SCL */
        PORT_SetPinMux(PORTC, 1U, kPORT_PinDisabledOrAnalog);

        /* PORTC2 is configured as I2C1_SDA */
        PORT_SetPinMux(PORTC, 2U, kPORT_PinDisabledOrAnalog);
    }
}

static void power_post_switch_hook(smc_power_state_t originPowerState, app_power_mode_t targetMode)
{
    /* Configure I/O pin SWCLK */
    PIN_SWCLK_PORT->PCR[PIN_SWCLK_BIT] = PORT_PCR_MUX(1)  | /* GPIO */
                                         PORT_PCR_PE_MASK;  /* Pull (Down) enable */
    PIN_SWCLK_GPIO->PSOR  = PIN_SWCLK;                      /* High level */
    PIN_SWCLK_GPIO->PDDR |= PIN_SWCLK;                      /* Output */
    /* Configure I/O pin SWDIO */
    PIN_SWDIO_PORT->PCR[PIN_SWDIO_BIT] = PORT_PCR_MUX(1)  |  /* GPIO */
                                         PORT_PCR_PE_MASK |  /* Pull enable */
                                         PORT_PCR_PS_MASK;   /* Pull-up */
    
    /* re-configure pinmux of disabled pins */
    PORT_SetPinMux(UART_PORT, PIN_UART_RX_BIT, (port_mux_t)PIN_UART_RX_MUX_ALT);
    PORT_SetPinMux(UART_PORT, PIN_UART_TX_BIT, (port_mux_t)PIN_UART_TX_MUX_ALT);

    uart_initialize();
    // TODO: Check if this is necessary, when we have time to test. This has always been in the V2 code.
    // It used to be at the end of board_handle_powerdown. We are not aware that this is causing a problem,
    // but it seems odd if we have been woken by an I2C transaction from the target.
    i2c_deinitialize();
    i2c_initialize();
}
