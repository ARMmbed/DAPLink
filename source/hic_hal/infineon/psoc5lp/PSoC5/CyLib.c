/***************************************************************************//**
* \file CyLib.c
* \version 5.70
*
* \brief Provides a system API for the clocking, interrupts and watchdog timer.
*
********************************************************************************
* Copyright (2019-2021) Cypress Semiconductor Corporation (an Infineon company)
* or an affiliate of Cypress Semiconductor Corporation.
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
*****************************************************************************/

#include "CyLib.h"


/*******************************************************************************
* The CyResetStatus variable is used to obtain value of RESET_SR0 register after
* a device reset. It is set from initialize_psoc() at the early initialization
* stage. In case of IAR EW IDE, initialize_psoc() is executed before the data
* sections are initialized. To avoid zeroing, CyResetStatus should be placed
* to the .noinit section.
*******************************************************************************/
CY_NOINIT uint8 CYXDATA CyResetStatus;


/* Variable Vdda */
#if(CYDEV_VARIABLE_VDDA == 1)

    uint8 CyScPumpEnabled = (uint8)(CYDEV_VDDA_MV < 2700);

#endif  /* (CYDEV_VARIABLE_VDDA == 1) */


/* Do not use these definitions directly in your application */
uint32 cydelay_freq_hz  = BCLK__BUS_CLK__HZ;
uint32 cydelay_freq_khz = (BCLK__BUS_CLK__HZ + 999u) / 1000u;
uint8  cydelay_freq_mhz = (uint8)((BCLK__BUS_CLK__HZ + 999999u) / 1000000u);
uint32 cydelay_32k_ms   = 32768u * ((BCLK__BUS_CLK__HZ + 999u) / 1000u);


/* Function Prototypes */
static uint8 CyUSB_PowerOnCheck(void)  ;
static void CyIMO_SetTrimValue(uint8 freq) ;
static void CyBusClk_Internal_SetDivider(uint16 divider);

#if(CY_PSOC5)
    static cySysTickCallback CySysTickCallbacks[CY_SYS_SYST_NUM_OF_CALLBACKS];
    static void CySysTickServiceCallbacks(void);
    uint32 CySysTickInitVar = 0u;
#endif  /* (CY_PSOC5) */


#if(CY_PSOC3)
    CY_ISR_PROTO(IntDefaultHandler);
#endif /* (CY_PSOC3) */


/*******************************************************************************
* Function Name: CyPLL_OUT_Start
****************************************************************************//**
*
*   Enables the PLL.  Optionally waits for it to become stable.
*   Waits at least 250 us or until it is detected that the PLL is stable.
*
*   \param wait:
*    \param 0: Return immediately after configuration
*    \param 1: Wait for PLL lock or timeout.
*
* \return
*   Status
*    CYRET_SUCCESS - Completed successfully
*    CYRET_TIMEOUT - Timeout occurred without detecting a stable clock.
*     If the input source of the clock is jittery, then the lock indication
*     may not occur.  However, after the timeout has expired the generated PLL
*     clock can still be used.
*
* \sideeffect
*  If wait is enabled: This function uses the Fast Time Wheel to time the wait.
*  Any other use of the Fast Time Wheel will be stopped during the period of
*  this function and then restored. This function also uses the 100 KHz ILO.
*  If not enabled, this function will enable the 100 KHz ILO for the period of
*  this function.
*
*  No changes to the setup of the ILO, Fast Time Wheel, Central Time Wheel or
*  Once Per Second interrupt may be made by interrupt routines during the period
*  of this function execution. The current operation of the ILO, Central Time
*  Wheel and Once Per Second interrupt are maintained during the operation of
*  this function provided the reading of the Power Manager Interrupt Status
*  Register is only done using the CyPmReadStatus() function.
*
*******************************************************************************/
cystatus CyPLL_OUT_Start(uint8 wait)
{
    cystatus status = CYRET_SUCCESS;

    uint8 iloEnableState;
    uint8 pmTwCfg0State;
    uint8 pmTwCfg2State;


    /* Enables PLL circuit  */
    CY_CLK_PLL_CFG0_REG |= CY_CLK_PLL_ENABLE;

    if(wait != 0u)
    {
        /* Save 100 KHz ILO, FTW interval, enable and interrupt enable */
        iloEnableState = CY_LIB_SLOWCLK_ILO_CR0_REG & CY_LIB_SLOWCLK_ILO_CR0_EN_100KHZ;
        pmTwCfg0State = CY_LIB_PM_TW_CFG0_REG;
        pmTwCfg2State = CY_LIB_PM_TW_CFG2_REG;

        CyPmFtwSetInterval(CY_CLK_PLL_FTW_INTERVAL);

        status = CYRET_TIMEOUT;

        while(0u == (CY_PM_FTW_INT & CyPmReadStatus(CY_PM_FTW_INT)))
        {
            /* Wait for interrupt status */
            if(0u != (CY_CLK_PLL_SR_REG & CY_CLK_PLL_LOCK_STATUS))
            {
                if(0u != (CY_CLK_PLL_SR_REG & CY_CLK_PLL_LOCK_STATUS))
                {
                    status = CYRET_SUCCESS;
                    break;
                }
            }
        }

        /* Restore 100 KHz ILO, FTW interval, enable and interrupt enable */
        if(0u == iloEnableState)
        {
            CyILO_Stop100K();
        }

        CY_LIB_PM_TW_CFG0_REG = pmTwCfg0State;
        CY_LIB_PM_TW_CFG2_REG = pmTwCfg2State;
    }

    return(status);
}


/*******************************************************************************
* Function Name: CyPLL_OUT_Stop
****************************************************************************//**
*
*  Disables the PLL.
*
*******************************************************************************/
void CyPLL_OUT_Stop(void)
{
    CY_CLK_PLL_CFG0_REG &= ((uint8)(~CY_CLK_PLL_ENABLE));
}


/*******************************************************************************
* Function Name: CyPLL_OUT_SetPQ
****************************************************************************//**
*
*  Sets the P and Q dividers and the charge pump current.
*  The Frequency Out will be P/Q * Frequency In.
*  The PLL must be disabled before calling this function.
*
*  \param uint8 pDiv:
*   Valid range [8 - 255].
*
*  \param uint8 qDiv:
*   Valid range [1 - 16]. Input Frequency / Q must be in range of 1 to 3 MHz.

*  \param uint8 current:
*   Valid range [1 - 7]. Charge pump current in uA. Refer to the device TRM and
*   datasheet for more information.
*
* \sideeffect
*  If this function execution results in the CPU clock frequency increasing,
*  then the number of clock cycles the cache will wait before it samples data
*  coming back from the Flash must be adjusted by calling
*  CyFlash_SetWaitCycles() with an appropriate parameter. It can be optionally
*  called if the CPU clock frequency is lowered in order to improve the CPU
*  performance. See CyFlash_SetWaitCycles() description for more information.
*
*******************************************************************************/
void CyPLL_OUT_SetPQ(uint8 pDiv, uint8 qDiv, uint8 current)
{
    /* Halt CPU in debug mode if PLL is enabled */
    CYASSERT(0u == (CY_CLK_PLL_CFG0_REG & CY_CLK_PLL_ENABLE));

    if((pDiv    >= CY_CLK_PLL_MIN_P_VALUE  ) &&
       (qDiv    <= CY_CLK_PLL_MAX_Q_VALUE  ) && (qDiv    >= CY_CLK_PLL_MIN_Q_VALUE  ) &&
       (current >= CY_CLK_PLL_MIN_CUR_VALUE) && (current <= CY_CLK_PLL_MAX_CUR_VALUE))
    {
        /* Set new values */
        CY_CLK_PLL_P_REG = pDiv;
        CY_CLK_PLL_Q_REG = ((uint8)(qDiv - 1u));
        CY_CLK_PLL_CFG1_REG = (CY_CLK_PLL_CFG1_REG & CY_CLK_PLL_CURRENT_MASK) |
                                ((uint8)(((uint8)(current - 1u)) << CY_CLK_PLL_CURRENT_POSITION));
    }
    else
    {
        /***********************************************************************
        * Halt CPU in debug mode if:
        * - P divider is less than required
        * - Q divider is out of range
        * - pump current is out of range
        ***********************************************************************/
        CYASSERT(0u != 0u);
    }

}


/*******************************************************************************
* Function Name: CyPLL_OUT_SetSource
****************************************************************************//**
*
*  Sets the input clock source to the PLL. The PLL must be disabled before
*  calling this function.
*
*   \param source: One of the three available PLL clock sources
*    \param CY_PLL_SOURCE_IMO  :   IMO
*    \param CY_PLL_SOURCE_XTAL :   MHz Crystal
*    \param CY_PLL_SOURCE_DSI  :   DSI
*
* \sideeffect
*  If this function execution results in the CPU clock frequency increasing,
*  then the number of clock cycles the cache will wait before it samples data
*  coming back from the3 Flash must be adjusted by calling
*  CyFlash_SetWaitCycles() with an appropriate parameter. It can be optionally
*  called if the CPU clock frequency is lowered in order to improve the CPU
*  performance. See CyFlash_SetWaitCycles() description for more information.
*
*******************************************************************************/
void CyPLL_OUT_SetSource(uint8 source)
{
    /* Halt CPU in debug mode if PLL is enabled */
    CYASSERT(0u == (CY_CLK_PLL_CFG0_REG & CY_CLK_PLL_ENABLE));

    switch(source)
    {
        case CY_PLL_SOURCE_IMO:
        case CY_PLL_SOURCE_XTAL:
        case CY_PLL_SOURCE_DSI:
            CY_LIB_CLKDIST_CR_REG = ((CY_LIB_CLKDIST_CR_REG & CY_LIB_CLKDIST_CR_PLL_SCR_MASK) | source);
        break;

        default:
            CYASSERT(0u != 0u);
        break;
    }
}


/*******************************************************************************
* Function Name: CyIMO_Start
****************************************************************************//**
*
*  Enables the IMO. Optionally waits at least 6 us for it to settle.
*
*  \param uint8 wait:
*   \param 0: Return immediately after configuration
*   \param 1: Wait for at least 6 us for the IMO to settle.
*
* \sideeffect
*  If wait is enabled: This function uses the Fast Time Wheel to time the wait.
*  Any other use of the Fast Time Wheel will be stopped during the period of
*  this function and then restored. This function also uses the 100 KHz ILO.
*  If not enabled, this function will enable the 100 KHz ILO for the period of
*  this function.
*
*  No changes to the setup of the ILO, Fast Time Wheel, Central Time Wheel or
*  Once Per Second interrupt may be made by interrupt routines during the period
*  of this function execution. The current operation of the ILO, Central Time
*  Wheel and Once Per Second interrupt are maintained during the operation of
*  this function provided the reading of the Power Manager Interrupt Status
*  Register is only done using the CyPmReadStatus() function.
*
*******************************************************************************/
void CyIMO_Start(uint8 wait)
{
    uint8 pmFtwCfg2Reg;
    uint8 pmFtwCfg0Reg;
    uint8 ilo100KhzEnable;


    CY_LIB_PM_ACT_CFG0_REG  |= CY_LIB_PM_ACT_CFG0_IMO_EN;
    CY_LIB_PM_STBY_CFG0_REG |= CY_LIB_PM_STBY_CFG0_IMO_EN;

    if(0u != wait)
    {
        /* Need to turn on 100KHz ILO if it happens to not already be running.*/
        ilo100KhzEnable = CY_LIB_SLOWCLK_ILO_CR0_REG & CY_LIB_SLOWCLK_ILO_CR0_EN_100KHZ;
        pmFtwCfg0Reg = CY_LIB_PM_TW_CFG0_REG;
        pmFtwCfg2Reg = CY_LIB_PM_TW_CFG2_REG;

        CyPmFtwSetInterval(CY_LIB_CLK_IMO_FTW_TIMEOUT);

        while (0u == (CY_PM_FTW_INT & CyPmReadStatus(CY_PM_FTW_INT)))
        {
            /* Wait for interrupt status */
        }

        if(0u == ilo100KhzEnable)
        {
            CyILO_Stop100K();
        }

        CY_LIB_PM_TW_CFG0_REG = pmFtwCfg0Reg;
        CY_LIB_PM_TW_CFG2_REG = pmFtwCfg2Reg;
    }
}


/*******************************************************************************
* Function Name: CyIMO_Stop
****************************************************************************//**
*
*   Disables the IMO.
*
*******************************************************************************/
void CyIMO_Stop(void)
{
    CY_LIB_PM_ACT_CFG0_REG  &= ((uint8) (~CY_LIB_PM_ACT_CFG0_IMO_EN));
    CY_LIB_PM_STBY_CFG0_REG &= ((uint8) (~CY_LIB_PM_STBY_CFG0_IMO_EN));
}


/*******************************************************************************
* Function Name: CyUSB_PowerOnCheck
****************************************************************************//**
*
*  Returns the USB power status value. A private function to cy_boot.
*
* \return
*   uint8: one if the USB is enabled, 0 if not enabled.
*
*******************************************************************************/
static uint8 CyUSB_PowerOnCheck(void)
{
    uint8 poweredOn = 0u;

    /* Check whether device is in Active or AltActive and if USB is powered on */
    if((((CY_PM_MODE_CSR_REG & CY_PM_MODE_CSR_MASK) == CY_PM_MODE_CSR_ACTIVE ) &&
       (0u != (CY_LIB_PM_ACT_CFG5_REG & CY_ACT_USB_ENABLED     )))  ||
       (((CY_PM_MODE_CSR_REG & CY_PM_MODE_CSR_MASK) == CY_PM_MODE_CSR_ALT_ACT) &&
       (0u != (CY_LIB_PM_STBY_CFG5_REG & CY_ALT_ACT_USB_ENABLED))))
    {
        poweredOn = 1u;
    }

    return (poweredOn);
}


