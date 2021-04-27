/**
 * @file    usbd.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2018, ARM Limited, All Rights Reserved
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

#include "usb_def.h"
#include "rl_usb.h"
#include "usbd_event.h"
#include "stm32f0xx_hal.h"
#include "util.h"

#define __NO_USB_LIB_C
#include "usb_config.c"

#define USBD_BTABLE_SIZE                (8 * (USBD_EP_NUM + 1))
#define USBD_GET_FLAG(__INTERRUPT__)    (((USB->ISTR) & (__INTERRUPT__)) == (__INTERRUPT__))
#define USBD_CLEAR_FLAG(__INTERRUPT__)  ((USB->ISTR) &= ~(__INTERRUPT__))

#define __wait_todo() util_assert(0)


void USBD_Init(void);
void USBD_Connect(BOOL con);
void USBD_Reset(void);
void USBD_WakeUpCfg(BOOL cfg);
void USBD_SetAddress(U32 adr, U32 setup);
void USBD_Configure(BOOL cfg);
void USBD_ConfigEP(USB_ENDPOINT_DESCRIPTOR *pEPD);
void USBD_DirCtrlEP(U32 dir);
void USBD_EnableEP(U32 EPNum);
void USBD_DisableEP(U32 EPNum);
void USBD_ResetEP(U32 EPNum);
void USBD_SetStallEP(U32 EPNum);
void USBD_ClrStallEP(U32 EPNum);
void USBD_ClearEPBuf(U32 EPNum);
U32 USBD_ReadEP(U32 EPNum, U8 *pData, U32 cnt);
U32 USBD_WriteEP(U32 EPNum, U8 *pData, U32 cnt);
U32 USBD_GetFrame(void);
U32 USBD_GetError(void);
void USBD_Handler(void);

/* see stm32f1xx_ll_usb.c */
#define PCD_ReadPMA         USB_ReadPMA
#define PCD_WritePMA        USB_WritePMA

uint16_t Buf_Offset = 0x18;



static PCD_EPTypeDef *USBD_GetEP(uint16_t ep_addr)
{
    static PCD_EPTypeDef IN_ep[USBD_EP_NUM + 1] = {0};
    static PCD_EPTypeDef OUT_ep[USBD_EP_NUM + 1] = {0};

    util_assert((0x7f & ep_addr) <= USBD_EP_NUM);

    if ((ep_addr & 0x80U) == 0x80U)
    {
        return &IN_ep[ep_addr & 0x7FU];
    }
    else
    {
        return &OUT_ep[ep_addr];
    }
}

static void _USBD_ConfigEP(uint16_t ep_addr, uint8_t ep_type, uint16_t ep_buf_size)
{
    util_assert(Buf_Offset + ep_buf_size < 512);
    PCD_EPTypeDef *ep = USBD_GetEP(ep_addr);
    ep->doublebuffer = 0;
    ep->pmaadress = Buf_Offset;
    ep->maxpacket = ep_buf_size;
    ep->type = ep_type;
    Buf_Offset += ep_buf_size;
    
    switch (ep_type)
    {
    case PCD_EP_TYPE_CTRL:
      PCD_SET_EPTYPE(USB, ep->num, USB_EP_CONTROL);
      break;
    case PCD_EP_TYPE_BULK:
      PCD_SET_EPTYPE(USB, ep->num, USB_EP_BULK);
      break;
    case PCD_EP_TYPE_INTR:
      PCD_SET_EPTYPE(USB, ep->num, USB_EP_INTERRUPT);
      break;
    case PCD_EP_TYPE_ISOC:
      PCD_SET_EPTYPE(USB, ep->num, USB_EP_ISOCHRONOUS);
      break;
    default:
      break;
    }  
}

static void USB_EnableInt(USB_TypeDef *USBx)
{
    uint32_t winterruptmask = 0;

    /* Set winterruptmask variable */
    winterruptmask = USB_CNTR_CTRM | USB_CNTR_WKUPM | USB_CNTR_SUSPM | USB_CNTR_RESETM;

    if (USBD_P_Error_Event)
    {
        winterruptmask |= USB_CNTR_ERRM | USB_CNTR_PMAOVRM;
    }

    if (USBD_P_SOF_Event)
    {
        winterruptmask |= USB_CNTR_SOFM | USB_CNTR_ESOFM;
    }

    /* Set interrupt mask */
    USBx->CNTR |= winterruptmask;
}

