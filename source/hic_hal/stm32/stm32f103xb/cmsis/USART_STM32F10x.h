/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2016 Arm Limited (or its affiliates). All
 * rights reserved.
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
 * $Date:        29. March 2016
 * $Revision:    V2.2
 *
 * Project:      USART Driver definitions for ST STM32F1xx
 * -------------------------------------------------------------------------- */

#ifndef __USART_STM32F1XX_H
#define __USART_STM32F1XX_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "stm32f10x.h"
#include "GPIO_STM32F10x.h"
#include "DMA_STM32F10x.h"

#include "Driver_USART.h"

#include "RTE_Components.h"
#include "RTE_Device.h"

#if ((defined(RTE_Drivers_USART1) || defined(RTE_Drivers_USART2) || \
      defined(RTE_Drivers_USART3) || defined(RTE_Drivers_USART4) || \
      defined(RTE_Drivers_USART5))                                  \
     && (RTE_USART1 == 0)   \
     && (RTE_USART2 == 0)   \
     && (RTE_USART3 == 0)   \
     && (RTE_UART4  == 0)   \
     && (RTE_UART5  == 0))
  #error "USART not configured in RTE_Device.h!"
#endif

// USART1 configuration definitions
#if (RTE_USART1 == 1)
  #define MX_USART1

  #if (RTE_USART1_RX_DMA == 1)
    #define MX_USART1_RX_DMA_Instance DMAx_CHANNELy(RTE_USART1_RX_DMA_NUMBER, RTE_USART1_RX_DMA_CHANNEL)
    #define MX_USART1_RX_DMA_Number   RTE_USART1_RX_DMA_NUMBER
    #define MX_USART1_RX_DMA_Channel  RTE_USART1_RX_DMA_CHANNEL
    #define MX_USART1_RX_DMA_Priority RTE_USART1_RX_DMA_PRIORITY

    #define USART1_RX_DMA_Handler     DMAx_CHANNELy_EVENT(RTE_USART1_RX_DMA_NUMBER, RTE_USART1_RX_DMA_CHANNEL)
  #endif
  #if (RTE_USART1_TX_DMA == 1)
    #define MX_USART1_TX_DMA_Instance DMAx_CHANNELy(RTE_USART1_TX_DMA_NUMBER, RTE_USART1_TX_DMA_CHANNEL)
    #define MX_USART1_TX_DMA_Number   RTE_USART1_TX_DMA_NUMBER
    #define MX_USART1_TX_DMA_Channel  RTE_USART1_TX_DMA_CHANNEL
    #define MX_USART1_TX_DMA_Priority RTE_USART1_TX_DMA_PRIORITY

    #define USART1_TX_DMA_Handler     DMAx_CHANNELy_EVENT(RTE_USART1_TX_DMA_NUMBER, RTE_USART1_TX_DMA_CHANNEL)
  #endif

  #if (RTE_USART1_TX == 1)
    #define MX_USART1_TX_Pin          1U
    #define MX_USART1_TX_GPIOx        RTE_USART1_TX_PORT
    #define MX_USART1_TX_GPIO_Pin     RTE_USART1_TX_BIT
  #endif

  #if (RTE_USART1_RX == 1)
    #define MX_USART1_RX_Pin          1U
    #define MX_USART1_RX_GPIOx        RTE_USART1_RX_PORT
    #define MX_USART1_RX_GPIO_Pin     RTE_USART1_RX_BIT
  #endif

  #if (RTE_USART1_CK == 1)
    #define MX_USART1_CK_Pin          1U
    #define MX_USART1_CK_GPIOx        RTE_USART1_CK_PORT
    #define MX_USART1_CK_GPIO_Pin     RTE_USART1_CK_BIT
  #endif


  #if (RTE_USART1_RTS == 1)
    #define MX_USART1_RTS_Pin         1U
    #define MX_USART1_RTS_GPIOx       RTE_USART1_RTS_PORT
    #define MX_USART1_RTS_GPIO_Pin    RTE_USART1_RTS_BIT
  #endif

  #if (RTE_USART1_CTS == 1)
    #define MX_USART1_CTS_Pin         1U
    #define MX_USART1_CTS_GPIOx       RTE_USART1_CTS_PORT
    #define MX_USART1_CTS_GPIO_Pin    RTE_USART1_CTS_BIT
  #endif

  #define MX_USART1_REMAP_DEF         AFIO_USART1_NO_REMAP
  #define MX_USART1_REMAP             RTE_USART1_AF_REMAP

