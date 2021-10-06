/*******************************************************************************
* @file    usbd_PSoC5LP.c
* @brief   PSoC5LP usbd API implementation
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

#include "RTL.h"
#include "usb_def.h"
#include "usb_msc.h"
#include "usbd_core.h"
#include "usbd_hw.h"
#include "usbd_msc.h"
#include "usbd_event.h"
#include "usbd_hid.h"
#include "USBFS_Dp.h"
#include "usbd_PSoC5LP.h"
#include "info.h"
#include "usbd_cdc_acm.h"
#include "UART_Bridge.h"


#include "rl_usb.h"
#include "vfs_manager.h"

#define EP_NOT_FOUND                (0xFFFFFFFFu)

static volatile uint8 USBFS_EP_INT_Pending[USBFS_MAX_EP];
static volatile bool USBFS_BUS_RESET = false;
static volatile bool USBFS_SOF = false;

static uint8 USBFS_initVar = 0u;
static uint16 buffCount = 0u;
static volatile uint8  USBFS_ep0Count;
static volatile uint8  USBFS_ep0Mode;
static volatile uint8  USBFS_ep0Toggle;

static volatile USBFS_EP_CTL_BLOCK USBFS_EP[USBFS_MAX_EP];

/*******************************************************************************
* Function Name: IncEPReq
****************************************************************************//**
*
*  This function increments the pending event counter at the USB endpoint.
*
* \param epNumber Data endpoint number
*
*******************************************************************************/
static void IncEPReq(uint8 epNumber)
{
    if (USBFS_EP_INT_Pending[epNumber] < 0xFFu)
    {
        USBFS_EP_INT_Pending[epNumber]++;
    }
}

/*******************************************************************************
* Function Name: ClearEPReq
****************************************************************************//**
*
*  This function clears the pending event counter at the USB endpoint.
*
* \param epNumber Data endpoint number
*
*******************************************************************************/
static void ClearEPReq(uint8 epNumber)
{
    USBFS_EP_INT_Pending[epNumber] = 0;
}

/*******************************************************************************
* Function Name: FwEPToHwEP
****************************************************************************//**
*
*  This function translates the USB endpoint number into an index
*  in the list of endpoints
*
* \param epNumber Data endpoint number
*
* \return
*  Returns the index in the list of endpoints
*
********************************************************************************/
static uint32 FwEPToHwEP(uint32 FwEP)
{
    uint32 HwEP = FwEP & (~((uint32)USBFS_DIR_IN));

    if ( HwEP >= USBFS_MAX_EP )
    {
        HwEP = EP_NOT_FOUND;
    }
    return HwEP;
}

/*******************************************************************************
* Function Name: USBFS_ReInitComponent
****************************************************************************//**
*
*  This function reinitialize the component configuration and is
*  intend to be called from the Reset interrupt.
*
* \reentrant
*  No.
*
*******************************************************************************/
static void USBFS_ReInitComponent(void)
{
    /* Set EP0.CR: ACK Setup, STALL IN/OUT. */
    USBFS_EP0_CR_REG = USBFS_MODE_STALL_IN_OUT;

    /* Enable device to respond to USB traffic with address 0. */
    USBFS_CR0_REG = USBFS_DEFUALT_CR0;
}

/*******************************************************************************
* Function Name: USBFS_BUS_RESET_ISR
****************************************************************************//**
*
*  USB Bus Reset Interrupt Service Routine.
*
*******************************************************************************/
static CY_ISR(USBFS_BUS_RESET_ISR)
{
    USBFS_ClearSieInterruptSource(USBFS_INTR_SIE_BUS_RESET_INTR);

    USBFS_ReInitComponent();

    USBFS_BUS_RESET = true;
    USBD_SignalHandler();
}

/*******************************************************************************
* Function Name: USBFS_DP_ISR
****************************************************************************//**
*
*  This Interrupt Service Routine handles DP pin changes for wake-up from
*  the sleep mode.
*
*******************************************************************************/
static CY_ISR(USBFS_DP_ISR)
{
    (void) USBFS_Dp_ClearInterrupt();
}

/******************************************************************************
* Function Name: USBFS_EP_X_ISR
***************************************************************************//**
*
*  Common part of Endpoint Interrupt Service Routine
*
* \param epNumber Endpoint number
*
******************************************************************************/
static void USBFS_EP_X_ISR(uint8 epNumber)
{
    if (epNumber < USBFS_MAX_EP)
    {
        const static uint8 EpInterruptSource[USBFS_MAX_EP] = {
            [0u] = (uint8)USBFS_INTR_SIE_EP0_INTR,
            [1u] = (uint8)USBFS_SIE_INT_EP1_INTR,
            [2u] = (uint8)USBFS_SIE_INT_EP2_INTR,
            [3u] = (uint8)USBFS_SIE_INT_EP3_INTR,
            [4u] = (uint8)USBFS_SIE_INT_EP4_INTR,
            [5u] = (uint8)USBFS_SIE_INT_EP5_INTR,
            [6u] = (uint8)USBFS_SIE_INT_EP6_INTR,
            [7u] = (uint8)USBFS_SIE_INT_EP7_INTR,
            [8u] = (uint8)USBFS_SIE_INT_EP8_INTR,
        };
        if (epNumber == USBFS_EP0)
        {
            USBFS_ClearSieInterruptSource(EpInterruptSource[epNumber]);
        }
        else
        {
            USBFS_ClearSieEpInterruptSource(EpInterruptSource[epNumber]);

            /* Notifies user that transfer IN or OUT transfer is completed.
            * IN endpoint: endpoint buffer can be reloaded, Host is read data.
            * OUT endpoint: data is ready to be read from endpoint buffer.
            */

            /* Read CR0 register to clear SIE lock. */
            (void) USBFS_SIE_EP_BASE.sieEp[epNumber].epCr0;

            /* Toggle all endpoint types except ISOC. */
            if (USBFS_GET_EP_TYPE(epNumber) != USBFS_EP_TYPE_ISOC)
            {
                USBFS_EP[epNumber].epToggle ^= USBFS_EPX_CNT_DATA_TOGGLE;
            }
            USBFS_EP[epNumber].apiEpState = USBFS_EVENT_PENDING;
        }
        IncEPReq(epNumber);
        USBD_SignalHandler();
    }
}

/*******************************************************************************
* Function Name: USBFS_ep_0_Interrupt
****************************************************************************//**
*
*  This Interrupt Service Routine handles Endpoint 0 (Control Pipe) traffic.
*
*******************************************************************************/
static CY_ISR(USBFS_EP_0_ISR)
{
    USBFS_EP_X_ISR(USBFS_EP0);
}