/* USB init function */
void USBD_Init(void)
{
    __HAL_RCC_USB_CLK_ENABLE();
    USB_DisableGlobalInt(USB);
    HAL_NVIC_SetPriority(USB_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_IRQn);
}

void USBD_Connect(BOOL con)
{
    USB_DisableGlobalInt(USB);
    if (con)
    {
        USB->CNTR = USB_CNTR_FRES;
        HAL_Delay(1);
        USB->ISTR = 0;
        USB->CNTR = USB_CNTR_RESETM;
        USB->BTABLE = BTABLE_ADDRESS;
        
        USB_EnableGlobalInt (USB);
        
        /* Enabling DP Pull-Down bit to Connect internal pull-up on USB DP line */
        USB->BCDR |= USB_BCDR_DPPU;
    }
    else
    {
        USB->CNTR = USB_CNTR_FRES | USB_CNTR_PDWN;
        
        /* Disable DP Pull-Down bit*/
        USB->BCDR &= (uint16_t)(~(USB_BCDR_DPPU));
    }
}

void USBD_Reset(void)
{
    for (int index = 0; index <= USBD_EP_NUM; index++)
    {
        PCD_EPTypeDef *ep = USBD_GetEP(index | 0x80);
        ep->is_in = 1;
        ep->num = index;
        ep->type = PCD_EP_TYPE_CTRL;
        ep->maxpacket = 0;
        ep->xfer_buff = 0;
        ep->xfer_len = 0;
        ep = USBD_GetEP(index);
        ep->is_in = 0;
        ep->num = index;
        ep->type = PCD_EP_TYPE_CTRL;
        ep->maxpacket = 0;
        ep->xfer_buff = 0;
        ep->xfer_len = 0;
    }

    Buf_Offset = USBD_BTABLE_SIZE;
    _USBD_ConfigEP(0x00, PCD_EP_TYPE_CTRL, USBD_MAX_PACKET0);
    _USBD_ConfigEP(0x80, PCD_EP_TYPE_CTRL, USBD_MAX_PACKET0);
    USBD_EnableEP(0x00);
    USBD_EnableEP(0x80);
    USB_EnableInt(USB);
}

void USBD_WakeUpCfg(BOOL cfg)
{
    __wait_todo();
}

void USBD_SetAddress(U32 adr, U32 setup)
{
    if (!adr)
    {
        USB->DADDR = USB_DADDR_EF;
        return;
    }

    if (setup)
    {
        return;
    }

    if (adr)
    {
        USB->DADDR = (adr | USB_DADDR_EF);
    }
}

void USBD_Configure(BOOL cfg)
{
    if (cfg == __TRUE)
    {
        return;
    }
    Buf_Offset = USBD_BTABLE_SIZE + USBD_MAX_PACKET0 * 2;
}

void USBD_ConfigEP(USB_ENDPOINT_DESCRIPTOR *pEPD)
{
    uint8_t type;
    switch (pEPD->bmAttributes & USB_ENDPOINT_TYPE_MASK)
    {
    case USB_ENDPOINT_TYPE_CONTROL:
        type = PCD_EP_TYPE_CTRL;
        break;
    case USB_ENDPOINT_TYPE_ISOCHRONOUS:
        type = PCD_EP_TYPE_ISOC;
        break;
    case USB_ENDPOINT_TYPE_BULK:
        type = PCD_EP_TYPE_BULK;
        break;
    case USB_ENDPOINT_TYPE_INTERRUPT:
        type = PCD_EP_TYPE_INTR;
        break;
    }
    
    _USBD_ConfigEP(pEPD->bEndpointAddress, type, pEPD->wMaxPacketSize);
}

void USBD_DirCtrlEP(U32 dir)
{
}