#endif

// USART2 configuration definitions
#if (RTE_USART2 == 1)
  #define MX_USART2

  #if (RTE_USART2_RX_DMA == 1)
    #define MX_USART2_RX_DMA_Instance DMAx_CHANNELy(RTE_USART2_RX_DMA_NUMBER, RTE_USART2_RX_DMA_CHANNEL)
    #define MX_USART2_RX_DMA_Number   RTE_USART2_RX_DMA_NUMBER
    #define MX_USART2_RX_DMA_Channel  RTE_USART2_RX_DMA_CHANNEL
    #define MX_USART2_RX_DMA_Priority RTE_USART2_RX_DMA_PRIORITY

    #define USART2_RX_DMA_Handler     DMAx_CHANNELy_EVENT(RTE_USART2_RX_DMA_NUMBER, RTE_USART2_RX_DMA_CHANNEL)
  #endif
  #if (RTE_USART2_TX_DMA == 1)
    #define MX_USART2_TX_DMA_Instance DMAx_CHANNELy(RTE_USART2_TX_DMA_NUMBER, RTE_USART2_TX_DMA_CHANNEL)
    #define MX_USART2_TX_DMA_Number   RTE_USART2_TX_DMA_NUMBER
    #define MX_USART2_TX_DMA_Channel  RTE_USART2_TX_DMA_CHANNEL
    #define MX_USART2_TX_DMA_Priority RTE_USART2_TX_DMA_PRIORITY

    #define USART2_TX_DMA_Handler     DMAx_CHANNELy_EVENT(RTE_USART2_TX_DMA_NUMBER, RTE_USART2_TX_DMA_CHANNEL)
  #endif

  #if (RTE_USART2_TX == 1)
    #define MX_USART2_TX_Pin          1U
    #define MX_USART2_TX_GPIOx        RTE_USART2_TX_PORT
    #define MX_USART2_TX_GPIO_Pin     RTE_USART2_TX_BIT
  #endif

  #if (RTE_USART2_RX == 1)
    #define MX_USART2_RX_Pin          1U
    #define MX_USART2_RX_GPIOx        RTE_USART2_RX_PORT
    #define MX_USART2_RX_GPIO_Pin     RTE_USART2_RX_BIT
  #endif

  #if (RTE_USART2_CK == 1)
    #define MX_USART2_CK_Pin          1U
    #define MX_USART2_CK_GPIOx        RTE_USART2_CK_PORT
    #define MX_USART2_CK_GPIO_Pin     RTE_USART2_CK_BIT
  #endif


  #if (RTE_USART2_RTS == 1)
    #define MX_USART2_RTS_Pin         1U
    #define MX_USART2_RTS_GPIOx       RTE_USART2_RTS_PORT
    #define MX_USART2_RTS_GPIO_Pin    RTE_USART2_RTS_BIT
  #endif

  #if (RTE_USART2_CTS == 1)
    #define MX_USART2_CTS_Pin         1U
    #define MX_USART2_CTS_GPIOx       RTE_USART2_CTS_PORT
    #define MX_USART2_CTS_GPIO_Pin    RTE_USART2_CTS_BIT
  #endif

  #define MX_USART2_REMAP_DEF         AFIO_USART2_NO_REMAP
  #define MX_USART2_REMAP             RTE_USART2_AF_REMAP

#endif