/******************************************************************************
* Function Name: USBFS_EP_1_ISR
***************************************************************************//**
*
*  Endpoint 1 Interrupt Service Routine
*
******************************************************************************/
static CY_ISR(USBFS_EP_1_ISR)
{
    USBFS_EP_X_ISR(USBFS_EP1);
}


/*******************************************************************************
* Function Name: USBFS_EP_2_ISR
****************************************************************************//**
*
*  Endpoint 2 Interrupt Service Routine.
*
*******************************************************************************/
static CY_ISR(USBFS_EP_2_ISR)
{
    USBFS_EP_X_ISR(USBFS_EP2);
}

/*******************************************************************************
* Function Name: USBFS_EP_3_ISR
****************************************************************************//**
*
*  Endpoint 3 Interrupt Service Routine.
*
*******************************************************************************/
static CY_ISR(USBFS_EP_3_ISR)
{
    USBFS_EP_X_ISR(USBFS_EP3);
}

/*******************************************************************************
* Function Name: USBFS_EP_4_ISR
****************************************************************************//**
*
*  Endpoint 4 Interrupt Service Routine.
*
*******************************************************************************/
static CY_ISR(USBFS_EP_4_ISR)
{
    USBFS_EP_X_ISR(USBFS_EP4);
}

/*******************************************************************************
* Function Name: USBFS_EP_5_ISR
****************************************************************************//**
*
*  Endpoint 5 Interrupt Service Routine
*
*******************************************************************************/
static CY_ISR(USBFS_EP_5_ISR)
{
    USBFS_EP_X_ISR(USBFS_EP5);
}

/*******************************************************************************
* Function Name: USBFS_EP_6_ISR
****************************************************************************//**
*
*  Endpoint 6 Interrupt Service Routine.
*
*******************************************************************************/
static CY_ISR(USBFS_EP_6_ISR)
{
    USBFS_EP_X_ISR(USBFS_EP6);
}

/*******************************************************************************
* Function Name: USBFS_EP_7_ISR
****************************************************************************//**
*
*  Endpoint 7 Interrupt Service Routine.
*
*******************************************************************************/
static CY_ISR(USBFS_EP_7_ISR)
{
    USBFS_EP_X_ISR(USBFS_EP7);
}

/*******************************************************************************
* Function Name: USBFS_SOF_ISR
****************************************************************************//**
*
*  Start of Frame Interrupt Service Routine.
*
*******************************************************************************/
static CY_ISR(USBFS_SOF_ISR)
{
    USBFS_ClearSieInterruptSource(USBFS_INTR_SIE_SOF_INTR);

    USBFS_SOF = true;
    USBD_SignalHandler();
}

/*******************************************************************************
* Function Name: USBFS_Init
****************************************************************************//**
*
* This function initializes USBFS component.
*
* \reentrant
*  No.
*
*******************************************************************************/
static void USBFS_Init(void)
{
    uint8 enableInterrupts = CyEnterCriticalSection();

    /* Enable USB block. */
    USBFS_PM_ACT_CFG_REG |= USBFS_PM_ACT_EN_FSUSB;
    /* Enable USB block for Standby Power Mode. */
    USBFS_PM_STBY_CFG_REG |= USBFS_PM_STBY_EN_FSUSB;

    /* Enable core clock. */
    USBFS_USB_CLK_EN_REG = USBFS_USB_CLK_ENABLE;

    USBFS_CR1_REG = USBFS_CR1_ENABLE_LOCK;

    /* ENABLING USBIO PADS IN USB MODE FROM I/O MODE. */
    /* Ensure USB transmit enable is low (USB_USBIO_CR0.ten). - Manual Transmission - Disabled. */
    USBFS_USBIO_CR0_REG &= (uint8) ~USBFS_USBIO_CR0_TEN;
    CyDelayUs(USBFS_WAIT_REG_STABILITY_50NS);  /* ~50ns delay. */
    /* Disable USBIO by asserting PM.USB_CR0.fsusbio_pd_n(Inverted.
    *  high. These bits will be set low by the power manager out-of-reset.
    *  Also confirm USBIO pull-up is disabled.
    */
    USBFS_PM_USB_CR0_REG &= (uint8) ~(USBFS_PM_USB_CR0_PD_N |
                                                 USBFS_PM_USB_CR0_PD_PULLUP_N);

    /* Select IOMODE to USB mode. */
    USBFS_USBIO_CR1_REG &= (uint8) ~USBFS_USBIO_CR1_IOMODE;

    /* Enable USBIO reference by setting PM.USB_CR0.fsusbio_ref_en. */
    USBFS_PM_USB_CR0_REG |= USBFS_PM_USB_CR0_REF_EN;
    /* Reference is available for 1us after regulator is enabled. */
    CyDelayUs(USBFS_WAIT_REG_STABILITY_1US);
    /* OR 40us after power is restored. */
    CyDelayUs(USBFS_WAIT_VREF_RESTORE);
    /* Ensure single-ended disable bits are low (PRT15.INP_DIS[7:6])(input receiver enabled). */
    USBFS_DM_INP_DIS_REG &= (uint8) ~USBFS_DM_MASK;
    USBFS_DP_INP_DIS_REG &= (uint8) ~USBFS_DP_MASK;

    /* Enable USBIO. */
    USBFS_PM_USB_CR0_REG |= USBFS_PM_USB_CR0_PD_N;
    CyDelayUs(USBFS_WAIT_PD_PULLUP_N_ENABLE);
    /* Set USBIO pull-up enable. */
    USBFS_PM_USB_CR0_REG |= USBFS_PM_USB_CR0_PD_PULLUP_N;

    /* Reset Arbiter Write Address register for endpoint 1. */
    CY_SET_REG8(USBFS_ARB_RW1_WA_PTR,     0u);
    CY_SET_REG8(USBFS_ARB_RW1_WA_MSB_PTR, 0u);

    CyExitCriticalSection(enableInterrupts);

    /* Configure interrupts from USB block. */
    /* Set bus reset interrupt. */
    CyIntSetPriority(USBFS_BUS_RESET_VECT_NUM, USBFS_BUS_RESET_PRIOR);
    (void) CyIntSetVector(USBFS_BUS_RESET_VECT_NUM,   &USBFS_BUS_RESET_ISR);

    /* Set Control Endpoint Interrupt. */
    CyIntSetPriority(USBFS_EP_0_VECT_NUM, USBFS_EP_0_PRIOR);
    (void) CyIntSetVector(USBFS_EP_0_VECT_NUM,   &USBFS_EP_0_ISR);

    /* Set SOF interrupt. */
    CyIntSetPriority     (USBFS_SOF_VECT_NUM,  USBFS_SOF_PRIOR);
    (void) CyIntSetVector(USBFS_SOF_VECT_NUM, &USBFS_SOF_ISR);

    /* Set Data Endpoint 1 Interrupt. */
    CyIntSetPriority     (USBFS_EP_1_VECT_NUM,  USBFS_EP_1_PRIOR);
    (void) CyIntSetVector(USBFS_EP_1_VECT_NUM,  &USBFS_EP_1_ISR);

    /* Set Data Endpoint 2 Interrupt. */
    CyIntSetPriority     (USBFS_EP_2_VECT_NUM,  USBFS_EP_2_PRIOR);
    (void) CyIntSetVector(USBFS_EP_2_VECT_NUM, &USBFS_EP_2_ISR);

    /* Set Data Endpoint 3 Interrupt. */
    CyIntSetPriority     (USBFS_EP_3_VECT_NUM,  USBFS_EP_3_PRIOR);
    (void) CyIntSetVector(USBFS_EP_3_VECT_NUM, &USBFS_EP_3_ISR);

    /* Set Data Endpoint 4 Interrupt. */
    CyIntSetPriority     (USBFS_EP_4_VECT_NUM,  USBFS_EP_4_PRIOR);
    (void) CyIntSetVector(USBFS_EP_4_VECT_NUM, &USBFS_EP_4_ISR);

    /* Set Data Endpoint 5 Interrupt. */
    CyIntSetPriority     (USBFS_EP_5_VECT_NUM,  USBFS_EP_5_PRIOR);
    (void) CyIntSetVector(USBFS_EP_5_VECT_NUM, &USBFS_EP_5_ISR);

    /* Set Data Endpoint 6 Interrupt. */
    CyIntSetPriority     (USBFS_EP_6_VECT_NUM,  USBFS_EP_6_PRIOR);
    (void) CyIntSetVector(USBFS_EP_6_VECT_NUM, &USBFS_EP_6_ISR);

     /* Set Data Endpoint 7 Interrupt. */
    CyIntSetPriority     (USBFS_EP_7_VECT_NUM,  USBFS_EP_7_PRIOR);
    (void) CyIntSetVector(USBFS_EP_7_VECT_NUM, &USBFS_EP_7_ISR);

    /* Common: Configure GPIO interrupt for wakeup. */
    CyIntSetPriority     (USBFS_DP_INTC_VECT_NUM,  USBFS_DP_INTC_PRIORITY);
    (void) CyIntSetVector(USBFS_DP_INTC_VECT_NUM, &USBFS_DP_ISR);
}

