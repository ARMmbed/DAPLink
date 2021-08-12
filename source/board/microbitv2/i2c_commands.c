/**
 * @file    i2c_commands.c
 * @brief Microbit appliation layer for I2C operations
 *
 * DAPLink Interface Firmware
 * Copyright 2020 NXP
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

#include "i2c_commands.h"
#include "fsl_i2c.h"
#include "fsl_clock.h"
#include "fsl_port.h"
#include "fsl_flash.h"
#include "settings.h" // for config_get_overflow_detect
#include "main_interface.h"
#include "pwr_mon.h"
#include "power.h"
#include "microbitv2.h"

/* I2C source clock */
#define I2C_SLAVE_BASEADDR I2C1
#define I2C_SLAVE_CLK_SRC I2C1_CLK_SRC
#define I2C_SLAVE_CLK_FREQ CLOCK_GetFreq(I2C1_CLK_SRC)

#define I2C_SLAVE_LOWER_ADDR_7BIT   (0x70U)
#define I2C_SLAVE_UPPER_ADDR_7BIT   (0x72U)
#define I2C_DATA_LENGTH             (1024U + 8U)


static uint8_t g_slave_TX_buff[I2C_DATA_LENGTH];
static uint8_t g_slave_RX_buff[I2C_DATA_LENGTH];
i2c_slave_handle_t g_s_handle;
static volatile bool g_SlaveCompletionFlag = false;
static volatile bool g_SlaveRxFlag = false;
static uint8_t address_match = 0;
static uint32_t transferredCount = 0;

static i2cWriteCallback_t pfWriteCommsCallback = NULL;
static i2cReadCallback_t pfReadCommsCallback = NULL;
static i2cWriteCallback_t pfWriteFlashCallback = NULL;
static i2cReadCallback_t pfReadFlashCallback = NULL;

extern uint8_t i2c_wake_timeout;
extern bool i2c_allow_sleep;
extern uint16_t board_id_hex;
extern power_source_t power_source;
extern main_usb_connect_t usb_state;
extern app_power_mode_t interface_power_mode;
extern bool power_led_sleep_state_on;
extern bool automatic_sleep_on;
extern main_shutdown_state_t main_shutdown_state;
extern bool do_remount;
extern flashConfig_t gflashConfig;
extern flash_config_t g_flash;


