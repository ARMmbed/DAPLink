/**
 * @file    usbd_LPC55xx.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2020, ARM Limited, All Rights Reserved
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

#include "rl_usb.h"
#include "device.h"
#include "compiler.h"
#include "util.h"
#include "fsl_usb.h"
#include "usb_phy.h"
#include "fsl_clock.h"
#include "fsl_iocon.h"
#include "hic_init.h"

#define __NO_USB_LIB_C
#include "usb_config.c"

// Warning: this driver does *not* correctly support either isochronous endpoints or
//          interrupt endpoints in rate feedback mode!

// Number of logical endpoints.
#define USBD_MAX_EPS (FSL_FEATURE_USBHSD_EP_NUM)

// Mask for bit set on EP number to indicate IN EP.
#define USB_IN_MASK         (0x80)
// Mask to extract logical EP number from physical.
#define USB_LOGICAL_EP_MASK (0x7f)

#define EP_BUF_ACTIVE       (1UL << 31)
#define EP_DISABLED         (1UL << 30)
#define EP_STALL            (1UL << 29)
#define EP_TOGGLE_RESET     (1UL << 28)
#define EP_RF_TV            (1UL << 27)
#define EP_TYPE             (1UL << 26) // 0=generic, 1=periodic
#define EP_NBYTES_MASK      (0x03fff800)
#define EP_NBYTES_SHIFT     (11)
#define EP_BUF_OFFSET_MASK  (0x000007ff)
#define EP_BUF_OFFSET_SHIFT (0)

// DEVCMDSTAT Speed field values.
#define DEVCMDSTAT_SPEED_FULL (0x1)
#define DEVCMDSTAT_SPEED_HIGH (0x2)

// Alignment and rounding size for endpoint buffers.
#define MIN_BUF_SIZE        (64)

#define N_BYTES(n)         (((n) << EP_NBYTES_SHIFT) & EP_NBYTES_MASK)
#define BUF_ADDR(s_next_ep_buf_addr)     (((s_next_ep_buf_addr) >> 6) & EP_BUF_OFFSET_MASK)

#define EP_OUT_IDX(EPNum)  (EPNum * 2    )
#define EP_IN_IDX(EPNum)   (EPNum * 2 + 1)

#define EP_LIST_BASE       (0x40100000)
#define EP_BUF_BASE        (uint32_t)(EP_LIST_BASE + 0x100)

#define EP0_OUT_BUF_OFFSET      (0)
#define EP0_SETUP_BUF_OFFSET    (1)
#define EP0_IN_BUF_OFFSET       (2)

#define EP0_OUT_BUF_BASE        (EP0_OUT_BUF_OFFSET * USBD_MAX_PACKET0 + EP_BUF_BASE)
#define EP0_SETUP_BUF_BASE      (EP0_SETUP_BUF_OFFSET * USBD_MAX_PACKET0 + EP_BUF_BASE)
#define EP0_IN_BUF_BASE         (EP0_IN_BUF_OFFSET * USBD_MAX_PACKET0 + EP_BUF_BASE)

//! @brief Base address of the EP1 OUT buffer.
//!
//! The EP1 OUT buffer starts after the EP0 OUT buffer, SETUP buffer, and EP0 IN buffer.
#define EP1_BUF_BASE (3 * USBD_MAX_PACKET0 + EP_BUF_BASE)

typedef struct BUF_INFO {
    uint32_t  buf_len;
    uint32_t  buf_ptr;
} EP_BUF_INFO;

EP_BUF_INFO EPBufInfo[(USBD_EP_NUM + 1) * 2];
#if defined ( __CC_ARM ) || defined (__ARMCC_VERSION)
volatile uint32_t EPList[USBD_MAX_EPS * 4]  __attribute__((at(EP_LIST_BASE)));
#elif defined ( __GNUC__ )
volatile uint32_t EPList[USBD_MAX_EPS * 4]  __attribute__((section(".usbram")));
#else
#error "Unsupported compiler!"
#endif

static uint32_t s_next_ep_buf_addr = EP1_BUF_BASE;
static uint32_t s_read_ctrl_out_next = 0;

/*
 *  Get EP CmdStat pointer
 *    Parameters:    EPNum: endpoint number
 *
 */

