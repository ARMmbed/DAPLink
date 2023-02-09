/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2021 Arm Limited (or its affiliates). All
 * rights reserved.
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
 *
 *
 * $Date:        16. July 2021
 * $Revision:    V1.0
 *
 * Driver:       Driver_USART0
 * Configured:   via RTE_Device.h configuration file
 * Project:      USART Driver definitions for Nordic Semiconductor nRF52820
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                   Value   UART Interface
 *   ---------------------                   -----   --------------
 *   Connect to hardware via Driver_USART# = 0       use USART0
 * --------------------------------------------------------------------------
 * Notes:
 *  - this driver does not implement CTS change detection because it would
 *    require usage of GPIOTE peripheral and IRQ shared between all pins
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.0
 *    Initial release
 */


#include "USART_nRF52820.h"

#define ARM_USART_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)  /* driver version */

/* Driver Version */
static const ARM_DRIVER_VERSION driver_version = {
    ARM_USART_API_VERSION,
    ARM_USART_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_USART_CAPABILITIES driver_capabilities = {
  1U, /* supports UART (Asynchronous) mode */
  0U, /* supports Synchronous Master mode */
  0U, /* supports Synchronous Slave mode */
  0U, /* supports UART Single-wire mode */
  0U, /* supports UART IrDA mode */
  0U, /* supports UART Smart Card mode */
  0U, /* Smart Card Clock generator available */
  (RTE_USART0_RTS_EN != 0U) ? 1U : 0U, /* RTS Flow Control support */
  (RTE_USART0_CTS_EN != 0U) ? 1U : 0U, /* CTS Flow Control support */
  1U, /* Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE */
  1U, /* Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT */
  (RTE_USART0_RTS_EN != 0U) ? 1U : 0U, /* RTS Line: 0=not available, 1=available */
  (RTE_USART0_CTS_EN != 0U) ? 1U : 0U, /* CTS Line: 0=not available, 1=available */
  0U, /* DTR Line: 0=not available, 1=available */
  0U, /* DSR Line: 0=not available, 1=available */
  0U, /* DCD Line: 0=not available, 1=available */
  0U, /* RI Line: 0=not available, 1=available */
  0U, /* Signal CTS change event: \ref ARM_USART_EVENT_CTS */
  0U, /* Signal DSR change event: \ref ARM_USART_EVENT_DSR */
  0U, /* Signal DCD change event: \ref ARM_USART_EVENT_DCD */
  0U, /* Signal RI change event: \ref ARM_USART_EVENT_RI */
  0U  /* Reserved (must be zero) */
};

static ARM_USART_SignalEvent_t fn_signal_event = NULL;  // Event callback
static ARM_USART_STATUS        usart_status;

static uint32_t driver_state = 0U;      // Driver state
static uint32_t flow_control = 0U;      // Flow control mode
static uint32_t rts_gpio_cnf = 0U;      // Index of RTS gpio cnf + 1 (0 = unconfigured)
static uint32_t cts_gpio_cnf = 0U;      // Index of CTS gpio cnf + 1 (0 = unconfigured)

static void    *ptr_tx_data  = NULL;    // Pointer to data for transmission
static uint32_t tx_num       = 0U;      // Number of bytes requested for transmission
static uint32_t rx_num       = 0U;      // Number of bytes requested for reception
static uint32_t tx_cnt       = 0U;      // Number of transmitted bytes
static uint32_t tx_buf[32/4];           // Intermediate RAM buffer for transmission
                                        // (EasyDMA cannot transmit from flash directly)

/* Function prototypes */
static ARM_DRIVER_VERSION     USART_GetVersion      (void);
static ARM_USART_CAPABILITIES USART_GetCapabilities (void);
static int32_t                USART_Initialize      (ARM_USART_SignalEvent_t cb_event);
static int32_t                USART_Uninitialize    (void);
static int32_t                USART_PowerControl    (ARM_POWER_STATE state);
static int32_t                USART_Send            (const void *data, uint32_t num);
static int32_t                USART_Receive         (      void *data, uint32_t num);
static int32_t                USART_Transfer        (const void *data_out, void *data_in, uint32_t num);
static uint32_t               USART_GetTxCount      (void);
static uint32_t               USART_GetRxCount      (void);
static int32_t                USART_Control         (uint32_t control, uint32_t arg);
static ARM_USART_STATUS       USART_GetStatus       (void);
static int32_t                USART_SetModemControl (ARM_USART_MODEM_CONTROL control);
static ARM_USART_MODEM_STATUS USART_GetModemStatus  (void);


/* Helper functions */

/* Start EasyDMA transfer Tx/Rx*/
void usart_dma_start (uint32_t dir, void *data, uint32_t num) {

  if (dir == RX_DIR) {
    NRF_UARTE0->RXD.PTR       = (uint32_t)data;
    NRF_UARTE0->RXD.MAXCNT    = num;
    NRF_UARTE0->TASKS_STARTRX = UARTE_TASKS_STARTRX_TASKS_STARTRX_Trigger;
    NRF_UARTE0->INTENSET      = UARTE_INTENSET_ENDRX_Msk;
  } else {
    NRF_UARTE0->TXD.PTR       = (uint32_t)data;
    NRF_UARTE0->TXD.MAXCNT    = num;
    NRF_UARTE0->TASKS_STARTTX = UARTE_TASKS_STARTTX_TASKS_STARTTX_Trigger;
    NRF_UARTE0->INTENSET      = UARTE_INTENSET_ENDTX_Msk;
  }
}


/* Driver functions */

/**
  \fn          ARM_DRIVER_VERSION USART_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION USART_GetVersion (void) {
  return driver_version;
}

/**
  \fn          ARM_USART_CAPABILITIES USART_GetCapabilities (void)
  \brief       Get driver capabilities
  \return      \ref ARM_USART_CAPABILITIES
*/
static ARM_USART_CAPABILITIES USART_GetCapabilities (void) {
  return driver_capabilities;
}

/**
  \fn          int32_t USART_Initialize (ARM_USART_SignalEvent_t cb_event)
  \brief       Initialize USART Interface.
  \param[in]   cb_event  Pointer to \ref ARM_USART_SignalEvent
  \return      \ref execution_status
*/
static int32_t USART_Initialize (ARM_USART_SignalEvent_t cb_event) {

  fn_signal_event = cb_event;           /* Register callback event function */

  if ((driver_state & USART_FLAG_INITIALIZED) != 0U) {
    // If Driver is already initialized
    return ARM_DRIVER_OK;
  }

  driver_state = USART_FLAG_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Uninitialize (void)
  \brief       De-initialize USART Interface.
  \return      \ref execution_status
*/
static int32_t USART_Uninitialize (void) {

  if ((driver_state & USART_FLAG_POWERED) != 0U) {
    // If peripheral is powered, power-off the peripheral
    (void)USART_PowerControl(ARM_POWER_OFF);
  }

  /* Uninitialize pins */
#if (RTE_USART0_TX_EN != 0U)            /* If Tx Pin is enabled in config file */
  /* Disconnect TXD */
  NRF_UARTE0->PSEL.TXD = RTE_USART0_TX_PIN_NUM | UARTE_PSEL_TXD_CONNECT_Msk;

  /* Configure Tx pin as GPIO with configuration as after reset */
  NRF_GPIO->PIN_CNF[RTE_USART0_TX_PIN_NUM] = 2U;
#endif
#if (RTE_USART0_RX_EN != 0U)            /* If Rx Pin is enabled in config file */
  /* Disconnect RXD */
  NRF_UARTE0->PSEL.RXD = RTE_USART0_RX_PIN_NUM | UARTE_PSEL_RXD_CONNECT_Msk;

  /* Configure Rx pin as GPIO with configuration as after reset */
  NRF_GPIO->PIN_CNF[RTE_USART0_RX_PIN_NUM] = 2U;
#endif
#if (RTE_USART0_CTS_EN != 0U)           /* If CTS Pin is enabled in config file */
  /* Disconnect CTS */
  NRF_UARTE0->PSEL.CTS = RTE_USART0_CTS_PIN_NUM | UARTE_PSEL_CTS_CONNECT_Msk;

  /* Configure CTS pin as GPIO with configuration as after reset */
  NRF_GPIO->PIN_CNF[RTE_USART0_CTS_PIN_NUM] = 2U;
#endif
#if (RTE_USART0_RTS_EN != 0U)           /* If RTS Pin is enabled in config file */
  /* Disconnect RTS */
  NRF_UARTE0->PSEL.RTS = RTE_USART0_RTS_PIN_NUM | UARTE_PSEL_RTS_CONNECT_Msk;

  /* Configure RTS pin as GPIO with configuration as after reset */
  NRF_GPIO->PIN_CNF[RTE_USART0_RTS_PIN_NUM] = 2U;
#endif

  driver_state = 0U;

  fn_signal_event = NULL;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_PowerControl (ARM_POWER_STATE state)
  \brief       Control USART Interface Power.
  \param[in]   state  Power state
  \return      \ref execution_status
*/
static int32_t USART_PowerControl (ARM_POWER_STATE state) {

  if ((driver_state & USART_FLAG_INITIALIZED) == 0U) {
    return ARM_DRIVER_ERROR;
  }

  if ((state != ARM_POWER_OFF)  &&
      (state != ARM_POWER_FULL) &&
      (state != ARM_POWER_LOW)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  switch (state) {
    case ARM_POWER_OFF:
      if (usart_status.tx_busy != 0U) {
        (void)USART_Control(ARM_USART_ABORT_SEND, 0U);
      }
      if (usart_status.rx_busy != 0U) {
        (void)USART_Control(ARM_USART_ABORT_RECEIVE, 0U);
      }

      /* Disable USARTE peripheral */
      NRF_UARTE0->ENABLE = UARTE_ENABLE_ENABLE_Disabled;

      NVIC_DisableIRQ(UARTE0_UART0_IRQn);

      USART_Control(ARM_USART_ABORT_SEND,    0U);
      USART_Control(ARM_USART_ABORT_RECEIVE, 0U);

      /* Clear variables */
      memset((void *) &usart_status, 0, sizeof(ARM_USART_STATUS));
      ptr_tx_data = NULL;
      tx_num      = 0U;
      rx_num      = 0U;
      tx_cnt      = 0U;
      memset(tx_buf, 0, sizeof(tx_buf));

      driver_state &= ~USART_FLAG_POWERED;
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      if ((driver_state & USART_FLAG_POWERED) != 0U) {
        return ARM_DRIVER_OK;
      }

      /* Clear variables */
      memset((void *) &usart_status, 0, sizeof(ARM_USART_STATUS));
      ptr_tx_data = NULL;
      tx_num      = 0U;
      rx_num      = 0U;
      tx_cnt      = 0U;
      memset(tx_buf, 0, sizeof(tx_buf));

      /* Enable USARTE peripheral */
      NRF_UARTE0->ENABLE = UARTE_ENABLE_ENABLE_Enabled;

      driver_state |= USART_FLAG_POWERED;

      NVIC_SetPriority(UARTE0_UART0_IRQn, NRFX_UARTE_DEFAULT_CONFIG_IRQ_PRIORITY);
      NVIC_ClearPendingIRQ(UARTE0_UART0_IRQn);
      NVIC_EnableIRQ(UARTE0_UART0_IRQn);
      break;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Send (const void *data, uint32_t num)
  \brief       Start sending data to USART transmitter.
  \param[in]   data  Pointer to buffer with data to send to USART transmitter
  \param[in]   num   Number of data items to send
  \return      \ref execution_status
*/
static int32_t USART_Send (const void *data, uint32_t num) {
  uint32_t num_local;

  if (usart_status.tx_busy != 0U) {
    return ARM_DRIVER_ERROR_BUSY;
  }

  usart_status.tx_busy = 1U;

  num_local = num;
  if (num > sizeof(tx_buf)) {
    num_local = sizeof(tx_buf);
  }

  /* Set auxiliary variables used for transmission */
  ptr_tx_data = (void *)data;
  tx_num = num;
  tx_cnt = 0U;

  /* Copy data to intermediate Tx buffer (up to intermediate Tx buffer size) */
  memcpy(tx_buf, data, num_local);

  /* Start transmission from intermediate Tx buffer */
  usart_dma_start(TX_DIR, tx_buf, num_local);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Receive (void *data, uint32_t num)
  \brief       Start receiving data from USART receiver.
  \param[out]  data  Pointer to buffer for data to receive from USART receiver
  \param[in]   num   Number of data items to receive
  \return      \ref execution_status
*/
static int32_t USART_Receive (void *data, uint32_t num) {

  if (usart_status.rx_busy != 0U) {
    return ARM_DRIVER_ERROR_BUSY;
  }

  usart_status.rx_busy = 1U;

  /* Start reception */
  usart_dma_start(RX_DIR, data, num);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Transfer (const void *data_out, void *data_in, uint32_t num)
  \brief       Start sending/receiving data to/from USART transmitter/receiver.
  \param[in]   data_out  Pointer to buffer with data to send to USART transmitter
  \param[out]  data_in   Pointer to buffer for data to receive from USART receiver
  \param[in]   num       Number of data items to transfer
  \return      \ref execution_status
*/
static int32_t USART_Transfer (const void *data_out, void *data_in, uint32_t num) {

  /* This USART Controller does not support Synchronous mode */
  return ARM_DRIVER_ERROR;
}

/**
  \fn          uint32_t USART_GetTxCount (void)
  \brief       Get transmitted data count.
  \return      number of data items transmitted
*/
static uint32_t USART_GetTxCount (void) {
  return tx_cnt;
}

/**
  \fn          uint32_t USART_GetRxCount (void)
  \brief       Get received data count.
  \return      number of data items received
*/
static uint32_t USART_GetRxCount (void) {
  if (rx_num != 0U) {
    return NRF_UARTE0->RXD.AMOUNT;
  }

  return 0U;
}

/**
  \fn          int32_t USART_Control (uint32_t control, uint32_t arg)
  \brief       Control USART Interface.
  \param[in]   control  Operation
  \param[in]   arg      Argument of operation (optional)
  \return      common \ref execution_status and driver specific \ref usart_execution_status
*/
static int32_t USART_Control (uint32_t control, uint32_t arg) {
  uint32_t cnt;

  switch (control & ARM_USART_CONTROL_Msk) {
    case ARM_USART_MODE_ASYNCHRONOUS:
      /* Only Asynchronous mode is supported */

      /* Configure data bits */
      switch (control & ARM_USART_DATA_BITS_Msk) {
        case ARM_USART_DATA_BITS_8:
          break;
        case ARM_USART_DATA_BITS_5:
        case ARM_USART_DATA_BITS_6:
        case ARM_USART_DATA_BITS_7:
        case ARM_USART_DATA_BITS_9:
        default:
          return ARM_USART_ERROR_DATA_BITS;
      }

      /* Configure parity */
      switch (control & ARM_USART_PARITY_Msk) {
        case ARM_USART_PARITY_NONE:
          NRF_UARTE0->CONFIG &= ~UARTE_CONFIG_PARITY_Msk;
          break;
        case ARM_USART_PARITY_EVEN:
          NRF_UARTE0->CONFIG &= ~UARTE_CONFIG_PARITYTYPE_Msk;
          NRF_UARTE0->CONFIG &= ~UARTE_CONFIG_PARITY_Msk;
          break;
        case ARM_USART_PARITY_ODD:
          NRF_UARTE0->CONFIG |=  UARTE_CONFIG_PARITYTYPE_Msk;
          NRF_UARTE0->CONFIG &= ~UARTE_CONFIG_PARITY_Msk;
          break;
        default:
          return ARM_USART_ERROR_PARITY;
      }

      /* Configure stop bits */
      switch (control & ARM_USART_STOP_BITS_Msk) {
        case ARM_USART_STOP_BITS_1:
          NRF_UARTE0->CONFIG &= ~UARTE_CONFIG_STOP_Msk;
          break;
        case ARM_USART_STOP_BITS_2:
          NRF_UARTE0->CONFIG |=  UARTE_CONFIG_STOP_Msk;
          break;
        case ARM_USART_STOP_BITS_1_5:
        case ARM_USART_STOP_BITS_0_5:
        default:
          return ARM_USART_ERROR_STOP_BITS;
      }

      /* Configure flow control */
      switch (control & ARM_USART_FLOW_CONTROL_Msk) {
        case ARM_USART_FLOW_CONTROL_NONE:
#if (RTE_USART0_CTS_EN != 0U)           /* If CTS Pin is enabled in config file */
          /* Disconnect CTS */
          NRF_UARTE0->PSEL.CTS = RTE_USART0_CTS_PIN_NUM | UARTE_PSEL_CTS_CONNECT_Msk;

          /* Configure CTS pin as GPIO input with pull-up */
          NRF_GPIO->PIN_CNF[RTE_USART0_CTS_PIN_NUM] = GPIO_PIN_CNF_PULL_Msk;
#endif
#if (RTE_USART0_RTS_EN != 0U)           /* If RTS Pin is enabled in config file */
          /* Disconnect RTS */
          NRF_UARTE0->PSEL.RTS = RTE_USART0_RTS_PIN_NUM | UARTE_PSEL_RTS_CONNECT_Msk;

          /* Configure RTS pin as GPIO output with pull-up */
          NRF_GPIO->PIN_CNF[RTE_USART0_RTS_PIN_NUM] = GPIO_PIN_CNF_PULL_Msk | GPIO_PIN_CNF_DIR_Output | GPIO_PIN_CNF_INPUT_Msk;
#endif
          /* Disable hardware flow control */
          NRF_UARTE0->CONFIG &= ~UARTE_CONFIG_HWFC_Msk;
          break;
        case ARM_USART_FLOW_CONTROL_RTS_CTS:
#if (RTE_USART0_CTS_EN != 0U)           /* If CTS Pin is enabled in config file */
          /* Configure CTS pin as GPIO input with pull-up */
          NRF_GPIO->PIN_CNF[RTE_USART0_CTS_PIN_NUM] = GPIO_PIN_CNF_PULL_Msk;

          /* Connect CTS */
          NRF_UARTE0->PSEL.CTS = RTE_USART0_CTS_PIN_NUM;
#endif
#if (RTE_USART0_RTS_EN != 0U)           /* If RTS Pin is enabled in config file */
          /* Configure RTS pin as GPIO output with pull-up */
          NRF_GPIO->PIN_CNF[RTE_USART0_RTS_PIN_NUM] = GPIO_PIN_CNF_PULL_Msk | GPIO_PIN_CNF_DIR_Output | GPIO_PIN_CNF_INPUT_Msk;

          /* Connect RTS */
          NRF_UARTE0->PSEL.RTS = RTE_USART0_RTS_PIN_NUM;
#endif
          /* Enable hardware flow control */
          NRF_UARTE0->CONFIG |=  UARTE_CONFIG_HWFC_Msk;
          break;
        case ARM_USART_FLOW_CONTROL_CTS:
        case ARM_USART_FLOW_CONTROL_RTS:
        default:
          return ARM_USART_ERROR_FLOW_CONTROL;
      }

      /* Ignore clock polarity and clock phase setting */

      /* Configure baudrate */
      switch (arg) {
        case 1200:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud1200;
          break;
        case 2400:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud2400;
          break;
        case 4800:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud4800;
          break;
        case 9600:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud9600;
          break;
        case 14400:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud14400;
          break;
        case 19200:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud19200;
          break;
        case 28800:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud28800;
          break;
        case 31250:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud31250;
          break;
        case 38400:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud38400;
          break;
        case 56000:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud56000;
          break;
        case 57600:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud57600;
          break;
        case 76800:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud76800;
          break;
        case 115200:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud115200;
          break;
        case 230400:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud230400;
          break;
        case 250000:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud250000;
          break;
        case 460800:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud460800;
          break;
        case 921600:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud921600;
          break;
        case 1000000:
          NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud1M;
          break;
        default:
          return ARM_USART_ERROR_BAUDRATE;
      }
      break;

    case ARM_USART_CONTROL_TX:
#if (RTE_USART0_TX_EN != 0U)            /* If Tx Pin is enabled in config file */
      switch (arg) {
        case 0:                         /* Disable transmitter */
          /* Disconnect TXD */
          NRF_UARTE0->PSEL.TXD = RTE_USART0_TX_PIN_NUM | UARTE_PSEL_TXD_CONNECT_Msk;

          /* Leave pull-up */
          break;
        case 1:                         /* Enable transmitter */
          /* Configure Tx pin as GPIO output with pull-up */
          NRF_GPIO->PIN_CNF[RTE_USART0_TX_PIN_NUM] = GPIO_PIN_CNF_PULL_Msk | GPIO_PIN_CNF_DIR_Output | GPIO_PIN_CNF_INPUT_Msk;

          /* Connect TXD */
          NRF_UARTE0->PSEL.TXD = RTE_USART0_TX_PIN_NUM;
          break;
        default:
          return ARM_DRIVER_ERROR_PARAMETER;
      }
      break;
#else
      return ARM_DRIVER_ERROR_UNSUPPORTED;
#endif

    case ARM_USART_CONTROL_RX:
#if (RTE_USART0_RX_EN != 0U)            /* If Rx Pin is enabled in config file */
      switch (arg) {
        case 0:                         /* Disable receiver */
          /* Disconnect RXD */
          NRF_UARTE0->PSEL.RXD = RTE_USART0_RX_PIN_NUM | UARTE_PSEL_RXD_CONNECT_Msk;

          /* Leave pull-up */

          /* Disable Rx timeout and error event detection */
          NRF_UARTE0->INTENCLR = UARTE_INTENCLR_RXTO_Msk | UARTE_INTENCLR_ERROR_Msk;
          break;
        case 1:                         /* Enable receiver */
          /* Configure Rx pin as GPIO input with pull-up */
          NRF_GPIO->PIN_CNF[RTE_USART0_RX_PIN_NUM] = GPIO_PIN_CNF_PULL_Msk;

          /* Connect RXD */
          NRF_UARTE0->PSEL.RXD = RTE_USART0_RX_PIN_NUM;

          /* Enable Rx timeout and error event detection */
          NRF_UARTE0->INTENSET = UARTE_INTENSET_RXTO_Msk | UARTE_INTENSET_ERROR_Msk;
          break;
        default:
          return ARM_DRIVER_ERROR_PARAMETER;
      }
      break;
#else
      return ARM_DRIVER_ERROR_UNSUPPORTED;
#endif

    case ARM_USART_ABORT_SEND:
      /* Disable TXSTOPPED and ENDTX interrupt */
      NRF_UARTE0->INTENCLR = UARTE_INTENCLR_TXSTOPPED_Msk | UARTE_INTENCLR_ENDTX_Msk;

      /* Stop Tx */
      NRF_UARTE0->TASKS_STOPTX = UARTE_TASKS_STOPTX_TASKS_STOPTX_Trigger;
      for (cnt = 160000U; cnt != 0U; cnt--) {
        if (NRF_UARTE0->EVENTS_TXSTOPPED == 1U) {
          usart_status.tx_busy = 0U;
          break;
        }
      }
      if (cnt == 0U) {                  /* If timeout */
        return ARM_DRIVER_ERROR_TIMEOUT;
      }
      break;

    case ARM_USART_ABORT_RECEIVE:
      /* Disable RXTO and ENDRX interrupts */
      NRF_UARTE0->INTENCLR = UARTE_INTENCLR_RXTO_Msk | UARTE_INTENCLR_ENDRX_Msk;

      /* Stop Rx */
      NRF_UARTE0->TASKS_STOPRX = UARTE_TASKS_STOPRX_TASKS_STOPRX_Trigger;
      for (cnt = 160000U; cnt != 0U; cnt--) {
        if (NRF_UARTE0->EVENTS_RXTO == 1U) {
          usart_status.rx_busy = 0U;
          break;
        }
      }
      if (cnt == 0U) {                  /* If timeout */
        return ARM_DRIVER_ERROR_TIMEOUT;
      }

      /* Flush Rx FIFO to Rx buffer */
      NRF_UARTE0->TASKS_FLUSHRX = UARTE_TASKS_FLUSHRX_TASKS_FLUSHRX_Trigger;
      for (cnt = 160000U; cnt != 0U; cnt--) {
        if (NRF_UARTE0->EVENTS_ENDRX == 1U) {
          break;
        }
      }
      if (cnt == 0U) {                  /* If timeout */
        return ARM_DRIVER_ERROR_TIMEOUT;
      }
      break;

    /* Unsupported modes */
    case ARM_USART_MODE_SYNCHRONOUS_MASTER:
    case ARM_USART_MODE_SYNCHRONOUS_SLAVE:
    case ARM_USART_MODE_SINGLE_WIRE:
    case ARM_USART_MODE_IRDA:
    case ARM_USART_MODE_SMART_CARD:
      return ARM_USART_ERROR_MODE;

    /* Unsupported miscellaneous operations */
    case ARM_USART_SET_DEFAULT_TX_VALUE:
    case ARM_USART_SET_IRDA_PULSE:
    case ARM_USART_SET_SMART_CARD_GUARD_TIME:
    case ARM_USART_SET_SMART_CARD_CLOCK:
    case ARM_USART_CONTROL_SMART_CARD_NACK:
    case ARM_USART_CONTROL_BREAK:
    case ARM_USART_ABORT_TRANSFER:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_USART_STATUS USART_GetStatus (void)
  \brief       Get USART status.
  \return      USART status \ref ARM_USART_STATUS
*/
static ARM_USART_STATUS USART_GetStatus (void) {
  return usart_status;
}

/**
  \fn          int32_t USART_SetModemControl (ARM_USART_MODEM_CONTROL control)
  \brief       Set USART Modem Control line state.
  \param[in]   control   \ref ARM_USART_MODEM_CONTROL
  \return      \ref execution_status
*/
static int32_t USART_SetModemControl (ARM_USART_MODEM_CONTROL control) {

  switch (control) {
    case ARM_USART_RTS_CLEAR:
      if ((flow_control == ARM_USART_FLOW_CONTROL_NONE) ||
          (flow_control == ARM_USART_FLOW_CONTROL_CTS)) {
#if (RTE_USART0_RTS_EN != 0U)
        if ((rts_gpio_cnf == 0U)  ||
            (rts_gpio_cnf > 328U) ||
           ((NRF_GPIOTE->CONFIG[rts_gpio_cnf-1U] & 1) != 1U)) {
          /* If RTS pin is not configured as GPIO Output */
          return ARM_DRIVER_ERROR;
        }
        NRF_GPIO->OUTSET = (1U << RTE_USART0_RTS_PIN_NUM);
#else
        return ARM_DRIVER_ERROR;
#endif
      } else {
        /* Hardware controlled RTS */
        return ARM_DRIVER_ERROR;
      }
      break;
    case ARM_USART_RTS_SET:
      if ((flow_control == ARM_USART_FLOW_CONTROL_NONE) ||
          (flow_control == ARM_USART_FLOW_CONTROL_CTS)) {
#if (RTE_USART0_RTS_EN != 0U)
        if ((rts_gpio_cnf == 0U)  ||
            (rts_gpio_cnf > 328U) ||
            ((NRF_GPIOTE->CONFIG[rts_gpio_cnf-1U] & 1) != 1U)) {
          /* If RTS pin is not configured as GPIO Output */
          return ARM_DRIVER_ERROR;
        }
        NRF_GPIO->OUTCLR = (1U << RTE_USART0_RTS_PIN_NUM);
#else
        return ARM_DRIVER_ERROR;
#endif
      } else {
        /* Hardware controlled RTS */
        return ARM_DRIVER_ERROR;
      }
      break;
    case ARM_USART_DTR_CLEAR:
    case ARM_USART_DTR_SET:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
    default:
      return ARM_DRIVER_ERROR_PARAMETER;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_USART_MODEM_STATUS USART_GetModemStatus (void)
  \brief       Get USART Modem Status lines state.
  \return      modem status \ref ARM_USART_MODEM_STATUS
*/
static ARM_USART_MODEM_STATUS USART_GetModemStatus (void) {
  ARM_USART_MODEM_STATUS modem_status;

  modem_status.cts      = 0U;
  if ((flow_control == ARM_USART_FLOW_CONTROL_NONE) ||
      (flow_control == ARM_USART_FLOW_CONTROL_RTS)) {
#if (RTE_USART0_CTS_EN != 0U)
    if ((cts_gpio_cnf != 0U)  ||
        (cts_gpio_cnf <= 32U) ||
       ((NRF_GPIOTE->CONFIG[cts_gpio_cnf-1U] & 3) == 0U)) {
      /* If CTS pin is configured as GPIO Input */
      if ((NRF_GPIO->IN & (1U << RTE_USART0_RTS_PIN_NUM)) == 0U) {
        modem_status.cts = 1U;
      }
    }
#endif
  }
  modem_status.dsr      = 0U;
  modem_status.dcd      = 0U;
  modem_status.ri       = 0U;
  modem_status.reserved = 0U;

  return modem_status;
}


/**
  \fn          void UARTE0_UART0_IRQHandler (void)
  \brief       USART Interrupt handler.
*/
void UARTE0_UART0_IRQHandler (void) {
  uint32_t events, error_src, num_to_send, num_sent;

  events = 0U;

  /* Handle transmission interrupt */
  if (NRF_UARTE0->EVENTS_ENDTX != 0U) {
    NRF_UARTE0->EVENTS_ENDTX = 0U;
    num_sent = NRF_UARTE0->TXD.AMOUNT;
    tx_cnt      += num_sent;
    ptr_tx_data += num_sent;
    if (tx_cnt < tx_num) {
      num_to_send = tx_num - tx_cnt;
      if (num_to_send > sizeof(tx_buf)) {
        num_to_send = sizeof(tx_buf);
      }

      /* Still data to transmit, continue transmission */
      /* Copy data to intermediate Tx buffer (up to intermediate Tx buffer size) */
      memcpy(tx_buf, ptr_tx_data, num_to_send);

      /* Start transmission from intermediate Tx buffer */
      usart_dma_start(TX_DIR, tx_buf, num_to_send);
    } else {
      /* Disable ENDTX interrupt */
      NRF_UARTE0->INTENCLR = UARTE_INTENCLR_ENDTX_Msk;

      usart_status.tx_busy = 0U;

      events = ARM_USART_EVENT_SEND_COMPLETE | ARM_USART_EVENT_TX_COMPLETE;
    }
  }

  /* Handle reception interrupt */
  if (NRF_UARTE0->EVENTS_ENDRX != 0U) {
    NRF_UARTE0->EVENTS_ENDRX = 0U;
    NRF_UARTE0->INTENCLR     = UARTE_INTENCLR_ENDRX_Msk;
    usart_status.rx_busy     = 0U;
    events |= ARM_USART_EVENT_RECEIVE_COMPLETE;
  }

  /* Handle reception timeout */
  if (NRF_UARTE0->EVENTS_RXTO != 0U) {
    NRF_UARTE0->EVENTS_RXTO = 0U;
    events |= ARM_USART_EVENT_RX_TIMEOUT;
  }

  /* Handle break and error events */
  if (NRF_UARTE0->EVENTS_ERROR != 0U) {
    error_src = NRF_UARTE0->ERRORSRC;
    NRF_UARTE0->ERRORSRC     = error_src;
    NRF_UARTE0->EVENTS_ERROR = 0U;
    if ((error_src & UART_ERRORSRC_BREAK_Msk) != 0U) {
      events |= ARM_USART_EVENT_RX_BREAK;
    }
    if ((error_src & UART_ERRORSRC_FRAMING_Msk) != 0U) {
      events |= ARM_USART_EVENT_RX_FRAMING_ERROR;
    }
    if ((error_src & UART_ERRORSRC_PARITY_Msk) != 0U) {
      events |= ARM_USART_EVENT_RX_PARITY_ERROR;
    }
    if ((error_src & UART_ERRORSRC_OVERRUN_Msk) != 0U) {
      events |= ARM_USART_EVENT_RX_OVERFLOW;
    }
  }

  /* Call SignalEvent function */
  if (fn_signal_event != NULL) {
    fn_signal_event(events);
  }
}


// USART0 Driver Control Block
extern \
ARM_DRIVER_USART Driver_USART0;
ARM_DRIVER_USART Driver_USART0 = {
  USART_GetVersion,
  USART_GetCapabilities,
  USART_Initialize,
  USART_Uninitialize,
  USART_PowerControl,
  USART_Send,
  USART_Receive,
  USART_Transfer,
  USART_GetTxCount,
  USART_GetRxCount,
  USART_Control,
  USART_GetStatus,
  USART_SetModemControl,
  USART_GetModemStatus
};