// USART3 configuration definitions
#if (RTE_USART3 == 1)

  #if defined (STM32F10X_LD) || defined (STM32F10X_LD_VL)
    #error "USART3 is not available for selected device!"
  #endif

  #define MX_USART3

  #if (RTE_USART3_RX_DMA == 1)
    #define MX_USART3_RX_DMA_Instance DMAx_CHANNELy(RTE_USART3_RX_DMA_NUMBER, RTE_USART3_RX_DMA_CHANNEL)
    #define MX_USART3_RX_DMA_Number   RTE_USART3_RX_DMA_NUMBER
    #define MX_USART3_RX_DMA_Channel  RTE_USART3_RX_DMA_CHANNEL
    #define MX_USART3_RX_DMA_Priority RTE_USART3_RX_DMA_PRIORITY

    #define USART3_RX_DMA_Handler     DMAx_CHANNELy_EVENT(RTE_USART3_RX_DMA_NUMBER, RTE_USART3_RX_DMA_CHANNEL)
  #endif
  #if (RTE_USART3_TX_DMA == 1)
    #define MX_USART3_TX_DMA_Instance DMAx_CHANNELy(RTE_USART3_TX_DMA_NUMBER, RTE_USART3_TX_DMA_CHANNEL)
    #define MX_USART3_TX_DMA_Number   RTE_USART3_TX_DMA_NUMBER
    #define MX_USART3_TX_DMA_Channel  RTE_USART3_TX_DMA_CHANNEL
    #define MX_USART3_TX_DMA_Priority RTE_USART3_TX_DMA_PRIORITY

    #define USART3_TX_DMA_Handler     DMAx_CHANNELy_EVENT(RTE_USART3_TX_DMA_NUMBER, RTE_USART3_TX_DMA_CHANNEL)
  #endif

  #if (RTE_USART3_TX == 1)
    #define MX_USART3_TX_Pin          1U
    #define MX_USART3_TX_GPIOx        RTE_USART3_TX_PORT
    #define MX_USART3_TX_GPIO_Pin     RTE_USART3_TX_BIT
  #endif

  #if (RTE_USART3_RX == 1)
    #define MX_USART3_RX_Pin          1U
    #define MX_USART3_RX_GPIOx        RTE_USART3_RX_PORT
    #define MX_USART3_RX_GPIO_Pin     RTE_USART3_RX_BIT
  #endif

  #if (RTE_USART3_CK == 1)
    #define MX_USART3_CK_Pin          1U
    #define MX_USART3_CK_GPIOx        RTE_USART3_CK_PORT
    #define MX_USART3_CK_GPIO_Pin     RTE_USART3_CK_BIT
  #endif


  #if (RTE_USART3_RTS == 1)
    #define MX_USART3_RTS_Pin         1U
    #define MX_USART3_RTS_GPIOx       RTE_USART3_RTS_PORT
    #define MX_USART3_RTS_GPIO_Pin    RTE_USART3_RTS_BIT
  #endif

  #if (RTE_USART3_CTS == 1)
    #define MX_USART3_CTS_Pin         1U
    #define MX_USART3_CTS_GPIOx       RTE_USART3_CTS_PORT
    #define MX_USART3_CTS_GPIO_Pin    RTE_USART3_CTS_BIT
  #endif

  #define MX_USART3_REMAP_DEF         AFIO_USART3_NO_REMAP
  #define MX_USART3_REMAP             RTE_USART3_AF_REMAP

#endif

