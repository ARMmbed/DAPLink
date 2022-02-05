/**
 * @file    i2c.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2021 Micro:bit Educational Foundation
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

#include <string.h>

#include "main_interface.h"

#include "i2c.h"
#include "i2c_commands.h"

#include "Driver_I2C.h"

// Set to 1 to enable debugging
#define DEBUG_I2C       0

#if DEBUG_I2C
#include "daplink_debug.h"
#define debug_i2c_printf        debug_msg
#define debug_i2c_data          debug_data
#define debug_i2c_array(a, i)   do {   \
        for (int x = 0; x < i; x++) debug_msg("%02x ", a[x]); \
        debug_data((uint8_t *)"\n", 1); \
    } while(0)
#else
#define debug_i2c_printf(...)
#define debug_i2c_data(...)
#define debug_i2c_array(...)
#endif

extern ARM_DRIVER_I2C            Driver_I2C0;
static ARM_DRIVER_I2C *I2Cdrv = &Driver_I2C0;

static uint16_t g_slave_TX_i = 0;
static uint8_t __ALIGNED(4) g_slave_TX_buff[I2C_DATA_LENGTH] = { 0 };
static uint8_t __ALIGNED(4) g_slave_RX_buff[I2C_DATA_LENGTH] = { 0 };

static i2cCallback_t pfWriteCommsCallback = NULL;
static i2cCallback_t pfReadCommsCallback = NULL;
static i2cCallback_t pfWriteFlashCallback = NULL;
static i2cCallback_t pfReadFlashCallback = NULL;
typedef struct callbackToExecute_s {
    i2cCallback_t pfCallback;
    uint8_t*    pData;
    uint8_t     size;
} callbackToExecute_t;
static callbackToExecute_t callbackToExecute = {
    .pfCallback = NULL,
    .pData = NULL,
    .size = 0,
};

static uint8_t i2c_wake_timeout = 0;
static bool i2c_allow_sleep = true;

static void i2c_clearTxBuffer(void);


static void i2c_scheduleCallback(i2cCallback_t callback, uint8_t* pData, uint8_t size)
{
    if ((callback == pfReadCommsCallback) || (callback == pfReadFlashCallback)) {
        // Run the I2C TX callback in the interrupt context
        debug_i2c_data((uint8_t *)"[cbTx]\n", 7);
        callback(pData, size);
        i2c_clearTxBuffer();
    } else {
        // Raise an RTOS event to run the heavier I2C RX callback in main task
        callbackToExecute.pfCallback = callback;
        callbackToExecute.pData = pData;
        callbackToExecute.size = size;
        i2c_allow_sleep = false;
        main_board_event();
    }
}

// Hook function executed in the main task
void board_custom_event()
{
    if (callbackToExecute.pfCallback != NULL) {
        debug_i2c_data((uint8_t *)"[cbRx]\n", 7);
        callbackToExecute.pfCallback(callbackToExecute.pData, callbackToExecute.size);

        // Clear the callback data
        callbackToExecute.pfCallback = NULL;
        callbackToExecute.pData = NULL;
        callbackToExecute.size = 0;

        i2c_allow_sleep = true;
    }
}

static void i2c_signalEvent(uint32_t event) {
    static uint32_t prev_event = 0;

    // debug_i2c_printf("evt[%d] ", event);

    if ((event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) ||  /* Less data was transferred than requested */
        (event & ARM_I2C_EVENT_ADDRESS_NACK)        ||  /* Slave address was not acknowledged */
        (event & ARM_I2C_EVENT_ARBITRATION_LOST)    ||  /* Master lost bus arbitration */
        (event & ARM_I2C_EVENT_BUS_CLEAR)) {            /* Bus clear operation completed */
        // TODO: Deal with errors
        debug_i2c_printf("? %d", event);
    }

    if (event & ARM_I2C_EVENT_TRANSFER_DONE) {
        // debug_i2c_data((uint8_t *)"[d", 2);

        if (prev_event & ARM_I2C_EVENT_SLAVE_RECEIVE) {
            int32_t data_count = I2Cdrv->GetDataCount();
            debug_i2c_printf("[R0%x]\n", g_slave_RX_buff[0]);
            // debug_i2c_array(g_slave_RX_buff, data_count);

            // Ignore NOP commands and 0 length transmissions
            if ((data_count != 0) && (g_slave_RX_buff[0] != gNopCmd_c)) {
                if (event & EXTENSION_I2C_EVENT_SLAVE_ADDR_0) {
                    i2c_scheduleCallback(pfWriteCommsCallback, &g_slave_RX_buff[0], data_count);
                } else if (event & EXTENSION_I2C_EVENT_SLAVE_ADDR_1) {
                    i2c_scheduleCallback(pfWriteFlashCallback, &g_slave_RX_buff[0], data_count);
                }
            }
        } else if (prev_event & ARM_I2C_EVENT_SLAVE_TRANSMIT) {
            int32_t data_count = I2Cdrv->GetDataCount();
            debug_i2c_printf("[T0%x]\n", g_slave_TX_buff[0]);
            // debug_i2c_array(g_slave_TX_buff, 5);

            if (event & EXTENSION_I2C_EVENT_SLAVE_ADDR_0) {
                i2c_scheduleCallback(pfReadCommsCallback, &g_slave_TX_buff[0], data_count);
            } else if (event & EXTENSION_I2C_EVENT_SLAVE_ADDR_1) {
                i2c_scheduleCallback(pfReadFlashCallback, &g_slave_TX_buff[0], data_count);
            }
        }
    }
    if (event & ARM_I2C_EVENT_BUS_ERROR) {
        /* Invalid start/stop position detected */
        debug_i2c_data((uint8_t *)"E\n", 2);
    }
    if (event & ARM_I2C_EVENT_GENERAL_CALL) {
        /* Slave was addressed with a general call address */
        debug_i2c_data((uint8_t *)"g\n", 2);
    }
    if (event & ARM_I2C_EVENT_SLAVE_RECEIVE) {
        int32_t ret = I2Cdrv->SlaveReceive(&g_slave_RX_buff[0], I2C_DATA_LENGTH);
        i2c_wake_timeout = 4;   // 4 * 30ms tick = 120ms timeout
        // debug_i2c_printf("rx[%d]\n", ret);
    }
    if (event & ARM_I2C_EVENT_SLAVE_TRANSMIT) {
        int32_t ret = I2Cdrv->SlaveTransmit(&g_slave_TX_buff[0], I2C_DATA_LENGTH);
        i2c_wake_timeout = 4;   // 4 * 30ms tick = 120ms timeout
        // debug_i2c_printf("tx[%d]\n", ret);
    }

    prev_event = event;
}

