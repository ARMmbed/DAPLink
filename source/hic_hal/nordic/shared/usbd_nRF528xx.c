/* -----------------------------------------------------------------------------
 * Copyright (c) 2021 Arm Limited (or its affiliates). All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        14. July 2021
 * $Revision:    V1.0
 *
 * Project:      USB Device Hardware Layer module for RL-USB v4.70 for 
 *               Nordic Semiconductor nRF528xx Device Series
 * --------------------------------------------------------------------------
 * Defines used for driver configuration (at compile time):
 *
 *   USBD_EP_NUM_MAX:   defines maximum number of IN/OUT Endpoint pairs 
 *                      that driver will support with Control Endpoint 0 
 *                      included, this value impacts driver memory 
 *                      requirements
 *     - default value: 8
 *     - maximum value: 8
 *
 *   __DAPLINK:         compatible with DAPLink
 * --------------------------------------------------------------------------
 * Notes:
 *  - this driver does not implement Isochronous Endpoint due to limitation 
 *    that only Endpoint 8 is available for Isochronous Endpoint
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.0
 *    Initial release
 */

#include <string.h>

#ifdef   __DAPLINK
#include "usb.h"
#else
#include <RTL.h>
#endif
#include "rl_usb.h"

#include "nrf.h"                        /* Device header */
#include "nrfx.h"
#include "nrf52_erratas.h"

#define  __NO_USB_LIB_C
#include "usb_config.c"

/* Definition for overriding maximum number of endpoints supported by 
   this driver, used for reducing RAM memory consumption. */
#ifndef USBD_EP_NUM_MAX
#define USBD_EP_NUM_MAX                 (8)
#elif  (USBD_EP_NUM_MAX == 0)
#error  Maximum endpoints cannot be 0! Please increase USBD_EP_NUM_MAX!
#elif  (USBD_EP_NUM_MAX > 8)
#error  Maximum endpoints supported is 8! Please reduce USBD_EP_NUM_MAX!
#endif


/* Typedefs */
typedef struct {                        /* Endpoint configuration */
  uint16_t max_packet_size;
} ep_config_t;


/* Local variables */
#ifdef __RTX
static OS_MUT      usbd_hw_mutex;
#endif
static uint32_t    setup_packet_available;
static uint32_t    ep0_no_data_stage;
static uint32_t    ep0_status_next;
static uint32_t    ep0_dir;
static ep_config_t ep_config[2][USBD_EP_NUM_MAX];
static uint32_t    ep_buf   [2][USBD_EP_NUM_MAX][64/4];


/* Helper functions */

/* Enable USB Device */
void usbd_enable (void) {
  uint32_t i;

  if (nrf52_errata_187()) {             /* If errata [187] (USBD: USB cannot be enabled) is present */
    *(volatile uint32_t *)0x4006EC00 = 0x00009375;
    *(volatile uint32_t *)0x4006ED14 = 0x00000003;
    *(volatile uint32_t *)0x4006EC00 = 0x00009375;
  }

  /* Enable the peripheral */
  NRF_USBD->ENABLE = USBD_ENABLE_ENABLE_Enabled << USBD_ENABLE_ENABLE_Pos;

  /* Waiting for peripheral to enable (max 10 ms), this should take a few �s */
  for (i = 160000U; i != 0U; i--) {
    if ((NRF_USBD->EVENTCAUSE & USBD_EVENTCAUSE_READY_Msk) != 0U) {
      break;
    }
  }

  /* Enable ready event */
  NRF_USBD->EVENTCAUSE &= ~USBD_EVENTCAUSE_READY_Msk;

  if (nrf52_errata_187()) {             /* If errata [187] (USBD: USB cannot be enabled) is present */
    *(volatile uint32_t *)0x4006EC00 = 0x00009375;
    *(volatile uint32_t *)0x4006ED14 = 0x00000000;
    *(volatile uint32_t *)0x4006EC00 = 0x00009375;
  }
}