// UART4 configuration definitions
#if (RTE_UART4 == 1)

  #if !defined(STM32F10X_HD) && !defined(STM32F10X_CL) && !defined(STM32F10X_XL) && !defined(STM32F10X_HD_VL)
    #error "UART4 is not available for selected device!"
  #endif

  #define MX_UART4

  #if (RTE_UART4_RX_DMA == 1)
    #define MX_UART4_RX_DMA_Instance  DMAx_CHANNELy(RTE_UART4_RX_DMA_NUMBER, RTE_UART4_RX_DMA_CHANNEL)
    #define MX_UART4_RX_DMA_Number    RTE_UART4_RX_DMA_NUMBER
    #define MX_UART4_RX_DMA_Channel   RTE_UART4_RX_DMA_CHANNEL
    #define MX_UART4_RX_DMA_Priority  RTE_UART4_RX_DMA_PRIORITY

    #define UART4_RX_DMA_Handler      DMAx_CHANNELy_EVENT(RTE_UART4_RX_DMA_NUMBER, RTE_UART4_RX_DMA_CHANNEL)
  #endif
  #if (RTE_UART4_TX_DMA == 1)
    #define MX_UART4_TX_DMA_Instance  DMAx_CHANNELy(RTE_UART4_TX_DMA_NUMBER, RTE_UART4_TX_DMA_CHANNEL)
    #define MX_UART4_TX_DMA_Number    RTE_UART4_TX_DMA_NUMBER
    #define MX_UART4_TX_DMA_Channel   RTE_UART4_TX_DMA_CHANNEL
    #define MX_UART4_TX_DMA_Priority  RTE_UART4_TX_DMA_PRIORITY

    #define UART4_TX_DMA_Handler      DMAx_CHANNELy_EVENT(RTE_UART4_TX_DMA_NUMBER, RTE_UART4_TX_DMA_CHANNEL)
  #endif

  #if (RTE_UART4_TX == 1)
    #define MX_UART4_TX_Pin           1U
    #define MX_UART4_TX_GPIOx         RTE_UART4_TX_PORT
    #define MX_UART4_TX_GPIO_Pin      RTE_UART4_TX_BIT
  #endif

  #if (RTE_UART4_RX == 1)
    #define MX_UART4_RX_Pin           1U
    #define MX_UART4_RX_GPIOx         RTE_UART4_RX_PORT
    #define MX_UART4_RX_GPIO_Pin      RTE_UART4_RX_BIT
  #endif

  #define MX_UART4_REMAP_DEF          AFIO_UNAVAILABLE_REMAP
  #define MX_UART4_REMAP              AFIO_UNAVAILABLE_REMAP

#endif

// UART5 configuration definitions
#if (RTE_UART5 == 1)

  #if !defined(STM32F10X_HD) && !defined(STM32F10X_CL) && !defined(STM32F10X_XL) && !defined(STM32F10X_HD_VL)
    #error "UART5 is not available for selected device!"
  #endif

  #define MX_UART5

  #if (RTE_UART5_RX_DMA == 1)
    #define MX_UART5_RX_DMA_Instance  DMAx_CHANNELy(RTE_UART5_RX_DMA_NUMBER, RTE_UART5_RX_DMA_CHANNEL)
    #define MX_UART5_RX_DMA_Number    RTE_UART5_RX_DMA_NUMBER
    #define MX_UART5_RX_DMA_Channel   RTE_UART5_RX_DMA_CHANNEL
    #define MX_UART5_RX_DMA_Priority  RTE_UART5_RX_DMA_PRIORITY

    #define UART5_RX_DMA_Handler      DMAx_CHANNELy_EVENT(RTE_UART5_RX_DMA_NUMBER, RTE_UART5_RX_DMA_CHANNEL)
  #endif
  #if (RTE_UART5_TX_DMA == 1)
    #define MX_UART5_TX_DMA_Instance  DMAx_CHANNELy(RTE_UART5_TX_DMA_NUMBER, RTE_UART5_TX_DMA_CHANNEL)
    #define MX_UART5_TX_DMA_Number    RTE_UART5_TX_DMA_NUMBER
    #define MX_UART5_TX_DMA_Channel   RTE_UART5_TX_DMA_CHANNEL
    #define MX_UART5_TX_DMA_Priority  RTE_UART5_TX_DMA_PRIORITY

    #define UART5_TX_DMA_Handler      DMAx_CHANNELy_EVENT(RTE_UART5_TX_DMA_NUMBER, RTE_UART5_TX_DMA_CHANNEL)
  #endif

  #if (RTE_UART5_TX == 1)
    #define MX_UART5_TX_Pin           1U
    #define MX_UART5_TX_GPIOx         RTE_UART5_TX_PORT
    #define MX_UART5_TX_GPIO_Pin      RTE_UART5_TX_BIT
  #endif

  #if (RTE_UART5_RX == 1)
    #define MX_UART5_RX_Pin           1U
    #define MX_UART5_RX_GPIOx         RTE_UART5_RX_PORT
    #define MX_UART5_RX_GPIO_Pin      RTE_UART5_RX_BIT
  #endif

  #define MX_UART5_REMAP_DEF          AFIO_UNAVAILABLE_REMAP
  #define MX_UART5_REMAP              AFIO_UNAVAILABLE_REMAP

