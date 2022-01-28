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
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        23. July 2021
 * $Revision:    V1.0
 *
 * Driver:       Driver_I2C1
 * Configured:   via RTE_Device.h configuration file
 * Project:      I2C Slave Driver for Nordic Semiconductor nRF52820
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                 Value   I2C Interface
 *   ---------------------                 -----   -------------
 *   Connect to hardware via Driver_I2C# = 1       use TWIS1
 * --------------------------------------------------------------------------
 * Note:
 *  Only slave implementation
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.0
 *    Initial release
 */

#include "I2C_Slave_nRF52820.h"

#define ARM_I2C_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,0)    /* driver version */

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
  ARM_I2C_API_VERSION,
  ARM_I2C_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_I2C_CAPABILITIES DriverCapabilities = {
  0U                    // Does not support 10-bit addressing
#if (defined(ARM_I2C_API_VERSION) && (ARM_I2C_API_VERSION >= 0x203U))
, 0U                    // Reserved bits
#endif
};

/* TWIS0 */
#if RTE_TWIS0
static I2C_INFO I2C0_Info;
static void TWIS0_event_handler (nrfx_twis_evt_t const * p_event);

static I2C_RESOURCES I2C0_Resources = {
  // nrfx_twis
  {NRF_TWIS0, 0},
  TWIS0_event_handler,
  &I2C0_Info
};

extern void nrfx_twis_0_irq_handler(void);

void SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler (void) {
  nrfx_twis_0_irq_handler();
}
#endif

// Function prototypes
static int32_t I2C_PowerControl (ARM_POWER_STATE state, I2C_RESOURCES *i2c);

// Local functions
static void twis_event_handler (nrfx_twis_evt_t const * p_event, I2C_RESOURCES *i2c) {
  uint32_t event = 0U;

  switch (p_event->type) {
    case NRFX_TWIS_EVT_READ_REQ:
      // TODO: Check if this fix is correct
      // if (nrfx_twis_is_waiting_tx_buff(&i2c->nrfx_twis)) {
      if (p_event->data.buf_req) {
        event |= ARM_I2C_EVENT_SLAVE_TRANSMIT;
      }
      break;
    case NRFX_TWIS_EVT_WRITE_REQ:
      // TODO: Check if this fix is correct
      // if (nrfx_twis_is_waiting_rx_buff(&i2c->nrfx_twis)) {
      if (p_event->data.buf_req) {
        event |= ARM_I2C_EVENT_SLAVE_RECEIVE;
      }
      break;
    case NRFX_TWIS_EVT_READ_DONE:
      if (i2c->info->tx_num < nrfx_twis_tx_amount(&i2c->nrfx_twis)) {
        event |= ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
      }
      event |= ARM_I2C_EVENT_TRANSFER_DONE;
      break;
    case NRFX_TWIS_EVT_WRITE_DONE:
      if (i2c->info->rx_num < nrfx_twis_rx_amount(&i2c->nrfx_twis)) {
        event |= ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
      }
      event |= ARM_I2C_EVENT_TRANSFER_DONE;
      break;

    case NRFX_TWIS_EVT_READ_ERROR:
    case NRFX_TWIS_EVT_WRITE_ERROR:
    case NRFX_TWIS_EVT_GENERAL_ERROR:
      i2c->info->err = 1U;
      nrfx_twis_error_get_and_clear (&i2c->nrfx_twis);
      event |= ARM_I2C_EVENT_BUS_ERROR;
      break;
    default:
      break;
  }

  if (i2c->info->cb_event != NULL) {
    uint8_t address = nrf_twis_match_get(i2c->nrfx_twis.p_reg);
    if (address == 0) {
      event |= EXTENSION_I2C_EVENT_SLAVE_ADDR_0;
    } else if (address == 1) {
      event |= EXTENSION_I2C_EVENT_SLAVE_ADDR_1;
    }
    i2c->info->cb_event (event);
  }
}

