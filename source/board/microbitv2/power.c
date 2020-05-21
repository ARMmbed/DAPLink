/**
 * @file power.c
 * @brief
 */
#include "fsl_smc.h"
#include "fsl_rcm.h"
#include "fsl_llwu.h"
#include "fsl_pmc.h"
#include "fsl_port.h"
#include "fsl_clock.h"

#include "power.h"
#include "IO_Config.h"
#include "uart.h"
#include "rl_usb.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void power_set_wakeup_config(app_power_mode_t targetMode);
static void power_mode_switch(smc_power_state_t curPowerState, app_power_mode_t targetPowerMode);
static void power_pre_switch_hook(smc_power_state_t originPowerState, app_power_mode_t targetMode);
static void power_post_switch_hook(smc_power_state_t originPowerState, app_power_mode_t targetMode);
static void power_enter_mode(app_power_mode_t targetPowerMode);

/*******************************************************************************
 * Code
 ******************************************************************************/
void LLWU_IRQHandler(void)
{
    /* If wakeup by external pin BTN_NOT_PRESSED. */
    if (LLWU_GetExternalWakeupPinFlag(LLWU, PIN_SW_RESET_LLWU_PIN))
    {
        PORT_SetPinInterruptConfig(PIN_SW_RESET_PORT, PIN_SW_RESET_BIT, kPORT_InterruptOrDMADisabled);
        PORT_ClearPinsInterruptFlags(PIN_SW_RESET_PORT, (1U << PIN_SW_RESET_BIT));
        LLWU_ClearExternalWakeupPinFlag(LLWU, PIN_SW_RESET_LLWU_PIN);
    }
    /* If wakeup by external pin WAKE_ON_EDGE. */
    if (LLWU_GetExternalWakeupPinFlag(LLWU, PIN_WAKE_ON_EDGE_LLWU_PIN))
    {
        PORT_ClearPinsInterruptFlags(PIN_WAKE_ON_EDGE_PORT, (1U << PIN_WAKE_ON_EDGE_BIT));
        LLWU_ClearExternalWakeupPinFlag(LLWU, PIN_WAKE_ON_EDGE_LLWU_PIN);
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

        /* Reset USB on cable detach (VBUS falling edge) */
        USBD_Reset();
        usbd_reset_core();
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
        NVIC_ClearPendingIRQ(LLWU_IRQn);
    }

    /* Enable rising edge interrupt on WAKE_ON_EDGE pin (VBUS falling edge) to detect USB detach */
    PORT_SetPinInterruptConfig(PIN_WAKE_ON_EDGE_PORT, PIN_WAKE_ON_EDGE_BIT, kPORT_InterruptRisingEdge);

    NVIC_EnableIRQ(LLWU_IRQn);
    NVIC_EnableIRQ(PORTC_PORTD_IRQn);
}

void power_enter_VLLS0()
{
    power_enter_mode(kAPP_PowerModeVlls0);
}

void power_enter_VLPS()
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
    PORT_SetPinInterruptConfig(PIN_WAKE_ON_EDGE_PORT, PIN_WAKE_ON_EDGE_BIT, PIN_WAKE_ON_EDGE_PORT_WAKEUP_TYPE);

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
    /* Wait for debug console output finished. */
    while (!(LPUART_STAT_TC_MASK & UART->STAT))
    {
    }
    uart_uninitialize();

    /*
     * Set pin for current leakage.
     * UART RX pin: Set to pinmux to disable.
     * UART TX pin: Don't need to change.
     */
    PORT_SetPinMux(UART_PORT, PIN_UART_RX_BIT, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(PIN_RED_LED_PORT, PIN_RED_LED_BIT, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(PIN_HID_LED_PORT, PIN_HID_LED_BIT, kPORT_PinDisabledOrAnalog);
}

static void power_post_switch_hook(smc_power_state_t originPowerState, app_power_mode_t targetMode)
{
    /*
     * UART RX pin is set to disable for current leakage, need to re-configure pinmux.
     * UART TX pin: Don't need to change.
     */
    PORT_SetPinMux(UART_PORT, PIN_UART_RX_BIT, (port_mux_t)PIN_UART_RX_MUX_ALT);
    PORT_SetPinMux(PIN_RED_LED_PORT, PIN_RED_LED_BIT, kPORT_MuxAlt3);
    PORT_SetPinMux(PIN_HID_LED_PORT, PIN_HID_LED_BIT, kPORT_PinDisabledOrAnalog);

    uart_initialize();

    /* Change to rising edge interrupt on WAKE_ON_EDGE pin (VBUS falling edge) to detect USB detach */
    PORT_SetPinInterruptConfig(PIN_WAKE_ON_EDGE_PORT, PIN_WAKE_ON_EDGE_BIT, kPORT_InterruptRisingEdge);
}
