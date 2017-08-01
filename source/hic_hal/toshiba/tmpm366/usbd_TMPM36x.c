/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "RTL.h"
#include "rl_usb.h"
#include "usb_hw.h"
#include "string.h"

#define __NO_USB_LIB_C
#include "usb_config.c"
#include "gpio.h"

// Endpoint Transfer Information
uint32_t gEPxMaxPacket[USBD_EP_NUM + 1];         // Endpoint Buffer Lenght
USBD_EPxConfig gEPxConfigArray[USBD_EP_NUM + 1]; // Endpoint Configuration

// Endpoint 0 Direction
volatile uint8_t bmDirection = 0x00;

volatile uint8_t setup_stage = 0x00;

#define SOFTWARE_DELAY         0x20000U

void usbd_wait_1ms(void)
{
    volatile uint32_t delay = SOFTWARE_DELAY;
    while (delay) {
        delay--;
    }
    return;
}

// Usb interrupt enable/disable
// ena: enable/disable
//   0: disable interrupt
//   1: enable interrupt
void USBD_IntrEna(void)
{
    NVIC_EnableIRQ(INTUSB_IRQn); // Enable USB interrupt
}

// USB Device Initialize Function
// Called by the User to initialize USB Device
//    None
void USBD_Init (void)
{
    USBD_PowerCtrl pwr = { 0U };

    TSB_CG_USBCTL_USBCLKSEL = 0U;
    TSB_CG->USBCTL = 0x100U; // enable USB Clock

    TSB_PG_IE_PG5IE = 1U;

    // pin PE4 control the pull up of D+, must be set output '1'
    TSB_PE_CR_PE4C = 1U;
    TSB_PE_DATA_PE4 = 1U;

    USBD_SetINTMask(USBD_INT_USB_RESET_END, ENABLE);
    USBD_SetINTMask(USBD_INT_USB_RESET, ENABLE);

    //  UDPWCTL Power Reset and
    //  PHY Reset & Suspend: 1ms
    pwr = USBD_GetPowerCtrlStatus();
    pwr.Bit.PW_Resetb = 0U;
    pwr.Bit.PHY_Resetb = 0U;
    pwr.Bit.PHY_Suspend = 1U;
    USBD_SetPowerCtrl(pwr);
    usbd_wait_1ms();

    // PHY Reset off : 1ms
    pwr = USBD_GetPowerCtrlStatus();
    pwr.Bit.PHY_Resetb = 1U;
    pwr.Bit.PHY_Suspend = 1U;
    USBD_SetPowerCtrl(pwr);
    usbd_wait_1ms();

    //  PHY Suspend off : 1ms
    pwr = USBD_GetPowerCtrlStatus();
    pwr.Bit.PHY_Suspend = 0U;
    USBD_SetPowerCtrl(pwr);
    usbd_wait_1ms();
    usbd_wait_1ms();

    // UDPWCTL Power Reset Off: 1ms
    pwr = USBD_GetPowerCtrlStatus();
    pwr.Bit.PW_Resetb = 1U;
    USBD_SetPowerCtrl(pwr);
    usbd_wait_1ms();
    usbd_wait_1ms();
    pwr = USBD_GetPowerCtrlStatus();

    // clear pending UDC2 interrupt and disable INT for STATUS_NAK
    USBD_WriteUDC2Reg(UDC2_INT, 0x02FFU);

    USBD_SetEPCMD(USBD_EP0, USBD_CMD_ALL_EP_INVALID);
    USBD_SetEPCMD(USBD_EP0, USBD_CMD_USB_READY);

    USBD_IntrEna();

    return;
}