uint32_t *GetEpCmdStatPtr(uint32_t EPNum)
{
    uint32_t *ptr = (uint32_t *)EP_LIST_BASE;

    // 4 control words per endpoint: OUT 0, OUT 1, IN 0, IN 1
    // EP0 is different but still has 4 words.
    ptr += (EPNum & USB_LOGICAL_EP_MASK) * 4;

    // For IN endpoints,
    if (EPNum & USB_IN_MASK) {
        ptr += 2;
    }

    return ptr;
}


/*
 *  Usb interrupt enable/disable
 *    Parameters:      ena: enable/disable
 *                       0: disable interrupt
 *                       1: enable interrupt
 */

#ifdef __RTX
void __svc(1) USBD_Intr(int ena);
void __SVC_1(int ena)
{
    if (ena) {
        NVIC_EnableIRQ(USB1_IRQn);           /* Enable USB interrupt */
    } else {
        NVIC_DisableIRQ(USB1_IRQn);          /* Disable USB interrupt */
    }
}
#endif



/*
 *  USB Device Initialize Function
 *   Called by the User to initialize USB Device
 *    Return Value:    None
 */

void USBD_Init(void)
{
    // Init clocks, HS PHY, and VBUS pin.
    hic_enable_usb_clocks();

    // Force clocks enabled.
    USBHSD->DEVCMDSTAT  |= USBHSD_DEVCMDSTAT_FORCE_NEEDCLK_MASK;

    // Setup EP
    USBHSD->DATABUFSTART = EP_BUF_BASE & 0xFFC00000;
    USBHSD->EPLISTSTART  = EP_LIST_BASE;

    NVIC_EnableIRQ(USB1_IRQn);
    USBD_Reset();
}


/*
 *  USB Device Connect Function
 *   Called by the User to Connect/Disconnect USB Device
 *    Parameters:      con:   Connect/Disconnect
 *    Return Value:    None
 */

void USBD_Connect(BOOL con)
{
    if (con) {
        USBHSD->DEVCMDSTAT |= USBHSD_DEVCMDSTAT_DCON_MASK;    /* Set device connect status */
    } else {
        USBHSD->DEVCMDSTAT &= ~USBHSD_DEVCMDSTAT_DCON_MASK;   /* Clear device connect status */
    }

    return;
}


// Disable optimization of this function. It gets a "iteration 8 invokes undefined behavior
// [-Waggressive-loop-optimizations]" warning in gcc if optimisation is enabled, for the first
// loop where EPList[i] is written to disable EPs.
NO_OPTIMIZE_PRE
/*
 *  USB Device Reset Function
 *   Called automatically on USB Device Reset
 *    Return Value:    None
 */
