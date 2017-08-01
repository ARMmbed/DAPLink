/**
 *******************************************************************************
 * @file    TMPM366.h
 * @brief   CMSIS Cortex-M3 Core Peripheral Access Layer Header File for the
 *          TOSHIBA 'TMPM366' Device Series
 * @version V2.0.2.5
 * @date    2014/05/20
 *
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
 *******************************************************************************
 */

/** @addtogroup TOSHIBA_TX03_MICROCONTROLLER
  * @{
  */

/** @addtogroup TMPM366
  * @{
  */

#ifndef __TMPM366_H__
#define __TMPM366_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Configuration_of_CMSIS
  * @{
  */

/** Interrupt Number Definition */
typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************************/
  NonMaskableInt_IRQn           = -14,       /*!< 2 Non Maskable Interrupt                                    */
  HardFault_IRQn                = -13,       /*!< 3 Cortex-M3 Hard Fault Interrupt                            */
  MemoryManagement_IRQn         = -12,       /*!< 4 Cortex-M3 Memory Management Interrupt                     */
  BusFault_IRQn                 = -11,       /*!< 5 Cortex-M3 Bus Fault Interrupt                             */
  UsageFault_IRQn               = -10,       /*!< 6 Cortex-M3 Usage Fault Interrupt                           */
  SVCall_IRQn                   = -5,        /*!< 11 Cortex-M3 SV Call Interrupt                              */
  DebugMonitor_IRQn             = -4,        /*!< 12 Cortex-M3 Debug Monitor Interrupt                        */
  PendSV_IRQn                   = -2,        /*!< 14 Cortex-M3 Pend SV Interrupt                              */
  SysTick_IRQn                  = -1,        /*!< 15 Cortex-M3 System Tick Interrupt                          */

/******  TMPM366 Specific Interrupt Numbers *******************************************************************/
  INT0_IRQn                     = 0,         /*!< Interrupt pin 0                                             */
  INT1_IRQn                     = 1,         /*!< Interrupt pin 1                                             */
  INT2_IRQn                     = 2,         /*!< Interrupt pin 2                                             */
  INT3_IRQn                     = 3,         /*!< Interrupt pin 3                                             */
  INT4_IRQn                     = 4,         /*!< Interrupt pin 4                                             */
  INT5_IRQn                     = 5,         /*!< Interrupt pin 5                                             */
  INT6_IRQn                     = 6,         /*!< Interrupt pin 6                                             */
  INT7_IRQn                     = 7,         /*!< Interrupt pin 7                                             */
  INTRX0_IRQn                   = 8,         /*!< Serial reception (channel.0)                                */
  INTTX0_IRQn                   = 9,         /*!< Serial transmission (channel.0)                             */
  INTRX1_IRQn                   = 10,        /*!< Serial reception (channel.1)                                */
  INTTX1_IRQn                   = 11,        /*!< Serial transmission (channel.1)                             */
  INTUSBWKUP_IRQn               = 12,        /*!< USB WakeUp                                                  */
  INTSBI0_IRQn                  = 14,        /*!< Serial bus interface 0                                      */
  INTSBI1_IRQn                  = 15,        /*!< Serial bus interface 1                                      */
  INTADHP_IRQn                  = 16,        /*!< Highest priority AD conversion complete interrupt           */
  INTAD_IRQn                    = 17,        /*!< AD conversion interrupt                                     */
  INTADM0_IRQn                  = 18,        /*!< AD conversion monitoring function interrupt 0               */
  INTADM1_IRQn                  = 19,        /*!< AD conversion monitoring function interrupt 1               */
  INTTB0_IRQn                   = 20,        /*!< 16-bit TMRB match detection 0                               */
  INTTB1_IRQn                   = 21,        /*!< 16-bit TMRB match detection 1                               */
  INTTB2_IRQn                   = 22,        /*!< 16-bit TMRB match detection 2                               */
  INTTB3_IRQn                   = 23,        /*!< 16-bit TMRB match detection 3                               */
  INTTB4_IRQn                   = 24,        /*!< 16-bit TMRB match detection 4                               */
  INTTB5_IRQn                   = 25,        /*!< 16-bit TMRB match detection 5                               */
  INTTB6_IRQn                   = 26,        /*!< 16-bit TMRB match detection 6                               */
  INTTB7_IRQn                   = 27,        /*!< 16-bit TMRB match detection 7                               */
  INTTB8_IRQn                   = 28,        /*!< 16-bit TMRB match detection 8                               */
  INTTB9_IRQn                   = 29,        /*!< 16-bit TMRB match detection 9                               */
  INTUSB_IRQn                   = 30,        /*!< USB Interrupt                                               */
  INTSSP2_IRQn                  = 31,        /*!< SPI serial interface 2                                      */
  INTUSBPON_IRQn                = 34,        /*!< USB V-Bus _Power on Detect                                  */
  INTUART_IRQn                  = 35,        /*!< UART Interrupt                                              */
  INTCAP00_IRQn                 = 36,        /*!< 16-bit TMRB input capture 00                                */
  INTCAP01_IRQn                 = 37,        /*!< 16-bit TMRB input capture 01                                */
  INTCAP10_IRQn                 = 38,        /*!< 16-bit TMRB input capture 10                                */
  INTCAP11_IRQn                 = 39,        /*!< 16-bit TMRB input capture 11                                */
  INTCAP20_IRQn                 = 40,        /*!< 16-bit TMRB input capture 20                                */
  INTCAP21_IRQn                 = 41,        /*!< 16-bit TMRB input capture 21                                */
  INTCAP30_IRQn                 = 42,        /*!< 16-bit TMRB input capture 30                                */
  INTCAP31_IRQn                 = 43,        /*!< 16-bit TMRB input capture 31                                */
  INTCAP40_IRQn                 = 44,        /*!< 16-bit TMRB input capture 40                                */
  INTCAP41_IRQn                 = 45,        /*!< 16-bit TMRB input capture 41                                */
  INTCAP50_IRQn                 = 46,        /*!< 16-bit TMRB input capture 50                                */
  INTCAP51_IRQn                 = 47,        /*!< 16-bit TMRB input capture 51                                */
  INTCAP60_IRQn                 = 48,        /*!< 16-bit TMRB input capture 60                                */
  INTCAP61_IRQn                 = 49,        /*!< 16-bit TMRB input capture 61                                */
  INTCAP70_IRQn                 = 50,        /*!< 16-bit TMRB input capture 70                                */
  INTCAP71_IRQn                 = 51,        /*!< 16-bit TMRB input capture 71                                */
  INTCAP80_IRQn                 = 52,        /*!< 16-bit TMRB input capture 80                                */
  INTCAP81_IRQn                 = 53,        /*!< 16-bit TMRB input capture 81                                */
  INTCAP90_IRQn                 = 54,        /*!< 16-bit TMRB input capture 90                                */
  INTCAP91_IRQn                 = 55,        /*!< 16-bit TMRB input capture 91                                */
  INT8_IRQn                     = 56,        /*!< Interrupt pin 8                                             */
  INT9_IRQn                     = 57,        /*!< Interrupt pin 9                                             */
  INTSSP1_IRQn                  = 58,        /*!< SPI serial interface 1                                      */
  INTSSP0_IRQn                  = 59,        /*!< SPI serial interface 0                                      */
  INTDMAC0TC_IRQn               = 60,        /*!< DMA terminal count status interrupt 0                       */
  INTDMAC1TC_IRQn               = 61,        /*!< DMA terminal count status interrupt 1                       */
  INTDMAC0ERR_IRQn              = 62,        /*!< DMA error status interrupt 0                                */
  INTDMAC1ERR_IRQn              = 63         /*!< DMA error status interrupt 1                                */
} IRQn_Type;

/** Processor and Core Peripheral Section */

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __CM3_REV              0x0200       /*!< Cortex-M3 Core Revision                           */
#define __MPU_PRESENT             0         /*!< MPU present or not                                */
#define __NVIC_PRIO_BITS          3         /*!< Number of Bits used for Priority Levels           */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used      */

/** @} */ /* End of group Configuration_of_CMSIS */

#include "core_cm3.h"                       /* Cortex-M3 processor and core peripherals            */
#include "system_TMPM366.h"                 /* TMPM366 System                                      */

/** @addtogroup Device_Peripheral_registers
  * @{
  */

/** Device Specific Peripheral registers structures */

/**
  * @brief DMA Controller (DMAC)
  */
typedef struct
{
  __I  uint32_t INTSTATUS;         /*!< DMAC Interrupt Status Register               */
  __I  uint32_t INTTCSTATUS;       /*!< DMAC Interrupt Terminal Count Status Register*/
  __O  uint32_t INTTCCLEAR;        /*!< DMAC Interrupt Terminal Count Clear Register */
  __I  uint32_t INTERRORSTATUS;    /*!< DMAC Interrupt Error Status Register         */
  __O  uint32_t INTERRCLR;         /*!< DMAC Interrupt Error Clear Register          */
  __I  uint32_t RAWINTTCSTATUS;    /*!< DMAC Raw Interrupt Terminal Count Status Register*/
  __I  uint32_t RAWINTERRORSTATUS; /*!< DMAC Raw Error Interrupt Status Register     */
  __I  uint32_t ENBLDCHNS;         /*!< DMAC Enabled Channel Register                */
  __IO uint32_t SOFTBREQ;          /*!< DMAC Software Burst Request Register         */
  __IO uint32_t SOFTSREQ;          /*!< DMAC Software Single Request Register        */
       uint32_t RESERVED0[2];
  __IO uint32_t CONFIGURATION;     /*!< DMAC Configuration Register                  */
       uint32_t RESERVED1[51];
  __IO uint32_t C0SRCADDR;         /*!< DMAC Channel0 Source Address Register        */
  __IO uint32_t C0DESTADDR;        /*!< DMAC Channel0 Destination Address Register   */
  __IO uint32_t C0LLI;             /*!< DMAC Channel0 Linked List Item Register      */
  __IO uint32_t C0CONTROL;         /*!< DMAC Channel0 Control Register               */
  __IO uint32_t C0CONFIGURATION;   /*!< DMAC Channel0 Configuration Register         */
       uint32_t RESERVED2[3];
  __IO uint32_t C1SRCADDR;         /*!< DMAC Channel1 Source Address Register        */
  __IO uint32_t C1DESTADDR;        /*!< DMAC Channel1 Destination Address Register   */
  __IO uint32_t C1LLI;             /*!< DMAC Channel1 Linked List Item Register      */
  __IO uint32_t C1CONTROL;         /*!< DMAC Channel1 Control Register               */
  __IO uint32_t C1CONFIGURATION;   /*!< DMAC Channel1 Configuration Register         */
} TSB_DMAC_TypeDef;

/**
  * @brief UDC2 AHB Bridge (UDFS)
  */
typedef struct
{
  __IO uint32_t INTSTS;            /*!< Interrupt Status Register                    */
  __IO uint32_t INTENB;            /*!< Interrupt Enable Register                    */
  __IO uint32_t MWTOUT;            /*!< Master Write Timeout Register                */
  __IO uint32_t C2STSET;           /*!< UDC2 setting                                 */
  __IO uint32_t MSTSET;            /*!< DMAC setting                                 */
  __IO uint32_t DMACRDREQ;         /*!< DMAC Read request                            */
  __I  uint32_t DMACRDVL;          /*!< DMAC Read Value                              */
  __IO uint32_t UDC2RDREQ;         /*!< UDC2 Read Request                            */
  __I  uint32_t UDC2RDVL;          /*!< UDC2 Read Value                              */
       uint32_t RESERVED0[6];
  __IO uint32_t ARBTSET;           /*!< Arbiter Setting                              */
  __IO uint32_t MWSADR;            /*!< Master Write Start Address                   */
  __IO uint32_t MWEADR;            /*!< Master Write End Address                     */
  __I  uint32_t MWCADR;            /*!< Master Write Current Address                 */
  __I  uint32_t MWAHBADR;          /*!< Master Write AHB Address                     */
  __IO uint32_t MRSADR;            /*!< Master Read Start Address                    */
  __IO uint32_t MREADR;            /*!< Master Read End Address                      */
  __I  uint32_t MRCADR;            /*!< Master Read Current Address                  */
  __I  uint32_t MRAHBADR;          /*!< Master Read AHB Address                      */
       uint32_t RESERVED1[8];
  __IO uint32_t PWCTL;             /*!< Power Detect Control                         */
  __I  uint32_t MSTSTS;            /*!< Master Status                                */
  __I  uint32_t TOUTCNT;           /*!< Timeout Count                                */
} TSB_UDFS_TypeDef;

/**
  * @brief UDC2(USB -Spec2.0 Device contoller) (UDFS2)
  */