/**
  \fn          ARM_DRV_VERSION I2C_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRV_VERSION
*/
static ARM_DRIVER_VERSION I2CX_GetVersion (void) {
  return DriverVersion;
}


/**
  \fn          ARM_I2C_CAPABILITIES I2C_GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      \ref ARM_I2C_CAPABILITIES
*/
static ARM_I2C_CAPABILITIES I2CX_GetCapabilities (void) {
  return DriverCapabilities;
}


/**
  \fn          int32_t I2C_Initialize (ARM_I2C_SignalEvent_t cb_event, I2C_RESOURCES *i2c)
  \brief       Initialize I2C Interface.
  \param[in]   cb_event  Pointer to \ref ARM_I2C_SignalEvent
  \param[in]   i2c   Pointer to I2C resources
  \return      \ref ARM_I2C_STATUS
*/
static int32_t I2C_Initialize (ARM_I2C_SignalEvent_t cb_event, I2C_RESOURCES *i2c) {
  nrfx_err_t err;
  int32_t    ret = ARM_DRIVER_ERROR;

  memset (i2c->info, 0, sizeof(I2C_INFO));
  i2c->info->cb_event = cb_event;

  if ((i2c->info->state & I2C_INITIALIZED) != 0U) {
    // If Driver is already initialized
    return ARM_DRIVER_OK;
  }

  // TWIS Configuration
  i2c->info->config.addr[0]   = RTE_TWIS0_ADDR0;
  i2c->info->config.addr[1]   = RTE_TWIS0_ADDR1;
  i2c->info->config.scl       = RTE_TWIS0_SCL_PIN_NUM;
  i2c->info->config.scl_pull  = NRF_GPIO_PIN_NOPULL;
  i2c->info->config.sda       = RTE_TWIS0_SDA_PIN_NUM;
  i2c->info->config.sda_pull  = NRF_GPIO_PIN_NOPULL;
  i2c->info->config.interrupt_priority = NRFX_TWIS_DEFAULT_CONFIG_IRQ_PRIORITY;

  err = nrfx_twis_init (&i2c->nrfx_twis,
                        &i2c->info->config,
                        i2c->twis_event_handler);

  if (err == NRFX_SUCCESS) {
    i2c->info->state = I2C_INITIALIZED;
    ret = ARM_DRIVER_OK;
  }

  return ret;
}