void i2c_initialize()
{
    i2c_clearState();

    // I2C addresses configured via default values from RTE_Device.h
    I2Cdrv->Initialize(i2c_signalEvent);
    I2Cdrv->PowerControl(ARM_POWER_FULL);
}

void i2c_deinitialize()
{
    I2Cdrv->Uninitialize();
}

i2c_status_t i2c_registerWriteCallback(i2cCallback_t writeCallback, uint8_t slaveAddress)
{
    i2c_status_t status = I2C_STATUS_SUCCESS;

    switch (slaveAddress){
        case I2C_SLAVE_NRF_KL_COMMS:
            pfWriteCommsCallback = writeCallback;
            break;
        case I2C_SLAVE_HID:
            break;
        case I2C_SLAVE_FLASH:
            pfWriteFlashCallback = writeCallback;
            break;
        default:
            status = I2C_STATUS_FAIL;
            break;
    }

    return status;
}

i2c_status_t i2c_registerReadCallback(i2cCallback_t readCallback, uint8_t slaveAddress)
{
    i2c_status_t status = I2C_STATUS_SUCCESS;

    switch (slaveAddress) {
        case I2C_SLAVE_NRF_KL_COMMS:
            pfReadCommsCallback = readCallback;
            break;
        case I2C_SLAVE_HID:
            break;
        case I2C_SLAVE_FLASH:
            pfReadFlashCallback = readCallback;
            break;
        default:
            status = I2C_STATUS_FAIL;
            break;
    }

    return status;
}

void i2c_clearState()
{
    i2c_clearTxBuffer();
    memset(&g_slave_RX_buff, 0, sizeof(g_slave_RX_buff));
    callbackToExecute.pfCallback = NULL;
    callbackToExecute.pData = NULL;
    callbackToExecute.size = 0;
    i2c_wake_timeout = 0;
    i2c_allow_sleep = true;
}

void i2c_clearTxBuffer()
{
    memset(&g_slave_TX_buff, 0, g_slave_TX_i);
    g_slave_TX_i = 0;
    // Set the buffer with the "busy flag"
    g_slave_TX_buff[0] = gErrorResponse_c;
    g_slave_TX_buff[1] = gErrorBusy_c;
}

void i2c_fillBuffer(uint8_t* data, uint32_t position, uint32_t size)
{
    if ((position + size) > I2C_DATA_LENGTH) {
        return;
    }
    memcpy(g_slave_TX_buff + position, data, size);
    if (position + size > g_slave_TX_i) {
        g_slave_TX_i = position + size;
    }
    i2c_allow_sleep = false;
}

void i2c_fillBufferHead(uint8_t data)
{
    g_slave_TX_buff[0] = data;
    if (0 == g_slave_TX_i) {
        g_slave_TX_i = 1;
    }
    i2c_allow_sleep = false;
}

bool i2c_canSleep()
{
    ARM_I2C_STATUS status = I2Cdrv->GetStatus();
    return i2c_allow_sleep && !status.busy && i2c_wake_timeout == 0;
}

void i2c_30ms_tick()
{
    if (i2c_wake_timeout > 0) {
        i2c_wake_timeout--;
    }
}