/*******************************************************************************
* Function Name: USBFS_InitComponent
****************************************************************************//**
*
*   This function initializes the component’s global variables and initiates
*   communication with the host by pull-up D+ line.
*
* \param device:
*   Contains the device number of the desired device descriptor. The device
*   number can be found in the Device Descriptor Tab of "Configure" dialog,
*   under the settings of desired Device Descriptor, in the *Device Number*
*   field.
*  \param mode:
*   The operating voltage. This determines whether the voltage regulator
*   is enabled for 5V operation or if pass through mode is used for 3.3V
*   operation. Symbolic names and their associated values are given in the
*   following list.
*
*       *USBFS_3V_OPERATION* - Disable voltage regulator and pass-
*                                      through Vcc for pull-up
*
*       *USBFS_5V_OPERATION* - Enable voltage regulator and use
*                                      regulator for pull-up
*
*       *USBFS_DWR_POWER_OPERATION* - Enable or disable the voltage
*                                      regulator depending on the power supply
*                                      voltage configuration in the DWR tab.
*                                      For PSoC 3/5LP devices, the VDDD supply
*                                      voltage is considered and for PSoC 4A-L,
*                                      the VBUS supply voltage is considered.
*
* \reentrant
*  No.
*
*******************************************************************************/
static void USBFS_InitComponent(uint8 mode)
{
    /* Enable component interrupts. */
    CyIntEnable(USBFS_BUS_RESET_VECT_NUM);
    CyIntEnable(USBFS_EP_0_VECT_NUM);
    CyIntEnable(USBFS_SOF_VECT_NUM);
    CyIntEnable(USBFS_EP_1_VECT_NUM);
    CyIntEnable(USBFS_EP_2_VECT_NUM);
    CyIntEnable(USBFS_EP_3_VECT_NUM);
    CyIntEnable(USBFS_EP_4_VECT_NUM);
    CyIntEnable(USBFS_EP_5_VECT_NUM);
    CyIntEnable(USBFS_EP_6_VECT_NUM);
    CyIntEnable(USBFS_EP_7_VECT_NUM);

    /* Enable USB regulator depends on operation voltage. IMO Locking is enabled in Init(). */
    switch(mode)
    {
    case USBFS_3V_OPERATION:
        /* Disable regulator for 3V operation. */
        USBFS_CR1_REG &= (uint8) ~USBFS_CR1_REG_ENABLE;
        break;

    case USBFS_5V_OPERATION:
        /* Enable regulator for 5V operation. */
        USBFS_CR1_REG |= (uint8)  USBFS_CR1_REG_ENABLE;
        break;

    default:
        /* Enable regulator for 5V operation. */
        USBFS_CR1_REG |= (uint8)  USBFS_CR1_REG_ENABLE;
        break;
    }

    /* Set EP0.CR: ACK Setup, STALL IN/OUT. */
    USBFS_EP0_CR_REG = USBFS_MODE_STALL_IN_OUT;

    /* Enable device to respond to USB traffic with address 0. */
    USBFS_CR0_REG = USBFS_DEFUALT_CR0;
    CyDelayCycles(USBFS_WAIT_CR0_REG_STABILITY);

    /* Enable D+ pull-up and keep USB control on IO. */
    USBFS_USBIO_CR1_REG = USBFS_USBIO_CR1_USBPUEN;
}

