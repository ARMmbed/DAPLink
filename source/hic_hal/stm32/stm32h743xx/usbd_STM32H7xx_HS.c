/*
 * Copyright (c) 2004-2013,2022 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    usbd_STM32H7xx_HS.c (based on usbd_STM32F4xx_HS.c)
 *      Purpose: Hardware Layer module for ST STM32H743
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <rl_usb.h>
#include <stdio.h>
#include "stm32h7xx.h"
#include "cmsis_gcc.h"
#include "usb_def.h"
#include "util.h"
#include "IO_Config.h"

#define __NO_USB_LIB_C
#include "usb_config.c"

#define OTG             USB1_OTG_HS
#define USBx_BASE       USB1_OTG_HS_PERIPH_BASE
#define USBx_DEVICE     ((USB_OTG_DeviceTypeDef *)(USBx_BASE + USB_OTG_DEVICE_BASE))

// 00: Control, 01: Isochronous, 10: Bulk, 11: Interrupt
#define EP_IN_TYPE(num)     ((USBx_INEP(num)->DIEPCTL & USB_OTG_DIEPCTL_EPTYP) >> USB_OTG_DIEPCTL_EPTYP_Pos)
#define EP_OUT_TYPE(num)    ((USBx_OUTEP(num)->DOEPCTL & USB_OTG_DOEPCTL_EPTYP) >> USB_OTG_DOEPCTL_EPTYP_Pos)

#define RX_FIFO_SIZE    1024    // RX MAX
#define TX0_FIFO_SIZE   64      // EP0 MAX
#define TX1_FIFO_SIZE   512
#define TX2_FIFO_SIZE   512
#define TX3_FIFO_SIZE   128     // Interrupt EP, reduced to meet the 4kB total size limit
#define TX4_FIFO_SIZE   512
#define TX5_FIFO_SIZE   128     // Interrupt EP, reduced to meet the 4kB total size limit
#define TX6_FIFO_SIZE   512
#define TX7_FIFO_SIZE   128     // Interrupt EP, reduced to meet the 4kB total size limit
#define TX8_FIFO_SIZE   512

// This chip has 4kB shared RAM for the FIFOs. Check if the FIFOs combined are within this limit.
COMPILER_ASSERT((RX_FIFO_SIZE + TX0_FIFO_SIZE + TX1_FIFO_SIZE + TX2_FIFO_SIZE + TX3_FIFO_SIZE +
                 TX4_FIFO_SIZE + TX5_FIFO_SIZE + TX6_FIFO_SIZE + TX7_FIFO_SIZE + TX8_FIFO_SIZE)
                 <= 4096);

// If a change to the FIFO sizes is required, first investigate the max packet size
// and the driver for the respective endpoints.
// If the EP definitions change, update the TX FIFO sizes and InPacketDataPtr respectively
COMPILER_ASSERT((USBD_BULK_EP_BULKIN == 1) && (USBD_BULK_HS_WMAXPACKETSIZE <= TX1_FIFO_SIZE));
COMPILER_ASSERT((USBD_MSC_HS_ENABLE == 1) && (USBD_MSC_EP_BULKIN == 2) && (USBD_MSC_HS_WMAXPACKETSIZE <= TX2_FIFO_SIZE));
COMPILER_ASSERT(USBD_CDC_ACM_1_EP_INTIN == 3);
COMPILER_ASSERT((USBD_CDC_ACM_1_HS_ENABLE == 1) && (USBD_CDC_ACM_1_EP_BULKIN == 4) && (USBD_CDC_ACM_1_HS_WMAXPACKETSIZE<= TX4_FIFO_SIZE));
COMPILER_ASSERT(USBD_CDC_ACM_2_EP_INTIN == 5);
COMPILER_ASSERT((USBD_CDC_ACM_2_HS_ENABLE == 1) && (USBD_CDC_ACM_2_EP_BULKIN == 6) && (USBD_CDC_ACM_2_HS_WMAXPACKETSIZE <= TX6_FIFO_SIZE));
COMPILER_ASSERT(USBD_CDC_ACM_3_EP_INTIN == 7);
COMPILER_ASSERT((USBD_CDC_ACM_3_HS_ENABLE == 1) && (USBD_CDC_ACM_3_EP_BULKIN == 8) && (USBD_CDC_ACM_3_HS_WMAXPACKETSIZE <= TX8_FIFO_SIZE));
#define TXFIFO_WAIT_TIMEOUT_MS    (5)

#if defined(DAPLINK_IF) && defined(UDB)
// Use larger descriptor size. After enabling the multiple CDC EPs and the MSC EP, the config
// descriptor size will exceed the original defined size 200.
#define UDB_USBD_CONFIG_DESCRIPTOR_SIZE   (400)
U8 USBD_ConfigDescriptor[UDB_USBD_CONFIG_DESCRIPTOR_SIZE] = { 0 };
#if (USBD_HS_ENABLE == 0)
U8 USBD_ConfigDescriptor_HS[] = { 0 };
#else
U8 USBD_ConfigDescriptor_HS[UDB_USBD_CONFIG_DESCRIPTOR_SIZE] = { 0 };
#endif
#endif

#if (USBD_HID_ENABLE == 1)
static uint32_t HID_IntInPacketData[(USBD_HID_MAX_PACKET + 3) / 4];
#endif

#if (USBD_CDC_ACM_ENABLE == 1)
static uint32_t CDC_ACM_IntInPacketData[USB_CDC_ACM_EP_COUNT][(USBD_CDC_ACM_MAX_PACKET + 3) / 4];
#endif

static uint32_t *InPacketDataPtr[USBD_EP_NUM] =
{
/* endpoint 0 */
    0,