// USB Device Connect Function
// Called by the User to Connect/Disconnect USB Device
//  Parameters:      con:   Connect/Disconnect
//  Return Value:    None
void USBD_Connect (BOOL con)
{
    if (con) {
        // pin PE4 control the pull up of D+, must be set output '1'
        TSB_PE_DATA_PE4 = 1U;
        USBD_SetEPCMD(USBD_EP0, USBD_CMD_ALL_EP_INVALID);
        USBD_SetEPCMD(USBD_EP0, USBD_CMD_USB_READY);
    } else {
        // pin PE4 control the pull up of D+, must be set output '0'
        TSB_PE_DATA_PE4 = 0U;
        USBD_SetEPCMD(USBD_EP0, USBD_CMD_ALL_EP_INVALID);
    }
    return;
}

// USB Device Reset Function
// Called automatically on USB Device Reset
//  Return Value:    None
void USBD_Reset (void)
{
    uint8_t i = 0x00;

    // Initialize endpoint information
    for (i = 0; i < USBD_EP_NUM + 1; ++i) {
        gEPxMaxPacket[i] = USBD_MAX_PACKET0;
        gEPxConfigArray[i].All = 0x00U;
    }

    return;
}

// USB Device Suspend Function
//  Called automatically on USB Device Suspend
//   Return Value:    None
void USBD_Suspend (void)
{
    // Performed by Hardware
    return;
}

// USB Device Resume Function
//  Called automatically on USB Device Resume
//   Return Value:    None
void USBD_Resume (void)
{
    // Performed by Hardware
    return;
}

// USB Device Remote Wakeup Function
//  Called automatically on USB Device Remote Wakeup
//   Return Value:    None
void USBD_WakeUp (void)
{
    return;
}

// USB Device Remote Wakeup Configuration Function
//   Parameters:      cfg:   Device Enable/Disable
//   Return Value:    None
void USBD_WakeUpCfg (BOOL cfg)
{
    return;
}

// USB Device Set Address Function
//   Parameters:      adr:   USB Device Address
//   Return Value:    None
void USBD_SetAddress (U32 adr, U32 setup)
{
    if (!setup) {
        uint32_t addr = adr & 0x7FU;
        addr |= (0x01U << 0x09); // Set Address-State: Addressed
        USBD_WriteUDC2Reg(UDC2_ADDR, addr);
    } else {
        // Status to Host
    }

    return;
}

// USB Device Configure Function
//   Parameters:      cfg:   Device Configure/Deconfigure
//   Return Value:    None
void USBD_Configure (BOOL cfg)
{
    uint32_t addr = 0x00U;
    if (cfg) {
        USBD_ReadUDC2Reg(UDC2_ADDR, &addr);
        addr &= ~0x700U; // Clear current state
        addr |= (0x01U << 0x0A); // Set Address-State: Configure
        USBD_WriteUDC2Reg(UDC2_ADDR, addr);
    }

    return;
}

// Configure USB Device Endpoint according to Descriptor
//   Parameters:      pEPD:  Pointer to Device Endpoint Descriptor
//   Return Value:    None
void USBD_ConfigEP (USB_ENDPOINT_DESCRIPTOR *pEPD)
{
    uint32_t addr = pEPD->bEndpointAddress;
    uint32_t type = pEPD->bmAttributes & USB_ENDPOINT_TYPE_MASK;
    uint32_t epNum = addr & 0x0F;
    gEPxMaxPacket[epNum] = pEPD->wMaxPacketSize;

    // Endpoint In
    if (addr & 0x80) {
        gEPxConfigArray[epNum].All &= ~0xFF; // Clear
        gEPxConfigArray[epNum].All |= 0x80; // Set endpoint direction
        gEPxConfigArray[epNum].All |= (type << 2); // Set endpoint transfer type
    }
    // Endpoint Out
    else {
        gEPxConfigArray[epNum].All &= ~0xFF; // Clear
        gEPxConfigArray[epNum].All |= (type << 2); // Set endpoint transfer type
    }

    USBD_ResetEP(epNum);
}

// Set Direction for USB Device Control Endpoint
//   Parameters:      dir:   Out (dir == 0), In (dir <> 0)
//   Return Value:    None
void USBD_DirCtrlEP (U32 dir)
{
    bmDirection = (uint8_t)dir;
}

