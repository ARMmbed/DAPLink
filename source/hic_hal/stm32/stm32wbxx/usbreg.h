/*
 * Copyright (c) 2004-2016 ARM Limited. All rights reserved.
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
 *      Name:    usbreg.h
 *      Purpose: Hardware Layer Definitions for ST STM32F37x
 *---------------------------------------------------------------------------*/

#ifndef __USBREG_H
#define __USBREG_H


#define REG(x)  (*((volatile unsigned int *)(x)))
	
//#define PERIPH_BASE           0x40000000UL /*!< Peripheral base address in the alias region */
#define APB1PERIPH_BASE       PERIPH_BASE

//#define USB_BASE               (APB1PERIPH_BASE + 0x00005C00UL) /*!< USB_IP Peripheral Registers base address */
#define USB_PMA_ADDR           (APB1PERIPH_BASE + 0x00006000UL) /*!< USB_IP Packet Memory Area base address */

/* Common Registers */
#define CNTR                             REG(USB_BASE + 0x40U)             /*!< Control register */  //biby
#define ISTR                             REG(USB_BASE + 0x44U)             /*!< Interrupt status register */
#define FNR                              REG(USB_BASE + 0x48U)             /*!< Frame number register */
#define DADDR                            REG(USB_BASE + 0x4CU)             /*!< Device address register */
#define BTABLE                           REG(USB_BASE + 0x50U)             /*!< Buffer Table address register */  //biby

/* CNTR: Control Register Bit Definitions */
#define CNTR_CTRM                        ((uint16_t)0x8000U)               /*!< Correct TRansfer Mask */
#define CNTR_PMAOVRM                     ((uint16_t)0x4000U)               /*!< DMA OVeR/underrun Mask */
#define CNTR_ERRM                        ((uint16_t)0x2000U)               /*!< ERRor Mask */
#define CNTR_WKUPM                       ((uint16_t)0x1000U)               /*!< WaKe UP Mask */
#define CNTR_SUSPM                       ((uint16_t)0x0800U)               /*!< SUSPend Mask */
#define CNTR_RESETM                      ((uint16_t)0x0400U)               /*!< RESET Mask   */
#define CNTR_SOFM                        ((uint16_t)0x0200U)               /*!< Start Of Frame Mask */
#define CNTR_ESOFM                       ((uint16_t)0x0100U)               /*!< Expected Start Of Frame Mask */
#define CNTR_RESUME                      ((uint16_t)0x0010U)               /*!< RESUME request */
#define CNTR_FSUSP                       ((uint16_t)0x0008U)               /*!< Force SUSPend */
#define CNTR_LPMODE                      ((uint16_t)0x0004U)               /*!< Low-power MODE */
#define CNTR_PDWN                        ((uint16_t)0x0002U)               /*!< Power DoWN */
#define CNTR_FRES                        ((uint16_t)0x0001U)               /*!< Force USB RESet */

/* ISTR: Interrupt Status Register Bit Definitions */
#define ISTR_CTR                         ((uint16_t)0x8000U)               /*!< Correct TRansfer (clear-only bit) */
#define ISTR_PMAOVR                      ((uint16_t)0x4000U)               /*!< DMA OVeR/underrun (clear-only bit) */
#define ISTR_ERR                         ((uint16_t)0x2000U)               /*!< ERRor (clear-only bit) */
#define ISTR_WKUP                        ((uint16_t)0x1000U)               /*!< WaKe UP (clear-only bit) */
#define ISTR_SUSP                        ((uint16_t)0x0800U)               /*!< SUSPend (clear-only bit) */
#define ISTR_RESET                       ((uint16_t)0x0400U)               /*!< RESET (clear-only bit) */
#define ISTR_SOF                         ((uint16_t)0x0200U)               /*!< Start Of Frame (clear-only bit) */
#define ISTR_ESOF                        ((uint16_t)0x0100U)               /*!< Expected Start Of Frame (clear-only bit) */
#define ISTR_DIR                         ((uint16_t)0x0010U)               /*!< DIRection of transaction (read-only bit)  */
#define ISTR_EP_ID                       ((uint16_t)0x000FU)               /*!< EndPoint IDentifier (read-only bit)  */

/* FNR: Frame Number Register Bit Definitions */
#define FNR_RXDP                         ((uint16_t)0x8000U)               /*!< status of D+ data line */
#define FNR_RXDM                         ((uint16_t)0x4000U)               /*!< status of D- data line */
#define FNR_LCK                          ((uint16_t)0x2000U)               /*!< LoCKed */
#define FNR_LSOF                         ((uint16_t)0x1800U)               /*!< Lost SOF */
#define FNR_FN                           ((uint16_t)0x07FFU)               /*!< Frame Number */

