/**
 * @file    usbd_gd32f303.c
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

#include "usbd_regs.h"

#include "IO_Config.h"

#define __NO_USB_LIB_C
#include "usb_config.c"

#define USB_DBL_BUF_EP      0x0000

/* Endpoint Buf Adr */
#define EP_BUF_ADDR (sizeof(EP_BUF_DSCR) * (USBD_EP_NUM + 1))

/* Ptr to EP Buf Desc */
EP_BUF_DSCR *pBUF_DSCR = (EP_BUF_DSCR *)USBD_RAM;

/* Endpoint Free Buffer Address */
U16 FreeBufAddr;

/**
  * @brief  Reset Endpoint
  * @param  EPNum: Endpoint Number
                   EPNum.bit 0..6: Address
                   EPNum.bit 7: Dir
  * @retval None
  */
void EP_Reset (U32 EPNum)
{
    U32 num, val;

    num = EPNum & 0x0F;

    val = USBD_REG_GET(USBD_EPxCS(num));

    if (EPNum & 0x80)
    {
        /* IN Endpoint */
        USBD_REG_SET(USBD_EPxCS(num), val & (EPCS_MASK | EPxCS_TX_DTG));
    }
    else
    {
        /* OUT Endpoint */
        USBD_REG_SET(USBD_EPxCS(num), val & (EPCS_MASK | EPxCS_RX_DTG));
    }
}

/**
  * @brief  Set Endpoint Status
  * @param  EPNum: Endpoint Number
                   EPNum.bit 0..6: Address
                   EPNum.bit 7: Dir
  * @param  stat: New Status
  * @retval None
  */
void EP_Status (U32 EPNum, U32 stat)
{
    U32 num, val;

    num = EPNum & 0x0F;

    val = USBD_REG_GET(USBD_EPxCS(num));

    if (EPNum & 0x80)
    {
        /* IN Endpoint */
        USBD_REG_SET(USBD_EPxCS(num), (val ^ (stat & EPxCS_TX_STA)) & (EPCS_MASK | EPxCS_TX_STA));
    }
    else
    {
        /* OUT Endpoint */
        USBD_REG_SET(USBD_EPxCS(num), (val ^ (stat & EPxCS_RX_STA)) & (EPCS_MASK | EPxCS_RX_STA));
    }
}

/**
  * @brief  USB Device Interrupt enable
            Called by USBD_Init to enable the USB Interrupt
  * @param  None
  * @retval None
  */
#ifdef __RTX
void __svc(1) USBD_IntrEna (void);
void __SVC_1 (void)
{
#else
void USBD_IntrEna (void)
{
#endif
    nvic_irq_enable(USBD_LP_CAN0_RX0_IRQn, 0, 1);
}

/**
  * @brief  USB Device Initialize Function
            Called by the User to initialize USB
  * @param  None
  * @retval None
  */
void USBD_Init (void)
{
    /* Configure USB model clock from PLL clock */
    rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV2_5);

    rcu_periph_clock_enable(RCU_USBD);

    /* Enable USB Interrupts */
    USBD_IntrEna ();
}

/**
  * @brief  USB Device Connect Function
            Called by the User to Connect/Disconnect USB Device
  * @param  con: Connect/Disconnect
  * @retval None
  */
void USBD_Connect (BOOL con)
{
    if (con)
    {
        PIN_USB_CONNECT_ON();

        /* Force USB Reset */
        USBD_REG_SET(USBD_CTL, CTL_SETRST);
        USBD_REG_SET(USBD_CTL, 0);

        /* Clear Interrupt Status */
        USBD_REG_SET(USBD_INTF, 0);
        USBD_REG_SET(USBD_CTL, CTL_RSTIE);

    }
    else
    {
        /* Switch Off USB Device */
        USBD_REG_SET(USBD_CTL, CTL_SETRST | CTL_CLOSE);

        PIN_USB_CONNECT_OFF();
    }
}

/**
  * @brief  USB Device Reset Function
            Called automatically on USB Device Reset
  * @param  None
  * @retval None
  */
void USBD_Reset (void)
{
    /* Clear Interrupt Status */
    USBD_REG_SET(USBD_INTF, 0);

    USBD_REG_SET(USBD_CTL, CTL_STIE | CTL_RSTIE | CTL_SOFIE);

    FreeBufAddr = EP_BUF_ADDR;

    /* set BTABLE Address */
    USBD_REG_SET(USBD_BADDR, 0x00);

    /* Setup Control Endpoint 0 */
    pBUF_DSCR->ADDR_TX = FreeBufAddr;
    FreeBufAddr += USBD_MAX_PACKET0;

    pBUF_DSCR->ADDR_RX = FreeBufAddr;
    FreeBufAddr += USBD_MAX_PACKET0;

    if (USBD_MAX_PACKET0 > 62)
    {
        pBUF_DSCR->COUNT_RX = ((USBD_MAX_PACKET0 << 5) - 1) | 0x8000;
    }
    else
    {
        pBUF_DSCR->COUNT_RX = USBD_MAX_PACKET0 << 9;
    }

    USBD_REG_SET(USBD_EPxCS(0), EP_CONTROL | EPRX_VALID | EPTX_NAK);

    /* Enable USB Default Address */
    USBD_REG_SET(USBD_DADDR, DADDR_USBEN | 0);
}