void NO_OPTIMIZE_INLINE USBD_Reset(void)
{
    uint32_t i;
    uint32_t *ptr;
    s_next_ep_buf_addr = EP1_BUF_BASE;

    // Disable all EPs.
    for (i = 0; i < sizeof(EPList) / sizeof(uint32_t); i++) {
        EPList[i] = EP_DISABLED;
    }

    s_read_ctrl_out_next = 0;
    EPBufInfo[0].buf_len = USBD_MAX_PACKET0;
    EPBufInfo[0].buf_ptr = EP0_OUT_BUF_BASE;
    EPBufInfo[1].buf_len = USBD_MAX_PACKET0;
    EPBufInfo[1].buf_ptr = EP0_IN_BUF_BASE;
    ptr  = GetEpCmdStatPtr(0);
    *ptr = N_BYTES(EPBufInfo[0].buf_len) |     /* EP0 OUT */
           BUF_ADDR(EPBufInfo[0].buf_ptr) |
           EP_BUF_ACTIVE;
    // EP0 SETUP buf info follows EP0 OUT, precedes EP0 IN.
    ptr++;
    *ptr = BUF_ADDR(EP0_SETUP_BUF_BASE);/* SETUP */
    USBHSD->DEVCMDSTAT |= USBHSD_DEVCMDSTAT_DEV_EN_MASK;         /*USB device enable */
    USBHSD->INTSTAT     = 0x2FC;              /* clear EP interrupt flags */
    USBHSD->INTEN = (USBHSD_INTEN_FRAME_INT_EN_MASK |            /* SOF intr enable */
                      USBHSD_INTEN_EP_INT_EN(0)     | /* EP0 OUT intr enable */
                      USBHSD_INTEN_EP_INT_EN(1)     | /* EP0 IN intr enable */
                      USBHSD_INTEN_DEV_INT_EN_MASK);    /* stat change int en */

    // Set bus speed.
    uint32_t speed = (USBHSD->DEVCMDSTAT & USBHSD_DEVCMDSTAT_Speed_MASK) >> USBHSD_DEVCMDSTAT_Speed_SHIFT;
    USBD_HighSpeed = (speed == DEVCMDSTAT_SPEED_HIGH);
}
NO_OPTIMIZE_POST

/*
 *  USB Device Suspend Function
 *   Called automatically on USB Device Suspend
 *    Return Value:    None
 */

void USBD_Suspend(void)
{
    /* Performed by Hardware */
}


/*
 *  USB Device Resume Function
 *   Called automatically on USB Device Resume
 *    Return Value:    None
 */

void USBD_Resume(void)
{
    /* Performed by Hardware */
}


/*
 *  USB Device Remote Wakeup Function
 *   Called automatically on USB Device Remote Wakeup
 *    Return Value:    None
 */

void USBD_WakeUp(void)
{
    SYSCON->USB1NEEDCLKCTRL = SYSCON_USB1NEEDCLKCTRL_AP_HS_DEV_NEEDCLK_MASK;
    USBHSD->DEVCMDSTAT &= ~USBHSD_DEVCMDSTAT_DSUS_MASK;  /*clear device suspend status */

    while (USBHSD->DEVCMDSTAT & USBHSD_DEVCMDSTAT_DSUS_MASK);

    SYSCON->USB1NEEDCLKCTRL = 0;
}


/*
 *  USB Device Remote Wakeup Configuration Function
 *    Parameters:      cfg:   Device Enable/Disable
 *    Return Value:    None
 */

void USBD_WakeUpCfg(BOOL cfg)
{
    if (cfg == __TRUE) {
        USBHSD->DEVCMDSTAT &= ~USBHSD_DEVCMDSTAT_FORCE_NEEDCLK_MASK; /*PPL_ON=0, in suspend clk is stoped */
    } else {
        USBHSD->DEVCMDSTAT |= USBHSD_DEVCMDSTAT_FORCE_NEEDCLK_MASK;  /*PPL_ON=1, in suspend clk isnt stoped */
        SYSCON->USB1NEEDCLKCTRL = 0;
    }
}


/*
 *  USB Device Set Address Function
 *    Parameters:      adr:   USB Device Address
 *    Return Value:    None
 */

void USBD_SetAddress(uint32_t adr, uint32_t setup)
{
    if (!setup) {
        USBHSD->DEVCMDSTAT &= ~USBHSD_DEVCMDSTAT_DEV_ADDR_MASK;
        USBHSD->DEVCMDSTAT |= adr | USBHSD_DEVCMDSTAT_DEV_EN_MASK;
    }
}



/*
 *  USB Device Configure Function
 *    Parameters:      cfg:   Device Configure/Deconfigure
 *    Return Value:    None
 */

void USBD_Configure(BOOL cfg)
{
    s_next_ep_buf_addr = EP1_BUF_BASE;
}


/*
 *  Configure USB Device Endpoint according to Descriptor
 *    Parameters:      pEPD:  Pointer to Device Endpoint Descriptor
 *    Return Value:    None
 */

