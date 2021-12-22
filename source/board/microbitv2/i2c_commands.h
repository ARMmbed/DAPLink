/**
 * @file    i2c_commands.h
 * @brief
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

#ifndef I2C_COMMANDS_H
#define I2C_COMMANDS_H

#include <stdint.h>
#include <stdbool.h>

#include "cmsis_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 7-bit addresses */
#define I2C_SLAVE_NRF_KL_COMMS      (0x70U)
#define I2C_SLAVE_HID               (0x71U)
#define I2C_SLAVE_FLASH             (0x72U)

#define I2C_PROTOCOL_VERSION        (0x02)

/*! i2c command Id type enumeration */
typedef enum cmdId_tag {
    gNopCmd_c        = 0x00,
    gReadRequest_c   = 0x10,
    gReadResponse_c  = 0x11,
    gWriteRequest_c  = 0x12,
    gWriteResponse_c = 0x13,
    gErrorResponse_c = 0x20
} cmdId_t;

/*! property Id type enumeration */
typedef enum propertyId_tag {
    gDAPLinkBoardVersion_c  = 0x01,
    gI2CProtocolVersion_c   = 0x02,
    gDAPLinkVersion_c       = 0x03,
    gPowerState_c           = 0x04,
    gPowerConsumption_c     = 0x05,
    gUSBEnumerationState_c  = 0x06,
    gPowerMode_c            = 0x07,
    gPowerLedSleepState_c   = 0x08,
    gUserEvent_c            = 0x09,
    gAutomaticSleep_c       = 0x0A
} propertyId_t;

/*! user event type enumeration */
typedef enum userEventType_tag {
    gWakeFromResetButton_c  = 0x01,
    gWakeFromWakeOnEdge_c   = 0x02,
    gResetButtonLongPress_c = 0x03,
} userEventType_t;

/*! property Id type enumeration */
typedef enum errorCode_tag {
    gErrorSuccess               = 0x30,
    gErrorIncompleteCommand_c   = 0x31,
    gErrorUnknownCommand_c      = 0x32,
    gErrorCommandDisallowed_c   = 0x33,
    gErrorUnknownProperty_c     = 0x34,
    gErrorWrongPropertySize_c   = 0x35,
    gErrorReadDisallowed_c      = 0x36,
    gErrorWriteDisallowed_c     = 0x37,
    gErrorWriteFail_c           = 0x38,
    gErrorBusy_c                = 0x39
} errorCode_t;

typedef __PACKED_STRUCT readReqCmd_tag {
    uint8_t     propertyId;
} readReqCmd_t;

#define MAX_PROPERTY_SIZE   8

typedef __PACKED_STRUCT readRspCmd_tag {
    uint8_t     propertyId;
    uint8_t     dataSize;
    uint8_t     data[MAX_PROPERTY_SIZE];
} readRspCmd_t;

typedef __PACKED_STRUCT writeReqCmd_tag {
    uint8_t     propertyId;
    uint8_t     dataSize;
    uint8_t     data[MAX_PROPERTY_SIZE];
} writeReqCmd_t;

typedef __PACKED_STRUCT writeRspCmd_tag {
    uint8_t     propertyId;
} writeRspCmd_t;

typedef __PACKED_STRUCT errorRspCmd_tag {
    uint8_t     errorCode;
} errorRspCmd_t;

/*! i2c command structure*/
typedef __PACKED_STRUCT i2cCommand_tag {
    uint8_t cmdId;
    __PACKED_UNION {
        readReqCmd_t    readReqCmd;
        readRspCmd_t    readRspCmd;
        writeReqCmd_t   writeReqCmd;
        writeRspCmd_t   writeRspCmd;
        errorRspCmd_t   errorRspCmd;
    } cmdData;
} i2cCommand_t;

/*! Flash interface command type */
typedef enum flashCmdId_tag {
    gFlashCfgFileName_c     = 0x01,
    gFlashCfgFileSize_c     = 0x02,
    gFlashCfgFileVisible_c  = 0x03,
    gFlashCfgWrite_c        = 0x04,
    gFlashCfgErase_c        = 0x05,
    gFlashStorageSize_c     = 0x06,
    gFlashSectorSize_c      = 0x07,
    gFlashRemountMSD_c      = 0x08,
    gFlashCfgEncWindow_c    = 0x09,
    gFlashDataRead_c        = 0x0A,
    gFlashDataWrite_c       = 0x0B,
    gFlashDataErase_c       = 0x0C,
    gFlashError_c           = 0x20
} flashCmdId_t;

typedef __PACKED_STRUCT flashDataWriteCmd_tag {
    uint8_t addr2;
    uint8_t addr1;
    uint8_t addr0;
    uint32_t length;
    uint8_t data[1024];
} flashDataWriteCmd_t;

typedef __PACKED_STRUCT flashDataEraseCmd_tag {
    uint8_t sAddr2;
    uint8_t sAddr1;
    uint8_t sAddr0;
    uint8_t pad;
    uint8_t eAddr2;
    uint8_t eAddr1;
    uint8_t eAddr0;
} flashDataEraseCmd_t;

/*! i2c command structure*/
typedef __PACKED_STRUCT i2cFlashCmd_tag {
    uint8_t cmdId;
    __PACKED_UNION {
        flashDataWriteCmd_t write;
        flashDataWriteCmd_t read;
        flashDataEraseCmd_t erase;
        uint8_t data[1024];
    } cmdData;
} i2cFlashCmd_t;

void i2c_cmds_init(void);

#ifdef __cplusplus
}
#endif

#endif