/*******************************************************************************
* Function Name: CyIMO_SetTrimValue
****************************************************************************//**
*
*  Sets the IMO factory trim values.
*
*  uint8 freq - frequency for which trims must be set
*
*******************************************************************************/
static void CyIMO_SetTrimValue(uint8 freq)
{
    uint8 usbPowerOn = CyUSB_PowerOnCheck();

    /* If USB is powered */
    if(usbPowerOn == 1u)
    {
        /* Unlock USB write */
        CY_LIB_USB_CR1_REG &= ((uint8)(~CY_LIB_USB_CLK_EN));
    }
    switch(freq)
    {
    case CY_IMO_FREQ_3MHZ:
        CY_LIB_IMO_TR1_REG = CY_GET_XTND_REG8(CY_LIB_TRIM_IMO_3MHZ_PTR);
        break;

    case CY_IMO_FREQ_6MHZ:
        CY_LIB_IMO_TR1_REG = CY_GET_XTND_REG8(CY_LIB_TRIM_IMO_6MHZ_PTR);
        break;

    case CY_IMO_FREQ_12MHZ:
        CY_LIB_IMO_TR1_REG = CY_GET_XTND_REG8(CY_LIB_TRIM_IMO_12MHZ_PTR);
        break;

    case CY_IMO_FREQ_24MHZ:
        CY_LIB_IMO_TR1_REG = CY_GET_XTND_REG8(CY_LIB_TRIM_IMO_24MHZ_PTR);
        break;

    case CY_IMO_FREQ_48MHZ:
        CY_LIB_IMO_TR1_REG = CY_GET_XTND_REG8(CY_LIB_TRIM_IMO_TR1_PTR);
        break;

    case CY_IMO_FREQ_62MHZ:
        CY_LIB_IMO_TR1_REG = CY_GET_XTND_REG8(CY_LIB_TRIM_IMO_67MHZ_PTR);
        break;

#if(CY_PSOC5)
    case CY_IMO_FREQ_74MHZ:
        CY_LIB_IMO_TR1_REG = CY_GET_XTND_REG8(CY_LIB_TRIM_IMO_80MHZ_PTR);
        break;
#endif  /* (CY_PSOC5) */

    case CY_IMO_FREQ_USB:
        CY_LIB_IMO_TR1_REG = CY_GET_XTND_REG8(CY_LIB_TRIM_IMO_USB_PTR);

        /* If USB is powered */
        if(usbPowerOn == 1u)
        {
            /* Lock USB Oscillator */
            CY_LIB_USB_CR1_REG |= CY_LIB_USB_CLK_EN;
        }
        break;

    default:
            CYASSERT(0u != 0u);
        break;
    }

}


/*******************************************************************************
* Function Name: CyIMO_SetFreq
****************************************************************************//**
*
*  Sets the frequency of the IMO. Changes may be made while the IMO is running.
*
*  \param freq: Frequency of IMO operation
*       CY_IMO_FREQ_3MHZ  to set  3   MHz
*       CY_IMO_FREQ_6MHZ  to set  6   MHz
*       CY_IMO_FREQ_12MHZ to set 12   MHz
*       CY_IMO_FREQ_24MHZ to set 24   MHz
*       CY_IMO_FREQ_48MHZ to set 48   MHz
*       CY_IMO_FREQ_62MHZ to set 62.6 MHz
*       CY_IMO_FREQ_74MHZ to set 74.7 MHz (not applicable for PSoC 3)
*       CY_IMO_FREQ_USB   to set 24   MHz (Trimmed for USB operation)
*
* \sideeffect
*  If this function execution results in the CPU clock frequency increasing,
*  then the number of clock cycles the cache will wait before it samples data
*  coming back from the Flash must be adjusted by calling
*  CyFlash_SetWaitCycles() with an appropriate parameter. It can be optionally
*  called if the CPU clock frequency is lowered in order to improve the CPU
*  performance. See CyFlash_SetWaitCycles() description for more information.
*
*  When the USB setting is chosen, the USB clock locking circuit is enabled.
*  Otherwise this circuit is disabled. The USB block must be powered before
*  selecting the USB setting.
*
*******************************************************************************/
void CyIMO_SetFreq(uint8 freq)
{
    uint8 currentFreq;
    uint8 nextFreq;

    /***************************************************************************
    * If the IMO frequency is changed,the Trim values must also be set
    * accordingly.This requires reading the current frequency. If the new
    * frequency is faster, then set a new trim and then change the frequency,
    * otherwise change the frequency and then set new trim values.
    ***************************************************************************/

    currentFreq = CY_LIB_FASTCLK_IMO_CR_REG & ((uint8)(~CY_LIB_FASTCLK_IMO_CR_RANGE_MASK));

    /* Check if requested frequency is USB. */
    nextFreq = (freq == CY_IMO_FREQ_USB) ? CY_IMO_FREQ_24MHZ : freq;

    switch (currentFreq)
    {
    case 0u:
        currentFreq = CY_IMO_FREQ_12MHZ;
        break;

    case 1u:
        currentFreq = CY_IMO_FREQ_6MHZ;
        break;

    case 2u:
        currentFreq = CY_IMO_FREQ_24MHZ;
        break;

    case 3u:
        currentFreq = CY_IMO_FREQ_3MHZ;
        break;

    case 4u:
        currentFreq = CY_IMO_FREQ_48MHZ;
        break;

    case 5u:
        currentFreq = CY_IMO_FREQ_62MHZ;
        break;

#if(CY_PSOC5)
    case 6u:
        currentFreq = CY_IMO_FREQ_74MHZ;
        break;
#endif  /* (CY_PSOC5) */

    default:
        CYASSERT(0u != 0u);
        break;
    }

    if (nextFreq >= currentFreq)
    {
        /* Set new trim first */
        CyIMO_SetTrimValue(freq);
    }

    /* Set usbclk_on bit when using CY_IMO_FREQ_USB, if not clear it */
    switch(freq)
    {
    case CY_IMO_FREQ_3MHZ:
        CY_LIB_FASTCLK_IMO_CR_REG = ((CY_LIB_FASTCLK_IMO_CR_REG & CY_LIB_FASTCLK_IMO_CR_RANGE_MASK) |
            CY_LIB_IMO_3MHZ_VALUE) & ((uint8)(~CY_LIB_IMO_USBCLK_ON_SET));
        break;

    case CY_IMO_FREQ_6MHZ:
        CY_LIB_FASTCLK_IMO_CR_REG = ((CY_LIB_FASTCLK_IMO_CR_REG & CY_LIB_FASTCLK_IMO_CR_RANGE_MASK) |
            CY_LIB_IMO_6MHZ_VALUE) & ((uint8)(~CY_LIB_IMO_USBCLK_ON_SET));
        break;

    case CY_IMO_FREQ_12MHZ:
        CY_LIB_FASTCLK_IMO_CR_REG = ((CY_LIB_FASTCLK_IMO_CR_REG & CY_LIB_FASTCLK_IMO_CR_RANGE_MASK) |
            CY_LIB_IMO_12MHZ_VALUE) & ((uint8)(~CY_LIB_IMO_USBCLK_ON_SET));
        break;

    case CY_IMO_FREQ_24MHZ:
        CY_LIB_FASTCLK_IMO_CR_REG = ((CY_LIB_FASTCLK_IMO_CR_REG & CY_LIB_FASTCLK_IMO_CR_RANGE_MASK) |
            CY_LIB_IMO_24MHZ_VALUE) & ((uint8)(~CY_LIB_IMO_USBCLK_ON_SET));
        break;

    case CY_IMO_FREQ_48MHZ:
        CY_LIB_FASTCLK_IMO_CR_REG = ((CY_LIB_FASTCLK_IMO_CR_REG & CY_LIB_FASTCLK_IMO_CR_RANGE_MASK) |
            CY_LIB_IMO_48MHZ_VALUE) & ((uint8)(~CY_LIB_IMO_USBCLK_ON_SET));
        break;

    case CY_IMO_FREQ_62MHZ:
        CY_LIB_FASTCLK_IMO_CR_REG = ((CY_LIB_FASTCLK_IMO_CR_REG & CY_LIB_FASTCLK_IMO_CR_RANGE_MASK) |
            CY_LIB_IMO_62MHZ_VALUE) & ((uint8)(~CY_LIB_IMO_USBCLK_ON_SET));
        break;

#if(CY_PSOC5)
    case CY_IMO_FREQ_74MHZ:
        CY_LIB_FASTCLK_IMO_CR_REG = ((CY_LIB_FASTCLK_IMO_CR_REG & CY_LIB_FASTCLK_IMO_CR_RANGE_MASK) |
            CY_LIB_IMO_74MHZ_VALUE) & ((uint8)(~CY_LIB_IMO_USBCLK_ON_SET));
        break;
#endif  /* (CY_PSOC5) */

    case CY_IMO_FREQ_USB:
        CY_LIB_FASTCLK_IMO_CR_REG = ((CY_LIB_FASTCLK_IMO_CR_REG & CY_LIB_FASTCLK_IMO_CR_RANGE_MASK) |
            CY_LIB_IMO_24MHZ_VALUE) | CY_LIB_IMO_USBCLK_ON_SET;
        break;

    default:
        CYASSERT(0u != 0u);
        break;
    }

    /* Tu rn onIMO Doubler, if switching to CY_IMO_FREQ_USB */
    if (freq == CY_IMO_FREQ_USB)
    {
        CyIMO_EnableDoubler();
    }
    else
    {
        CyIMO_DisableDoubler();
    }

    if (nextFreq < currentFreq)
    {
        /* Set the trim after setting frequency */
        CyIMO_SetTrimValue(freq);
    }
}


/*******************************************************************************
* Function Name: CyIMO_SetSource
****************************************************************************//**
*
*  Sets the source of the clock output from the IMO block.
*
*  The output from the IMO is by default the IMO itself. Optionally the MHz
*  Crystal or DSI input can be the source of the IMO output instead.
*
*   \param source: CY_IMO_SOURCE_DSI to set the DSI as source.
*           CY_IMO_SOURCE_XTAL to set the MHz as source.
*           CY_IMO_SOURCE_IMO to set the IMO itself.
*
* \sideeffect
*  If this function execution resulted in the CPU clock frequency increasing,
*  then the number of clock cycles the cache will wait before it samples data
*  coming back from the Flash must be adjusted by calling
*  CyFlash_SetWaitCycles() with an appropriate parameter. It can be optionally
*  called if the CPU clock frequency is lowered in order to improve the CPU
*  performance. See CyFlash_SetWaitCycles() description for more information.
*
*******************************************************************************/
void CyIMO_SetSource(uint8 source)
{
    switch(source)
    {
    case CY_IMO_SOURCE_DSI:
        CY_LIB_CLKDIST_CR_REG     &= ((uint8)(~CY_LIB_CLKDIST_CR_IMO2X));
        CY_LIB_FASTCLK_IMO_CR_REG |= CY_LIB_FASTCLK_IMO_IMO;
        break;

    case CY_IMO_SOURCE_XTAL:
        CY_LIB_CLKDIST_CR_REG     |= CY_LIB_CLKDIST_CR_IMO2X;
        CY_LIB_FASTCLK_IMO_CR_REG |= CY_LIB_FASTCLK_IMO_IMO;
        break;

    case CY_IMO_SOURCE_IMO:
        CY_LIB_FASTCLK_IMO_CR_REG &= ((uint8)(~CY_LIB_FASTCLK_IMO_IMO));
        break;

    default:
        /* Incorrect source value */
        CYASSERT(0u != 0u);
        break;
    }
}


/*******************************************************************************
* Function Name: CyIMO_EnableDoubler
****************************************************************************//**
*
*  Enables the IMO doubler.  The 2x frequency clock is used to convert a 24 MHz
*  input to a 48 MHz output for use by the USB block.
*
*******************************************************************************/
void CyIMO_EnableDoubler(void)
{
    /* Set FASTCLK_IMO_CR_PTR regigster's 4th bit */
    CY_LIB_FASTCLK_IMO_CR_REG |= CY_LIB_FASTCLK_IMO_DOUBLER;
}


/*******************************************************************************
* Function Name: CyIMO_DisableDoubler
****************************************************************************//**
*
*   Disables the IMO doubler.
*
*******************************************************************************/
void CyIMO_DisableDoubler(void)
{
    CY_LIB_FASTCLK_IMO_CR_REG &= ((uint8)(~CY_LIB_FASTCLK_IMO_DOUBLER));
}


/*******************************************************************************
* Function Name: CyMasterClk_SetSource
****************************************************************************//**
*
*  Sets the source of the master clock.
*
*   \param source: One of the four available Master clock sources.
*     CY_MASTER_SOURCE_IMO
*     CY_MASTER_SOURCE_PLL
*     CY_MASTER_SOURCE_XTAL
*     CY_MASTER_SOURCE_DSI
*
* \sideeffect
*  The current source and the new source must both be running and stable before
*  calling this function.
*
*  If this function execution resulted in the CPU clock frequency increasing,
*  then the number of clock cycles the cache will wait before it samples data
*  coming back from the Flash must be adjusted by calling
*  CyFlash_SetWaitCycles() with an appropriate parameter. It can be optionally
*  called if the CPU clock frequency is lowered in order to improve the CPU
*  performance. See CyFlash_SetWaitCycles() description for more information.
*
*******************************************************************************/
void CyMasterClk_SetSource(uint8 source)
{
    CY_LIB_CLKDIST_MSTR1_REG = (CY_LIB_CLKDIST_MSTR1_REG & MASTER_CLK_SRC_CLEAR) |
                                (source & ((uint8)(~MASTER_CLK_SRC_CLEAR)));
}