typedef struct
{
  __IO uint32_t ADR;               /*!< UDC2 Address State                           */
  __IO uint32_t FRM;               /*!< UDC2 Frame                                   */
       uint32_t RESERVED0;
  __IO uint32_t CMD;               /*!< UDC2 Command                                 */
  __I  uint32_t BRQ;               /*!< UDC2 bRequest-bmRequest Type                 */
  __I  uint32_t WVL;               /*!< UDC2 wValue                                  */
  __I  uint32_t WIDX;              /*!< UDC2 wIndex                                  */
  __I  uint32_t WLGTH;             /*!< UDC2 wLength                                 */
  __IO uint32_t INT;               /*!< UDC2 INT                                     */
  __IO uint32_t INTEP;             /*!< UDC2 INT_EP                                  */
  __IO uint32_t INTEPMSK;          /*!< UDC2 INT_EP_MASK                             */
  __IO uint32_t INTRX0;            /*!< UDC2 INT RX DATA0                            */
  __IO uint32_t EP0MSZ;            /*!< UDC2 EP0 Max Packet Size                     */
  __I  uint32_t EP0STS;            /*!< UDC2 EP0 Status                              */
  __I  uint32_t EP0DSZ;            /*!< UDC2 EP0 Data Size                           */
  __IO uint32_t EP0FIFO;           /*!< UDC2 EP0 FIFO                                */
  __IO uint32_t EP1MSZ;            /*!< UDC2 EP1 Max Packet Size                     */
  __IO uint32_t EP1STS;            /*!< UDC2 EP1 Status                              */
  __I  uint32_t EP1DSZ;            /*!< UDC2 EP1 Data Size                           */
  __IO uint32_t EP1FIFO;           /*!< UDC2 EP1 FIFO                                */
  __IO uint32_t EP2MSZ;            /*!< UDC2 EP2 Max Packet Size                     */
  __IO uint32_t EP2STS;            /*!< UDC2 EP2 Status                              */
  __I  uint32_t EP2DSZ;            /*!< UDC2 EP2 Data Size                           */
  __IO uint32_t EP2FIFO;           /*!< UDC2 EP2 FIFO                                */
  __IO uint32_t EP3MSZ;            /*!< UDC2 EP3 Max Packet Size                     */
  __IO uint32_t EP3STS;            /*!< UDC2 EP3 Status                              */
  __I  uint32_t EP3DSZ;            /*!< UDC2 EP3 Data Size                           */
  __IO uint32_t EP3FIFO;           /*!< UDC2 EP3 FIFO                                */
  __IO uint32_t EP4MSZ;            /*!< UDC2 EP4 Max Packet Size                     */
  __IO uint32_t EP4STS;            /*!< UDC2 EP4 Status                              */
  __I  uint32_t EP4DSZ;            /*!< UDC2 EP4 Data Size                           */
  __IO uint32_t EP4FIFO;           /*!< UDC2 EP4 FIFO                                */
  __IO uint32_t EP5MSZ;            /*!< UDC2 EP5 Max Packet Size                     */
  __IO uint32_t EP5STS;            /*!< UDC2 EP5 Status                              */
  __I  uint32_t EP5DSZ;            /*!< UDC2 EP5 Data Size                           */
  __IO uint32_t EP5FIFO;           /*!< UDC2 EP5 FIFO                                */
  __IO uint32_t EP6MSZ;            /*!< UDC2 EP6 Max Packet Size                     */
  __IO uint32_t EP6STS;            /*!< UDC2 EP6 Status                              */
  __I  uint32_t EP6DSZ;            /*!< UDC2 EP6 Data Size                           */
  __IO uint32_t EP6FIFO;           /*!< UDC2 EP6 FIFO                                */
  __IO uint32_t EP7MSZ;            /*!< UDC2 EP7 Max Packet Size                     */
  __IO uint32_t EP7STS;            /*!< UDC2 EP7 Status                              */
  __I  uint32_t EP7DSZ;            /*!< UDC2 EP7 Data Size                           */
  __IO uint32_t EP7FIFO;           /*!< UDC2 EP7 FIFO                                */
       uint32_t RESERVED1[32];
  __IO uint32_t INTNAK;            /*!< UDC2 INT NAK                                 */
  __IO uint32_t INTNAKMSK;         /*!< UDC2 INT NAK MASK                            */
} TSB_UDFS2_TypeDef;

/**
  * @brief Synchronous Serial Port (SSP)
  */
typedef struct
{
  __IO uint32_t CR0;               /*!< SSP Control Register 0                       */
  __IO uint32_t CR1;               /*!< SSP Control Register 1                       */
  __IO uint32_t DR;                /*!< SSP Data Register                            */
  __I  uint32_t SR;                /*!< SSP Status Register                          */
  __IO uint32_t CPSR;              /*!< SSP Clock Prescaler Register                 */
  __IO uint32_t IMSC;              /*!< SSP Interrupt Mask Set and Clear Register    */
  __I  uint32_t RIS;               /*!< SSP Raw Interrupt Status Register            */
  __I  uint32_t MIS;               /*!< SSP Masked Interrupt Status Register         */
  __O  uint32_t ICR;               /*!< SSP Interrupt Clear Register                 */
  __IO uint32_t DMACR;             /*!< SSP DMA Control Register                     */
} TSB_SSP_TypeDef;

#if defined ( __CC_ARM   )           /* RealView Compiler */
#pragma anon_unions
#elif (defined (__ICCARM__))         /*  ICC Compiler     */
#pragma language=extended
#endif

/**
  * @brief ARM Prime Cell PL011 (UART)
  */
typedef struct
{
  __IO uint32_t DR;                /*!< Data Register                                */
union {
  __I  uint32_t RSR;               /*!< Receive Status Register                      */
  __O  uint32_t ECR;               /*!< Error Clear Register                         */
  };
       uint32_t RESERVED0[4];
  __IO uint32_t FR;                /*!< Flag Register                                */
       uint32_t RESERVED1;
  __IO uint32_t ILPR;              /*!< IrDA Low-power Counter register              */
  __IO uint32_t IBDR;              /*!< Integer Baud Rate Register                   */
  __IO uint32_t FBDR;              /*!< Fractional Baud Rate Register                */
  __IO uint32_t LCR_H;             /*!< Line Control Register                        */
  __IO uint32_t CR;                /*!< Cntrol Register                              */
  __IO uint32_t IFLS;              /*!< Interrupt FIFO Level Select Register         */
  __IO uint32_t IMSC;              /*!< Interrupt Mask set/Clear Register            */
  __I  uint32_t RIS;               /*!< Raw Interrupt Status Register                */
  __I  uint32_t MIS;               /*!< Masked Interrupt Status Register             */
  __O  uint32_t ICR;               /*!< Interrupt Clear Register                     */
  __IO uint32_t DMACR;             /*!< DMA Control Register                         */
} TSB_UART_TypeDef;

/**
  * @brief Analog-to-Digital Converter (AD)
  */
typedef struct
{
  __IO uint32_t CLK;               /*!< AD Conversion Clock Setting Register         */
  __O  uint32_t MOD0;              /*!< AD Mode Control Register 0                   */
  __IO uint32_t MOD1;              /*!< AD Mode Control Register 1                   */
  __IO uint32_t MOD2;              /*!< AD Mode Control Register 2                   */
  __IO uint32_t MOD3;              /*!< AD Mode Control Register 3                   */
  __IO uint32_t MOD4;              /*!< AD Mode Control Register 4                   */
  __I  uint32_t MOD5;              /*!< AD Mode Control Register 5                   */
  __O  uint32_t MOD6;              /*!< AD Mode Control Register 6                   */
  __IO uint32_t MOD7;              /*!< AD Mode Control Register 7                   */
  __IO uint32_t CMPCR0;            /*!< AD Monitoring Setting Register 0             */
  __IO uint32_t CMPCR1;            /*!< AD Monitoring Setting Register 1             */
  __IO uint32_t CMP0;              /*!< AD Conversion Result Comparison Register 0   */
  __IO uint32_t CMP1;              /*!< AD Conversion Result Comparison Register 1   */
  __I  uint32_t REG00;             /*!< AD Conversion Result Register 00             */
  __I  uint32_t REG01;             /*!< AD Conversion Result Register 01             */
  __I  uint32_t REG02;             /*!< AD Conversion Result Register 02             */
  __I  uint32_t REG03;             /*!< AD Conversion Result Register 03             */
  __I  uint32_t REG04;             /*!< AD Conversion Result Register 04             */
  __I  uint32_t REG05;             /*!< AD Conversion Result Register 05             */
  __I  uint32_t REG06;             /*!< AD Conversion Result Register 06             */
  __I  uint32_t REG07;             /*!< AD Conversion Result Register 07             */
  __I  uint32_t REG08;             /*!< AD Conversion Result Register 08             */
  __I  uint32_t REG09;             /*!< AD Conversion Result Register 09             */
  __I  uint32_t REG10;             /*!< AD Conversion Result Register 10             */
  __I  uint32_t REG11;             /*!< AD Conversion Result Register 11             */
       uint32_t RESERVED0[4];
  __I  uint32_t REGSP;             /*!< AD Conversion Result Register SP             */
} TSB_AD_TypeDef;

/**
  * @brief External Bus Interface(EXB)
  */
typedef struct
{
  __IO uint32_t MOD;               /*!< External Bus Mode Register                   */
       uint32_t RESERVED0[3];
  __IO uint32_t AS0;               /*!< External Bus Base Address and CS Space setting Register 0*/
  __IO uint32_t AS1;               /*!< External Bus Base Address and CS Space setting Register 1 */
       uint32_t RESERVED1[10];
  __IO uint32_t CS0;               /*!< Chip Select and Wait Controller Register 0   */
  __IO uint32_t CS1;               /*!< Chip Select and Wait Controller Register 1   */
} TSB_EXB_TypeDef;

/**
  * @brief General Purpose Input/Output Port (PA)
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PA Data Register                             */
  __IO uint32_t CR;                /*!< PA Control Register                          */
  __IO uint32_t FR1;               /*!< PA Function Register 1                       */
       uint32_t RESERVED0[7];
  __IO uint32_t OD;                /*!< PA Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PA Pull-Up Control Register                  */
       uint32_t RESERVED1[2];
  __IO uint32_t IE;                /*!< PA Input Enable Control Register             */
} TSB_PA_TypeDef;

/**
  * @brief General Purpose Input/Output Port (PB)
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PB Data Register                             */
  __IO uint32_t CR;                /*!< PB Control Register                          */
  __IO uint32_t FR1;               /*!< PB Function Register 1                       */
  __IO uint32_t FR2;               /*!< PB Function Register 2                       */
  __IO uint32_t FR3;               /*!< PB Function Register 3                       */
       uint32_t RESERVED0[5];
  __IO uint32_t OD;                /*!< PB Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PB Pull-Up Control Register                  */
       uint32_t RESERVED1[2];
  __IO uint32_t IE;                /*!< PB Input Enable Control Register             */
} TSB_PB_TypeDef;

/**
  * @brief General Purpose Input/Output Port (PC)
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PC Data Register                             */
  __IO uint32_t CR;                /*!< PC Control Register                          */
  __IO uint32_t FR1;               /*!< PC Function Register 1                       */
  __IO uint32_t FR2;               /*!< PC Function Register 2                       */
  __IO uint32_t FR3;               /*!< PC Function Register 3                       */
  __IO uint32_t FR4;               /*!< PC Function Register 4                       */
       uint32_t RESERVED0[4];
  __IO uint32_t OD;                /*!< PC Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PC Pull-Up Control Register                  */
       uint32_t RESERVED1[2];
  __IO uint32_t IE;                /*!< PC Input Enable Control Register             */
} TSB_PC_TypeDef;

/**
  * @brief General Purpose Input/Output Port (PD)
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PD Data Register                             */
  __IO uint32_t CR;                /*!< PD Control Register                          */
       uint32_t RESERVED0;
  __IO uint32_t FR2;               /*!< PD Function Register 2                       */
  __IO uint32_t FR3;               /*!< PD Function Register 3                       */
       uint32_t RESERVED1[5];
  __IO uint32_t OD;                /*!< PD Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PD Pull-Up Control Register                  */
       uint32_t RESERVED2[2];
  __IO uint32_t IE;                /*!< PD Input Enable Control Register             */
} TSB_PD_TypeDef;

/**
  * @brief General Purpose Input/Output Port (PE)
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PE Data Register                             */
  __IO uint32_t CR;                /*!< PE Control Register                          */
  __IO uint32_t FR1;               /*!< PE Function Register 1                       */
  __IO uint32_t FR2;               /*!< PE Function Register 2                       */
  __IO uint32_t FR3;               /*!< PE Function Register 3                       */
  __IO uint32_t FR4;               /*!< PE Function Register 4                       */
  __IO uint32_t FR5;               /*!< PE Function Register 5                       */
       uint32_t RESERVED0[3];
  __IO uint32_t OD;                /*!< PE Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PE Pull-Up Control Register                  */
       uint32_t RESERVED1[2];
  __IO uint32_t IE;                /*!< PE Input Enable Control Register             */
} TSB_PE_TypeDef;

/**
  * @brief General Purpose Input/Output Port (PF)
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PF Data Register                             */
  __IO uint32_t CR;                /*!< PF Control Register                          */
  __IO uint32_t FR1;               /*!< PF Function Register 1                       */
  __IO uint32_t FR2;               /*!< PF Function Register 2                       */
  __IO uint32_t FR3;               /*!< PF Function Register 3                       */
       uint32_t RESERVED0[5];
  __IO uint32_t OD;                /*!< PF Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PF Pull-Up Control Register                  */
       uint32_t RESERVED1[2];
  __IO uint32_t IE;                /*!< PF Input Enable Control Register             */
} TSB_PF_TypeDef;

/**
  * @brief General Purpose Input/Output Port (PG)
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PG Data Register                             */
  __IO uint32_t CR;                /*!< PG Control Register                          */
  __IO uint32_t FR1;               /*!< PG Function Register 2                       */
  __IO uint32_t FR2;               /*!< PG Function Register 2                       */
  __IO uint32_t FR3;               /*!< PG Function Register 3                       */
  __IO uint32_t FR4;               /*!< PG Function Register 4                       */
  __IO uint32_t FR5;               /*!< PG Function Register 5                       */
       uint32_t RESERVED0[3];
  __IO uint32_t OD;                /*!< PG Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PG Pull-Up Control Register                  */
       uint32_t RESERVED1[2];
  __IO uint32_t IE;                /*!< PG Input Enable Control Register             */
} TSB_PG_TypeDef;

/**
  * @brief General Purpose Input/Output Port (PH)
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PH Data Register                             */
  __IO uint32_t CR;                /*!< PH Control Register                          */
  __IO uint32_t FR1;               /*!< PH Function Register 1                       */
  __IO uint32_t FR2;               /*!< PH Function Register 2                       */
  __IO uint32_t FR3;               /*!< PH Function Register 3                       */
  __IO uint32_t FR4;               /*!< PH Function Register 4                       */
       uint32_t RESERVED0[4];
  __IO uint32_t OD;                /*!< PH Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PH Pull-Up Control Register                  */
       uint32_t RESERVED1[2];
  __IO uint32_t IE;                /*!< PH Input Enable Control Register             */
} TSB_PH_TypeDef;

/**
  * @brief General Purpose Input/Output Port (PI)
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PI Data Register                             */
  __IO uint32_t CR;                /*!< PI Control Register                          */
  __IO uint32_t FR1;               /*!< PI Function Register 1                       */
       uint32_t RESERVED0[7];
  __IO uint32_t OD;                /*!< PI Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PI Pull-Up Control Register                  */
  __IO uint32_t PDN;               /*!< PI Pull-Down Control Register                */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< PI Input Enable Control Register             */
} TSB_PI_TypeDef;

/**
  * @brief General Purpose Input/Output Port (PJ)
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PJ Data Register                             */
  __IO uint32_t CR;                /*!< PJ Control Register                          */
       uint32_t RESERVED0;
  __IO uint32_t FR2;               /*!< PJ Function Register 2                       */
  __IO uint32_t FR3;               /*!< PJ Function Register 3                       */
       uint32_t RESERVED1[6];
  __IO uint32_t PUP;               /*!< PJ Pull-Up Control Register                  */
       uint32_t RESERVED2[2];
  __IO uint32_t IE;                /*!< PJ Input Enable Control Register             */
} TSB_PJ_TypeDef;