/* endpoint 1 */
#if ((USBD_HID_ENABLE == 1) && (USBD_HID_EP_INTIN == 1))
    HID_IntInPacketData,
#elif (USBD_EP_NUM > 1)
    0,
#endif
/* endpoint 2 */
#if (USBD_EP_NUM > 2)
    0,
#endif
/* endpoint 3 */
#if ((USBD_CDC_ACM_ENABLE == 1) && (USBD_CDC_ACM_1_EP_INTIN == 3))
    CDC_ACM_IntInPacketData[0],
#elif (USBD_EP_NUM > 3)
    0,
#endif
/* endpoint 4 */
#if (USBD_EP_NUM > 4)
    0,
#endif
/* endpoint 5 */
#if ((USBD_CDC_ACM_ENABLE == 1) && (USB_CDC_ACM_EP_COUNT > 1) && (USBD_CDC_ACM_2_EP_INTIN == 5))
    CDC_ACM_IntInPacketData[1],
#elif (USBD_EP_NUM > 5)
    0,
#endif
/* endpoint 6 */
#if (USBD_EP_NUM > 6)
    0,
#endif
/* endpoint 7 */
#if ((USBD_CDC_ACM_ENABLE == 1) && (USB_CDC_ACM_EP_COUNT > 2) && (USBD_CDC_ACM_3_EP_INTIN == 7))
    CDC_ACM_IntInPacketData[2],
#elif (USBD_EP_NUM > 7)
    0,
#endif
/* endpoint 8 */
#if (USBD_EP_NUM > 8)
    0,
#endif
};

static uint32_t OutMaxPacketSize[USBD_EP_NUM];
static uint8_t  OutPacketCnt[USBD_EP_NUM];
static uint8_t  InPacketCnt[USBD_EP_NUM];
static uint32_t InPacketDataCnt[USBD_EP_NUM];
static uint32_t InPacketDataReady;
static uint32_t SyncWriteEP;

/*
 *  USB Device Interrupt enable
 *   Called by USBD_Init to enable the USB Interrupt
 *    Return Value:    None
 */

static void USBD_IntrEna(void)
{
    NVIC_EnableIRQ(OTG_HS_IRQn); /* Enable OTG_HS interrupt */
}

static void reset_usbhub_cy7c65642_28ltxc(void)
{
    HAL_GPIO_WritePin(USBHUB_RESET_L_PORT, USBHUB_RESET_L_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(USBHUB_RESET_L_PORT, USBHUB_RESET_L_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
}

static void reset_external_phy_usb3320c_ezk(void)
{
    HAL_GPIO_WritePin(USBPHY_RESET_L_PORT, USBPHY_RESET_L_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(USBPHY_RESET_L_PORT, USBPHY_RESET_L_PIN, GPIO_PIN_SET);
    // T_start in USB3320c-ezk datasheet
    HAL_Delay(4);
}

/*
 *  USB Device Initialize Function
 *   Called by the User to initialize USB
 *   Return Value:    None
 */

void USBD_Init(void)
{
    int32_t timeout_cntr;
    GPIO_InitTypeDef gpio_init_struct;

    // PA5 (OTG_HS_ULPI alternate function, CLOCK)
    gpio_init_struct.Pin = GPIO_PIN_5;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init_struct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    // ULPI data pins
    // PA3 (OTG_HS_ULPI alternate function, DATA0)
    gpio_init_struct.Pin = GPIO_PIN_3;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init_struct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    // PB0, PB1 (OTG_HS_ULPI alternate function, DATA1, DATA2)
    // PB10..13 (OTG_HS_ULPI alternate function, DATA3 to DATA6)
    // PB5 (OTG_HS_ULPI alternate function, DATA7)
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5 |
                           GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);

    // ULPI control pins
    // PC0 (OTG_HS_ULPI alternate function, STP)
    gpio_init_struct.Pin = GPIO_PIN_0;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);

    // PH4 (OTG_HS_ULPI alternate function, NXT)
    gpio_init_struct.Pin = GPIO_PIN_4;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOH, &gpio_init_struct);

    // PI11 (OTG_HS_ULPI alternate functon, DIR)
    gpio_init_struct.Pin = GPIO_PIN_11;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOI, &gpio_init_struct);

    gpio_init_struct.Pin = USBPHY_RESET_L_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(USBPHY_RESET_L_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = USBHUB_RESET_L_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(USBHUB_RESET_L_PORT, &gpio_init_struct);

    __HAL_RCC_USB1_OTG_HS_ULPI_CLK_ENABLE();
    __HAL_RCC_USB1_OTG_HS_CLK_ENABLE();

    HAL_Delay(20);

    reset_external_phy_usb3320c_ezk();
    reset_usbhub_cy7c65642_28ltxc();

    timeout_cntr = 1000;
    while (!(OTG->GRSTCTL & USB_OTG_GRSTCTL_AHBIDL))
    {
        if (!timeout_cntr)
        {
            break;
        }
        timeout_cntr--;
        HAL_Delay(1);
    }
    OTG->GRSTCTL |= USB_OTG_GRSTCTL_CSRST;
    // reset otg core
    timeout_cntr = 1000;
    while (OTG->GRSTCTL & USB_OTG_GRSTCTL_CSRST)
    {
        if (!timeout_cntr)
        {
            break;
        }
        timeout_cntr--;
        HAL_Delay(1);
    }
    HAL_Delay(3);

    /* Keeping all the settings in the default reset state, except the below
     */
    USB_SetCurrentMode(OTG, USB_DEVICE_MODE);
    USB_SetDevSpeed(OTG, USB_OTG_SPEED_HIGH);

    OTG->GINTMSK =
        USB_OTG_GINTMSK_USBSUSPM |  // suspend int unmask
        USB_OTG_GINTMSK_USBRST |    // reset int unmask
        USB_OTG_GINTMSK_ENUMDNEM |  // enumeration done int unmask
        USB_OTG_GINTMSK_RXFLVLM |   // receive fifo non-empty int  unmask
        USB_OTG_GINTMSK_IEPINT |    // IN EP int unmask
        USB_OTG_GINTMSK_OEPINT |    // OUT EP int unmask
        USB_OTG_GINTMSK_WUIM |      // resume int unmask
        ((USBD_P_SOF_Event != 0) ? USB_OTG_GINTMSK_SOFM : 0); // SOF int unmask

    USBD_IntrEna(); // Enable OTG interrupt
    USB_EnableGlobalInt(OTG);
}