// Enable USB Device Endpoint
//   Parameters:      EPNum: Device Endpoint Number
//                      EPNum.0..3: Address
//                      EPNum.7:    Dir
//   Return Value:    None
void USBD_EnableEP (U32 EPNum)
{
    uint32_t epNum = EPNum & 0x0F;
    USBD_SetEPCMD((USBD_EPx)epNum, USBD_CMD_EP_ENABLE);
}

// Disable USB Device Endpoint
//   Parameters:      EPNum: Device Endpoint Number
//                      EPNum.0..3: Address
//                      EPNum.7:    Dir
//   Return Value:    None
void USBD_DisableEP (U32 EPNum)
{
    uint32_t epNum = EPNum & 0x0F;
    USBD_SetEPCMD((USBD_EPx)epNum, USBD_CMD_EP_DISABLE);
}

// Reset USB Device Endpoint
//   Parameters:      EPNum: Device Endpoint Number
//                      EPNum.0..3: Address
//                      EPNum.7:    Dir
//   Return Value:    None
void USBD_ResetEP (U32 EPNum)
{
    uint32_t tmpAddr = 0U;
    uint32_t epNum = EPNum & 0x0F;

    if ((USBD_EPx) epNum == USBD_EP0) {
        tmpAddr = (uint32_t)UDC2_EP0MAXPACKETSIZE;
        USBD_WriteUDC2Reg(tmpAddr, gEPxMaxPacket[epNum]);
    } else {
        tmpAddr = (uint32_t)UDC2_EP1MAXPACKETSIZE + (0x10U * (epNum - 1U));
        USBD_WriteUDC2Reg(tmpAddr, gEPxMaxPacket[epNum]);
        USBD_ConfigEPx((USBD_EPx)epNum, gEPxConfigArray[epNum]);
    }

    USBD_SetEPCMD((USBD_EPx)epNum, USBD_CMD_SET_DATA0);
    USBD_SetEPCMD((USBD_EPx)epNum, USBD_CMD_EP_RESET);
    USBD_SetEPCMD((USBD_EPx)epNum, USBD_CMD_EP_ENABLE);
}

// Set Stall for USB Device Endpoint
//   Parameters:      EPNum: Device Endpoint Number
//                      EPNum.0..3: Address
//                      EPNum.7:    Dir
//   Return Value:    None
void USBD_SetStallEP (U32 EPNum)
{
    uint32_t epNum = EPNum & 0x0F;
    USBD_SetEPCMD((USBD_EPx) epNum, USBD_CMD_EP_STALL);
}

// Clear Stall for USB Device Endpoint
//   Parameters:      EPNum: Device Endpoint Number
//                      EPNum.0..3: Address
//                      EPNum.7:    Dir
//   Return Value:    None
void USBD_ClrStallEP (U32 EPNum)
{
    uint32_t epNum = EPNum & 0x0F;
    USBD_SetEPCMD((USBD_EPx) epNum, USBD_CMD_EP_RESET);
}

// Clear USB Device Endpoint Buffer
//   Parameters:      EPNum: Device Endpoint Number
//                      EPNum.0..3: Address
//                      EPNum.7:    Dir
//   Return Value:    None
void USBD_ClearEPBuf (U32 EPNum)
{
    uint32_t epNum = EPNum & 0x0F;
    USBD_SetEPCMD((USBD_EPx) epNum, USBD_CMD_EP_FIFO_CLEAR);
}