static void i2c_write_comms_callback(uint8_t* pData, uint8_t size);
static void i2c_read_comms_callback(uint8_t* pData, uint8_t size);
static void i2c_write_flash_callback(uint8_t* pData, uint8_t size);
static void i2c_read_flash_callback(uint8_t* pData, uint8_t size);

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
            // Hack: Default driver can't differentiate between RX or TX on
            // completion event, so we set a flag here. Can't process more
            // than I2C_DATA_LENGTH bytes on RX
            memset(&g_slave_RX_buff, 0, sizeof(g_slave_RX_buff));
            xfer->data     = g_slave_RX_buff;
            xfer->dataSize = I2C_DATA_LENGTH;
            g_SlaveRxFlag = true;
            break;

        /*  Transfer done */
        case kI2C_SlaveCompletionEvent:
            g_SlaveCompletionFlag = true;
            xfer->data            = NULL;
            xfer->dataSize        = 0;
            transferredCount = xfer->transferredCount;
            
            // Default driver couldn't differentiate between RX or TX completion
            // Check flag set in kI2C_SlaveReceiveEvent
            
            // Ignore NOP cmd in I2C Write
            if (!(g_SlaveRxFlag && g_slave_RX_buff[0] == 0x00)) {
                main_board_event();
            }
        
            i2c_allow_sleep = false;
            break;

        default:
            g_SlaveCompletionFlag = false;
            break;
    }
}
// Hook function executed in the main task
void board_custom_event() {
    
    if (g_SlaveRxFlag) {
        if (pfWriteCommsCallback && address_match == I2C_SLAVE_NRF_KL_COMMS) {
            pfWriteCommsCallback(&g_slave_RX_buff[0], transferredCount);
        }
        if (pfWriteFlashCallback && address_match == I2C_SLAVE_FLASH) {
            pfWriteFlashCallback(&g_slave_RX_buff[0], transferredCount);
        }
    } else { 
        if (pfReadCommsCallback && address_match == I2C_SLAVE_NRF_KL_COMMS) {
            pfReadCommsCallback(&g_slave_TX_buff[0], transferredCount);
        }
        if (pfReadFlashCallback && address_match == I2C_SLAVE_FLASH) {
            pfReadFlashCallback(&g_slave_TX_buff[0], transferredCount);
        }
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

void i2c_initialize(void) {
    i2c_slave_config_t slaveConfig;
    
    i2c_init_pins();

    I2C_SlaveGetDefaultConfig(&slaveConfig);

    slaveConfig.addressingMode = kI2C_RangeMatch;
    slaveConfig.slaveAddress   = I2C_SLAVE_LOWER_ADDR_7BIT;
    slaveConfig.upperAddress   = I2C_SLAVE_UPPER_ADDR_7BIT;
    slaveConfig.enableWakeUp   = true;

    I2C_SlaveInit(I2C_SLAVE_BASEADDR, &slaveConfig, I2C_SLAVE_CLK_FREQ);
    
    NVIC_SetPriority(I2C1_IRQn, 0x00);  /* set highest priority */

    i2c_start_transfer();
    
    i2c_registerWriteCallback(i2c_write_comms_callback, I2C_SLAVE_NRF_KL_COMMS);
    i2c_registerReadCallback(i2c_read_comms_callback, I2C_SLAVE_NRF_KL_COMMS);
    i2c_registerWriteCallback(i2c_write_flash_callback, I2C_SLAVE_FLASH);
    i2c_registerReadCallback(i2c_read_flash_callback, I2C_SLAVE_FLASH);

    return;
}

void i2c_deinitialize(void) {
    I2C_SlaveDeinit(I2C_SLAVE_BASEADDR);
    return ;
}

status_t i2c_registerWriteCallback(i2cWriteCallback_t writeCallback, uint8_t slaveAddress)
{
    status_t status = kStatus_Success;
    
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
            status = kStatus_Fail;
            break;
    }
    
    return status;
}

status_t i2c_registerReadCallback(i2cReadCallback_t readCallback, uint8_t slaveAddress)
{
    status_t status = kStatus_Success;
    
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
            status = kStatus_Fail;
            break;
    }
    
    return status;
}

void i2c_clearBuffer (void) {
    memset(&g_slave_TX_buff, 0, sizeof(g_slave_TX_buff));
}

void i2c_fillBuffer (uint8_t* data, uint32_t position, uint32_t size) {
    if ((position + size) > I2C_DATA_LENGTH) {
        return;
    }
    
    for (uint32_t i = 0; i < size; i++) { 
            g_slave_TX_buff[position + i] = data[i];
    }
}