/* Disable USB Device */
void usbd_disable (void) {
  NRF_USBD->ENABLE = USBD_ENABLE_ENABLE_Disabled << USBD_ENABLE_ENABLE_Pos;
}

/* Do EasyDMA transfer to/from USB controller's internal buffer */
void usbd_dma (uint32_t EPNum, uint32_t *buf, uint32_t cnt) {
  uint8_t  ep_dir, ep_num;
  uint16_t len, ep_max_len;

  ep_dir     = (EPNum &  USB_ENDPOINT_DIRECTION_MASK) >> 7;
  ep_num     =  EPNum & ~USB_ENDPOINT_DIRECTION_MASK;
  ep_max_len =  ep_config[ep_dir][ep_num].max_packet_size;
  len        =  cnt;

  if (ep_num >= USBD_EP_NUM_MAX) {      /* If ep_num is not available */
    return;
  }

  if (len > ep_max_len) {
    len = ep_max_len;
  }

#ifdef __RTX
  os_mut_wait(&usbd_hw_mutex, 0xFFFFU);
#endif

  if (ep_dir != 0U) {                   /* If IN endpoint */
      NRF_USBD->EPIN[ep_num].PTR        = (uint32_t)buf;
      NRF_USBD->EPIN[ep_num].MAXCNT     = len;
      NRF_USBD->TASKS_STARTEPIN[ep_num] = USBD_TASKS_STARTEPIN_TASKS_STARTEPIN_Trigger;
      while (NRF_USBD->EVENTS_ENDEPIN[ep_num] == 0U);
      NRF_USBD->EVENTS_ENDEPIN[ep_num]  = 0U;
  } else {                              /* If OUT endpoint */
      NRF_USBD->EPOUT[ep_num].PTR       = (uint32_t)buf;
      NRF_USBD->EPOUT[ep_num].MAXCNT    = len;
      NRF_USBD->TASKS_STARTEPOUT[ep_num]= USBD_TASKS_STARTEPOUT_TASKS_STARTEPOUT_Trigger;
      while (NRF_USBD->EVENTS_ENDEPOUT[ep_num] == 0U);
      NRF_USBD->EVENTS_ENDEPOUT[ep_num] = 0U;
  }

#ifdef __RTX
  os_mut_release(&usbd_hw_mutex);
#endif
}


/* Driver functions */

/*
 *  USB Device Initialize Function
 *   Called by the User to initialize USB Device
 *    Return Value:    None
 */

void USBD_Init (void) {

#ifdef __RTX
  os_mut_init(&usbd_hw_mutex);
#endif

  if (nrf52_errata_223()) {             /* If errata [223] (USBD: Unexpected behavior after reset) is present */
    usbd_enable();
    usbd_disable();
  }

  usbd_enable();

  /* Enable only USB Reset interrupt */
  NRF_USBD->INTENSET = USBD_INTEN_USBRESET_Msk;

  NVIC_SetPriority(USBD_IRQn, NRFX_USBD_DEFAULT_CONFIG_IRQ_PRIORITY);
  NVIC_ClearPendingIRQ(USBD_IRQn);
  NVIC_EnableIRQ(USBD_IRQn);
}


/*
 *  USB Device Connect Function
 *   Called by the User to Connect/Disconnect USB Device
 *    Parameters:      con:   Connect/Disconnect
 *    Return Value:    None
 */