#endif

#ifdef MX_USART1
#if (defined(MX_USART1_RX_DMA_Instance) || defined(MX_USART1_TX_DMA_Instance))
#ifndef MX_USART1_RX_DMA_Instance
  #error "USART1 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_USART1_TX_DMA_Instance
  #error "USART1 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#ifdef MX_USART2
#if (defined(MX_USART2_RX_DMA_Instance) || defined(MX_USART2_TX_DMA_Instance))
#ifndef MX_USART2_RX_DMA_Instance
  #error "USART2 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_USART2_TX_DMA_Instance
  #error "USART2 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#ifdef MX_USART3
#if (defined(MX_USART3_RX_DMA_Instance) || defined(MX_USART3_TX_DMA_Instance))
#ifndef MX_USART3_RX_DMA_Instance
  #error "USART3 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_USART3_TX_DMA_Instance
  #error "USART3 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#ifdef MX_UART4
#if (defined(MX_UART4_RX_DMA_Instance) || defined(MX_UART4_TX_DMA_Instance))
#ifndef MX_UART4_RX_DMA_Instance
  #error "UART4 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_UART4_TX_DMA_Instance
  #error "UART4 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#ifdef MX_UART5
#if (defined(MX_UART5_RX_DMA_Instance) || defined(MX_UART5_TX_DMA_Instance))
#ifndef MX_UART5_RX_DMA_Instance
  #error "UART5 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_UART5_TX_DMA_Instance
  #error "UART5 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#if (defined(MX_USART1_RX_DMA_Instance) || \
     defined(MX_USART2_RX_DMA_Instance) || \
     defined(MX_USART3_RX_DMA_Instance) || \
     defined(MX_UART4_RX_DMA_Instance ) || \
     defined(MX_UART5_RX_DMA_Instance ))
#define __USART_DMA_RX
#endif
#if (defined(MX_USART1_TX_DMA_Instance) || \
     defined(MX_USART2_TX_DMA_Instance) || \
     defined(MX_USART3_TX_DMA_Instance) || \
     defined(MX_UART4_TX_DMA_Instance ) || \
     defined(MX_UART5_TX_DMA_Instance ))
#define __USART_DMA_TX
#endif
#if (defined(__USART_DMA_RX) && defined(__USART_DMA_TX))
#define __USART_DMA
#endif

// USART BRR macro
#define USART_DIVIDER(_PCLK_, _BAUD_)           (((_PCLK_)*25)/(4*(_BAUD_)))
#define USART_DIVIDER_MANTISA(_PCLK_, _BAUD_)     (USART_DIVIDER((_PCLK_), (_BAUD_))/100)
#define USART_DIVIDER_FRACTION(_PCLK_, _BAUD_)  (((USART_DIVIDER((_PCLK_), (_BAUD_)) - (USART_DIVIDER_MANTISA((_PCLK_), (_BAUD_)) * 100)) * 16 + 50) / 100)
#define USART_BAUDRATE_DIVIDER(_PCLK_, _BAUD_)   ((USART_DIVIDER_MANTISA((_PCLK_), (_BAUD_)) << 4)|(USART_DIVIDER_FRACTION((_PCLK_), (_BAUD_)) & 0x0F))

// USART flags
#define USART_FLAG_INITIALIZED      ((uint8_t)(1U))
#define USART_FLAG_POWERED          ((uint8_t)(1U << 1))
#define USART_FLAG_CONFIGURED       ((uint8_t)(1U << 2))
#define USART_FLAG_TX_ENABLED       ((uint8_t)(1U << 3))
#define USART_FLAG_RX_ENABLED       ((uint8_t)(1U << 4))