/*******************************************************************************
* Function Name: CyMasterClk_SetDivider
****************************************************************************//**
*
*  Sets the divider value used to generate Master Clock.
*
*  \param uint8 divider:
*   The valid range is [0-255]. The clock will be divided by this value + 1.
*   For example to divide this parameter by two should be set to 1.
*
* \sideeffect
*  If this function execution resulted in the CPU clock frequency increasing,
*  then the number of clock cycles the cache will wait before it samples data
*  coming back from the Flash must be adjusted by calling
*  CyFlash_SetWaitCycles() with an appropriate parameter. It can be optionally
*  called if the CPU clock frequency is lowered in order to improve the CPU
*  performance. See CyFlash_SetWaitCycles() description for more information.
*
*  When changing the Master or Bus clock divider value from div-by-n to div-by-1
*  the first clock cycle output after the div-by-1 can be up to 4 ns shorter
*  than the final/expected div-by-1 period.
*
*******************************************************************************/
void CyMasterClk_SetDivider(uint8 divider)
{
    CY_LIB_CLKDIST_MSTR0_REG = divider;
}


/*******************************************************************************
* Function Name: CyBusClk_Internal_SetDivider
****************************************************************************//**
*
*  The function used by CyBusClk_SetDivider(). For internal use only.
*
*   \param divider: Valid range [0-65535].
*   The clock will be divided by this value + 1.
*   For example, to divide this parameter by two should be set to 1.
*
*******************************************************************************/
static void CyBusClk_Internal_SetDivider(uint16 divider)
{
    /* Mask bits to enable shadow loads  */
    CY_LIB_CLKDIST_AMASK_REG &= CY_LIB_CLKDIST_AMASK_MASK;
    CY_LIB_CLKDIST_DMASK_REG  = CY_LIB_CLKDIST_DMASK_MASK;

    /* Enable mask bits to enable shadow loads */
    CY_LIB_CLKDIST_BCFG2_REG |= CY_LIB_CLKDIST_BCFG2_MASK;

    /* Update Shadow Divider Value Register with new divider */
    CY_LIB_CLKDIST_WRK_LSB_REG = LO8(divider);
    CY_LIB_CLKDIST_WRK_MSB_REG = HI8(divider);


    /***************************************************************************
    * Copy shadow value defined in Shadow Divider Value Register
    * (CY_LIB_CLKDIST_WRK_LSB_REG and CY_LIB_CLKDIST_WRK_MSB_REG) to all
    * dividers selected in Analog and Digital Clock Mask Registers
    * (CY_LIB_CLKDIST_AMASK_REG and CY_LIB_CLKDIST_DMASK_REG).
    ***************************************************************************/
    CY_LIB_CLKDIST_LD_REG |= CY_LIB_CLKDIST_LD_LOAD;
}


/*******************************************************************************
* Function Name: CyBusClk_SetDivider
****************************************************************************//**
*
*  Sets the divider value used to generate the Bus Clock.
*
*  \param divider: Valid range [0-65535]. The clock will be divided by this value + 1.
*  For example, to divide this parameter by two should be set to 1.
*
* \sideeffect
*  If this function execution resulted in the CPU clock frequency increasing,
*  then the number of clock cycles the cache will wait before it samples data
*  coming back from the Flash must be adjusted by calling
*  CyFlash_SetWaitCycles() with an appropriate parameter. It can be optionally
*  called if the CPU clock frequency is lowered in order to improve the CPU
*  performance. See CyFlash_SetWaitCycles() description for more information.
*
*******************************************************************************/
void CyBusClk_SetDivider(uint16 divider)
{
    uint8  masterClkDiv;
    uint16 busClkDiv;
    uint8 interruptState;

    interruptState = CyEnterCriticalSection();

    /* Work around to set bus clock divider value */
    busClkDiv = (uint16)((uint16)CY_LIB_CLKDIST_BCFG_MSB_REG << 8u);
    busClkDiv |= CY_LIB_CLKDIST_BCFG_LSB_REG;

    if ((divider == 0u) || (busClkDiv == 0u))
    {
        /* Save away master clock divider value */
        masterClkDiv = CY_LIB_CLKDIST_MSTR0_REG;

        if (masterClkDiv < CY_LIB_CLKDIST_MASTERCLK_DIV)
        {
            /* Set master clock divider to 7 */
            CyMasterClk_SetDivider(CY_LIB_CLKDIST_MASTERCLK_DIV);
        }

        if (divider == 0u)
        {
            /* Set SSS bit and divider register desired value */
            CY_LIB_CLKDIST_BCFG2_REG |= CY_LIB_CLKDIST_BCFG2_SSS;
            CyBusClk_Internal_SetDivider(divider);
        }
        else
        {
            CyBusClk_Internal_SetDivider(divider);
            CY_LIB_CLKDIST_BCFG2_REG &= ((uint8)(~CY_LIB_CLKDIST_BCFG2_SSS));
        }

        /* Restore master clock */
        CyMasterClk_SetDivider(masterClkDiv);
    }
    else
    {
        CyBusClk_Internal_SetDivider(divider);
    }

    CyExitCriticalSection(interruptState);
}


#if(CY_PSOC3)

    /*******************************************************************************
    * Function Name: CyCpuClk_SetDivider
    ****************************************************************************//**
    *
    *  Sets the divider value used to generate the CPU Clock. Only applicable for
    *  PSoC 3 parts.
    *
    *  \param divider: Valid range [0-15]. The clock will be divided by this value + 1.
    *  For example, to divide this parameter by two should be set to 1.
    *
    * \sideeffect
    *  If this function execution resulted in the CPU clock frequency increasing,
    *  then the number of clock cycles the cache will wait before it samples data
    *  coming back from the Flash must be adjusted by calling
    *  CyFlash_SetWaitCycles() with an appropriate parameter. It can be optionally
    *  called if the CPU clock frequency is lowered in order to improve the CPU
    *  performance. See CyFlash_SetWaitCycles() description for more information.
    *
    *******************************************************************************/
    void CyCpuClk_SetDivider(uint8 divider)
    {
            CY_LIB_CLKDIST_MSTR1_REG = (CY_LIB_CLKDIST_MSTR1_REG & CY_LIB_CLKDIST_MSTR1_DIV_MASK) |
                                ((uint8)(divider << CY_LIB_CLKDIST_DIV_POSITION));
    }

#endif /* (CY_PSOC3) */


/*******************************************************************************
* Function Name: CyUsbClk_SetSource
****************************************************************************//**
*
*  Sets the source of the USB clock.
*
*  \param source: One of the four available USB clock sources
*    CY_LIB_USB_CLK_IMO2X     - IMO 2x
*    CY_LIB_USB_CLK_IMO       - IMO
*    CY_LIB_USB_CLK_PLL       - PLL
*    CY_LIB_USB_CLK_DSI       - DSI
*
*******************************************************************************/
void CyUsbClk_SetSource(uint8 source)
{
    CY_LIB_CLKDIST_UCFG_REG = (CY_LIB_CLKDIST_UCFG_REG & ((uint8)(~CY_LIB_CLKDIST_UCFG_SRC_SEL_MASK))) |
                        (CY_LIB_CLKDIST_UCFG_SRC_SEL_MASK & source);
}


/*******************************************************************************
* Function Name: CyILO_Start1K
****************************************************************************//**
*
*  Enables the ILO 1 KHz oscillator.
*
*  Note The ILO 1 KHz oscillator is always enabled by default, regardless of the
*  selection in the Clock Editor. Therefore, this API is only needed if the
*  oscillator was turned off manually.
*
*******************************************************************************/
void CyILO_Start1K(void)
{
    /* Set bit 1 of ILO RS */
    CY_LIB_SLOWCLK_ILO_CR0_REG |= CY_LIB_SLOWCLK_ILO_CR0_EN_1KHZ;
}


/*******************************************************************************
* Function Name: CyILO_Stop1K
****************************************************************************//**
*
*  Disables the ILO 1 KHz oscillator.
*
*  Note The ILO 1 KHz oscillator must be enabled if the Sleep or Hibernate low
*  power mode APIs are expected to be used. For more information, refer to the
*  Power Management section of this document.
*
* \sideeffect
*  PSoC5: Stopping the ILO 1 kHz could break the active WDT functionality.
*
*******************************************************************************/
void CyILO_Stop1K(void)
{
    /* Clear bit 1 of ILO RS */
    CY_LIB_SLOWCLK_ILO_CR0_REG &= ((uint8)(~CY_LIB_SLOWCLK_ILO_CR0_EN_1KHZ));
}


/*******************************************************************************
* Function Name: CyILO_Start100K
****************************************************************************//**
*
*  Enables the ILO 100 KHz oscillator.
*
*******************************************************************************/
void CyILO_Start100K(void)
{
    CY_LIB_SLOWCLK_ILO_CR0_REG |= CY_LIB_SLOWCLK_ILO_CR0_EN_100KHZ;
}


/*******************************************************************************
* Function Name: CyILO_Stop100K
****************************************************************************//**
*
*  Disables the ILO 100 KHz oscillator.
*
*******************************************************************************/
void CyILO_Stop100K(void)
{
    CY_LIB_SLOWCLK_ILO_CR0_REG &= ((uint8)(~CY_LIB_SLOWCLK_ILO_CR0_EN_100KHZ));
}


/*******************************************************************************
* Function Name: CyILO_Enable33K
****************************************************************************//**
*
*  Enables the ILO 33 KHz divider.
*
*  Note that the 33 KHz clock is generated from the 100 KHz oscillator,
*  so it must also be running in order to generate the 33 KHz output.
*
*******************************************************************************/
void CyILO_Enable33K(void)
{
    /* Set bit 5 of ILO RS */
    CY_LIB_SLOWCLK_ILO_CR0_REG |= CY_LIB_SLOWCLK_ILO_CR0_EN_33KHZ;
}


/*******************************************************************************
* Function Name: CyILO_Disable33K
****************************************************************************//**
*
*  Disables the ILO 33 KHz divider.
*
*  Note that the 33 KHz clock is generated from the 100 KHz oscillator, but this
*  API does not disable the 100 KHz clock.
*
*******************************************************************************/
void CyILO_Disable33K(void)
{
    CY_LIB_SLOWCLK_ILO_CR0_REG &= ((uint8)(~CY_LIB_SLOWCLK_ILO_CR0_EN_33KHZ));
}


/*******************************************************************************
* Function Name: CyILO_SetSource
****************************************************************************//**
*
*  Sets the source of the clock output from the ILO block.
*
*  \param source: One of the three available ILO output sources
*       Value        Define                Source
*       0            CY_ILO_SOURCE_100K    ILO 100 KHz
*       1            CY_ILO_SOURCE_33K     ILO 33 KHz
*       2            CY_ILO_SOURCE_1K      ILO 1 KHz
*
*******************************************************************************/
void CyILO_SetSource(uint8 source)
{
    CY_LIB_CLKDIST_CR_REG = (CY_LIB_CLKDIST_CR_REG & CY_ILO_SOURCE_BITS_CLEAR) |
                    (((uint8) (source << 2u)) & ((uint8)(~CY_ILO_SOURCE_BITS_CLEAR)));
}


/*******************************************************************************
* Function Name: CyILO_SetPowerMode
****************************************************************************//**
*
* Sets the power mode used by the ILO during power down. Allows for lower power
* down power usage resulting in a slower startup time.
*
* \param mode
* CY_ILO_FAST_START - Faster start-up, internal bias left on when powered down
* CY_ILO_SLOW_START - Slower start-up, internal bias off when powered down
*
* \return Prevous power mode state.
*
*******************************************************************************/
uint8 CyILO_SetPowerMode(uint8 mode)
{
    uint8 state;

    /* Get current state. */
    state = CY_LIB_SLOWCLK_ILO_CR0_REG;

    /* Set the oscillator power mode. */
    if(mode != CY_ILO_FAST_START)
    {
        CY_LIB_SLOWCLK_ILO_CR0_REG = (state | CY_ILO_CONTROL_PD_MODE);
    }
    else
    {
        CY_LIB_SLOWCLK_ILO_CR0_REG = (state & ((uint8)(~CY_ILO_CONTROL_PD_MODE)));
    }

    /* Return old mode. */
    return ((state & CY_ILO_CONTROL_PD_MODE) >> CY_ILO_CONTROL_PD_POSITION);
}


/*******************************************************************************
* Function Name: CyXTAL_32KHZ_Start
****************************************************************************//**
*
*  Enables the 32 KHz Crystal Oscillator.
*
*******************************************************************************/
void CyXTAL_32KHZ_Start(void)
{
    volatile uint16 i;

    CY_CLK_XTAL32_TST_REG = CY_CLK_XTAL32_TST_DEFAULT;
    CY_CLK_XTAL32_TR_REG  = CY_CLK_XTAL32_TR_STARTUP;
    CY_CLK_XTAL32_CFG_REG = (CY_CLK_XTAL32_CFG_REG & ((uint8)(~CY_CLK_XTAL32_CFG_LP_MASK))) |
                                CY_CLK_XTAL32_CFG_LP_DEFAULT;

    #if(CY_PSOC3)
        CY_CLK_XTAL32_CR_REG |= CY_CLK_XTAL32_CR_PDBEN;
    #endif  /* (CY_PSOC3) */

    /* Enable operation of 32K Crystal Oscillator */
    CY_CLK_XTAL32_CR_REG |= CY_CLK_XTAL32_CR_EN;

    for (i = 1000u; i > 0u; i--)
    {
        if(0u != (CyXTAL_32KHZ_ReadStatus() & CY_XTAL32K_ANA_STAT))
        {
            /* Ready - switch to high power mode */
            (void) CyXTAL_32KHZ_SetPowerMode(0u);

            break;
        }
        CyDelayUs(1u);
    }
}