void USBD_EnableEP(U32 EPNum)
{
    PCD_EPTypeDef *ep = USBD_GetEP(EPNum);

    PCD_SET_EP_ADDRESS(USB, ep->num, ep->num);
    
    if (ep->is_in)
    {
      /*Set the endpoint Transmit buffer address */
      PCD_SET_EP_TX_ADDRESS(USB, ep->num, ep->pmaadress);
      PCD_CLEAR_TX_DTOG(USB, ep->num);
      /* Configure NAK status for the Endpoint*/
      PCD_SET_EP_TX_STATUS(USB, ep->num, USB_EP_TX_NAK); 
    }
    else
    {
      /*Set the endpoint Receive buffer address */
      PCD_SET_EP_RX_ADDRESS(USB, ep->num, ep->pmaadress);
      /*Set the endpoint Receive buffer counter*/
      PCD_SET_EP_RX_CNT(USB, ep->num, ep->maxpacket);
      PCD_CLEAR_RX_DTOG(USB, ep->num);
      /* Configure VALID status for the Endpoint*/
      PCD_SET_EP_RX_STATUS(USB, ep->num, USB_EP_RX_VALID);
    }
}

void USBD_DisableEP(U32 EPNum)
{
    // USB_DeactivateEndpoint(USB, USBD_GetEP(EPNum));
    PCD_EPTypeDef *ep = USBD_GetEP(EPNum);
    
    if (ep->is_in==0U)
    {
      /* Clear the data toggle bits for the endpoint IN/OUT*/
      PCD_CLEAR_RX_DTOG(USB, ep->num);
      PCD_CLEAR_TX_DTOG(USB, ep->num);
      
      /* Reset value of the data toggle bits for the endpoint out*/
      PCD_TX_DTOG(USB, ep->num);
      
      PCD_SET_EP_RX_STATUS(USB, ep->num, USB_EP_RX_DIS);
      PCD_SET_EP_TX_STATUS(USB, ep->num, USB_EP_TX_DIS);
    }
    else
    {
      /* Clear the data toggle bits for the endpoint IN/OUT*/
      PCD_CLEAR_RX_DTOG(USB, ep->num);
      PCD_CLEAR_TX_DTOG(USB, ep->num);
      PCD_RX_DTOG(USB, ep->num);
      /* Configure DISABLE status for the Endpoint*/
      PCD_SET_EP_TX_STATUS(USB, ep->num, USB_EP_TX_DIS);
      PCD_SET_EP_RX_STATUS(USB, ep->num, USB_EP_RX_DIS);
    }
}

void USBD_ResetEP(U32 EPNum)
{
    if (EPNum & 0x80)
    {
        PCD_CLEAR_TX_DTOG(USB, EPNum & 0x7f);
    }
    else
    {
        PCD_CLEAR_RX_DTOG(USB, EPNum);
    }
}

/**
 * @brief Set Stall for USB Device Endpoint
 */
void USBD_SetStallEP(U32 EPNum)
{
    if (EPNum & 0x80)
    {
        PCD_SET_EP_TX_STATUS(USB, EPNum & 0x7f, USB_EP_TX_STALL);
    }
    else
    {
        PCD_SET_EP_RX_STATUS(USB, EPNum, USB_EP_RX_STALL);
    }
}

/**
 * @brief Clear Stall for USB Device Endpoint
 */
void USBD_ClrStallEP(U32 EPNum)
{
    USBD_ResetEP (EPNum);    /* reset DTog Bits	*/
    
    if (EPNum & 0x80)
    {
        PCD_SET_EP_TX_STATUS(USB, EPNum & 0x7f, USB_EP_TX_VALID);
    }
    else
    {
        PCD_SET_EP_RX_STATUS(USB, EPNum, USB_EP_RX_VALID);
    }
}

/**
 * @brief Clear USB Device Endpoint Buffer
 */
void USBD_ClearEPBuf(U32 EPNum)
{
    
}

U32 USBD_ReadEP(U32 EPNum, U8 *pData, U32 cnt)
{
    PCD_EPTypeDef *ep = USBD_GetEP(EPNum);
    U32 count = 0;
    count = MIN(cnt, PCD_GET_EP_RX_CNT(USB, ep->num));
    PCD_ReadPMA(USB, pData, ep->pmaadress, count);
    PCD_SET_EP_RX_STATUS(USB, ep->num, USB_EP_RX_VALID);
    return count;
}