/*
 *  USB Device Connect Function
 *   Called by the User to Connect/Disconnect USB Device
 *    Parameters:      con:   Connect/Disconnect
 *    Return Value:    None
 */

void USBD_Connect(BOOL con)
{
    if (con)
    {
        USBx_DEVICE->DCTL &= ~USB_OTG_DCTL_SDIS; // soft disconnect disabled
    }
    else
    {
        USBx_DEVICE->DCTL |= USB_OTG_DCTL_SDIS; // soft disconnect enabled
    }
}

/*
 *  USB Device Reset Function
 *   Called automatically on USB Device Reset
 *    Return Value:    None
 */

void USBD_Reset(void)
{
    uint32_t fifo_offset;

    SyncWriteEP = 0;
    InPacketDataReady = 0;

    USB_StopDevice(OTG);

    for (uint32_t i = 0; i < (USBD_EP_NUM + 1); i++)
    {
        if (USBx_OUTEP(i)->DOEPCTL & USB_OTG_DOEPCTL_EPENA)
        {
            // OUT EP disable, Set NAK
            USBx_OUTEP(i)->DOEPCTL = USB_OTG_DOEPCTL_EPDIS | USB_OTG_DOEPCTL_SNAK;
        }
        if (USBx_INEP(i)->DIEPCTL & USB_OTG_DIEPCTL_EPENA)
        {
            // IN EP disable, Set NAK
            USBx_INEP(i)->DIEPCTL = USB_OTG_DIEPCTL_EPDIS | USB_OTG_DIEPCTL_SNAK;
        }
    }

    USBD_SetAddress(0, 1);

    USBx_DEVICE->DAINTMSK = (1 << USB_OTG_DAINTMSK_OEPM_Pos) | // unmask IN&OUT EP0 interrupts
                            (1 << USB_OTG_DAINTMSK_IEPM_Pos);
    USBx_DEVICE->DOEPMSK = USB_OTG_DOEPMSK_STUPM |  // setup phase done
                           USB_OTG_DOEPMSK_EPDM |   // endpoint disabled
                           USB_OTG_DOEPMSK_XFRCM;   // transfer complete
    USBx_DEVICE->DIEPMSK = USB_OTG_DIEPMSK_EPDM |   // endpoint disabled
                           USB_OTG_DIEPMSK_XFRCM;   // transfer completed
                           // USB_OTG_DIEPMSK_TOM   // the ref man suggest turning this on, but we'll also have to handle it then

    // Program the RX FIFO size
    OTG->GRXFSIZ = RX_FIFO_SIZE / 4;
    fifo_offset = RX_FIFO_SIZE;
    // Program the TX FIFO size for endpoint 0
    OTG->DIEPTXF0_HNPTXFSIZ = (fifo_offset / 4) | ((TX0_FIFO_SIZE / 4) << 16);
    fifo_offset += TX0_FIFO_SIZE;
    // Program the TX FIFO size for all other endpoints
    OTG->DIEPTXF[0] = (fifo_offset / 4) | ((TX1_FIFO_SIZE / 4) << 16);
    fifo_offset += TX1_FIFO_SIZE;
    OTG->DIEPTXF[1] = (fifo_offset / 4) | ((TX2_FIFO_SIZE / 4) << 16);
    fifo_offset += TX2_FIFO_SIZE;
    OTG->DIEPTXF[2] = (fifo_offset / 4) | ((TX3_FIFO_SIZE / 4) << 16);
    fifo_offset += TX3_FIFO_SIZE;
    OTG->DIEPTXF[3] = (fifo_offset / 4) | ((TX4_FIFO_SIZE / 4) << 16);
    fifo_offset += TX4_FIFO_SIZE;
    OTG->DIEPTXF[4] = (fifo_offset / 4) | ((TX5_FIFO_SIZE / 4) << 16);
    fifo_offset += TX5_FIFO_SIZE;
    OTG->DIEPTXF[5] = (fifo_offset / 4) | ((TX6_FIFO_SIZE / 4) << 16);
    fifo_offset += TX6_FIFO_SIZE;
    OTG->DIEPTXF[6] = (fifo_offset / 4) | ((TX7_FIFO_SIZE / 4) << 16);
    fifo_offset += TX7_FIFO_SIZE;
    OTG->DIEPTXF[7] = (fifo_offset / 4) | ((TX8_FIFO_SIZE / 4) << 16);

    USBx_OUTEP(0)->DOEPTSIZ = USB_OTG_DOEPTSIZ_STUPCNT_0 | // setup count = 1
                              (USBD_OUT_PACKET_CNT0 << USB_OTG_DOEPTSIZ_PKTCNT_Pos) |  // packet count
                              USBD_MAX_PACKET0;

    OutMaxPacketSize[0] = USBD_MAX_PACKET0;
    OutPacketCnt[0] = USBD_OUT_PACKET_CNT0;
    InPacketCnt[0] = USBD_IN_PACKET_CNT0;
}