/**
  * @brief  USB Device Suspend Function
            Called automatically on USB Device Suspend
  * @param  None
  * @retval None
  */
void USBD_Suspend (void)
{
    /* Force Suspend */
    USBD_REG_SET(USBD_CTL, CTL_SETSPS);

    /* Low Power Mode */
    USBD_REG_SET(USBD_CTL, USBD_REG_GET(USBD_CTL) | CTL_LOWM);
}

/**
  * @brief  USB Device Resume Function
            Called automatically on USB Device Resume
  * @param  None
  * @retval None
  */
void USBD_Resume (void)
{
    /* Performed by Hardware */
}

/**
  * @brief  USB Device Remote Wakeup Function
            Called automatically on USB Device Remote Wakeup
  * @param  None
  * @retval None
  */
void USBD_WakeUp (void)
{
    /* Clear Suspend */
    USBD_REG_SET(USBD_CTL, USBD_REG_GET(USBD_CTL) & ~CTL_SETSPS);
}

/**
  * @brief  USB Device Remote Wakeup Configuration Function
  * @param  cfg: Device Enable/Disable
  * @retval None
  */
void USBD_WakeUpCfg (BOOL cfg)
{
    /* Not needed */
}

/**
  * @brief  USB Device Set Address Function
  * @param  adr: USB Device Address
  * @param  setup: Called in setup stage (!=0), else after status stage
  * @retval None
  */
void USBD_SetAddress (U32 adr, U32 setup)
{
    if (setup) return;

    USBD_REG_SET(USBD_DADDR, DADDR_USBEN | adr);
}

/**
  * @brief  USB Device Configure Function
  * @param  cfg: Device Configure/Deconfigure
  * @retval None
  */
void USBD_Configure (BOOL cfg)
{
    if (cfg == __FALSE)
    {
        FreeBufAddr  = EP_BUF_ADDR;

        /* reset Buffer address */
        FreeBufAddr += 2 * USBD_MAX_PACKET0;
    }
}

/**
  * @brief  Configure USB Device Endpoint according to Descriptor
  * @param  pEPD: Pointer to Device Endpoint Descriptor
  * @retval None
  */
void USBD_ConfigEP (USB_ENDPOINT_DESCRIPTOR *pEPD)
{
    /* Double Buffering is not yet supported */
    U32 num, val;

    num = pEPD->bEndpointAddress & 0x0F;

    val = pEPD->wMaxPacketSize;

    if (pEPD->bEndpointAddress & USB_ENDPOINT_DIRECTION_MASK)
    {
        (pBUF_DSCR + num)->ADDR_TX = FreeBufAddr;
        val = (val + 1) & ~1;
    }
    else
    {
        (pBUF_DSCR + num)->ADDR_RX = FreeBufAddr;

        if (val > 62)
        {
            val = (val + 31) & ~31;
            (pBUF_DSCR + num)->COUNT_RX = ((val << 5) - 1) | 0x8000;
        }
        else
        {
            val = (val + 1)  & ~1;
            (pBUF_DSCR + num)->COUNT_RX = val << 9;
        }
    }

    FreeBufAddr += val;

    switch (pEPD->bmAttributes & USB_ENDPOINT_TYPE_MASK)
    {
        case USB_ENDPOINT_TYPE_CONTROL:
            val = EP_CONTROL;
            break;

        case USB_ENDPOINT_TYPE_ISOCHRONOUS:
            val = EP_ISO;
            break;

        case USB_ENDPOINT_TYPE_BULK:
            val = EP_BULK;
            if (USB_DBL_BUF_EP & (1 << num))
            {
                val |= EPxCS_KCTL;//EP_KIND;
            }
            break;

        case USB_ENDPOINT_TYPE_INTERRUPT:
            val = EP_INTERRUPT;
            break;
    }

    val |= num;

    USBD_REG_SET(USBD_EPxCS(num), val);
}

/**
  * @brief  Set Direction for USB Device Control Endpointn
  * @param  dir: Out (dir == 0), In (dir <> 0)
  * @retval None
  */
void USBD_DirCtrlEP (U32 dir)
{
    /* Not needed */
}

