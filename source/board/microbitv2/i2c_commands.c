/**
 * @file    i2c_commands.c
 * @brief Microbit application layer for I2C operations
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

#include <string.h>

#include "i2c_commands.h"
#include "i2c.h"
#include "main_interface.h"
#include "pwr_mon.h"
#include "power.h"
#include "storage.h"
#include "gpio_extra.h"
#include "microbitv2.h"


extern uint16_t board_id_hex;
extern power_source_t power_source;
extern main_usb_connect_t usb_state;
extern microbit_if_power_mode_t interface_power_mode;
extern bool power_led_sleep_state_on;
extern bool automatic_sleep_on;
extern main_shutdown_state_t main_shutdown_state;
extern bool do_remount;


static void i2c_write_comms_callback(uint8_t* pData, uint8_t size);
static void i2c_read_comms_callback(uint8_t* pData, uint8_t size);
static void i2c_write_flash_callback(uint8_t* pData, uint8_t size);
static void i2c_read_flash_callback(uint8_t* pData, uint8_t size);


static void i2c_write_comms_callback(uint8_t* pData, uint8_t size) {
    i2cCommand_t* pI2cCommand = (i2cCommand_t*) pData;
    i2cCommand_t i2cResponse = {0};
    bool assert_interrupt = true;

    if (size < 1) {
        return;
    }

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
                    uint16_t i2c_version = I2C_PROTOCOL_VERSION;
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
                        if (pI2cCommand->cmdData.writeReqCmd.data[0] == MB_POWER_DOWN) {
                            interface_power_mode = MB_POWER_DOWN;
                            main_shutdown_state = MAIN_SHUTDOWN_REQUESTED;
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
        gpio_assert_combined_int();
    }
}

static void i2c_read_comms_callback(uint8_t* pData, uint8_t size) {
    // Release COMBINED_SENSOR_INT
    gpio_disable_combined_int();
}

static void i2c_write_flash_callback(uint8_t* pData, uint8_t size) {
    i2cFlashCmd_t* pI2cCommand = (i2cFlashCmd_t*) pData;
    uint32_t storage_address = pI2cCommand->cmdData.write.addr2 << 16 |
                            pI2cCommand->cmdData.write.addr1 << 8 |
                            pI2cCommand->cmdData.write.addr0 << 0;
    uint32_t length = __REV(pI2cCommand->cmdData.write.length);

    switch (pI2cCommand->cmdId) {
        case gFlashDataWrite_c:
            /* Validate length field matches with I2C Write data */
            if (size == length + 8) {
                storage_status_t status = storage_write(storage_address, length, &pI2cCommand->cmdData.write.data[0]);
                if (STORAGE_SUCCESS == status) {
                    /* Echo back all the received data */
                    i2c_fillBuffer((uint8_t *)pI2cCommand, 0, size);
                } else {
                    i2c_fillBufferHead(gFlashError_c);
                }
            } else {
                i2c_fillBufferHead(gFlashError_c);
            }
        break;
        case gFlashDataRead_c: {
            /* Do address range validation */
            uint8_t* storage_data = storage_get_data_pointer(storage_address);
            if (storage_data != NULL) {
                /* Fill TX Buffer with received command args */
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 8);
                /* Fill TX Buffer with Flash Data Read */
                i2c_fillBuffer(storage_data, 8, length);
            } else {
                i2c_fillBufferHead(gFlashError_c);
            }
        }
        break;
        case gFlashDataErase_c: {
            uint32_t start_addr = pI2cCommand->cmdData.erase.sAddr2 << 16 |
                            pI2cCommand->cmdData.erase.sAddr1 << 8 |
                            pI2cCommand->cmdData.erase.sAddr0 << 0;
            uint32_t end_addr = pI2cCommand->cmdData.erase.eAddr2 << 16 |
                            pI2cCommand->cmdData.erase.eAddr1 << 8 |
                            pI2cCommand->cmdData.erase.eAddr0 << 0;
            if (storage_erase_range(start_addr, end_addr) == STORAGE_SUCCESS) {
                i2c_fillBufferHead(pI2cCommand->cmdId);
            } else {
                i2c_fillBufferHead(gFlashError_c);
            }
        }
        break;
        case gFlashCfgFileName_c:
             if (size == 1) {
                /* If size is 1 (only cmd id), this means it's a read */
                i2c_fillBufferHead(pI2cCommand->cmdId);
                i2c_fillBuffer((uint8_t *)storage_cfg_get_filename(), 1, STORAGE_CFG_FILENAME_SIZE);
            } else if (size == (1 + STORAGE_CFG_FILENAME_SIZE)) {
                /* If size is 12 (cmd id + 11B data), this means it's a write */
                storage_status_t status = storage_cfg_set_filename((char *)pI2cCommand->cmdData.data);
                if (STORAGE_SUCCESS == status) {
                    i2c_fillBufferHead(pI2cCommand->cmdId);
                    i2c_fillBuffer(pI2cCommand->cmdData.data, 1, STORAGE_CFG_FILENAME_SIZE);
                } else {
                    i2c_fillBufferHead(gFlashError_c);
                }
            } else {
                i2c_fillBufferHead(gFlashError_c);
            }
        break;
        case gFlashCfgFileSize_c:
            if (size == 1) {
                /* If size is 1 (only cmd id), this means it's a read */
                uint32_t tempFileSize = __REV(storage_cfg_get_file_size());
                i2c_fillBufferHead(pI2cCommand->cmdId);
                i2c_fillBuffer((uint8_t*) &tempFileSize, 1, sizeof(tempFileSize));
            } else if (size == 5) {
                /* If size is 5 (cmd id + 4B data), this means it's a write */
                //uint32_t tempFileSize = __REV((uint32_t)pI2cCommand->cmdData.data);
                uint32_t tempFileSize = pI2cCommand->cmdData.data[0] << 24 |
                                        pI2cCommand->cmdData.data[1] << 16 |
                                        pI2cCommand->cmdData.data[2] << 8 |
                                        pI2cCommand->cmdData.data[3] << 0;
                storage_status_t status = storage_cfg_set_file_size(tempFileSize);
                if (status == STORAGE_SUCCESS) {
                    i2c_fillBufferHead(pI2cCommand->cmdId);
                    i2c_fillBuffer(pI2cCommand->cmdData.data, 1, sizeof(tempFileSize));
                } else {
                    i2c_fillBufferHead(gFlashError_c);
                }
            } else {
                i2c_fillBufferHead(gFlashError_c);
            }
        break;
        case gFlashCfgEncWindow_c:
            if (size == 1) {
                /* If size is 1 (only cmd id), this means it's a read */
                uint32_t tempFileEncWindowStart = __REV(storage_cfg_get_encoding_start());
                uint32_t tempFileEncWindowEnd = __REV(storage_cfg_get_encoding_end());
                i2c_fillBufferHead(pI2cCommand->cmdId);
                i2c_fillBuffer((uint8_t*) &tempFileEncWindowStart, 1, sizeof(tempFileEncWindowStart));
                i2c_fillBuffer((uint8_t*) &tempFileEncWindowEnd, 5, sizeof(tempFileEncWindowEnd));
            } else if (size == 9) {
                /* If size is 9 (cmd id + 8B data), this means it's a write */
                uint32_t tempFileEncWindowStart = pI2cCommand->cmdData.data[0] << 24 |
                                        pI2cCommand->cmdData.data[1] << 16 |
                                        pI2cCommand->cmdData.data[2] << 8 |
                                        pI2cCommand->cmdData.data[3] << 0;
                uint32_t tempFileEncWindowEnd = pI2cCommand->cmdData.data[4] << 24 |
                                        pI2cCommand->cmdData.data[5] << 16 |
                                        pI2cCommand->cmdData.data[6] << 8 |
                                        pI2cCommand->cmdData.data[7] << 0;

                /* Validate encoding window */
                if (tempFileEncWindowStart <= tempFileEncWindowEnd) {
                    storage_cfg_set_encoding_window(tempFileEncWindowStart, tempFileEncWindowEnd);
                    /* Echo back all the received data */
                    i2c_fillBuffer((uint8_t *)pI2cCommand, 0, size);
                } else {
                    i2c_fillBufferHead(gFlashError_c);
                }
            } else {
                i2c_fillBufferHead(gFlashError_c);
            }
        break;
        case gFlashCfgFileVisible_c:
            if (size == 1) {
                /* If size is 1 (only cmd id), this means it's a read */
                i2c_fillBufferHead(pI2cCommand->cmdId);
                uint8_t file_visible = storage_cfg_get_file_visible();
                i2c_fillBuffer((uint8_t*) &file_visible, 1, sizeof(file_visible));
            } else if (size == 2) {
                /* If size is 2 (cmd id + 1B data), this means it's a write */
                storage_cfg_set_file_visible(pI2cCommand->cmdData.data[0]);
                i2c_fillBufferHead(pI2cCommand->cmdId);
                i2c_fillBuffer(pI2cCommand->cmdData.data, 1, sizeof(pI2cCommand->cmdData.data[0]));
            } else {
                i2c_fillBufferHead(gFlashError_c);
            }
        break;
        case gFlashCfgWrite_c:
            if (storage_cfg_write() == STORAGE_SUCCESS) {
                i2c_fillBufferHead(pI2cCommand->cmdId);
            } else {
                i2c_fillBufferHead(gFlashError_c);
            }
        break;
        case gFlashCfgErase_c:
            if (storage_cfg_erase() == STORAGE_SUCCESS) {
                i2c_fillBufferHead(pI2cCommand->cmdId);
            } else {
                i2c_fillBufferHead(gFlashError_c);
            }
        break;
        case gFlashStorageSize_c:
            pI2cCommand->cmdData.data[0] = STORAGE_SECTOR_COUNT;
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 2);
        break;
        case gFlashSectorSize_c:
            pI2cCommand->cmdData.data[0] = (DAPLINK_SECTOR_SIZE >> 8) & 0xFF;
            pI2cCommand->cmdData.data[1] = DAPLINK_SECTOR_SIZE & 0xFF;
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 3);
        break;
        case gFlashRemountMSD_c:
            do_remount = true;
            i2c_fillBufferHead(pI2cCommand->cmdId);
        break;
        default:
            i2c_fillBufferHead(gFlashError_c);
        break;
    }

    // Response ready, assert COMBINED_SENSOR_INT
    gpio_assert_combined_int();
}

static void i2c_read_flash_callback(uint8_t* pData, uint8_t size) {
    // Release COMBINED_SENSOR_INT
    gpio_disable_combined_int();
}

void i2c_cmds_init() {
    // Needs storage_init() to have been run, currently called in microbitv2.c prerun_board_config()
    i2c_initialize();
    i2c_registerWriteCallback(i2c_write_comms_callback, I2C_SLAVE_NRF_KL_COMMS);
    i2c_registerReadCallback(i2c_read_comms_callback, I2C_SLAVE_NRF_KL_COMMS);
    i2c_registerWriteCallback(i2c_write_flash_callback, I2C_SLAVE_FLASH);
    i2c_registerReadCallback(i2c_read_flash_callback, I2C_SLAVE_FLASH);
}