void USBD_ConfigEP(USB_ENDPOINT_DESCRIPTOR *pEPD)
{
    uint32_t num, val, type;
    uint32_t *ptr;
    num  = pEPD->bEndpointAddress;
    val  = pEPD->wMaxPacketSize;
    type = pEPD->bmAttributes & USB_ENDPOINT_TYPE_MASK;

    /* IN EPs */
    if (num & USB_IN_MASK) {
        num &= ~USB_IN_MASK;
        EPBufInfo[EP_IN_IDX(num)].buf_len  = val;
        EPBufInfo[EP_IN_IDX(num)].buf_ptr  = s_next_ep_buf_addr;
        s_next_ep_buf_addr += ROUND_UP(val, MIN_BUF_SIZE);     /* calc new free buffer address */
        ptr  = GetEpCmdStatPtr(num | USB_IN_MASK);
        *ptr = EP_DISABLED;

        if (type == USB_ENDPOINT_TYPE_INTERRUPT) {
            *ptr |= EP_TYPE | EP_RF_TV;
        }
        else if (type == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
            *ptr |= EP_TYPE;
        }
    }

    /* OUT EPs */
    else {
        EPBufInfo[EP_OUT_IDX(num)].buf_len  = val;
        EPBufInfo[EP_OUT_IDX(num)].buf_ptr  = s_next_ep_buf_addr;
        ptr  = GetEpCmdStatPtr(num);
        *ptr = N_BYTES(EPBufInfo[EP_OUT_IDX(num)].buf_len) |
               BUF_ADDR(EPBufInfo[EP_OUT_IDX(num)].buf_ptr) |
               EP_DISABLED;

        if (type == USB_ENDPOINT_TYPE_INTERRUPT) {
            *ptr |= EP_TYPE | EP_RF_TV;
        }
        else if (type == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
            *ptr |= EP_TYPE;
        }

        s_next_ep_buf_addr += ROUND_UP(val, MIN_BUF_SIZE);     /* calc new free buffer address */
    }
}


/*
 *  Set Direction for USB Device Control Endpoint
 *    Parameters:      dir:   Out (dir == 0), In (dir <> 0)
 *    Return Value:    None
 */

void USBD_DirCtrlEP(uint32_t dir)
{
    /* Not needed */
}


/*
 *  Enable USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_EnableEP(uint32_t EPNum)
{
    uint32_t *ptr;;
    ptr = GetEpCmdStatPtr(EPNum);

    /* IN EP */
    if (EPNum & USB_IN_MASK) {
        EPNum &= ~USB_IN_MASK;
        *ptr &= ~EP_DISABLED;
        USBHSD->INTSTAT = (1 << EP_IN_IDX(EPNum));
        USBHSD->INTEN  |= (1 << EP_IN_IDX(EPNum));
    }

    /* OUT EP */
    else {
        *ptr &= ~EP_DISABLED;
        *ptr |=  EP_BUF_ACTIVE;
        USBHSD->INTSTAT = (1 << EP_OUT_IDX(EPNum));
        USBHSD->INTEN  |= (1 << EP_OUT_IDX(EPNum));
    }
}


/*
 *  Disable USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_DisableEP(uint32_t EPNum)
{
    uint32_t *ptr;
    ptr = GetEpCmdStatPtr(EPNum);
    *ptr = EP_DISABLED;

    if (EPNum & USB_IN_MASK) {
        EPNum &= ~USB_IN_MASK;
        USBHSD->INTEN &= ~(1 << EP_IN_IDX(EPNum));

    } else {
        USBHSD->INTEN &= ~(1 << EP_OUT_IDX(EPNum));
    }
}


/*
 *  Reset USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ResetEP(uint32_t EPNum)
{
    uint32_t *ptr;
    ptr = GetEpCmdStatPtr(EPNum);
    *ptr |= EP_TOGGLE_RESET;
}


/*
 *  Set Stall for USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_SetStallEP(uint32_t EPNum)
{
    uint32_t *ptr;
    ptr = GetEpCmdStatPtr(EPNum);

    if (EPNum & USB_LOGICAL_EP_MASK) {
        if (*ptr & EP_BUF_ACTIVE) {
            *ptr &= ~(EP_BUF_ACTIVE);
        }

    } else {
        if (EPNum & USB_IN_MASK) {
            EPNum &= ~USB_IN_MASK;
            USBHSD->EPSKIP |= (1 << EP_IN_IDX(EPNum));

            while (USBHSD->EPSKIP & (1 << EP_IN_IDX(EPNum)));

        } else {
            USBHSD->EPSKIP |= (1 << EP_OUT_IDX(EPNum));

            while (USBHSD->EPSKIP & (1 << EP_OUT_IDX(EPNum)));
        }
    }

    if ((EPNum & USB_LOGICAL_EP_MASK) == 0) {
        /* Endpoint is stalled so control out won't be next */
        s_read_ctrl_out_next = 0;
    }

    *ptr |=  EP_STALL;
}


