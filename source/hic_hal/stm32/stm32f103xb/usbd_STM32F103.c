/*----------------------------------------------------------------------------
 *    RL-ARM - USB
 *----------------------------------------------------------------------------
 *    Name: usbd_STM32F103.c
 *    Purpose: Hardware Layer module for ST STM32F103
 *    Rev.: V4.70
 *----------------------------------------------------------------------------
 *    This code is part of the RealView Run-Time Library.
 *    Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include "RTL.h"
#include "rl_usb.h"
#include "usbreg.h"
#include "stm32f10x.h"

#include "DAP_config.h"
#include "IO_Config.h"
#include "uart.h"

#define __NO_USB_LIB_C
#include "usb_config.c"

#define USB_DBL_BUF_EP  0x0000

#define EP_BUF_ADDR     (sizeof(EP_BUF_DSCR) * (USBD_EP_NUM + 1))   /* 16*(4+1)Endpoint Buf Adr */
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

    if (EPNum & 0x80)
    {   // IN Endpoint
        EPxREG(num) = val & (EP_MASK | EP_DTOG_TX);
    }
    else
    {   // OUT Endpoint
        EPxREG(num) = val & (EP_MASK | EP_DTOG_RX);
    }
}

/**
 * @brief   Set Endpoint Status
 * @perems  EPNum:  Endpoint Number
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
    if (EPNum & 0x80)
    {   /* IN Endpoint                      */
        EPxREG(num) = (val ^ (stat & EP_STAT_TX)) & (EP_MASK | EP_STAT_TX);
    }
    else
    {   /* OUT Endpoint                     */
        EPxREG(num) = (val ^ (stat & EP_STAT_RX)) & (EP_MASK | EP_STAT_RX);
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
    /* Select USBCLK source */
    RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
    /* Enable USB clock */
    RCC->APB1ENR |= RCC_APB1ENR_USBEN;

    /* Enable USB Interrupts */
    USBD_IntrEna ();
    /* Set USB connect pin to low */
    USB_CONNECT_OFF();
}

/*
 *  USB Device Connect Function
 *  Called by the User to Connect/Disconnect USB Device
 *  Parameters:   con:  Connect/Disconnect
 *  Return Value:   None
 */
void USBD_Connect (BOOL con)
{
    if (con)
    {
        CNTR = CNTR_FRES;               /* Force USB Reset                      */
        CNTR = 0;
        ISTR = 0;                       /* Clear Interrupt Status               */
        CNTR = CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM;   /* USB Interrupt Mask   */

        USB_CONNECT_ON();
    }
    else
    {
        CNTR = CNTR_FRES | CNTR_PDWN;   /* Switch Off USB Device              */
        USB_CONNECT_OFF();
    }
}

/*
 *  USB Device Reset Function
 *  Called automatically on USB Device Reset
 *  Return Value:   None
 */
void USBD_Reset (void)
{
    /* Double Buffering is not yet supported    */

    ISTR = 0;   /* Clear Interrupt Status       */

    CNTR = CNTR_CTRM | CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM
#ifdef __RTX
            | ((USBD_RTX_DevTask    != 0) ? CNTR_ERRM   : 0)
            | ((USBD_RTX_DevTask    != 0) ? CNTR_PMAOVRM : 0)
            | ((USBD_RTX_DevTask    != 0) ? CNTR_SOFM   : 0)
            | ((USBD_RTX_DevTask    != 0) ? CNTR_ESOFM  : 0);
#else
            | ((USBD_P_Error_Event != 0) ? CNTR_ERRM    : 0)
            | ((USBD_P_Error_Event != 0) ? CNTR_PMAOVRM : 0)
            | ((USBD_P_SOF_Event    != 0) ? CNTR_SOFM   : 0)
            | ((USBD_P_SOF_Event    != 0) ? CNTR_ESOFM  : 0);
#endif

    FreeBufAddr = EP_BUF_ADDR;

    BTABLE = 0x00;  /* set BTABLE Address       */

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
        pBUF_DSCR->COUNT_RX =   USBD_MAX_PACKET0 << 9;
    }
    EPxREG(0) = EP_CONTROL | EP_RX_VALID;

    DADDR = DADDR_EF | 0;   /* Enable USB Default Address   */
}

/*
 *  USB Device Suspend Function
 *  Called automatically on USB Device Suspend
 *  Return Value:   None
 */
void USBD_Suspend (void)
{
    CNTR |= CNTR_FSUSP;     /* Force Suspend    */
    CNTR |= CNTR_LPMODE;    /* Low Power Mode   */
}

/*
 *  USB Device Resume Function
 *  Called automatically on USB Device Resume
 *  Return Value:   None
 */
void USBD_Resume (void)
{
    /* Performed by Hardware    */
}

