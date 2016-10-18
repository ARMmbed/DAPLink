/**
 * @file    usbd_GD32F103.c
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
 
#include "RTL.h"
#include "rl_usb.h"
#include "usbreg.h"
#include "gd32f10x.h"

#include "DAP_config.h"
#include "IO_Config.h"

#define __NO_USB_LIB_C
#include "usb_config.c"

#define USB_DBL_BUF_EP  0x0000
// 16*(4+1)Endpoint Buf Adr
#define EP_BUF_ADDR     (sizeof(EP_BUF_DSCR) * (USBD_EP_NUM + 1))
// Ptr to EP Buf Desc
EP_BUF_DSCR *pBUF_DSCR = (EP_BUF_DSCR *)USB_PMA_ADDR;
// Endpoint Free Buffer Address
U16 FreeBufAddr;

/**
 * @brief   Reset Endpoint
 * @params  EPNum:  Endpoint Number
 *                  EPNum.0..3: Address
 *                  EPNum.7:    Dir
 * @return  None
 */
void EP_Reset (U32 EPNum)
{
    U32 num, val;

    num = EPNum & 0x0F;
    val = EPxREG(num);

    if (EPNum & 0x80) {
        // IN Endpoint
        EPxREG(num) = val & (EP_MASK | EP_TX_DTG);
    }
    else {
        // OUT Endpoint
        EPxREG(num) = val & (EP_MASK | EP_RX_DTG);
    }
}

/**
 * @brief   Set Endpoint Status
 * @params  EPNum:  Endpoint Number
 *                  EPNum.0..3: Address
 *                  EPNum.7:    Dir
 *          stat:   New Status
 * @return  None
 */
void EP_Status (U32 EPNum, U32 stat)
{
    U32 num, val;

    num = EPNum & 0x0F;
    val = EPxREG(num);
    if (EPNum & 0x80) {
        // IN Endpoint
        EPxREG(num) = (val ^ (stat & EP_TX_STA)) & (EP_MASK | EP_TX_STA);
    }
    else{
        // OUT Endpoint
        EPxREG(num) = (val ^ (stat & EP_RX_STA)) & (EP_MASK | EP_RX_STA);
    }
}

/**
 * @brief   USB Device Interrupt enable
 *          Called by USBD_Init to enable the USB Interrupt
 * @return  None
 */
#ifdef __RTX
void __svc(1) USBD_IntrEna  (void);
void __SVC_1                (void)
{
#else
void          USBD_IntrEna (void)
{
#endif
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}

/**
 * @brief   USB Device Initialize Function
 *          Called by the User to initialize USB
 * @return  None
 */
void USBD_Init (void)
{
    // Select USBCLK source
    RCC_USBCLKConfig(RCC_USBCLK_PLL_DIV2);
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_USB, ENABLE);

    USB_CONNECT_OFF();
    // Enable USB Interrupts
    USBD_IntrEna ();
}

/**
 * @brief   USB Device Connect Function
 *          Called by the User to Connect/Disconnect USB Device
 * @params  con:  Connect/Disconnect
 * @Return  Value:   None
 */
void USBD_Connect (BOOL con)
{
    if (con) {
        CNTR = CNTR_SETRST;         // Force USB Reset
        CNTR = 0;
        ISTR = 0;                   // Clear Interrupt Status
        CNTR = CNTR_RSTIE;          // USB Interrupt Mask

        USB_CONNECT_ON();
    }
    else {
        CNTR = CNTR_SETRST | CNTR_CLOSE;   // Switch Off USB Device
        USB_CONNECT_OFF();
    }
}

/**
 * @brief   USB Device Reset Function
 *          Called automatically on USB Device Reset
 * @return  Value:   None
 */
void USBD_Reset (void)
{
    // Double Buffering is not yet supported
    // Clear Interrupt Status
    ISTR = 0;
    CNTR = CNTR_STIE | CNTR_RSTIE

#ifdef __RTX
            | ((USBD_RTX_DevTask    != 0) ? CNTR_ERRIE   : 0)
            | ((USBD_RTX_DevTask    != 0) ? CNTR_PMOUIE : 0)
            | ((USBD_RTX_DevTask    != 0) ? CNTR_SOFIE   : 0)
            | ((USBD_RTX_DevTask    != 0) ? CNTR_ESOFIE  : 0);
#else
            | ((USBD_P_Error_Event != 0) ? CNTR_ERRIE    : 0)
            | ((USBD_P_Error_Event != 0) ? CNTR_PMOUIE : 0)
            | ((USBD_P_SOF_Event    != 0) ? CNTR_SOFIE   : 0)
            | ((USBD_P_SOF_Event    != 0) ? CNTR_ESOFIE  : 0);
#endif

    FreeBufAddr = EP_BUF_ADDR;
    // set BTABLE Address
    BTABLE = 0x00;

    // Setup Control Endpoint 0
    pBUF_DSCR->ADDR_TX = FreeBufAddr;
    FreeBufAddr += USBD_MAX_PACKET0;

    pBUF_DSCR->ADDR_RX = FreeBufAddr;
    FreeBufAddr += USBD_MAX_PACKET0;

    if (USBD_MAX_PACKET0 > 62) {
        pBUF_DSCR->COUNT_RX = ((USBD_MAX_PACKET0 << 5) - 1) | 0x8000;
    }
    else {
        pBUF_DSCR->COUNT_RX =   USBD_MAX_PACKET0 << 9;
    }

    EPxREG(0) = EPxREG(0);
    EPxREG(0) = EP_CONTROL | EP_RX_VALID | EP_TX_NAK | EP_TX_DTG;
    // Enable USB Default Address
    DADDR = DADDR_USBEN | 0;
}