// Read USB Device Endpoint Data
//   Parameters:      EPNum: Device Endpoint Number
//                      EPNum.0..3: Address
//                      EPNum.7:    Dir
//                    pData: Pointer to Data Buffer
//   Return Value:    Number of bytes read
U32 USBD_ReadEP (U32 EPNum, U8 *pData, U32 cnt)
{
    uint32_t length = 0x00U;        // Read data length
    uint16_t *idx16;                // Data buffer pointer
    uint32_t tmp32 = 0x00U;         // Read data buffer from EPx_FIFO
    uint32_t epNum = 0x00U;
    uint32_t ulTransSize = 0x00U;

    epNum |= EPNum & 0x0FU;

    // Check endpoint (IN)
    if (((USBD_EPx)epNum != USBD_EP0) && ((USBD_EPx)epNum != USBD_EP2)
            && ((USBD_EPx)epNum != USBD_EP4) && ((USBD_EPx)epNum != USBD_EP6)) {
        // Illegal endpoint (not OUT)
        return 0;
    }

    // Check buffer pointer
    if (pData == (uint8_t*)NULL) {
        // Illegal buffer pointer
        return 0;
    }

    idx16 = (uint16_t *) pData;

    // Setup Packet
    if ((epNum == 0) && (setup_stage == 1)) {
        // Clear setup stage flag
        setup_stage = 0;

        // Get device requests data
        USBD_ReadUDC2Reg(UDC2_BREQ_BMREQTYPE, &tmp32);
        // Byte 0: bmRequestType, Byte 1: bRequest
        *idx16 = (uint16_t)(tmp32 & 0xFFFFU);
        idx16++;
        ulTransSize += 2U;

        // wValue
        USBD_ReadUDC2Reg(UDC2_WVALUE, &tmp32);
        *idx16 = (uint16_t)(tmp32 & 0xFFFFU);
        idx16++;
        ulTransSize += 2U;

        // wIndex
        USBD_ReadUDC2Reg(UDC2_WINDEX, &tmp32);
        *idx16 = (uint16_t)(tmp32 & 0xFFFFU);
        idx16++;
        ulTransSize += 2U;

        // wLength
        USBD_ReadUDC2Reg(UDC2_WLENGTH, &tmp32);
        *idx16 = (uint16_t)(tmp32 & 0xFFFFU);
        idx16++;
        ulTransSize += 2U;

        // Recognize setup stage (Received device request)
        USBD_SetEPCMD(USBD_EP0, USBD_CMD_SETUP_RECEIVED);
    }
    // OUT packet
    else {
        // Get the valid data length that are stored in the EPx_FIFO
        NVIC_DisableIRQ(INTUSB_IRQn);
        USBD_ReadUDC2Reg(UDC2_EPxDATASIZE((USBD_EPx)epNum), &length);
        NVIC_EnableIRQ(INTUSB_IRQn);

        // Read data from EPx_FIFO
        while (length != 0U) {
            NVIC_DisableIRQ(INTUSB_IRQn);
            USBD_ReadUDC2Reg(UDC2_EPxFIFO((USBD_EPx)epNum), &tmp32);
            NVIC_EnableIRQ(INTUSB_IRQn);
            if (length == 1U) {
                *(uint8_t *)idx16 = (uint8_t)tmp32;
                length = 0U;
                ulTransSize++;
            } else {
                *idx16 = (uint16_t)tmp32;
                idx16++;
                length -= 2U;       // Size of uint16_t
                ulTransSize += 2U;
            }
        }
    }

    return ulTransSize;
}