/**
  * @brief General Purpose Input/Output Port (PK)
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PK Data Register                             */
  __IO uint32_t CR;                /*!< PK Control Register                          */
       uint32_t RESERVED0;
  __IO uint32_t FR2;               /*!< PK Function Register 2                       */
  __IO uint32_t FR3;               /*!< PK Function Register 3                       */
       uint32_t RESERVED1[6];
  __IO uint32_t PUP;               /*!< PK Pull-Up Control Register                  */
       uint32_t RESERVED2[2];
  __IO uint32_t IE;                /*!< PK Input Enable Control Register             */
} TSB_PK_TypeDef;

/**
  * @brief 16-bit Timer/Event Counter (TB)
  */
typedef struct
{
  __IO uint32_t EN;                /*!< TB Enable Register                           */
  __IO uint32_t RUN;               /*!< TB RUN Register                              */
  __IO uint32_t CR;                /*!< TB Control Register                          */
  __IO uint32_t MOD;               /*!< TB Mode Register                             */
  __IO uint32_t FFCR;              /*!< TB Flip-Flop Control Register                */
  __I  uint32_t ST;                /*!< TB Status Register                           */
  __IO uint32_t IM;                /*!< TB Interrupt Mask Register                   */
  __I  uint32_t UC;                /*!< TB Read Capture Register                     */
  __IO uint32_t RG0;               /*!< TB RG0 Timer Register                        */
  __IO uint32_t RG1;               /*!< TB RG1 Timer Register                        */
  __I  uint32_t CP0;               /*!< TB CP0 Capture Register                      */
  __I  uint32_t CP1;               /*!< TB CP1 Capture Register                      */
  __IO uint32_t DMA;               /*!< TB DMA Enable Register                       */
} TSB_TB_TypeDef;

/**
  * @brief Serial Bus Interface (SBI)
  */
typedef struct
{
  __IO uint32_t CR0;               /*!< SBI Control Register 0                       */
  __IO uint32_t CR1;               /*!< SBI Control Register 1 (I2C Mode)            */
  __IO uint32_t DBR;               /*!< SBI Data Buffer Register                     */
  __IO uint32_t I2CAR;             /*!< SBI I2C Bus Address Register                 */
union {
  __O  uint32_t CR2;               /*!< SBI Control Register 2 (I2C Mode)            */
  __I  uint32_t SR;                /*!< SBI Status Register (I2C Mode)               */
  };
  __IO uint32_t BR0;               /*!< SBI Baud Rate Register 0                     */
} TSB_SBI_TypeDef;

/**
  * @brief Serial Channel (SC)
  */
typedef struct
{
  __IO uint32_t EN;                /*!< SC Enable Register                           */
  __IO uint32_t BUF;               /*!< SC Buffer Register                           */
  __IO uint32_t CR;                /*!< SC Control Register                          */
  __IO uint32_t MOD0;              /*!< SC Mode Control Register 0                   */
  __IO uint32_t BRCR;              /*!< SC Baud Rate Generator Control Register      */
  __IO uint32_t BRADD;             /*!< SC Baud Rate Generator Control Register 2    */
  __IO uint32_t MOD1;              /*!< SC Mode Control Register 1                   */
  __IO uint32_t MOD2;              /*!< SC Mode Control Register 2                   */
  __IO uint32_t RFC;               /*!< SC RX FIFO Configuration Register            */
  __IO uint32_t TFC;               /*!< SC TX FIFO Configuration Register            */
  __I  uint32_t RST;               /*!< SC RX FIFO Status Register                   */
  __I  uint32_t TST;               /*!< SC TX FIFO Status Register                   */
  __IO uint32_t FCNF;              /*!< SC FIFO Configuration Register               */
  __IO uint32_t DMA;               /*!< SC DMA Enable Register                       */
} TSB_SC_TypeDef;

/**
  * @brief Watchdog Timer (WD)
  */
typedef struct
{
  __IO uint32_t MOD;               /*!< WD Mode Register                             */
  __O  uint32_t CR;                /*!< WD Control Register                          */
} TSB_WD_TypeDef;

/**
  * @brief Clock Generator (CG)
  */
typedef struct
{
  __IO uint32_t SYSCR;             /*!< System Control Register                      */
  __IO uint32_t OSCCR;             /*!< Oscillation Control Register                 */
  __IO uint32_t STBYCR;            /*!< Standby Control Register                     */
  __IO uint32_t PLLSEL;            /*!< PLL Selection Register                       */
  __IO uint32_t CKSEL;             /*!< System Clock Selection Register              */
       uint32_t RESERVED0[9];
  __IO uint32_t USBCTL;            /*!< USB Clock and Reset Control register         */
  __IO uint32_t PROTECT;           /*!< Protect Register                             */
  __IO uint32_t IMCGA;             /*!< CG Interrupt Mode Control Register A         */
  __IO uint32_t IMCGB;             /*!< CG Interrupt Mode Control Register B         */
  __IO uint32_t IMCGC;             /*!< CG Interrupt Mode Control Register C         */
       uint32_t RESERVED1[5];
  __O  uint32_t ICRCG;             /*!< CG Interrupt Request Clear Register          */
  __IO uint32_t RSTFLG;            /*!< Reset Flag Register                          */
  __I  uint32_t NMIFLG;            /*!< NMI Flag Register                            */
} TSB_CG_TypeDef;

/**
  * @brief Flash Control (FC)
  */
typedef struct
{
       uint32_t RESERVED0[4];
  __IO uint32_t SECBIT;            /*!< FC Security Bit Register                     */
       uint32_t RESERVED1[3];
  __I  uint32_t FLCS;              /*!< FC Flash Control Register                    */
} TSB_FC_TypeDef;


/* Memory map */
#define FLASH_BASE            (0x00000000UL)
#define RAM_BASE              (0x20000000UL)
#define PERI_BASE             (0x40000000UL)


#define TSB_DMACA_BASE             (PERI_BASE  + 0x0000000UL)
#define TSB_DMACB_BASE             (PERI_BASE  + 0x0001000UL)
#define TSB_UDFS_BASE              (PERI_BASE  + 0x0008000UL)
#define TSB_UDFS2_BASE             (PERI_BASE  + 0x0008200UL)
#define TSB_SSP0_BASE              (PERI_BASE  + 0x0040000UL)
#define TSB_SSP1_BASE              (PERI_BASE  + 0x0041000UL)
#define TSB_SSP2_BASE              (PERI_BASE  + 0x0042000UL)
#define TSB_UART_BASE              (PERI_BASE  + 0x0048000UL)
#define TSB_AD_BASE                (PERI_BASE  + 0x0050000UL)
#define TSB_EXB_BASE               (PERI_BASE  + 0x005C000UL)
#define TSB_PA_BASE                (PERI_BASE  + 0x00C0000UL)
#define TSB_PB_BASE                (PERI_BASE  + 0x00C0100UL)
#define TSB_PC_BASE                (PERI_BASE  + 0x00C0200UL)
#define TSB_PD_BASE                (PERI_BASE  + 0x00C0300UL)
#define TSB_PE_BASE                (PERI_BASE  + 0x00C0400UL)
#define TSB_PF_BASE                (PERI_BASE  + 0x00C0500UL)
#define TSB_PG_BASE                (PERI_BASE  + 0x00C0600UL)
#define TSB_PH_BASE                (PERI_BASE  + 0x00C0700UL)
#define TSB_PI_BASE                (PERI_BASE  + 0x00C0800UL)
#define TSB_PJ_BASE                (PERI_BASE  + 0x00C0900UL)
#define TSB_PK_BASE                (PERI_BASE  + 0x00C0A00UL)
#define TSB_TB0_BASE               (PERI_BASE  + 0x00C4000UL)
#define TSB_TB1_BASE               (PERI_BASE  + 0x00C4100UL)
#define TSB_TB2_BASE               (PERI_BASE  + 0x00C4200UL)
#define TSB_TB3_BASE               (PERI_BASE  + 0x00C4300UL)
#define TSB_TB4_BASE               (PERI_BASE  + 0x00C4400UL)
#define TSB_TB5_BASE               (PERI_BASE  + 0x00C4500UL)
#define TSB_TB6_BASE               (PERI_BASE  + 0x00C4600UL)
#define TSB_TB7_BASE               (PERI_BASE  + 0x00C4700UL)
#define TSB_TB8_BASE               (PERI_BASE  + 0x00C4800UL)
#define TSB_TB9_BASE               (PERI_BASE  + 0x00C4900UL)
#define TSB_SBI0_BASE              (PERI_BASE  + 0x00E0000UL)
#define TSB_SBI1_BASE              (PERI_BASE  + 0x00E0100UL)
#define TSB_SC0_BASE               (PERI_BASE  + 0x00E1000UL)
#define TSB_SC1_BASE               (PERI_BASE  + 0x00E1100UL)
#define TSB_WD_BASE                (PERI_BASE  + 0x00F2000UL)
#define TSB_CG_BASE                (PERI_BASE  + 0x00F3000UL)
#define TSB_FC_BASE                (PERI_BASE  + 0x1FFF000UL)


/* Peripheral declaration */
#define TSB_DMACA                  ((   TSB_DMAC_TypeDef *) TSB_DMACA_BASE)
#define TSB_DMACB                  ((   TSB_DMAC_TypeDef *) TSB_DMACB_BASE)
#define TSB_UDFS                   ((   TSB_UDFS_TypeDef *)  TSB_UDFS_BASE)
#define TSB_UDFS2                  ((  TSB_UDFS2_TypeDef *) TSB_UDFS2_BASE)
#define TSB_SSP0                   ((    TSB_SSP_TypeDef *)  TSB_SSP0_BASE)
#define TSB_SSP1                   ((    TSB_SSP_TypeDef *)  TSB_SSP1_BASE)
#define TSB_SSP2                   ((    TSB_SSP_TypeDef *)  TSB_SSP2_BASE)
#define TSB_UART                   ((   TSB_UART_TypeDef *)  TSB_UART_BASE)
#define TSB_AD                     ((     TSB_AD_TypeDef *)    TSB_AD_BASE)
#define TSB_EXB                    ((    TSB_EXB_TypeDef *)   TSB_EXB_BASE)
#define TSB_PA                     ((     TSB_PA_TypeDef *)    TSB_PA_BASE)
#define TSB_PB                     ((     TSB_PB_TypeDef *)    TSB_PB_BASE)
#define TSB_PC                     ((     TSB_PC_TypeDef *)    TSB_PC_BASE)
#define TSB_PD                     ((     TSB_PD_TypeDef *)    TSB_PD_BASE)
#define TSB_PE                     ((     TSB_PE_TypeDef *)    TSB_PE_BASE)
#define TSB_PF                     ((     TSB_PF_TypeDef *)    TSB_PF_BASE)
#define TSB_PG                     ((     TSB_PG_TypeDef *)    TSB_PG_BASE)
#define TSB_PH                     ((     TSB_PH_TypeDef *)    TSB_PH_BASE)
#define TSB_PI                     ((     TSB_PI_TypeDef *)    TSB_PI_BASE)
#define TSB_PJ                     ((     TSB_PJ_TypeDef *)    TSB_PJ_BASE)
#define TSB_PK                     ((     TSB_PK_TypeDef *)    TSB_PK_BASE)
#define TSB_TB0                    ((     TSB_TB_TypeDef *)   TSB_TB0_BASE)
#define TSB_TB1                    ((     TSB_TB_TypeDef *)   TSB_TB1_BASE)
#define TSB_TB2                    ((     TSB_TB_TypeDef *)   TSB_TB2_BASE)
#define TSB_TB3                    ((     TSB_TB_TypeDef *)   TSB_TB3_BASE)
#define TSB_TB4                    ((     TSB_TB_TypeDef *)   TSB_TB4_BASE)
#define TSB_TB5                    ((     TSB_TB_TypeDef *)   TSB_TB5_BASE)
#define TSB_TB6                    ((     TSB_TB_TypeDef *)   TSB_TB6_BASE)
#define TSB_TB7                    ((     TSB_TB_TypeDef *)   TSB_TB7_BASE)
#define TSB_TB8                    ((     TSB_TB_TypeDef *)   TSB_TB8_BASE)
#define TSB_TB9                    ((     TSB_TB_TypeDef *)   TSB_TB9_BASE)
#define TSB_SBI0                   ((    TSB_SBI_TypeDef *)  TSB_SBI0_BASE)
#define TSB_SBI1                   ((    TSB_SBI_TypeDef *)  TSB_SBI1_BASE)
#define TSB_SC0                    ((     TSB_SC_TypeDef *)   TSB_SC0_BASE)
#define TSB_SC1                    ((     TSB_SC_TypeDef *)   TSB_SC1_BASE)
#define TSB_WD                     ((     TSB_WD_TypeDef *)    TSB_WD_BASE)
#define TSB_CG                     ((     TSB_CG_TypeDef *)    TSB_CG_BASE)
#define TSB_FC                     ((     TSB_FC_TypeDef *)    TSB_FC_BASE)


/* Bit-Band for Device Specific Peripheral Registers */
#define BITBAND_OFFSET (0x02000000UL)
#define BITBAND_PERI_BASE (PERI_BASE + BITBAND_OFFSET)
#define BITBAND_PERI(addr, bitnum) (BITBAND_PERI_BASE + (((uint32_t)(addr) - PERI_BASE) << 5) + ((uint32_t)(bitnum) << 2))



/* DMA Controller */
#define TSB_DMACA_INTSTATUS_INTSTATUS0            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->INTSTATUS,0)))
#define TSB_DMACA_INTSTATUS_INTSTATUS1            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->INTSTATUS,1)))
#define TSB_DMACA_INTTCSTATUS_INTTCSTATUS0        (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->INTTCSTATUS,0)))
#define TSB_DMACA_INTTCSTATUS_INTTCSTATUS1        (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->INTTCSTATUS,1)))
#define TSB_DMACA_INTERRORSTATUS_INTERRSTATUS0    (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->INTERRORSTATUS,0)))
#define TSB_DMACA_INTERRORSTATUS_INTERRSTATUS1    (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->INTERRORSTATUS,1)))
#define TSB_DMACA_RAWINTTCSTATUS_RAWINTTCS0       (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->RAWINTTCSTATUS,0)))
#define TSB_DMACA_RAWINTTCSTATUS_RAWINTTCS1       (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->RAWINTTCSTATUS,1)))
#define TSB_DMACA_RAWINTERRORSTATUS_RAWINTERRS0   (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->RAWINTERRORSTATUS,0)))
#define TSB_DMACA_RAWINTERRORSTATUS_RAWINTERRS1   (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->RAWINTERRORSTATUS,1)))
#define TSB_DMACA_ENBLDCHNS_ENABLEDCH0            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->ENBLDCHNS,0)))
#define TSB_DMACA_ENBLDCHNS_ENABLEDCH1            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->ENBLDCHNS,1)))
#define TSB_DMACA_C0CONFIGURATION_ACTIVE          (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->C0CONFIGURATION,17)))
#define TSB_DMACA_C1CONFIGURATION_ACTIVE          (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->C1CONFIGURATION,17)))