/**
 * @brief   USB Device Suspend Function
 *          Called automatically on USB Device Suspend
 * @Return  Value:   None
 */
void USBD_Suspend (void)
{
    // Force Suspend
    CNTR |= CNTR_SETSPS;
    // Low Power Mode
    CNTR |= CNTR_LOWM;
}

/**
 * @brief   USB Device Resume Function
 *          Called automatically on USB Device Resume
 * @Return  Value:   None
 */
void USBD_Resume (void)
{
    // Performed by Hardware
}

/**
 * @brief   USB Device Remote Wakeup Function
 *          Called automatically on USB Device Remote Wakeup
 * @Return  Value:   None
 */
void USBD_WakeUp (void)
{
    // Clear Suspend
    CNTR &= ~CNTR_SETSPS;
}

/**
 * @brief   USB Device Remote Wakeup Configuration Function
 * @params  cfg:  Device Enable/Disable
 * @Return  Value:  None
 */
void USBD_WakeUpCfg (BOOL cfg)
{
    // Not needed
}

/**
 * @brief   USB Device Set Address Function
 * @params  adr:    USB Device Address
 *          setup:  Called in setup stage (!=0), else after status stage
 * @Return  Value:  None
 */
void USBD_SetAddress (U32 adr, U32 setup)
{
    if (setup)
        return;
    DADDR = DADDR_USBEN | adr;
}

/**
 * @brief   USB Device Configure Function
 * @params  cfg:   Device Configure/Deconfigure
 * @Return  Value: None
 */
void USBD_Configure (BOOL cfg)
{
    if (cfg == __FALSE) {
        FreeBufAddr  = EP_BUF_ADDR;
        // reset Buffer address
        FreeBufAddr += 2 * USBD_MAX_PACKET0;
    }
}

/**
 * @brief   Configure USB Device Endpoint according to Descriptor
 * @params  pEPD:  Pointer to Device Endpoint Descriptor
 * @Return  Value: None
 */
void USBD_ConfigEP (USB_ENDPOINT_DESCRIPTOR *pEPD)
{
    // Double Buffering is not yet supported
    U32 num, val;

    num = pEPD->bEndpointAddress & 0x0F;

    val = pEPD->wMaxPacketSize;
    if (pEPD->bEndpointAddress & USB_ENDPOINT_DIRECTION_MASK) {
        (pBUF_DSCR + num)->ADDR_TX = FreeBufAddr;
        val = (val + 1) & ~1;
    }
    else {
        (pBUF_DSCR + num)->ADDR_RX = FreeBufAddr;
        if (val > 62) {
            val = (val + 31) & ~31;
            (pBUF_DSCR + num)->COUNT_RX = ((val << 5) - 1) | 0x8000;
        }
        else {
            val = (val + 1) & ~1;
            (pBUF_DSCR + num)->COUNT_RX = val << 9;
        }
    }

    FreeBufAddr += val;

    switch (pEPD->bmAttributes & USB_ENDPOINT_TYPE_MASK) {
        case USB_ENDPOINT_TYPE_CONTROL:
            val = EP_CONTROL;
        break;

        case USB_ENDPOINT_TYPE_ISOCHRONOUS:
            val = EP_ISOCHRONOUS;
        break;

        case USB_ENDPOINT_TYPE_BULK:
            val = EP_BULK;
            if (USB_DBL_BUF_EP & (1 << num)) {
                val |= EP_KCTL;
            }
        break;

        case USB_ENDPOINT_TYPE_INTERRUPT:
            val = EP_INTERRUPT;
        break;
    }

    val |= num;
    EPxREG(num) = val;
}

/**
 * @brief   Set Direction for USB Device Control Endpoint
 * @params  dir:   Out (dir == 0), In (dir <> 0)
 * @Return  Value: None
 */