/*
 *  USB Device Remote Wakeup Function
 *  Called automatically on USB Device Remote Wakeup
 *  Return Value:   None
 */
void USBD_WakeUp (void)
{
    CNTR &= ~CNTR_FSUSP;    /* Clear Suspend    */
}

/*
 *  USB Device Remote Wakeup Configuration Function
 *  Parameters:   cfg:  Device Enable/Disable
 *  Return Value:   None
 */
void USBD_WakeUpCfg (BOOL cfg)
{
    /* Not needed */
}

/*
 *  USB Device Set Address Function
 *  Parameters:     adr:    USB Device Address
 *                  setup: Called in setup stage (!=0), else after status stage
 *  Return Value:   None
 */
void USBD_SetAddress (U32 adr, U32 setup)
{
    if (setup)
        return;
    DADDR = DADDR_EF | adr;
}

/*
 *  USB Device Configure Function
 *  Parameters:     cfg:    Device Configure/Deconfigure
 *  Return Value:   None
 */
void USBD_Configure (BOOL cfg)
{
    if (cfg == __FALSE)
    {
        FreeBufAddr  = EP_BUF_ADDR;
        FreeBufAddr += 2 * USBD_MAX_PACKET0;    /* reset Buffer address */
    }
}


/*
 *  Configure USB Device Endpoint according to Descriptor
 *  Parameters:   pEPD:  Pointer to Device Endpoint Descriptor
 *  Return Value:   None
 */
void USBD_ConfigEP (USB_ENDPOINT_DESCRIPTOR *pEPD)
{
    /* Double Buffering is not yet supported                                    */
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
        val = EP_ISOCHRONOUS;
        break;
    case USB_ENDPOINT_TYPE_BULK:
        val = EP_BULK;
        if (USB_DBL_BUF_EP & (1 << num))
        {
            val |= EP_KIND;
        }
        break;
    case USB_ENDPOINT_TYPE_INTERRUPT:
        val = EP_INTERRUPT;
        break;
    }
    val |= num;
    EPxREG(num) = val;
}

/*
 *  Set Direction for USB Device Control Endpoint
 *  Parameters:     dir:    Out (dir == 0), In (dir <> 0)
 *  Return Value:   None
 */
void USBD_DirCtrlEP (U32 dir)
{
    /* Not needed */
}

/*
 *  Enable USB Device Endpoint
 *  Parameters:     EPNum: Device Endpoint Number
 *                      EPNum.0..3: Address
 *                      EPNum.7:    Dir
 *  Return Value:   None
 */
void USBD_EnableEP (U32 EPNum)
{
    EP_Status(EPNum, EP_TX_NAK | EP_RX_VALID);  /* EP is able to receive */
}

/*
 *  Disable USB Endpoint
 *  Parameters:     EPNum: Endpoint Number
 *                      EPNum.0..3: Address
 *                      EPNum.7:    Dir
 *  Return Value:   None
 */
void USBD_DisableEP (U32 EPNum)
{
    EP_Status(EPNum, EP_TX_DIS | EP_RX_DIS);
}

/*
 *  Reset USB Device Endpoint
 *  Parameters:     EPNum: Device Endpoint Number
 *                      EPNum.0..3: Address
 *                      EPNum.7:    Dir
 *  Return Value:   None
 */
void USBD_ResetEP (U32 EPNum)
{
    EP_Reset(EPNum);
}


/*
 *  Set Stall for USB Device Endpoint
 *  Parameters:     EPNum: Device Endpoint Number
 *                      EPNum.0..3: Address
 *                      EPNum.7:    Dir
 *  Return Value:   None
 */
void USBD_SetStallEP (U32 EPNum)
{
    EP_Status(EPNum, EP_TX_STALL | EP_RX_STALL);
}

/*
 *  Clear Stall for USB Device Endpoint
 *  Parameters:     EPNum: Device Endpoint Number
 *                      EPNum.0..3: Address
 *                      EPNum.7:    Dir
 *  Return Value:   None
 */
void USBD_ClrStallEP (U32 EPNum)
{
    EP_Reset(EPNum);    /* reset DTog Bits  */
    EP_Status(EPNum, EP_TX_VALID | EP_RX_VALID);
}


/*
 *  Clear USB Device Endpoint Buffer
 *  Parameters:     EPNum: Device Endpoint Number
 *                      EPNum.0..3: Address
 *                      EPNum.7:    Dir
 *  Return Value:   None
 */
void USBD_ClearEPBuf (U32 EPNum)
{

}


/*
 *  Read USB Device Endpoint Data
 *  Parameters:     EPNum: Device Endpoint Number
 *                      EPNum.0..3: Address
 *                      EPNum.7:    Dir
 *                      pData: Pointer to Data Buffer
 *  Return Value:   Number of bytes read
 */