/**
  \fn          int32_t I2C_Uninitialize (I2C_RESOURCES *i2c)
  \brief       De-initialize I2C Interface.
  \param[in]   i2c  Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_Uninitialize (I2C_RESOURCES *i2c) {

  if ((i2c->info->state & I2C_POWERED) != 0U) {
    // If peripheral is powered, power-off the peripheral
    (void)I2C_PowerControl(ARM_POWER_OFF, i2c);
  }

  nrfx_twis_uninit (&i2c->nrfx_twis);

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t ARM_I2C_PowerControl (ARM_POWER_STATE state, I2C_RESOURCES *i2c)
  \brief       Control I2C Interface Power.
  \param[in]   state  Power state
  \param[in]   i2c  Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_PowerControl (ARM_POWER_STATE state, I2C_RESOURCES *i2c) {

  if ((i2c->info->state & I2C_INITIALIZED) == 0U) {
    return ARM_DRIVER_ERROR;
  }

  if ((state != ARM_POWER_OFF)  &&
      (state != ARM_POWER_FULL) &&
      (state != ARM_POWER_LOW)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }
  switch (state) {
    case ARM_POWER_OFF:
      nrfx_twis_disable (&i2c->nrfx_twis);
      i2c->info->state &= ~I2C_POWERED;
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      if ((i2c->info->state & I2C_POWERED) != 0U) {
        return ARM_DRIVER_OK;
      }
      nrfx_twis_enable (&i2c->nrfx_twis);
      i2c->info->state |= I2C_POWERED;

      break;
  }

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t I2C_MasterTransmit (uint32_t       addr,
                                           const uint8_t *data,
                                           uint32_t       num,
                                           bool           xfer_pending,
                                           I2C_RESOURCES *i2c)
  \brief       Start transmitting data as I2C Master.
  \param[in]   addr          Slave address (7-bit or 10-bit)
  \param[in]   data          Pointer to buffer with data to send to I2C Slave
  \param[in]   num           Number of data bytes to send
  \param[in]   xfer_pending  Transfer operation is pending - Stop condition will not be generated
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_MasterTransmit (uint32_t       addr,
                                   const uint8_t *data,
                                   uint32_t       num,
                                   bool           xfer_pending,
                                   I2C_RESOURCES *i2c) {

  return ARM_DRIVER_ERROR_UNSUPPORTED;
}


/**
  \fn          int32_t I2C_MasterReceive (uint32_t       addr,
                                          uint8_t       *data,
                                          uint32_t       num,
                                          bool           xfer_pending,
                                          I2C_RESOURCES *i2c)
  \brief       Start receiving data as I2C Master.
  \param[in]   addr          Slave address (7-bit or 10-bit)
  \param[out]  data          Pointer to buffer for data to receive from I2C Slave
  \param[in]   num           Number of data bytes to receive
  \param[in]   xfer_pending  Transfer operation is pending - Stop condition will not be generated
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_MasterReceive (uint32_t       addr,
                                  uint8_t       *data,
                                  uint32_t       num,
                                  bool           xfer_pending,
                                  I2C_RESOURCES *i2c) {
  return ARM_DRIVER_ERROR_UNSUPPORTED;
}


/**
  \fn          int32_t I2C_SlaveTransmit (const uint8_t *data, uint32_t num, I2C_RESOURCES *i2c)
  \brief       Start transmitting data as I2C Slave.
  \param[in]   data          Pointer to buffer with data to send to I2C Master
  \param[in]   num           Number of data bytes to send
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_SlaveTransmit (const uint8_t *data, uint32_t num, I2C_RESOURCES *i2c) {
  nrfx_err_t  err;
  int32_t     ret = ARM_DRIVER_ERROR;

  if ((data == NULL) || (num == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  i2c->info->err = 0U;
  err = nrfx_twis_tx_prepare (&i2c->nrfx_twis, data, num);
  if (err == NRFX_SUCCESS) {
    i2c->info->dir = I2C_DIR_TX;
    i2c->info->tx_num   = num;
    ret = ARM_DRIVER_OK;
  }

  return ret;
}


/**
  \fn          int32_t I2C_SlaveReceive (uint8_t *data, uint32_t num, I2C_RESOURCES *i2c)
  \brief       Start receiving data as I2C Slave.
  \param[out]  data          Pointer to buffer for data to receive from I2C Master
  \param[in]   num           Number of data bytes to receive
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_SlaveReceive (uint8_t *data, uint32_t num, I2C_RESOURCES *i2c) {
  nrfx_err_t  err;
  int32_t     ret = ARM_DRIVER_ERROR;

  if ((data == NULL) || (num == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  i2c->info->err = 0U;
  err = nrfx_twis_rx_prepare (&i2c->nrfx_twis, data, num);
  if (err == NRFX_SUCCESS) {
    i2c->info->dir = I2C_DIR_RX;
    i2c->info->rx_num = num;
    ret = ARM_DRIVER_OK;
  }

  return ret;
}


/**
  \fn          int32_t I2C_GetDataCount (void)
  \brief       Get transferred data count.
  \return      number of data bytes transferred; -1 when Slave is not addressed by Master
*/
static int32_t I2C_GetDataCount (I2C_RESOURCES *i2c) {
  int32_t val = 0U;

  if (i2c->info->dir == I2C_DIR_TX) {
    val = nrfx_twis_tx_amount(&i2c->nrfx_twis);
  } else {
    val = nrfx_twis_rx_amount(&i2c->nrfx_twis);
  }
  return (val);
}