/*
 *  Clear Stall for USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ClrStallEP(uint32_t EPNum)
{
    uint32_t *ptr;
    ptr = GetEpCmdStatPtr(EPNum);

    if (EPNum & USB_IN_MASK) {
        *ptr &=  ~EP_STALL;

    } else {
        *ptr &=  ~EP_STALL;
        *ptr |=   EP_BUF_ACTIVE;
    }

    USBD_ResetEP(EPNum);
}


/*
 *  Clear USB Device Endpoint Buffer
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ClearEPBuf(uint32_t EPNum)
{
    uint32_t  cnt, i;
    uint8_t  *dataptr;

    if (EPNum & USB_IN_MASK) {
        EPNum &= ~USB_IN_MASK;
        dataptr = (uint8_t *)EPBufInfo[EP_IN_IDX(EPNum)].buf_ptr;
        cnt     =       EPBufInfo[EP_IN_IDX(EPNum)].buf_len;

        for (i = 0; i < cnt; i++) {
            dataptr[i] = 0;
        }

    } else {
        dataptr = (uint8_t *)EPBufInfo[EP_OUT_IDX(EPNum)].buf_ptr;
        cnt     =       EPBufInfo[EP_OUT_IDX(EPNum)].buf_len;

        for (i = 0; i < cnt; i++) {
            dataptr[i] = 0;
        }
    }
}


/*
 *  Read USB Device Endpoint Data
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *    Return Value:    Number of bytes read
 */