static void i2c_write_comms_callback(uint8_t* pData, uint8_t size) {
    i2cCommand_t* pI2cCommand = (i2cCommand_t*) pData;
    i2cCommand_t i2cResponse = {0};
    bool assert_interrupt = true;

    switch (pI2cCommand->cmdId) {
        case gReadRequest_c:
            i2cResponse.cmdId = gReadResponse_c;
            i2cResponse.cmdData.readRspCmd.propertyId = pI2cCommand->cmdData.readReqCmd.propertyId;
            switch (pI2cCommand->cmdData.readReqCmd.propertyId) {
                case gDAPLinkBoardVersion_c:
                    i2cResponse.cmdData.readRspCmd.dataSize = sizeof(board_id_hex);
                    memcpy(&i2cResponse.cmdData.readRspCmd.data, &board_id_hex, sizeof(board_id_hex));
                break;
                case gI2CProtocolVersion_c: {
                    uint16_t i2c_version = 1;
                    i2cResponse.cmdData.readRspCmd.dataSize = sizeof(i2c_version);
                    memcpy(&i2cResponse.cmdData.readRspCmd.data, &i2c_version, sizeof(i2c_version));
                }
                break;
                case gDAPLinkVersion_c: {
                    uint16_t daplink_version = DAPLINK_VERSION;
                    i2cResponse.cmdData.readRspCmd.dataSize = sizeof(daplink_version);
                    memcpy(&i2cResponse.cmdData.readRspCmd.data, &daplink_version, sizeof(daplink_version));
                }
                break;
                case gPowerState_c:
                    power_source = pwr_mon_get_power_source();
                    i2cResponse.cmdData.readRspCmd.dataSize = sizeof(power_source);
                    memcpy(&i2cResponse.cmdData.readRspCmd.data, &power_source, sizeof(power_source));
                break;
                case gPowerConsumption_c: {
                    uint32_t vin_voltage_uv = pwr_mon_get_vin_mv() * 1000;
                    uint32_t vbat_voltage_uv = pwr_mon_get_vbat_mv() * 1000;
                    i2cResponse.cmdData.readRspCmd.dataSize = sizeof(vin_voltage_uv) + sizeof(vbat_voltage_uv);
                    memcpy(&i2cResponse.cmdData.readRspCmd.data[0], &vbat_voltage_uv, sizeof(vbat_voltage_uv));
                    memcpy(&i2cResponse.cmdData.readRspCmd.data[sizeof(vbat_voltage_uv)], &vin_voltage_uv, sizeof(vin_voltage_uv));
                }
                break;
                case gUSBEnumerationState_c:
                    i2cResponse.cmdData.readRspCmd.dataSize = sizeof(usb_state);
                    memcpy(&i2cResponse.cmdData.readRspCmd.data, &usb_state, sizeof(usb_state));
                break;
                case gPowerMode_c:
                    i2cResponse.cmdId = gErrorResponse_c;
                    i2cResponse.cmdData.errorRspCmd.errorCode = gErrorReadDisallowed_c;
                break;
                default:
                    i2cResponse.cmdId = gErrorResponse_c;
                    i2cResponse.cmdData.errorRspCmd.errorCode = gErrorUnknownProperty_c;
                break;
            }
        break;
        case gReadResponse_c:
            i2cResponse.cmdId = gErrorResponse_c;
            i2cResponse.cmdData.errorRspCmd.errorCode = gErrorCommandDisallowed_c;
        break;
        case gWriteRequest_c:
            switch (pI2cCommand->cmdData.writeReqCmd.propertyId) {
                case gDAPLinkBoardVersion_c:
                case gI2CProtocolVersion_c:
                case gDAPLinkVersion_c:
                case gPowerState_c:
                case gPowerConsumption_c:
                case gUSBEnumerationState_c:
                    i2cResponse.cmdId = gErrorResponse_c;
                    i2cResponse.cmdData.errorRspCmd.errorCode = gErrorWriteDisallowed_c;
                break;
                case gPowerMode_c:
                    if (pI2cCommand->cmdData.writeReqCmd.dataSize == 1) {
                        if (pI2cCommand->cmdData.writeReqCmd.data[0] == kAPP_PowerModeVlls0) {
                            interface_power_mode = kAPP_PowerModeVlls0;
                            i2cResponse.cmdId = gWriteResponse_c;
                            i2cResponse.cmdData.writeRspCmd.propertyId = pI2cCommand->cmdData.writeReqCmd.propertyId;
                        } else { 
                            i2cResponse.cmdId = gErrorResponse_c;
                            i2cResponse.cmdData.errorRspCmd.errorCode = gErrorWriteFail_c;
                        }
                    } else {
                        i2cResponse.cmdId = gErrorResponse_c;
                        i2cResponse.cmdData.errorRspCmd.errorCode = gErrorWrongPropertySize_c;
                    }
                break;
                case gPowerLedSleepState_c:
                    if (pI2cCommand->cmdData.writeReqCmd.dataSize == 1) {
                        if (pI2cCommand->cmdData.writeReqCmd.data[0] != 0) {
                            power_led_sleep_state_on = true;
                        }
                        else {
                            power_led_sleep_state_on = false;
                        }
                        i2cResponse.cmdId = gWriteResponse_c;
                        i2cResponse.cmdData.writeRspCmd.propertyId = pI2cCommand->cmdData.writeReqCmd.propertyId;
                    } else {
                        i2cResponse.cmdId = gErrorResponse_c;
                        i2cResponse.cmdData.errorRspCmd.errorCode = gErrorWrongPropertySize_c;
                    }
                break;
                case gAutomaticSleep_c:
                    if (pI2cCommand->cmdData.writeReqCmd.dataSize == 1) {
                        if (pI2cCommand->cmdData.writeReqCmd.data[0] != 0) {
                            automatic_sleep_on = true;
                        }
                        else {
                            automatic_sleep_on = false;
                        }
                        i2cResponse.cmdId = gWriteResponse_c;
                        i2cResponse.cmdData.writeRspCmd.propertyId = pI2cCommand->cmdData.writeReqCmd.propertyId;
                    } else {
                        i2cResponse.cmdId = gErrorResponse_c;
                        i2cResponse.cmdData.errorRspCmd.errorCode = gErrorWrongPropertySize_c;
                    }
                break;
                default:
                    i2cResponse.cmdId = gErrorResponse_c;
                    i2cResponse.cmdData.errorRspCmd.errorCode = gErrorUnknownProperty_c;
                break;
            }
        break;
        case gWriteResponse_c:
        break;
        case gErrorResponse_c:
        break;
        case gNopCmd_c:
            assert_interrupt = false;
        break;
        default:
            i2cResponse.cmdId = gErrorResponse_c;
            i2cResponse.cmdData.errorRspCmd.errorCode = gErrorUnknownCommand_c;
        break;
    }
    
    if (assert_interrupt) {
        i2c_fillBuffer((uint8_t*) &i2cResponse, 0, sizeof(i2cResponse));
        // Response ready, assert COMBINED_SENSOR_INT
        PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_MuxAsGpio);
    }
}

