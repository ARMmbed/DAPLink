/**
 * @file    i2c.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2020 NXP
 * Copyright 2021 Micro:bit Educational foundation
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

#include "i2c.h"
#include "i2c_commands.h"

#include "main_interface.h"

#include "fsl_i2c.h"
#include "fsl_clock.h"
#include "fsl_port.h"

/* I2C source clock */
#define I2C_SLAVE_BASEADDR I2C1
#define I2C_SLAVE_CLK_SRC I2C1_CLK_SRC
#define I2C_SLAVE_CLK_FREQ CLOCK_GetFreq(I2C1_CLK_SRC)

static uint16_t g_slave_TX_i = 0;
static uint8_t __ALIGNED(4) g_slave_TX_buff[I2C_DATA_LENGTH];
static uint8_t __ALIGNED(4) g_slave_RX_buff[I2C_DATA_LENGTH];

i2c_slave_handle_t g_s_handle;
static volatile bool g_SlaveCompletionFlag = false;
static volatile bool g_SlaveRxFlag = false;
static uint8_t address_match = 0;
static uint32_t transferredCount = 0;

static i2cCallback_t pfWriteCommsCallback = NULL;
static i2cCallback_t pfReadCommsCallback = NULL;
static i2cCallback_t pfWriteFlashCallback = NULL;
static i2cCallback_t pfReadFlashCallback = NULL;

static uint8_t i2c_wake_timeout = 0;
static bool i2c_allow_sleep = true;

static void i2c_clearTxBuffer(void);
static void i2c_scheduleCallback(void);


static void i2c_slave_callback(I2C_Type *base, i2c_slave_transfer_t *xfer, void *userData) {
    switch (xfer->event)
    {
        /*  Address match event */
        case kI2C_SlaveAddressMatchEvent:
            xfer->data     = NULL;
            xfer->dataSize = 0;
            // fsl_i2c.c IRQ updated in fsl_i2c_mod.c to include I2C D register
            address_match = *(uint8_t*)userData >> 1;
            i2c_wake_timeout = 3;   // 3 * 30ms tick = 90ms timeout
            break;
        /*  Transmit request */
        case kI2C_SlaveTransmitEvent:
            /*  Update information for transmit process */
            xfer->data     = g_slave_TX_buff;
            xfer->dataSize = I2C_DATA_LENGTH;
            g_SlaveRxFlag = false;
            break;

        /*  Receive request */
        case kI2C_SlaveReceiveEvent:
            /*  Update information for received process */
            // We don't need to clear g_slave_RX_buff because we also have the
            // transferredCount to know what data is valid
            xfer->data     = g_slave_RX_buff;
            xfer->dataSize = I2C_DATA_LENGTH;

            // Hack: Default driver can't differentiate between RX or TX on
            // completion event, so we set a flag here. Can't process more
            // than I2C_DATA_LENGTH bytes on RX
            g_SlaveRxFlag = true;

            break;

        /*  Transfer done */
        case kI2C_SlaveCompletionEvent:
            g_SlaveCompletionFlag = true;
            xfer->data            = NULL;
            xfer->dataSize        = 0;
            transferredCount = xfer->transferredCount;

            i2c_allow_sleep = false;

            // Default driver couldn't differentiate between RX or TX completion
            // Check flag set in kI2C_SlaveReceiveEvent
            // Ignore NOP cmd in I2C Write
            if (!(g_SlaveRxFlag && g_slave_RX_buff[0] == gNopCmd_c)) {
                // Only process events if the busy error was not read
                i2c_scheduleCallback();
            }
            break;

        default:
            g_SlaveCompletionFlag = false;
            break;
    }
}