// Write USB Device Endpoint Data
//   Parameters:      EPNum: Endpoint Number
//                      EPNum.0..3: Address
//                      EPNum.7:    Dir
//                    pData: Pointer to Data Buffer
//                    cnt:   Number of bytes to write
//   Return Value:    Number of bytes written
U32 USBD_WriteEP (U32 EPNum, U8 *pData, U32 cnt)
{
    uint32_t epNum = 0x00;

    uint32_t length;                // Write data length
    uint32_t tmp32;                 // Write data buffer to EPx_FIFO
    uint16_t *idx16;                // Data buffer pointer
    uint8_t  chardata;              // Write fraction data
    uint32_t ulTransSize = 0x00U;

    epNum |= EPNum & 0x0F;

    // Check endpoint (OUT)
    if (((USBD_EPx)epNum != USBD_EP0) && ((USBD_EPx)epNum != USBD_EP1)
            && ((USBD_EPx)epNum != USBD_EP3) && ((USBD_EPx)epNum != USBD_EP5) && ((USBD_EPx)epNum != USBD_EP7)) {
        // Illegal endpoint (not IN)
        return 0;
    }

    // Check buffer pointer
    if (pData == (uint8_t *)NULL && cnt != 0) {
        // Illegal buffer pointer
        return 0;
    }

    // Get send request buffer
    idx16 = (uint16_t *) pData;

    // Judgement of remain total length
    if (cnt >= gEPxMaxPacket[epNum]) {
        // Get payload size
        length = gEPxMaxPacket[epNum];
        // Write data to EPx_FIFO
        while (length > 0U) {
            tmp32 = (uint32_t)(*idx16);
            USBD_WriteUDC2Reg(UDC2_EPxFIFO((USBD_EPx)epNum), tmp32);
            idx16++;
            length -= 2U;       // Size of uint16
            ulTransSize += 2U;
        }
    } else if (cnt != 0) {
        length = cnt;
        // Write data to EPx_FIFO
        while (length != 0U) {
            if (length == 1U) {
                // Write fraction data
                chardata = (uint8_t)(*idx16);
                *((uint8_t *)UDC2_EPxFIFO((USBD_EPx)epNum)) = chardata;
                length = 0U;
                ulTransSize++;
            } else {
                tmp32 = (uint32_t)(*idx16);
                USBD_WriteUDC2Reg(UDC2_EPxFIFO((USBD_EPx)epNum), tmp32);
                idx16++;
                length -= 2U;   // Size of uint16
                ulTransSize += 2U;
            }
        }

        // Enable interrupt STATUS_NAK
        USBD_SetINTMask(USBD_INT_STATUS_NAK, ENABLE);
        // Notification short packet
        USBD_SetEPCMD((USBD_EPx)epNum, USBD_CMD_EP_EOP);
    } else {
        // Enable interrupt STATUS_NAK
        USBD_SetINTMask(USBD_INT_STATUS_NAK, ENABLE);
        USBD_SetEPCMD((USBD_EPx)epNum, USBD_CMD_EP_TX_0DATA);
    }

    return ulTransSize;
}

// Get USB Device Last Frame Number
//   Parameters:      None
//   Return Value:    Frame Number
U32 USBD_GetFrame (void)
{
    uint32_t frame = 0x00U;
    USBD_ReadUDC2Reg(UDC2_FRAME, &frame);
    return (0x7FFU & frame);
}

// USB Device Interrupt Service Routine
void INTUSB_IRQHandler(void)
{
    NVIC_DisableIRQ(INTUSB_IRQn);
    USBD_SignalHandler();
}

