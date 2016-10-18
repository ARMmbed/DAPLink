/**
 * @file    usbreg.c
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

#ifndef __USBREG_H
#define __USBREG_H


#define REG(x)  (*((volatile unsigned int *)(x)))

#define USB_BASE_ADDR   0x40005C00  /* USB Registers Base Address */
#define USB_PMA_ADDR    0x40006000  /* USB Packet Memory Area Address */


/* Common Registers */
#define CNTR    REG(USB_BASE_ADDR + 0x40)   /* Control Register */
#define ISTR    REG(USB_BASE_ADDR + 0x44)   /* Interrupt Status Register */
#define FNR     REG(USB_BASE_ADDR + 0x48)   /* Frame Number Register */
#define DADDR   REG(USB_BASE_ADDR + 0x4C)   /* Device Address Register */
#define BTABLE  REG(USB_BASE_ADDR + 0x50)   /* Buffer Table Address Register */

/* CNTR: Control Register Bit Definitions */
#define CNTR_STIE       0x8000      /* Correct Transfer Interrupt Mask */
#define CNTR_PMOUIE     0x4000      /* Packet Memory Aerea Over/underrun Interrupt Mask */
#define CNTR_ERRIE      0x2000      /* Error Interrupt Mask */
#define CNTR_WKUPIE     0x1000      /* Wake-up Interrupt Mask */
#define CNTR_SPSIE      0x0800      /* Suspend Mode Interrupt Mask  */
#define CNTR_RSTIE      0x0400      /* USB Reset Interrupt Mask   */
#define CNTR_SOFIE      0x0200      /* Start of Frame Interrupt Mask */
#define CNTR_ESOFIE     0x0100      /* Expected Start of Frame Interrupt Mask */
#define CNTR_RSREQ      0x0010      /* Resume Request */
#define CNTR_SETSPS     0x0008      /* Force Suspend */
#define CNTR_LOWM       0x0004      /* Low-power Mode */
#define CNTR_CLOSE      0x0002      /* Power Down */
#define CNTR_SETRST     0x0001      /* Force USB Reset */

/* ISTR: Interrupt Status Register Bit Definitions */
#define ISTR_STIF       0x8000      /* Correct Transfer */
#define ISTR_PMOUIF     0x4000      /* Packet Memory Aerea Over/underrun */
#define ISTR_ERRIF      0x2000      /* Error */
#define ISTR_WKUPIF     0x1000      /* Wake-up */
#define ISTR_SPSIF      0x0800      /* Suspend Mode */
#define ISTR_RSTIF      0x0400      /* USB Reset */
#define ISTR_SOFIF      0x0200      /* Start of Frame */
#define ISTR_ESOFIF     0x0100      /* Expected Start of Frame */
#define ISTR_DIR        0x0010      /* Direction of Transaction */
#define ISTR_EPNUM      0x000F      /* EndPoint Identifier */

/* FNR: Frame Number Register Bit Definitions */
#define FNR_RX_DP       0x8000      /* D+ Data Line Status */
#define FNR_RX_DM       0x4000      /* D- Data Line Status */
#define FNR_LOCK        0x2000      /* Locked */
#define FNR_SOFLN       0x1800      /* Lost SOF */
#define FNR_FCNT        0x07FF      /* Frame Number */

/* DADDR: Device Address Register Bit Definitions */
#define DADDR_USBEN     0x0080      /* Enable Function */
#define DADDR_USBADD    0x007F      /* Device Address */


/* EndPoint Registers */
#define EPxREG(x)       REG(USB_BASE_ADDR + 4*(x))

/* EPxREG: EndPoint Registers Bit Definitions */
#define EP_RX_ST        0x8000      /* Correct RX Transfer */
#define EP_RX_DTG       0x4000      /* RX Data Toggle */
#define EP_RX_STA       0x3000      /* RX Status */
#define EP_SETUP        0x0800      /* EndPoint Setup */
#define EP_CTL          0x0600      /* EndPoint Type */
#define EP_KCTL         0x0100      /* EndPoint Kind */
#define EP_TX_ST        0x0080      /* Correct TX Transfer */
#define EP_TX_DTG       0x0040      /* TX Data Toggle */
#define EP_TX_STA       0x0030      /* TX Status */
#define EP_AR           0x000F      /* EndPoint Address */

/* EndPoint Register Mask (No Toggle Fields) */
#define EP_MASK         (EP_RX_ST|EP_SETUP|EP_CTL|EP_KCTL|EP_TX_ST|EP_AR)

/* EP_CTL: EndPoint Types */
#define EP_BULK         0x0000      /* BULK EndPoint */
#define EP_CONTROL      0x0200      /* CONTROL EndPoint */
#define EP_ISOCHRONOUS  0x0400      /* ISOCHRONOUS EndPoint */
#define EP_INTERRUPT    0x0600      /* INTERRUPT EndPoint */

/* EP_KCTL: EndPoint Kind */
//#define EP_DBL_BUF      EP_KCTL     /* Double Buffer for Bulk Endpoint */
//#define EP_STATUS_OUT   EP_KCTL     /* Status Out for Control Endpoint */

/* EP_TX_STA: TX Status */
#define EP_TX_DIS       0x0000      /* Disabled */
#define EP_TX_STALL     0x0010      /* Stalled */
#define EP_TX_NAK       0x0020      /* NAKed */
#define EP_TX_VALID     0x0030      /* Valid */

/* EP_RX_STA: RX Status */
#define EP_RX_DIS       0x0000      /* Disabled */
#define EP_RX_STALL     0x1000      /* Stalled */
#define EP_RX_NAK       0x2000      /* NAKed */
#define EP_RX_VALID     0x3000      /* Valid */


/* Endpoint Buffer Descriptor */
typedef struct _EP_BUF_DSCR {
  U32 ADDR_TX;
  U32 COUNT_TX;
  U32 ADDR_RX;
  U32 COUNT_RX;
} EP_BUF_DSCR;

#define EP_ADDR_MASK    0xFFFE      /* Address Mask */
#define EP_COUNT_MASK   0x03FF      /* Count Mask */


#endif  /* __USBREG_H */