static void i2c_read_comms_callback(uint8_t* pData, uint8_t size) {
    i2cCommand_t* pI2cCommand = (i2cCommand_t*) pData;

    switch (pI2cCommand->cmdId) {
        case gWriteResponse_c:
            switch (pI2cCommand->cmdData.writeRspCmd.propertyId) {
                case gPowerMode_c:
                    main_shutdown_state = MAIN_SHUTDOWN_REQUESTED;
                break;
            }
        break;
    }
    
    i2c_clearBuffer();
    
    // Release COMBINED_SENSOR_INT
    PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_PinDisabledOrAnalog);
}

static bool file_extension_allowed(const vfs_filename_t  filename)
{
    const char *valid_extensions[] = {
        "BIN",
        "TXT",
        "CSV",
        "HTM",
        "WAV",
    };
    uint32_t i;

    // Check for invalid starting characters
    for (i = 0; i < (sizeof((valid_extensions))/sizeof((valid_extensions)[0])); i++) {
        if (0 == memcmp(&filename[8], valid_extensions[i], 3)) {
            return true;
        }
    }

    // Some checks failed so file extension is invalid
    return false;
}

static uint32_t erase_storage_sector(uint32_t adr)
{
    int status = FLASH_Erase(&g_flash, adr, g_flash.PFlashSectorSize, kFLASH_apiEraseKey);
    if (status == kStatus_Success)
    {
        status = FLASH_VerifyErase(&g_flash, adr, g_flash.PFlashSectorSize, kFLASH_marginValueNormal);
    }
    return status;
}

static uint32_t program_storage_page(uint32_t adr, uint32_t sz, uint8_t *buf)
{
    /* Verify data is word aligned */
    util_assert(!((uint32_t)buf & 0x3));
    
    int status = FLASH_Program(&g_flash, adr, (uint32_t *) buf, sz);
    if (status == kStatus_Success)
    {
        // Must use kFlashMargin_User, or kFlashMargin_Factory for verify program
        status = FLASH_VerifyProgram(&g_flash, adr, sz,
                              (uint32_t *) buf, kFLASH_marginValueUser,
                              NULL, NULL);
    }
    return status;
}