/*******************************************************************************
* Function Name: CyXTAL_32KHZ_Stop
****************************************************************************//**
*
*  Disables the 32KHz Crystal Oscillator.
*
*******************************************************************************/
void CyXTAL_32KHZ_Stop(void)
{
    CY_CLK_XTAL32_TST_REG  = CY_CLK_XTAL32_TST_DEFAULT;
    CY_CLK_XTAL32_TR_REG   = CY_CLK_XTAL32_TR_POWERDOWN;
    CY_CLK_XTAL32_CFG_REG = (CY_CLK_XTAL32_CFG_REG & ((uint8)(~CY_CLK_XTAL32_CFG_LP_MASK))) |
                             CY_CLK_XTAL32_CFG_LP_DEFAULT;
    CY_CLK_XTAL32_CR_REG &= ((uint8)(~(CY_CLK_XTAL32_CR_EN | CY_CLK_XTAL32_CR_LPM)));

    #if(CY_PSOC3)
        CY_CLK_XTAL32_CR_REG &= ((uint8)(~CY_CLK_XTAL32_CR_PDBEN));
    #endif  /* (CY_PSOC3) */
}


/*******************************************************************************
* Function Name: CyXTAL_32KHZ_ReadStatus
****************************************************************************//**
*
*  Returns status of the 32 KHz oscillator.
*
* \return
*  Value     Define                    Source
*  20        CY_XTAL32K_ANA_STAT       Analog measurement
*                                       1: Stable
*                                       0: Not stable
*
*******************************************************************************/
uint8 CyXTAL_32KHZ_ReadStatus(void)
{
    return(CY_CLK_XTAL32_CR_REG & CY_XTAL32K_ANA_STAT);
}


/*******************************************************************************
* Function Name: CyXTAL_32KHZ_SetPowerMode
****************************************************************************//**
*
*  Sets the power mode for the 32 KHz oscillator used during the sleep mode.
*  Allows for lower power during sleep when there are fewer sources of noise.
*  During the active mode the oscillator is always run in the high power mode.
*
*  uint8 mode
*       \param 0: High power mode
*       \param 1: Low power mode during sleep
*
* \return
*  Previous power mode.
*
*******************************************************************************/
uint8 CyXTAL_32KHZ_SetPowerMode(uint8 mode)
{
    uint8 state = (0u != (CY_CLK_XTAL32_CR_REG & CY_CLK_XTAL32_CR_LPM)) ? 1u : 0u;

    CY_CLK_XTAL32_TST_REG = CY_CLK_XTAL32_TST_DEFAULT;

    if(1u == mode)
    {
        /* Low power mode during Sleep */
        CY_CLK_XTAL32_TR_REG  = CY_CLK_XTAL32_TR_LOW_POWER;
        CyDelayUs(10u);
        CY_CLK_XTAL32_CFG_REG = (CY_CLK_XTAL32_CFG_REG & ((uint8)(~CY_CLK_XTAL32_CFG_LP_MASK))) |
                                CY_CLK_XTAL32_CFG_LP_LOWPOWER;
        CyDelayUs(20u);
        CY_CLK_XTAL32_CR_REG |= CY_CLK_XTAL32_CR_LPM;
    }
    else
    {
        /* High power mode */
        CY_CLK_XTAL32_TR_REG  = CY_CLK_XTAL32_TR_HIGH_POWER;
        CyDelayUs(10u);
        CY_CLK_XTAL32_CFG_REG = (CY_CLK_XTAL32_CFG_REG & ((uint8)(~CY_CLK_XTAL32_CFG_LP_MASK))) |
                                CY_CLK_XTAL32_CFG_LP_DEFAULT;
        CY_CLK_XTAL32_CR_REG &= ((uint8)(~CY_CLK_XTAL32_CR_LPM));
    }

    return(state);
}


/*******************************************************************************
* Function Name: CyXTAL_Start
****************************************************************************//**
*
*  Enables the megahertz crystal.
*
*  PSoC 3:
*  Waits until the XERR bit is low (no error) for a millisecond or until the
*  number of milliseconds specified by the wait parameter has expired.
*
*   \param wait: Valid range [0-255].
*   This is the timeout value in milliseconds.
*   The appropriate value is crystal specific.
*
* \return
*   CYRET_SUCCESS - Completed successfully
*   CYRET_TIMEOUT - Timeout occurred without detecting a low value on XERR.
*
* Side Effects and Restrictions:
*  If wait is enabled (non-zero wait). Uses the Fast Timewheel to time the wait.
*  Any other use of the Fast Timewheel (FTW) will be stopped during the period
*  of this function and then restored.
*
*  Uses the 100KHz ILO.  If not enabled, this function will enable the 100KHz
*  ILO for the period of this function. No changes to the setup of the ILO,
*  Fast Timewheel, Central Timewheel or Once Per Second interrupt may be made
*  by interrupt routines during the period of this function.
*
*  The current operation of the ILO, Central Timewheel and Once Per Second
*  interrupt are maintained during the operation of this function provided the
*  reading of the Power Manager Interrupt Status Register is only done using the
*  CyPmReadStatus() function.
*
*******************************************************************************/
cystatus CyXTAL_Start(uint8 wait)
{
    cystatus status = CYRET_SUCCESS;
    volatile uint8  timeout = wait;
    volatile uint8 count;
    uint8 iloEnableState;
    uint8 pmTwCfg0Tmp;
    uint8 pmTwCfg2Tmp;


    /* Enables MHz crystal oscillator circuit  */
    CY_CLK_XMHZ_CSR_REG |= CY_CLK_XMHZ_CSR_ENABLE;


    if(wait > 0u)
    {
        /* Save 100 KHz ILO, FTW interval, enable and interrupt enable */
        iloEnableState = CY_LIB_SLOWCLK_ILO_CR0_REG;
        pmTwCfg0Tmp = CY_LIB_PM_TW_CFG0_REG;
        pmTwCfg2Tmp = CY_LIB_PM_TW_CFG2_REG;

        /* Set 250 us interval */
        CyPmFtwSetInterval(CY_CLK_XMHZ_FTW_INTERVAL);
        status = CYRET_TIMEOUT;


        for( ; timeout > 0u; timeout--)
        {
            /* Read XERR bit to clear it */
            (void) CY_CLK_XMHZ_CSR_REG;

            /* Wait for 1 millisecond - 4 x 250 us */
            for(count = 4u; count > 0u; count--)
            {
                while(0u == (CY_PM_FTW_INT & CyPmReadStatus(CY_PM_FTW_INT)))
                {
                    /* Wait for FTW interrupt event */
                }
            }


            /*******************************************************************
            * High output indicates an oscillator failure.
            * Only can be used after a start-up interval (1 ms) is completed.
            *******************************************************************/
            if(0u == (CY_CLK_XMHZ_CSR_REG & CY_CLK_XMHZ_CSR_XERR))
            {
                status = CYRET_SUCCESS;
                break;
            }
        }


        /* Restore 100 KHz ILO, FTW interval, enable and interrupt enable */
        if(0u == (iloEnableState & CY_LIB_SLOWCLK_ILO_CR0_EN_100KHZ))
        {
            CyILO_Stop100K();
        }
        CY_LIB_PM_TW_CFG0_REG = pmTwCfg0Tmp;
        CY_LIB_PM_TW_CFG2_REG = pmTwCfg2Tmp;
    }

    return(status);
}


/*******************************************************************************
* Function Name: CyXTAL_Stop
****************************************************************************//**
*
*  Disables the megahertz crystal oscillator.
*
*******************************************************************************/
void CyXTAL_Stop(void)
{
    /* Disable oscillator. */
    FASTCLK_XMHZ_CSR &= ((uint8)(~XMHZ_CONTROL_ENABLE));
}


/*******************************************************************************
* Function Name: CyXTAL_EnableErrStatus
****************************************************************************//**
*
*  Enables the generation of the XERR status bit for the megahertz crystal.
*  This function is not available for PSoC5.
*
*******************************************************************************/
void CyXTAL_EnableErrStatus(void)
{
    /* If oscillator has insufficient amplitude, XERR bit will be high. */
    CY_CLK_XMHZ_CSR_REG &= ((uint8)(~CY_CLK_XMHZ_CSR_XFB));
}


/*******************************************************************************
* Function Name: CyXTAL_DisableErrStatus
****************************************************************************//**
*
*  Disables the generation of the XERR status bit for the megahertz crystal.
*  This function is not available for PSoC5.
*
*******************************************************************************/
void CyXTAL_DisableErrStatus(void)
{
    /* If oscillator has insufficient amplitude, XERR bit will be high. */
    CY_CLK_XMHZ_CSR_REG |= CY_CLK_XMHZ_CSR_XFB;
}


/*******************************************************************************
* Function Name: CyXTAL_ReadStatus
****************************************************************************//**
*
*  Reads the XERR status bit for the megahertz crystal. This status bit is a
*  sticky, clear on read. This function is not available for PSoC5.
*
* \return
*   Status
*    0: No error
*    1: Error
*
*******************************************************************************/
uint8 CyXTAL_ReadStatus(void)
{
    /***************************************************************************
    * High output indicates an oscillator failure. Only use this after a start-up
    * interval is completed. This can be used for the status and failure recovery.
    ***************************************************************************/
    return((0u != (CY_CLK_XMHZ_CSR_REG & CY_CLK_XMHZ_CSR_XERR)) ? 1u : 0u);
}


/*******************************************************************************
* Function Name: CyXTAL_EnableFaultRecovery
****************************************************************************//**
*
*  Enables the fault recovery circuit which will switch to the IMO in the case
*  of a fault in the megahertz crystal circuit. The crystal must be up and
*  running with the XERR bit at 0, before calling this function to prevent
*  an immediate fault switchover. This function is not available for PSoC5.
*
*******************************************************************************/
void CyXTAL_EnableFaultRecovery(void)
{
    CY_CLK_XMHZ_CSR_REG |= CY_CLK_XMHZ_CSR_XPROT;
}


/*******************************************************************************
* Function Name: CyXTAL_DisableFaultRecovery
****************************************************************************//**
*
*  Disables the fault recovery circuit which will switch to the IMO in the case
*  of a fault in the megahertz crystal circuit. This function is not available
*  for PSoC5.
*
*******************************************************************************/
void CyXTAL_DisableFaultRecovery(void)
{
    CY_CLK_XMHZ_CSR_REG &= ((uint8)(~CY_CLK_XMHZ_CSR_XPROT));
}


/*******************************************************************************
* Function Name: CyXTAL_SetStartup
****************************************************************************//**
*
*  Sets the startup settings for the crystal. The logic model outputs a
*  frequency (setting + 4) MHz when enabled.
*
*  This is artificial as the actual frequency is determined by an attached
*  external crystal.
*
*  \param setting: Valid range [0-31].
*   The value is dependent on the frequency and quality of the crystal being
*   used. Refer to the device TRM and datasheet for more information.
*
*******************************************************************************/
void CyXTAL_SetStartup(uint8 setting)
{
    CY_CLK_XMHZ_CFG0_REG = (CY_CLK_XMHZ_CFG0_REG & ((uint8)(~CY_CLK_XMHZ_CFG0_XCFG_MASK))) |
                           (setting & CY_CLK_XMHZ_CFG0_XCFG_MASK);
}



/*******************************************************************************
* Function Name: CyXTAL_SetFbVoltage
****************************************************************************//**
*
*  Sets the feedback reference voltage to use for the crystal circuit.
*  This function is only available for PSoC3 and PSoC 5LP.
*
*  \param setting: Valid range [0-15].
*  Refer to the device TRM and datasheet for more information.
*
*******************************************************************************/
void CyXTAL_SetFbVoltage(uint8 setting)
{
    CY_CLK_XMHZ_CFG1_REG = ((CY_CLK_XMHZ_CFG1_REG & ((uint8)(~CY_CLK_XMHZ_CFG1_VREF_FB_MASK))) |
                            (setting & CY_CLK_XMHZ_CFG1_VREF_FB_MASK));
}


/*******************************************************************************
* Function Name: CyXTAL_SetWdVoltage
****************************************************************************//**
*
*  Sets the reference voltage used by the watchdog to detect a failure in the
*  crystal circuit. This function is only available for PSoC3 and PSoC 5LP.
*
*  \param setting: Valid range [0-7].
*  Refer to the device TRM and datasheet for more information.
*
*******************************************************************************/
void CyXTAL_SetWdVoltage(uint8 setting)
{
    CY_CLK_XMHZ_CFG1_REG = ((CY_CLK_XMHZ_CFG1_REG & ((uint8)(~CY_CLK_XMHZ_CFG1_VREF_WD_MASK))) |
                            (((uint8)(setting << 4u)) & CY_CLK_XMHZ_CFG1_VREF_WD_MASK));
}


/*******************************************************************************
* Function Name: CyHalt
****************************************************************************//**
*
*  Halts the CPU.
*
*  \param uint8 reason: Value to be used during debugging.
*
*******************************************************************************/
void CyHalt(uint8 reason) CYREENTRANT
{
    if(0u != reason)
    {
        /* To remove unreferenced local variable warning */
    }

    #if defined (__ARMCC_VERSION)
        __breakpoint(0x0);
    #elif defined(__GNUC__) || defined (__ICCARM__)
        __asm("    bkpt    1");
    #elif defined(__C51__)
        CYDEV_HALT_CPU;
    #endif  /* (__ARMCC_VERSION) */
}