/**
  * @brief  Enable USB Device Endpoint
  * @param  EPNum: Device Endpoint Number
                   bit.0..6: Address
                   bit.7: Dir
  * @retval None
  */
void USBD_EnableEP (U32 EPNum)
{
    /* EP is able to receive */
    EP_Status(EPNum, EPTX_NAK | EPRX_VALID);
}

/**
  * @brief  Disable USB Device Endpoint
  * @param  EPNum: Device Endpoint Number
                   bit.0..6: Address
                   bit.7: Dir
  * @retval None
  */
void USBD_DisableEP (U32 EPNum)
{
    EP_Status(EPNum, EPTX_NAK | EPRX_NAK);
}

/**
  * @brief  Reset USB Device Endpoint
  * @param  EPNum: Device Endpoint Number
                   bit.0..6: Address
                   bit.7: Dir
  * @retval None
  */
void USBD_ResetEP (U32 EPNum)
{
    EP_Reset(EPNum);
}

/**
  * @brief  Set Stall for USB Device Endpoint
  * @param  EPNum: Device Endpoint Number
                   bit.0..6: Address
                   bit.7: Dir
  * @retval None
  */
void USBD_SetStallEP (U32 EPNum)
{
    EP_Status(EPNum, EPTX_STALL | EPRX_STALL);
}

/**
  * @brief  Clear Stall for USB Device Endpoint
  * @param  EPNum: Device Endpoint Number
                   bit.0..6: Address
                   bit.7: Dir
  * @retval None
  */
void USBD_ClrStallEP (U32 EPNum)
{
    /* Reset DTog Bits */
    EP_Reset(EPNum);

    EP_Status(EPNum, EPTX_VALID | EPRX_VALID);
}

/**
  * @brief  Clear USB Device Endpoint Buffer
  * @param  EPNum: Device Endpoint Number
                   bit.0..6: Address
                   bit.7: Dir
  * @retval None
  */
void USBD_ClearEPBuf (U32 EPNum)
{

}

/**
  * @brief  Read USB Device Endpoint Data
  * @param  EPNum: Device Endpoint Number
                   bit.0..6: Address
                   bit.7: Dir
  * @param  pData: Pointer to Data Buffer
  * @retval None
  */
U32 USBD_ReadEP (U32 EPNum, U8 *pData, U32 size)
{
    /* Double Buffering is not yet supported */
    U32 num, cnt, *pv, n;

    num = EPNum & 0x0F;

    pv  = (U32 *)(USBD_RAM + 2*((pBUF_DSCR + num)->ADDR_RX));

    cnt = (pBUF_DSCR + num)->COUNT_RX & EPRCNT_CNT;

    for (n = 0; n < (cnt + 1) / 2; n++)
    {
        *((__packed U16 *)pData) = *pv++;

        pData += 2;
    }

    EP_Status(EPNum, EPRX_VALID);

    return (cnt);
}

/**
  * @brief  Write USB Device Endpoint Data
  * @param  EPNum: Device Endpoint Number
                   bit.0..6: Address
                   bit.7: Dir
  * @param  pData: Pointer to Data Buffer
  * @param  cnt: Number of bytes to write
  * @retval None
  */
U32 USBD_WriteEP (U32 EPNum, U8 *pData, U32 cnt)
{
    /* Double Buffering is not yet supported */
    U32 num, *pv, n;
    U16 statusEP;

    num = EPNum & 0x0F;

    pv  = (U32 *)(USBD_RAM + 2 * ((pBUF_DSCR + num)->ADDR_TX));

    for (n = 0; n < (cnt + 1) / 2; n++)
    {
        *pv++ = *((__packed U16 *)pData);
        pData += 2;
    }

    (pBUF_DSCR + num)->COUNT_TX = cnt;

    statusEP = USBD_REG_GET(USBD_EPxCS(num));

    if ((statusEP & EPxCS_TX_STA) != EPTX_STALL)
    {
        /* do not make EP valid if stalled */
        EP_Status(EPNum, EPTX_VALID);      
    }

    return (cnt);
}

/**
  * @brief  Get USB Device Last Frame Number
  * @param  None
  * @retval Frame Number
  */
U32 USBD_GetFrame (void)
{
    return USBD_REG_GET(USBD_STAT) & STAT_FCNT;
}


#ifdef __RTX
U32 LastError;                          /* Last Error */

/**
  * @brief  Get USB Last Error Code
  * @param  None
  * @retval Error Code
  */
U32 USBD_GetError (void)
{
    return (LastError);
}

#endif

/**
  * @brief  USB Device Interrupt Service Routine
  * @param  None
  * @retval None
  */