U32 USBD_WriteEP(U32 EPNum, U8 *pData, U32 cnt)
{
    PCD_EPTypeDef *ep = USBD_GetEP(EPNum);
    U32 count = 0;
    count = MIN(cnt, ep->maxpacket);
    PCD_WritePMA(USB, pData, ep->pmaadress, count);
    PCD_SET_EP_TX_CNT(USB, ep->num, count);
    PCD_SET_EP_TX_STATUS(USB, ep->num, USB_EP_TX_VALID);
    return count;
}

/**
 * @brief Get USB Device Last Frame Number
 */
U32 USBD_GetFrame(void)
{
    //__wait_todo();
    return 0;
}

/**
 * @brief Get USB Last Error Code
 */
U32 USBD_GetError(void)
{
    __wait_todo();
    return 0;
}

/**
 * @brief USB Device Interrupt Service Routine
 */
void USBD_Handler(void)
{
    if (USBD_GET_FLAG(USB_ISTR_CTR))
    {
        uint8_t epindex = 0;
        __IO uint16_t wIstr = 0;
        U32 event;

        while (((wIstr = USB->ISTR) & USB_ISTR_CTR) != 0)
        {
            epindex = (uint8_t)(wIstr & USB_ISTR_EP_ID);

            if (wIstr & USB_ISTR_DIR)
            {
                event = USBD_EVT_OUT;
                if (epindex == 0 && (PCD_GET_ENDPOINT(USB, PCD_ENDP0) & USB_EP_SETUP))
                {
                    event = USBD_EVT_SETUP;
                }
                PCD_CLEAR_RX_EP_CTR(USB, epindex);
            }
            else
            {
                event = USBD_EVT_IN;
                PCD_CLEAR_TX_EP_CTR(USB, epindex);
            }

            if (USBD_P_EP[epindex])
            {
                USBD_P_EP[epindex](event);
            }
        }
    }

    if (USBD_GET_FLAG(USB_ISTR_RESET))
    {
        USBD_CLEAR_FLAG(USB_ISTR_RESET);
        USBD_Reset();
        usbd_reset_core();
        if (USBD_P_Reset_Event)
        {
            USBD_P_Reset_Event();
        }
        USBD_SetAddress(0, 0);
    }

    if (USBD_GET_FLAG(USB_ISTR_PMAOVR))
    {
        if (USBD_P_Error_Event)
        {
            USBD_P_Error_Event(2);
        }
        USBD_CLEAR_FLAG(USB_ISTR_PMAOVR);
    }

    if (USBD_GET_FLAG(USB_ISTR_ERR))
    {
        if (USBD_P_Error_Event)
        {
            USBD_P_Error_Event(1);
        }
        USBD_CLEAR_FLAG(USB_ISTR_ERR);
    }

    if (USBD_GET_FLAG(USB_ISTR_WKUP))
    {
        USB->CNTR &= ~(USB_CNTR_LPMODE);
        USB->CNTR &= ~(USB_CNTR_FSUSP);

        if (USBD_P_Resume_Event)
        {
            USBD_P_Resume_Event();
        }

        USBD_CLEAR_FLAG(USB_ISTR_WKUP);
    }

    if (USBD_GET_FLAG(USB_ISTR_SUSP))
    {
        /* Force low-power mode in the macrocell */
        USB->CNTR |= USB_CNTR_FSUSP;

        /* clear of the ISTR bit must be done after setting of CNTR_FSUSP */
        USBD_CLEAR_FLAG(USB_ISTR_SUSP);

        USB->CNTR |= USB_CNTR_LPMODE;
        if (USBD_GET_FLAG(USB_ISTR_WKUP) == 0U)
        {
            if (USBD_P_Suspend_Event)
            {
                USBD_P_Suspend_Event();
            }
        }
    }

    if (USBD_GET_FLAG(USB_ISTR_SOF))
    {
        USBD_CLEAR_FLAG(USB_ISTR_SOF);
        if (USBD_P_SOF_Event)
        {
            USBD_P_SOF_Event();
        }
    }

    if (USBD_GET_FLAG(USB_ISTR_ESOF))
    {
        USBD_CLEAR_FLAG(USB_ISTR_ESOF);
    }

    HAL_NVIC_EnableIRQ(USB_IRQn);
}

void USB_IRQHandler(void)
{
    HAL_NVIC_DisableIRQ(USB_IRQn);
    USBD_SignalHandler();
}