#define TSB_DMACB_INTSTATUS_INTSTATUS0            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->INTSTATUS,0)))
#define TSB_DMACB_INTSTATUS_INTSTATUS1            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->INTSTATUS,1)))
#define TSB_DMACB_INTTCSTATUS_INTTCSTATUS0        (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->INTTCSTATUS,0)))
#define TSB_DMACB_INTTCSTATUS_INTTCSTATUS1        (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->INTTCSTATUS,1)))
#define TSB_DMACB_INTERRORSTATUS_INTERRSTATUS0    (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->INTERRORSTATUS,0)))
#define TSB_DMACB_INTERRORSTATUS_INTERRSTATUS1    (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->INTERRORSTATUS,1)))
#define TSB_DMACB_RAWINTTCSTATUS_RAWINTTCS0       (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->RAWINTTCSTATUS,0)))
#define TSB_DMACB_RAWINTTCSTATUS_RAWINTTCS1       (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->RAWINTTCSTATUS,1)))
#define TSB_DMACB_RAWINTERRORSTATUS_RAWINTERRS0   (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->RAWINTERRORSTATUS,0)))
#define TSB_DMACB_RAWINTERRORSTATUS_RAWINTERRS1   (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->RAWINTERRORSTATUS,1)))
#define TSB_DMACB_ENBLDCHNS_ENABLEDCH0            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->ENBLDCHNS,0)))
#define TSB_DMACB_ENBLDCHNS_ENABLEDCH1            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->ENBLDCHNS,1)))
#define TSB_DMACB_C0CONFIGURATION_ACTIVE          (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->C0CONFIGURATION,17)))
#define TSB_DMACB_C1CONFIGURATION_ACTIVE          (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->C1CONFIGURATION,17)))


/* UDC2 AHB Bridge */
#define TSB_UDFS_INTSTS_INT_SETUP                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,0)))
#define TSB_UDFS_INTSTS_INT_STATUS_NAK            (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,1)))
#define TSB_UDFS_INTSTS_INT_STATUS                (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,2)))
#define TSB_UDFS_INTSTS_INT_RX_ZERO               (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,3)))
#define TSB_UDFS_INTSTS_INT_SOF                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,4)))
#define TSB_UDFS_INTSTS_INT_EP0                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,5)))
#define TSB_UDFS_INTSTS_INT_EP                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,6)))
#define TSB_UDFS_INTSTS_INT_NAK                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,7)))
#define TSB_UDFS_INTSTS_INT_SUSPEND_RESUME        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,8)))
#define TSB_UDFS_INTSTS_INT_USB_RESET             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,9)))
#define TSB_UDFS_INTSTS_INT_USB_RESET_END         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,10)))
#define TSB_UDFS_INTSTS_INT_MW_SET_ADD            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,17)))
#define TSB_UDFS_INTSTS_INT_MW_END_ADD            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,18)))
#define TSB_UDFS_INTSTS_INT_MW_TIMEOUT            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,19)))
#define TSB_UDFS_INTSTS_INT_MW_AHBERR             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,20)))
#define TSB_UDFS_INTSTS_INT_MR_END_ADD            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,21)))
#define TSB_UDFS_INTSTS_INT_MR_EP_DSET            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,22)))
#define TSB_UDFS_INTSTS_INT_MR_AHBERR             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,23)))
#define TSB_UDFS_INTSTS_INT_UDC2_REGINT__RD       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,24)))
#define TSB_UDFS_INTSTS_INT_DMAC_REG_RD           (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,25)))
#define TSB_UDFS_INTSTS_INT_POWERDETECT           (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,28)))
#define TSB_UDFS_INTSTS_INT_MW_RERROR             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,29)))
#define TSB_UDFS_INTENB_SUSPEND_RESUME_EN         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,8)))
#define TSB_UDFS_INTENB_RESET_EN                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,9)))
#define TSB_UDFS_INTENB_RESET_END_EN              (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,10)))
#define TSB_UDFS_INTENB_MW_SET_ADD_EN             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,17)))
#define TSB_UDFS_INTENB_MW_END_ADD_EN             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,18)))
#define TSB_UDFS_INTENB_MW_TIMEOUT                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,19)))
#define TSB_UDFS_INTENB_MW_AHBERR                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,20)))
#define TSB_UDFS_INTENB_MR_END_ADD_EN             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,21)))
#define TSB_UDFS_INTENB_MR_EP_DSET_EN             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,22)))
#define TSB_UDFS_INTENB_MR_AHBERR                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,23)))
#define TSB_UDFS_INTENB_UDC2_REG_RD               (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,24)))
#define TSB_UDFS_INTENB_DMAC_REG_RD_EN            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,25)))
#define TSB_UDFS_INTENB_POWER_DETECT_EN           (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,28)))
#define TSB_UDFS_INTENB_MW_RERROR_EN              (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,29)))
#define TSB_UDFS_MWTOUT_TIMEOUT_EN                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->MWTOUT,0)))
#define TSB_UDFS_C2STSET_TX0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->C2STSET,0)))
#define TSB_UDFS_C2STSET_EOPB_ENABLE              (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->C2STSET,4)))
#define TSB_UDFS_DMACRDREQ_DMARDCLR               (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->DMACRDREQ,30)))
#define TSB_UDFS_DMACRDREQ_DMARDREQ               (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->DMACRDREQ,31)))
#define TSB_UDFS_UDC2RDREQ_UDC2RDCLR              (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->UDC2RDREQ,30)))
#define TSB_UDFS_UDC2RDREQ_UDC2RDREQ              (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->UDC2RDREQ,31)))
#define TSB_UDFS_ARBTSET_ABTMOD                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->ARBTSET,28)))
#define TSB_UDFS_ARBTSET_ABT_EN                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->ARBTSET,31)))
#define TSB_UDFS_PWCTL_USB_RESET                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,0)))
#define TSB_UDFS_PWCTL_PW_RESETB                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,1)))
#define TSB_UDFS_PWCTL_PW_DETECT                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,2)))
#define TSB_UDFS_PWCTL_PHY_SUSPEND                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,3)))
#define TSB_UDFS_PWCTL_SUSPEND_X                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,4)))
#define TSB_UDFS_PWCTL_PHY_RESETB                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,5)))
#define TSB_UDFS_PWCTL_PHY_REMOTE_WKUP            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,6)))
#define TSB_UDFS_PWCTL_WAKEUP_EN                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,7)))
#define TSB_UDFS_MSTSTS_MWEPDSET                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->MSTSTS,0)))
#define TSB_UDFS_MSTSTS_MREPDSET                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->MSTSTS,1)))
#define TSB_UDFS_MSTSTS_MWBFEMP                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->MSTSTS,2)))
#define TSB_UDFS_MSTSTS_MRBFEMP                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->MSTSTS,3)))
#define TSB_UDFS_MSTSTS_MREPEMPTY                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->MSTSTS,4)))


/* UDC2(USB -Spec2.0 Device contoller) */
#define TSB_UDFS2_ADR_SUSPEND                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->ADR,11)))
#define TSB_UDFS2_ADR_EP_BI_MODE                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->ADR,14)))
#define TSB_UDFS2_ADR_STAGE_ERR                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->ADR,15)))
#define TSB_UDFS2_FRM_CREATE_SOF                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->FRM,15)))
#define TSB_UDFS2_CMD_INT_TOGGLE                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->CMD,15)))
#define TSB_UDFS2_BRQ_DIR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->BRQ,7)))
#define TSB_UDFS2_INT_I_SETUP                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,0)))
#define TSB_UDFS2_INT_I_STATUS_NAK                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,1)))
#define TSB_UDFS2_INT_I_STATUS                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,2)))
#define TSB_UDFS2_INT_I_RX_DATA0                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,3)))
#define TSB_UDFS2_INT_I_SOF                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,4)))
#define TSB_UDFS2_INT_I_EP0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,5)))
#define TSB_UDFS2_INT_I_EP                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,6)))
#define TSB_UDFS2_INT_I_NAK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,7)))
#define TSB_UDFS2_INT_M_SETUP                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,8)))
#define TSB_UDFS2_INT_M_STATUS_NAK                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,9)))
#define TSB_UDFS2_INT_M_STATUS                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,10)))
#define TSB_UDFS2_INT_M_RX_DATA0                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,11)))
#define TSB_UDFS2_INT_M_SOF                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,12)))
#define TSB_UDFS2_INT_M_EP0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,13)))
#define TSB_UDFS2_INT_M_EP                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,14)))
#define TSB_UDFS2_INT_M_NAK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,15)))
#define TSB_UDFS2_INTEP_I_EP1                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEP,1)))
#define TSB_UDFS2_INTEP_I_EP2                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEP,2)))
#define TSB_UDFS2_INTEP_I_EP3                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEP,3)))
#define TSB_UDFS2_INTEP_I_EP4                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEP,4)))
#define TSB_UDFS2_INTEP_I_EP5                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEP,5)))
#define TSB_UDFS2_INTEP_I_EP6                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEP,6)))
#define TSB_UDFS2_INTEP_I_EP7                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEP,7)))
#define TSB_UDFS2_INTEPMSK_M_EP0                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEPMSK,0)))
#define TSB_UDFS2_INTEPMSK_M_EP1                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEPMSK,1)))
#define TSB_UDFS2_INTEPMSK_M_EP2                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEPMSK,2)))
#define TSB_UDFS2_INTEPMSK_M_EP3                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEPMSK,3)))
#define TSB_UDFS2_INTEPMSK_M_EP4                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEPMSK,4)))
#define TSB_UDFS2_INTEPMSK_M_EP5                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEPMSK,5)))
#define TSB_UDFS2_INTEPMSK_M_EP6                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEPMSK,6)))
#define TSB_UDFS2_INTEPMSK_M_EP7                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEPMSK,7)))
#define TSB_UDFS2_INTRX0_RX_D0_EP0                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTRX0,0)))
#define TSB_UDFS2_INTRX0_RX_D0_EP1                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTRX0,1)))
#define TSB_UDFS2_INTRX0_RX_D0_EP2                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTRX0,2)))
#define TSB_UDFS2_INTRX0_RX_D0_EP3                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTRX0,3)))
#define TSB_UDFS2_INTRX0_RX_D0_EP4                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTRX0,4)))
#define TSB_UDFS2_INTRX0_RX_D0_EP5                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTRX0,5)))
#define TSB_UDFS2_INTRX0_RX_D0_EP6                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTRX0,6)))
#define TSB_UDFS2_INTRX0_RX_D0_EP7                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTRX0,7)))
#define TSB_UDFS2_EP0MSZ_DSET                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP0MSZ,12)))
#define TSB_UDFS2_EP0MSZ_TX_0DATA                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP0MSZ,15)))
#define TSB_UDFS2_EP0STS_EP0_MASK                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP0STS,15)))
#define TSB_UDFS2_EP1MSZ_DSET                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP1MSZ,12)))
#define TSB_UDFS2_EP1MSZ_TX_0DATA                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP1MSZ,15)))
#define TSB_UDFS2_EP1STS_DIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP1STS,7)))
#define TSB_UDFS2_EP1STS_DISABLE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP1STS,8)))
#define TSB_UDFS2_EP1STS_BUS_SEL                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP1STS,14)))
#define TSB_UDFS2_EP1STS_PKT_MODE                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP1STS,15)))
#define TSB_UDFS2_EP2MSZ_DSET                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP2MSZ,12)))
#define TSB_UDFS2_EP2MSZ_TX_0DATA                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP2MSZ,15)))
#define TSB_UDFS2_EP2STS_DIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP2STS,7)))
#define TSB_UDFS2_EP2STS_DISABLE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP2STS,8)))
#define TSB_UDFS2_EP2STS_BUS_SEL                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP2STS,14)))
#define TSB_UDFS2_EP2STS_PKT_MODE                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP2STS,15)))
#define TSB_UDFS2_EP3MSZ_DSET                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP3MSZ,12)))
#define TSB_UDFS2_EP3MSZ_TX_0DATA                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP3MSZ,15)))
#define TSB_UDFS2_EP3STS_DIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP3STS,7)))
#define TSB_UDFS2_EP3STS_DISABLE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP3STS,8)))
#define TSB_UDFS2_EP3STS_BUS_SEL                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP3STS,14)))
#define TSB_UDFS2_EP3STS_PKT_MODE                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP3STS,15)))
#define TSB_UDFS2_EP4MSZ_DSET                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP4MSZ,12)))
#define TSB_UDFS2_EP4MSZ_TX_0DATA                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP4MSZ,15)))
#define TSB_UDFS2_EP4STS_DIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP4STS,7)))
#define TSB_UDFS2_EP4STS_DISABLE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP4STS,8)))
#define TSB_UDFS2_EP4STS_BUS_SEL                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP4STS,14)))
#define TSB_UDFS2_EP4STS_PKT_MODE                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP4STS,15)))
#define TSB_UDFS2_EP5MSZ_DSET                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP5MSZ,12)))
#define TSB_UDFS2_EP5MSZ_TX_0DATA                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP5MSZ,15)))
#define TSB_UDFS2_EP5STS_DIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP5STS,7)))
#define TSB_UDFS2_EP5STS_DISABLE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP5STS,8)))
#define TSB_UDFS2_EP5STS_BUS_SEL                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP5STS,14)))
#define TSB_UDFS2_EP5STS_PKT_MODE                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP5STS,15)))
#define TSB_UDFS2_EP6MSZ_DSET                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP6MSZ,12)))
#define TSB_UDFS2_EP6MSZ_TX_0DATA                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP6MSZ,15)))
#define TSB_UDFS2_EP6STS_DIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP6STS,7)))
#define TSB_UDFS2_EP6STS_DISABLE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP6STS,8)))
#define TSB_UDFS2_EP6STS_BUS_SEL                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP6STS,14)))
#define TSB_UDFS2_EP6STS_PKT_MODE                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP6STS,15)))
#define TSB_UDFS2_EP7MSZ_DSET                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP7MSZ,12)))
#define TSB_UDFS2_EP7MSZ_TX_0DATA                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP7MSZ,15)))
#define TSB_UDFS2_EP7STS_DIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP7STS,7)))
#define TSB_UDFS2_EP7STS_DISABLE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP7STS,8)))
#define TSB_UDFS2_EP7STS_BUS_SEL                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP7STS,14)))
#define TSB_UDFS2_EP7STS_PKT_MODE                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP7STS,15)))
#define TSB_UDFS2_INTNAK_I_EP1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAK,1)))
#define TSB_UDFS2_INTNAK_I_EP2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAK,2)))
#define TSB_UDFS2_INTNAK_I_EP3                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAK,3)))
#define TSB_UDFS2_INTNAK_I_EP4                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAK,4)))
#define TSB_UDFS2_INTNAK_I_EP5                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAK,5)))
#define TSB_UDFS2_INTNAK_I_EP6                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAK,6)))
#define TSB_UDFS2_INTNAK_I_EP7                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAK,7)))
#define TSB_UDFS2_INTNAKMSK_M_EP1                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAKMSK,1)))
#define TSB_UDFS2_INTNAKMSK_M_EP2                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAKMSK,2)))
#define TSB_UDFS2_INTNAKMSK_M_EP3                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAKMSK,3)))
#define TSB_UDFS2_INTNAKMSK_M_EP4                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAKMSK,4)))
#define TSB_UDFS2_INTNAKMSK_M_EP5                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAKMSK,5)))
#define TSB_UDFS2_INTNAKMSK_M_EP6                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAKMSK,6)))
#define TSB_UDFS2_INTNAKMSK_M_EP7                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAKMSK,7)))