void USBD_LP_CAN0_RX0_IRQHandler(void)
{
    NVIC_DisableIRQ(USBD_LP_CAN0_RX0_IRQn);
    USBD_SignalHandler();
}

void USBD_Handler(void)
{
    U32 istr, num, val;

    istr = USBD_REG_GET(USBD_INTF);

    /* USB Reset Request */
    if (istr & INTF_RSTIF)
    {
        USBD_Reset();
        usbd_reset_core();

#ifdef __RTX
        if (USBD_RTX_DevTask)
        {
            isr_evt_set(USBD_EVT_RESET, USBD_RTX_DevTask);
        }
#else
        if (USBD_P_Reset_Event)
        {
            USBD_P_Reset_Event();
        }
#endif

        USBD_REG_SET(USBD_INTF, CLR_RSTIF);
    }

    /* USB Suspend Request */
    if (istr & INTF_SPSIF)
    {
        USBD_Suspend();

#ifdef __RTX
        if (USBD_RTX_DevTask)
        {
            isr_evt_set(USBD_EVT_SUSPEND, USBD_RTX_DevTask);
        }
#else
        if (USBD_P_Suspend_Event)
        {
            USBD_P_Suspend_Event();
        }
#endif

        USBD_REG_SET(USBD_INTF, CLR_SPSIF);
    }

    /* USB Wakeup */
    if (istr & INTF_WKUPIF)
    {
        USBD_WakeUp();

#ifdef __RTX
        if (USBD_RTX_DevTask)
        {
            isr_evt_set(USBD_EVT_RESUME,  USBD_RTX_DevTask);
        }
#else
        if (USBD_P_Resume_Event)
        {
            USBD_P_Resume_Event();
        }
#endif

        USBD_REG_SET(USBD_INTF, CLR_WKUPIF);
    }

    /* Start of Frame */
    if (istr & INTF_SOFIF)
    {
#ifdef __RTX
        if (USBD_RTX_DevTask)
        {
            isr_evt_set(USBD_EVT_SOF, USBD_RTX_DevTask);
        }
#else
        if (USBD_P_SOF_Event)
        {
            USBD_P_SOF_Event();
        }
#endif

        USBD_REG_SET(USBD_INTF, CLR_SOFIF);
    }

    /* PMA Over/underrun */
    if (istr & INTF_PMOUIF)
    {
#ifdef __RTX
        LastError = 2;
        if (USBD_RTX_DevTask)
        {
            isr_evt_set(USBD_EVT_ERROR, USBD_RTX_DevTask);
        }
#else
        if (USBD_P_Error_Event)
        {
            USBD_P_Error_Event(2);
        }
#endif

        USBD_REG_SET(USBD_INTF, CLR_PMOUIF);
    }

    /* Error: No Answer, CRC Error, Bit Stuff Error, Frame Format Error */
    if (istr & INTF_ERRIF)
    {
#ifdef __RTX
        LastError = 1;
        if (USBD_RTX_DevTask)
        {
            isr_evt_set(USBD_EVT_ERROR, USBD_RTX_DevTask);
        }
#else
        if (USBD_P_Error_Event)
        {
            USBD_P_Error_Event(1);
        }
#endif

        USBD_REG_SET(USBD_INTF, CLR_ERRIF);
    }

    /* Endpoint Interrupts */
    while ((istr = USBD_REG_GET(USBD_INTF)) & INTF_STIF)
    {
        USBD_REG_SET(USBD_INTF, CLR_STIF);

        num = istr & INTF_EPNUM;

        val = USBD_REG_GET(USBD_EPxCS(num));

        if (val & EPxCS_RX_ST)
        {
            USBD_REG_SET(USBD_EPxCS(num), val & ~EPxCS_RX_ST & EPCS_MASK);

#ifdef __RTX
            if (USBD_RTX_EPTask[num])
            {
                isr_evt_set((val & EP_SETUP) ? USBD_EVT_SETUP : USBD_EVT_OUT, USBD_RTX_EPTask[num]);
            }
#else
            if (USBD_P_EP[num])
            {
                USBD_P_EP[num]((val & EPxCS_SETUP) ? USBD_EVT_SETUP : USBD_EVT_OUT);
            }
#endif
        }

        if (val & EPxCS_TX_ST)
        {
            USBD_REG_SET(USBD_EPxCS(num), val & ~EPxCS_TX_ST & EPCS_MASK);

#ifdef __RTX
            if (USBD_RTX_EPTask[num])
            {
                isr_evt_set(USBD_EVT_IN,  USBD_RTX_EPTask[num]);
            }
#else
            if (USBD_P_EP[num])
            {
                USBD_P_EP[num](USBD_EVT_IN);
            }
#endif
        }
    }

    NVIC_EnableIRQ(USBD_LP_CAN0_RX0_IRQn);
}