static void i2c_write_flash_callback(uint8_t* pData, uint8_t size) {
    i2cFlashCmd_t* pI2cCommand = (i2cFlashCmd_t*) pData;
    
    uint32_t status = 0;
    
    uint32_t storage_address = pI2cCommand->cmdData.write.addr2 << 16 |
                            pI2cCommand->cmdData.write.addr1 << 8 |
                            pI2cCommand->cmdData.write.addr0 << 0;
    uint32_t address = storage_address + FLASH_STORAGE_ADDRESS;
    uint32_t length = __REV(pI2cCommand->cmdData.write.length);
    uint32_t data = (uint32_t) pI2cCommand->cmdData.write.data;

    i2c_clearBuffer();
    
    switch (pI2cCommand->cmdId) {
        case gFlashDataWrite_c:
            /* Validate length field matches with I2C Write data */
            if (size == length + 8) { 
                /* Address range and alignment validation done inside program_storage_page() */
                status = program_storage_page(address, length, (uint8_t *) data);
            
                if (0 != status) {
                    pI2cCommand->cmdId = gFlashError_c;
                    i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                }
                else {
                    /* Fill TX Buffer with received command args */
                    i2c_fillBuffer((uint8_t*) pI2cCommand, 0, sizeof(i2cFlashCmd_t) - 1024);
                    /* Fill TX Buffer with Flash Data Written */
                    i2c_fillBuffer((uint8_t*) address, sizeof(i2cFlashCmd_t) - 1024, length);
                }
            } else {
                pI2cCommand->cmdId = gFlashError_c;
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
            }
        break;
        case gFlashDataRead_c: {
            /* Do address range validation */
            if (address + length > (FLASH_CONFIG_ADDRESS + FLASH_INTERFACE_SIZE)) {
                pI2cCommand->cmdId = gFlashError_c;
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
            } else { 
                /* Fill TX Buffer with received command args */
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, sizeof(i2cFlashCmd_t) - 1024);
                /* Fill TX Buffer with Flash Data Read */
                i2c_fillBuffer((uint8_t*) address, sizeof(i2cFlashCmd_t) - 1024, length);
            }
        }
        break;
        case gFlashDataErase_c: {
            uint32_t address = pI2cCommand->cmdData.erase.sAddr2 << 16 |
                            pI2cCommand->cmdData.erase.sAddr1 << 8 |
                            pI2cCommand->cmdData.erase.sAddr0 << 0;
            uint32_t start_addr = address + FLASH_STORAGE_ADDRESS;
            
            address = pI2cCommand->cmdData.erase.eAddr2 << 16 |
                            pI2cCommand->cmdData.erase.eAddr1 << 8 |
                            pI2cCommand->cmdData.erase.eAddr0 << 0;
            uint32_t end_addr = address + FLASH_STORAGE_ADDRESS;
            
            /* Do address range validation */
            if (start_addr % DAPLINK_SECTOR_SIZE == 0 &&
                end_addr % DAPLINK_SECTOR_SIZE == 0 &&
                start_addr <= end_addr &&
                start_addr < (FLASH_CONFIG_ADDRESS + FLASH_INTERFACE_SIZE) &&
                end_addr < (FLASH_CONFIG_ADDRESS + FLASH_INTERFACE_SIZE)) {
                for (uint32_t addr = start_addr; addr <= end_addr && status == 0; addr += DAPLINK_SECTOR_SIZE) {
                    status = erase_storage_sector(addr);
                }
                
                if (status != 0) {
                    pI2cCommand->cmdId = gFlashError_c;
                }
            } else {
                pI2cCommand->cmdId = gFlashError_c;
            }
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);

        }
        break;
        case gFlashCfgFileName_c:        
             if (size == 1) {
                /* If size is 1 (only cmd id), this means it's a read */
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                i2c_fillBuffer((uint8_t*) &gflashConfig.fileName, 1, sizeof(gflashConfig.fileName));
            } else if (size == 12) {
                /* If size is 12 (cmd id + 11B data), this means it's a write */
                /* Validate 8.3 filename  */
                if (filename_valid((char *) pI2cCommand->cmdData.data)) {
                    // Check allowed extensions (.bin, .txt, .csv, .htm, .wav)
                    if (file_extension_allowed((char *) pI2cCommand->cmdData.data)) {
                        memcpy(gflashConfig.fileName, pI2cCommand->cmdData.data, 11);
                    }
                    // If disallowed extension is requested, .bin will be used
                    else { 
                        memcpy(gflashConfig.fileName, pI2cCommand->cmdData.data, 8);
                        memcpy(&gflashConfig.fileName[8], "BIN", 3);
                    }
                    i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                    i2c_fillBuffer((uint8_t*) &gflashConfig.fileName, 1, sizeof(gflashConfig.fileName));
                }
                else { 
                    // Send error if invalid filename
                    pI2cCommand->cmdId = gFlashError_c;
                    i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                }
            } else {
                pI2cCommand->cmdId = gFlashError_c;
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
            }
        break;
        case gFlashCfgFileSize_c:        
            if (size == 1) {
                /* If size is 1 (only cmd id), this means it's a read */
                uint32_t tempFileSize = __REV(gflashConfig.fileSize);
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                i2c_fillBuffer((uint8_t*) &tempFileSize, 1, sizeof(gflashConfig.fileSize));
            } else if (size == 5) {
                /* If size is 5 (cmd id + 4B data), this means it's a write */
                uint32_t tempFileSize = pI2cCommand->cmdData.data[0] << 24 |
                                        pI2cCommand->cmdData.data[1] << 16 |
                                        pI2cCommand->cmdData.data[2] << 8 |
                                        pI2cCommand->cmdData.data[3] << 0;
                
                /* Validate file size */
                if (tempFileSize <= (FLASH_INTERFACE_SIZE - FLASH_CONFIG_SIZE)) {
                    gflashConfig.fileSize = tempFileSize;
                    tempFileSize = __REV(gflashConfig.fileSize);
                    i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                    i2c_fillBuffer((uint8_t*) &tempFileSize, 1, sizeof(gflashConfig.fileSize));
                } else {
                    pI2cCommand->cmdId = gFlashError_c;
                    i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                }
            } else {
                pI2cCommand->cmdId = gFlashError_c;
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
            }
        break;
        case gFlashCfgFileVisible_c:
            if (size == 1) {
                /* If size is 1 (only cmd id), this means it's a read */
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                i2c_fillBuffer((uint8_t*) &gflashConfig.fileVisible, 1, sizeof(gflashConfig.fileVisible));
            } else if (size == 2) {
                /* If size is 2 (cmd id + 1B data), this means it's a write */
                gflashConfig.fileVisible = pI2cCommand->cmdData.data[0];
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                i2c_fillBuffer((uint8_t*) &gflashConfig.fileVisible, 1, sizeof(gflashConfig.fileVisible));
            } else {
                pI2cCommand->cmdId = gFlashError_c;
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
            }
        break;
        case gFlashCfgWrite_c:
            // Check first is config is already present in flash
            // If differences are found, erase and write new config
            if (0 != memcmp(&gflashConfig, (void *)FLASH_CONFIG_ADDRESS, sizeof(flashConfig_t))) {
                status = erase_storage_sector(FLASH_CONFIG_ADDRESS);

                if (status != 0) {
                    pI2cCommand->cmdId = gFlashError_c;
                }
                else {
                    status = program_storage_page(FLASH_CONFIG_ADDRESS, sizeof(flashConfig_t), (uint8_t *) &gflashConfig);
                }
            }
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
        break;
        case gFlashCfgErase_c:
            // Erase flash sector containing flash config
            status = erase_storage_sector(FLASH_CONFIG_ADDRESS);
            
            if (status != 0) {
                pI2cCommand->cmdId = gFlashError_c;
            }
            else {
                // Return flash config (RAM) to default values
                gflashConfig.key = CFG_KEY;
                memcpy(gflashConfig.fileName, FLASH_CFG_FILENAME, 11);
                gflashConfig.fileSize = FLASH_CFG_FILESIZE;
                gflashConfig.fileVisible = FLASH_CFG_FILEVISIBLE;
            }
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
        break;
        case gFlashStorageSize_c:
            pI2cCommand->cmdData.data[0] = (FLASH_INTERFACE_SIZE - FLASH_CONFIG_SIZE)/1024;
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 2);
        break;
        case gFlashSectorSize_c:
            pI2cCommand->cmdData.data[0] = (DAPLINK_SECTOR_SIZE >> 8) & 0xFF;
            pI2cCommand->cmdData.data[1] = DAPLINK_SECTOR_SIZE & 0xFF;
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 3);
        break;
        case gFlashRemountMSD_c:
            do_remount = true;
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
        break;
        default:
            pI2cCommand->cmdId = gFlashError_c;
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
        break;
    }
    
    // Response ready, assert COMBINED_SENSOR_INT
    PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_MuxAsGpio);
}

static void i2c_read_flash_callback(uint8_t* pData, uint8_t size) {
    i2c_clearBuffer();
    
    // Release COMBINED_SENSOR_INT
    PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_PinDisabledOrAnalog);
}