/*******************************************************************************
* Function Name: USBFS_Stop
****************************************************************************//**
*
*  This function shuts down the USB function including to release
*  the D+ pull-up and disabling the SIE.
*
*   \ref USBFS_intiVar -  This variable is set to zero
*
*******************************************************************************/
static void USBFS_Stop(void)
{
    uint8 enableInterrupts = CyEnterCriticalSection();

    /* Disable USB IP to respond to USB traffic. */
    USBFS_CR0_REG &= (uint8) ~USBFS_CR0_ENABLE;

    /* Disable D+ pull-up. */
    USBFS_USBIO_CR1_REG &= (uint8) ~ USBFS_USBIO_CR1_USBPUEN;

    /* Clear power active and standby mode templates. */
    USBFS_PM_ACT_CFG_REG  &= (uint8) ~USBFS_PM_ACT_EN_FSUSB;
    USBFS_PM_STBY_CFG_REG &= (uint8) ~USBFS_PM_STBY_EN_FSUSB;

    /* Ensure single-ended disable bits are high (PRT15.INP_DIS[7:6])
     * (input receiver disabled). */
    USBFS_DM_INP_DIS_REG |= (uint8) USBFS_DM_MASK;
    USBFS_DP_INP_DIS_REG |= (uint8) USBFS_DP_MASK;

    CyExitCriticalSection(enableInterrupts);

    /* Disable component interrupts. */

    CyIntDisable(USBFS_BUS_RESET_VECT_NUM);
    CyIntDisable(USBFS_EP_0_VECT_NUM);
    CyIntDisable(USBFS_SOF_VECT_NUM);
    CyIntDisable(USBFS_EP_1_VECT_NUM);
    CyIntDisable(USBFS_EP_2_VECT_NUM);
    CyIntDisable(USBFS_EP_3_VECT_NUM);
    CyIntDisable(USBFS_EP_4_VECT_NUM);
    CyIntDisable(USBFS_EP_5_VECT_NUM);
    CyIntDisable(USBFS_EP_6_VECT_NUM);
    CyIntDisable(USBFS_EP_7_VECT_NUM);
    /* unused Interrupt.
    CyIntDisable(USBFS_EP_8_VECT_NUM);
    */

    /* Clear active mode Dp interrupt source history. */
    (void) USBFS_Dp_ClearInterrupt();
    CyIntClearPending(USBFS_DP_INTC_VECT_NUM);

    /* It is mandatory for correct device startup. */
    USBFS_initVar = 0u;
}

/*******************************************************************************
* Function Name: USBFS_GetEPState
****************************************************************************//**
*
*  This function returns the state of the requested endpoint.
*
* \param epNumber Data endpoint number
*
* \return
*  Returns the current state of the specified USBFS endpoint. Symbolic names and
*  their associated values are given in the following table. Use these constants
*  whenever you write code to change the state of the endpoints, such as ISR
*  code, to handle data sent or received.
*
*  Return Value           | Description
*  -----------------------|-----------------------------------------------------
*  USBFS_NO_EVENT_PENDING |The endpoint is awaiting SIE action
*  USBFS_EVENT_PENDING    |The endpoint is awaiting CPU action
*  USBFS_NO_EVENT_ALLOWED |The endpoint is locked from access
*  USBFS_IN_BUFFER_FULL   |The IN endpoint is loaded and the mode is set to ACK IN
*  USBFS_IN_BUFFER_EMPTY  |An IN transaction occurred and more data can be loaded
*  USBFS_OUT_BUFFER_EMPTY |The OUT endpoint is set to ACK OUT and is waiting for data
*  USBFS_OUT_BUFFER_FULL  |An OUT transaction has occurred and data can be read
*
*******************************************************************************/
static uint8 USBFS_GetEPState(uint8 epNumber)
{
    return (USBFS_EP[epNumber].apiEpState);
}

/*******************************************************************************
* Function Name: USBFS_GetEPCount
****************************************************************************//**
*
*  This function supports Data Endpoints only(EP1-EP8).
*  Returns the transfer count for the requested endpoint.  The value from
*  the count registers includes 2 counts for the two byte checksum of the
*  packet.  This function subtracts the two counts.
*
*  \param epNumber Data Endpoint Number.
*                   Valid values are between 1 and 8.
*
* \return
*  Returns the current byte count from the specified endpoint or 0 for an
*  invalid endpoint.
*
*******************************************************************************/
static uint16 USBFS_GetEPCount(uint8 epNumber)
{
    uint16 cntr = 0u;

    if ((epNumber > USBFS_EP0) && (epNumber < USBFS_MAX_EP))
    {
        /* Get 11-bits EP counter where epCnt0 - 3 bits MSB and epCnt1 - 8 bits LSB. */
        cntr  = ((uint16) USBFS_SIE_EP_BASE.sieEp[epNumber].epCnt0) & USBFS_EPX_CNT0_MASK;
        cntr  = ((uint16) (cntr << 8u)) | ((uint16) USBFS_SIE_EP_BASE.sieEp[epNumber].epCnt1);
        if (cntr > 0u)
        {
            cntr -= USBFS_EPX_CNTX_CRC_COUNT;
        }
    }

    return (cntr);
}

/*******************************************************************************
* Function Name: USBFS_LoadInEP
****************************************************************************//**
*
*  This function loads and enables the specified USB data endpoint for an IN
*  data transfer.
*
*  \param epNumber Contains the data endpoint number.
*            Valid values are between 1 and 8.
*  \param *pData A pointer to a data array from which the data for the endpoint space
*          is loaded.
*  \param length The number of bytes to transfer from the array and then send as
*          a result of an IN request. Valid values are between 0 and 512
*          (1023 for DMA with Automatic Buffer Management mode). The value 512
*          is applicable if only one endpoint is used.
*
*
* \reentrant
*  No.
*
*******************************************************************************/
static void USBFS_LoadInEP(uint8 epNumber, const uint8 pData[], uint16 dataLength)
{
    if ((epNumber > USBFS_EP0) && (epNumber < USBFS_MAX_EP))
    {
        uint16 length;
        /* Limit length to available buffer USB IP buffer size.*/
        if (dataLength > (USBFS_EPX_DATA_BUF_MAX - USBFS_EP[epNumber].buffOffset))
        {
            length = USBFS_EPX_DATA_BUF_MAX - USBFS_EP[epNumber].buffOffset;
        }
        else
        {
            length = dataLength;
        }

        /* Set count and data toggle. */
        USBFS_SIE_EP_BASE.sieEp[epNumber].epCnt0 = (uint8) HI8(length) | USBFS_EP[epNumber].epToggle;
        USBFS_SIE_EP_BASE.sieEp[epNumber].epCnt1 = (uint8) LO8(length);

        if (NULL != pData)
        {
            /* Copy data using arbiter data register. */
            for (uint16 i = 0u; i < length; ++i)
            {
                USBFS_ARB_EP_BASE.arbEp[epNumber].rwDr = pData[i];
            }
        }

        /* IN endpoint buffer is full - read to be read. */
        USBFS_EP[epNumber].apiEpState = USBFS_NO_EVENT_PENDING;

        /* Arm IN endpoint. */
        USBFS_SIE_EP_BASE.sieEp[epNumber].epCr0 = USBFS_EP[epNumber].epMode;
    }
}