/* Synchronous Serial Port */
#define TSB_SSP0_SR_TFE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->SR,0)))
#define TSB_SSP0_SR_TNF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->SR,1)))
#define TSB_SSP0_SR_RNE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->SR,2)))
#define TSB_SSP0_SR_RFF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->SR,3)))
#define TSB_SSP0_SR_BSY                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->SR,4)))
#define TSB_SSP0_RIS_RORRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->RIS,0)))
#define TSB_SSP0_RIS_RTRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->RIS,1)))
#define TSB_SSP0_RIS_RXRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->RIS,2)))
#define TSB_SSP0_RIS_TXRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->RIS,3)))
#define TSB_SSP0_MIS_RORMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->MIS,0)))
#define TSB_SSP0_MIS_RTMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->MIS,1)))
#define TSB_SSP0_MIS_RXMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->MIS,2)))
#define TSB_SSP0_MIS_TXMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->MIS,3)))

#define TSB_SSP1_SR_TFE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->SR,0)))
#define TSB_SSP1_SR_TNF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->SR,1)))
#define TSB_SSP1_SR_RNE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->SR,2)))
#define TSB_SSP1_SR_RFF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->SR,3)))
#define TSB_SSP1_SR_BSY                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->SR,4)))
#define TSB_SSP1_RIS_RORRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->RIS,0)))
#define TSB_SSP1_RIS_RTRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->RIS,1)))
#define TSB_SSP1_RIS_RXRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->RIS,2)))
#define TSB_SSP1_RIS_TXRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->RIS,3)))
#define TSB_SSP1_MIS_RORMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->MIS,0)))
#define TSB_SSP1_MIS_RTMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->MIS,1)))
#define TSB_SSP1_MIS_RXMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->MIS,2)))
#define TSB_SSP1_MIS_TXMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->MIS,3)))

#define TSB_SSP2_SR_TFE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->SR,0)))
#define TSB_SSP2_SR_TNF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->SR,1)))
#define TSB_SSP2_SR_RNE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->SR,2)))
#define TSB_SSP2_SR_RFF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->SR,3)))
#define TSB_SSP2_SR_BSY                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->SR,4)))
#define TSB_SSP2_RIS_RORRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->RIS,0)))
#define TSB_SSP2_RIS_RTRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->RIS,1)))
#define TSB_SSP2_RIS_RXRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->RIS,2)))
#define TSB_SSP2_RIS_TXRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->RIS,3)))
#define TSB_SSP2_MIS_RORMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->MIS,0)))
#define TSB_SSP2_MIS_RTMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->MIS,1)))
#define TSB_SSP2_MIS_RXMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->MIS,2)))
#define TSB_SSP2_MIS_TXMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->MIS,3)))


/* ARM Prime Cell PL011 */
#define TSB_UART_DR_FE                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->DR,8)))
#define TSB_UART_DR_PE                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->DR,9)))
#define TSB_UART_DR_BE                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->DR,10)))
#define TSB_UART_DR_OE                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->DR,11)))
#define TSB_UART_RSR_FE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RSR,0)))
#define TSB_UART_RSR_PE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RSR,1)))
#define TSB_UART_RSR_BE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RSR,2)))
#define TSB_UART_RSR_OE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RSR,3)))
#define TSB_UART_FR_CTS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->FR,0)))
#define TSB_UART_FR_DSR                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->FR,1)))
#define TSB_UART_FR_DCD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->FR,2)))
#define TSB_UART_FR_BUSY                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->FR,3)))
#define TSB_UART_FR_RXFE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->FR,4)))
#define TSB_UART_FR_TXFF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->FR,5)))
#define TSB_UART_FR_RXFF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->FR,6)))
#define TSB_UART_FR_TXFE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->FR,7)))
#define TSB_UART_FR_RI                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->FR,8)))
#define TSB_UART_LCR_H_BRK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->LCR_H,0)))
#define TSB_UART_LCR_H_PEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->LCR_H,1)))
#define TSB_UART_LCR_H_EPS                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->LCR_H,2)))
#define TSB_UART_LCR_H_STP2                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->LCR_H,3)))
#define TSB_UART_LCR_H_FEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->LCR_H,4)))
#define TSB_UART_LCR_H_SPS                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->LCR_H,7)))
#define TSB_UART_CR_UARTEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->CR,0)))
#define TSB_UART_CR_SIREN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->CR,1)))
#define TSB_UART_CR_SIRLP                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->CR,2)))
#define TSB_UART_CR_LBE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->CR,7)))
#define TSB_UART_CR_TXE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->CR,8)))
#define TSB_UART_CR_RXE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->CR,9)))
#define TSB_UART_CR_DTR                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->CR,10)))
#define TSB_UART_CR_RTS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->CR,11)))
#define TSB_UART_CR_OUT1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->CR,12)))
#define TSB_UART_CR_OUT2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->CR,13)))
#define TSB_UART_CR_RTSEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->CR,14)))
#define TSB_UART_CR_CTSEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->CR,15)))
#define TSB_UART_IMSC_RIMIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->IMSC,0)))
#define TSB_UART_IMSC_CTSMIM                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->IMSC,1)))
#define TSB_UART_IMSC_DCDMIM                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->IMSC,2)))
#define TSB_UART_IMSC_DSRMIM                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->IMSC,3)))
#define TSB_UART_IMSC_RXIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->IMSC,4)))
#define TSB_UART_IMSC_TXIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->IMSC,5)))
#define TSB_UART_IMSC_RTIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->IMSC,6)))
#define TSB_UART_IMSC_FEIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->IMSC,7)))
#define TSB_UART_IMSC_PEIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->IMSC,8)))
#define TSB_UART_IMSC_BEIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->IMSC,9)))
#define TSB_UART_IMSC_OEIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->IMSC,10)))
#define TSB_UART_RIS_RIRMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RIS,0)))
#define TSB_UART_RIS_CTSRMIS                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RIS,1)))
#define TSB_UART_RIS_DCDRMIS                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RIS,2)))
#define TSB_UART_RIS_DSRRMIS                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RIS,3)))
#define TSB_UART_RIS_RXRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RIS,4)))
#define TSB_UART_RIS_TXRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RIS,5)))
#define TSB_UART_RIS_RTRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RIS,6)))
#define TSB_UART_RIS_FERIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RIS,7)))
#define TSB_UART_RIS_PERIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RIS,8)))
#define TSB_UART_RIS_BERIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RIS,9)))
#define TSB_UART_RIS_OERIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->RIS,10)))
#define TSB_UART_MIS_RIMMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->MIS,0)))
#define TSB_UART_MIS_CTSMMIS                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->MIS,1)))
#define TSB_UART_MIS_DCDMMIS                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->MIS,2)))
#define TSB_UART_MIS_DSRMMIS                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->MIS,3)))
#define TSB_UART_MIS_RXMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->MIS,4)))
#define TSB_UART_MIS_TXMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->MIS,5)))
#define TSB_UART_MIS_RTMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->MIS,6)))
#define TSB_UART_MIS_FEMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->MIS,7)))
#define TSB_UART_MIS_PEMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->MIS,8)))
#define TSB_UART_MIS_BEMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->MIS,9)))
#define TSB_UART_MIS_OEMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART->MIS,10)))
#define TSB_UART_DMACR_RXDMAE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->DMACR,0)))
#define TSB_UART_DMACR_TXDMAE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->DMACR,1)))
#define TSB_UART_DMACR_DMAONERR                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART->DMACR,2)))


/* Analog-to-Digital Converter (AD) */
#define TSB_AD_MOD0_ADS                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_AD->MOD0,0)))
#define TSB_AD_MOD0_HPADS                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_AD->MOD0,1)))
#define TSB_AD_MOD1_ADHWE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,0)))
#define TSB_AD_MOD1_ADHWS                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,1)))
#define TSB_AD_MOD1_HPADHWE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,2)))
#define TSB_AD_MOD1_HPADHWS                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,3)))
#define TSB_AD_MOD1_RCUT                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,5)))
#define TSB_AD_MOD1_I2AD                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,6)))
#define TSB_AD_MOD1_VREFON                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,7)))
#define TSB_AD_MOD3_SCAN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD3,0)))
#define TSB_AD_MOD3_REPEAT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD3,1)))
#define TSB_AD_MOD7_INTADDMA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD7,0)))
#define TSB_AD_MOD7_INTADHPDMA                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD7,1)))
#define TSB_AD_CMPCR0_ADBIG0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->CMPCR0,4)))
#define TSB_AD_CMPCR0_CMPCOND0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->CMPCR0,5)))
#define TSB_AD_CMPCR0_CMP0EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->CMPCR0,7)))
#define TSB_AD_CMPCR1_ADBIG1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->CMPCR1,4)))
#define TSB_AD_CMPCR1_CMPCOND1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->CMPCR1,5)))
#define TSB_AD_CMPCR1_CMP1EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->CMPCR1,7)))


/* External Bus Interface(EXB) */
#define TSB_EXB_MOD_EXBSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->MOD,0)))
#define TSB_EXB_CS0_CSW0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS0,0)))
#define TSB_EXB_CS0_CSW1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS0,1)))
#define TSB_EXB_CS0_CSW2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS0,2)))
#define TSB_EXB_CS1_CSW0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS1,0)))
#define TSB_EXB_CS1_CSW1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS1,1)))
#define TSB_EXB_CS1_CSW2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS1,2)))


/* General Purpose Input/Output Port (PA) */
#define TSB_PA_DATA_PA0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,0)))
#define TSB_PA_DATA_PA1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,1)))
#define TSB_PA_DATA_PA2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,2)))
#define TSB_PA_DATA_PA3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,3)))
#define TSB_PA_DATA_PA4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,4)))
#define TSB_PA_DATA_PA5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,5)))
#define TSB_PA_DATA_PA6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,6)))
#define TSB_PA_DATA_PA7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,7)))
#define TSB_PA_CR_PA0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,0)))
#define TSB_PA_CR_PA1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,1)))
#define TSB_PA_CR_PA2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,2)))
#define TSB_PA_CR_PA3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,3)))
#define TSB_PA_CR_PA4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,4)))
#define TSB_PA_CR_PA5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,5)))
#define TSB_PA_CR_PA6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,6)))
#define TSB_PA_CR_PA7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,7)))
#define TSB_PA_FR1_PA0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,0)))
#define TSB_PA_FR1_PA1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,1)))
#define TSB_PA_FR1_PA2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,2)))
#define TSB_PA_FR1_PA3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,3)))
#define TSB_PA_FR1_PA4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,4)))
#define TSB_PA_FR1_PA5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,5)))
#define TSB_PA_FR1_PA6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,6)))
#define TSB_PA_FR1_PA7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,7)))
#define TSB_PA_OD_PA0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,0)))
#define TSB_PA_OD_PA1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,1)))
#define TSB_PA_OD_PA2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,2)))
#define TSB_PA_OD_PA3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,3)))
#define TSB_PA_OD_PA4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,4)))
#define TSB_PA_OD_PA5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,5)))
#define TSB_PA_OD_PA6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,6)))
#define TSB_PA_OD_PA7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,7)))
#define TSB_PA_PUP_PA0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,0)))
#define TSB_PA_PUP_PA1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,1)))
#define TSB_PA_PUP_PA2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,2)))
#define TSB_PA_PUP_PA3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,3)))
#define TSB_PA_PUP_PA4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,4)))
#define TSB_PA_PUP_PA5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,5)))
#define TSB_PA_PUP_PA6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,6)))
#define TSB_PA_PUP_PA7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,7)))
#define TSB_PA_IE_PA0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,0)))
#define TSB_PA_IE_PA1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,1)))
#define TSB_PA_IE_PA2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,2)))
#define TSB_PA_IE_PA3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,3)))
#define TSB_PA_IE_PA4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,4)))
#define TSB_PA_IE_PA5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,5)))
#define TSB_PA_IE_PA6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,6)))
#define TSB_PA_IE_PA7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,7)))


