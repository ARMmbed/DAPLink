/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2017 Arm Limited (or its affiliates). All 
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
 * $Date:        11. July 2017
 * $Revision:    V2.3
 *
 * Driver:       Driver_USART1, Driver_USART2, Driver_USART3,
 *               Driver_USART4, Driver_USART5
 * Configured:   via RTE_Device.h or MX_Device.h configuration file
 * Project:      USART Driver for ST STM32F1xx
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                   Value   UART Interface
 *   ---------------------                   -----   --------------
 *   Connect to hardware via Driver_USART# = 1       use USART1
 *   Connect to hardware via Driver_USART# = 2       use USART2
 *   Connect to hardware via Driver_USART# = 3       use USART3
 *   Connect to hardware via Driver_USART# = 4       use UART4
 *   Connect to hardware via Driver_USART# = 5       use UART5
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 2.3
 *    Corrected condition for ARM_USART_SET_IRDA_PULSE control
 *  Version 2.2
 *    Corrected CTS handling and added Signal CTS change event.
 *  Version 2.1
 *    Corrected USART Peripheral Reset and Clock enable/disable
 *      - Added checking if peripheral is available on selected device
 *  Version 2.0
 *    Updated to CMSIS Driver API V2.02
 *  Version 1.01
 *    Based on API V1.10 (namespace prefix ARM_ added)
 *  Version 1.00
 *    Initial release
 */

#include "USART_STM32F10x.h"

#define ARM_USART_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(2,3)

// Driver Version
static const ARM_DRIVER_VERSION usart_driver_version = { ARM_USART_API_VERSION, ARM_USART_DRV_VERSION };

// USART1
#ifdef MX_USART1

// USART1 Run-Time Information
static USART_INFO          USART1_Info = {0};
static USART_TRANSFER_INFO USART1_TransferInfo = {0};

#ifdef MX_USART1_TX_Pin
  static USART_PIN USART1_tx = {MX_USART1_TX_GPIOx,  MX_USART1_TX_GPIO_Pin  };
#endif
#ifdef MX_USART1_RX_Pin
  static USART_PIN USART1_rx = {MX_USART1_RX_GPIOx,  MX_USART1_RX_GPIO_Pin  };
#endif
#ifdef MX_USART1_CK_Pin
  static USART_PIN USART1_ck = {MX_USART1_CK_GPIOx,  MX_USART1_CK_GPIO_Pin  };
#endif
#ifdef MX_USART1_RTS_Pin
  static USART_PIN USART1_rts = {MX_USART1_RTS_GPIOx, MX_USART1_RTS_GPIO_Pin};
#endif
#ifdef MX_USART1_CTS_Pin
  static USART_PIN USART1_cts = {MX_USART1_CTS_GPIOx, MX_USART1_CTS_GPIO_Pin};
#endif

#ifdef MX_USART1_TX_DMA_Instance
  static USART_DMA USART1_DMA_Tx = {
    MX_USART1_TX_DMA_Instance,
    MX_USART1_TX_DMA_Number,
    MX_USART1_TX_DMA_Channel,
    MX_USART1_TX_DMA_Priority
  };
#endif
#ifdef MX_USART1_RX_DMA_Instance
  static USART_DMA USART1_DMA_Rx = {
    MX_USART1_RX_DMA_Instance,
    MX_USART1_RX_DMA_Number,
    MX_USART1_RX_DMA_Channel,
    MX_USART1_RX_DMA_Priority
  };
#endif

// USART1 Resources
static const USART_RESOURCES USART1_Resources = {
  {     // Capabilities
    1,  // supports UART (Asynchronous) mode
#ifdef MX_USART1_CK_Pin
    1,  // supports Synchronous Master mode
#else
    0,  // supports Synchronous Master mode
#endif
    0,  // supports Synchronous Slave mode
    1,  // supports UART Single-wire mode
    1,  // supports UART IrDA mode
    1,  // supports UART Smart Card mode
    0,  // Smart Card Clock generator
#ifdef MX_USART1_RTS_Pin
    1,  // RTS Flow Control available
#else
    0,  // RTS Flow Control available
#endif
#ifdef MX_USART1_CTS_Pin
    1,  // CTS Flow Control available
#else
    0,  // CTS Flow Control available
#endif
    1,  // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    1,  // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
#ifdef MX_USART1_RTS_Pin
    1,  // RTS Line: 0=not available, 1=available
#else
    0,  // RTS Line: 0=not available, 1=available
#endif
#ifdef MX_USART1_CTS_Pin
    1,  // CTS Line: 0=not available, 1=available
#else
    0,  // CTS Line: 0=not available, 1=available
#endif
    0,  // DTR Line: 0=not available, 1=available
    0,  // DSR Line: 0=not available, 1=available
    0,  // DCD Line: 0=not available, 1=available
    0,  // RI Line: 0=not available, 1=available
#ifdef MX_USART1_CTS_Pin
    1,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#else
    0,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#endif
    0,  // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,  // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0,  // Signal RI change event: \ref ARM_USART_EVENT_RI
  },

    USART1,
    RTE_PCLK2,

  // PINS
  {
#ifdef MX_USART1_TX_Pin
    &USART1_tx,
#else
    NULL,
#endif
#ifdef MX_USART1_RX_Pin
    &USART1_rx,
#else
    NULL,
#endif
#ifdef MX_USART1_CK_Pin
    &USART1_ck,
#else
    NULL,
#endif
#ifdef MX_USART1_RTS_Pin 
    &USART1_rts,
#else
    NULL,
#endif
#ifdef MX_USART1_CTS_Pin 
    &USART1_cts,
#else
    NULL,
#endif
    MX_USART1_REMAP_DEF,
    MX_USART1_REMAP,
  },

    USART1_IRQn,

#ifdef MX_USART1_TX_DMA_Instance
  &USART1_DMA_Tx,
#else
  NULL,
#endif
#ifdef MX_USART1_RX_DMA_Instance
  &USART1_DMA_Rx,
#else
  NULL,
#endif

  &USART1_Info,
  &USART1_TransferInfo
};
#endif

// USART2
#ifdef MX_USART2

// USART2 Run-Time Information
static USART_INFO          USART2_Info = {0};
static USART_TRANSFER_INFO USART2_TransferInfo = {0};

#ifdef MX_USART2_TX_Pin
  static USART_PIN USART2_tx = {MX_USART2_TX_GPIOx,  MX_USART2_TX_GPIO_Pin  };
#endif
#ifdef MX_USART2_RX_Pin
  static USART_PIN USART2_rx = {MX_USART2_RX_GPIOx,  MX_USART2_RX_GPIO_Pin  };
#endif
#ifdef MX_USART2_CK_Pin
  static USART_PIN USART2_ck = {MX_USART2_CK_GPIOx,  MX_USART2_CK_GPIO_Pin  };
#endif
#ifdef MX_USART2_RTS_Pin
  static USART_PIN USART2_rts = {MX_USART2_RTS_GPIOx, MX_USART2_RTS_GPIO_Pin};
#endif
#ifdef MX_USART2_CTS_Pin
  static USART_PIN USART2_cts = {MX_USART2_CTS_GPIOx, MX_USART2_CTS_GPIO_Pin};
#endif

#ifdef MX_USART2_TX_DMA_Instance
  static USART_DMA USART2_DMA_Tx = {
    MX_USART2_TX_DMA_Instance,
    MX_USART2_TX_DMA_Number,
    MX_USART2_TX_DMA_Channel,
    MX_USART2_TX_DMA_Priority
  };
#endif
#ifdef MX_USART2_RX_DMA_Instance
  static USART_DMA USART2_DMA_Rx = {
    MX_USART2_RX_DMA_Instance,
    MX_USART2_RX_DMA_Number,
    MX_USART2_RX_DMA_Channel,
    MX_USART2_RX_DMA_Priority
  };
#endif

// USART2 Resources
static const USART_RESOURCES USART2_Resources = {
  {     // Capabilities
    1,  // supports UART (Asynchronous) mode
#ifdef MX_USART2_CK_Pin
    1,  // supports Synchronous Master mode
#else
    0,  // supports Synchronous Master mode
#endif
    0,  // supports Synchronous Slave mode
    1,  // supports UART Single-wire mode
    1,  // supports UART IrDA mode
    1,  // supports UART Smart Card mode
    0,  // Smart Card Clock generator
#ifdef MX_USART2_RTS_Pin
    1,  // RTS Flow Control available
#else
    0,  // RTS Flow Control available
#endif
#ifdef MX_USART2_CTS_Pin
    1,  // CTS Flow Control available
#else
    0,  // CTS Flow Control available
#endif
    1,  // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    1,  // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
#ifdef MX_USART2_RTS_Pin
    1,  // RTS Line: 0=not available, 1=available
#else
    0,  // RTS Line: 0=not available, 1=available
#endif
#ifdef MX_USART2_CTS_Pin
    1,  // CTS Line: 0=not available, 1=available
#else
    0,  // CTS Line: 0=not available, 1=available
#endif
    0,  // DTR Line: 0=not available, 1=available
    0,  // DSR Line: 0=not available, 1=available
    0,  // DCD Line: 0=not available, 1=available
    0,  // RI Line: 0=not available, 1=available
#ifdef MX_USART2_CTS_Pin
    1,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#else
    0,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#endif
    0,  // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,  // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0,  // Signal RI change event: \ref ARM_USART_EVENT_RI
  },

    USART2,
    RTE_PCLK1,

  // PINS
  {
#ifdef MX_USART2_TX_Pin
    &USART2_tx,
#else
    NULL,
#endif
#ifdef MX_USART2_RX_Pin
    &USART2_rx,
#else
    NULL,
#endif
#ifdef MX_USART2_CK_Pin
    &USART2_ck,
#else
    NULL,
#endif
#ifdef MX_USART2_RTS_Pin 
    &USART2_rts,
#else
    NULL,
#endif
#ifdef MX_USART2_CTS_Pin 
    &USART2_cts,
#else
    NULL,
#endif
    MX_USART2_REMAP_DEF,
    MX_USART2_REMAP,
  },

    USART2_IRQn,

#ifdef MX_USART2_TX_DMA_Instance
  &USART2_DMA_Tx,
#else
  NULL,
#endif
#ifdef MX_USART2_RX_DMA_Instance
  &USART2_DMA_Rx,
#else
  NULL,
#endif

  &USART2_Info,
  &USART2_TransferInfo
};
#endif

// USART3
#ifdef MX_USART3

// USART3 Run-Time Information
static USART_INFO          USART3_Info = {0};
static USART_TRANSFER_INFO USART3_TransferInfo = {0};

#ifdef MX_USART3_TX_Pin
  static USART_PIN USART3_tx = {MX_USART3_TX_GPIOx,  MX_USART3_TX_GPIO_Pin  };
#endif
#ifdef MX_USART3_RX_Pin
  static USART_PIN USART3_rx = {MX_USART3_RX_GPIOx,  MX_USART3_RX_GPIO_Pin  };