#ifdef   __DAPLINK
void USBD_Connect (BOOL con) {
#else
void USBD_Connect (uint32_t con) {
#endif
  if (con != 0U) {                      /* Connect */
    NRF_USBD->USBPULLUP |=  USBD_USBPULLUP_CONNECT_Msk;
  } else {                              /* Disconnect */
    NRF_USBD->USBPULLUP &= ~USBD_USBPULLUP_CONNECT_Msk;
  }
}


/*
 *  USB Device Reset Function
 *   Called automatically on USB Device Reset
 *    Return Value:    None
 */

void USBD_Reset (void) {

  /* Disable interrupts */
  NRF_USBD->INTENCLR = 0xFFFFFFFFU;

  /* Clear variables */
  setup_packet_available = 0U;
  ep0_dir = 1U;
  memset(ep_config, 0, sizeof(ep_config));

  /* Endpoint 0 is ready for SETUP packet reception when USB is ENABLED,
     set maximum packet size to 8 bytes */
  ep_config[0][0].max_packet_size = USBD_MAX_PACKET0;
  ep_config[1][0].max_packet_size = USBD_MAX_PACKET0;

  /* Enable relevant interrupts (USB Reset and Control Endpoint 0) */
  NRF_USBD->INTENSET = USBD_INTEN_USBRESET_Msk    | 
                       USBD_INTEN_EPDATA_Msk      | 
                       USBD_INTEN_EP0SETUP_Msk    | 
                       USBD_INTEN_SOF_Msk         |
                       USBD_INTEN_EP0DATADONE_Msk ;
}


/*
 *  USB Device Suspend Function
 *   Called automatically on USB Device Suspend
 *    Return Value:    None
 */

void USBD_Suspend (void) {
  /* Performed by Hardware */
}


/*
 *  USB Device Resume Function
 *   Called automatically on USB Device Resume
 *    Return Value:    None
 */

void USBD_Resume (void) {
  /* Performed by Hardware */
}


/*
 *  USB Device Remote Wakeup Function
 *   Called automatically on USB Device Remote Wakeup
 *    Return Value:    None
 */

void USBD_WakeUp (void) {
  NRF_USBD->LOWPOWER        = USBD_LOWPOWER_LOWPOWER_ForceNormal;
  NRF_USBD->DPDMVALUE       = USBD_DPDMVALUE_STATE_Resume;
  NRF_USBD->TASKS_DPDMDRIVE = USBD_TASKS_DPDMDRIVE_TASKS_DPDMDRIVE_Trigger;
}


/*
 *  USB Device Remote Wakeup Configuration Function
 *    Parameters:      cfg:   Device Enable/Disable
 *    Return Value:    None
 */

#ifdef   __DAPLINK
void USBD_WakeUpCfg (BOOL cfg) {
#else
void USBD_WakeUpCfg (uint32_t cfg) {
#endif
  /* Not needed */
}


/*
 *  USB Device Set Address Function
 *    Parameters:      adr:   USB Device Address
 *    Return Value:    None
 */

void USBD_SetAddress (uint32_t adr, uint32_t setup) {
  /* Performed by Hardware */
}


/*
 *  USB Device Configure Function
 *    Parameters:      cfg:   Device Configure/Deconfigure
 *    Return Value:    None
 */
#ifdef   __DAPLINK
void USBD_Configure (BOOL cfg) {
#else
void USBD_Configure (uint32_t cfg) {
#endif
  /* Not needed */
}


/*
 *  Configure USB Device Endpoint according to Descriptor
 *    Parameters:      pEPD:  Pointer to Device Endpoint Descriptor
 *    Return Value:    None
 */

void USBD_ConfigEP (USB_ENDPOINT_DESCRIPTOR *pEPD) {
  uint8_t  ep_dir, ep_num, ep_type;
  uint16_t ep_max_len;

  ep_dir     = (pEPD->bEndpointAddress &  USB_ENDPOINT_DIRECTION_MASK) >> 7;
  ep_num     =  pEPD->bEndpointAddress & ~USB_ENDPOINT_DIRECTION_MASK;
  ep_type    =  pEPD->bmAttributes & USB_ENDPOINT_TYPE_MASK;
  ep_max_len =  pEPD->wMaxPacketSize;

  if (ep_num >= USBD_EP_NUM_MAX) {      /* If ep_num is not available */
    return;
  }
  if (ep_type == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
    /* Isochronous Endpoint is not supported */
    return;
  }

  if (ep_max_len > 64U) {
    ep_max_len = 64U;                   /* Maximum packet size is 64 */
  }

  /* Store maximum packet size information into ep_config */
  ep_config[ep_dir][ep_num].max_packet_size = ep_max_len;
}


/*
 *  Set Direction for USB Device Control Endpoint
 *    Parameters:      dir:   Out (dir == 0), In (dir <> 0)
 *    Return Value:    None
 */

void USBD_DirCtrlEP (uint32_t dir) {
  if (dir == 0U) {
    ep0_dir = 0U;
  } else {
    ep0_dir = 1U;
  }
}


/*
 *  Enable USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_EnableEP (uint32_t EPNum) {
  uint8_t  ep_dir, ep_num;

  ep_dir = (EPNum &  USB_ENDPOINT_DIRECTION_MASK) >> 7;
  ep_num =  EPNum & ~USB_ENDPOINT_DIRECTION_MASK;

  if (ep_num >= USBD_EP_NUM_MAX) {      /* If ep_num is not available */
    return;
  }

  if (ep_dir != 0U) {                   /* If IN endpoint */
    /* Enable Endpoint */
    NRF_USBD->EPINEN  |= USBD_EPINEN_IN0_Msk     << ep_num;
  } else {                              /* If OUT endpoint */
    /* Enable Endpoint */
    NRF_USBD->EPOUTEN |= USBD_EPOUTEN_OUT0_Msk    << ep_num;
  }
}


/*
 *  Disable USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_DisableEP (uint32_t EPNum) {
  uint8_t  ep_dir, ep_num;

  ep_dir = (EPNum &  USB_ENDPOINT_DIRECTION_MASK) >> 7;
  ep_num =  EPNum & ~USB_ENDPOINT_DIRECTION_MASK;

  if (ep_num >= USBD_EP_NUM_MAX) {      /* If ep_num is not available */
    return;
  }

  if (ep_dir != 0U) {                   /* If IN endpoint */
    /* Disable Endpoint interrupt */
    NRF_USBD->INTENCLR = USBD_INTEN_ENDEPIN0_Msk << ep_num;

    /* Disable Endpoint */
    NRF_USBD->EPINEN  &= ~(USBD_EPINEN_IN0_Msk   << ep_num);
  } else {                              /* If OUT endpoint */
    /* Disable Endpoint interrupt */
    NRF_USBD->INTENCLR = USBD_INTEN_ENDEPIN0_Msk << ep_num;

    /* Disable Endpoint */
    NRF_USBD->EPINEN  &= ~(USBD_EPOUTEN_OUT0_Msk << ep_num);
  }
}