/* DADDR: Device Address Register Bit Definitions */
#define DADDR_EF                         ((uint8_t)0x80U)                  /*!< USB device address Enable Function */
#define DADDR_ADD                        ((uint8_t)0x7FU)                  /*!< USB device address */

/* EndPoint Registers */
#define EPxREG(x)       REG(USB_BASE + 4*(x))
//#define EP0REG(0)                             REG(USB_BASE)                    /*!< endpoint 0 register address */
//#define EP1REG(1)                             REG(USB_BASE + 0x04U)           /*!< endpoint 1 register address */
//#define EP2REG(2)                             REG(USB_BASE + 0x08U)           /*!< endpoint 2 register address */
//#define EP3REG(3)                             REG(USB_BASE + 0x0CU)           /*!< endpoint 3 register address */
//#define EP4REG(4)                             REG(USB_BASE + 0x10U)           /*!< endpoint 4 register address */
//#define EP5REG(5)                             REG(USB_BASE + 0x14U)           /*!< endpoint 5 register address */
//#define EP6REG(6)                             REG(USB_BASE + 0x18U)           /*!< endpoint 6 register address */
//#define EP7REG(7)                             REG(USB_BASE + 0x1CU)           /*!< endpoint 7 register address */


/* EPxREG: EndPoint Registers Bit Definitions */
#define EP_CTR_RX                        ((uint16_t)0x8000U)               /*!<  EndPoint Correct TRansfer RX */
#define EP_DTOG_RX                       ((uint16_t)0x4000U)               /*!<  EndPoint Data TOGGLE RX */
#define EP_STAT_RX                       ((uint16_t)0x3000U)               /*!<  EndPoint RX STATus bit field */
#define EP_SETUP                         ((uint16_t)0x0800U)               /*!<  EndPoint SETUP */
#define EP_TYPE                          ((uint16_t)0x0600U)               /*!<  EndPoint TYPE */
#define EP_KIND                          ((uint16_t)0x0100U)               /*!<  EndPoint KIND */
#define EP_CTR_TX                        ((uint16_t)0x0080U)               /*!<  EndPoint Correct TRansfer TX */
#define EP_DTOG_TX                       ((uint16_t)0x0040U)               /*!<  EndPoint Data TOGGLE TX */
#define EP_STAT_TX                       ((uint16_t)0x0030U)               /*!<  EndPoint TX STATus bit field */
#define EP_EA                            ((uint16_t)0x000FU)               /*!<  EndPoint ADDRess FIELD */

/* EndPoint Register Mask (No Toggle Fields) */
#define EP_MASK         (EP_CTR_RX|EP_SETUP|EP_TYPE|EP_KIND|EP_CTR_TX|EP_EA)
/* EndPoint Register Mask (Write zero to clear) */
#define EP_MASK_RC_W0   (EP_CTR_RX|EP_CTR_TX)
/* Mask off all toggle bits and set write zero to clear bits to 1.          */
/* This creates a value that can be written back to the EndPoint register   */
/* which does not change any status bits.                                   */
#define EP_VAL_UNCHANGED(val)       (((val) & EP_MASK) | EP_MASK_RC_W0)

/* EP_TYPE: EndPoint Types */
#define EP_BULK                          ((uint16_t)0x0000U)               /*!< EndPoint BULK */
#define EP_CONTROL                       ((uint16_t)0x0200U)               /*!< EndPoint CONTROL */
#define EP_ISOCHRONOUS                   ((uint16_t)0x0400U)               /*!< EndPoint ISOCHRONOUS */
#define EP_INTERRUPT                     ((uint16_t)0x0600U)               /*!< EndPoint INTERRUPT */

/* EP_KIND: EndPoint Kind */
#define EP_DBL_BUF      EP_KIND     /* Double Buffer for Bulk Endpoint */
#define EP_STATUS_OUT   EP_KIND     /* Status Out for Control Endpoint */

/* EP_STAT_TX: TX Status */
#define EP_TX_DIS                        ((uint16_t)0x0000U)               /*!< EndPoint TX DISabled */
#define EP_TX_STALL                      ((uint16_t)0x0010U)               /*!< EndPoint TX STALLed */
#define EP_TX_NAK                        ((uint16_t)0x0020U)               /*!< EndPoint TX NAKed */
#define EP_TX_VALID                      ((uint16_t)0x0030U)               /*!< EndPoint TX VALID */

/* EP_STAT_RX: RX Status */
#define EP_RX_DIS                        ((uint16_t)0x0000U)               /*!< EndPoint RX DISabled */
#define EP_RX_STALL                      ((uint16_t)0x1000U)               /*!< EndPoint RX STALLed */
#define EP_RX_NAK                        ((uint16_t)0x2000U)               /*!< EndPoint RX NAKed */
#define EP_RX_VALID                      ((uint16_t)0x3000U)               /*!< EndPoint RX VALID */

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