#endif
#ifdef MX_USART3_CK_Pin
  static USART_PIN USART3_ck = {MX_USART3_CK_GPIOx,  MX_USART3_CK_GPIO_Pin  };
#endif
#ifdef MX_USART3_RTS_Pin
  static USART_PIN USART3_rts = {MX_USART3_RTS_GPIOx, MX_USART3_RTS_GPIO_Pin};
#endif
#ifdef MX_USART3_CTS_Pin
  static USART_PIN USART3_cts = {MX_USART3_CTS_GPIOx, MX_USART3_CTS_GPIO_Pin};
#endif

#ifdef MX_USART3_TX_DMA_Instance
  static USART_DMA USART3_DMA_Tx = {
    MX_USART3_TX_DMA_Instance,
    MX_USART3_TX_DMA_Number,
    MX_USART3_TX_DMA_Channel,
    MX_USART3_TX_DMA_Priority
  };
#endif
#ifdef MX_USART3_RX_DMA_Instance
  static USART_DMA USART3_DMA_Rx = {
    MX_USART3_RX_DMA_Instance,
    MX_USART3_RX_DMA_Number,
    MX_USART3_RX_DMA_Channel,
    MX_USART3_RX_DMA_Priority
  };
#endif

// USART3 Resources
static const USART_RESOURCES USART3_Resources = {
  {     // Capabilities
    1,  // supports UART (Asynchronous) mode
#ifdef MX_USART3_CK_Pin
    1,  // supports Synchronous Master mode
#else
    0,  // supports Synchronous Master mode
#endif
    0,  // supports Synchronous Slave mode
    1,  // supports UART Single-wire mode
    1,  // supports UART IrDA mode
    1,  // supports UART Smart Card mode
    0,  // Smart Card Clock generator
#ifdef MX_USART3_RTS_Pin
    1,  // RTS Flow Control available
#else
    0,  // RTS Flow Control available
#endif
#ifdef MX_USART3_CTS_Pin
    1,  // CTS Flow Control available
#else
    0,  // CTS Flow Control available
#endif
    1,  // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    1,  // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
#ifdef MX_USART3_RTS_Pin
    1,  // RTS Line: 0=not available, 1=available
#else
    0,  // RTS Line: 0=not available, 1=available
#endif
#ifdef MX_USART3_CTS_Pin
    1,  // CTS Line: 0=not available, 1=available
#else
    0,  // CTS Line: 0=not available, 1=available
#endif
    0,  // DTR Line: 0=not available, 1=available
    0,  // DSR Line: 0=not available, 1=available
    0,  // DCD Line: 0=not available, 1=available
    0,  // RI Line: 0=not available, 1=available
#ifdef MX_USART3_CTS_Pin
    1,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#else
    0,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#endif
    0,  // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,  // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0,  // Signal RI change event: \ref ARM_USART_EVENT_RI
  },

    USART3,
    RTE_PCLK1,

  // PINS
  {
#ifdef MX_USART3_TX_Pin
    &USART3_tx,
#else
    NULL,
#endif
#ifdef MX_USART3_RX_Pin
    &USART3_rx,
#else
    NULL,
#endif
#ifdef MX_USART3_CK_Pin
    &USART3_ck,
#else
    NULL,
#endif
#ifdef MX_USART3_RTS_Pin 
    &USART3_rts,
#else
    NULL,
#endif
#ifdef MX_USART3_CTS_Pin 
    &USART3_cts,
#else
    NULL,
#endif
    MX_USART3_REMAP_DEF,
    MX_USART3_REMAP,
  },

    USART3_IRQn,

#ifdef MX_USART3_TX_DMA_Instance
  &USART3_DMA_Tx,
#else
  NULL,
#endif
#ifdef MX_USART3_RX_DMA_Instance
  &USART3_DMA_Rx,
#else
  NULL,
#endif

  &USART3_Info,
  &USART3_TransferInfo
};
#endif

// UART4
#ifdef MX_UART4

// UART4 Run-Time Information
static USART_INFO          UART4_Info = {0};
static USART_TRANSFER_INFO UART4_TransferInfo = {0};

#ifdef MX_UART4_TX_Pin
  static USART_PIN UART4_tx = {MX_UART4_TX_GPIOx,  MX_UART4_TX_GPIO_Pin};
#endif
#ifdef MX_UART4_RX_Pin
  static USART_PIN UART4_rx = {MX_UART4_RX_GPIOx,  MX_UART4_RX_GPIO_Pin};
#endif

#ifdef MX_UART4_TX_DMA_Instance
  static USART_DMA UART4_DMA_Tx = {
    MX_UART4_TX_DMA_Instance,
    MX_UART4_TX_DMA_Number,
    MX_UART4_TX_DMA_Channel,
    MX_UART4_TX_DMA_Priority
  };
#endif
#ifdef MX_UART4_RX_DMA_Instance
  static USART_DMA UART4_DMA_Rx = {
    MX_UART4_RX_DMA_Instance,
    MX_UART4_RX_DMA_Number,
    MX_UART4_RX_DMA_Channel,
    MX_UART4_RX_DMA_Priority
  };
#endif

// UART4 Resources
static const USART_RESOURCES USART4_Resources = {
  {     // Capabilities
    1,  // supports UART (Asynchronous) mode
    0,  // supports Synchronous Master mode
    0,  // supports Synchronous Slave mode
    1,  // supports UART Single-wire mode
    1,  // supports UART IrDA mode
    0,  // supports UART Smart Card mode
    0,  // Smart Card Clock generator
    0,  // RTS Flow Control available
    0,  // CTS Flow Control available
    1,  // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    1,  // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
    0,  // RTS Line: 0=not available, 1=available
    0,  // CTS Line: 0=not available, 1=available
    0,  // DTR Line: 0=not available, 1=available
    0,  // DSR Line: 0=not available, 1=available
    0,  // DCD Line: 0=not available, 1=available
    0,  // RI Line: 0=not available, 1=available
    0,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
    0,  // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,  // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0,  // Signal RI change event: \ref ARM_USART_EVENT_RI
  },

    UART4,
    RTE_PCLK1,

  // PINS
  {
#ifdef MX_UART4_TX_Pin
    &UART4_tx,
#else
    NULL,
#endif
#ifdef MX_UART4_RX_Pin
    &UART4_rx,
#else
    NULL,
#endif
    NULL,
    NULL,
    NULL,
    MX_UART4_REMAP_DEF,
    MX_UART4_REMAP,
  },

    UART4_IRQn,

#ifdef MX_UART4_TX_DMA_Instance
  &UART4_DMA_Tx,
#else
  NULL,
#endif
#ifdef MX_UART4_RX_DMA_Instance
  &UART4_DMA_Rx,
#else
  NULL,
#endif

  &UART4_Info,
  &UART4_TransferInfo
};
#endif

// UART5
#ifdef MX_UART5

// UART5 Run-Time Information
static USART_INFO          UART5_Info = {0};
static USART_TRANSFER_INFO UART5_TransferInfo = {0};

#ifdef MX_UART5_TX_Pin
  static USART_PIN UART5_tx = {MX_UART5_TX_GPIOx,  MX_UART5_TX_GPIO_Pin};
#endif
#ifdef MX_UART5_RX_Pin
  static USART_PIN UART5_rx = {MX_UART5_RX_GPIOx,  MX_UART5_RX_GPIO_Pin};
#endif

#ifdef MX_UART5_TX_DMA_Instance
  static USART_DMA UART5_DMA_Tx = {
    MX_UART5_TX_DMA_Instance,
    MX_UART5_TX_DMA_Number,
    MX_UART5_TX_DMA_Channel,
    MX_UART5_TX_DMA_Priority
  };
#endif
#ifdef MX_UART5_RX_DMA_Instance
  static USART_DMA UART5_DMA_Rx = {
    MX_UART5_RX_DMA_Instance,
    MX_UART5_RX_DMA_Number,
    MX_UART5_RX_DMA_Channel,
    MX_UART5_RX_DMA_Priority
  };
#endif

// UART5 Resources
static const USART_RESOURCES USART5_Resources = {
  {     // Capabilities
    1,  // supports UART (Asynchronous) mode
    0,  // supports Synchronous Master mode
    0,  // supports Synchronous Slave mode
    1,  // supports UART Single-wire mode
    1,  // supports UART IrDA mode
    0,  // supports UART Smart Card mode
    0,  // Smart Card Clock generator
    0,  // RTS Flow Control available
    0,  // CTS Flow Control available
    1,  // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    1,  // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
    0,  // RTS Line: 0=not available, 1=available
    0,  // CTS Line: 0=not available, 1=available
    0,  // DTR Line: 0=not available, 1=available
    0,  // DSR Line: 0=not available, 1=available
    0,  // DCD Line: 0=not available, 1=available
    0,  // RI Line: 0=not available, 1=available
    0,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
    0,  // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,  // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0,  // Signal RI change event: \ref ARM_USART_EVENT_RI
  },

    UART5,
    RTE_PCLK1,

  // PINS
  {
#ifdef MX_UART5_TX_Pin
    &UART5_tx,
#else
    NULL,
#endif
#ifdef MX_UART5_RX_Pin
    &UART5_rx,
#else
    NULL,
#endif
    NULL,
    NULL,
    NULL,
    MX_UART5_REMAP_DEF,
    MX_UART5_REMAP,
  },

    UART5_IRQn,

#ifdef MX_UART5_TX_DMA_Instance
  &UART5_DMA_Tx,
#else
  NULL,
#endif
#ifdef MX_UART5_RX_DMA_Instance
  &UART5_DMA_Rx,
#else
  NULL,
#endif

  &UART5_Info,
  &UART5_TransferInfo
};
#endif

// Auxiliary functions
/**
  \fn          void USART_PeripheralReset (const USART_TypeDef *usart)
  \brief       USART Reset
*/
static void USART_PeripheralReset (const USART_TypeDef *usart) {

  if (usart == USART1) { RCC->APB2RSTR |= RCC_APB2RSTR_USART1RST; }
  if (usart == USART2) { RCC->APB1RSTR |= RCC_APB1RSTR_USART2RST; }
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
  if (usart == USART3) { RCC->APB1RSTR |= RCC_APB1RSTR_USART3RST; }
#endif
#if defined(STM32F10X_HD) || defined(STM32F10X_CL) || defined(STM32F10X_XL) || defined(STM32F10X_HD_VL)
  if (usart == UART4)  { RCC->APB1RSTR |= RCC_APB1RSTR_UART4RST;  }
  if (usart == UART5)  { RCC->APB1RSTR |= RCC_APB1RSTR_UART5RST;  }
#endif

  __NOP(); __NOP(); __NOP(); __NOP();

  if (usart == USART1) { RCC->APB2RSTR &= ~RCC_APB2RSTR_USART1RST; }
  if (usart == USART2) { RCC->APB1RSTR &= ~RCC_APB1RSTR_USART2RST; }
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
  if (usart == USART3) { RCC->APB1RSTR &= ~RCC_APB1RSTR_USART3RST; }
#endif
#if defined(STM32F10X_HD) || defined(STM32F10X_CL) || defined(STM32F10X_XL) || defined(STM32F10X_HD_VL)
  if (usart == UART4)  { RCC->APB1RSTR &= ~RCC_APB1RSTR_UART4RST;  }
  if (usart == UART5)  { RCC->APB1RSTR &= ~RCC_APB1RSTR_UART5RST;  }
#endif
}