void USBD_DirCtrlEP (U32 dir)
{
    // Not needed
}

/**
 * @brief   Enable USB Device Endpoint
 * @params  EPNum: Device Endpoint Number
 *                 EPNum.0..3: Address
 *                 EPNum.7: Dir
 * @Return  Value: None
 */
void USBD_EnableEP (U32 EPNum)
{
    // EP is able to receive
    EP_Status(EPNum, EP_TX_NAK | EP_RX_VALID);
}

/**
 * @brief   Disable USB Endpoint
 * @params  EPNum: Endpoint Number
 *                 EPNum.0..3: Address
 *                 EPNum.7: Dir
 * @Return  Value: None
 */
void USBD_DisableEP (U32 EPNum)
{
    EP_Status(EPNum, EP_TX_DIS | EP_RX_DIS);
}

/**
 * @brief   Reset USB Device Endpoint
 * @params  EPNum: Device Endpoint Number
 *                 EPNum.0..3: Address
 *                 EPNum.7: Dir
 * @Return  Value: None
 */
void USBD_ResetEP (U32 EPNum)
{
    EP_Reset(EPNum);
}


/**
 * @brief   Set Stall for USB Device Endpoint
 * @params  EPNum: Device Endpoint Number
 *                 EPNum.0..3: Address
 *                 EPNum.7: Dir
 * @Return  Value: None
 */
void USBD_SetStallEP (U32 EPNum)
{
    EP_Status(EPNum, EP_TX_STALL | EP_RX_STALL);
}

/**
 * @brief   Clear Stall for USB Device Endpoint
 * @params  EPNum: Device Endpoint Number
 *                 EPNum.0..3: Address
 *                 EPNum.7: Dir
 * @Return  Value: None
 */
void USBD_ClrStallEP (U32 EPNum)
{
    // reset DTog Bits
    EP_Reset(EPNum);
    EP_Status(EPNum, EP_TX_VALID | EP_RX_VALID);
}


/**
 * @brief   Clear USB Device Endpoint Buffer
 * @params  EPNum: Device Endpoint Number
 *                 EPNum.0..3: Address
 *                 EPNum.7: Dir
 * @Return  Value: None
 */
void USBD_ClearEPBuf (U32 EPNum)
{
    // do nothing
}

/**
 * @brief   Read USB Device Endpoint Data
 * @params  EPNum: Device Endpoint Number
 *                 EPNum.0..3: Address
 *                 EPNum.7: Dir
 *                 pData: Pointer to Data Buffer
 * @Return  Value: Number of bytes read
 */
U32 USBD_ReadEP (U32 EPNum, U8 *pData, U32 size)
{
    // Double Buffering is not yet supported
    U32 num, cnt, *pv, n;

    num = EPNum & 0x0F;

    pv  = (U32 *)(USB_PMA_ADDR + 2 * ((pBUF_DSCR + num)->ADDR_RX));
    cnt = (pBUF_DSCR + num)->COUNT_RX & EP_COUNT_MASK;
    if(cnt > size) {
        cnt = size;
    }
    for (n = 0; n < (cnt + 1) / 2; n++) {
        *((__packed U16 *)pData) = *pv++;
        pData += 2;
    }
    EP_Status(EPNum, EP_RX_VALID);

    return (cnt);
}

/**
 * @brief   Write USB Device Endpoint Data
 * @params  EPNum: Device Endpoint Number
 *                 EPNum.0..3: Address
 *                 EPNum.7: Dir
 *                 pData: Pointer to Data Buffer
 *                 cnt:   Number of bytes to write
 * @Return  Value: Number of bytes written
 */
U32 USBD_WriteEP (U32 EPNum, U8 *pData, U32 cnt)
{
    // Double Buffering is not yet supported
    U32 num, *pv, n;
    U16 statusEP;

    num = EPNum & 0x0F;

    pv  = (U32 *)(USB_PMA_ADDR + 2 * ((pBUF_DSCR + num)->ADDR_TX));
    for (n = 0; n < (cnt + 1) / 2; n++) {
        *pv++ = *((__packed U16 *)pData);
        pData += 2;

    }
    (pBUF_DSCR + num)->COUNT_TX = cnt;

    statusEP = EPxREG(num);
    if ((statusEP & EP_TX_STA) != EP_TX_STALL) {
        // do not make EP valid if stalled
        EP_Status(EPNum, EP_TX_VALID);
    }

    return (cnt);
}

/**
 * @brief   Get USB Device Last Frame Number
 * @params  None
 * @return  Value: Frame Number
 */
U32 USBD_GetFrame (void)
{
    return (FNR & FNR_FCNT);
}