static void i2c_scheduleCallback() {
    if (g_SlaveRxFlag) {
        // Raise an RTOS event to run the heavier I2C RX callback in main task
        main_board_event();
    } else {
        // Run the I2C TX callback in the interrupt context
        if (pfReadCommsCallback && address_match == I2C_SLAVE_NRF_KL_COMMS) {
            pfReadCommsCallback(&g_slave_TX_buff[0], transferredCount);
        }
        if (pfReadFlashCallback && address_match == I2C_SLAVE_FLASH) {
            pfReadFlashCallback(&g_slave_TX_buff[0], transferredCount);
        }
        i2c_clearTxBuffer();
        i2c_allow_sleep = true;
    }
}

// Hook function executed in the main task
void board_custom_event() {
    if (pfWriteCommsCallback && address_match == I2C_SLAVE_NRF_KL_COMMS) {
        pfWriteCommsCallback(&g_slave_RX_buff[0], transferredCount);
    }
    if (pfWriteFlashCallback && address_match == I2C_SLAVE_FLASH) {
        pfWriteFlashCallback(&g_slave_RX_buff[0], transferredCount);
    }
    i2c_allow_sleep = true;
}

static void i2c_init_pins(void) {
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC1 is configured as I2C1_SCL */
    PORT_SetPinMux(PORTC, 1U, kPORT_MuxAlt2);

    /* PORTC2 is configured as I2C1_SDA */
    PORT_SetPinMux(PORTC, 2U, kPORT_MuxAlt2);
}

static int32_t i2c_start_transfer(void) {
    memset(&g_s_handle, 0, sizeof(g_s_handle));

    I2C_SlaveTransferCreateHandle(I2C_SLAVE_BASEADDR, &g_s_handle,
            i2c_slave_callback, &address_match);

    /* Set up slave transfer. */
    I2C_SlaveTransferNonBlocking(I2C_SLAVE_BASEADDR, &g_s_handle,
            kI2C_SlaveCompletionEvent | kI2C_SlaveAddressMatchEvent);

    // i2c handle will be busy until an address match
    g_s_handle.isBusy = false;

    return 1;
}

void i2c_initialize() {
    i2c_slave_config_t slaveConfig;

    i2c_init_pins();

    i2c_clearState();

    I2C_SlaveGetDefaultConfig(&slaveConfig);

    slaveConfig.addressingMode = kI2C_RangeMatch;
    slaveConfig.slaveAddress   = I2C_SLAVE_NRF_KL_COMMS;
    slaveConfig.upperAddress   = I2C_SLAVE_FLASH;
    slaveConfig.enableWakeUp   = true;

    I2C_SlaveInit(I2C_SLAVE_BASEADDR, &slaveConfig, I2C_SLAVE_CLK_FREQ);

    NVIC_SetPriority(I2C1_IRQn, 0x00);  /* set highest priority */

    i2c_start_transfer();

    return;
}

void i2c_deinitialize(void) {
    I2C_SlaveDeinit(I2C_SLAVE_BASEADDR);
    return ;
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

void i2c_clearState(void)
{
    i2c_clearTxBuffer();
    memset(&g_slave_RX_buff, 0, sizeof(g_slave_RX_buff));
    g_SlaveCompletionFlag = false;
    g_SlaveRxFlag = false;
    address_match = 0;
    transferredCount = 0;
    i2c_wake_timeout = 0;
    i2c_allow_sleep = true;
}

void i2c_clearTxBuffer(void)
{
    memset(&g_slave_TX_buff, 0, g_slave_TX_i);
    g_slave_TX_i = 0;
    // Set the buffer with the "busy flag"
    g_slave_TX_buff[0] = gErrorResponse_c;
    g_slave_TX_buff[1] = gErrorBusy_c;
}

void i2c_fillBuffer (uint8_t* data, uint32_t position, uint32_t size) {
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
    return i2c_allow_sleep && !g_s_handle.isBusy && i2c_wake_timeout == 0;
}

void i2c_30ms_tick()
{
    if (i2c_wake_timeout > 0) {
        i2c_wake_timeout--;
    }
}