/*
 *  USB Device Suspend Function
 *   Called automatically on USB Device Suspend
 *    Return Value:    None
 */

void USBD_Suspend(void) {}

/*
 *  USB Device Resume Function
 *   Called automatically on USB Device Resume
 *    Return Value:    None
 */

void USBD_Resume(void) {}

/*
 *  USB Device Remote Wakeup Function
 *   Called automatically on USB Device Remote Wakeup
 *    Return Value:    None
 */

void USBD_WakeUp(void){}

/*
 *  USB Device Remote Wakeup Configuration Function
 *    Parameters:      cfg:   Device Enable/Disable
 *    Return Value:    None
 */

void USBD_WakeUpCfg(BOOL cfg) {}

/*
 *  USB Device Set Address Function
 *    Parameters:      adr:   USB Device Address
 *    Return Value:    None
 */

void USBD_SetAddress(U32 adr, U32 setup)
{
    if (setup)
    {
        USB_SetDevAddress(OTG, (uint8_t)adr);
    }
}

/*
 *  USB Device Configure Function
 *    Parameters:      cfg:   Device Configure/Deconfigure
 *    Return Value:    None
 */

void USBD_Configure(BOOL cfg)
{
    InPacketDataReady &= ~1UL;
}

/*
 *  Configure USB Device Endpoint according to Descriptor
 *    Parameters:      pEPD:  Pointer to Device Endpoint Descriptor
 *    Return Value:    None
 */

void USBD_ConfigEP(USB_ENDPOINT_DESCRIPTOR *pEPD)
{
    uint32_t num = (pEPD->bEndpointAddress & ~USB_ENDPOINT_DIRECTION_MASK);
    uint32_t val = pEPD->wMaxPacketSize;
    uint32_t type = (pEPD->bmAttributes & USB_ENDPOINT_TYPE_MASK);

    if (pEPD->bEndpointAddress & USB_ENDPOINT_DIRECTION_MASK)
    {
        InPacketCnt[num] = 1;

        USBx_DEVICE->DAINTMSK |= (1 << num);     // unmask IN EP int
        USBx_INEP(num)->DIEPCTL = (num << USB_OTG_DIEPCTL_TXFNUM_Pos) | // fifo number
                                  (type << USB_OTG_DIEPCTL_EPTYP_Pos) | // ep type
                                  (val & USB_OTG_DIEPCTL_MPSIZ);        // max packet size
        if ((type == USB_ENDPOINT_TYPE_BULK) || (type == USB_ENDPOINT_TYPE_INTERRUPT))
        {
            USBx_INEP(num)->DIEPCTL |= USB_OTG_DIEPCTL_SD0PID_SEVNFRM;
        }
    }
    else
    {
        OutMaxPacketSize[num] = val & USB_OTG_DOEPCTL_MPSIZ;
        OutPacketCnt[num] = 1;

        USBx_DEVICE->DAINTMSK |= (1 << (num + 16)); // unmask OUT EP int

        USBx_OUTEP(num)->DOEPCTL = (type << USB_OTG_DOEPCTL_EPTYP_Pos) |    // EP type
                                   (val & USB_OTG_DOEPCTL_MPSIZ);           // max packet size

        USBx_OUTEP(num)->DOEPTSIZ = (OutPacketCnt[num] << USB_OTG_DOEPTSIZ_PKTCNT_Pos) | // packet count
                                    (val & USB_OTG_DOEPCTL_MPSIZ);                       // transfer size
        if ((type == USB_ENDPOINT_TYPE_BULK) || (type == USB_ENDPOINT_TYPE_INTERRUPT))
        {
            USBx_OUTEP(num)->DOEPCTL |= USB_OTG_DOEPCTL_SD0PID_SEVNFRM;
        }
    }
}

/*
 *  Set Direction for USB Device Control Endpoint
 *    Parameters:      dir:   Out (dir == 0), In (dir <> 0)
 *    Return Value:    None
 */

void USBD_DirCtrlEP(U32 dir)
{
}