/**
  \fn          int32_t I2C_Control (uint32_t control, uint32_t arg, I2C_RESOURCES *i2c)
  \brief       Control I2C Interface.
  \param[in]   control  operation
  \param[in]   arg      argument of operation (optional)
  \param[in]   i2c      pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_Control (uint32_t control, uint32_t arg, I2C_RESOURCES *i2c) {
  nrfx_err_t err;
  int32_t    ret = ARM_DRIVER_ERROR;

  if ((i2c->info->state & I2C_POWERED) == 0U) {
    /* I2C not powered */
    return ARM_DRIVER_ERROR;
  }

  switch (control) {
    case ARM_I2C_OWN_ADDRESS:
      nrfx_twis_disable (&i2c->nrfx_twis);
      nrfx_twis_uninit (&i2c->nrfx_twis);

      i2c->info->config.addr[0] = arg;
      i2c->info->config.addr[1] = 0U;

      err = nrfx_twis_init (&i2c->nrfx_twis,
                      &i2c->info->config,
                      i2c->twis_event_handler);

      if (err == NRFX_SUCCESS) {
        i2c->info->state = I2C_INITIALIZED;
        nrfx_twis_enable (&i2c->nrfx_twis);
        ret = ARM_DRIVER_OK;
      }
      break;

    case ARM_I2C_BUS_SPEED:
      break;

    case ARM_I2C_ABORT_TRANSFER:
      break;

    default:
      break;
  }
  return ret;
}


/**
  \fn          ARM_I2C_STATUS I2C_GetStatus (I2C_RESOURCES *i2c)
  \brief       Get I2C status.
  \param[in]   i2c      pointer to I2C resources
  \return      I2C status \ref ARM_I2C_STATUS
*/
static ARM_I2C_STATUS I2C_GetStatus (I2C_RESOURCES *i2c) {
  ARM_I2C_STATUS status;

  memset(&status, 0U, sizeof(ARM_I2C_STATUS));
  if (nrfx_twis_is_busy(&i2c->nrfx_twis)) {
    status.busy = 1U;
  }
  status.direction = i2c->info->dir;
  status.bus_error = i2c->info->err;

  return (status);
}

#if defined(RTE_TWIS0)
/* I2C1 Driver wrapper functions */
static int32_t I2C0_Initialize (ARM_I2C_SignalEvent_t cb_event) {
  return I2C_Initialize(cb_event, &I2C0_Resources);
}
static int32_t I2C0_Uninitialize (void) {
  return I2C_Uninitialize(&I2C0_Resources);
}
static int32_t I2C0_PowerControl (ARM_POWER_STATE state) {
  return I2C_PowerControl(state, &I2C0_Resources);
}
static int32_t I2C0_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterTransmit(addr, data, num, xfer_pending, &I2C0_Resources);
}
static int32_t I2C0_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterReceive(addr, data, num, xfer_pending, &I2C0_Resources);
}
static int32_t I2C0_SlaveTransmit (const uint8_t *data, uint32_t num) {
  return I2C_SlaveTransmit(data, num, &I2C0_Resources);
}
static int32_t I2C0_SlaveReceive (uint8_t *data, uint32_t num) {
  return I2C_SlaveReceive(data, num, &I2C0_Resources);
}
static int32_t I2C0_GetDataCount (void) {
  return I2C_GetDataCount(&I2C0_Resources);
}
static int32_t I2C0_Control (uint32_t control, uint32_t arg) {
  return I2C_Control(control, arg, &I2C0_Resources);
}
static ARM_I2C_STATUS I2C0_GetStatus (void) {
  return I2C_GetStatus(&I2C0_Resources);
}
static void TWIS0_event_handler (nrfx_twis_evt_t const * p_event) {
  return twis_event_handler (p_event, &I2C0_Resources);
}

/* I2C0 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C0 = {
  I2CX_GetVersion,
  I2CX_GetCapabilities,
  I2C0_Initialize,
  I2C0_Uninitialize,
  I2C0_PowerControl,
  I2C0_MasterTransmit,
  I2C0_MasterReceive,
  I2C0_SlaveTransmit,
  I2C0_SlaveReceive,
  I2C0_GetDataCount,
  I2C0_Control,
  I2C0_GetStatus
};
#endif