// Function prototypes
void USART_IRQHandler (const USART_RESOURCES *usart);
#ifdef __USART_DMA_TX
void USART_TX_DMA_Complete(const USART_RESOURCES *usart);
#endif
#ifdef __USART_DMA_RX
void USART_RX_DMA_Complete(const USART_RESOURCES *usart);
#endif
static int32_t USART_Receive (      void            *data,
                                    uint32_t         num,
                              const USART_RESOURCES *usart);

// USART Driver functions

/**
  \fn          ARM_DRIVER_VERSION USARTx_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION USARTx_GetVersion (void) {
  return usart_driver_version;
}

/**
  \fn          ARM_USART_CAPABILITIES USART_GetCapabilities (const USART_RESOURCES *usart)
  \brief       Get driver capabilities
  \param[in]   usart     Pointer to USART resources
  \return      \ref ARM_USART_CAPABILITIES
*/
static ARM_USART_CAPABILITIES USART_GetCapabilities (const USART_RESOURCES *usart) {
  return usart->capabilities;
}

/**
  \fn          int32_t USART_Initialize (      ARM_USART_SignalEvent_t  cb_event
                                         const USART_RESOURCES         *usart)
  \brief       Initialize USART Interface.
  \param[in]   cb_event  Pointer to \ref ARM_USART_SignalEvent
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Initialize (      ARM_USART_SignalEvent_t  cb_event,
                                 const USART_RESOURCES         *usart) {

  if (usart->info->flags & USART_FLAG_INITIALIZED) {
    // Driver is already initialized
    return ARM_DRIVER_OK;
  }

  // Initialize USART Run-time Resources
  usart->info->cb_event = cb_event;

  usart->info->status.tx_busy          = 0U;
  usart->info->status.rx_busy          = 0U;
  usart->info->status.tx_underflow     = 0U;
  usart->info->status.rx_overflow      = 0U;
  usart->info->status.rx_break         = 0U;
  usart->info->status.rx_framing_error = 0U;
  usart->info->status.rx_parity_error  = 0U;

  usart->info->mode        = 0U;
  usart->xfer->send_active = 0U;

  // Clear transfer information
  memset(usart->xfer, 0, sizeof(USART_TRANSFER_INFO));

  // Setup pin remap
  GPIO_AFConfigure(usart->io.afio);

  // Enable TX pin port clock
  if (usart->io.tx) {
    GPIO_PortClock (usart->io.tx->port, true);
  }

  // Enable RX pin port clock
  if (usart->io.rx) {
    GPIO_PortClock (usart->io.rx->port, true);
  }

  // Enable CK pin port clock
  if (usart->io.ck) {
    GPIO_PortClock (usart->io.ck->port, true);
  }

  // Enable RTS pin port clock
  if (usart->io.rts) {
    GPIO_PortClock (usart->io.rts->port, true);
  }

  // Enable CTS pin port clock
  if (usart->io.cts) {
    GPIO_PortClock (usart->io.cts->port, true);
  }

  usart->info->flags = USART_FLAG_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Uninitialize (const USART_RESOURCES *usart)
  \brief       De-initialize USART Interface.
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Uninitialize (const USART_RESOURCES *usart) {

  // Unconfigure USART pins
  if (usart->io.tx)  GPIO_PinConfigure(usart->io.tx->port,  usart->io.tx->pin,  GPIO_IN_ANALOG, GPIO_MODE_INPUT);
  if (usart->io.rx)  GPIO_PinConfigure(usart->io.rx->port,  usart->io.rx->pin,  GPIO_IN_ANALOG, GPIO_MODE_INPUT);
  if (usart->io.ck)  GPIO_PinConfigure(usart->io.ck->port,  usart->io.ck->pin,  GPIO_IN_ANALOG, GPIO_MODE_INPUT);
  if (usart->io.rts) GPIO_PinConfigure(usart->io.rts->port, usart->io.rts->pin, GPIO_IN_ANALOG, GPIO_MODE_INPUT);
  if (usart->io.cts) GPIO_PinConfigure(usart->io.cts->port, usart->io.cts->pin, GPIO_IN_ANALOG, GPIO_MODE_INPUT);

  // Unconfigure pin remap
  GPIO_AFConfigure(usart->io.afio_def);

  // Reset USART status flags
  usart->info->flags = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_PowerControl (ARM_POWER_STATE state)
  \brief       Control USART Interface Power.
  \param[in]   state  Power state
  \param[in]   usart  Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_PowerControl (      ARM_POWER_STATE  state,
                                   const USART_RESOURCES *usart) {

  switch (state) {
    case ARM_POWER_OFF:
      // Peripheral reset
      USART_PeripheralReset (usart->reg);

      NVIC_DisableIRQ (usart->irq_num);

#ifdef __USART_DMA
      if (usart->dma_rx) {
        // Deinitialize DMA
        DMA_ChannelUninitialize (usart->dma_rx->dma_num, usart->dma_rx->ch_num);
      }

      if (usart->dma_tx) {
        // Deinitialize DMA
        DMA_ChannelUninitialize (usart->dma_tx->dma_num, usart->dma_tx->ch_num);
      }
#endif

      // Disable USART clock
      if (usart->reg == USART1) { RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN; }
      if (usart->reg == USART2) { RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN; }
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
      if (usart->reg == USART3) { RCC->APB1ENR &= ~RCC_APB1ENR_USART3EN; }
#endif
#if defined(STM32F10X_HD) || defined(STM32F10X_CL) || defined(STM32F10X_XL) || defined(STM32F10X_HD_VL)
      if (usart->reg == UART4)  { RCC->APB1ENR &= ~RCC_APB1ENR_UART4EN;  }
      if (usart->reg == UART5)  { RCC->APB1ENR &= ~RCC_APB1ENR_UART5EN;  }
#endif

      // Clear Status flags
      usart->info->status.tx_busy          = 0U;
      usart->info->status.rx_busy          = 0U;
      usart->info->status.tx_underflow     = 0U;
      usart->info->status.rx_overflow      = 0U;
      usart->info->status.rx_break         = 0U;
      usart->info->status.rx_framing_error = 0U;
      usart->info->status.rx_parity_error  = 0U;
      usart->xfer->send_active             = 0U;

      usart->info->flags &= ~USART_FLAG_POWERED;
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      if ((usart->info->flags & USART_FLAG_INITIALIZED) == 0U) {
        return ARM_DRIVER_ERROR;
      }
      if ((usart->info->flags & USART_FLAG_POWERED)     != 0U) {
        return ARM_DRIVER_OK;
      }

      // Clear Status flags
      usart->info->status.tx_busy          = 0U;
      usart->info->status.rx_busy          = 0U;
      usart->info->status.tx_underflow     = 0U;
      usart->info->status.rx_overflow      = 0U;
      usart->info->status.rx_break         = 0U;
      usart->info->status.rx_framing_error = 0U;
      usart->info->status.rx_parity_error  = 0U;

      usart->xfer->send_active             = 0U;
      usart->xfer->def_val                 = 0U;
      usart->xfer->sync_mode               = 0U;
      usart->xfer->break_flag              = 0U;
      usart->info->mode                    = 0U;
      usart->info->flow_control            = 0U;

      usart->info->flags = USART_FLAG_POWERED | USART_FLAG_INITIALIZED;

      // Enable USART clock
      if (usart->reg == USART1) { RCC->APB2ENR |= RCC_APB2ENR_USART1EN; }
      if (usart->reg == USART2) { RCC->APB1ENR |= RCC_APB1ENR_USART2EN; }
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
      if (usart->reg == USART3) { RCC->APB1ENR |= RCC_APB1ENR_USART3EN; }
#endif
#if defined(STM32F10X_HD) || defined(STM32F10X_CL) || defined(STM32F10X_XL) || defined(STM32F10X_HD_VL)
      if (usart->reg == UART4)  { RCC->APB1ENR |= RCC_APB1ENR_UART4EN;  }
      if (usart->reg == UART5)  { RCC->APB1ENR |= RCC_APB1ENR_UART5EN;  }
#endif

      // Clear and Enable USART IRQ
      NVIC_ClearPendingIRQ(usart->irq_num);
      NVIC_EnableIRQ(usart->irq_num);

#ifdef __USART_DMA
      if (usart->dma_rx) {
         // Initialize DMA
         DMA_ChannelInitialize (usart->dma_rx->dma_num, usart->dma_rx->ch_num);
      }

      if (usart->dma_tx) {
        // Initialize DMA
        DMA_ChannelInitialize (usart->dma_tx->dma_num, usart->dma_tx->ch_num);
      }
#endif

      // Peripheral reset
      USART_PeripheralReset (usart->reg);
    break;
    default: return ARM_DRIVER_ERROR_UNSUPPORTED;
  }
  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Send (const void            *data,
                                         uint32_t         num,
                                   const USART_RESOURCES *usart)
  \brief       Start sending data to USART transmitter.
  \param[in]   data  Pointer to buffer with data to send to USART transmitter
  \param[in]   num   Number of data items to send
  \param[in]   usart Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Send (const void            *data,
                                 uint32_t         num,
                           const USART_RESOURCES *usart) {
  int32_t  stat;

#ifdef __USART_DMA_TX
  uint32_t cfg, cr1;
#endif

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  if (usart->xfer->send_active != 0U) {
    // Send is not completed yet
    return ARM_DRIVER_ERROR_BUSY;
  }

  // Set Send active flag
  usart->xfer->send_active = 1U;

  // Save transmit buffer info
  usart->xfer->tx_buf = (uint8_t *)data;
  usart->xfer->tx_num = num;
  usart->xfer->tx_cnt = 0U;

#ifdef __USART_DMA_TX
  cfg = DMA_MEMORY_INCREMENT;
#endif

  // Synchronous mode
  if (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) {
    if (usart->xfer->sync_mode == 0U) {
      usart->xfer->sync_mode = USART_SYNC_MODE_TX;
      // Start dummy reads
      stat = USART_Receive (&usart->xfer->dump_val, num, usart);
      if (stat == ARM_DRIVER_ERROR_BUSY) { return ARM_DRIVER_ERROR_BUSY; }

#ifdef __USART_DMA_TX
    } else {
      if (usart->xfer->sync_mode == USART_SYNC_MODE_RX) {
        // Dummy DMA writes (do not increment source address)
        cfg = 0;
      }
#endif
    }
  }

#ifdef __USART_DMA_TX
  // DMA mode
  if (usart->dma_tx) {
    // Configure and enable tx DMA channel
    cfg |= ((usart->dma_tx->priority << DMA_PRIORITY_POS) & DMA_PRIORITY_MASK) |
             DMA_READ_MEMORY                |
             DMA_TRANSFER_COMPLETE_INTERRUPT;

    cr1 = usart->reg->CR1;
    if (((cr1 & USART_CR1_M) != 0U) && ((cr1 & USART_CR1_PCE) == 0U)) {
      // 9-bit data frame, no parity
      cfg |= DMA_PERIPHERAL_DATA_16BIT | DMA_MEMORY_DATA_16BIT;
    } else {
      // 8-bit data frame
      cfg |= DMA_PERIPHERAL_DATA_8BIT | DMA_MEMORY_DATA_8BIT;
    }

    DMA_ChannelConfigure(usart->dma_tx->instance,
                         cfg,
                         (uint32_t)(&usart->reg->DR),
                         (uint32_t)(uint32_t)data,
                         num);
    DMA_ChannelEnable(usart->dma_tx->instance);

    // DMA Enable transmitter
    usart->reg->CR3 |= USART_CR3_DMAT;
  } else
#endif
  // Interrupt mode
  {
    // TXE interrupt enable
    usart->reg->CR1 |= USART_CR1_TXEIE;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Receive (      void            *data,
                                            uint32_t         num,
                                      const USART_RESOURCES *usart)
  \brief       Start receiving data from USART receiver.
  \param[out]  data  Pointer to buffer for data to receive from USART receiver
  \param[in]   num   Number of data items to receive
  \param[in]   usart Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Receive (      void            *data,
                                    uint32_t         num,
                              const USART_RESOURCES *usart) {

  int32_t  stat;

#ifdef __USART_DMA_RX
  uint32_t cfg, cr1;
#endif

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  // Check if receiver is busy
  if (usart->info->status.rx_busy == 1U) {
    return ARM_DRIVER_ERROR_BUSY;
  }

  // Disable RXNE Interrupt
  usart->reg->CR1 &= ~USART_CR1_RXNEIE;

  // Save number of data to be received
  usart->xfer->rx_num = num;

  // Clear RX statuses
  usart->info->status.rx_break          = 0U;
  usart->info->status.rx_framing_error  = 0U;
  usart->info->status.rx_overflow       = 0U;
  usart->info->status.rx_parity_error   = 0U;

  // Save receive buffer info
  usart->xfer->rx_buf = (uint8_t *)data;
  usart->xfer->rx_cnt =  0U;

  // Set RX busy flag
  usart->info->status.rx_busy = 1U;

#ifdef __USART_DMA_RX

  cfg = DMA_MEMORY_INCREMENT;

  // Synchronous mode
  if (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) {
    if (usart->xfer->sync_mode == USART_SYNC_MODE_TX) {
      // Dummy DMA reads (do not increment destination address)
      cfg = 0U;
    }
  }

  // DMA mode
  if (usart->dma_rx) {
    // Configure and enable rx DMA channel
    cfg |= ((usart->dma_rx->priority << DMA_PRIORITY_POS) & DMA_PRIORITY_MASK) |
             DMA_PERIPHERAL_TO_MEMORY       |
             DMA_TRANSFER_COMPLETE_INTERRUPT;

    cr1 = usart->reg->CR1;
    if (((cr1 & USART_CR1_M) != 0U) && ((cr1 & USART_CR1_PCE) == 0U)) {
      // 9-bit data frame, no parity
      cfg |= DMA_PERIPHERAL_DATA_16BIT | DMA_MEMORY_DATA_16BIT;
    } else {
      // 8-bit data frame
      cfg |= DMA_PERIPHERAL_DATA_8BIT | DMA_MEMORY_DATA_8BIT;
    }

    DMA_ChannelConfigure(usart->dma_rx->instance,
                         cfg,
                         (uint32_t)(&usart->reg->DR),
                         (uint32_t)(uint32_t)data,
                         num);
    DMA_ChannelEnable(usart->dma_rx->instance);

    usart->reg->CR3 |= USART_CR3_DMAR;
    // Enable IDLE interrupt
    usart->reg->CR1 |= USART_CR1_IDLEIE;
  } else
#endif
  {
    // Enable RXNE and IDLE interrupt
    usart->reg->CR1 |= (USART_CR1_IDLEIE | USART_CR1_RXNEIE);
  }  

  // Synchronous mode
  if (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) {
    if (usart->xfer->sync_mode == 0U) {
      usart->xfer->sync_mode = USART_SYNC_MODE_RX;
      // Send dummy data
      stat = USART_Send (&usart->xfer->def_val, num, usart);
      if (stat == ARM_DRIVER_ERROR_BUSY) { return ARM_DRIVER_ERROR_BUSY; }
    }
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Transfer (const void             *data_out,
                                             void             *data_in,
                                             uint32_t          num,
                                       const USART_RESOURCES  *usart)
  \brief       Start sending/receiving data to/from USART transmitter/receiver.
  \param[in]   data_out  Pointer to buffer with data to send to USART transmitter
  \param[out]  data_in   Pointer to buffer for data to receive from USART receiver
  \param[in]   num       Number of data items to transfer
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Transfer (const void             *data_out,
                                     void             *data_in,
                                     uint32_t          num,
                               const USART_RESOURCES  *usart) {
  int32_t status;

  if ((data_out == NULL) || (data_in == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured
    return ARM_DRIVER_ERROR;
  }

  if (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) {

    // Set xfer mode
    usart->xfer->sync_mode = USART_SYNC_MODE_TX_RX;

    // Receive
    status = USART_Receive (data_in, num, usart);
    if (status != ARM_DRIVER_OK) { return status; }

    // Send
    status = USART_Send (data_out, num, usart);
    if (status != ARM_DRIVER_OK) { return status; }

  } else {
    // Only in synchronous mode
    return ARM_DRIVER_ERROR;
  }
  return ARM_DRIVER_OK;
}

/**
  \fn          uint32_t USART_GetTxCount (const USART_RESOURCES *usart)
  \brief       Get transmitted data count.
  \param[in]   usart     Pointer to USART resources
  \return      number of data items transmitted
*/
static uint32_t USART_GetTxCount (const USART_RESOURCES *usart) {

#ifdef __USART_DMA_TX
  if (usart->dma_tx) {
    return (usart->xfer->tx_num - DMA_ChannelTransferItemCount(usart->dma_tx->instance));
  } else
#endif
  {
    return usart->xfer->tx_cnt;
  }
}