/*******************************************************************************
* Function Name: CySoftwareReset
****************************************************************************//**
*
*  Forces a device software reset.
*
*******************************************************************************/
void CySoftwareReset(void)
{
    CY_LIB_RESET_CR2_REG |= CY_LIB_RESET_CR2_RESET;
}


/*******************************************************************************
* Function Name: CyDelay
****************************************************************************//**
*
*  Blocks for milliseconds.
*
*  Note:
*  CyDelay has been implemented with the instruction cache assumed enabled. When
*  the instruction cache is disabled on PSoC5, CyDelay will be two times larger.
*  For example, with instruction cache disabled CyDelay(100) would result in
*  about 200 ms delay instead of 100 ms.
*
*  \param milliseconds: number of milliseconds to delay.
*
*******************************************************************************/
void CyDelay(uint32 milliseconds) CYREENTRANT
{
    while (milliseconds > 32768u)
    {
        /***********************************************************************
        * This loop prevents overflow.At 100MHz, milliseconds * delay_freq_khz
        * overflows at about 42 seconds.
        ***********************************************************************/
        CyDelayCycles(cydelay_32k_ms);
        milliseconds = ((uint32)(milliseconds - 32768u));
    }

    CyDelayCycles(milliseconds * cydelay_freq_khz);
}


#if(!CY_PSOC3)

    /* For PSoC3 devices function is defined in CyBootAsmKeil.a51 file */

    /*******************************************************************************
    * Function Name: CyDelayUs
    ****************************************************************************//**
    *
    *  Blocks for microseconds.
    *
    *  Note:
    *   CyDelay has been implemented with the instruction cache assumed enabled.
    *   When instruction cache is disabled on PSoC5, CyDelayUs will be two times
    *   larger. Ex: With instruction cache disabled CyDelayUs(100) would result
    *   in about 200us delay instead of 100us.
    *
    *  \param uint16 microseconds: number of microseconds to delay.
    *
    * \sideeffect
    *  CyDelayUS has been implemented with the instruction cache assumed enabled.
    *  When the instruction cache is disabled on PSoC 5, CyDelayUs will be two times
    *  larger. For example, with the instruction cache disabled CyDelayUs(100) would
    *  result in about 200 us delay instead of 100 us.
    *
    *  If the bus clock frequency is a small non-integer number, the actual delay
    *  can be up to twice as long as the nominal value. The actual delay cannot be
    *  shorter than the nominal one.
    *******************************************************************************/
    void CyDelayUs(uint16 microseconds) CYREENTRANT
    {
        CyDelayCycles((uint32)microseconds * cydelay_freq_mhz);
    }

#endif  /* (!CY_PSOC3) */


/*******************************************************************************
* Function Name: CyDelayFreq
****************************************************************************//**
*
*  Sets the clock frequency for CyDelay.
*
*  \param freq: The frequency of the bus clock in Hertz.
*
*******************************************************************************/
void CyDelayFreq(uint32 freq) CYREENTRANT
{
    if (freq != 0u)
    {
        cydelay_freq_hz = freq;
    }
    else
    {
        cydelay_freq_hz = BCLK__BUS_CLK__HZ;
    }

    cydelay_freq_mhz = (uint8)((cydelay_freq_hz + 999999u) / 1000000u);
    cydelay_freq_khz = (cydelay_freq_hz + 999u) / 1000u;
    cydelay_32k_ms   = 32768u * cydelay_freq_khz;
}


/*******************************************************************************
* Function Name: CyWdtStart
****************************************************************************//**
*
*  Enables the watchdog timer.
*
*  The timer is configured for the specified count interval, the central
*  timewheel is cleared, the setting for the low power mode is configured and
*  the watchdog timer is enabled.
*
*  Once enabled the watchdog cannot be disabled. The watchdog counts each time
*  the Central Time Wheel (CTW) reaches the period specified. The watchdog must
*  be cleared using the CyWdtClear() function before three ticks of the watchdog
*  timer occur. The CTW is free running, so this will occur after between 2 and
*  3 timer periods elapse.
*
*  PSoC5: The watchdog timer should not be used during sleep modes. Since the
*  WDT cannot be disabled after it is enabled, the WDT timeout period can be
*  set to be greater than the sleep wakeup period, then feed the dog on each
*  wakeup from Sleep.
*
*  \param ticks: One of the four available timer periods. Once WDT enabled, the
   interval cannot be changed.
*         CYWDT_2_TICKS     -     4 - 6     ms
*         CYWDT_16_TICKS    -    32 - 48    ms
*         CYWDT_128_TICKS   -   256 - 384   ms
*         CYWDT_1024_TICKS  - 2.048 - 3.072 s
*
*  \param lpMode: Low power mode configuration. This parameter is ignored for PSoC 5.
*          The WDT always acts as if CYWDT_LPMODE_NOCHANGE is passed.
*
*          CYWDT_LPMODE_NOCHANGE - No Change
*          CYWDT_LPMODE_MAXINTER - Switch to longest timer mode during low power
*                                 mode
*          CYWDT_LPMODE_DISABLED - Disable WDT during low power mode
*
* \sideeffect
*  PSoC5: The ILO 1 KHz must be enabled for proper WDT operation. Stopping the
*  ILO 1 kHz could break the active WDT functionality.
*
*******************************************************************************/
void CyWdtStart(uint8 ticks, uint8 lpMode)
{
    /* Set WDT interval */
    CY_WDT_CFG_REG = (CY_WDT_CFG_REG & ((uint8)(~CY_WDT_CFG_INTERVAL_MASK))) | (ticks & CY_WDT_CFG_INTERVAL_MASK);

    /* Reset CTW to ensure that first watchdog period is full */
    CY_WDT_CFG_REG |= CY_WDT_CFG_CTW_RESET;
    CY_WDT_CFG_REG &= ((uint8)(~CY_WDT_CFG_CTW_RESET));

    /* Setting low power mode */
    CY_WDT_CFG_REG = (((uint8)(lpMode << CY_WDT_CFG_LPMODE_SHIFT)) & CY_WDT_CFG_LPMODE_MASK) |
                       (CY_WDT_CFG_REG & ((uint8)(~CY_WDT_CFG_LPMODE_MASK)));

    /* Enables watchdog reset */
    CY_WDT_CFG_REG |= CY_WDT_CFG_WDR_EN;
}


/*******************************************************************************
* Function Name: CyWdtClear
****************************************************************************//**
*
*  Clears (feeds) the watchdog timer.
*
*******************************************************************************/
void CyWdtClear(void)
{
    CY_WDT_CR_REG = CY_WDT_CR_FEED;
}