#ifdef __RTX
U32 LastError;    // Last Error

/**
 * @brief   Get USB Last Error Code
 * @params  None
 * @return  Value: Error Code
 */
U32 USBD_GetError (void)
{
    return (LastError);
}
#endif

/**
 * @brief   USB Device Interrupt Service Routine
 */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
    USBD_SignalHandler();
}

void USBWakeUp_IRQHandler(void)
{
    EXTI_ClearIntBitState(EXTI_LINE18);
}

/**
 * @brief   Get USB Last Error Code
 * @params  None
 * @return  Value: Error Code
 */
void USBD_Handler(void)
{
    U32 istr, num, val;

    istr = ISTR;

    if (istr & ISTR_RSTIF) {
        // USB Reset Request
        USBD_Reset();
        usbd_reset_core();
#ifdef __RTX
        if (USBD_RTX_DevTask) {
            isr_evt_set(USBD_EVT_RESET, USBD_RTX_DevTask);
        }
#else
        if (USBD_P_Reset_Event) {
            USBD_P_Reset_Event();
        }
#endif
        ISTR = ~ISTR_RSTIF;
    }

    if (istr & ISTR_SPSIF) {
        // USB Suspend Request
        USBD_Suspend();
#ifdef __RTX
        if (USBD_RTX_DevTask) {
            isr_evt_set(USBD_EVT_SUSPEND, USBD_RTX_DevTask);
        }
#else
        if (USBD_P_Suspend_Event) {
            USBD_P_Suspend_Event();
        }
#endif
        ISTR = ~ISTR_SPSIF;
    }

    if (istr & ISTR_WKUPIF) {
        // USB Wakeup
        USBD_WakeUp();
#ifdef __RTX
        if (USBD_RTX_DevTask) {
            isr_evt_set(USBD_EVT_RESUME,  USBD_RTX_DevTask);
        }
#else
        if (USBD_P_Resume_Event) {
            USBD_P_Resume_Event();
        }
#endif
        ISTR = ~ISTR_WKUPIF;
    }

    if (istr & ISTR_SOFIF) {
        // Start of Frame
#ifdef __RTX
        if (USBD_RTX_DevTask) {
            isr_evt_set(USBD_EVT_SOF, USBD_RTX_DevTask);
        }
#else
        if (USBD_P_SOF_Event) {
            USBD_P_SOF_Event();
        }
#endif
        ISTR = ~ISTR_SOFIF;
    }

    if (istr & ISTR_PMOUIF) {
#ifdef __RTX
        LastError = 2;
        if (USBD_RTX_DevTask) {
            // PMA Over/underrun
            isr_evt_set(USBD_EVT_ERROR, USBD_RTX_DevTask);
        }
#else
        if (USBD_P_Error_Event) {
            USBD_P_Error_Event(2);
        }
#endif
        ISTR = ~ISTR_PMOUIF;
    }

    if (istr & ISTR_ERRIF) {
        // Error: No Answer, CRC Error, Bit Stuff Error, Frame Format Error
#ifdef __RTX
        LastError = 1;
        if (USBD_RTX_DevTask) {
            isr_evt_set(USBD_EVT_ERROR, USBD_RTX_DevTask);
        }
#else
        if (USBD_P_Error_Event) {
            USBD_P_Error_Event(1);
        }
#endif
        ISTR = ~ISTR_ERRIF;
    }

    while ((istr = ISTR) & ISTR_STIF) {
        // Endpoint Interrupts
        ISTR = ~ISTR_STIF;
        num = istr & ISTR_EPNUM;

        val = EPxREG(num);
        if (val & EP_RX_ST) {
            EPxREG(num) = val & ~EP_RX_ST & EP_MASK;
#ifdef __RTX
            if (USBD_RTX_EPTask[num]) {
                isr_evt_set((val & EP_SETUP) ? USBD_EVT_SETUP : USBD_EVT_OUT, USBD_RTX_EPTask[num]);
            }
#else
            if (USBD_P_EP[num]) {
                USBD_P_EP[num]((val & EP_SETUP) ? USBD_EVT_SETUP : USBD_EVT_OUT);
            }
#endif
        }
        if (val & EP_TX_ST) {
            EPxREG(num) = val & ~EP_TX_ST & EP_MASK;
#ifdef __RTX
            if (USBD_RTX_EPTask[num]) {
                isr_evt_set(USBD_EVT_IN,  USBD_RTX_EPTask[num]);
            }
#else
            if (USBD_P_EP[num]) {
                USBD_P_EP[num](USBD_EVT_IN);
            }
#endif
        }
    }
    // Need to clean ISTR_ESOFIF, otherwise it will produce interrupt always.
    ISTR = ~(ISTR_ESOFIF);
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}