// USART synchronous xfer modes
#define USART_SYNC_MODE_TX           ( 1UL )
#define USART_SYNC_MODE_RX           ( 2UL )
#define USART_SYNC_MODE_TX_RX        (USART_SYNC_MODE_TX | \
                                      USART_SYNC_MODE_RX)

// USART DMA
typedef const struct _USART_DMA {
  DMA_Channel_TypeDef *instance;        // Channel registry interface
  uint8_t              dma_num;         // DMA number
  uint8_t              ch_num;          // Channel number
  uint8_t              priority;        // Channel priority
} USART_DMA;

// USART pin
typedef const struct _USART_PIN {
  GPIO_TypeDef         *port;           // Port
  uint16_t              pin;            // Pin
} USART_PIN;

// USART Input/Output Configuration
typedef const struct _USART_IO {
  USART_PIN            *tx;             // TX  Pin identifier
  USART_PIN            *rx;             // RX  Pin identifier
  USART_PIN            *ck;             // CLK Pin identifier
  USART_PIN            *rts;            // RTS Pin identifier
  USART_PIN            *cts;            // CTS Pin identifier
  AFIO_REMAP            afio_def;       // Default pin remapping setting
  AFIO_REMAP            afio;           // Pin remapping information
} USART_IO;

// USART Transfer Information (Run-Time)
typedef struct _USART_TRANSFER_INFO {
  uint32_t              rx_num;         // Total number of receive data
  uint32_t              tx_num;         // Total number of transmit data
  uint8_t              *rx_buf;         // Pointer to in data buffer
  uint8_t              *tx_buf;         // Pointer to out data buffer
  uint32_t              rx_cnt;         // Number of data received
  uint32_t              tx_cnt;         // Number of data sent
  uint16_t              dump_val;       // Variable for dumping DMA data
  uint16_t              def_val;        // Default transfer value
  uint32_t              sync_mode;      // Synchronous mode flag
  uint8_t               break_flag;     // Transmit break flag
  uint8_t               send_active;    // Send active flag
} USART_TRANSFER_INFO;

typedef struct _USART_STATUS {
  uint8_t tx_busy;                      // Transmitter busy flag
  uint8_t rx_busy;                      // Receiver busy flag
  uint8_t tx_underflow;                 // Transmit data underflow detected (cleared on start of next send operation)
  uint8_t rx_overflow;                  // Receive data overflow detected (cleared on start of next receive operation)
  uint8_t rx_break;                     // Break detected on receive (cleared on start of next receive operation)
  uint8_t rx_framing_error;             // Framing error detected on receive (cleared on start of next receive operation)
  uint8_t rx_parity_error;              // Parity error detected on receive (cleared on start of next receive operation)
} USART_STATUS;

// USART Information (Run-time)
typedef struct _USART_INFO {
  ARM_USART_SignalEvent_t cb_event;            // Event Callback
  USART_STATUS            status;              // Status flags
  uint8_t                 flags;               // Current USART flags
  uint32_t                mode;                // Current USART mode
  uint32_t                flow_control;        // Flow control
} USART_INFO;

// USART Resources definition
typedef const struct {
  ARM_USART_CAPABILITIES  capabilities;        // Capabilities
  USART_TypeDef          *reg;                 // USART peripheral pointer
  uint32_t                pclk;                // Peripheral clock frequency
  USART_IO                io;                  // USART Input/Output pins
  IRQn_Type               irq_num;             // USART IRQ Number
  USART_DMA               *dma_tx;             // Transmit stream register interface
  USART_DMA               *dma_rx;             // Receive stream register interface
  USART_INFO              *info;               // Run-Time Information
  USART_TRANSFER_INFO     *xfer;               // USART transfer information
} USART_RESOURCES;

#endif /* __USART_STM32F1XX_H */