uint32_t USBD_ReadEP(uint32_t EPNum, uint8_t *pData, uint32_t size)
{
    uint32_t cnt, i;//, xfer_size;
    volatile uint32_t *ptr;
    uint8_t *dataptr;
    ptr = GetEpCmdStatPtr(EPNum);
    int timeout = 256;

    /* Setup packet */
    if ((EPNum == 0) && !s_read_ctrl_out_next && (USBHSD->DEVCMDSTAT & USBHSD_DEVCMDSTAT_SETUP_MASK)) {
        cnt = sizeof(USB_SETUP_PACKET);

        if (size < cnt) {
            util_assert(0);
            cnt = size;
        }

        dataptr = (uint8_t *)(EPBufInfo[EP_OUT_IDX(EPNum)].buf_ptr + EP0_SETUP_BUF_OFFSET * USBD_MAX_PACKET0);
        memcpy(pData, dataptr, cnt);

        // Copy the SETUP packet into a struct we can read from more understandably.
        USB_SETUP_PACKET setup;
        memcpy(&setup, pData, sizeof(setup));

        if ((setup.wLength > 0) && setup.bmRequestType.Dir) {
            /* This control transfer has a data IN stage            */
            /* and ends with a zero length data OUT transfer.       */
            /* Ensure the data OUT token is not skipped even if     */
            /* a SETUP token arrives before USBD_ReadEP has         */
            /* been called.                                         */
            s_read_ctrl_out_next = 1;
        }

        USBHSD->EPSKIP |= (1 << EP_IN_IDX(EPNum));

        while (USBHSD->EPSKIP & (1 << EP_IN_IDX(EPNum)));

        // Clear stall on EP0 IN.
        if (*(ptr + 2) & EP_STALL) {
            *(ptr + 2) &= ~(EP_STALL);
        }

        // Clear stall on EP0 OUT.
        if (*ptr & EP_STALL) {
            *ptr &= ~(EP_STALL);
        }

        USBHSD->DEVCMDSTAT |= USBHSD_DEVCMDSTAT_SETUP_MASK;
    }

    /* OUT packet */
    else {
        ptr = GetEpCmdStatPtr(EPNum);
        cnt = EPBufInfo[EP_OUT_IDX(EPNum)].buf_len - ((*ptr & EP_NBYTES_MASK) >> EP_NBYTES_SHIFT);
        dataptr = (uint8_t *)EPBufInfo[EP_OUT_IDX(EPNum)].buf_ptr;

        while ((timeout-- > 0) && (*ptr & EP_BUF_ACTIVE)); //spin on the hardware until it's done
        util_assert(!(*ptr & EP_BUF_ACTIVE)); //check for timeout

        if (size < cnt) {
            util_assert(0);
            cnt = size;
        }

        cnt = MIN(cnt, size);

        memcpy(pData, dataptr, cnt);

        *ptr = (*ptr & (EP_TYPE | EP_RF_TV)) |
               N_BYTES(EPBufInfo[EP_OUT_IDX(EPNum)].buf_len) |
               BUF_ADDR(EPBufInfo[EP_OUT_IDX(EPNum)].buf_ptr) |
               EP_BUF_ACTIVE;

        if (EPNum == 0) {
            /* If s_read_ctrl_out_next is set then this should be a zero length        */
            /* data OUT packet.                                                 */
            util_assert(!s_read_ctrl_out_next || (cnt == 0));
            s_read_ctrl_out_next = 0;
            if (USBHSD->DEVCMDSTAT & USBHSD_DEVCMDSTAT_SETUP_MASK)  {
                // A setup packet is still pending so trigger another interrupt
                USBHSD->INTSETSTAT |= USBHSD_INTSETSTAT_EP_SET_INT(0);
            }
        }
    }

    return (cnt);
}


/*
 *  Write USB Device Endpoint Data
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *                     cnt:   Number of bytes to write
 *    Return Value:    Number of bytes written
 */

uint32_t USBD_WriteEP(uint32_t EPNum, uint8_t *pData, uint32_t cnt)
{
    uint32_t i;
    volatile uint32_t *ptr;
    uint32_t *dataptr;
    ptr = GetEpCmdStatPtr(EPNum);
    EPNum &= ~USB_IN_MASK;

    while (*ptr & EP_BUF_ACTIVE);

    uint32_t cmdstat = *ptr;
    cmdstat &= ~(EP_NBYTES_MASK | EP_BUF_OFFSET_MASK);
    cmdstat |=  BUF_ADDR(EPBufInfo[EP_IN_IDX(EPNum)].buf_ptr) | N_BYTES(cnt);
    *ptr = cmdstat;

    dataptr = (uint32_t *)EPBufInfo[EP_IN_IDX(EPNum)].buf_ptr;
    memcpy(dataptr, pData, cnt);

    if (EPNum && (*ptr & EP_STALL)) {
        return (0);
    }

    *ptr |= EP_BUF_ACTIVE;
    return (cnt);
}


/*
 *  Get USB Device Last Frame Number
 *    Parameters:      None
 *    Return Value:    Frame Number
 */

uint32_t USBD_GetFrame(void)
{
    return (USBHSD->INFO & USBHSD_INFO_FRAME_NR_MASK);
}


/*
 *  USB Device Interrupt Service Routine
 */

void USB1_IRQHandler(void)
{
    NVIC_DisableIRQ(USB1_IRQn);
    USBD_SignalHandler();
}