/*
 *  Reset USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ResetEP (uint32_t EPNum) {
  uint8_t  ep_dir, ep_num;

  ep_dir = (EPNum &  USB_ENDPOINT_DIRECTION_MASK) >> 7;
  ep_num =  EPNum & ~USB_ENDPOINT_DIRECTION_MASK;

  if (ep_num >= USBD_EP_NUM_MAX) {      /* If ep_num is not available */
    return;
  }

  /* Reset data toggle to DATA0 */
  if (ep_num != 0U) {
    if (ep_dir != 0U) {                   /* If IN endpoint */
      NRF_USBD->DTOGGLE = ((USBD_DTOGGLE_VALUE_Nop   << USBD_DTOGGLE_VALUE_Pos) & USBD_DTOGGLE_VALUE_Msk) | ep_num | USBD_DTOGGLE_IO_Msk;
      NRF_USBD->DTOGGLE = ((USBD_DTOGGLE_VALUE_Data0 << USBD_DTOGGLE_VALUE_Pos) & USBD_DTOGGLE_VALUE_Msk) | ep_num | USBD_DTOGGLE_IO_Msk;
    } else {                              /* If OUT endpoint */
      NRF_USBD->DTOGGLE = ((USBD_DTOGGLE_VALUE_Nop   << USBD_DTOGGLE_VALUE_Pos) & USBD_DTOGGLE_VALUE_Msk) | ep_num;
      NRF_USBD->DTOGGLE = ((USBD_DTOGGLE_VALUE_Data0 << USBD_DTOGGLE_VALUE_Pos) & USBD_DTOGGLE_VALUE_Msk) | ep_num;
    }
  }

  if (ep_dir ==0U) {
    /* Start OUT transfer (Receive data to USB controller's internal buffer) */
    NRF_USBD->SIZE.EPOUT[ep_num] = 0U;
  }
}