/*******************************************************************************
* Function Name: USBFS_EnableOutEP
****************************************************************************//**
*
*  This function enables the specified endpoint for OUT transfers. Do not call
*  this function for IN endpoints.
*
*  \param epNumber: Contains the data endpoint number. Valid values are between
*  1 and 8.
*
* \globalvars
*
*  \ref USBFS_EP[epNumber].apiEpState - set to NO_EVENT_PENDING
*
* \reentrant
*  No.
*
*******************************************************************************/
static void USBFS_EnableOutEP(uint8 epNumber)
{
    if ((epNumber > USBFS_EP0) && (epNumber < USBFS_MAX_EP))
    {
        USBFS_EP[epNumber].apiEpState = USBFS_NO_EVENT_PENDING;

        /* Enable OUT endpoint to be written by Host. */
        USBFS_SIE_EP_BASE.sieEp[epNumber].epCr0 = USBFS_EP[epNumber].epMode;

    }
}

/*******************************************************************************
* Function Name: USBFS_ReadOutEP
****************************************************************************//**
*
*   This function moves the specified number of bytes from endpoint buffer to
*   system RAM. The number of bytes actually transferred from endpoint buffer to
*   system RAM is the lesser of the actual number of bytes sent by the host or
*   the number of bytes requested by the length parameter.
*
*   \snippet /USBFS_sut_02.cydsn/main.c checking EPstatey
*
*   The USBFS_EnableOutEP() has to be called to allow host to write data into
*   the endpoint buffer after DMA has completed transfer data from OUT endpoint
*   buffer to SRAM.
*
*  \param epNumber: Contains the data endpoint number.
*            Valid values are between 1 and 8.
*  \param pData: A pointer to a data array from which the data for the endpoint
*         space is loaded.
*  \param length: The number of bytes to transfer from the USB Out endpoint and
*          loads it into data array. Valid values are between 0 and 1023. The
*          function moves fewer than the requested number of bytes if the host
*          sends fewer bytes than requested.
*
* \return
*  Number of bytes received, 0 for an invalid endpoint.
*
* \reentrant
*  No.
*
*******************************************************************************/
static uint16 USBFS_ReadOutEP(uint8 epNumber, uint8 pData[], uint16 dataLength)
{
    uint16 length;
    if ((pData != NULL) && (epNumber > USBFS_EP0) && (epNumber < USBFS_MAX_EP))
    {
        /* Adjust requested length to available data. */
        length = (dataLength > USBFS_GetEPCount(epNumber)) ? USBFS_GetEPCount(epNumber) : dataLength;
        /* Copy data using arbiter data register. */
        for (uint16 i = 0u; i < length; ++i)
        {
            pData[i] = (uint8) USBFS_ARB_EP_BASE.arbEp[epNumber].rwDr;
        }

        /* Arm OUT endpoint after data has been copied from endpoint buffer. */
        USBFS_EnableOutEP(epNumber);
    }
    else
    {
        length = 0u;
    }

    return (length);
}

/*******************************************************************************
* Function Name: USBFS_GetEPAckState
****************************************************************************//**
*
*  This function determines whether an ACK transaction occurred on this endpoint
*  by reading the ACK bit in the control register of the endpoint. It does not
*  clear the ACK bit.
*
*  \param epNumber Contains the data endpoint number.
*            Valid values are between 1 and 8.
*
*  \return
*  If an ACKed transaction occurred, this function returns a non-zero value.
*  Otherwise, it returns zero.
*
*******************************************************************************/
static uint8 USBFS_GetEPAckState(uint8 epNumber)
{
    uint8 cr = 0u;

    if ((epNumber > USBFS_EP0) && (epNumber < USBFS_MAX_EP))
    {
        cr = USBFS_SIE_EP_BASE.sieEp[epNumber].epCr0 & USBFS_MODE_ACKD;
    }

    return cr;
}

/*******************************************************************************
* Function Name: USBD_ControlEPEvent
****************************************************************************//**
*
*  This function serves USB Control Endpoint Events
*
*******************************************************************************/
static void USBD_ControlEPEvent(void)
{
    uint8 tempReg = USBFS_EP0_CR_REG;

    if ((tempReg & USBFS_MODE_ACKD) != 0u)
    {
        uint8 modifyReg = 1u;
        if ((tempReg & USBFS_MODE_SETUP_RCVD) != 0u)
        {
            /* Clear register lock by SIE (read register) and clear setup bit
            (write any value in register). */
            USBFS_EP0_CR_REG = tempReg;

            if ((tempReg & USBFS_MODE_MASK) != USBFS_MODE_NAK_IN_OUT)
            {
                /* Mode not equal to NAK_IN_OUT: invalid setup */
                modifyReg = 0u;
            }
            else
            {
                USBFS_ep0Toggle = 0;
                USBD_EndPoint0(USBD_EVT_SETUP);

                if ((USBFS_ep0Mode & USBFS_MODE_SETUP_RCVD) != 0u)
                {
                    /* SETUP bit set: exit without mode modification */
                    modifyReg = 0u;
                }
            }
        }
        else if ((tempReg & USBFS_MODE_IN_RCVD) != 0u)
        {
            USBD_EndPoint0(USBD_EVT_IN);
        }
        else if ((tempReg & USBFS_MODE_OUT_RCVD) != 0u)
        {
            USBD_EndPoint0(USBD_EVT_OUT);
        }
        else
        {
            modifyReg = 0u;
        }

        /* Modify the EP0_CR register */
        if (modifyReg != 0u)
        {

            tempReg = USBFS_EP0_CR_REG;

            /* Make sure that SETUP bit is cleared before modification */
            if ((tempReg & USBFS_MODE_SETUP_RCVD) == 0u)
            {
                /* Update count register */
                tempReg = USBFS_ep0Toggle;
                tempReg |= USBFS_ep0Count;
                USBFS_EP0_CNT_REG = tempReg;

                /* Make sure that previous write operation was successful */
                if (tempReg == USBFS_EP0_CNT_REG)
                {
                    /* Repeat until next successful write operation */
                    do
                    {
                        /* Init temporary variable */
                        modifyReg = USBFS_ep0Mode;

                        /* Unlock register */
                        tempReg = (uint8) (USBFS_EP0_CR_REG & USBFS_MODE_SETUP_RCVD);

                        /* Check if SETUP bit is not set */
                        if (0u == tempReg)
                        {
                            /* Set the Mode Register  */
                            USBFS_EP0_CR_REG = USBFS_ep0Mode;

                            /* Writing check */
                            modifyReg = USBFS_EP0_CR_REG & USBFS_MODE_MASK;
                        }
                    }
                    while (modifyReg != USBFS_ep0Mode);
                }
            }
        }
    }
}