/*
 *  Enable USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_EnableEP(U32 EPNum)
{
    if (EPNum & USB_ENDPOINT_DIRECTION_MASK)
    {
        EPNum &= ~USB_ENDPOINT_DIRECTION_MASK;
        USBx_INEP(EPNum)->DIEPCTL |= USB_OTG_DIEPCTL_USBAEP | // EP active
                                     USB_OTG_DIEPCTL_SNAK;    // set EP NAK
        if (USBx_INEP(EPNum)->DIEPCTL & USB_OTG_DIEPCTL_EPENA)
        {
            USBx_INEP(EPNum)->DIEPCTL |= USB_OTG_DIEPCTL_EPDIS; // disable EP
        }

        InPacketDataReady &= ~(1 << EPNum);
    }
    else
    {
        USBx_OUTEP(EPNum)->DOEPCTL |= USB_OTG_DOEPCTL_USBAEP |  // EP active
                                      USB_OTG_DOEPCTL_EPENA |   // enable EP
                                      USB_OTG_DOEPCTL_CNAK;     // clear EP NAK
    }
}

/*
 *  Disable USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_DisableEP(U32 EPNum)
{
    uint32_t wcnt;

    // Disable IN Endpoint
    if (EPNum & USB_ENDPOINT_DIRECTION_MASK)
    {
        EPNum &= ~USB_ENDPOINT_DIRECTION_MASK;
        InPacketDataReady &= ~(1 << EPNum);

        if (USBx_INEP(EPNum)->DIEPCTL & USB_OTG_DIEPCTL_EPENA)
        {
            // disable EP
            USBx_INEP(EPNum)->DIEPCTL |= USB_OTG_DIEPCTL_EPDIS;
        }
        // set EP NAK
        USBx_INEP(EPNum)->DIEPCTL |= USB_OTG_DIEPCTL_SNAK;
        // deactivate EP
        USBx_INEP(EPNum)->DIEPCTL &= ~USB_OTG_DIEPCTL_USBAEP;

    }
    else
    {
        // Disable OUT Endpoint
        // set global out nak
        USBx_DEVICE->DCTL |= USB_OTG_DCTL_SGONAK;

        wcnt = 1000;
        while (!(OTG->GINTSTS & USB_OTG_GINTSTS_BOUTNAKEFF))
        {
            // wait until global NAK
            if ((wcnt--) == 0)
            {
                break;
            }
        }

        if (USBx_OUTEP(EPNum)->DOEPCTL & USB_OTG_DOEPCTL_EPENA)
        {
            // disable EP
            USBx_OUTEP(EPNum)->DOEPCTL |= USB_OTG_DOEPCTL_EPDIS;
        }
        // set EP NAK
        USBx_OUTEP(EPNum)->DOEPCTL |= USB_OTG_DOEPCTL_SNAK;
        //  deactivate EP
        USBx_OUTEP(EPNum)->DOEPCTL &= ~USB_OTG_DOEPCTL_USBAEP;

        wcnt = 1000;
        while (!(USBx_OUTEP(EPNum)->DOEPINT & USB_OTG_DOEPINT_EPDISD))
        {
            // wait until EP disabled
            if ((wcnt--) == 0)
            {
                break;
            }
        }
        // clear global nak
        USBx_DEVICE->DCTL |= USB_OTG_DCTL_CGONAK;
    }
}

/*
 *  Reset USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ResetEP(U32 EPNum)
{
    // Reset IN Endpoint
    if (EPNum & USB_ENDPOINT_DIRECTION_MASK)
    {
        EPNum &= ~USB_ENDPOINT_DIRECTION_MASK;
        InPacketDataReady &= ~(1 << EPNum);
        if (USBx_INEP(EPNum)->DIEPCTL & USB_OTG_DIEPCTL_EPENA)
        {
            // disable EP
            USBx_INEP(EPNum)->DIEPCTL |= USB_OTG_DIEPCTL_EPDIS;
        }
        // set EP NAK
        USBx_INEP(EPNum)->DIEPCTL |= USB_OTG_DIEPCTL_SNAK;

        // Flush endpoint fifo
        USB_FlushTxFifo(OTG, EPNum);
    }
}

/*
 *  Set Stall for USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_SetStallEP(U32 EPNum)
{
    uint32_t wcnt;

    if (!(EPNum & USB_ENDPOINT_DIRECTION_MASK))
    {
        // Stall OUT Endpoint

        // set global out nak
        USBx_DEVICE->DCTL |= USB_OTG_DCTL_SGONAK;
        wcnt = 1000;
        while (!(OTG->GINTSTS & USB_OTG_GINTSTS_BOUTNAKEFF))
        {
            // wait until global NAK
            if ((wcnt--) == 0)
            {
                break;
            }
        }

        if (USBx_OUTEP(EPNum)->DOEPCTL & USB_OTG_DOEPCTL_EPENA)
        {
            // disable EP
            USBx_OUTEP(EPNum)->DOEPCTL |= USB_OTG_DOEPCTL_EPDIS;
        }
        // set stall
        USBx_OUTEP(EPNum)->DOEPCTL |= USB_OTG_DOEPCTL_STALL;

        wcnt = 1000;
        while (!(USBx_OUTEP(EPNum)->DOEPINT & USB_OTG_DOEPINT_EPDISD))
        {
            // wait until EP disabled
            if ((wcnt--) == 0)
            {
                break;
            }
        }

        // clear global nak
        USBx_DEVICE->DCTL |= USB_OTG_DCTL_CGONAK;

    }
    else
    {
        // Stall IN endpoint
        EPNum &= ~USB_ENDPOINT_DIRECTION_MASK;
        if (USBx_INEP(EPNum)->DIEPCTL & USB_OTG_DIEPCTL_EPENA)
        {
            // disable endpoint
            USBx_INEP(EPNum)->DIEPCTL |= USB_OTG_DIEPCTL_EPDIS;
        }
        // set stall
        USBx_INEP(EPNum)->DIEPCTL |= USB_OTG_DIEPCTL_STALL;

        USB_FlushTxFifo(OTG, EPNum);
    }
}

/*
 *  Clear Stall for USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ClrStallEP(U32 EPNum)
{
    if (!(EPNum & USB_ENDPOINT_DIRECTION_MASK))
    {
        // Clear OUT endpoint Stall
        if (EP_OUT_TYPE(EPNum) > 1)
        {
            // if EP type Bulk or Interrupt
            // Set DATA0 PID
            USBx_OUTEP(EPNum)->DOEPCTL |= USB_OTG_DOEPCTL_SD0PID_SEVNFRM;
            // Clear stall
            USBx_OUTEP(EPNum)->DOEPCTL &= ~USB_OTG_DOEPCTL_STALL;
        }
    }
    else
    {
        // Clear IN Endpoint Stall
        EPNum &= ~USB_ENDPOINT_DIRECTION_MASK;

        if (USBx_INEP(EPNum)->DIEPCTL & USB_OTG_DIEPCTL_EPENA)
        {
            // disable endpoint
            USBx_INEP(EPNum)->DIEPCTL |= USB_OTG_DIEPCTL_EPDIS;
        }

        // Flush endpoint fifo
        USB_FlushTxFifo(OTG, EPNum);

        if (EP_IN_TYPE(EPNum) > 1)
        {
            // if Interrupt or bulk EP, Set DATA0 PID
            USBx_INEP(EPNum)->DIEPCTL |= USB_OTG_DIEPCTL_SD0PID_SEVNFRM;
        }

        // clear Stall
        USBx_INEP(EPNum)->DIEPCTL &= ~USB_OTG_DIEPCTL_STALL;
    }
}

/*
 *  Clear USB Device Endpoint Buffer
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ClearEPBuf(U32 EPNum)
{
    if (EPNum & USB_ENDPOINT_DIRECTION_MASK)
    {
        EPNum &= ~USB_ENDPOINT_DIRECTION_MASK;
        USB_FlushTxFifo(OTG, EPNum);
    }
    else
    {
        USB_FlushRxFifo(OTG);
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

uint32_t USBD_ReadEP(U32 EPNum, U8 *pData, uint32_t bufsz)
{
    U32 val, sz;
    U32 pktsts;

    if ((USBx_OUTEP(EPNum)->DOEPCTL & USB_OTG_DOEPCTL_USBAEP) == 0)
    {
        // if Ep not active
        return (0);
    }

    sz = (OTG->GRXSTSP & USB_OTG_GRXSTSP_BCNT) >> USB_OTG_GRXSTSP_BCNT_Pos; // get available data size

    if (sz > bufsz)
    {
        // Read data from fifo. If there's not enough buffer space, still have to read the data
        // otherwise we never get a OUT/Setup transfer completed data packet and GRXSTSP pops
        // the wrong data and we might have lingering data in the RX FIFO. This shouldn't
        // happen, but if does, the drivers need more checks before reading the EP. Just abort
        // before something else goes wrong.
        util_assert(false);
    }
    else
    {
        for (val = 0; val < (uint32_t)((sz + 3) / 4); val++)
        {
            __UNALIGNED_UINT32_WRITE(pData, USBx_DFIFO(0U));
            pData += 4;
        }
    }

    // wait for OUT/Setup transfer completed data packet from USB core
    // Since USBD_Handler is heavy, we don't want to just pop single register in it
    // But it is possible that some packet gets into the FIFO before the core pushes
    // transfer completed packet. Verify it is really a transfer completed
    // packet and then pop it out.
    while ((OTG->GINTSTS & USB_OTG_GINTSTS_RXFLVL) == 0) {}
    pktsts = ((OTG->GRXSTSR & USB_OTG_GRXSTSP_PKTSTS) >> USB_OTG_GRXSTSP_PKTSTS_Pos);
    if ((pktsts == STS_XFER_COMP) || (pktsts == STS_SETUP_COMP))
    {
        OTG->GRXSTSP;
    }

    OTG->GINTMSK |= USB_OTG_GINTMSK_RXFLVLM;    // unmask RxFIFO non-empty interrupt

    return (sz);
}

/*
 *  Write USB Device Endpoint Data
 *  If write was requested synchronously from IRQ then data is written to FIFO
 *  directly else data is written to the intermediate buffer and synchronously
 *  transferred to FIFO on next NAK event.
 *    Parameters:      EPNum: Device Endpoint Number
 *                        EPNum.0..3: Address
 *                        EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *                     cnt:   Number of bytes to write
 *    Return Value:    Number of bytes written
 */