U32 USBD_ReadEP (U32 EPNum, U8 *pData, U32 size)
{
    /* Double Buffering is not yet supported    */
    U32 num, cnt, *pv, n;

    num = EPNum & 0x0F;

    pv  = (U32 *)(USB_PMA_ADDR + 2 * ((pBUF_DSCR + num)->ADDR_RX));
    cnt = (pBUF_DSCR + num)->COUNT_RX & EP_COUNT_MASK;
//  if(cnt > size)
//  {
//      cnt = size;
//  }
    for (n = 0; n < (cnt + 1) / 2; n++)
    {
        *((__packed U16 *)pData) = *pv++;
        pData += 2;
    }
    EP_Status(EPNum, EP_RX_VALID);

    return (cnt);
}

/*
 *  Write USB Device Endpoint Data
 *  Parameters:   EPNum: Device Endpoint Number
 *                      EPNum.0..3: Address
 *                      EPNum.7:    Dir
 *                   pData: Pointer to Data Buffer
 *                   cnt:   Number of bytes to write
 *  Return Value:   Number of bytes written
 */

U32 USBD_WriteEP (U32 EPNum, U8 *pData, U32 cnt)
{
    /* Double Buffering is not yet supported */
    U32 num, *pv, n;
    U16 statusEP;

    num = EPNum & 0x0F;

    pv  = (U32 *)(USB_PMA_ADDR + 2 * ((pBUF_DSCR + num)->ADDR_TX));
    for (n = 0; n < (cnt + 1) / 2; n++)
    {
        *pv++ = *((__packed U16 *)pData);
        pData += 2;
    }
    (pBUF_DSCR + num)->COUNT_TX = cnt;

    statusEP = EPxREG(num);
    if ((statusEP & EP_STAT_TX) != EP_TX_STALL)
    {   /* do not make EP valid if stalled */
        EP_Status(EPNum, EP_TX_VALID);
    }

    return (cnt);
}

/*
 *  Get USB Device Last Frame Number
 *  Parameters:     None
 *  Return Value:   Frame Number
 */
U32 USBD_GetFrame (void)
{
    return (FNR & FNR_FN);
}

#ifdef __RTX
U32 LastError;                        /* Last Error                      */

/*
 *  Get USB Last Error Code
 *  Parameters:   None
 *  Return Value:   Error Code
 */
U32 USBD_GetError (void)
{
    return (LastError);
}
#endif

/*
 *  USB Device Interrupt Service Routine
 */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
    USBD_SignalHandler();
}

void USBWakeUp_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line18);
}


void USBD_Handler(void)
{
    U32 istr, num, val;

    istr = ISTR;

    if (istr & ISTR_RESET)
    {   /* USB Reset Request */
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
        ISTR = ~ISTR_RESET;
    }

    if (istr & ISTR_SUSP)
    {   /* USB Suspend Request */
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
        ISTR = ~ISTR_SUSP;
    }

    if (istr & ISTR_WKUP)
    {   /* USB Wakeup */
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
        ISTR = ~ISTR_WKUP;
    }

    if (istr & ISTR_SOF)
    {   /* Start of Frame */
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
        ISTR = ~ISTR_SOF;
    }

    if (istr & ISTR_PMAOVR)
    {
#ifdef __RTX
        LastError = 2;
        if (USBD_RTX_DevTask)
        {   /* PMA Over/underrun */
            isr_evt_set(USBD_EVT_ERROR, USBD_RTX_DevTask);
        }
#else
        if (USBD_P_Error_Event)
        {
            USBD_P_Error_Event(2);
        }
#endif
        ISTR = ~ISTR_PMAOVR;
    }

    if (istr & ISTR_ERR)
    {   /* Error: No Answer, CRC Error, Bit Stuff Error, Frame Format Error */
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
        ISTR = ~ISTR_ERR;
    }

    while ((istr = ISTR) & ISTR_CTR)
    {   /* Endpoint Interrupts */
        ISTR = ~ISTR_CTR;

        num = istr & ISTR_EP_ID;

        val = EPxREG(num);
        if (val & EP_CTR_RX)
        {
            EPxREG(num) = val & ~EP_CTR_RX & EP_MASK;
#ifdef __RTX
            if (USBD_RTX_EPTask[num])
            {
                isr_evt_set((val & EP_SETUP) ? USBD_EVT_SETUP : USBD_EVT_OUT, USBD_RTX_EPTask[num]);
            }
#else
            if (USBD_P_EP[num])
            {
                USBD_P_EP[num]((val & EP_SETUP) ? USBD_EVT_SETUP : USBD_EVT_OUT);
            }
#endif
        }
        if (val & EP_CTR_TX)
        {
            EPxREG(num) = val & ~EP_CTR_TX & EP_MASK;
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
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}