// USB Device Service Routine
//  Called by the User to Service Routine
//   Return Value:    None
void USBD_Handler (void)
{
    USBD_INTStatus tmp = { 0U };
    tmp = USBD_GetINTStatus();

    // reset interrupt
    if (tmp.Bit.USB_Reset) {
        USBD_ClearINT(USBD_INT_USB_RESET);
    }

    if (tmp.Bit.USB_Reset_End) {
        USBD_ClearINT(USBD_INT_USB_RESET_END);
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
    }

    // suspend/resume interrupt
    if (tmp.Bit.Suspend_Resume) {
        USBD_PowerCtrl pwr = { 0U };

        USBD_ClearINT(USBD_INT_SUSPEND_RESUME);

        pwr = USBD_GetPowerCtrlStatus();

        if (pwr.Bit.Suspend_x) { // Resume event
            USBD_Resume();
#ifdef __RTX
            if (USBD_RTX_DevTask) {
                isr_evt_set(USBD_EVT_RESUME, USBD_RTX_DevTask);
            }
#else
            if (USBD_P_Resume_Event) {
                USBD_P_Resume_Event();
            }
#endif
        } else { // Suspend event
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
        }
    }

    // Start Of Frame
    if (tmp.Bit.SOF) {
        USBD_ClearINT(USBD_INT_SOF);
#ifdef __RTX
        if (USBD_RTX_DevTask) {
            isr_evt_set(USBD_EVT_SOF, USBD_RTX_DevTask);
        }
#else
        if (USBD_P_SOF_Event) {
            USBD_P_SOF_Event();
        }
#endif
    }

    // Setup packet
    if (tmp.Bit.Setup) {
        USBD_ClearINT(USBD_INT_SETUP);
        setup_stage = 1;

        // Enable interrupt STATUS_NAK
        USBD_SetINTMask(USBD_INT_STATUS_NAK, ENABLE);

#ifdef __RTX
        if (USBD_RTX_EPTask[0]) {
            isr_evt_set(USBD_EVT_SETUP, USBD_RTX_EPTask[0]);
        }
#else
        if (USBD_P_EP[0]) {
            USBD_P_EP[0](USBD_EVT_SETUP);
        }
#endif
    }

    if (tmp.Bit.Status_NAK) {
        USBD_ClearINT(USBD_INT_STATUS_NAK);
        USBD_SetEPCMD(USBD_EP0, USBD_CMD_SETUP_FIN);
        // Disable interrupt STATUS_NAK
        USBD_SetINTMask(USBD_INT_STATUS_NAK, DISABLE);
    }

    if (tmp.Bit.Status) {
        USBD_ClearINT(USBD_INT_STATUS);

#ifdef __RTX
        if (USBD_RTX_EPTask[0]) {
            isr_evt_set(USBD_EVT_IN, USBD_RTX_EPTask[0]);
        }
#else
        if (USBD_P_EP[0]) {
            USBD_P_EP[0](USBD_EVT_IN);
        }
#endif
    }

    if (tmp.Bit.NAK) {
        USBD_ClearINT(USBD_INT_NAK);
    }

    if (tmp.Bit.Rx_Zero) {
        USBD_ClearINT(USBD_INT_RX_ZERO);
    }

    // Endpoint 0 Interrupt
    if (tmp.Bit.EP0) {
        USBD_ClearINT(USBD_INT_EP0);
        if (bmDirection) {
#ifdef __RTX
            if (USBD_RTX_EPTask[0]) {
                isr_evt_set(USBD_EVT_IN, USBD_RTX_EPTask[0]);
            }
#else
            if (USBD_P_EP[0]) {
                USBD_P_EP[0](USBD_EVT_IN);
            }
#endif
        } else {
#ifdef __RTX
            if (USBD_RTX_EPTask[0]) {
                isr_evt_set(USBD_EVT_OUT, USBD_RTX_EPTask[0]);
            }
#else
            if (USBD_P_EP[0]) {
                USBD_P_EP[0](USBD_EVT_OUT);
            }
#endif
        }
    }

    if (tmp.Bit.EP) {
        uint32_t tmp = 0U;
        uint8_t i;

        // Read EPx ACK interruption
        USBD_ReadUDC2Reg(UDC2_INTEP, &tmp);

        // Clear i_ep
        USBD_ClearINT(USBD_INT_EP);

        tmp >>= 1U;

        // Read/Write process
        for (i = 1U; i < USBD_EP_NUM + 1; ++i) {
            if (tmp & 0x01U) {

                // EPi ACK interruption
                if (i & 0x01U) {
                    // IN
#ifdef __RTX
                    if (USBD_RTX_EPTask[i]) {
                        isr_evt_set(USBD_EVT_IN,  USBD_RTX_EPTask[i]);
                    }
#else
                    if (USBD_P_EP[i]) {
                        USBD_P_EP[i](USBD_EVT_IN);
                    }
#endif
                } else {
                    // OUT
#ifdef __RTX
                    if (USBD_RTX_EPTask[i]) {
                        isr_evt_set(USBD_EVT_OUT, USBD_RTX_EPTask[i]);
                    }
#else
                    if (USBD_P_EP[i]) {
                        USBD_P_EP[i](USBD_EVT_OUT);
                    }
#endif
                }
            } else {
                // Do nothing
            }
            tmp >>= 1U;
        }
    }
    NVIC_EnableIRQ(INTUSB_IRQn);
}