uint32_t USBD_WriteEP(U32 EPNum, U8 *pData, U32 cnt)
{
    U32 *ptr, val;
    U32 txfifo_wait_start_time_ms;

    EPNum &= ~USB_ENDPOINT_DIRECTION_MASK;

    if ((USBx_INEP(EPNum)->DIEPCTL & USB_OTG_DIEPCTL_USBAEP) == 0)
    {
        // if Ep not active
        return (0);
    }

    /* Asynchronous write to intermediate buffer */
    if (!SyncWriteEP && InPacketDataPtr[EPNum])
    {
        if (!(InPacketDataReady & (1 << EPNum)))
        {
            InPacketDataCnt[EPNum] = cnt; /* save Data size */
            ptr = InPacketDataPtr[EPNum];
            val = (cnt + 3) / 4;
            if (val)
            {
                while (val--)
                {
                    // save data to intermediate buffer
                    *ptr++ = *((U32 *)pData);
                    pData += 4;
                }
            }
            InPacketDataReady |= 1 << EPNum;
            // Set NAK to enable interrupt on NAK
            USBx_INEP(EPNum)->DIEPCTL |= USB_OTG_DIEPCTL_SNAK;
            // INEPNEM = 1, IN EP NAK efective msk
            USBx_DEVICE->DIEPMSK |= USB_OTG_DIEPMSK_INEPNEM;
        }
        else
        {
            /* If packet already loaded to buffer */
            return 0;
        }
    }
    else
    {
        // get space in Ep TxFIFO
        // Reset the IN endpoint if we can't get enough space. Usually this
        // means something goes wrong on this endpoint and usb host stops
        // sending IN token.
        txfifo_wait_start_time_ms = HAL_GetTick();
        while (cnt > 0 && (USBx_INEP(EPNum)->DTXFSTS * 4) < cnt)
        {
            if (HAL_GetTick() - txfifo_wait_start_time_ms >= TXFIFO_WAIT_TIMEOUT_MS)
            {
                USBD_ResetEP(EPNum | USB_ENDPOINT_DIRECTION_MASK);
                return 0;
            }
        }

        // set transfer size and packet count
        USBx_INEP(EPNum)->DIEPTSIZ =
            cnt | (InPacketCnt[EPNum] << USB_OTG_DIEPTSIZ_PKTCNT_Pos) | (InPacketCnt[EPNum] << USB_OTG_DIEPTSIZ_MULCNT_Pos);

        // enable ep and clear NAK
        USBx_INEP(EPNum)->DIEPCTL |= USB_OTG_DIEPCTL_EPENA | USB_OTG_DIEPCTL_CNAK;
        if (cnt)
        {
            ptr = (uint32_t *)pData;
            val = (cnt + 3) / 4;
            while (val--)
            {
                // copy data to endpoint TxFIFO
                USBx_DFIFO((uint32_t)EPNum) = __UNALIGNED_UINT32_READ(pData);
                pData += 4;
            }
        }
        InPacketDataReady &= ~(1 << EPNum);
    }
    return (cnt);
}