/**
  \fn          uint32_t USART_GetRxCount (const USART_RESOURCES *usart)
  \brief       Get received data count.
  \param[in]   usart     Pointer to USART resources
  \return      number of data items received
*/
static uint32_t USART_GetRxCount (const USART_RESOURCES *usart) {

#ifdef __USART_DMA_RX
  if (usart->dma_rx) {
    return (usart->xfer->rx_num - DMA_ChannelTransferItemCount(usart->dma_rx->instance));
  } else
#endif
  {
    return usart->xfer->rx_cnt;
  }
}

/**
  \fn          int32_t USART_Control (      uint32_t          control,
                                            uint32_t          arg,
                                      const USART_RESOURCES  *usart)
  \brief       Control USART Interface.
  \param[in]   control  Operation
  \param[in]   arg      Argument of operation (optional)
  \param[in]   usart    Pointer to USART resources
  \return      common \ref execution_status and driver specific \ref usart_execution_status
*/
static int32_t USART_Control (      uint32_t          control,
                                    uint32_t          arg,
                              const USART_RESOURCES  *usart) {
  uint32_t val, mode, flow_control, br, i;
  uint32_t cr1, cr2, cr3;

  if ((usart->info->flags & USART_FLAG_POWERED) == 0U) {
    // USART not powered
    return ARM_DRIVER_ERROR;
  }

  cr1  = 0U;
  cr2  = 0U;
  cr3  = 0U;

  switch (control & ARM_USART_CONTROL_Msk) {
     // Control break
    case ARM_USART_CONTROL_BREAK:
      if (arg) {
        if (usart->xfer->send_active != 0U) { return ARM_DRIVER_ERROR_BUSY; }

        // Set Send active and Break flag
        usart->xfer->send_active = 1U;
        usart->xfer->break_flag  = 1U;

        // Enable TX interrupt and send break
        usart->reg->CR1 |=  USART_CR1_TXEIE | USART_CR1_SBK;
      } else {
          if (usart->xfer->break_flag) {
            // Disable TX interrupt
            usart->reg->CR1 &= ~USART_CR1_TXEIE;

            // Clear break and Send Active flag
            usart->xfer->break_flag  = 0U;
            usart->xfer->send_active = 0U;
          }
      }
      return ARM_DRIVER_OK;

    // Abort Send
    case ARM_USART_ABORT_SEND:
      // Disable TX and TC interrupt
      usart->reg->CR1 &= ~(USART_CR1_TXEIE | USART_CR1_TCIE);

      // If DMA mode - disable DMA channel
      if ((usart->dma_tx != NULL) && (usart->xfer->send_active != 0)) {
        // DMA disable transmitter
        usart->reg->CR3 &= ~USART_CR3_DMAT;

        // Abort TX DMA transfer
        DMA_ChannelDisable (usart->dma_tx->instance);
      }

      // Clear break flag
      usart->xfer->break_flag = 0U;

      // Clear Send active flag
      usart->xfer->send_active = 0U;
      return ARM_DRIVER_OK;

    // Abort receive
    case ARM_USART_ABORT_RECEIVE:
      // Disable RX interrupt
      usart->reg->CR1 &= ~USART_CR1_RXNEIE;

      // If DMA mode - disable DMA channel
      if ((usart->dma_rx != NULL) && (usart->info->status.rx_busy != 0)) {
        // DMA disable Receiver
        usart->reg->CR3 &= ~USART_CR3_DMAR;

        // Abort RX DMA transfer
        DMA_ChannelDisable (usart->dma_rx->instance);
      }

      // Clear RX busy status
      usart->info->status.rx_busy = 0U;

      return ARM_DRIVER_OK;

    // Abort transfer
    case ARM_USART_ABORT_TRANSFER:
      // Disable TX, TC and RX interrupt
      usart->reg->CR1 &= ~(USART_CR1_TXEIE | USART_CR1_TCIE | USART_CR1_RXNEIE);

      // If DMA mode - disable DMA channel
      if ((usart->dma_tx != NULL) && (usart->xfer->send_active != 0U)) {
        // DMA disable transmitter
        usart->reg->CR3 &= ~USART_CR3_DMAT;

        // Abort TX DMA transfer
        DMA_ChannelDisable (usart->dma_tx->instance);
      }

      // If DMA mode - disable DMA channel
      if ((usart->dma_rx != NULL) && (usart->info->status.rx_busy != 0U)) {
        // DMA disable Receiver
        usart->reg->CR3 &= ~USART_CR3_DMAR;

        // Abort RX DMA transfer
        DMA_ChannelDisable (usart->dma_rx->instance);
      }

      // Clear busy statuses
      usart->info->status.rx_busy = 0U;
      usart->xfer->send_active    = 0U;
      return ARM_DRIVER_OK;

    // Control TX
    case ARM_USART_CONTROL_TX:
      // Check if TX pin available
      if (usart->io.tx == NULL) { return ARM_DRIVER_ERROR; }
      if (arg) {
        if (usart->info->mode != ARM_USART_MODE_SMART_CARD) {
          // USART TX pin function selected
          GPIO_PinConfigure(usart->io.tx->port, usart->io.tx->pin, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
        }
        usart->info->flags |= USART_FLAG_TX_ENABLED;

        // Transmitter enable
        usart->reg->CR1 |= USART_CR1_TE;
      } else {
        // Transmitter disable
        usart->reg->CR1 &= ~USART_CR1_TE;

        usart->info->flags &= ~USART_FLAG_TX_ENABLED;

        if (usart->info->mode != ARM_USART_MODE_SMART_CARD) {
          // GPIO pin function selected
          if (usart->io.tx) {
            GPIO_PinConfigure(usart->io.tx->port, usart->io.tx->pin, GPIO_IN_ANALOG, GPIO_MODE_INPUT);
          }
        }
      }
      return ARM_DRIVER_OK;

    // Control RX
    case ARM_USART_CONTROL_RX:
      // Check if RX line available
      if (usart->io.rx == NULL) { return ARM_DRIVER_ERROR; }
      if (arg) {
        if ((usart->info->mode != ARM_USART_MODE_SMART_CARD)   &&
            (usart->info->mode != ARM_USART_MODE_SINGLE_WIRE )) {
          // USART RX pin function selected
          GPIO_PinConfigure(usart->io.rx->port, usart->io.rx->pin, GPIO_IN_FLOATING, GPIO_MODE_INPUT);
        }
        usart->info->flags |= USART_FLAG_RX_ENABLED;

        // Enable Error interrupt,
        usart->reg->CR3 |= USART_CR3_EIE;

        // Break detection interrupt enable
        usart->reg->CR2 |= USART_CR2_LBDIE;

        // Enable Idle line interrupt
        usart->reg->CR1 |= USART_CR1_IDLEIE;

        if (((usart->info->status.rx_busy != 0U) && (usart->dma_rx != NULL)) == false) {
          usart->reg->CR1 |= USART_CR1_RXNEIE;
        }

        // Receiver enable
        usart->reg->CR1 |= USART_CR1_RE;

      } else {
        // Receiver disable
        usart->reg->CR1 &= ~USART_CR1_RE;

        usart->info->flags &= ~USART_FLAG_RX_ENABLED;

        if ((usart->info->mode != ARM_USART_MODE_SMART_CARD)   &&
            (usart->info->mode != ARM_USART_MODE_SINGLE_WIRE )) {
          // GPIO pin function selected
          if (usart->io.rx) {
            GPIO_PinConfigure(usart->io.rx->port, usart->io.rx->pin, GPIO_IN_ANALOG, GPIO_MODE_INPUT);
          }
        }
      }
      return ARM_DRIVER_OK;
    default: break;
  }

  // Check if busy
  if ((usart->info->status.rx_busy != 0U) || (usart->xfer->send_active != 0U)) {
    return ARM_DRIVER_ERROR_BUSY;
  }

  if (((usart->reg->CR1 & USART_CR1_TE) != 0U) && ((usart->reg->SR & USART_SR_TC) == 0U)) {
    return ARM_DRIVER_ERROR_BUSY;
  }

  switch (control & ARM_USART_CONTROL_Msk) {
    case ARM_USART_MODE_ASYNCHRONOUS:
      mode = ARM_USART_MODE_ASYNCHRONOUS;
      break;
    case ARM_USART_MODE_SYNCHRONOUS_MASTER:
      if (usart->capabilities.synchronous_master) {
        // Enable Clock pin
        cr2 |= USART_CR2_CLKEN;

        // Enable last bit clock pulse
        cr2 |= USART_CR2_LBCL;
      } else { return ARM_USART_ERROR_MODE; }
      mode = ARM_USART_MODE_SYNCHRONOUS_MASTER;
      break;
    case ARM_USART_MODE_SYNCHRONOUS_SLAVE:
      return ARM_USART_ERROR_MODE;
    case ARM_USART_MODE_SINGLE_WIRE:
      // Enable Half duplex
      cr3 |= USART_CR3_HDSEL;
      mode = ARM_USART_MODE_SINGLE_WIRE;
      break;
    case ARM_USART_MODE_IRDA:
      // Enable IrDA mode
      cr3 |= USART_CR3_IREN;
      mode = ARM_USART_MODE_IRDA;
      break;
    case ARM_USART_MODE_SMART_CARD:
      if (usart->capabilities.smart_card) {
        // Enable Smart card mode
        cr3 |= USART_CR3_SCEN;
      } else { return ARM_USART_ERROR_MODE; }
      mode = ARM_USART_MODE_SMART_CARD;
      break;

    // Default TX value
    case ARM_USART_SET_DEFAULT_TX_VALUE:
      usart->xfer->def_val = (uint16_t)arg;
      return ARM_DRIVER_OK;

    // IrDA pulse
    case ARM_USART_SET_IRDA_PULSE:
      if (usart->info->mode == ARM_USART_MODE_IRDA) {
        if (arg != 0U) {
          // IrDa low-power
          usart->reg->CR3 |= USART_CR3_IRLP;

          // Get clock
          val = usart->pclk;

          // Calculate period in ns
          val = 1000000000U / val;
          for (i = 1U; i < 256U; i++) {
            if ((val * i) > arg) { break; }
          }
          if (i == 256U) { return ARM_DRIVER_ERROR; }
          usart->reg->GTPR = (usart->reg->GTPR & ~USART_GTPR_PSC) | i;
        }
      } else { return ARM_DRIVER_ERROR; }
      return ARM_DRIVER_OK;

    // SmartCard guard time
    case ARM_USART_SET_SMART_CARD_GUARD_TIME:
      if (usart->info->mode == ARM_USART_MODE_SMART_CARD) {
        if (arg > 255U) return ARM_DRIVER_ERROR;

        usart->reg->GTPR = (usart->reg->GTPR & ~USART_GTPR_GT) | arg;
      } else { return ARM_DRIVER_ERROR; }
      return ARM_DRIVER_OK;

    // SmartCard clock
    case ARM_USART_SET_SMART_CARD_CLOCK:
      if (usart->info->mode == ARM_USART_MODE_SMART_CARD) {
        // Get clock
        val = usart->pclk;

        // Calculate period in ns
        val = 1000000000U / val;
        for (i = 1U; i <64U; i++) {
          // if in +-2% tolerance
          if (((val * i * 2U * 100U) < (arg * 102U)) &&
              ((val * i * 2U * 100U) > (arg * 98U))    ) {
            break;
          }
        }
        if (i == 64U) { return ARM_DRIVER_ERROR; }

        usart->reg->GTPR = (usart->reg->GTPR & ~USART_GTPR_PSC) | i;
      } else { return ARM_DRIVER_ERROR; }
      return ARM_DRIVER_OK;

    // SmartCard NACK
    case ARM_USART_CONTROL_SMART_CARD_NACK:
      if (usart->info->mode == ARM_USART_MODE_SMART_CARD) {
        // SmartCard NACK Enable
        if (arg != 0U) { usart->reg->CR3 |= USART_CR3_NACK; }
      } else { return ARM_DRIVER_ERROR; }
      return ARM_DRIVER_OK;

    // Unsupported command
    default: { return ARM_DRIVER_ERROR_UNSUPPORTED; }
  }

  // USART Data bits
  switch (control & ARM_USART_DATA_BITS_Msk) {
    case ARM_USART_DATA_BITS_7:
      if ((control & ARM_USART_PARITY_Msk) == ARM_USART_PARITY_NONE) {
        return ARM_USART_ERROR_DATA_BITS;
      }

      // 7 data bits, 8. data bit is parity bit
      break;
    case ARM_USART_DATA_BITS_8:
      if ((control & ARM_USART_PARITY_Msk) == ARM_USART_PARITY_NONE) {
        // 8-data bits, no parity
      } else {
        // 11-bit break detection
        cr2 |= USART_CR2_LBDL;

        // 8-data bits, 9. bit is parity bit
        cr1 |= USART_CR1_M;
      }
      break;
    case ARM_USART_DATA_BITS_9:
      if ((control & ARM_USART_PARITY_Msk) != ARM_USART_PARITY_NONE) {
        return ARM_USART_ERROR_DATA_BITS;
      }

      // 11-bit break detection
      cr2 |= USART_CR2_LBDL;

      // 9-data bits, no parity
      cr1 |= USART_CR1_M;
      break;
    default: return ARM_USART_ERROR_DATA_BITS;
  }

  // USART Parity
  switch (control & ARM_USART_PARITY_Msk) {
    case ARM_USART_PARITY_NONE:                              break;
    case ARM_USART_PARITY_EVEN:   cr1 |=  USART_CR1_PCE;     break;
    case ARM_USART_PARITY_ODD:    cr1 |= (USART_CR1_PCE | 
                                          USART_CR1_PS);     break;
    default: return ARM_USART_ERROR_PARITY;
  }

  // USART Stop bits
  switch (control & ARM_USART_STOP_BITS_Msk) {
    case ARM_USART_STOP_BITS_1:                              break;
    case ARM_USART_STOP_BITS_2:   cr2 |= USART_CR2_STOP_1;   break;
    case ARM_USART_STOP_BITS_1_5: cr2 |= USART_CR2_STOP_0 |
                                         USART_CR2_STOP_1;   break;
    case ARM_USART_STOP_BITS_0_5: cr2 |= USART_CR2_STOP_0;   break;
    default: return ARM_USART_ERROR_STOP_BITS;
  }

  // USART Flow control
  switch (control & ARM_USART_FLOW_CONTROL_Msk) {
    case ARM_USART_FLOW_CONTROL_NONE:
      flow_control = ARM_USART_FLOW_CONTROL_NONE;
      break;
    case ARM_USART_FLOW_CONTROL_RTS:
      if (usart->capabilities.flow_control_rts) {
        flow_control = ARM_USART_FLOW_CONTROL_RTS;
        // RTS Enable
        cr3 |= USART_CR3_RTSE;
      }
      else  { return ARM_USART_ERROR_FLOW_CONTROL; }
      break;
    case ARM_USART_FLOW_CONTROL_CTS:
      if (usart->capabilities.flow_control_cts) {
        flow_control = ARM_USART_FLOW_CONTROL_CTS;
        // CTS Enable, CTS interrupt enable
        cr3 |= USART_CR3_CTSE | USART_CR3_CTSIE;
      }
      else { return ARM_USART_ERROR_FLOW_CONTROL; }
      break;
    case ARM_USART_FLOW_CONTROL_RTS_CTS:
      if ((usart->capabilities.flow_control_rts != 0U) &&
          (usart->capabilities.flow_control_cts != 0U)) {
        flow_control = ARM_USART_FLOW_CONTROL_RTS_CTS;
        // RTS and CTS Enable, CTS interrupt enable
        cr3 |= (USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_CTSIE);
      } else { return ARM_USART_ERROR_FLOW_CONTROL; }
      break;
    default: return ARM_USART_ERROR_FLOW_CONTROL;
  }

  // Clock setting for synchronous mode
  if (mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) {

    // Polarity
    switch (control & ARM_USART_CPOL_Msk) {
      case ARM_USART_CPOL0:
        break;
      case ARM_USART_CPOL1:
        cr2 |= USART_CR2_CPOL;
        break;
      default: return ARM_USART_ERROR_CPOL;
    }

    // Phase
    switch (control & ARM_USART_CPHA_Msk) {
      case ARM_USART_CPHA0:
        break;
      case ARM_USART_CPHA1:
        cr2 |= USART_CR2_CPHA;
        break;
      default: return ARM_USART_ERROR_CPHA;
    }
  }

  // USART Baudrate
  val = (uint32_t) (USART_BAUDRATE_DIVIDER(usart->pclk, arg));
  br = ((usart->pclk << 4U) / (val & 0xFFFFU)) >> 4U;
  // If inside +/- 2% tolerance, baud rate configured correctly
  if (!(((br * 100U) < (arg * 102U)) && ((br * 100U) > (arg * 98U)))) {
    return ARM_USART_ERROR_BAUDRATE;
  }

  // USART Disable
  usart->reg->CR1 &= ~USART_CR1_UE;

  // Configure Baud rate register
  usart->reg->BRR = val;

  // Configuration is OK - Mode is valid
  usart->info->mode = mode;

  // Save flow control mode
  usart->info->flow_control = flow_control;

  // Configure TX pin regarding mode and transmitter state
  switch (usart->info->mode) {
    case ARM_USART_MODE_SMART_CARD:
      // USART TX pin function selected
      if (usart->io.tx) {
        GPIO_PinConfigure(usart->io.tx->port, usart->io.tx->pin, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
      }
      break;
    default:
      // Synchronous master/slave, asynchronous, single-wire and IrDA mode
      if (usart->info->flags & USART_FLAG_TX_ENABLED) {
        // USART TX pin function selected
        if (usart->io.tx) {
          GPIO_PinConfigure(usart->io.tx->port, usart->io.tx->pin, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
        }
      } else {
        // GPIO pin function selected
        if (usart->io.tx) {
          GPIO_PinConfigure(usart->io.tx->port, usart->io.tx->pin, GPIO_IN_ANALOG, GPIO_MODE_INPUT);
        }
      }
  }

  // Configure RX pin regarding mode and receiver state
  switch (usart->info->mode) {
    case ARM_USART_MODE_SINGLE_WIRE:
    case ARM_USART_MODE_SMART_CARD:
      // GPIO pin function selected
      if (usart->io.rx) {
        GPIO_PinConfigure(usart->io.rx->port, usart->io.rx->pin, GPIO_IN_ANALOG, GPIO_MODE_INPUT);
      }
      break;
    default:
      // Synchronous master/slave, asynchronous and  IrDA mode
      if (usart->info->flags & USART_FLAG_RX_ENABLED) {
        // USART RX pin function selected
        if (usart->io.rx) {
          GPIO_PinConfigure(usart->io.rx->port, usart->io.rx->pin, GPIO_IN_FLOATING, GPIO_MODE_INPUT);
        }
      } else {
       // GPIO pin function selected
        if (usart->io.rx) {
          GPIO_PinConfigure(usart->io.rx->port, usart->io.rx->pin, GPIO_IN_ANALOG, GPIO_MODE_INPUT);
        }
      }
      break;
  }

  // Configure CLK pin regarding mode
  if (usart->io.ck) {
    switch (usart->info->mode) {
      case ARM_USART_MODE_SMART_CARD:
      case ARM_USART_MODE_SYNCHRONOUS_MASTER:
        // USART CK pin function selected
        GPIO_PinConfigure(usart->io.ck->port, usart->io.ck->pin, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
        break;
      default:
        // Asynchronous, Single-wire and IrDA mode
        // GPIO pin function selected
        GPIO_PinConfigure(usart->io.ck->port, usart->io.ck->pin, GPIO_IN_ANALOG, GPIO_MODE_INPUT);
    }
  }

  // Configure RTS pin regarding Flow control configuration
  if (usart->io.rts) {
    if ((flow_control == ARM_USART_FLOW_CONTROL_RTS) ||
        (flow_control == ARM_USART_FLOW_CONTROL_RTS_CTS)) {
      // USART RTS Alternate function
      GPIO_PinConfigure(usart->io.rts->port, usart->io.rts->pin, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
    } else {
      // GPIO output
      GPIO_PinConfigure(usart->io.rts->port, usart->io.rts->pin, GPIO_OUT_PUSH_PULL, GPIO_MODE_INPUT);
    }
  }

  // Configure CTS pin regarding Flow control configuration
  if (usart->io.cts) {
    if ((flow_control == ARM_USART_FLOW_CONTROL_CTS) ||
        (flow_control == ARM_USART_FLOW_CONTROL_RTS_CTS)) {
      // USART CTS Alternate function
      GPIO_PinConfigure(usart->io.cts->port, usart->io.cts->pin, GPIO_IN_FLOATING, GPIO_MODE_INPUT);
    } else {
      // GPIO input
      GPIO_PinConfigure(usart->io.cts->port, usart->io.cts->pin, GPIO_IN_FLOATING, GPIO_MODE_INPUT);
    }
  }

  // Configure USART control registers
  usart->reg->CR1 = cr1;
  usart->reg->CR2 = cr2;
  usart->reg->CR3 = cr3;

  // USART Enable
  usart->reg->CR1 |= USART_CR1_UE;

  // Set configured flag
  usart->info->flags |= USART_FLAG_CONFIGURED;

  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_USART_STATUS USART_GetStatus (const USART_RESOURCES *usart)
  \brief       Get USART status.
  \param[in]   usart     Pointer to USART resources
  \return      USART status \ref ARM_USART_STATUS
*/
static ARM_USART_STATUS USART_GetStatus (const USART_RESOURCES *usart) {
  ARM_USART_STATUS status;

  if (usart->xfer->send_active != 0U) {
    status.tx_busy        = 1U;
  } else {
    status.tx_busy        = ((usart->reg->SR & USART_SR_TC) ? (0U) : (1U));
  }
  status.rx_busy          = usart->info->status.rx_busy;
  status.tx_underflow     = usart->info->status.tx_underflow;
  status.rx_overflow      = usart->info->status.rx_overflow;
  status.rx_break         = usart->info->status.rx_break;
  status.rx_framing_error = usart->info->status.rx_framing_error;
  status.rx_parity_error  = usart->info->status.rx_parity_error;

  return status;
}

/**
  \fn          int32_t USART_SetModemControl (      ARM_USART_MODEM_CONTROL  control,
                                              const USART_RESOURCES         *usart)
  \brief       Set USART Modem Control line state.
  \param[in]   control   \ref ARM_USART_MODEM_CONTROL
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_SetModemControl (      ARM_USART_MODEM_CONTROL  control,
                                      const USART_RESOURCES         *usart) {

  switch (control) {
    case ARM_USART_RTS_CLEAR:
      if ((usart->info->flow_control == ARM_USART_FLOW_CONTROL_NONE) ||
          (usart->info->flow_control == ARM_USART_FLOW_CONTROL_CTS)) {
        if (usart->io.rts) {
          GPIO_PinWrite (usart->io.rts->port, usart->io.rts->pin, 1U);
        }
      } else {
        // Hardware RTS
        return ARM_DRIVER_ERROR;
      }
      break;
    case ARM_USART_RTS_SET:
      if ((usart->info->flow_control == ARM_USART_FLOW_CONTROL_NONE) ||
          (usart->info->flow_control == ARM_USART_FLOW_CONTROL_CTS)) {
        if (usart->io.rts) {
          GPIO_PinWrite (usart->io.rts->port, usart->io.rts->pin, 0U);
        }
      } else {
        // Hardware RTS
        return ARM_DRIVER_ERROR;
      }
      break;
    case ARM_USART_DTR_CLEAR:
    case ARM_USART_DTR_SET:
    default: return ARM_DRIVER_ERROR;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_USART_MODEM_STATUS USART_GetModemStatus (const USART_RESOURCES *usart)
  \brief       Get USART Modem Status lines state.
  \param[in]   usart     Pointer to USART resources
  \return      modem status \ref ARM_USART_MODEM_STATUS
*/
static ARM_USART_MODEM_STATUS USART_GetModemStatus (const USART_RESOURCES *usart) {
  ARM_USART_MODEM_STATUS modem_status;

  modem_status.cts = 0U;
  if ((usart->info->flow_control == ARM_USART_FLOW_CONTROL_NONE) ||
      (usart->info->flow_control == ARM_USART_FLOW_CONTROL_RTS)) {
    if (usart->io.cts) {
      if (GPIO_PinRead (usart->io.cts->port, usart->io.cts->pin) == 0U) {
        modem_status.cts = 1U;
      }
    }
  }
  modem_status.dsr = 0U;
  modem_status.ri  = 0U;
  modem_status.dcd = 0U;

  return modem_status;
}


/**
  \fn          void USART_IRQHandler (const USART_RESOURCES *usart)
  \brief       USART Interrupt handler.
  \param[in]   usart     Pointer to USART resources
*/
void USART_IRQHandler (const USART_RESOURCES *usart) {
  uint32_t val, sr, event;
  uint16_t data;

  // Read USART status register
  sr = usart->reg->SR;

  // Reset local variables
  val   = 0U;
  event = 0U;
  data  = 0U;

  // Read Data register not empty
  if (sr & USART_SR_RXNE & usart->reg->CR1) {
    // Check for RX overflow
    if (usart->info->status.rx_busy == 0U) {
      // New receive has not been started
      // Dump RX data
      usart->reg->DR;
      usart->info->status.rx_overflow = 1;
      event |= ARM_USART_EVENT_RX_OVERFLOW;
    } else {
      if ((usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER)  &&
          (usart->xfer->sync_mode == USART_SYNC_MODE_TX)) {
        // Dummy read in synchronous transmit only mode
        usart->reg->DR;
      } else {
        // Read data from RX FIFO into receive buffer
        data = (uint16_t)usart->reg->DR;
      }

      *(usart->xfer->rx_buf++) = (uint8_t)data;

      // If nine bit data, no parity
      val = usart->reg->CR1;
      if (((val & USART_CR1_PCE) == 0U) &&
          ((val & USART_CR1_M)   != 0U)) {
        *(usart->xfer->rx_buf++) = (uint8_t)(data >> 8U);
      }
      usart->xfer->rx_cnt++;

      // Check if requested amount of data is received
      if (usart->xfer->rx_cnt == usart->xfer->rx_num) {

        // Disable IDLE interrupt
        usart->reg->CR1 &= ~USART_CR1_IDLEIE;

        // Clear RX busy flag and set receive transfer complete event
        usart->info->status.rx_busy = 0U;
        if (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) {
          val = usart->xfer->sync_mode;
          usart->xfer->sync_mode = 0U;
          switch (val) {
            case USART_SYNC_MODE_TX:
              event |= ARM_USART_EVENT_SEND_COMPLETE;
              break;
            case USART_SYNC_MODE_RX:
              event |= ARM_USART_EVENT_RECEIVE_COMPLETE;
              break;
            case USART_SYNC_MODE_TX_RX:
              event |= ARM_USART_EVENT_TRANSFER_COMPLETE;
              break;
            default: break;
          }
        } else {
          event |= ARM_USART_EVENT_RECEIVE_COMPLETE;
        }
      }
    }
  }

  // IDLE line
  if (sr & USART_SR_IDLE & usart->reg->CR1) {
    // Dummy read to clear IDLE interrupt
    usart->reg->DR;
    event |= ARM_USART_EVENT_RX_TIMEOUT;
  }

  // Transmit data register empty
  if (sr & USART_SR_TXE & usart->reg->CR1) {

    // Break handling
    if (usart->xfer->break_flag) {
      // Send break
      usart->reg->CR1 |= USART_CR1_SBK;
    } else {
      if(usart->xfer->tx_num != usart->xfer->tx_cnt) {
        if ((usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) &&
             (usart->xfer->sync_mode == USART_SYNC_MODE_RX)) {
          // Dummy write in synchronous receive only mode
          data = usart->xfer->def_val;
        } else {
          // Write data to TX FIFO
          data = *(usart->xfer->tx_buf++);

          // If nine bit data, no parity
          val = usart->reg->CR1;
          if (((val & USART_CR1_PCE) == 0U) &&
              ((val & USART_CR1_M)   != 0U)) {
            data |= *(usart->xfer->tx_buf++) << 8U;
          }
        }
      }
      usart->xfer->tx_cnt++;

      // Write to data register
      usart->reg->DR = data;

      // Check if all data is transmitted
      if (usart->xfer->tx_num == usart->xfer->tx_cnt) {
        // Disable TXE interrupt
        usart->reg->CR1 &= ~USART_CR1_TXEIE;

        // Enable TC interrupt
        usart->reg->CR1 |= USART_CR1_TCIE;

        usart->xfer->send_active = 0U;

        // Set send complete event
        if (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) {
          if ((usart->xfer->sync_mode == USART_SYNC_MODE_TX)    &&
              ((usart->info->flags & USART_FLAG_RX_ENABLED) == 0U)) {
            event |= ARM_USART_EVENT_SEND_COMPLETE;
          }
        } else {
          event |= ARM_USART_EVENT_SEND_COMPLETE;
        }
      }
    }
  }

  // Transmission complete
  if (sr & USART_SR_TC & usart->reg->CR1) {
    // Disable transmission complete interrupt
    usart->reg->CR1 &= ~USART_CR1_TCIE;
    event |= ARM_USART_EVENT_TX_COMPLETE;
  }

  // RX Overrun
  if ((sr & USART_SR_ORE) != 0U) {
    // Shift register has been overwritten
    // Dummy data read to clear the ORE flag
    usart->reg->DR;
    usart->info->status.rx_overflow = 1U;
    event |= ARM_USART_EVENT_RX_OVERFLOW;
  }

  // Framing error
  if ((sr & USART_SR_FE) != 0U) {
    // Dummy data read to clear the FE flag
    usart->reg->DR;
    usart->info->status.rx_framing_error = 1U;
    event |= ARM_USART_EVENT_RX_FRAMING_ERROR;
  }

  // Parity error
  if ((sr & USART_SR_PE) != 0U) {
    // Dummy data read to clear the PE flag
    usart->reg->DR;
    usart->info->status.rx_parity_error = 1U;
    event |= ARM_USART_EVENT_RX_PARITY_ERROR;
  }

  // Break Detection
  if ((sr & USART_SR_LBD) != 0U) {
    // Clear Break detection flag
    usart->reg->SR &= ~USART_SR_LBD;

    usart->info->status.rx_break = 1U;
    event |= ARM_USART_EVENT_RX_BREAK;
  }

  // CTS changed
  if ((sr & USART_SR_CTS) != 0U) {
    // Clear CTS flag
    usart->reg->SR &= ~USART_SR_CTS;

    event |= ARM_USART_EVENT_CTS;
  }

  // Send Event
  if ((event && usart->info->cb_event) != 0U) {
    usart->info->cb_event (event);
  }
}

#ifdef __USART_DMA_TX
void USART_TX_DMA_Complete(const USART_RESOURCES *usart) {

  if ((DMA_ChannelTransferItemCount(usart->dma_tx->instance) != 0U) && (usart->xfer->tx_num != 0U)) {
    // TX DMA Complete caused by send/transfer abort
    return;
  }

  usart->xfer->tx_cnt = usart->xfer->tx_num;
  // Clear TX busy flag
  usart->xfer->send_active = 0U;

  // TC interrupt enable
  usart->reg->CR1 |= USART_CR1_TCIE; 

  // Set Send Complete event for asynchronous transfers
  if (usart->info->mode != ARM_USART_MODE_SYNCHRONOUS_MASTER) {
    if (usart->info->cb_event) {
      usart->info->cb_event (ARM_USART_EVENT_SEND_COMPLETE);
    }
  }
}
#endif

#ifdef __USART_DMA_RX
void USART_RX_DMA_Complete(const USART_RESOURCES *usart) {
  uint32_t val, event;

  if ((DMA_ChannelTransferItemCount(usart->dma_rx->instance) != 0U) && (usart->xfer->rx_num != 0U)) {
    // RX DMA Complete caused by receive/transfer abort
    return;
  }

  // Disable IDLE interrupt
  usart->reg->CR1 &= ~USART_CR1_IDLEIE;

  event = 0U;

  if (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) {
    val = usart->xfer->sync_mode;
    usart->xfer->sync_mode = 0U;
    switch (val) {
      case USART_SYNC_MODE_TX:
        event = ARM_USART_EVENT_SEND_COMPLETE;
        break;
      case USART_SYNC_MODE_RX:
        event = ARM_USART_EVENT_RECEIVE_COMPLETE;
        break;
      case USART_SYNC_MODE_TX_RX:
        event = ARM_USART_EVENT_TRANSFER_COMPLETE;
         break;
      default: break;
    }
  } else {
    event = ARM_USART_EVENT_RECEIVE_COMPLETE;
  }

  usart->xfer->rx_cnt = usart->xfer->rx_num;
  usart->info->status.rx_busy = 0U;

  // Enable RXNE interrupt to detect RX overrun
  usart->reg->CR1 |= USART_CR1_RXNEIE;

  if (usart->info->cb_event && event) { usart->info->cb_event (event); }
}
#endif



#ifdef MX_USART1
// USART1 Driver Wrapper functions
static ARM_USART_CAPABILITIES  USART1_GetCapabilities (void)                                                { return USART_GetCapabilities (&USART1_Resources); }
static int32_t                 USART1_Initialize      (ARM_USART_SignalEvent_t cb_event)                    { return USART_Initialize (cb_event, &USART1_Resources); }
static int32_t                 USART1_Uninitialize    (void)                                                { return USART_Uninitialize (&USART1_Resources); }
static int32_t                 USART1_PowerControl    (ARM_POWER_STATE state)                               { return USART_PowerControl (state, &USART1_Resources); }
static int32_t                 USART1_Send            (const void *data, uint32_t num)                      { return USART_Send (data, num, &USART1_Resources); }
static int32_t                 USART1_Receive         (void *data, uint32_t num)                            { return USART_Receive (data, num, &USART1_Resources); }
static int32_t                 USART1_Transfer        (const void *data_out, void *data_in, uint32_t num)   { return USART_Transfer (data_out, data_in, num, &USART1_Resources); }
static uint32_t                USART1_GetTxCount      (void)                                                { return USART_GetTxCount (&USART1_Resources); }
static uint32_t                USART1_GetRxCount      (void)                                                { return USART_GetRxCount (&USART1_Resources); }
static int32_t                 USART1_Control         (uint32_t control, uint32_t arg)                      { return USART_Control (control, arg, &USART1_Resources); }
static ARM_USART_STATUS        USART1_GetStatus       (void)                                                { return USART_GetStatus (&USART1_Resources); }
static int32_t                 USART1_SetModemControl (ARM_USART_MODEM_CONTROL control)                     { return USART_SetModemControl (control, &USART1_Resources); }
static ARM_USART_MODEM_STATUS  USART1_GetModemStatus  (void)                                                { return USART_GetModemStatus (&USART1_Resources); }
       void                    USART1_IRQHandler      (void)                                                {        USART_IRQHandler (&USART1_Resources); }

#ifdef MX_USART1_TX_DMA_Instance
       void                    USART1_TX_DMA_Handler  (uint32_t events)                                     {        USART_TX_DMA_Complete(&USART1_Resources); }
#endif
#ifdef MX_USART1_RX_DMA_Instance
       void                    USART1_RX_DMA_Handler  (uint32_t events)                                     {        USART_RX_DMA_Complete(&USART1_Resources); }
#endif

// USART1 Driver Control Block
ARM_DRIVER_USART Driver_USART1 = {
    USARTx_GetVersion,
    USART1_GetCapabilities,
    USART1_Initialize,
    USART1_Uninitialize,
    USART1_PowerControl,
    USART1_Send, 
    USART1_Receive,
    USART1_Transfer,
    USART1_GetTxCount,
    USART1_GetRxCount,
    USART1_Control,
    USART1_GetStatus,
    USART1_SetModemControl,
    USART1_GetModemStatus
};
#endif

#ifdef MX_USART2
// USART2 Driver Wrapper functions
static ARM_USART_CAPABILITIES  USART2_GetCapabilities (void)                                                { return USART_GetCapabilities (&USART2_Resources); }
static int32_t                 USART2_Initialize      (ARM_USART_SignalEvent_t cb_event)                    { return USART_Initialize (cb_event, &USART2_Resources); }
static int32_t                 USART2_Uninitialize    (void)                                                { return USART_Uninitialize (&USART2_Resources); }
static int32_t                 USART2_PowerControl    (ARM_POWER_STATE state)                               { return USART_PowerControl (state, &USART2_Resources); }
static int32_t                 USART2_Send            (const void *data, uint32_t num)                      { return USART_Send (data, num, &USART2_Resources); }
static int32_t                 USART2_Receive         (void *data, uint32_t num)                            { return USART_Receive (data, num, &USART2_Resources); }
static int32_t                 USART2_Transfer        (const void *data_out, void *data_in, uint32_t num)   { return USART_Transfer (data_out, data_in, num, &USART2_Resources); }
static uint32_t                USART2_GetTxCount      (void)                                                { return USART_GetTxCount (&USART2_Resources); }
static uint32_t                USART2_GetRxCount      (void)                                                { return USART_GetRxCount (&USART2_Resources); }
static int32_t                 USART2_Control         (uint32_t control, uint32_t arg)                      { return USART_Control (control, arg, &USART2_Resources); }
static ARM_USART_STATUS        USART2_GetStatus       (void)                                                { return USART_GetStatus (&USART2_Resources); }
static int32_t                 USART2_SetModemControl (ARM_USART_MODEM_CONTROL control)                     { return USART_SetModemControl (control, &USART2_Resources); }
static ARM_USART_MODEM_STATUS  USART2_GetModemStatus  (void)                                                { return USART_GetModemStatus (&USART2_Resources); }
       void                    USART2_IRQHandler      (void)                                                {        USART_IRQHandler (&USART2_Resources); }

#ifdef MX_USART2_TX_DMA_Instance
       void                    USART2_TX_DMA_Handler  (uint32_t events)                                     {        USART_TX_DMA_Complete(&USART2_Resources); }
#endif
#ifdef MX_USART2_RX_DMA_Instance
       void                    USART2_RX_DMA_Handler  (uint32_t events)                                     {        USART_RX_DMA_Complete(&USART2_Resources); }
#endif

// USART2 Driver Control Block
ARM_DRIVER_USART Driver_USART2 = {
    USARTx_GetVersion,
    USART2_GetCapabilities,
    USART2_Initialize,
    USART2_Uninitialize,
    USART2_PowerControl,
    USART2_Send, 
    USART2_Receive,
    USART2_Transfer,
    USART2_GetTxCount,
    USART2_GetRxCount,
    USART2_Control,
    USART2_GetStatus,
    USART2_SetModemControl,
    USART2_GetModemStatus
};
#endif

#ifdef MX_USART3
// USART3 Driver Wrapper functions
static ARM_USART_CAPABILITIES  USART3_GetCapabilities (void)                                                { return USART_GetCapabilities (&USART3_Resources); }
static int32_t                 USART3_Initialize      (ARM_USART_SignalEvent_t cb_event)                    { return USART_Initialize (cb_event, &USART3_Resources); }
static int32_t                 USART3_Uninitialize    (void)                                                { return USART_Uninitialize (&USART3_Resources); }
static int32_t                 USART3_PowerControl    (ARM_POWER_STATE state)                               { return USART_PowerControl (state, &USART3_Resources); }
static int32_t                 USART3_Send            (const void *data, uint32_t num)                      { return USART_Send (data, num, &USART3_Resources); }
static int32_t                 USART3_Receive         (void *data, uint32_t num)                            { return USART_Receive (data, num, &USART3_Resources); }
static int32_t                 USART3_Transfer        (const void *data_out, void *data_in, uint32_t num)   { return USART_Transfer (data_out, data_in, num, &USART3_Resources); }
static uint32_t                USART3_GetTxCount      (void)                                                { return USART_GetTxCount (&USART3_Resources); }
static uint32_t                USART3_GetRxCount      (void)                                                { return USART_GetRxCount (&USART3_Resources); }
static int32_t                 USART3_Control         (uint32_t control, uint32_t arg)                      { return USART_Control (control, arg, &USART3_Resources); }
static ARM_USART_STATUS        USART3_GetStatus       (void)                                                { return USART_GetStatus (&USART3_Resources); }
static int32_t                 USART3_SetModemControl (ARM_USART_MODEM_CONTROL control)                     { return USART_SetModemControl (control, &USART3_Resources); }
static ARM_USART_MODEM_STATUS  USART3_GetModemStatus  (void)                                                { return USART_GetModemStatus (&USART3_Resources); }
       void                    USART3_IRQHandler      (void)                                                {        USART_IRQHandler (&USART3_Resources); }

#ifdef MX_USART3_TX_DMA_Instance
       void                    USART3_TX_DMA_Handler  (uint32_t events)                                     {        USART_TX_DMA_Complete(&USART3_Resources); }
#endif
#ifdef MX_USART3_RX_DMA_Instance
       void                    USART3_RX_DMA_Handler  (uint32_t events)                                     {        USART_RX_DMA_Complete(&USART3_Resources); }
#endif

// USART3 Driver Control Block
ARM_DRIVER_USART Driver_USART3 = {
    USARTx_GetVersion,
    USART3_GetCapabilities,
    USART3_Initialize,
    USART3_Uninitialize,
    USART3_PowerControl,
    USART3_Send, 
    USART3_Receive,
    USART3_Transfer,
    USART3_GetTxCount,
    USART3_GetRxCount,
    USART3_Control,
    USART3_GetStatus,
    USART3_SetModemControl,
    USART3_GetModemStatus
};
#endif

#ifdef MX_UART4
// USART4 Driver Wrapper functions
static ARM_USART_CAPABILITIES  USART4_GetCapabilities (void)                                                { return USART_GetCapabilities (&USART4_Resources); }
static int32_t                 USART4_Initialize      (ARM_USART_SignalEvent_t cb_event)                    { return USART_Initialize (cb_event, &USART4_Resources); }
static int32_t                 USART4_Uninitialize    (void)                                                { return USART_Uninitialize (&USART4_Resources); }
static int32_t                 USART4_PowerControl    (ARM_POWER_STATE state)                               { return USART_PowerControl (state, &USART4_Resources); }
static int32_t                 USART4_Send            (const void *data, uint32_t num)                      { return USART_Send (data, num, &USART4_Resources); }
static int32_t                 USART4_Receive         (void *data, uint32_t num)                            { return USART_Receive (data, num, &USART4_Resources); }
static int32_t                 USART4_Transfer        (const void *data_out, void *data_in, uint32_t num)   { return USART_Transfer (data_out, data_in, num, &USART4_Resources); }
static uint32_t                USART4_GetTxCount      (void)                                                { return USART_GetTxCount (&USART4_Resources); }
static uint32_t                USART4_GetRxCount      (void)                                                { return USART_GetRxCount (&USART4_Resources); }
static int32_t                 USART4_Control         (uint32_t control, uint32_t arg)                      { return USART_Control (control, arg, &USART4_Resources); }
static ARM_USART_STATUS        USART4_GetStatus       (void)                                                { return USART_GetStatus (&USART4_Resources); }
static int32_t                 USART4_SetModemControl (ARM_USART_MODEM_CONTROL control)                     { return USART_SetModemControl (control, &USART4_Resources); }
static ARM_USART_MODEM_STATUS  USART4_GetModemStatus  (void)                                                { return USART_GetModemStatus (&USART4_Resources); }
       void                    UART4_IRQHandler       (void)                                                {        USART_IRQHandler (&USART4_Resources); }

#ifdef MX_UART4_TX_DMA_Instance
       void                    UART4_TX_DMA_Handler   (uint32_t events)                                     {        USART_TX_DMA_Complete(&USART4_Resources); }
#endif
#ifdef MX_UART4_RX_DMA_Instance
       void                    UART4_RX_DMA_Handler   (uint32_t events)                                     {        USART_RX_DMA_Complete(&USART4_Resources); }
#endif

// USART4 Driver Control Block
ARM_DRIVER_USART Driver_USART4 = {
    USARTx_GetVersion,
    USART4_GetCapabilities,
    USART4_Initialize,
    USART4_Uninitialize,
    USART4_PowerControl,
    USART4_Send, 
    USART4_Receive,
    USART4_Transfer,
    USART4_GetTxCount,
    USART4_GetRxCount,
    USART4_Control,
    USART4_GetStatus,
    USART4_SetModemControl,
    USART4_GetModemStatus
};
#endif

#ifdef MX_UART5
// USART5 Driver Wrapper functions
static ARM_USART_CAPABILITIES  USART5_GetCapabilities (void)                                                { return USART_GetCapabilities (&USART5_Resources); }
static int32_t                 USART5_Initialize      (ARM_USART_SignalEvent_t cb_event)                    { return USART_Initialize (cb_event, &USART5_Resources); }
static int32_t                 USART5_Uninitialize    (void)                                                { return USART_Uninitialize (&USART5_Resources); }
static int32_t                 USART5_PowerControl    (ARM_POWER_STATE state)                               { return USART_PowerControl (state, &USART5_Resources); }
static int32_t                 USART5_Send            (const void *data, uint32_t num)                      { return USART_Send (data, num, &USART5_Resources); }
static int32_t                 USART5_Receive         (void *data, uint32_t num)                            { return USART_Receive (data, num, &USART5_Resources); }
static int32_t                 USART5_Transfer        (const void *data_out, void *data_in, uint32_t num)   { return USART_Transfer (data_out, data_in, num, &USART5_Resources); }
static uint32_t                USART5_GetTxCount      (void)                                                { return USART_GetTxCount (&USART5_Resources); }
static uint32_t                USART5_GetRxCount      (void)                                                { return USART_GetRxCount (&USART5_Resources); }
static int32_t                 USART5_Control         (uint32_t control, uint32_t arg)                      { return USART_Control (control, arg, &USART5_Resources); }
static ARM_USART_STATUS        USART5_GetStatus       (void)                                                { return USART_GetStatus (&USART5_Resources); }
static int32_t                 USART5_SetModemControl (ARM_USART_MODEM_CONTROL control)                     { return USART_SetModemControl (control, &USART5_Resources); }
static ARM_USART_MODEM_STATUS  USART5_GetModemStatus  (void)                                                { return USART_GetModemStatus (&USART5_Resources); }
       void                    UART5_IRQHandler       (void)                                                {        USART_IRQHandler (&USART5_Resources); }

#ifdef MX_UART5_TX_DMA_Instance
       void                    UART5_TX_DMA_Handler   (uint32_t events)                                     {        USART_TX_DMA_Complete(&USART5_Resources); }
#endif
#ifdef MX_UART5_RX_DMA_Instance
       void                    UART5_RX_DMA_Handler   (uint32_t events)                                     {        USART_RX_DMA_Complete(&USART5_Resources); }
#endif

// USART5 Driver Control Block
ARM_DRIVER_USART Driver_USART5 = {
    USARTx_GetVersion,
    USART5_GetCapabilities,
    USART5_Initialize,
    USART5_Uninitialize,
    USART5_PowerControl,
    USART5_Send, 
    USART5_Receive,
    USART5_Transfer,
    USART5_GetTxCount,
    USART5_GetRxCount,
    USART5_Control,
    USART5_GetStatus,
    USART5_SetModemControl,
    USART5_GetModemStatus
};
#endif
