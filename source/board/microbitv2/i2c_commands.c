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

// 'kvld' in hex - key valid
#define CFG_KEY             0x6b766c64

flashConfig_t gflashConfig = {
    .key = CFG_KEY,
    .fileName = STORAGE_CFG_FILENAME,
    .fileSize = STORAGE_CFG_FILESIZE,
    .fileVisible = STORAGE_CFG_FILEVISIBLE,
    .fileEncWindowStart = 0,
    .fileEncWindowEnd = 0,
};


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

    // Release COMBINED_SENSOR_INT
    gpio_disable_combined_int();
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

static void i2c_write_flash_callback(uint8_t* pData, uint8_t size) {
    i2cFlashCmd_t* pI2cCommand = (i2cFlashCmd_t*) pData;

    uint32_t status = 0;

    uint32_t storage_address = pI2cCommand->cmdData.write.addr2 << 16 |
                            pI2cCommand->cmdData.write.addr1 << 8 |
                            pI2cCommand->cmdData.write.addr0 << 0;
    uint32_t address = storage_address + STORAGE_ADDRESS_START;
    uint32_t length = __REV(pI2cCommand->cmdData.write.length);
    uint32_t data = (uint32_t) pI2cCommand->cmdData.write.data;

    switch (pI2cCommand->cmdId) {
        case gFlashDataWrite_c:
            /* Validate length field matches with I2C Write data */
            if (size == length + 8) {
                /* Address range and alignment validation done inside storage_program_page() */
                status = storage_program_page(address, length, (uint8_t *) data);

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
            if (address + length > STORAGE_ADDRESS_END) {
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
            uint32_t start_addr = address + STORAGE_ADDRESS_START;

            address = pI2cCommand->cmdData.erase.eAddr2 << 16 |
                            pI2cCommand->cmdData.erase.eAddr1 << 8 |
                            pI2cCommand->cmdData.erase.eAddr0 << 0;
            uint32_t end_addr = address + STORAGE_ADDRESS_START;

            /* Do address range validation */
            if (start_addr % DAPLINK_SECTOR_SIZE == 0 &&
                end_addr % DAPLINK_SECTOR_SIZE == 0 &&
                start_addr <= end_addr &&
                start_addr < STORAGE_ADDRESS_END &&
                end_addr < STORAGE_ADDRESS_END) {
                for (uint32_t addr = start_addr; addr <= end_addr && status == 0; addr += DAPLINK_SECTOR_SIZE) {
                    status = storage_erase_sector(addr);
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
                if (tempFileSize <= STORAGE_SIZE) {
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
        case gFlashCfgEncWindow_c:
            if (size == 1) {
                /* If size is 1 (only cmd id), this means it's a read */
                uint32_t tempFileEncWindowStart = __REV(gflashConfig.fileEncWindowStart);
                uint32_t tempFileEncWindowEnd = __REV(gflashConfig.fileEncWindowEnd);
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                i2c_fillBuffer((uint8_t*) &tempFileEncWindowStart, 1, sizeof(gflashConfig.fileEncWindowStart));
                i2c_fillBuffer((uint8_t*) &tempFileEncWindowEnd, 5, sizeof(gflashConfig.fileEncWindowEnd));
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
                    gflashConfig.fileEncWindowStart = tempFileEncWindowStart;
                    tempFileEncWindowStart = __REV(gflashConfig.fileEncWindowStart);
                    gflashConfig.fileEncWindowEnd = tempFileEncWindowEnd;
                    tempFileEncWindowEnd = __REV(gflashConfig.fileEncWindowEnd);

                    i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                    i2c_fillBuffer((uint8_t*) &tempFileEncWindowStart, 1, sizeof(gflashConfig.fileEncWindowStart));
                    i2c_fillBuffer((uint8_t*) &tempFileEncWindowEnd, 5, sizeof(gflashConfig.fileEncWindowEnd));
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
            if (0 != memcmp(&gflashConfig, (void *)STORAGE_CONFIG_ADDRESS, sizeof(flashConfig_t))) {
                status = storage_erase_sector(STORAGE_CONFIG_ADDRESS);

                if (status != 0) {
                    pI2cCommand->cmdId = gFlashError_c;
                }
                else {
                    status = storage_program_page(STORAGE_CONFIG_ADDRESS, sizeof(flashConfig_t), (uint8_t *) &gflashConfig);
                }
            }
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
        break;
        case gFlashCfgErase_c:
            // Erase flash sector containing flash config
            status = storage_erase_sector(STORAGE_CONFIG_ADDRESS);

            if (status != 0) {
                pI2cCommand->cmdId = gFlashError_c;
            }
            else {
                // Return flash config (RAM) to default values
                gflashConfig.key = CFG_KEY;
                memcpy(gflashConfig.fileName, STORAGE_CFG_FILENAME, 11);
                gflashConfig.fileSize = STORAGE_CFG_FILESIZE;
                gflashConfig.fileVisible = STORAGE_CFG_FILEVISIBLE;
                gflashConfig.fileEncWindowStart = 0;
                gflashConfig.fileEncWindowEnd = 0;
            }
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
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
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
        break;
        default:
            pI2cCommand->cmdId = gFlashError_c;
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
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
    // Load Config from Flash if present
    flashConfig_t *pflashConfigROM = (flashConfig_t *) STORAGE_CONFIG_ADDRESS;
    if (CFG_KEY == pflashConfigROM->key) {
        memcpy(&gflashConfig, pflashConfigROM, sizeof(flashConfig_t));
    }

    i2c_initialize();
    i2c_registerWriteCallback(i2c_write_comms_callback, I2C_SLAVE_NRF_KL_COMMS);
    i2c_registerReadCallback(i2c_read_comms_callback, I2C_SLAVE_NRF_KL_COMMS);
    i2c_registerWriteCallback(i2c_write_flash_callback, I2C_SLAVE_FLASH);
    i2c_registerReadCallback(i2c_read_flash_callback, I2C_SLAVE_FLASH);
}

flashConfig_t* i2c_cmds_get_storage_config() {
    return &gflashConfig;
}

void i2c_cmds_reset_storate_config() {
    gflashConfig = (flashConfig_t) {
        .key = CFG_KEY,
        .fileName = STORAGE_CFG_FILENAME,
        .fileSize = STORAGE_CFG_FILESIZE,
        .fileVisible = STORAGE_CFG_FILEVISIBLE,
        .fileEncWindowStart = 0,
        .fileEncWindowEnd = 0,
    };
}