/*
 *  Get USB Device Last Frame Number
 *    Parameters:      None
 *    Return Value:    Frame Number
 */

uint32_t USBD_GetFrame(void)
{
    return ((USBx_DEVICE->DSTS & USB_OTG_DSTS_FNSOF) >> USB_OTG_DSTS_FNSOF_Pos);
}

/*
 *  USB Device Interrupt Service Routine
 */
void OTG_HS_IRQHandler(void)
{
    NVIC_DisableIRQ(OTG_HS_IRQn);
    USBD_SignalHandler();
}

void USBD_Handler(void)
{
    uint32_t istr, val, num, i, msk;

    istr = OTG->GINTSTS & OTG->GINTMSK;

    // reset interrupt
    if (istr & USB_OTG_GINTSTS_USBRST)
    {
        USBD_Reset();
        usbd_reset_core();
        if (USBD_P_Reset_Event)
        {
            USBD_P_Reset_Event();
        }
        OTG->GINTSTS = USB_OTG_GINTSTS_USBRST;
    }

    // suspend interrupt
    if (istr & USB_OTG_GINTSTS_USBSUSP)
    {
        USBD_Suspend();
        if (USBD_P_Suspend_Event)
        {
            USBD_P_Suspend_Event();
        }
        OTG->GINTSTS = USB_OTG_GINTSTS_USBSUSP;
    }

    // resume interrupt
    if (istr & USB_OTG_GINTSTS_WKUINT)
    {
        USBD_Resume();
        if (USBD_P_Resume_Event)
        {
            USBD_P_Resume_Event();
        }
        OTG->GINTSTS = USB_OTG_GINTSTS_WKUINT;
    }

    // speed enumeration completed
    if (istr & USB_OTG_GINTSTS_ENUMDNE)
    {
        if ((USBx_DEVICE->DSTS & USB_OTG_DSTS_ENUMSPD) == DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ)
        {
            USBD_HighSpeed = 1;
        }
        USBx_INEP(0)->DIEPCTL &= ~USB_OTG_DIEPCTL_MPSIZ;
        USBx_INEP(0)->DIEPCTL |= OutMaxPacketSize[0]; // EP0 max packet
        USBx_DEVICE->DCTL |= USB_OTG_DCTL_CGINAK;     // clear global IN NAK
        USBx_DEVICE->DCTL |= USB_OTG_DCTL_CGONAK;     // clear global OUT NAK
        OTG->GINTSTS |= USB_OTG_GINTSTS_ENUMDNE;
    }

    // Start Of Frame
    if (istr & USB_OTG_GINTSTS_SOF)
    {
        if (USBD_P_SOF_Event)
        {
            USBD_P_SOF_Event();
        }
        OTG->GINTSTS = USB_OTG_GINTSTS_SOF;
    }

    // RxFIFO non-empty
    if (istr & USB_OTG_GINTSTS_RXFLVL)
    {
        val = OTG->GRXSTSR;
        num = val & USB_OTG_GRXSTSP_EPNUM;

        switch ((val & USB_OTG_GRXSTSP_PKTSTS) >> USB_OTG_GRXSTSP_PKTSTS_Pos)
        {
            // setup packet
            case STS_SETUP_UPDT:
            {
                if (USBD_P_EP[num])
                {
                    USBD_P_EP[num](USBD_EVT_SETUP);
                }
                break;
            }

            // OUT packet
            case STS_DATA_UPDT:
            {
                OTG->GINTMSK &= ~USB_OTG_GINTMSK_RXFLVLM;
                if (USBD_P_EP[num])
                {
                    USBD_P_EP[num](USBD_EVT_OUT);
                }
                break;
            }
            // other valid packet types
            case STS_XFER_COMP:
            case STS_SETUP_COMP:
            {
                OTG->GINTMSK |= USB_OTG_GINTMSK_RXFLVLM;
                OTG->GRXSTSP;
                break;
            }
            case STS_GOUT_NAK:
            {
                OTG->GRXSTSP;
                break;
            }
            // We've seen bugs where we read non-valid packet info from the top of the FIFO,
            // log it in case there are other similar bugs
            default:
            {
                OTG->GRXSTSP;
                // Error: we are reading garbage packet status, so something went really wrong
                util_assert(false);
                break;
            }
        }
    }

    // OUT Packet
    if (istr & USB_OTG_GINTSTS_OEPINT)
    {
        msk = ((USBx_DEVICE->DAINT & USBx_DEVICE->DAINTMSK & USB_OTG_DAINTMSK_OEPM) >> USB_OTG_DAINTMSK_OEPM_Pos);
        i = 0;
        while (msk)
        {
            num = 0;
            for (; i < (USBD_EP_NUM + 1); i++)
            {
                if ((msk >> i) & 1)
                {
                    num = i;
                    msk &= ~(1 << i);
                    break;
                }
            }

            // Endpoint disabled
            if (USBx_OUTEP(num)->DOEPINT & USB_OTG_DOEPINT_EPDISD)
            {
                USBx_OUTEP(num)->DOEPINT |= USB_OTG_DOEPINT_EPDISD;
            }

            // Transfer complete interrupt
            if ((USBx_OUTEP(num)->DOEPINT & USB_OTG_DOEPINT_XFRC) | (USBx_OUTEP(num)->DOEPINT & USB_OTG_DOEPINT_STUP))
            {
                USBx_OUTEP(num)->DOEPTSIZ =
                    (OutPacketCnt[num] << USB_OTG_DOEPTSIZ_PKTCNT_Pos) | /* packet count */
                    (OutMaxPacketSize[num]);    /* transfer size */
                if (num == 0)
                {
                    USBx_OUTEP(0)->DOEPTSIZ |= USB_OTG_DOEPTSIZ_STUPCNT_0;
                }
                USBx_OUTEP(num)->DOEPCTL |= USB_OTG_DOEPCTL_EPENA | USB_OTG_DOEPCTL_CNAK;
                USBx_OUTEP(num)->DOEPINT |= USB_OTG_DOEPINT_XFRC;
            }
        }
    }

    // IN Packet
    if (istr & USB_OTG_GINTSTS_IEPINT)
    {
        msk = (USBx_DEVICE->DAINT & USBx_DEVICE->DAINTMSK & USB_OTG_DAINTMSK_IEPM);
        i = 0;
        while (msk)
        {
            num = 0;
            for (; i < (USBD_EP_NUM + 1); i++)
            {
                if ((msk >> i) & 1)
                {
                    num = i;
                    msk &= ~(1 << i);
                    break;
                }
            }

            // Endpoint disabled
            if (USBx_INEP(num)->DIEPINT & USB_OTG_DIEPINT_EPDISD)
            {
                USBx_INEP(num)->DIEPINT = USB_OTG_DIEPINT_EPDISD;
            }

            // IN endpoint NAK effective
            if (USBx_INEP(num)->DIEPINT & USB_OTG_DIEPINT_INEPNE)
            {
                if (InPacketDataPtr[num] &&
                    (InPacketDataReady & (1 << num)))
                {
                    SyncWriteEP = 1;
                    USBD_WriteEP(num, (uint8_t *)InPacketDataPtr[num],
                                 InPacketDataCnt[num]);
                    SyncWriteEP = 0;
                    if (!InPacketDataReady)
                    {
                        // No more pending IN transfers and disable IN NAK interrupts
                        USBx_DEVICE->DIEPMSK &= ~USB_OTG_DIEPMSK_INEPNEM;
                    }
                    continue;
                }
                else
                {
                    USBx_INEP(num)->DIEPCTL |= USB_OTG_DIEPCTL_CNAK;
                }
                USBx_INEP(num)->DIEPINT = USB_OTG_DIEPINT_INEPNE;
            }

            // Transmit completed
            if (USBx_INEP(num)->DIEPINT & USB_OTG_DIEPINT_XFRC)
            {
                USBx_INEP(num)->DIEPINT = USB_OTG_DIEPINT_XFRC;
                SyncWriteEP = 1;
                if (USBD_P_EP[num])
                {
                    USBD_P_EP[num](USBD_EVT_IN);
                }
                SyncWriteEP = 0;
            }
        }
    }

    NVIC_EnableIRQ(OTG_HS_IRQn);
}