/*******************************************************************************
* Function Name: USBD_Handler
****************************************************************************//**
*
*  This function handle USB Events
*
*******************************************************************************/
void USBD_Handler(void)
{
    if (USBFS_BUS_RESET)
    {
        USBFS_BUS_RESET = false;
        for (uint8 epNumber = 0u; epNumber < USBFS_MAX_EP; epNumber++)
        {
            ClearEPReq(epNumber);
        }
#ifdef DRAG_N_DROP_SUPPORT
        vfs_mngr_fs_remount();
#endif
        USBD_Reset_Event();
    }
    else
    {
        if (USBFS_SOF)
        {
            USBFS_SOF = false;
            USBD_CDC_ACM_SOF_Event();
        }

        for (uint8 epNumber = 0u; epNumber < USBFS_MAX_EP; epNumber++)
        {
            if (USBFS_EP_INT_Pending[epNumber] > 0u)
            {
                switch (epNumber)
                {
                    case USBFS_EP0:
                        USBD_ControlEPEvent();
                        break;
                    case USBFS_EP1:
                        USBD_BULK_EP_BULK_Event(USBD_EVT_OUT);
                        break;
                    case USBFS_EP2:
                        USBD_BULK_EP_BULK_Event(USBD_EVT_IN);
                        break;
                    case USBFS_EP3:
                        /* in this direction traffic is not provided */
                        break;
                    case USBFS_EP4:
                        USBD_CDC_ACM_EP_BULK_Event(USBD_EVT_OUT);
                        break;
                    case USBFS_EP5:
                        USBD_CDC_ACM_EP_BULK_Event(USBD_EVT_IN);
                        break;
                    case USBFS_EP6:
                        USBD_MSC_EP_BULK_Event(USBD_EVT_IN);
                        break;
                    case USBFS_EP7:
                        USBD_MSC_EP_BULK_Event(USBD_EVT_OUT);
                        break;
                    default:
                        /* unknown EP number */
                        break;
                }
            }
        }
    }
}

/*******************************************************************************
* Function Name: USBD_ClrStallEP
****************************************************************************//**
*
*  This function Clear Stall state for USB Device Endpoint
*
*  \param EPNum Contains the Device Endpoint Number
*               EPNum.0..3: Address
*               EPNum.7:    Dir
*
*******************************************************************************/
void USBD_ClrStallEP(U32 EPNum)
{
    uint32 HwEP = FwEPToHwEP(EPNum);
    if (HwEP != EP_NOT_FOUND)
    {
        /* Clear the endpoint Halt */
        USBFS_EP[HwEP].hwEpState &= (uint8) ~USBFS_ENDPOINT_STATUS_HALT;

        /* Clear the data toggle */
        USBFS_EP[HwEP].epToggle = 0u;

        /* Clear toggle bit for already armed packet */
        USBFS_SIE_EP_BASE.sieEp[HwEP].epCnt0 &=
                (uint8) ~(uint8) USBFS_EPX_CNT_DATA_TOGGLE;

        /* Return API State as it was defined before */
        USBFS_EP[HwEP].apiEpState &= (uint8) ~USBFS_NO_EVENT_ALLOWED;

        if ((USBFS_EP[HwEP].addr & USBFS_DIR_IN) != 0u)
        {
            /* IN Endpoint */
            if (USBFS_EP[HwEP].apiEpState == USBFS_IN_BUFFER_EMPTY)
            {
                /* Wait for next packet from application */
                USBFS_SIE_EP_BASE.sieEp[HwEP].epCr0 = USBFS_MODE_NAK_IN;
            }
            else /* Continue armed transfer */
            {
                USBFS_SIE_EP_BASE.sieEp[HwEP].epCr0 = USBFS_MODE_ACK_IN;
            }
        }
        else
        {
            /* OUT Endpoint */
            if (USBFS_EP[HwEP].apiEpState == USBFS_OUT_BUFFER_FULL)
            {
                /* Allow application to read full buffer */
                USBFS_SIE_EP_BASE.sieEp[HwEP].epCr0 = USBFS_MODE_NAK_OUT;
            }
            else /* Mark endpoint as empty, so it will be reloaded */
            {
                USBFS_SIE_EP_BASE.sieEp[HwEP].epCr0 = USBFS_MODE_ACK_OUT;
            }
        }
    }
}