/* General Purpose Input/Output Port (PB) */
#define TSB_PB_DATA_PB0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,0)))
#define TSB_PB_DATA_PB1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,1)))
#define TSB_PB_DATA_PB2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,2)))
#define TSB_PB_DATA_PB3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,3)))
#define TSB_PB_DATA_PB4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,4)))
#define TSB_PB_DATA_PB5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,5)))
#define TSB_PB_DATA_PB6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,6)))
#define TSB_PB_DATA_PB7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,7)))
#define TSB_PB_CR_PB0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,0)))
#define TSB_PB_CR_PB1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,1)))
#define TSB_PB_CR_PB2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,2)))
#define TSB_PB_CR_PB3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,3)))
#define TSB_PB_CR_PB4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,4)))
#define TSB_PB_CR_PB5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,5)))
#define TSB_PB_CR_PB6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,6)))
#define TSB_PB_CR_PB7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,7)))
#define TSB_PB_FR1_PB0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,0)))
#define TSB_PB_FR1_PB1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,1)))
#define TSB_PB_FR1_PB2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,2)))
#define TSB_PB_FR1_PB3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,3)))
#define TSB_PB_FR1_PB4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,4)))
#define TSB_PB_FR1_PB5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,5)))
#define TSB_PB_FR1_PB6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,6)))
#define TSB_PB_FR1_PB7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,7)))
#define TSB_PB_FR2_PB0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,0)))
#define TSB_PB_FR2_PB1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,1)))
#define TSB_PB_FR2_PB2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,2)))
#define TSB_PB_FR2_PB3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,3)))
#define TSB_PB_FR2_PB4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,4)))
#define TSB_PB_FR2_PB5F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,5)))
#define TSB_PB_FR2_PB6F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,6)))
#define TSB_PB_FR2_PB7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,7)))
#define TSB_PB_FR3_PB0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,0)))
#define TSB_PB_FR3_PB1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,1)))
#define TSB_PB_FR3_PB2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,2)))
#define TSB_PB_FR3_PB3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,3)))
#define TSB_PB_FR3_PB4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,4)))
#define TSB_PB_FR3_PB5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,5)))
#define TSB_PB_FR3_PB6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,6)))
#define TSB_PB_FR3_PB7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,7)))
#define TSB_PB_OD_PB0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,0)))
#define TSB_PB_OD_PB1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,1)))
#define TSB_PB_OD_PB2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,2)))
#define TSB_PB_OD_PB3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,3)))
#define TSB_PB_OD_PB4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,4)))
#define TSB_PB_OD_PB5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,5)))
#define TSB_PB_OD_PB6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,6)))
#define TSB_PB_OD_PB7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,7)))
#define TSB_PB_PUP_PB0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,0)))
#define TSB_PB_PUP_PB1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,1)))
#define TSB_PB_PUP_PB2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,2)))
#define TSB_PB_PUP_PB3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,3)))
#define TSB_PB_PUP_PB4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,4)))
#define TSB_PB_PUP_PB5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,5)))
#define TSB_PB_PUP_PB6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,6)))
#define TSB_PB_PUP_PB7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,7)))
#define TSB_PB_IE_PB0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,0)))
#define TSB_PB_IE_PB1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,1)))
#define TSB_PB_IE_PB2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,2)))
#define TSB_PB_IE_PB3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,3)))
#define TSB_PB_IE_PB4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,4)))
#define TSB_PB_IE_PB5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,5)))
#define TSB_PB_IE_PB6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,6)))
#define TSB_PB_IE_PB7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,7)))


/* General Purpose Input/Output Port (PC) */
#define TSB_PC_DATA_PC0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,0)))
#define TSB_PC_DATA_PC1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,1)))
#define TSB_PC_DATA_PC2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,2)))
#define TSB_PC_CR_PC0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,0)))
#define TSB_PC_CR_PC1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,1)))
#define TSB_PC_CR_PC2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,2)))
#define TSB_PC_FR1_PC0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,0)))
#define TSB_PC_FR1_PC1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,1)))
#define TSB_PC_FR1_PC2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,2)))
#define TSB_PC_FR2_PC0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR2,0)))
#define TSB_PC_FR2_PC1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR2,1)))
#define TSB_PC_FR2_PC2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR2,2)))
#define TSB_PC_FR3_PC0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR3,0)))
#define TSB_PC_FR3_PC1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR3,1)))
#define TSB_PC_FR3_PC2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR3,2)))
#define TSB_PC_FR4_PC2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR4,2)))
#define TSB_PC_OD_PC0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,0)))
#define TSB_PC_OD_PC1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,1)))
#define TSB_PC_OD_PC2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,2)))
#define TSB_PC_PUP_PC0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,0)))
#define TSB_PC_PUP_PC1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,1)))
#define TSB_PC_PUP_PC2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,2)))
#define TSB_PC_IE_PC0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,0)))
#define TSB_PC_IE_PC1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,1)))
#define TSB_PC_IE_PC2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,2)))


/* General Purpose Input/Output Port (PD) */
#define TSB_PD_DATA_PD0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,0)))
#define TSB_PD_DATA_PD1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,1)))
#define TSB_PD_DATA_PD2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,2)))
#define TSB_PD_DATA_PD3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,3)))
#define TSB_PD_DATA_PD4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,4)))
#define TSB_PD_DATA_PD5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,5)))
#define TSB_PD_DATA_PD6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,6)))
#define TSB_PD_DATA_PD7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,7)))
#define TSB_PD_CR_PD0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,0)))
#define TSB_PD_CR_PD1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,1)))
#define TSB_PD_CR_PD2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,2)))
#define TSB_PD_CR_PD3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,3)))
#define TSB_PD_CR_PD4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,4)))
#define TSB_PD_CR_PD5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,5)))
#define TSB_PD_CR_PD6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,6)))
#define TSB_PD_CR_PD7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,7)))
#define TSB_PD_FR2_PD0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,0)))
#define TSB_PD_FR2_PD1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,1)))
#define TSB_PD_FR2_PD2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,2)))
#define TSB_PD_FR2_PD3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,3)))
#define TSB_PD_FR2_PD4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,4)))
#define TSB_PD_FR2_PD5F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,5)))
#define TSB_PD_FR2_PD6F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,6)))
#define TSB_PD_FR2_PD7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,7)))
#define TSB_PD_FR3_PD0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR3,0)))
#define TSB_PD_FR3_PD1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR3,1)))
#define TSB_PD_FR3_PD2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR3,2)))
#define TSB_PD_FR3_PD3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR3,3)))
#define TSB_PD_FR3_PD7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR3,7)))
#define TSB_PD_OD_PD0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,0)))
#define TSB_PD_OD_PD1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,1)))
#define TSB_PD_OD_PD2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,2)))
#define TSB_PD_OD_PD3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,3)))
#define TSB_PD_OD_PD4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,4)))
#define TSB_PD_OD_PD5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,5)))
#define TSB_PD_OD_PD6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,6)))
#define TSB_PD_OD_PD7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,7)))
#define TSB_PD_PUP_PD0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,0)))
#define TSB_PD_PUP_PD1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,1)))
#define TSB_PD_PUP_PD2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,2)))
#define TSB_PD_PUP_PD3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,3)))
#define TSB_PD_PUP_PD4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,4)))
#define TSB_PD_PUP_PD5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,5)))
#define TSB_PD_PUP_PD6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,6)))
#define TSB_PD_PUP_PD7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,7)))
#define TSB_PD_IE_PD0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,0)))
#define TSB_PD_IE_PD1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,1)))
#define TSB_PD_IE_PD2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,2)))
#define TSB_PD_IE_PD3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,3)))
#define TSB_PD_IE_PD4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,4)))
#define TSB_PD_IE_PD5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,5)))
#define TSB_PD_IE_PD6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,6)))
#define TSB_PD_IE_PD7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,7)))


/* General Purpose Input/Output Port (PE) */
#define TSB_PE_DATA_PE0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,0)))
#define TSB_PE_DATA_PE1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,1)))
#define TSB_PE_DATA_PE2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,2)))
#define TSB_PE_DATA_PE3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,3)))
#define TSB_PE_DATA_PE4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,4)))
#define TSB_PE_DATA_PE5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,5)))
#define TSB_PE_DATA_PE6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,6)))
#define TSB_PE_DATA_PE7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,7)))
#define TSB_PE_CR_PE0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,0)))
#define TSB_PE_CR_PE1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,1)))
#define TSB_PE_CR_PE2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,2)))
#define TSB_PE_CR_PE3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,3)))
#define TSB_PE_CR_PE4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,4)))
#define TSB_PE_CR_PE5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,5)))
#define TSB_PE_CR_PE6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,6)))
#define TSB_PE_CR_PE7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,7)))
#define TSB_PE_FR1_PE0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,0)))
#define TSB_PE_FR1_PE1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,1)))
#define TSB_PE_FR1_PE2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,2)))
#define TSB_PE_FR1_PE3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,3)))
#define TSB_PE_FR1_PE4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,4)))
#define TSB_PE_FR1_PE5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,5)))
#define TSB_PE_FR1_PE6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,6)))
#define TSB_PE_FR1_PE7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,7)))
#define TSB_PE_FR2_PE3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR2,3)))
#define TSB_PE_FR2_PE4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR2,4)))
#define TSB_PE_FR2_PE5F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR2,5)))
#define TSB_PE_FR2_PE6F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR2,6)))
#define TSB_PE_FR2_PE7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR2,7)))
#define TSB_PE_FR3_PE2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,2)))
#define TSB_PE_FR3_PE3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,3)))
#define TSB_PE_FR4_PE2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR4,2)))
#define TSB_PE_FR5_PE0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,0)))
#define TSB_PE_FR5_PE1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,1)))
#define TSB_PE_FR5_PE2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,2)))
#define TSB_PE_FR5_PE3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,3)))
#define TSB_PE_OD_PE0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,0)))
#define TSB_PE_OD_PE1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,1)))
#define TSB_PE_OD_PE2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,2)))
#define TSB_PE_OD_PE3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,3)))
#define TSB_PE_OD_PE4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,4)))
#define TSB_PE_OD_PE5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,5)))
#define TSB_PE_OD_PE6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,6)))
#define TSB_PE_OD_PE7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,7)))
#define TSB_PE_PUP_PE0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,0)))
#define TSB_PE_PUP_PE1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,1)))
#define TSB_PE_PUP_PE2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,2)))
#define TSB_PE_PUP_PE3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,3)))
#define TSB_PE_PUP_PE4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,4)))
#define TSB_PE_PUP_PE5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,5)))
#define TSB_PE_PUP_PE6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,6)))
#define TSB_PE_PUP_PE7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,7)))
#define TSB_PE_IE_PE0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,0)))
#define TSB_PE_IE_PE1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,1)))
#define TSB_PE_IE_PE2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,2)))
#define TSB_PE_IE_PE3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,3)))
#define TSB_PE_IE_PE4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,4)))
#define TSB_PE_IE_PE5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,5)))
#define TSB_PE_IE_PE6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,6)))
#define TSB_PE_IE_PE7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,7)))


/* General Purpose Input/Output Port (PF) */
#define TSB_PF_DATA_PF0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,0)))
#define TSB_PF_DATA_PF1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,1)))
#define TSB_PF_DATA_PF2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,2)))
#define TSB_PF_DATA_PF3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,3)))
#define TSB_PF_DATA_PF4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,4)))
#define TSB_PF_DATA_PF5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,5)))
#define TSB_PF_DATA_PF6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,6)))
#define TSB_PF_DATA_PF7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,7)))
#define TSB_PF_CR_PF0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,0)))
#define TSB_PF_CR_PF1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,1)))
#define TSB_PF_CR_PF2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,2)))
#define TSB_PF_CR_PF3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,3)))
#define TSB_PF_CR_PF4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,4)))
#define TSB_PF_CR_PF5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,5)))
#define TSB_PF_CR_PF6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,6)))
#define TSB_PF_CR_PF7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,7)))
#define TSB_PF_FR1_PF0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,0)))
#define TSB_PF_FR1_PF1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,1)))
#define TSB_PF_FR1_PF2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,2)))
#define TSB_PF_FR1_PF3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,3)))
#define TSB_PF_FR1_PF4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,4)))
#define TSB_PF_FR1_PF5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,5)))
#define TSB_PF_FR1_PF6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,6)))
#define TSB_PF_FR1_PF7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,7)))
#define TSB_PF_FR2_PF4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR2,4)))
#define TSB_PF_FR2_PF5F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR2,5)))
#define TSB_PF_FR3_PF0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR3,0)))
#define TSB_PF_FR3_PF4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR3,4)))
#define TSB_PF_FR3_PF5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR3,5)))
#define TSB_PF_OD_PF0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,0)))
#define TSB_PF_OD_PF1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,1)))
#define TSB_PF_OD_PF2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,2)))
#define TSB_PF_OD_PF3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,3)))
#define TSB_PF_OD_PF4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,4)))
#define TSB_PF_OD_PF5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,5)))
#define TSB_PF_OD_PF6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,6)))
#define TSB_PF_OD_PF7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,7)))
#define TSB_PF_PUP_PF0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,0)))
#define TSB_PF_PUP_PF1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,1)))
#define TSB_PF_PUP_PF2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,2)))
#define TSB_PF_PUP_PF3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,3)))
#define TSB_PF_PUP_PF4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,4)))
#define TSB_PF_PUP_PF5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,5)))
#define TSB_PF_PUP_PF6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,6)))
#define TSB_PF_PUP_PF7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,7)))
#define TSB_PF_IE_PF1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,1)))
#define TSB_PF_IE_PF2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,2)))
#define TSB_PF_IE_PF3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,3)))
#define TSB_PF_IE_PF4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,4)))
#define TSB_PF_IE_PF5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,5)))
#define TSB_PF_IE_PF6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,6)))
#define TSB_PF_IE_PF7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,7)))