void USBD_Handler(void)
{
    uint32_t sts, val, num, i;
    sts = USBHSD->INTSTAT;
    USBHSD->INTSTAT = sts;

    /* Device Status Interrupt (Reset, Connect change, Suspend/Resume) */
    if (sts & USBHSD_INTSTAT_DEV_INT_MASK) {
        val = USBHSD->DEVCMDSTAT;

        /* reset interrupt */
        if (val & USBHSD_DEVCMDSTAT_DRES_C_MASK) {
            USBHSD->DEVCMDSTAT |= USBHSD_DEVCMDSTAT_DRES_C_MASK;
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

        /* connect interrupt */
        if (val & USBHSD_DEVCMDSTAT_DCON_C_MASK) {
            USBHSD->DEVCMDSTAT |= USBHSD_DEVCMDSTAT_DCON_C_MASK;
#ifdef __RTX

            if (USBD_RTX_DevTask) {
                if (val & USBHSD_DEVCMDSTAT_DCON_MASK) {
                    isr_evt_set(USBD_EVT_POWER_ON,  USBD_RTX_DevTask);
                } else {
                    isr_evt_set(USBD_EVT_POWER_OFF, USBD_RTX_DevTask);
                }
            }

#else

            if (USBD_P_Power_Event) {
                USBD_P_Power_Event((val & USBHSD_DEVCMDSTAT_DCON_MASK) >> USBHSD_DEVCMDSTAT_DCON_SHIFT);
            }

#endif
        }

        /* suspend/resume interrupt */
        if (val & USBHSD_DEVCMDSTAT_DSUS_C_MASK) {
            USBHSD->DEVCMDSTAT |= USBHSD_DEVCMDSTAT_DSUS_C_MASK;

            /* suspend interrupt */
            if (val & USBHSD_DEVCMDSTAT_DSUS_MASK) {
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

            /* resume interrupt */
            else {
#ifdef __RTX

                if (USBD_RTX_DevTask) {
                    isr_evt_set(USBD_EVT_RESUME,  USBD_RTX_DevTask);
                }

#else

                if (USBD_P_Resume_Event) {
                    USBD_P_Resume_Event();
                }

#endif
            }
        }
    }

    /* Start of Frame */
    if (sts & USBHSD_INTSTAT_FRAME_INT_MASK) {
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

    /* EndPoint Interrupt */
    if (sts & USBHSD_INTEN_EP_INT_EN_MASK) {
        const uint32_t endpoint_count = ((USBD_EP_NUM + 1) * 2);

        for (i = 0; i < endpoint_count; i++) {
            // Iterate through endpoints in the reverse order so IN endpoints
            // get processed before OUT endpoints if they are both pending.
            num = endpoint_count - i - 1;

            if (sts & (1UL << num)) {
                /* Setup */
                if ((num == 0) && !s_read_ctrl_out_next && (USBHSD->DEVCMDSTAT & USBHSD_DEVCMDSTAT_SETUP_MASK)) {
#ifdef __RTX

                    if (USBD_RTX_EPTask[num / 2]) {
                        isr_evt_set(USBD_EVT_SETUP, USBD_RTX_EPTask[num / 2]);
                    }

#else

                    if (USBD_P_EP[num / 2]) {
                        USBD_P_EP[num / 2](USBD_EVT_SETUP);
                    }

#endif
                }

                /* OUT */
                else if ((num % 2) == 0) {
#ifdef __RTX

                    if (USBD_RTX_EPTask[num / 2]) {
                        isr_evt_set(USBD_EVT_OUT, USBD_RTX_EPTask[num / 2]);
                    }

#else

                    if (USBD_P_EP[num / 2]) {
                        USBD_P_EP[num / 2](USBD_EVT_OUT);
                    }

#endif
                }

                /* IN */
                else {
#ifdef __RTX

                    if (USBD_RTX_EPTask[num / 2]) {
                        isr_evt_set(USBD_EVT_IN,  USBD_RTX_EPTask[num / 2]);
                    }

#else

                    if (USBD_P_EP[num / 2]) {
                        USBD_P_EP[num / 2](USBD_EVT_IN);
                    }

#endif
                }
            }
        }
    }

    NVIC_EnableIRQ(USB1_IRQn);
}