/*******************************************************************************
* Function Name: USBD_ConfigEP
****************************************************************************//**
*
*  This function configure USB Device Endpoint according to Descriptor
*
*  \param pEPD Contains the Pointer to Device Endpoint Descriptor
*
*******************************************************************************/
void USBD_ConfigEP(USB_ENDPOINT_DESCRIPTOR *pEPD)
{
    uint32 HwEP = FwEPToHwEP(pEPD->bEndpointAddress);
    if (HwEP != EP_NOT_FOUND)
    {
        USBFS_EP[HwEP].hwEpState  = 0u;
        USBFS_EP[HwEP].epToggle   = 0u;
        USBFS_EP[HwEP].interface  = 0u;
        USBFS_EP[HwEP].bufferSize = pEPD->wMaxPacketSize;
        USBFS_EP[HwEP].addr       = pEPD->bEndpointAddress;
        USBFS_EP[HwEP].attrib     = pEPD->bmAttributes;

        uint8 epType = pEPD->bmAttributes & USBFS_EP_TYPE_MASK;
        if (0u != (pEPD->bEndpointAddress & USBFS_DIR_IN))
        {
            /* IN Endpoint */
            USBFS_EP[HwEP].epMode     = USBFS_GET_ACTIVE_IN_EP_CR0_MODE(epType);
            USBFS_EP[HwEP].apiEpState = USBFS_EVENT_PENDING;
        }
        else
        {
            /* OUT Endpoint */
            USBFS_EP[HwEP].epMode     = USBFS_GET_ACTIVE_OUT_EP_CR0_MODE(epType);
            USBFS_EP[HwEP].apiEpState = USBFS_NO_EVENT_PENDING;
        }
        USBFS_EP[HwEP].buffOffset = buffCount;
        buffCount += USBFS_EP[HwEP].bufferSize;

        USBFS_ARB_EP_BASE.arbEp[HwEP].epCfg = USBFS_ARB_EPX_CFG_DEFAULT;
        if (USBFS_EP[HwEP].epMode != USBFS_MODE_DISABLE)
        {
            if (0u != (USBFS_EP[HwEP].addr & USBFS_DIR_IN))
            {
                USBFS_SIE_EP_BASE.sieEp[HwEP].epCr0 = USBFS_MODE_NAK_IN;
            }
            else
            {
                USBFS_SIE_EP_BASE.sieEp[HwEP].epCr0 = USBFS_MODE_NAK_OUT;
            }
        }
        else
        {
            USBFS_SIE_EP_BASE.sieEp[HwEP].epCr0 = USBFS_MODE_STALL_DATA_EP;
        }

        USBFS_ARB_EP_BASE.arbEp[HwEP].rwRa    = LO8(USBFS_EP[HwEP].buffOffset);
        USBFS_ARB_EP_BASE.arbEp[HwEP].rwRaMsb = HI8(USBFS_EP[HwEP].buffOffset);
        USBFS_ARB_EP_BASE.arbEp[HwEP].rwWa    = LO8(USBFS_EP[HwEP].buffOffset);
        USBFS_ARB_EP_BASE.arbEp[HwEP].rwWaMsb = HI8(USBFS_EP[HwEP].buffOffset);

        USBFS_SIE_EP_INT_EN_REG |= ((uint8)0x01u) << (HwEP - 1u) ;

        if (0u == (USBFS_EP[HwEP].addr & USBFS_DIR_IN))
        {
            USBFS_EnableOutEP((uint8_t)HwEP);
        }
    }
}

/*******************************************************************************
* Function Name: USBD_Configure
****************************************************************************//**
*
*  This function performs Configure USB Device
*
*  \param cfg Contains the Device Configure/Deconfigure flag
*
*******************************************************************************/
void USBD_Configure(BOOL cfg)
{
    (void)cfg;
    /* Performed by Hardware */
}

/*******************************************************************************
* Function Name: USBD_Connect
****************************************************************************//**
*
*  This function performs Connect/Disconnect USB Device
*
*  \param con Contains the Device Connect/Disconnect flag
*
*******************************************************************************/
void USBD_Connect(BOOL con)
{
    if (con != 0u)
    {

        if (0u == USBFS_initVar)
        {
            USBFS_Init();
            USBFS_initVar = 1u;
        }

        /* Start USBFS Component based on power settings in DWR (5V in this case) */
        USBFS_InitComponent(USBFS_DWR_VDDD_OPERATION);

        buffCount = 0u;
    }
    else
    {
        if ( USBFS_initVar != 0u )
        {
            USBFS_Stop();

            USBD_DeviceStatus = 0u;
            USBD_DeviceAddress = 0u;
            USBD_Configuration = 0u;
            USBD_EndPointMask = 0x00010001u;
            USBD_EndPointHalt = 0x00000000u;
            USBD_EndPointStall = 0x00000000u;
        }
    }
}

/*******************************************************************************
* Function Name: USBD_DirCtrlEP
****************************************************************************//**
*
*  This function Set Direction for USB Device Control Endpoint
*
*  \param dir Contains the Direction of USB Device Control Endpoint
*             Out (dir == 0), In (dir <> 0)
*
*******************************************************************************/
void USBD_DirCtrlEP(U32 dir)
{
    (void)dir;
    /* Performed by Hardware */
}

/*******************************************************************************
* Function Name: USBD_DisableEP
****************************************************************************//**
*
*  This function Disable USB Device Endpoint
*
*  \param EPNum Contains the Device Endpoint Number
*               EPNum.0..3: Address
*               EPNum.7:    Dir
*
*******************************************************************************/
void USBD_DisableEP(U32 EPNum)
{
    if ((EPNum & USBFS_DIR_IN) != 0u)
    {
        /* only OUT endpoint can be disabled in this way */
    }
    else
    {
        uint32 HwEP = FwEPToHwEP(EPNum);
        if ((HwEP > USBFS_EP0) && (HwEP < USBFS_MAX_EP))
        {
            /* Set NAK response for OUT endpoint. */
            USBFS_SIE_EP_BASE.sieEp[HwEP].epCr0 = USBFS_MODE_NAK_OUT;
        }
    }
}

/*******************************************************************************
* Function Name: USBD_EnableEP
****************************************************************************//**
*
*  This function Enable USB Device Endpoint
*
*  \param EPNum Contains the Device Endpoint Number
*               EPNum.0..3: Address
*               EPNum.7:    Dir
*
*******************************************************************************/
void USBD_EnableEP(U32 EPNum)
{
    if ((EPNum & USBFS_DIR_IN) != 0u)
    {
        /* only OUT endpoint can be enabled in this way */
    }
    else
    {
        uint32 HwEP = FwEPToHwEP(EPNum);
        if ((HwEP > USBFS_EP0) && (HwEP < USBFS_MAX_EP))
        {
            USBFS_EP[HwEP].apiEpState = USBFS_NO_EVENT_PENDING;
            /* Enable OUT endpoint to be written by Host. */
            USBFS_SIE_EP_BASE.sieEp[HwEP].epCr0 = USBFS_EP[HwEP].epMode;
        }
    }
}

/*******************************************************************************
* Function Name: USBD_Init
****************************************************************************//**
*
*  This function initialize USB Device
*
*******************************************************************************/
void USBD_Init(void)
{
    /* USB Device will be initialized the first time USBD_Connect is called */
}