/* General Purpose Input/Output Port (PG) */
#define TSB_PG_DATA_PG0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,0)))
#define TSB_PG_DATA_PG1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,1)))
#define TSB_PG_DATA_PG2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,2)))
#define TSB_PG_DATA_PG3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,3)))
#define TSB_PG_DATA_PG4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,4)))
#define TSB_PG_DATA_PG5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,5)))
#define TSB_PG_CR_PG0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,0)))
#define TSB_PG_CR_PG1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,1)))
#define TSB_PG_CR_PG2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,2)))
#define TSB_PG_CR_PG3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,3)))
#define TSB_PG_CR_PG4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,4)))
#define TSB_PG_CR_PG5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,5)))
#define TSB_PG_FR1_PG0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,0)))
#define TSB_PG_FR1_PG1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,1)))
#define TSB_PG_FR1_PG2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,2)))
#define TSB_PG_FR1_PG3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,3)))
#define TSB_PG_FR1_PG5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,5)))
#define TSB_PG_FR2_PG0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR2,0)))
#define TSB_PG_FR2_PG1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR2,1)))
#define TSB_PG_FR2_PG2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR2,2)))
#define TSB_PG_FR2_PG3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR2,3)))
#define TSB_PG_FR2_PG4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR2,4)))
#define TSB_PG_FR3_PG1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR3,1)))
#define TSB_PG_FR3_PG2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR3,2)))
#define TSB_PG_FR3_PG3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR3,3)))
#define TSB_PG_FR3_PG4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR3,4)))
#define TSB_PG_FR4_PG0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR4,0)))
#define TSB_PG_FR4_PG1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR4,1)))
#define TSB_PG_FR4_PG2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR4,2)))
#define TSB_PG_FR4_PG3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR4,3)))
#define TSB_PG_FR4_PG4F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR4,4)))
#define TSB_PG_FR4_PG5F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR4,5)))
#define TSB_PG_FR5_PG0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,0)))
#define TSB_PG_FR5_PG1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,1)))
#define TSB_PG_OD_PG0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,0)))
#define TSB_PG_OD_PG1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,1)))
#define TSB_PG_OD_PG2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,2)))
#define TSB_PG_OD_PG3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,3)))
#define TSB_PG_OD_PG4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,4)))
#define TSB_PG_OD_PG5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,5)))
#define TSB_PG_PUP_PG0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,0)))
#define TSB_PG_PUP_PG1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,1)))
#define TSB_PG_PUP_PG2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,2)))
#define TSB_PG_PUP_PG3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,3)))
#define TSB_PG_PUP_PG4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,4)))
#define TSB_PG_PUP_PG5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,5)))
#define TSB_PG_IE_PG0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,0)))
#define TSB_PG_IE_PG1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,1)))
#define TSB_PG_IE_PG2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,2)))
#define TSB_PG_IE_PG3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,3)))
#define TSB_PG_IE_PG4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,4)))
#define TSB_PG_IE_PG5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,5)))


/* General Purpose Input/Output Port (PH) */
#define TSB_PH_DATA_PH0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,0)))
#define TSB_PH_DATA_PH1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,1)))
#define TSB_PH_DATA_PH2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,2)))
#define TSB_PH_DATA_PH3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,3)))
#define TSB_PH_DATA_PH4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,4)))
#define TSB_PH_CR_PH0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,0)))
#define TSB_PH_CR_PH1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,1)))
#define TSB_PH_CR_PH2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,2)))
#define TSB_PH_CR_PH3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,3)))
#define TSB_PH_CR_PH4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,4)))
#define TSB_PH_FR1_PH0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR1,0)))
#define TSB_PH_FR1_PH2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR1,2)))
#define TSB_PH_FR2_PH2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR2,2)))
#define TSB_PH_FR2_PH3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR2,3)))
#define TSB_PH_FR2_PH4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR2,4)))
#define TSB_PH_FR3_PH2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,2)))
#define TSB_PH_FR3_PH3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,3)))
#define TSB_PH_FR3_PH4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,4)))
#define TSB_PH_FR4_PH2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR4,2)))
#define TSB_PH_FR4_PH3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR4,3)))
#define TSB_PH_FR4_PH4F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR4,4)))
#define TSB_PH_OD_PH0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,0)))
#define TSB_PH_OD_PH1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,1)))
#define TSB_PH_OD_PH2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,2)))
#define TSB_PH_OD_PH3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,3)))
#define TSB_PH_OD_PH4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,4)))
#define TSB_PH_PUP_PH0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,0)))
#define TSB_PH_PUP_PH1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,1)))
#define TSB_PH_PUP_PH2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,2)))
#define TSB_PH_PUP_PH3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,3)))
#define TSB_PH_PUP_PH4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,4)))
#define TSB_PH_IE_PH0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,0)))
#define TSB_PH_IE_PH1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,1)))
#define TSB_PH_IE_PH2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,2)))
#define TSB_PH_IE_PH3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,3)))
#define TSB_PH_IE_PH4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,4)))


/* General Purpose Input/Output Port (PI) */
#define TSB_PI_DATA_PI0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->DATA,0)))
#define TSB_PI_DATA_PI1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->DATA,1)))
#define TSB_PI_DATA_PI2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->DATA,2)))
#define TSB_PI_DATA_PI3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->DATA,3)))
#define TSB_PI_DATA_PI4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->DATA,4)))
#define TSB_PI_DATA_PI5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->DATA,5)))
#define TSB_PI_DATA_PI6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->DATA,6)))
#define TSB_PI_DATA_PI7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->DATA,7)))
#define TSB_PI_CR_PI0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->CR,0)))
#define TSB_PI_CR_PI1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->CR,1)))
#define TSB_PI_CR_PI2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->CR,2)))
#define TSB_PI_CR_PI3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->CR,3)))
#define TSB_PI_CR_PI4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->CR,4)))
#define TSB_PI_CR_PI5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->CR,5)))
#define TSB_PI_CR_PI6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->CR,6)))
#define TSB_PI_CR_PI7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->CR,7)))
#define TSB_PI_FR1_PI0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->FR1,0)))
#define TSB_PI_FR1_PI1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->FR1,1)))
#define TSB_PI_FR1_PI2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->FR1,2)))
#define TSB_PI_FR1_PI3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->FR1,3)))
#define TSB_PI_FR1_PI4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->FR1,4)))
#define TSB_PI_FR1_PI5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->FR1,5)))
#define TSB_PI_FR1_PI6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->FR1,6)))
#define TSB_PI_FR1_PI7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->FR1,7)))
#define TSB_PI_OD_PI0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->OD,0)))
#define TSB_PI_OD_PI1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->OD,1)))
#define TSB_PI_OD_PI2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->OD,2)))
#define TSB_PI_PUP_PI0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->PUP,0)))
#define TSB_PI_PUP_PI1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->PUP,1)))
#define TSB_PI_PUP_PI2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->PUP,2)))
#define TSB_PI_PUP_PI4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->PUP,4)))
#define TSB_PI_PUP_PI5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->PUP,5)))
#define TSB_PI_PUP_PI6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->PUP,6)))
#define TSB_PI_PUP_PI7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->PUP,7)))
#define TSB_PI_PDN_PI3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->PDN,3)))
#define TSB_PI_IE_PI0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->IE,0)))
#define TSB_PI_IE_PI1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->IE,1)))
#define TSB_PI_IE_PI2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->IE,2)))
#define TSB_PI_IE_PI3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->IE,3)))
#define TSB_PI_IE_PI4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->IE,4)))
#define TSB_PI_IE_PI5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->IE,5)))
#define TSB_PI_IE_PI6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->IE,6)))
#define TSB_PI_IE_PI7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PI->IE,7)))


/* General Purpose Input/Output Port (PJ) */
#define TSB_PJ_DATA_PJ0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,0)))
#define TSB_PJ_DATA_PJ1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,1)))
#define TSB_PJ_DATA_PJ2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,2)))
#define TSB_PJ_DATA_PJ3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,3)))
#define TSB_PJ_DATA_PJ4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,4)))
#define TSB_PJ_DATA_PJ5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,5)))
#define TSB_PJ_DATA_PJ6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,6)))
#define TSB_PJ_DATA_PJ7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,7)))
#define TSB_PJ_CR_PJ0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,0)))
#define TSB_PJ_CR_PJ1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,1)))
#define TSB_PJ_CR_PJ2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,2)))
#define TSB_PJ_CR_PJ3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,3)))
#define TSB_PJ_CR_PJ4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,4)))
#define TSB_PJ_CR_PJ5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,5)))
#define TSB_PJ_CR_PJ6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,6)))
#define TSB_PJ_CR_PJ7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,7)))
#define TSB_PJ_FR2_PJ7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR2,7)))
#define TSB_PJ_FR3_PJ6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,6)))
#define TSB_PJ_FR3_PJ7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,7)))
#define TSB_PJ_PUP_PJ0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,0)))
#define TSB_PJ_PUP_PJ1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,1)))
#define TSB_PJ_PUP_PJ2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,2)))
#define TSB_PJ_PUP_PJ3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,3)))
#define TSB_PJ_PUP_PJ4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,4)))
#define TSB_PJ_PUP_PJ5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,5)))
#define TSB_PJ_PUP_PJ6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,6)))
#define TSB_PJ_PUP_PJ7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,7)))
#define TSB_PJ_IE_PJ0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,0)))
#define TSB_PJ_IE_PJ1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,1)))
#define TSB_PJ_IE_PJ2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,2)))
#define TSB_PJ_IE_PJ3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,3)))
#define TSB_PJ_IE_PJ4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,4)))
#define TSB_PJ_IE_PJ5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,5)))
#define TSB_PJ_IE_PJ6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,6)))
#define TSB_PJ_IE_PJ7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,7)))


/* General Purpose Input/Output Port (PK) */
#define TSB_PK_DATA_PK0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,0)))
#define TSB_PK_DATA_PK1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,1)))
#define TSB_PK_DATA_PK2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,2)))
#define TSB_PK_DATA_PK3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,3)))
#define TSB_PK_CR_PK0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,0)))
#define TSB_PK_CR_PK1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,1)))
#define TSB_PK_CR_PK2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,2)))
#define TSB_PK_CR_PK3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,3)))
#define TSB_PK_FR2_PK0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR2,0)))
#define TSB_PK_FR2_PK1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR2,1)))
#define TSB_PK_FR3_PK0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,0)))
#define TSB_PK_FR3_PK1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,1)))
#define TSB_PK_FR3_PK2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,2)))
#define TSB_PK_FR3_PK3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,3)))
#define TSB_PK_PUP_PK0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,0)))
#define TSB_PK_PUP_PK1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,1)))
#define TSB_PK_PUP_PK2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,2)))
#define TSB_PK_PUP_PK3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,3)))
#define TSB_PK_IE_PK0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,0)))
#define TSB_PK_IE_PK1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,1)))
#define TSB_PK_IE_PK2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,2)))
#define TSB_PK_IE_PK3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,3)))


/* 16-bit Timer/Event Counter (TB) */
#define TSB_TB0_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->EN,6)))
#define TSB_TB0_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->EN,7)))
#define TSB_TB0_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->RUN,0)))
#define TSB_TB0_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->RUN,2)))
#define TSB_TB0_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->CR,0)))
#define TSB_TB0_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->CR,1)))
#define TSB_TB0_CR_TBINSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->CR,2)))
#define TSB_TB0_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->CR,3)))
#define TSB_TB0_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->CR,5)))
#define TSB_TB0_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->CR,7)))
#define TSB_TB0_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->MOD,3)))
#define TSB_TB0_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB0->MOD,6)))
#define TSB_TB0_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->FFCR,2)))
#define TSB_TB0_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->FFCR,3)))
#define TSB_TB0_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->FFCR,4)))
#define TSB_TB0_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->FFCR,5)))
#define TSB_TB0_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->IM,0)))
#define TSB_TB0_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->IM,1)))
#define TSB_TB0_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->IM,2)))
#define TSB_TB0_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->DMA,0)))
#define TSB_TB0_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->DMA,1)))
#define TSB_TB0_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->DMA,2)))

#define TSB_TB1_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->EN,6)))
#define TSB_TB1_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->EN,7)))
#define TSB_TB1_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->RUN,0)))
#define TSB_TB1_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->RUN,2)))
#define TSB_TB1_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->CR,0)))
#define TSB_TB1_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->CR,1)))
#define TSB_TB1_CR_TBINSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->CR,2)))
#define TSB_TB1_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->CR,3)))
#define TSB_TB1_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->CR,5)))
#define TSB_TB1_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->CR,7)))
#define TSB_TB1_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->MOD,3)))
#define TSB_TB1_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB1->MOD,6)))
#define TSB_TB1_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->FFCR,2)))
#define TSB_TB1_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->FFCR,3)))
#define TSB_TB1_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->FFCR,4)))
#define TSB_TB1_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->FFCR,5)))
#define TSB_TB1_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->IM,0)))
#define TSB_TB1_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->IM,1)))
#define TSB_TB1_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->IM,2)))
#define TSB_TB1_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->DMA,0)))
#define TSB_TB1_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->DMA,1)))
#define TSB_TB1_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->DMA,2)))

#define TSB_TB2_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->EN,6)))
#define TSB_TB2_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->EN,7)))
#define TSB_TB2_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->RUN,0)))
#define TSB_TB2_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->RUN,2)))
#define TSB_TB2_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->CR,0)))
#define TSB_TB2_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->CR,1)))
#define TSB_TB2_CR_TBINSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->CR,2)))
#define TSB_TB2_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->CR,3)))
#define TSB_TB2_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->CR,5)))
#define TSB_TB2_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->CR,7)))
#define TSB_TB2_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->MOD,3)))
#define TSB_TB2_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB2->MOD,6)))
#define TSB_TB2_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->FFCR,2)))
#define TSB_TB2_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->FFCR,3)))
#define TSB_TB2_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->FFCR,4)))
#define TSB_TB2_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->FFCR,5)))
#define TSB_TB2_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->IM,0)))
#define TSB_TB2_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->IM,1)))
#define TSB_TB2_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->IM,2)))
#define TSB_TB2_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->DMA,0)))
#define TSB_TB2_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->DMA,1)))
#define TSB_TB2_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->DMA,2)))

#define TSB_TB3_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->EN,6)))
#define TSB_TB3_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->EN,7)))
#define TSB_TB3_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->RUN,0)))
#define TSB_TB3_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->RUN,2)))
#define TSB_TB3_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->CR,0)))
#define TSB_TB3_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->CR,1)))
#define TSB_TB3_CR_TBINSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->CR,2)))
#define TSB_TB3_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->CR,3)))
#define TSB_TB3_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->CR,5)))
#define TSB_TB3_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->CR,7)))
#define TSB_TB3_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->MOD,3)))
#define TSB_TB3_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB3->MOD,6)))
#define TSB_TB3_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->FFCR,2)))
#define TSB_TB3_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->FFCR,3)))
#define TSB_TB3_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->FFCR,4)))
#define TSB_TB3_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->FFCR,5)))
#define TSB_TB3_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->IM,0)))
#define TSB_TB3_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->IM,1)))
#define TSB_TB3_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->IM,2)))
#define TSB_TB3_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->DMA,0)))
#define TSB_TB3_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->DMA,1)))
#define TSB_TB3_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->DMA,2)))