/*******************************************************************************
* Function Name: CyVdLvDigitEnable
****************************************************************************//**
*
*  Sets the voltage trip level, enables the output of the digital low-voltage
*  monitor, and optionally configures voltage monitor to reset device upon the
*  low-voltage event instead of generating an interrupt.
*
*  Note The associated interrupt enable/disable state is not changed by the
*  function. The Interrupt component API should be used to register the
*  interrupt service routine and to enable/disable associated interrupt.
*
*  \param reset: Enables device reset on digital low-voltage event:
*   Zero - Interrupt on digital low-voltage event
*   Non-zero - Reset on digital low-voltage event
*
*  \param threshold: Sets the trip point of the digital low-voltage monitoring circuit
*   in steps of approximately 250 mV in range from 1.70 V (0x00) to 5.45 V
*   (0x0F). For example, the trip point is set to 1.80 V when the threshold
*   parameter value is 0x04. Refer to the device TRM for the exact trip voltage
*   values.
*
* Side Effects and Restrictions:
*  The voltage resets are momentary. When a voltage reset (analog/digital
*  low-voltage and analog high-voltage) occurs, the RESET_CR1 and RESET_CR3
*  registers are restored to their default values. This means that the voltage
*  monitor circuit is no longer enabled and the device exits reset. If the
*  supply is below the trip level and firmware enables the voltage reset
*  functionality, the device will reset again. This will continue as long as the
*  supply is below the trip level or as long as the user enables the reset
*  functionality of the voltage monitor functionality.
*
*  When any voltage reset occurs, the RESET_SR0 and RESET_SR2 status registers
*  are cleared. This means that analog low-voltage, digital low-voltage and
*  analog high-voltage status bits are not persistent across any voltage reset.
*
*******************************************************************************/
void CyVdLvDigitEnable(uint8 reset, uint8 threshold)
{
    uint32 intRegTmp;
    uint8 interruptState;

    interruptState = CyEnterCriticalSection();

    /* Store interrupt enable state */
    intRegTmp = CY_INT_ENABLE_REG & CY_VD_INT_MASK;

    /* Disable VD interrupt (write 1) to protect against glitches */
    CY_INT_CLEAR_REG = CY_VD_INT_MASK;

    CY_VD_PRES_CONTROL_REG &= ((uint8)(~CY_VD_PRESD_EN));

    CY_VD_LVI_TRIP_REG = (threshold & CY_VD_LVI_TRIP_LVID_MASK) |
                            (CY_VD_LVI_TRIP_REG & ((uint8)(~CY_VD_LVI_TRIP_LVID_MASK)));
    CY_VD_LVI_HVI_CONTROL_REG |= CY_VD_LVID_EN;

    /* Timeout to eliminate glitches on LVI/HVI when enabling (ID # 127412) */
    CyDelayUs(1u);

    (void) CyVdStickyStatus(CY_VD_LVID);

    if(0u != reset)
    {
        CY_VD_PRES_CONTROL_REG |= CY_VD_PRESD_EN;
    }
    else
    {
        CY_VD_PRES_CONTROL_REG &= ((uint8)(~CY_VD_PRESD_EN));
    }

    /* Clear pending interrupt */
    CY_INT_CLR_PEND_REG = CY_VD_INT_MASK;

    /* Restore interrupt enable state */
    CY_INT_ENABLE_REG = intRegTmp;

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CyVdLvAnalogEnable
****************************************************************************//**
*
*  Sets the voltage trip level, enables the output of the analog low-voltage
*  monitor, and optionally configures voltage monitor to reset device upon the
*  low-voltage event instead of generating an interrupt.
*
*  Note The associated interrupt enable/disable state is not changed by the
*  function. The Interrupt component API should be used to register the
*  interrupt service routine and to enable/disable associated interrupt.
*
*  \param reset: Enables device reset on analog low-voltage event:
*  Zero - Interrupt on analog low-voltage event
*  Non-zero - Reset on analog low-voltage event
*
*  \param threshold: Sets the trip point of the analog low-voltage monitoring circuit
*  in steps of approximately 250 mV in range from 1.70 V (0x00) to 5.45 V
*  (0x0F). For example, the trip point is set to 1.80 V when value of the
*  threshold parameter is 0x04. Please refer to the device TRM for the exact
*  trip voltage values.
*
* Side Effects and Restrictions:
*  The voltage resets are momentary. When a voltage reset (analog/digital
*  low-voltage and analog high-voltage) occurs, the RESET_CR1 and RESET_CR3
*  registers are restored to their default values. This means that the voltage
*  monitor circuit is no longer enabled and the device exits reset. If the
*  supply is below the trip level and firmware enables the voltage reset
*  functionality, the device will reset again. This will continue as long as
*  the supply is below the trip level or as long as the user enables the reset
*  functionality of the voltage monitor functionality.
*
*  When any voltage reset occurs, the RESET_SR0 and RESET_SR2 status registers
*  are cleared. This means that analog low-voltage, digital low-voltage and
*  analog high-voltage status bits are not persistent across any voltage reset.
*
*******************************************************************************/
void CyVdLvAnalogEnable(uint8 reset, uint8 threshold)
{
    uint32 intRegTmp;
    uint8 interruptState;

    interruptState = CyEnterCriticalSection();

    /* Store interrupt enable state */
    intRegTmp = CY_INT_ENABLE_REG & CY_VD_INT_MASK;

    /* Disable VD interrupt (write 1) to protect against glitches */
    CY_INT_CLEAR_REG = CY_VD_INT_MASK;

    CY_VD_PRES_CONTROL_REG &= ((uint8)(~CY_VD_PRESA_EN));

    CY_VD_LVI_TRIP_REG = ((uint8)(threshold << 4u)) | (CY_VD_LVI_TRIP_REG & 0x0Fu);
    CY_VD_LVI_HVI_CONTROL_REG |= CY_VD_LVIA_EN;

    /* Timeout to eliminate glitches on LVI/HVI when enabling (ID # 127412) */
    CyDelayUs(1u);

    (void) CyVdStickyStatus(CY_VD_LVIA);

    if(0u != reset)
    {
        CY_VD_PRES_CONTROL_REG |= CY_VD_PRESA_EN;
    }
    else
    {
        CY_VD_PRES_CONTROL_REG &= ((uint8)(~CY_VD_PRESA_EN));
    }

    /* Clear pending interrupt */
    CY_INT_CLR_PEND_REG = CY_VD_INT_MASK;

    /* Restore interrupt enable state */
    CY_INT_ENABLE_REG = intRegTmp;

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CyVdLvDigitDisable
****************************************************************************//**
*
*  Disables the digital low-voltage monitor, turns off device reset upon the
*  digital low-voltage event, and clears the associated persistent status bit.
*
*  Note The associated interrupt enable/disable state is not changed by the
*  function. The pending interrupt status is not cleared. The Interrupt
*  component API should be used to manipulate with the associated interrupts.
*
*******************************************************************************/
void CyVdLvDigitDisable(void)
{
    CY_VD_LVI_HVI_CONTROL_REG &= ((uint8)(~CY_VD_LVID_EN));
    CY_VD_PRES_CONTROL_REG &= ((uint8)(~CY_VD_PRESD_EN));
    (void) CyVdStickyStatus(CY_VD_LVID);

    while(0u != (CyVdStickyStatus(CY_VD_LVID) & CY_VD_LVID))
    {

    }
}


/*******************************************************************************
* Function Name: CyVdLvAnalogDisable
****************************************************************************//**
*
*  Disables the analog low-voltage monitor, turns off device reset upon the
*  analog low-voltage event, and clears the associated persistent status bit.
*
*  Note The associated interrupt enable/disable state is not changed by the
*  function. The pending interrupt status is not cleared. The Interrupt
*  component API should be used to manipulate with the associated interrupts.
*
*******************************************************************************/
void CyVdLvAnalogDisable(void)
{
    CY_VD_LVI_HVI_CONTROL_REG &= ((uint8)(~CY_VD_LVIA_EN));
    CY_VD_PRES_CONTROL_REG &= ((uint8)(~CY_VD_PRESA_EN));
    while(0u != (CyVdStickyStatus(CY_VD_LVIA) & CY_VD_LVIA))
    {

    }
}


/*******************************************************************************
* Function Name: CyVdHvAnalogEnable
****************************************************************************//**
*
*  Enables the output of the analog high-voltage monitor and sets 5.75 V
*  threshold detection for Vdda.
*
*  Note The associated interrupt enable/disable state is not changed by the
*  function. The Interrupt component API should be used to register the
*  interrupt service routine and to enable/disable associated interrupt.
*
*******************************************************************************/
void CyVdHvAnalogEnable(void)
{
    uint32 intRegTmp;
    uint8 interruptState;

    interruptState = CyEnterCriticalSection();

    /* Store interrupt enable state */
    intRegTmp = CY_INT_ENABLE_REG & CY_VD_INT_MASK;

    /* Disable VD interrupt (write 1) to protect against glitches */
    CY_INT_CLEAR_REG = CY_VD_INT_MASK;

    CY_VD_PRES_CONTROL_REG &= ((uint8)(~CY_VD_PRESA_EN));

    CY_VD_LVI_HVI_CONTROL_REG |= CY_VD_HVIA_EN;

    /* Timeout to eliminate glitches on the LVI/HVI when enabling (ID # 127412)  */
    CyDelayUs(1u);

    (void) CyVdStickyStatus(CY_VD_HVIA);

    /* Clear pending interrupt */
    CY_INT_CLR_PEND_REG = CY_VD_INT_MASK;

    /* Restore interrupt enable state */
    CY_INT_ENABLE_REG = intRegTmp;

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CyVdHvAnalogDisable
****************************************************************************//**
*
*  Disables the analog high-voltage monitor and clears the associated persistent
*  status bit.
*
*  Note The associated interrupt enable/disable state is not changed by the
*  function. The pending interrupt status is not cleared. The Interrupt
*  component API should be used to manipulate with the associated interrupts.
*
*******************************************************************************/
void CyVdHvAnalogDisable(void)
{
    CY_VD_LVI_HVI_CONTROL_REG &= ((uint8)(~CY_VD_HVIA_EN));
    while(0u != (CyVdStickyStatus(CY_VD_HVIA) & CY_VD_HVIA))
    {

    }
}


/*******************************************************************************
* Function Name: CyVdStickyStatus
****************************************************************************//**
*
*  Reads and clears the voltage detection status bits in the RESET_SR0 register.
*  The bits are set to 1 by the voltage monitor circuit when the supply is
*  outside the detector trip point. They stay set to 1 until they are read or
*  a POR / LVI / PRES reset occurs. This function uses a shadow register, so
*  only the bits passed in the parameter will be cleared in the shadow register.
*
*  \param mask: Bits in the RESET_SR0 shadow register to clear and return.
*   Define                  Definition
*   CY_VD_LVID            Persistent status of digital LVI.
*   CY_VD_LVIA            Persistent status of analog LVI.
*   CY_VD_HVIA            Persistent status of analog HVI.
*
* \return
*  Status. Same enumerated bit values as used for the mask parameter. A zero is
*  returned for bits not used in the mask parameter.
*
* Side Effects and Restrictions:
*  When an LVI reset occurs, the RESET_SR0 status registers are cleared. This
*  means that the voltage detection status bits are not persistent across an LVI
*  reset and cannot be used to determine a reset source.
*
*******************************************************************************/
uint8 CyVdStickyStatus(uint8 mask)
{
    static uint8 interruptStatus;
    uint8 interruptState;
    uint8 tmpStatus;

    interruptState = CyEnterCriticalSection();

    interruptStatus |= CY_VD_PERSISTENT_STATUS_REG;
    tmpStatus = interruptStatus & (uint8)(CY_VD_LVID | CY_VD_LVIA | CY_VD_HVIA);
    interruptStatus &= ((uint8)(~mask));

    CyExitCriticalSection(interruptState);

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: CyVdRealTimeStatus
****************************************************************************//**
*
*  Reads the real-time voltage detection status bits in the RESET_SR2 register.
*  The bits are set to 1 by the voltage monitor circuit when the supply is
*  outside the detector’s trip point, and set to 0 when the supply is inside the
*  trip point.
*
* \return
*  Status of the LVID, LVIA, and HVIA bits in the RESET_SR2 register.
*   Define                  Definition
*   CY_VD_LVID            Real-time status of digital LVI.
*   CY_VD_LVIA            Real-time status of analog LVI.
*   CY_VD_HVIA            Real-time status of analog HVI.
*
* Side Effects and Restrictions:
*  When an LVI reset occurs, the RESET_SR2 status registers are cleared. This
*  means that the voltage detection status bits are not persistent across an LVI
*  reset and cannot be used to determine a reset source.
*
*******************************************************************************/
uint8 CyVdRealTimeStatus(void)
{
    uint8 interruptState;
    uint8 vdFlagsState;

    interruptState = CyEnterCriticalSection();
    vdFlagsState = CY_VD_RT_STATUS_REG & (CY_VD_LVID | CY_VD_LVIA | CY_VD_HVIA);
    CyExitCriticalSection(interruptState);

    return(vdFlagsState);
}


/*******************************************************************************
* Function Name: CyDisableInts
****************************************************************************//**
*
*  Disables the interrupt enable for each interrupt.
*
* \return
*  32 bit mask of previously enabled interrupts.
*
*******************************************************************************/
uint32 CyDisableInts(void)
{
    uint32 intState;
    uint8 interruptState;

    interruptState = CyEnterCriticalSection();

    #if(CY_PSOC3)

        /* Get the current interrupt state. */
        intState  = ((uint32) CY_GET_REG8(CY_INT_CLR_EN0_PTR));
        intState |= ((uint32) (((uint32) CY_GET_REG8(CY_INT_CLR_EN1_PTR)) << 8u));
        intState |= ((uint32) (((uint32) CY_GET_REG8(CY_INT_CLR_EN2_PTR)) << 16u));
        intState |= ((uint32) (((uint32) CY_GET_REG8(CY_INT_CLR_EN3_PTR)) << 24u));


        /* Disable all of the interrupts. */
        CY_SET_REG8(CY_INT_CLR_EN0_PTR, 0xFFu);
        CY_SET_REG8(CY_INT_CLR_EN1_PTR, 0xFFu);
        CY_SET_REG8(CY_INT_CLR_EN2_PTR, 0xFFu);
        CY_SET_REG8(CY_INT_CLR_EN3_PTR, 0xFFu);

    #else

        /* Get the current interrupt state. */
        intState = CY_GET_REG32(CY_INT_CLEAR_PTR);

        /* Disable all of the interrupts. */
        CY_SET_REG32(CY_INT_CLEAR_PTR, 0xFFFFFFFFu);

    #endif /* (CY_PSOC3) */

    CyExitCriticalSection(interruptState);

    return (intState);
}


/*******************************************************************************
* Function Name: CyEnableInts
****************************************************************************//**
*
*  Enables interrupts to a given state.
*
*  \param uint32 mask: 32 bit mask of interrupts to enable.
*
*******************************************************************************/
void CyEnableInts(uint32 mask)
{

    uint8 interruptState;

    interruptState = CyEnterCriticalSection();

    #if(CY_PSOC3)

        /* Set interrupts as enabled. */
        CY_SET_REG8(CY_INT_SET_EN3_PTR, ((uint8) (mask >> 24u)));
        CY_SET_REG8(CY_INT_SET_EN2_PTR, ((uint8) (mask >> 16u)));
        CY_SET_REG8(CY_INT_SET_EN1_PTR, ((uint8) (mask >> 8u )));
        CY_SET_REG8(CY_INT_SET_EN0_PTR, ((uint8) (mask )));

    #else

        CY_SET_REG32(CY_INT_ENABLE_PTR, mask);

    #endif /* (CY_PSOC3) */

    CyExitCriticalSection(interruptState);

}

#if(CY_PSOC5)

    /*******************************************************************************
    * Function Name: CyFlushCache
    ****************************************************************************//**
    *  Call this API after a flash row erase/write operation to invalidate or flush
    *  any of that particular flash region content already present in the cache.
    *  After a cache flush operation, any access to that flash region after the
    *  erase/write operation would reload the cache with the modified data from the
    *  flash region. If the flash region update involves multiple flash row write
    *  operations, then the flushing of the cache can be done once at the end of
    *  the operation as long as the flash data would not be accessed in the middle
    *  of the multiple row update process. Else, flush the cache after every flash
    *  row write.
    *
    *******************************************************************************/
    void CyFlushCache(void)
    {
        uint8 interruptState;

        /* Save current global interrupt enable and disable it */
        interruptState = CyEnterCriticalSection();

        /* Fill instruction prefectch unit to insure data integrity */
        CY_NOP;
        CY_NOP;
        CY_NOP;
        CY_NOP;
        CY_NOP;
        CY_NOP;
        CY_NOP;
        CY_NOP;
        CY_NOP;
        CY_NOP;
        CY_NOP;
        CY_NOP;
        CY_NOP;
        CY_NOP;
        CY_NOP;
        CY_NOP;

        /* All entries in cache are invalidated on next clock cycle. */
        CY_CACHE_CONTROL_REG |= CY_CACHE_CONTROL_FLUSH;

        /* Once this is executed it's guaranteed the cache has been flushed */
        (void) CY_CACHE_CONTROL_REG;

        /* Flush the pipeline */
        CY_SYS_ISB;

        /* Restore global interrupt enable state */
        CyExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: CyIntSetSysVector
    ****************************************************************************//**
    *  Sets the interrupt vector of the specified system interrupt number. System
    *  interrupts are present only for the ARM platform. These interrupts are for
    *  SysTick, PendSV and others.
    *
    *  \param number: System interrupt number:
    *    CY_INT_NMI_IRQN                - Non Maskable Interrupt
    *    CY_INT_HARD_FAULT_IRQN         - Hard Fault Interrupt
    *    CY_INT_MEM_MANAGE_IRQN         - Memory Management Interrupt
    *    CY_INT_BUS_FAULT_IRQN          - Bus Fault Interrupt
    *    CY_INT_USAGE_FAULT_IRQN        - Usage Fault Interrupt
    *    CY_INT_SVCALL_IRQN             - SV Call Interrupt
    *    CY_INT_DEBUG_MONITOR_IRQN      - Debug Monitor Interrupt
    *    CY_INT_PEND_SV_IRQN            - Pend SV Interrupt
    *    CY_INT_SYSTICK_IRQN            - System Tick Interrupt
    *
    *  \param address: Pointer to an interrupt service routine.
    *
    * \return
    *   The old ISR vector at this location.
    *
    *******************************************************************************/
    cyisraddress CyIntSetSysVector(uint8 number, cyisraddress address)
    {
        cyisraddress oldIsr;
        cyisraddress *ramVectorTable = *CY_INT_VECT_TABLE;

        CYASSERT(number <= CY_INT_SYS_NUMBER_MAX);

        /* Save old Interrupt service routine. */
        oldIsr = ramVectorTable[number & CY_INT_SYS_NUMBER_MASK];

        /* Set new Interrupt service routine. */
        ramVectorTable[number & CY_INT_SYS_NUMBER_MASK] = address;

        return (oldIsr);
    }


    /*******************************************************************************
    * Function Name: CyIntGetSysVector
    ****************************************************************************//**
    *
    *  Gets the interrupt vector of the specified system interrupt number. System
    *  interrupts are present only for the ARM platform. These interrupts are for
    *  SysTick, PendSV and others.
    *
    *  \param number: System interrupt number:
    *    CY_INT_NMI_IRQN                - Non Maskable Interrupt
    *    CY_INT_HARD_FAULT_IRQN         - Hard Fault Interrupt
    *    CY_INT_MEMORY_MANAGEMENT_IRQN  - Memory Management Interrupt
    *    CY_INT_BUS_FAULT_IRQN          - Bus Fault Interrupt
    *    CY_INT_USAGE_FAULT_IRQN        - Usage Fault Interrupt
    *    CY_INT_SVCALL_IRQN             - SV Call Interrupt
    *    CY_INT_DEBUG_MONITOR_IRQN      - Debug Monitor Interrupt
    *    CY_INT_PEND_SV_IRQN            - Pend SV Interrupt
    *    CY_INT_SYSTICK_IRQN            - System Tick Interrupt
    *
    * \return
    *   Address of the ISR in the interrupt vector table.
    *
    *******************************************************************************/
    cyisraddress CyIntGetSysVector(uint8 number)
    {
        cyisraddress *ramVectorTable = *CY_INT_VECT_TABLE;
        CYASSERT(number <= CY_INT_SYS_NUMBER_MAX);

        return ramVectorTable[number & CY_INT_SYS_NUMBER_MASK];
    }


    /*******************************************************************************
    * Function Name: CyIntSetVector
    ****************************************************************************//**
    *
    *  Sets the interrupt vector of the specified interrupt number.
    *
    *  \param number: Valid range [0-31].  Interrupt number
    *  \param address: Pointer to an interrupt service routine
    *
    * \return
    *   Previous interrupt vector value.
    *
    *******************************************************************************/
    cyisraddress CyIntSetVector(uint8 number, cyisraddress address)
    {
        cyisraddress oldIsr;
        cyisraddress *ramVectorTable = *CY_INT_VECT_TABLE;

        CYASSERT(number <= CY_INT_NUMBER_MAX);
				uint32_t i = CY_INT_IRQ_BASE + (number & CY_INT_NUMBER_MASK);

        /* Save old Interrupt service routine. */
        oldIsr = ramVectorTable[i];

        /* Set new Interrupt service routine. */
        ramVectorTable[i] = address;

        return (oldIsr);
    }


    /*******************************************************************************
    * Function Name: CyIntGetVector
    ****************************************************************************//**
    *
    *  Gets the interrupt vector of the specified interrupt number.
    *
    *  \param number: Valid range [0-31].  Interrupt number
    *
    * \return
    *  The address of the ISR in the interrupt vector table.
    *
    *******************************************************************************/
    cyisraddress CyIntGetVector(uint8 number)
    {
        cyisraddress *ramVectorTable = *CY_INT_VECT_TABLE;
        CYASSERT(number <= CY_INT_NUMBER_MAX);

        return (ramVectorTable[CY_INT_IRQ_BASE + (number & CY_INT_NUMBER_MASK)]);
    }


    /*******************************************************************************
    * Function Name: CyIntSetPriority
    ****************************************************************************//**
    *
    *  Sets the Priority of the Interrupt.
    *
    *  \param priority: Priority of the interrupt. 0 - 7, 0 being the highest.
    *  \param number: The number of the interrupt, 0 - 31.
    *
    *******************************************************************************/
    void CyIntSetPriority(uint8 number, uint8 priority)
    {
        CYASSERT(priority <= CY_INT_PRIORITY_MAX);
        CYASSERT(number <= CY_INT_NUMBER_MAX);
        CY_INT_PRIORITY_PTR[number & CY_INT_NUMBER_MASK] = (priority & CY_INT_PRIORITY_MASK)<< 5;
    }


    /*******************************************************************************
    * Function Name: CyIntGetPriority
    ****************************************************************************//**
    *
    *  Gets the Priority of the Interrupt.
    *
    *  \param number: The number of the interrupt, 0 - 31.
    *
    * \return
    *  Priority of the interrupt. 0 - 7, 0 being the highest.
    *
    *******************************************************************************/
    uint8 CyIntGetPriority(uint8 number)
    {
        uint8 priority;

        CYASSERT(number <= CY_INT_NUMBER_MAX);

        priority = CY_INT_PRIORITY_PTR[number & CY_INT_NUMBER_MASK] >> 5;

        return (priority);
    }


    /*******************************************************************************
    * Function Name: CyIntGetState
    ****************************************************************************//**
    *
    *   Gets the enable state of the specified interrupt number.
    *
    *   \param number: Valid range [0-31].  Interrupt number.
    *
    * \return
    *   Enable status: 1 if enabled, 0 if disabled
    *
    *******************************************************************************/
    uint8 CyIntGetState(uint8 number)
    {
        reg32 * stateReg;

        CYASSERT(number <= CY_INT_NUMBER_MAX);

        /* Get pointer to Interrupt enable register. */
        stateReg = CY_INT_ENABLE_PTR;

        /* Get state of interrupt. */
        return (0u != (*stateReg & (((uint32) 1u) << (0x1Fu & number)))) ? ((uint8)(1u)) : ((uint8)(0u));
    }


#else   /* PSoC3 */

    /*******************************************************************************
    * Function Name: IntDefaultHandler
    ****************************************************************************//**
    *
    *  This function is called for all interrupts, other than a reset that gets
    *  called before the system is setup.
    *
    * Theory:
    *  Any value other than zero is acceptable.
    *
    *******************************************************************************/
    CY_ISR(IntDefaultHandler)
    {
        #ifdef CY_BOOT_INT_DEFAULT_HANDLER_EXCEPTION_ENTRY_CALLBACK
            CyBoot_IntDefaultHandler_Exception_EntryCallback();
        #endif /* CY_BOOT_INT_DEFAULT_HANDLER_EXCEPTION_ENTRY_CALLBACK */

        while(1)
        {
            /***********************************************************************
            * We must not get here. If we do, a serious problem occurs, so go
            * into an infinite loop.
            ***********************************************************************/
        }
    }


    /*******************************************************************************
    * Function Name: IntDefaultHandler
    ****************************************************************************//**
    *
    *  This function is called during startup to initialize interrupt address vector
    *  registers with the address of the IntDefaultHandler().
    *
    *******************************************************************************/
    void CyIntInitVectors(void)
    {
        uint8 i;

        for (i = 0; i <= CY_INT_NUMBER_MAX; i++)
        {
            CY_SET_REG16(&CY_INT_VECT_TABLE[i], (uint16) &IntDefaultHandler);
        }
    }


    /*******************************************************************************
    * Function Name: CyIntSetVector
    ****************************************************************************//**
    *
    *  Sets the interrupt vector of the specified interrupt number.
    *
    *  \param number:  Valid range [0-31].  Interrupt number
    *  \param address: Pointer to an interrupt service routine
    *
    * \return
    *  Previous interrupt vector value.
    *
    *******************************************************************************/
    cyisraddress CyIntSetVector(uint8 number, cyisraddress address)
    {
        cyisraddress oldIsr;

        CYASSERT(number <= CY_INT_NUMBER_MAX);

        /* Save old Interrupt service routine. */
        oldIsr = (cyisraddress) \
                    CY_GET_REG16(&CY_INT_VECT_TABLE[number & CY_INT_NUMBER_MASK]);

        /* Set new Interrupt service routine. */
        CY_SET_REG16(&CY_INT_VECT_TABLE[number], (uint16) address);

        return (oldIsr);
    }


    /*******************************************************************************
    * Function Name: CyIntGetVector
    ****************************************************************************//**
    *
    *  Gets the interrupt vector of the specified interrupt number.
    *
    *  \param number: Valid range [0-31].  Interrupt number
    *
    * \return
    *  Address of the ISR in the interrupt vector table.
    *
    *******************************************************************************/
    cyisraddress CyIntGetVector(uint8 number)
    {
        CYASSERT(number <= CY_INT_NUMBER_MAX);

        return ((cyisraddress) \
                CY_GET_REG16(&CY_INT_VECT_TABLE[number & CY_INT_NUMBER_MASK]));
    }


    /*******************************************************************************
    * Function Name: CyIntSetPriority
    ****************************************************************************//**
    *
    *  Sets the Priority of the Interrupt.
    *
    *  \param priority: Priority of the interrupt. 0 - 7, 0 being the highest.
    *  \param number:   The number of the interrupt, 0 - 31.
    *
    *******************************************************************************/
    void CyIntSetPriority(uint8 number, uint8 priority)
    {
        CYASSERT(priority <= CY_INT_PRIORITY_MAX);

        CYASSERT(number <= CY_INT_NUMBER_MAX);

        CY_INT_PRIORITY_PTR[number & CY_INT_NUMBER_MASK] =
                    (priority & CY_INT_PRIORITY_MASK) << 5;
    }


    /*******************************************************************************
    * Function Name: CyIntGetPriority
    ****************************************************************************//**
    *
    *  Gets the Priority of the Interrupt.
    *
    *  \param number: The number of the interrupt, 0 - 31.
    *
    * \return
    *  Priority of the interrupt. 0 - 7, 0 being the highest.
    *
    *******************************************************************************/
    uint8 CyIntGetPriority(uint8 number)
    {
        uint8 priority;

        CYASSERT(number <= CY_INT_NUMBER_MAX);

        priority = CY_INT_PRIORITY_PTR[number & CY_INT_NUMBER_MASK] >> 5;

        return (priority);
    }


    /*******************************************************************************
    * Function Name: CyIntGetState
    ****************************************************************************//**
    *
    *   Gets the enable state of the specified interrupt number.
    *
    *   \param number: Valid range [0-31].  Interrupt number.
    *
    * \return
    *   Enable status: 1 if enabled, 0 if disabled
    *
    *******************************************************************************/
    uint8 CyIntGetState(uint8 number)
    {
        reg8 * stateReg;

        CYASSERT(number <= CY_INT_NUMBER_MAX);

        /* Get pointer to Interrupt enable register. */
        stateReg = CY_INT_ENABLE_PTR + ((number & CY_INT_NUMBER_MASK) >> 3u);

        /* Get state of interrupt. */
        return ((0u != (*stateReg & ((uint8)(1u << (0x07u & number))))) ? ((uint8)(1u)) : ((uint8)(0u)));
    }

#endif  /* (CY_PSOC5) */


#if(CYDEV_VARIABLE_VDDA == 1)

    /*******************************************************************************
    * Function Name: CySetScPumps
    ****************************************************************************//**
    *
    *  If 1 is passed as a parameter:
    *   - if any of the SC blocks are used - enable pumps for the SC blocks and
    *     start boost clock.
    *   - For each enabled SC block set a boost clock index and enable the boost
    *     clock.
    *
    *  If non-1 value is passed as a parameter:
    *   - If all SC blocks are not used - disable pumps for the SC blocks and
    *     stop the boost clock.
    *   - For each enabled SC block clear the boost clock index and disable the
    *     boost clock.
    *
    *  The global variable CyScPumpEnabled is updated to be equal to passed the
    *  parameter.
    *
    *   \param uint8 enable: Enable/disable SC pumps and the boost clock for the enabled
    *                 \param SC block:
    *                 1 - Enable
    *                 0 - Disable
    *
    *******************************************************************************/
    void CySetScPumps(uint8 enable)
    {
        if(1u == enable)
        {
            /* The SC pumps should be enabled */
            CyScPumpEnabled = 1u;
            /* Enable pumps if any of SC blocks are used */
            if(0u != (CY_LIB_ACT_CFG9_REG & CY_LIB_ACT_CFG9_SWCAPS_MASK))
            {
                CY_LIB_SC_MISC_REG |= CY_LIB_SC_MISC_PUMP_FORCE;
                CyScBoostClk_Start();
            }
            /* Set positive pump for each enabled SC block: set clock index and enable it */
            if(0u != (CY_LIB_ACT_CFG9_REG & CY_LIB_ACT_CFG9_SWCAP0_EN))
            {
                CY_LIB_SC0_BST_REG = (CY_LIB_SC0_BST_REG & CY_LIB_SC_BST_CLK_INDEX_MASK) | CyScBoostClk__INDEX;
                CY_LIB_SC0_BST_REG |= CY_LIB_SC_BST_CLK_EN;
            }
            if(0u != (CY_LIB_ACT_CFG9_REG & CY_LIB_ACT_CFG9_SWCAP1_EN))
            {
                CY_LIB_SC1_BST_REG = (CY_LIB_SC1_BST_REG & CY_LIB_SC_BST_CLK_INDEX_MASK) | CyScBoostClk__INDEX;
                CY_LIB_SC1_BST_REG |= CY_LIB_SC_BST_CLK_EN;
            }
            if(0u != (CY_LIB_ACT_CFG9_REG & CY_LIB_ACT_CFG9_SWCAP2_EN))
            {
                CY_LIB_SC2_BST_REG = (CY_LIB_SC2_BST_REG & CY_LIB_SC_BST_CLK_INDEX_MASK) | CyScBoostClk__INDEX;
                CY_LIB_SC2_BST_REG |= CY_LIB_SC_BST_CLK_EN;
            }
            if(0u != (CY_LIB_ACT_CFG9_REG & CY_LIB_ACT_CFG9_SWCAP3_EN))
            {
                CY_LIB_SC3_BST_REG = (CY_LIB_SC3_BST_REG & CY_LIB_SC_BST_CLK_INDEX_MASK) | CyScBoostClk__INDEX;
                CY_LIB_SC3_BST_REG |= CY_LIB_SC_BST_CLK_EN;
            }
        }
        else
        {
            /* The SC pumps should be disabled */
            CyScPumpEnabled = 0u;
            /* Disable pumps for all SC blocks and stop boost clock */
            CY_LIB_SC_MISC_REG &= ((uint8)(~CY_LIB_SC_MISC_PUMP_FORCE));
            CyScBoostClk_Stop();
            /* Disable boost clock and clear clock index for each SC block */
            CY_LIB_SC0_BST_REG &= ((uint8)(~CY_LIB_SC_BST_CLK_EN));
            CY_LIB_SC0_BST_REG = CY_LIB_SC0_BST_REG & CY_LIB_SC_BST_CLK_INDEX_MASK;
            CY_LIB_SC1_BST_REG &= ((uint8)(~CY_LIB_SC_BST_CLK_EN));
            CY_LIB_SC1_BST_REG = CY_LIB_SC1_BST_REG & CY_LIB_SC_BST_CLK_INDEX_MASK;
            CY_LIB_SC2_BST_REG &= ((uint8)(~CY_LIB_SC_BST_CLK_EN));
            CY_LIB_SC2_BST_REG = CY_LIB_SC2_BST_REG & CY_LIB_SC_BST_CLK_INDEX_MASK;
            CY_LIB_SC3_BST_REG &= ((uint8)(~CY_LIB_SC_BST_CLK_EN));
            CY_LIB_SC3_BST_REG = CY_LIB_SC3_BST_REG & CY_LIB_SC_BST_CLK_INDEX_MASK;
        }
    }

#endif  /* (CYDEV_VARIABLE_VDDA == 1) */


#if(CY_PSOC5)
    /*******************************************************************************
    * Function Name: CySysTickStart
    ****************************************************************************//**
    *
    *  Configures the SysTick timer to generate interrupt every 1 ms by call to the
    *  CySysTickInit() function and starts it by calling CySysTickEnable() function.
    *  Refer to the corresponding function description for the details.

    * \sideeffect
    *  Clears SysTick count flag if it was set
    *
    *******************************************************************************/
    void CySysTickStart(void)
    {
        if (0u == CySysTickInitVar)
        {
            CySysTickInit();
            CySysTickInitVar = 1u;
        }

        CySysTickEnable();
    }


    /*******************************************************************************
    * Function Name: CySysTickInit
    ****************************************************************************//**
    *
    *  Initializes the callback addresses with pointers to NULL, associates the
    *  SysTick system vector with the function that is responsible for calling
    *  registered callback functions, configures SysTick timer to generate interrupt
    * every 1 ms.
    *
    * \sideeffect
    *  Clears SysTick count flag if it was set.
    *
    *  The 1 ms interrupt interval is configured based on the frequency determined
    *  by PSoC Creator at build time. If System clock frequency is changed in
    *  runtime, the CyDelayFreq() with the appropriate parameter should be called.
    *
    *******************************************************************************/
    void CySysTickInit(void)
    {
        uint32 i;

        for (i = 0u; i<CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
        {
            CySysTickCallbacks[i] = (void *) 0;
        }

        (void) CyIntSetSysVector(CY_INT_SYSTICK_IRQN, &CySysTickServiceCallbacks);
        CySysTickSetClockSource(CY_SYS_SYST_CSR_CLK_SRC_SYSCLK);
        CySysTickSetReload(cydelay_freq_hz/1000u);
        CySysTickClear();
        CyIntEnable(CY_INT_SYSTICK_IRQN);
    }


    /*******************************************************************************
    * Function Name: CySysTickEnable
    ****************************************************************************//**
    *
    *  Enables the SysTick timer and its interrupt.
    *
    * \sideeffect
    *  Clears SysTick count flag if it was set
    *
    *******************************************************************************/
    void CySysTickEnable(void)
    {
        CySysTickEnableInterrupt();
        CY_SYS_SYST_CSR_REG |= CY_SYS_SYST_CSR_ENABLE;
    }


    /*******************************************************************************
    * Function Name: CySysTickStop
    ****************************************************************************//**
    *
    *  Stops the system timer (SysTick).
    *
    * \sideeffect
    *  Clears SysTick count flag if it was set
    *
    *******************************************************************************/
    void CySysTickStop(void)
    {
        CY_SYS_SYST_CSR_REG &= ((uint32) ~(CY_SYS_SYST_CSR_ENABLE));
    }


    /*******************************************************************************
    * Function Name: CySysTickEnableInterrupt
    ****************************************************************************//**
    *
    *  Enables the SysTick interrupt.
    *
    * \sideeffect
    *  Clears SysTick count flag if it was set
    *
    *******************************************************************************/
    void CySysTickEnableInterrupt(void)
    {
        CY_SYS_SYST_CSR_REG |= CY_SYS_SYST_CSR_ENABLE_INT;
    }


    /*******************************************************************************
    * Function Name: CySysTickDisableInterrupt
    ****************************************************************************//**
    *
    *  Disables the SysTick interrupt.
    *
    * \sideeffect
    *  Clears SysTick count flag if it was set
    *
    *******************************************************************************/
    void CySysTickDisableInterrupt(void)
    {
        CY_SYS_SYST_CSR_REG &= ((uint32) ~(CY_SYS_SYST_CSR_ENABLE_INT));
    }


    /*******************************************************************************
    * Function Name: CySysTickSetReload
    ****************************************************************************//**
    *
    *  Sets value the counter is set to on startup and after it reaches zero. This
    *  function do not change or reset current sysTick counter value, so it should
    *  be cleared using CySysTickClear() API.
    *
    *  \param value: Valid range [0x0-0x00FFFFFF]. Counter reset value.
    *
    *******************************************************************************/
    void CySysTickSetReload(uint32 value)
    {
        CY_SYS_SYST_RVR_REG = (value & CY_SYS_SYST_RVR_CNT_MASK);
    }


    /*******************************************************************************
    * Function Name: CySysTickGetReload
    ****************************************************************************//**
    *
    *  Sets value the counter is set to on startup and after it reaches zero.
    *
    * \return
    *  Counter reset value
    *
    *******************************************************************************/
    uint32 CySysTickGetReload(void)
    {
        return(CY_SYS_SYST_RVR_REG & CY_SYS_SYST_RVR_CNT_MASK);
    }


    /*******************************************************************************
    * Function Name: CySysTickGetValue
    ****************************************************************************//**
    *
    *  Gets current SysTick counter value.
    *
    * \return
    *  Current SysTick counter value
    *
    *******************************************************************************/
    uint32 CySysTickGetValue(void)
    {
        return(CY_SYS_SYST_CVR_REG & CY_SYS_SYST_CVR_CNT_MASK);
    }


    /*******************************************************************************
    * Function Name: CySysTickSetClockSource
    ****************************************************************************//**
    *
    *  Sets the clock source for the SysTick counter.
    *
    *  \param clockSource: Clock source for SysTick counter
    *         Define                     Clock Source
    *   CY_SYS_SYST_CSR_CLK_SRC_SYSCLK     SysTick is clocked by CPU clock.
    *   CY_SYS_SYST_CSR_CLK_SRC_LFCLK      SysTick is clocked by the low frequency
    *                                      clock (ILO 100 KHz for PSoC 5LP, and
    *                                      LFCLK for PSoC 4).
    *
    * \sideeffect
    *  Clears SysTick count flag if it was set. If clock source is not ready this
    *  function call will have no effect. After changing clock source to the low
    *  frequency clock the counter and reload register values will remain unchanged
    *  so time to the interrupt will be significantly bigger and vice versa.
    *
    *******************************************************************************/
    void CySysTickSetClockSource(uint32 clockSource)
    {
        if (clockSource == CY_SYS_SYST_CSR_CLK_SRC_SYSCLK)
        {
            CY_SYS_SYST_CSR_REG |= (uint32)(CY_SYS_SYST_CSR_CLK_SRC_SYSCLK << CY_SYS_SYST_CSR_CLK_SOURCE_SHIFT);
        }
        else
        {
            CY_SYS_SYST_CSR_REG &= ((uint32) ~((uint32)(CY_SYS_SYST_CSR_CLK_SRC_SYSCLK << CY_SYS_SYST_CSR_CLK_SOURCE_SHIFT)));
        }
    }


    /*******************************************************************************
    * Function Name: CySysTickGetClockSource
    ****************************************************************************//**
    *
    *  Returns the current clock source of the SysTick counter.
    *
    *  \return
    *   CY_SYS_SYST_CSR_CLK_SRC_SYSCLK     SysTick is clocked by CPU clock.
    *   CY_SYS_SYST_CSR_CLK_SRC_LFCLK      SysTick is clocked by the low frequency
    *                                      clock. (ILO 100 KHz for PSoC 5LP, and
    *                                      LFCLK for PSoC 4).
    *******************************************************************************/
    uint32 CySysTickGetClockSource(void)
    {
        return ((CY_SYS_SYST_CSR_REG >> CY_SYS_SYST_CSR_CLK_SOURCE_SHIFT) & CY_SYS_SYST_CSR_CLK_SRC_SYSCLK );
    }


    /*******************************************************************************
    * Function Name: CySysTickGetCountFlag
    ****************************************************************************//**
    *
    *  The count flag is set once SysTick counter reaches zero.
    *   The flag cleared on read.
    *
    * \return
    *  Returns non-zero value if flag is set, otherwise zero is returned.
    *
    *
    * \sideeffect
    *  Clears SysTick count flag if it was set.
    *
    *******************************************************************************/
    uint32 CySysTickGetCountFlag(void)
    {
        return ((CY_SYS_SYST_CSR_REG >> CY_SYS_SYST_CSR_COUNTFLAG_SHIFT) & 0x01u);
    }


    /*******************************************************************************
    * Function Name: CySysTickClear
    ****************************************************************************//**
    *
    *  Clears the SysTick counter for well-defined startup.
    *
    *******************************************************************************/
    void CySysTickClear(void)
    {
        CY_SYS_SYST_CVR_REG = 0u;
    }


    /*******************************************************************************
    * Function Name: CySysTickSetCallback
    ****************************************************************************//**
    *
    *  This function allows up to five user-defined interrupt service routine
    *  functions to be associated with the SysTick interrupt. These are specified
    *  through the use of pointers to the function.
    *
    *  To set a custom callback function without the overhead of the system provided
    *  one, use CyIntSetSysVector(CY_INT_SYSTICK_IRQN, cyisraddress <address>),
    *  where <address> is address of the custom defined interrupt service routine.
    *  Note: a custom callback function overrides the system defined callback
    *  functions.
    *
    *  \param number: The number of the callback function addresses to be set. The valid
    *          range is from 0 to 4.
    *
    *  void(*CallbackFunction(void): A pointer to the function that will be
    *                                associated with the SysTick ISR for the
    *                                specified number.
    *
    * \return
    *  Returns the address of the previous callback function.
    *  The NULL is returned if the specified address in not set.
    *
    * \sideeffect
    *  The registered callback functions will be executed in the interrupt.
    *
    *******************************************************************************/
    cySysTickCallback CySysTickSetCallback(uint32 number, cySysTickCallback function)
    {
        cySysTickCallback retVal;

        retVal = CySysTickCallbacks[number];
        CySysTickCallbacks[number] = function;
        return (retVal);
    }


    /*******************************************************************************
    * Function Name: CySysTickGetCallback
    ****************************************************************************//**
    *
    *  The function get the specified callback pointer.
    *
    *  \param number: The number of callback function address to get. The valid
    *          range is from 0 to 4.
    *
    * \return
    *  Returns the address of the specified callback function.
    *  The NULL is returned if the specified address in not initialized.
    *
    *******************************************************************************/
    cySysTickCallback CySysTickGetCallback(uint32 number)
    {
        return ((cySysTickCallback) CySysTickCallbacks[number]);
    }


    /*******************************************************************************
    * Function Name: CySysTickServiceCallbacks
    ****************************************************************************//**
    *
    *  System Tick timer interrupt routine
    *
    *******************************************************************************/
    static void CySysTickServiceCallbacks(void)
    {
        uint32 i;

        /* Verify that tick timer flag was set */
        if (1u == CySysTickGetCountFlag())
        {
            for (i=0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
            {
                if (CySysTickCallbacks[i] != (void *) 0)
                {
                    (void)(CySysTickCallbacks[i])();
                }
            }
        }
    }
#endif /* (CY_PSOC5) */


/*******************************************************************************
* Function Name: CyGetUniqueId
****************************************************************************//**
*
*  Returns the 64-bit unique ID of the device. The uniqueness of the number is
*  guaranteed for 10 years due to the die lot number having a cycle life of 10
*  years and even after 10 years, the probability of getting two identical
*  numbers is very small.
*
*  \param uniqueId: The pointer to a two element 32-bit unsigned integer array. Returns
*  the 64-bit unique ID of the device by loading them into the integer array
*  pointed to by uniqueId.
*
*******************************************************************************/
void CyGetUniqueId(uint32* uniqueId)
{
#if(CY_PSOC4)
    uniqueId[0u]  =  (uint32)(* (reg8 *) CYREG_SFLASH_DIE_LOT0  );
    uniqueId[0u] |= ((uint32)(* (reg8 *) CYREG_SFLASH_DIE_LOT1  ) <<  8u);
    uniqueId[0u] |= ((uint32)(* (reg8 *) CYREG_SFLASH_DIE_LOT2  ) << 16u);
    uniqueId[0u] |= ((uint32)(* (reg8 *) CYREG_SFLASH_DIE_WAFER ) << 24u);

    uniqueId[1u]  =  (uint32)(* (reg8 *) CYREG_SFLASH_DIE_X     );
    uniqueId[1u] |= ((uint32)(* (reg8 *) CYREG_SFLASH_DIE_Y     ) <<  8u);
    uniqueId[1u] |= ((uint32)(* (reg8 *) CYREG_SFLASH_DIE_SORT  ) << 16u);
    uniqueId[1u] |= ((uint32)(* (reg8 *) CYREG_SFLASH_DIE_MINOR ) << 24u);
#else
    uniqueId[0u]  =  (uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_FLSHID_CUST_TABLES_LOT_LSB   ));
    uniqueId[0u] |= ((uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_FLSHID_CUST_TABLES_LOT_MSB   )) <<  8u);
    uniqueId[0u] |= ((uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_MLOGIC_REV_ID                )) << 16u);
    uniqueId[0u] |= ((uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_FLSHID_CUST_TABLES_WAFER_NUM )) << 24u);

    uniqueId[1u]  =  (uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_FLSHID_CUST_TABLES_X_LOC     ));
    uniqueId[1u] |= ((uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_FLSHID_CUST_TABLES_Y_LOC     )) <<  8u);
    uniqueId[1u] |= ((uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_FLSHID_CUST_TABLES_WRK_WK    )) << 16u);
    uniqueId[1u] |= ((uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_FLSHID_CUST_TABLES_FAB_YR    )) << 24u);
#endif  /* (CY_PSOC4) */
}

/* [] END OF FILE */