/*******************************************************************************
* Function Name: USBD_ReadEP
****************************************************************************//**
*
*   This function moves the specified number of bytes from endpoint buffer to
*   system RAM. The number of bytes actually transferred from endpoint buffer to
*   system RAM is the lesser of the actual number of bytes sent by the host or
*   the number of bytes requested by the length parameter.
*
*  \param EPNum: Contains the Device Endpoint Number
*  \param pData: A pointer to a data array from which the data for the endpoint
*         space is loaded.
*  \param cnt: The number of bytes to transfer from the USB Out endpoint and
*          loads it into data array. Valid values are between 0 and 1023. The
*          function moves fewer than the requested number of bytes if the host
*          sends fewer bytes than requested.
*
* \return
*  Number of bytes received, 0 for an invalid endpoint.
*
*******************************************************************************/
U32 USBD_ReadEP(U32 EPNum, U8 *pData, U32 cnt)
{
    uint32_t res = 0u;
    uint32_t HwEP = FwEPToHwEP(EPNum);
    if (HwEP != EP_NOT_FOUND)
    {
        if (HwEP == USBFS_EP0)
        {
            ClearEPReq(USBFS_EP0);
            uint8_t ep0Count = (USBFS_EP0_CNT_REG & USBFS_EPX_CNT0_MASK) - USBFS_EPX_CNTX_CRC_COUNT;
            if (cnt < ep0Count)
            {
                ep0Count = (uint8_t)cnt;
            }
            for ( ; res < ep0Count; res++ )
            {
                pData[res] = USBFS_EP0_DR_BASE.epData[res];
            }
            if ( res == 0u )
            {
                USBFS_ep0Mode = USBFS_MODE_ACK_IN_STATUS_OUT;
            }
            else
            {
                USBFS_ep0Mode = USBFS_MODE_ACK_OUT_STATUS_IN;
            }
        }
        else
        {
            if (USBFS_GetEPAckState((uint8_t)HwEP) != 0u)
            {
                uint8 enableInterrupts = CyEnterCriticalSection();
                ClearEPReq((uint8_t)HwEP);
                res = USBFS_ReadOutEP((uint8_t)HwEP, pData, (uint16_t)cnt);
                /* Reenable EP */
                USBFS_EnableOutEP((uint8_t)HwEP);
                CyExitCriticalSection(enableInterrupts);
            }
            else
            {
                USBD_SignalHandler();
            }
        }
    }
    return res;
}

/*******************************************************************************
* Function Name: USBD_ResetEP
****************************************************************************//**
*
*   This function reset endpoint state.
*
*  \param EPNum: Contains the Device Endpoint Number
*
*******************************************************************************/
void USBD_ResetEP(U32 EPNum)
{
    (void)EPNum;
    /* not needed */
}

/*******************************************************************************
* Function Name: USBD_SetAddress
****************************************************************************//**
*
*   This function sets device address.
*
*  \param adr: Contains USB Device Address
*  \param setup: Setup stage flag. Called in setup stage (!=0), otherwise after status stage
*
*******************************************************************************/
void USBD_SetAddress(U32 adr, U32 setup)
{
    if (setup == 0u)
    {
        /* Update device address if we got new address. */
        USBFS_CR0_REG = (uint8) adr | USBFS_CR0_ENABLE;
    }
}

/*******************************************************************************
* Function Name: USBD_SetStallEP
****************************************************************************//**
*
*   This function sets stall state for the specified endpoint.
*
*  \param EPNum: Contains the Device Endpoint Number
*
*******************************************************************************/
void USBD_SetStallEP(U32 EPNum)
{
    uint32 HwEP = FwEPToHwEP(EPNum);
    if (HwEP != EP_NOT_FOUND)
    {
        if ( HwEP == USBFS_EP0 )
        {
            /* We expect no more data, so STALLs in case of OUT token 
               and ACKs with a zero length packet in case of IN token. */
            USBFS_ep0Mode = USBFS_MODE_STATUS_IN_ONLY;
        }
        else
        {
            /* Set the endpoint Halt */
            USBFS_EP[HwEP].hwEpState |= (USBFS_ENDPOINT_STATUS_HALT);

            /* Clear the data toggle */
            USBFS_EP[HwEP].epToggle = 0u;
            USBFS_EP[HwEP].apiEpState |= USBFS_NO_EVENT_ALLOWED;

            if ((USBFS_EP[HwEP].addr & USBFS_DIR_IN) != 0u)
            {
                /* IN Endpoint */
                USBFS_SIE_EP_BASE.sieEp[HwEP].epCr0 = (USBFS_MODE_STALL_DATA_EP
                        | USBFS_MODE_ACK_IN);
            }
            else
            {
                /* OUT Endpoint */
                USBFS_SIE_EP_BASE.sieEp[HwEP].epCr0 = (USBFS_MODE_STALL_DATA_EP
                        | USBFS_MODE_ACK_OUT);
            }
        }
    }
}

/*******************************************************************************
* Function Name: USBD_WakeUpCfg
****************************************************************************//**
*
*   This function sets ability of device remote wakeup.
*
*  \param cfg: Flag of Device Function Enable/Disable
*
*******************************************************************************/
void USBD_WakeUpCfg(BOOL cfg)
{
    (void)cfg;
    /* not needed */
}

/*******************************************************************************
* Function Name: USBD_WriteEP
****************************************************************************//**
*
*  This function loads and enables the specified USB data endpoint for an IN
*  data transfer.
*
*  \param EPNum Contains the data endpoint number.
*  \param *pData A pointer to a data array from which the data for the endpoint space
*          is loaded.
*  \param cnt The number of bytes to transfer from the array and then send as
*          a result of an IN request.
*
* \return
*  Number of bytes written, 0 for an invalid endpoint.
*
*******************************************************************************/
U32 USBD_WriteEP(U32 EPNum, U8 *pData, U32 cnt)
{
    uint32_t res = 0u;
    uint32_t HwEP = FwEPToHwEP(EPNum);
    if (HwEP != EP_NOT_FOUND)
    {
        if ( HwEP == USBFS_EP0 )
        {
            ClearEPReq(USBFS_EP0);
            uint32_t wCnt;
            if ( cnt > USBFS_EP0_DR_MAPPED_REG_CNT )
            {
                wCnt = USBFS_EP0_DR_MAPPED_REG_CNT;
            }
            else
            {
                wCnt = cnt;
            }
            for ( ; res < wCnt; res++ )
            {
                USBFS_EP0_DR_BASE.epData[res] = pData[res];
            }

            /* Update the data toggle */
            USBFS_ep0Toggle ^= USBFS_EP0_CNT_DATA_TOGGLE;
            USBFS_ep0Mode = USBFS_MODE_ACK_IN_STATUS_OUT;
            USBFS_ep0Count = (uint8) res;
        }
        else
        {
            if (USBFS_GetEPState((uint8_t)HwEP) == USBFS_IN_BUFFER_EMPTY)
            {
                uint8 enableInterrupts = CyEnterCriticalSection();
                ClearEPReq((uint8_t)HwEP);
                USBFS_LoadInEP((uint8_t)HwEP, pData, (uint16_t)cnt);
                res = cnt;
                CyExitCriticalSection(enableInterrupts);
            }
            else
            {
                /* set EP to stay stalled */
                USBD_SetStallEP(HwEP);
            }
        }
    }
    return res;
}