#define TSB_TB4_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->EN,6)))
#define TSB_TB4_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->EN,7)))
#define TSB_TB4_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->RUN,0)))
#define TSB_TB4_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->RUN,2)))
#define TSB_TB4_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->CR,0)))
#define TSB_TB4_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->CR,1)))
#define TSB_TB4_CR_TBINSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->CR,2)))
#define TSB_TB4_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->CR,3)))
#define TSB_TB4_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->CR,5)))
#define TSB_TB4_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->CR,7)))
#define TSB_TB4_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->MOD,3)))
#define TSB_TB4_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB4->MOD,6)))
#define TSB_TB4_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->FFCR,2)))
#define TSB_TB4_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->FFCR,3)))
#define TSB_TB4_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->FFCR,4)))
#define TSB_TB4_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->FFCR,5)))
#define TSB_TB4_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->IM,0)))
#define TSB_TB4_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->IM,1)))
#define TSB_TB4_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->IM,2)))
#define TSB_TB4_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->DMA,0)))
#define TSB_TB4_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->DMA,1)))
#define TSB_TB4_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->DMA,2)))

#define TSB_TB5_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->EN,6)))
#define TSB_TB5_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->EN,7)))
#define TSB_TB5_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->RUN,0)))
#define TSB_TB5_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->RUN,2)))
#define TSB_TB5_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->CR,0)))
#define TSB_TB5_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->CR,1)))
#define TSB_TB5_CR_TBINSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->CR,2)))
#define TSB_TB5_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->CR,3)))
#define TSB_TB5_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->CR,5)))
#define TSB_TB5_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->CR,7)))
#define TSB_TB5_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->MOD,3)))
#define TSB_TB5_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB5->MOD,6)))
#define TSB_TB5_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->FFCR,2)))
#define TSB_TB5_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->FFCR,3)))
#define TSB_TB5_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->FFCR,4)))
#define TSB_TB5_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->FFCR,5)))
#define TSB_TB5_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->IM,0)))
#define TSB_TB5_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->IM,1)))
#define TSB_TB5_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->IM,2)))
#define TSB_TB5_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->DMA,0)))
#define TSB_TB5_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->DMA,1)))
#define TSB_TB5_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->DMA,2)))

#define TSB_TB6_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->EN,6)))
#define TSB_TB6_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->EN,7)))
#define TSB_TB6_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->RUN,0)))
#define TSB_TB6_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->RUN,2)))
#define TSB_TB6_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->CR,0)))
#define TSB_TB6_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->CR,1)))
#define TSB_TB6_CR_TBINSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->CR,2)))
#define TSB_TB6_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->CR,3)))
#define TSB_TB6_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->CR,5)))
#define TSB_TB6_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->CR,7)))
#define TSB_TB6_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->MOD,3)))
#define TSB_TB6_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB6->MOD,6)))
#define TSB_TB6_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->FFCR,2)))
#define TSB_TB6_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->FFCR,3)))
#define TSB_TB6_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->FFCR,4)))
#define TSB_TB6_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->FFCR,5)))
#define TSB_TB6_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->IM,0)))
#define TSB_TB6_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->IM,1)))
#define TSB_TB6_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->IM,2)))
#define TSB_TB6_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->DMA,0)))
#define TSB_TB6_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->DMA,1)))
#define TSB_TB6_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->DMA,2)))

#define TSB_TB7_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->EN,6)))
#define TSB_TB7_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->EN,7)))
#define TSB_TB7_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->RUN,0)))
#define TSB_TB7_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->RUN,2)))
#define TSB_TB7_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->CR,0)))
#define TSB_TB7_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->CR,1)))
#define TSB_TB7_CR_TBINSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->CR,2)))
#define TSB_TB7_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->CR,3)))
#define TSB_TB7_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->CR,5)))
#define TSB_TB7_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->CR,7)))
#define TSB_TB7_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->MOD,3)))
#define TSB_TB7_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB7->MOD,6)))
#define TSB_TB7_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->FFCR,2)))
#define TSB_TB7_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->FFCR,3)))
#define TSB_TB7_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->FFCR,4)))
#define TSB_TB7_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->FFCR,5)))
#define TSB_TB7_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->IM,0)))
#define TSB_TB7_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->IM,1)))
#define TSB_TB7_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->IM,2)))
#define TSB_TB7_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->DMA,0)))
#define TSB_TB7_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->DMA,1)))
#define TSB_TB7_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->DMA,2)))

#define TSB_TB8_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->EN,6)))
#define TSB_TB8_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->EN,7)))
#define TSB_TB8_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->RUN,0)))
#define TSB_TB8_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->RUN,2)))
#define TSB_TB8_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->CR,0)))
#define TSB_TB8_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->CR,1)))
#define TSB_TB8_CR_TBINSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->CR,2)))
#define TSB_TB8_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->CR,3)))
#define TSB_TB8_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->CR,5)))
#define TSB_TB8_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->CR,7)))
#define TSB_TB8_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->MOD,3)))
#define TSB_TB8_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB8->MOD,6)))
#define TSB_TB8_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->FFCR,2)))
#define TSB_TB8_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->FFCR,3)))
#define TSB_TB8_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->FFCR,4)))
#define TSB_TB8_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->FFCR,5)))
#define TSB_TB8_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->IM,0)))
#define TSB_TB8_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->IM,1)))
#define TSB_TB8_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->IM,2)))
#define TSB_TB8_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->DMA,0)))
#define TSB_TB8_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->DMA,1)))
#define TSB_TB8_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB8->DMA,2)))

#define TSB_TB9_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->EN,6)))
#define TSB_TB9_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->EN,7)))
#define TSB_TB9_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->RUN,0)))
#define TSB_TB9_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->RUN,2)))
#define TSB_TB9_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->CR,0)))
#define TSB_TB9_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->CR,1)))
#define TSB_TB9_CR_TBINSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->CR,2)))
#define TSB_TB9_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->CR,3)))
#define TSB_TB9_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->CR,5)))
#define TSB_TB9_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->CR,7)))
#define TSB_TB9_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->MOD,3)))
#define TSB_TB9_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB9->MOD,6)))
#define TSB_TB9_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->FFCR,2)))
#define TSB_TB9_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->FFCR,3)))
#define TSB_TB9_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->FFCR,4)))
#define TSB_TB9_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->FFCR,5)))
#define TSB_TB9_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->IM,0)))
#define TSB_TB9_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->IM,1)))
#define TSB_TB9_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->IM,2)))
#define TSB_TB9_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->DMA,0)))
#define TSB_TB9_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->DMA,1)))
#define TSB_TB9_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB9->DMA,2)))


/* Serial Bus Interface (SBI) */
#define TSB_SBI0_CR0_SBIEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SBI0->CR0,7)))
#define TSB_SBI0_CR1_SWRMON                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI0->CR1,0)))
#define TSB_SBI0_I2CAR_ALS                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SBI0->I2CAR,0)))
#define TSB_SBI0_SR_LRB                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI0->SR,0)))
#define TSB_SBI0_SR_ADO                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI0->SR,1)))
#define TSB_SBI0_SR_AAS                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI0->SR,2)))
#define TSB_SBI0_SR_AL                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI0->SR,3)))
#define TSB_SBI0_SR_PIN                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI0->SR,4)))
#define TSB_SBI0_SR_BB                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI0->SR,5)))
#define TSB_SBI0_SR_TRX                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI0->SR,6)))
#define TSB_SBI0_SR_MST                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI0->SR,7)))
#define TSB_SBI0_SR_SEF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI0->SR,2)))
#define TSB_SBI0_SR_SIOF                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI0->SR,3)))
#define TSB_SBI0_BR0_I2SBI                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SBI0->BR0,6)))

#define TSB_SBI1_CR0_SBIEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SBI1->CR0,7)))
#define TSB_SBI1_CR1_SWRMON                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI1->CR1,0)))
#define TSB_SBI1_I2CAR_ALS                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SBI1->I2CAR,0)))
#define TSB_SBI1_SR_LRB                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI1->SR,0)))
#define TSB_SBI1_SR_ADO                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI1->SR,1)))
#define TSB_SBI1_SR_AAS                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI1->SR,2)))
#define TSB_SBI1_SR_AL                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI1->SR,3)))
#define TSB_SBI1_SR_PIN                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI1->SR,4)))
#define TSB_SBI1_SR_BB                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI1->SR,5)))
#define TSB_SBI1_SR_TRX                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI1->SR,6)))
#define TSB_SBI1_SR_MST                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI1->SR,7)))
#define TSB_SBI1_SR_SEF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI1->SR,2)))
#define TSB_SBI1_SR_SIOF                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_SBI1->SR,3)))
#define TSB_SBI1_BR0_I2SBI                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SBI1->BR0,6)))


/* Serial Channel (SC) */
#define TSB_SC0_EN_SIOE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->EN,0)))
#define TSB_SC0_MOD0_WU                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD0,4)))
#define TSB_SC0_MOD0_RXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD0,5)))
#define TSB_SC0_MOD0_CTSE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD0,6)))
#define TSB_SC0_MOD0_TB8                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD0,7)))
#define TSB_SC0_BRCR_BRADDE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->BRCR,6)))
#define TSB_SC0_MOD1_TXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD1,4)))
#define TSB_SC0_MOD1_I2SC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD1,7)))
#define TSB_SC0_MOD2_WBUF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,2)))
#define TSB_SC0_MOD2_DRCHG                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,3)))
#define TSB_SC0_MOD2_SBLEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,4)))
#define TSB_SC0_MOD2_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,5)))
#define TSB_SC0_MOD2_RBFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,6)))
#define TSB_SC0_MOD2_TBEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,7)))
#define TSB_SC0_RFC_RFIS                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->RFC,6)))
#define TSB_SC0_RFC_RFCS                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_SC0->RFC,7)))
#define TSB_SC0_TFC_TFIS                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->TFC,6)))
#define TSB_SC0_TFC_TFCS                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_SC0->TFC,7)))
#define TSB_SC0_RST_ROR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC0->RST,7)))
#define TSB_SC0_TST_TUR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC0->TST,7)))
#define TSB_SC0_FCNF_CNFG                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,0)))
#define TSB_SC0_FCNF_RXTXCNT                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,1)))
#define TSB_SC0_FCNF_RFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,2)))
#define TSB_SC0_FCNF_TFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,3)))
#define TSB_SC0_FCNF_RFST                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,4)))
#define TSB_SC0_DMA_DMAEN0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->DMA,0)))
#define TSB_SC0_DMA_DMAEN1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->DMA,1)))

#define TSB_SC1_EN_SIOE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->EN,0)))
#define TSB_SC1_MOD0_WU                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD0,4)))
#define TSB_SC1_MOD0_RXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD0,5)))
#define TSB_SC1_MOD0_CTSE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD0,6)))
#define TSB_SC1_MOD0_TB8                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD0,7)))
#define TSB_SC1_BRCR_BRADDE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->BRCR,6)))
#define TSB_SC1_MOD1_TXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD1,4)))
#define TSB_SC1_MOD1_I2SC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD1,7)))
#define TSB_SC1_MOD2_WBUF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,2)))
#define TSB_SC1_MOD2_DRCHG                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,3)))
#define TSB_SC1_MOD2_SBLEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,4)))
#define TSB_SC1_MOD2_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,5)))
#define TSB_SC1_MOD2_RBFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,6)))
#define TSB_SC1_MOD2_TBEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,7)))
#define TSB_SC1_RFC_RFIS                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->RFC,6)))
#define TSB_SC1_RFC_RFCS                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_SC1->RFC,7)))
#define TSB_SC1_TFC_TFIS                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->TFC,6)))
#define TSB_SC1_TFC_TFCS                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_SC1->TFC,7)))
#define TSB_SC1_RST_ROR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC1->RST,7)))
#define TSB_SC1_TST_TUR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC1->TST,7)))
#define TSB_SC1_FCNF_CNFG                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,0)))
#define TSB_SC1_FCNF_RXTXCNT                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,1)))
#define TSB_SC1_FCNF_RFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,2)))
#define TSB_SC1_FCNF_TFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,3)))
#define TSB_SC1_FCNF_RFST                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,4)))
#define TSB_SC1_DMA_DMAEN0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->DMA,0)))
#define TSB_SC1_DMA_DMAEN1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->DMA,1)))


/* Watchdog Timer (WD) */
#define TSB_WD_MOD_RESCR                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_WD->MOD,1)))
#define TSB_WD_MOD_I2WDT                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_WD->MOD,2)))
#define TSB_WD_MOD_WDTE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_WD->MOD,7)))


/* Clock Generator (CG) */
#define TSB_CG_SYSCR_FPSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->SYSCR,12)))
#define TSB_CG_SYSCR_FCSTOP                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->SYSCR,20)))
#define TSB_CG_OSCCR_WUEON                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,0)))
#define TSB_CG_OSCCR_WUEF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,1)))
#define TSB_CG_OSCCR_PLLON                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,2)))
#define TSB_CG_OSCCR_XEN1                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,8)))
#define TSB_CG_OSCCR_XEN2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,16)))
#define TSB_CG_OSCCR_OSCSEL                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,17)))
#define TSB_CG_OSCCR_EHOSCSEL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,18)))
#define TSB_CG_OSCCR_HWUPSEL                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,19)))
#define TSB_CG_STBYCR_DRVE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->STBYCR,16)))
#define TSB_CG_STBYCR_PTKEEP                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->STBYCR,17)))
#define TSB_CG_PLLSEL_PLLSEL                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->PLLSEL,0)))
#define TSB_CG_USBCTL_USBCLKEN                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->USBCTL,8)))
#define TSB_CG_USBCTL_USBCLKSEL                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->USBCTL,9)))
#define TSB_CG_IMCGA_INT0EN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGA,0)))
#define TSB_CG_IMCGA_INT1EN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGA,8)))
#define TSB_CG_IMCGA_INT2EN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGA,16)))
#define TSB_CG_IMCGA_INT3EN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGA,24)))
#define TSB_CG_IMCGB_INT4EN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGB,0)))
#define TSB_CG_IMCGB_INT5EN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGB,8)))
#define TSB_CG_IMCGB_INT6EN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGB,16)))
#define TSB_CG_IMCGB_INT7EN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGB,24)))
#define TSB_CG_IMCGC_INT8EN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGC,0)))
#define TSB_CG_IMCGC_INT9EN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGC,8)))
#define TSB_CG_IMCGC_INTAEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGC,16)))
#define TSB_CG_IMCGC_INTBEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGC,24)))


/** @} */ /* End of group Device_Peripheral_registers */

#ifdef __cplusplus
}
#endif

#endif  /* __TMPM366_H__ */

/** @} */ /* End of group TMPM366 */
/** @} */ /* End of group TOSHIBA_TX03_MICROCONTROLLER */