/*
 *  Set Stall for USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_SetStallEP (uint32_t EPNum) {
  uint8_t  ep_dir, ep_num;

  ep_dir = (EPNum &  USB_ENDPOINT_DIRECTION_MASK) >> 7;
  ep_num =  EPNum & ~USB_ENDPOINT_DIRECTION_MASK;

  if (ep_num >= USBD_EP_NUM_MAX) {      /* If ep_num is not available */
    return;
  }

  if (ep_num == 0U) {                   /* If Control Endpoint 0 */
    NRF_USBD->TASKS_EP0STALL = USBD_TASKS_EP0STALL_TASKS_EP0STALL_Trigger;
  } else {                              /* If any other Endpoint */
    /* Set stall */
    if (ep_dir != 0U) {                 /* If IN endpoint */
      NRF_USBD->EPSTALL = USBD_EPSTALL_STALL_Msk | ep_num | USBD_EPSTALL_IO_Msk;
    } else {                            /* If OUT endpoint */
      NRF_USBD->EPSTALL = USBD_EPSTALL_STALL_Msk | ep_num;
    }
  }
}


/*
 *  Clear Stall for USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ClrStallEP (uint32_t EPNum) {
  uint8_t ep_dir, ep_num;

  ep_dir = (EPNum &  USB_ENDPOINT_DIRECTION_MASK) >> 7;
  ep_num =  EPNum & ~USB_ENDPOINT_DIRECTION_MASK;

  if (ep_num >= USBD_EP_NUM_MAX) {      /* If ep_num is not available */
    return;
  }

  /* Clear stall */
  if (ep_dir != 0U) {                   /* If IN endpoint */
    NRF_USBD->EPSTALL = ep_num | USBD_EPSTALL_IO_Msk;
  } else {                              /* If OUT endpoint */
    NRF_USBD->EPSTALL = ep_num;
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

void USBD_ClearEPBuf (uint32_t EPNum) {
  /* Not needed */
}


/*
 *  Read USB Device Endpoint Data
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *    Return Value:    Number of bytes read
 */

#ifdef   __DAPLINK
uint32_t USBD_ReadEP (uint32_t EPNum, uint8_t *pData, U32 size) {
#else
uint32_t USBD_ReadEP (uint32_t EPNum, uint8_t *pData) {
#endif
  uint8_t  ep_dir, ep_num;
  uint16_t len;

  ep_dir = (EPNum &  USB_ENDPOINT_DIRECTION_MASK) >> 7;
  ep_num =  EPNum & ~USB_ENDPOINT_DIRECTION_MASK;

  if (ep_num >= USBD_EP_NUM_MAX) {      /* If ep_num is not available */
    return 0U;
  }
  if (ep_dir != 0U) {                   /* If not OUT Endpoint */
    return 0U;
  }
  if ((ep_num == 0U) && (setup_packet_available == 0U) && (ep0_dir != 0)) {
    /* Status stage of Device-to-Host handled in USBD_WriteEP */
    /* Note - ep0_dir is set only after the setup packet has been read */
    return 0U;
  }

  if ((ep_num == 0U) && (setup_packet_available != 0U)) {
    uint16_t wlength;
    setup_packet_available = 0U;

    /* If reading Endpoint 0 and Setup Packet is available */
    pData[0] = NRF_USBD->BMREQUESTTYPE;
    pData[1] = NRF_USBD->BREQUEST;
    pData[2] = NRF_USBD->WVALUEL;
    pData[3] = NRF_USBD->WVALUEH;
    pData[4] = NRF_USBD->WINDEXL;
    pData[5] = NRF_USBD->WINDEXH;
    pData[6] = NRF_USBD->WLENGTHL;
    pData[7] = NRF_USBD->WLENGTHH;
    len = 8U;

    /* Start status stage immediately if there is no data stage */
    wlength = (pData[7] << 8) | (pData[6] << 0);
    if (wlength == 0) {
        ep0_no_data_stage = 1;
        NRF_USBD->TASKS_EP0STATUS = USBD_TASKS_EP0STATUS_TASKS_EP0STATUS_Trigger;
    } else {
        ep0_no_data_stage = 0;
    }
  } else if ((ep_num == 0U) && (ep0_no_data_stage != 0)) {
      /* There isn't a data stage so don't try to read data */
      len = 0;
  } else {
    len = NRF_USBD->SIZE.EPOUT[ep_num];

#ifdef   __DAPLINK
    if (len > size) {
      len = size;
    }
#endif

    /* Read received data from USB controller's internal buffer to ep_buf.
       ep_buf is used to satisfy required 32-bit alignment, and size multiple of 4. */
    usbd_dma(EPNum, &ep_buf[ep_dir][ep_num][0], ((len+3U)/4U)*4U);

    /* Copy data received to requested pData */
    memcpy((void *)pData, (const void *)&ep_buf[ep_dir][ep_num][0], len);

    /* Start the status stage if endpoint 0 */
    if (ep_num == 0U) {
        /* TODO - Only start the status stage after reading the last OUT packet.
            Until that is done control Host-To-Device transfers consisting
            of multiple OUT packets likely won't work. */
        NRF_USBD->TASKS_EP0STATUS = USBD_TASKS_EP0STATUS_TASKS_EP0STATUS_Trigger;
    }
  }

  /* Start OUT transfer (Receive data to USB controller's internal buffer) */
  NRF_USBD->SIZE.EPOUT[ep_num] = 0U;

  return len;
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

uint32_t USBD_WriteEP (uint32_t EPNum, uint8_t *pData, uint32_t cnt) {
  uint8_t  ep_dir, ep_num;
  uint16_t len, ep_max_len;

  ep_dir     = (EPNum &  USB_ENDPOINT_DIRECTION_MASK) >> 7;
  ep_num     =  EPNum & ~USB_ENDPOINT_DIRECTION_MASK;
  len        =  cnt;

  if (ep_num >= USBD_EP_NUM_MAX) {      /* If ep_num is not available */
    return 0U;
  }
  if (ep_dir == 0U) {                   /* If not IN Endpoint */
    return 0U;
  }
  if ((ep_num == 0U) && (ep0_no_data_stage != 0)) {
    /* There isn't a data stage so don't try to write data */
    return 0U;
  }
  if ((ep_num == 0U) && (ep0_dir == 0)) {
    /* Status stage of Host-to-Device handled elsewhere */
    return 0U;
  }

  ep_max_len =  ep_config[ep_dir][ep_num].max_packet_size;

  if (len > ep_max_len) {
    len = ep_max_len;
  }

  /* If this is a short packet on endpoint 0 then it is the last packet
     of the transfer. Start the status stage after it is sent (the next IN event) */
  if ((ep_num == 0U) && (len < ep_max_len)) {
      ep0_status_next = 1U;
  }

  /* Copy data to be sent to ep_buf */
  memcpy((void *)&ep_buf[ep_dir][ep_num][0], (const void *)pData, len);

  /* Do DMA transfer of data from ep_buf to USB controller's internal buffer */
  usbd_dma(EPNum, &ep_buf[ep_dir][ep_num][0], len);

  /* After data is sent EP0DATADONE/EPDATA event will happen and be processed by IRQ */

  return len;
}


/*
 *  Get USB Device Last Frame Number
 *    Parameters:      None
 *    Return Value:    Frame Number
 */

uint32_t USBD_GetFrame (void) {
  return NRF_USBD->FRAMECNTR;
}


/*
 *  Get USB Device Last Error Code
 *    Parameters:      None
 *    Return Value:    Error Code
 */

uint32_t USBD_GetError (void) {
  return 0U;
}


/*
 *  USB Device Interrupt Service Routine
 */
#ifdef __DAPLINK
void USBD_IRQHandler (void) {
  NVIC_DisableIRQ(USBD_IRQn);
  USBD_SignalHandler();
}

void USBD_Handler (void) {
#else
void USBD_IRQHandler (void) {
#endif
  uint32_t eventcause, epdatastatus, ep_num;

  /* Check USB Reset event */
  if (NRF_USBD->EVENTS_USBRESET != 0U) {
    NRF_USBD->EVENTS_USBRESET = 0U;
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

  /* Check USB Suspend/Resume events */
  if (NRF_USBD->EVENTS_USBEVENT != 0U) {
    eventcause = NRF_USBD->EVENTCAUSE;
    NRF_USBD->EVENTCAUSE      = 0U;
    NRF_USBD->EVENTS_USBEVENT = 0U;
    if ((eventcause & USBD_EVENTCAUSE_SUSPEND_Msk) != 0U) {
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
    if ((eventcause & USBD_EVENTCAUSE_RESUME_Msk) != 0U) {
      USBD_Resume();
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

  /* Check USB SOF event */
  if (NRF_USBD->EVENTS_SOF != 0U) {
    NRF_USBD->EVENTS_SOF = 0U;
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

  /* Check Endpoint 0 Setup Packet received event */
  if (NRF_USBD->EVENTS_EP0SETUP != 0U) {
    setup_packet_available = 1U;
    ep0_status_next = 0U;
    NRF_USBD->EVENTS_EP0SETUP = 0U;
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

  /* Check Endpoint 0 Data Packet sent/received event */
  if (NRF_USBD->EVENTS_EP0DATADONE != 0U) {
    uint32_t local_ep0_status_next = ep0_status_next;
    NRF_USBD->EVENTS_EP0DATADONE = 0U;
#ifdef __RTX
    if (USBD_RTX_EPTask[0]) {
      if (ep0_dir != 0U) {
        isr_evt_set(USBD_EVT_IN, USBD_RTX_EPTask[0]);
      } else {
        isr_evt_set(USBD_EVT_OUT, USBD_RTX_EPTask[0]);
      }
    }
#else
    if (USBD_P_EP[0]) {
      if (ep0_dir != 0U) {
        USBD_P_EP[0](USBD_EVT_IN);
      } else {
        USBD_P_EP[0](USBD_EVT_OUT);
      }
    }
#endif
    /* Enable Status stage */
    if (local_ep0_status_next) {
		NRF_USBD->TASKS_EP0STATUS = USBD_TASKS_EP0STATUS_TASKS_EP0STATUS_Trigger;
    }
  }

  /* Check Endpoint sent/received event */
  if (NRF_USBD->EVENTS_EPDATA != 0U) {
    epdatastatus = NRF_USBD->EPDATASTATUS;
    NRF_USBD->EVENTS_EPDATA = 0U;
    NRF_USBD->EPDATASTATUS  = epdatastatus;
    for (ep_num = 1U; ep_num < USBD_EP_NUM_MAX; ep_num++) {
      if ((epdatastatus & (1U << ep_num)) != 0U) {
#ifdef __RTX
        if (USBD_RTX_EPTask[ep_num]) {
          isr_evt_set(USBD_EVT_IN, USBD_RTX_EPTask[ep_num]);
        }
#else
        if (USBD_P_EP[ep_num]) {
          USBD_P_EP[ep_num](USBD_EVT_IN);
        }
#endif
      }
    }

    for (ep_num = 1U; ep_num < USBD_EP_NUM_MAX; ep_num++) {
      if ((epdatastatus & ((1U << 16) << ep_num)) != 0U) {
#ifdef __RTX
        if (USBD_RTX_EPTask[ep_num]) {
          isr_evt_set(USBD_EVT_OUT, USBD_RTX_EPTask[ep_num]);
        }
#else
        if (USBD_P_EP[ep_num]) {
          USBD_P_EP[ep_num](USBD_EVT_OUT);
        }
#endif
      }
    }
  }

#ifdef __DAPLINK
  NVIC_EnableIRQ(USBD_IRQn);
#endif
}
