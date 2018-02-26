/**
 * @file    DAP.c
 * @brief   Implementation of DAP.h
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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
 *
 * ----------------------------------------------------------------------
 *
 * $Date:        20. May 2015
 * $Revision:    V1.10
 *
 * Project:      CMSIS-DAP Source
 * Title:        DAP.c CMSIS-DAP Commands
 *
 *---------------------------------------------------------------------------*/

#include <string.h>
#ifdef RTE_CMSIS_RTOS
#include "cmsis_os.h"
#endif
#include "DAP_config.h"
#include "DAP.h"


#define DAP_FW_VER      "1.10"  // Firmware Version


#if (DAP_PACKET_SIZE < 64U)
#error "Minimum Packet Size is 64"
#endif
#if (DAP_PACKET_SIZE > 32768U)
#error "Maximum Packet Size is 32768"
#endif
#if (DAP_PACKET_COUNT < 1U)
#error "Minimum Packet Count is 1"
#endif
#if (DAP_PACKET_COUNT > 255U)
#error "Maximum Packet Count is 255"
#endif

 // Clock Macros

#define MAX_SWJ_CLOCK(delay_cycles) \
  (CPU_CLOCK/2 / (IO_PORT_WRITE_CYCLES + delay_cycles))

#define CLOCK_DELAY(swj_clock) \
  ((CPU_CLOCK/2 / swj_clock) - IO_PORT_WRITE_CYCLES)


         DAP_Data_t DAP_Data;           // DAP Data
volatile uint8_t    DAP_TransferAbort;  // Transfer Abort Flag


#ifdef DAP_VENDOR
const char DAP_Vendor [] = DAP_VENDOR;
#endif
#ifdef DAP_PRODUCT
const char DAP_Product[] = DAP_PRODUCT;
#endif
#ifdef DAP_SER_NUM
const char DAP_SerNum [] = DAP_SER_NUM;
#endif
const char DAP_FW_Ver [] = DAP_FW_VER;

#if TARGET_DEVICE_FIXED
const char TargetDeviceVendor [] = TARGET_DEVICE_VENDOR;
const char TargetDeviceName   [] = TARGET_DEVICE_NAME;
#endif


// Get DAP Information
//   id:      info identifier
//   info:    pointer to info data
//   return:  number of bytes in info data
static uint8_t DAP_Info(uint8_t id, uint8_t *info) {
  uint8_t length = 0U;

  switch (id) {
    case DAP_ID_VENDOR:
#ifdef DAP_VENDOR
      memcpy(info, DAP_Vendor, sizeof(DAP_Vendor));
      length = (uint8_t)sizeof(DAP_Vendor);
#endif
      break;
    case DAP_ID_PRODUCT:
#ifdef DAP_PRODUCT
      memcpy(info, DAP_Product, sizeof(DAP_Product));
      length = (uint8_t)sizeof(DAP_Product);
#endif
      break;
    case DAP_ID_SER_NUM:
#ifdef DAP_SER_NUM
      memcpy(info, DAP_SerNum, sizeof(DAP_SerNum));
      length = (uint8_t)sizeof(DAP_SerNum);
#endif
      break;
    case DAP_ID_FW_VER:
      memcpy(info, DAP_FW_Ver, sizeof(DAP_FW_Ver));
      length = (uint8_t)sizeof(DAP_FW_Ver);
      break;
    case DAP_ID_DEVICE_VENDOR:
#if TARGET_DEVICE_FIXED
      memcpy(info, TargetDeviceVendor, sizeof(TargetDeviceVendor));
      length = (uint8_t)sizeof(TargetDeviceVendor);
#endif
      break;
    case DAP_ID_DEVICE_NAME:
#if TARGET_DEVICE_FIXED
      memcpy(info, TargetDeviceName, sizeof(TargetDeviceName));
      length = (uint8_t)sizeof(TargetDeviceName);
#endif
      break;
    case DAP_ID_CAPABILITIES:
      info[0] = ((DAP_SWD  != 0)       ? (1U << 0) : 0U) |
                ((DAP_JTAG != 0)       ? (1U << 1) : 0U) |
                ((SWO_UART != 0)       ? (1U << 2) : 0U) |
                ((SWO_MANCHESTER != 0) ? (1U << 3) : 0U) |
                /* Atomic Commands  */   (1U << 4);
      length = 1U;
      break;
    case DAP_ID_SWO_BUFFER_SIZE:
#if ((SWO_UART != 0) || (SWO_MANCHESTER != 0))
      info[0] = (uint8_t)(SWO_BUFFER_SIZE >>  0);
      info[1] = (uint8_t)(SWO_BUFFER_SIZE >>  8);
      info[2] = (uint8_t)(SWO_BUFFER_SIZE >> 16);
      info[3] = (uint8_t)(SWO_BUFFER_SIZE >> 24);
      length = 4U;
#endif
      break;
    case DAP_ID_PACKET_SIZE:
      info[0] = (uint8_t)(DAP_PACKET_SIZE >> 0);
      info[1] = (uint8_t)(DAP_PACKET_SIZE >> 8);
      length = 2U;
      break;
    case DAP_ID_PACKET_COUNT:
      info[0] = DAP_PACKET_COUNT;
      length = 1U;
      break;
    default:
      break;
  }

  return (length);
}


// Timer Functions

#if ((DAP_SWD != 0) || (DAP_JTAG != 0))

#ifdef RTE_CMSIS_RTOS

static uint32_t TimerTick;

// Start Timer
static __inline void TIMER_START (uint32_t usec) {
  TimerTick = osKernelSysTick() + osKernelSysTickMicroSec(usec);
}

// Stop Timer
static __inline void TIMER_STOP (void) {}

// Check if Timer expired
static __inline uint32_t TIMER_EXPIRED (void) {
  return ((osKernelSysTick() > TimerTick) ? 1U : 0U);
}

#else

// Start Timer
static __inline void TIMER_START (uint32_t usec) {
  SysTick->VAL  = 0U;
  SysTick->LOAD = usec * (CPU_CLOCK/1000000U);
  SysTick->CTRL = (1U << SysTick_CTRL_ENABLE_Pos) |
                  (1U << SysTick_CTRL_CLKSOURCE_Pos);
}

// Stop Timer
static __inline void TIMER_STOP (void) {
  SysTick->CTRL = 0U;
}

// Check if Timer expired
static __inline uint32_t TIMER_EXPIRED (void) {
  return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) ? 1U : 0U);
}

#endif

#endif


// Delay for specified time
//    delay:  delay time in ms
void Delayms(uint32_t delay) {
  delay *= ((CPU_CLOCK/1000U) + (DELAY_SLOW_CYCLES-1U)) / DELAY_SLOW_CYCLES;
  PIN_DELAY_SLOW(delay);
}


// Process Delay command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_Delay(const uint8_t *request, uint8_t *response) {
  uint32_t delay;

  delay  = *(request+0) | (*(request+1) << 8);
  delay *= ((CPU_CLOCK/1000000U) + (DELAY_SLOW_CYCLES-1U)) / DELAY_SLOW_CYCLES;

  PIN_DELAY_SLOW(delay);

  *response = DAP_OK;
  return ((2U << 16) | 1U);
}


// Process Host Status command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_HostStatus(const uint8_t *request, uint8_t *response) {

  switch (*request) {
    case DAP_DEBUGGER_CONNECTED:
      LED_CONNECTED_OUT((*(request+1) & 1U));
      break;
    case DAP_TARGET_RUNNING:
      LED_RUNNING_OUT((*(request+1) & 1U));
      break;
    default:
      *response = DAP_ERROR;
      return ((2U << 16) | 1U);
  }

  *response = DAP_OK;
  return ((2U << 16) | 1U);
}


// Process Connect command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_Connect(const uint8_t *request, uint8_t *response) {
  uint32_t port;

  if (*request == DAP_PORT_AUTODETECT) {
    port = DAP_DEFAULT_PORT;
  } else {
    port = *request;
  }
  
  switch (port) {
#if (DAP_SWD != 0)
    case DAP_PORT_SWD:
      DAP_Data.debug_port = DAP_PORT_SWD;
      PORT_SWD_SETUP();
      break;
#endif
#if (DAP_JTAG != 0)
    case DAP_PORT_JTAG:
      DAP_Data.debug_port = DAP_PORT_JTAG;
      PORT_JTAG_SETUP();
      break;
#endif
    default:
      port = DAP_PORT_DISABLED;
      break;
  }

  *response = (uint8_t)port;
  return ((1U << 16) | 1U);
}


// Process Disconnect command and prepare response
//   response: pointer to response data
//   return:   number of bytes in response
static uint32_t DAP_Disconnect(uint8_t *response) {

  DAP_Data.debug_port = DAP_PORT_DISABLED;
  PORT_OFF();

  *response = DAP_OK;
  return (1U);
}


// Process Reset Target command and prepare response
//   response: pointer to response data
//   return:   number of bytes in response
static uint32_t DAP_ResetTarget(uint8_t *response) {

  *(response+1) = RESET_TARGET();
  *(response+0) = DAP_OK;
  return (2U);
}


// Process SWJ Pins command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_SWJ_Pins(const uint8_t *request, uint8_t *response) {
#if ((DAP_SWD != 0) || (DAP_JTAG != 0))
  uint32_t value;
  uint32_t select;
  uint32_t wait;
  
  value  =  *(request+0);
  select =  *(request+1); 
  wait   = (*(request+2) <<  0) |
           (*(request+3) <<  8) |
           (*(request+4) << 16) |
           (*(request+5) << 24);

  if (select & (1U << DAP_SWJ_SWCLK_TCK)) {
    if (value & (1U << DAP_SWJ_SWCLK_TCK)) {
      PIN_SWCLK_TCK_SET();
    } else {
      PIN_SWCLK_TCK_CLR();
    }
  }
  if (select & (1U << DAP_SWJ_SWDIO_TMS)) {
    if (value & (1U << DAP_SWJ_SWDIO_TMS)) {
      PIN_SWDIO_TMS_SET();
    } else {
      PIN_SWDIO_TMS_CLR();
    }
  }
  if (select & (1U << DAP_SWJ_TDI)) {
    PIN_TDI_OUT(value >> DAP_SWJ_TDI);
  }
  if (select & (1U << DAP_SWJ_nTRST)) {
    PIN_nTRST_OUT(value >> DAP_SWJ_nTRST);
  }
  if (select & (1U << DAP_SWJ_nRESET)) {
    PIN_nRESET_OUT(value >> DAP_SWJ_nRESET);
  }

  if (wait) {
    if (wait > 3000000U) { wait = 3000000U; }
    TIMER_START(wait);
    do {
      if (select & (1U << DAP_SWJ_SWCLK_TCK)) {
        if ((value >> DAP_SWJ_SWCLK_TCK) ^ PIN_SWCLK_TCK_IN()) { continue; }
      }
      if (select & (1U << DAP_SWJ_SWDIO_TMS)) {
        if ((value >> DAP_SWJ_SWDIO_TMS) ^ PIN_SWDIO_TMS_IN()) { continue; }
      }
      if (select & (1U << DAP_SWJ_TDI)) {
        if ((value >> DAP_SWJ_TDI) ^ PIN_TDI_IN()) { continue; }
      }
      if (select & (1U << DAP_SWJ_nTRST)) {
        if ((value >> DAP_SWJ_nTRST) ^ PIN_nTRST_IN()) { continue; }
      }
      if (select & (1U << DAP_SWJ_nRESET)) {
        if ((value >> DAP_SWJ_nRESET) ^ PIN_nRESET_IN()) { continue; }
      }
      break;
    } while (!TIMER_EXPIRED());
    TIMER_STOP();
  }

  value = (PIN_SWCLK_TCK_IN() << DAP_SWJ_SWCLK_TCK) |
          (PIN_SWDIO_TMS_IN() << DAP_SWJ_SWDIO_TMS) |
          (PIN_TDI_IN()       << DAP_SWJ_TDI)       |
          (PIN_TDO_IN()       << DAP_SWJ_TDO)       |
          (PIN_nTRST_IN()     << DAP_SWJ_nTRST)     |
          (PIN_nRESET_IN()    << DAP_SWJ_nRESET);

  *response = (uint8_t)value;
#else
  *response = 0U;
#endif

  return ((6U << 16) | 1U);
}


// Process SWJ Clock command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_SWJ_Clock(const uint8_t *request, uint8_t *response) {
#if ((DAP_SWD != 0) || (DAP_JTAG != 0))
  uint32_t clock;
  uint32_t delay;

  clock = (*(request+0) <<  0) |
          (*(request+1) <<  8) |
          (*(request+2) << 16) |
          (*(request+3) << 24);

  if (clock == 0U) {
    *response = DAP_ERROR;
    return ((4U << 16) | 1U);
  }

  if (clock >= MAX_SWJ_CLOCK(DELAY_FAST_CYCLES)) {
    DAP_Data.fast_clock  = 1U;
    DAP_Data.clock_delay = 1U;
  } else {
    DAP_Data.fast_clock  = 0U;

    delay = ((CPU_CLOCK/2U) + (clock - 1U)) / clock;
    if (delay > IO_PORT_WRITE_CYCLES) {
      delay -= IO_PORT_WRITE_CYCLES;
      delay  = (delay + (DELAY_SLOW_CYCLES - 1U)) / DELAY_SLOW_CYCLES;
    } else {
      delay  = 1U;
    }

    DAP_Data.clock_delay = delay;
  }

  *response = DAP_OK;
#else
  *response = DAP_ERROR;
#endif

  return ((4U << 16) | 1U);
}


// Process SWJ Sequence command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_SWJ_Sequence(const uint8_t *request, uint8_t *response) {
  uint32_t count;

  count = *request++;
  if (count == 0U) { count = 256U; }

#if ((DAP_SWD != 0) || (DAP_JTAG != 0))
  SWJ_Sequence(count, request);
  *response = DAP_OK;
#else
  *response = DAP_ERROR;
#endif

  count = (count + 7U) >> 3;

  return (((uint16_t)(count + 1U) << 16) | 1U);
}


// Process SWD Configure command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_SWD_Configure(const uint8_t *request, uint8_t *response) {
#if (DAP_SWD != 0)
  uint8_t value;

  value = *request;
  DAP_Data.swd_conf.turnaround = (value & 0x03U) + 1U;
  DAP_Data.swd_conf.data_phase = (value & 0x04U) ? 1U : 0U;
  
  *response = DAP_OK;
#else
  *response = DAP_ERROR;
#endif

  return ((1U << 16) | 1U);
}


// Process JTAG Sequence command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_JTAG_Sequence(const uint8_t *request, uint8_t *response) {
  uint32_t sequence_info;
  uint32_t sequence_count;
  uint32_t request_count;
  uint32_t response_count;
  uint32_t count;

#if (DAP_JTAG != 0)
  *response++ = DAP_OK;
#else
  *response++ = DAP_ERROR;
#endif
  request_count  = 1U;
  response_count = 1U;

  sequence_count = *request++;
  while (sequence_count--) {
    sequence_info = *request++;
#if (DAP_JTAG != 0)
    JTAG_Sequence(sequence_info, request, response);
#endif
    count = sequence_info & JTAG_SEQUENCE_TCK;
    if (count == 0U) { count = 64U; }
    count = (count + 7U) / 8U;
    request += count;
    request_count += count + 1U;
#if (DAP_JTAG != 0)
    if (sequence_info & JTAG_SEQUENCE_TDO) {
      response += count;
      response_count += count;
    }
#endif
  }

  return (((uint16_t)request_count << 16) | (uint16_t)response_count);
}


// Process JTAG Configure command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_JTAG_Configure(const uint8_t *request, uint8_t *response) {
  uint32_t count;
#if (DAP_JTAG != 0)
  uint32_t length;
  uint32_t bits;
  uint32_t n;

  count = *request++;
  DAP_Data.jtag_dev.count = (uint8_t)count;

  bits = 0U;
  for (n = 0U; n < count; n++) {
    length = *request++;
    DAP_Data.jtag_dev.ir_length[n] =  (uint8_t)length;
    DAP_Data.jtag_dev.ir_before[n] = (uint16_t)bits;
    bits += length;
  }
  for (n = 0U; n < count; n++) {
    bits -= DAP_Data.jtag_dev.ir_length[n];
    DAP_Data.jtag_dev.ir_after[n] = (uint16_t)bits;
  }

  *response = DAP_OK;
#else
  count = *request;
  *response = DAP_ERROR;
#endif

  return (((uint16_t)(count + 1U) << 16) | 1U);
}


// Process JTAG IDCODE command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_JTAG_IDCode(const uint8_t *request, uint8_t *response) {
#if (DAP_JTAG != 0)
  uint32_t data;

  if (DAP_Data.debug_port != DAP_PORT_JTAG) {
    goto id_error;
  }

  // Device index (JTAP TAP)
  DAP_Data.jtag_dev.index = *request;
  if (DAP_Data.jtag_dev.index >= DAP_Data.jtag_dev.count) {
    goto id_error;
  }

  // Select JTAG chain
  JTAG_IR(JTAG_IDCODE);

  // Read IDCODE register
  data = JTAG_ReadIDCode();

  // Store Data
  *(response+0) =  DAP_OK;
  *(response+1) = (uint8_t)(data >>  0);
  *(response+2) = (uint8_t)(data >>  8);
  *(response+3) = (uint8_t)(data >> 16);
  *(response+4) = (uint8_t)(data >> 24);

  return ((1U << 16) | 5U);

id_error:
#endif
  *response = DAP_ERROR;
  return ((1U << 16) | 1U); 
}


// Process Transfer Configure command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_TransferConfigure(const uint8_t *request, uint8_t *response) {

  DAP_Data.transfer.idle_cycles = *(request+0);
  DAP_Data.transfer.retry_count = *(request+1) | (*(request+2) << 8);
  DAP_Data.transfer.match_retry = *(request+3) | (*(request+4) << 8);

  *response = DAP_OK;
  return ((5U << 16) | 1U); 
}


// Process SWD Transfer command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
#if (DAP_SWD != 0)
static uint32_t DAP_SWD_Transfer(const uint8_t *request, uint8_t *response) {
  const
  uint8_t  *request_head;
  uint32_t  request_count;
  uint32_t  request_value;
  uint8_t  *response_head;
  uint32_t  response_count;
  uint32_t  response_value;
  uint32_t  post_read;
  uint32_t  check_write;
  uint32_t  match_value;
  uint32_t  match_retry;
  uint32_t  retry;
  uint32_t  data;

  request_head   = request;

  response_count = 0U;
  response_value = 0U;
  response_head  = response;
  response      += 2;

  DAP_TransferAbort = 0U;

  post_read   = 0U;
  check_write = 0U;

  request++;            // Ignore DAP index

  request_count = *request++;

  for (; request_count; request_count--) {
    request_value = *request++;
    if (request_value & DAP_TRANSFER_RnW) {
      // Read register
      if (post_read) {
        // Read was posted before
        retry = DAP_Data.transfer.retry_count;
        if ((request_value & (DAP_TRANSFER_APnDP | DAP_TRANSFER_MATCH_VALUE)) == DAP_TRANSFER_APnDP) {
          // Read previous AP data and post next AP read
          do {
            response_value = SWD_Transfer(request_value, &data);
          } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
        } else {
          // Read previous AP data
          do {
            response_value = SWD_Transfer(DP_RDBUFF | DAP_TRANSFER_RnW, &data);
          } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
          post_read = 0U;
        }
        if (response_value != DAP_TRANSFER_OK) { break; }
        // Store previous AP data
        *response++ = (uint8_t) data;
        *response++ = (uint8_t)(data >>  8);
        *response++ = (uint8_t)(data >> 16);
        *response++ = (uint8_t)(data >> 24);
      }
      if (request_value & DAP_TRANSFER_MATCH_VALUE) {
        // Read with value match
        match_value = (*(request+0) <<  0) |
                      (*(request+1) <<  8) |
                      (*(request+2) << 16) |
                      (*(request+3) << 24);
        request += 4;
        match_retry = DAP_Data.transfer.match_retry;
        if (request_value & DAP_TRANSFER_APnDP) {
          // Post AP read
          retry = DAP_Data.transfer.retry_count;
          do {
            response_value = SWD_Transfer(request_value, NULL);
          } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
          if (response_value != DAP_TRANSFER_OK) { break; }
        }
        do {
          // Read register until its value matches or retry counter expires
          retry = DAP_Data.transfer.retry_count;
          do {
            response_value = SWD_Transfer(request_value, &data);
          } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
          if (response_value != DAP_TRANSFER_OK) { break; }
        } while (((data & DAP_Data.transfer.match_mask) != match_value) && match_retry-- && !DAP_TransferAbort);
        if ((data & DAP_Data.transfer.match_mask) != match_value) {
          response_value |= DAP_TRANSFER_MISMATCH;
        }
        if (response_value != DAP_TRANSFER_OK) { break; }
      } else {
        // Normal read
        retry = DAP_Data.transfer.retry_count;
        if (request_value & DAP_TRANSFER_APnDP) {
          // Read AP register
          if (post_read == 0U) {
            // Post AP read
            do {
              response_value = SWD_Transfer(request_value, NULL);
            } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
            if (response_value != DAP_TRANSFER_OK) { break; }
            post_read = 1U;
          }
        } else {
          // Read DP register
          do {
            response_value = SWD_Transfer(request_value, &data);
          } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
          if (response_value != DAP_TRANSFER_OK) { break; }
          // Store data
          *response++ = (uint8_t) data;
          *response++ = (uint8_t)(data >>  8);
          *response++ = (uint8_t)(data >> 16);
          *response++ = (uint8_t)(data >> 24);
        }
      }
      check_write = 0U;
    } else {
      // Write register
      if (post_read) {
        // Read previous data
        retry = DAP_Data.transfer.retry_count;
        do {
          response_value = SWD_Transfer(DP_RDBUFF | DAP_TRANSFER_RnW, &data);
        } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
        if (response_value != DAP_TRANSFER_OK) { break; }
        // Store previous data
        *response++ = (uint8_t) data;
        *response++ = (uint8_t)(data >>  8);
        *response++ = (uint8_t)(data >> 16);
        *response++ = (uint8_t)(data >> 24);
        post_read = 0U;
      }
      // Load data
      data = (*(request+0) <<  0) |
             (*(request+1) <<  8) |
             (*(request+2) << 16) |
             (*(request+3) << 24);
      request += 4;
      if (request_value & DAP_TRANSFER_MATCH_MASK) {
        // Write match mask
        DAP_Data.transfer.match_mask = data;
        response_value = DAP_TRANSFER_OK;
      } else {
        // Write DP/AP register
        retry = DAP_Data.transfer.retry_count;
        do {
          response_value = SWD_Transfer(request_value, &data);
        } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
        if (response_value != DAP_TRANSFER_OK) { break; }
        check_write = 1U;
      }
    }
    response_count++;
    if (DAP_TransferAbort) { break; }
  }

  for (; request_count; request_count--) {
    // Process canceled requests
    request_value = *request++;
    if (request_value & DAP_TRANSFER_RnW) {
      // Read register
      if (request_value & DAP_TRANSFER_MATCH_VALUE) {
        // Read with value match
        request += 4;
      }
    } else {
      // Write register
      request += 4;
    }
  }

  if (response_value == DAP_TRANSFER_OK) {
    if (post_read) {
      // Read previous data
      retry = DAP_Data.transfer.retry_count;
      do {
        response_value = SWD_Transfer(DP_RDBUFF | DAP_TRANSFER_RnW, &data);
      } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
      if (response_value != DAP_TRANSFER_OK) { goto end; }
      // Store previous data
      *response++ = (uint8_t) data;
      *response++ = (uint8_t)(data >>  8);
      *response++ = (uint8_t)(data >> 16);
      *response++ = (uint8_t)(data >> 24);
    } else if (check_write) {
      // Check last write
      retry = DAP_Data.transfer.retry_count;
      do {
        response_value = SWD_Transfer(DP_RDBUFF | DAP_TRANSFER_RnW, NULL);
      } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
    }
  }

end:
  *(response_head+0) = (uint8_t)response_count;
  *(response_head+1) = (uint8_t)response_value;

  return (((request - request_head) << 16) | (response - response_head));
}
#endif


// Process JTAG Transfer command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
#if (DAP_JTAG != 0)
static uint32_t DAP_JTAG_Transfer(const uint8_t *request, uint8_t *response) {
  const
  uint8_t  *request_head;
  uint32_t  request_count;
  uint32_t  request_value;
  uint32_t  request_ir;
  uint8_t  *response_head;
  uint32_t  response_count;
  uint32_t  response_value;
  uint32_t  post_read;
  uint32_t  match_value;
  uint32_t  match_retry;
  uint32_t  retry;
  uint32_t  data;
  uint32_t  ir;

  request_head   = request;

  response_count = 0U;
  response_value = 0U;
  response_head  = response;
  response      += 2;

  DAP_TransferAbort = 0U;

  ir        = 0U;
  post_read = 0U;

  // Device index (JTAP TAP)
  DAP_Data.jtag_dev.index = *request++;
  if (DAP_Data.jtag_dev.index >= DAP_Data.jtag_dev.count) { goto end; }

  request_count = *request++;

  for (; request_count; request_count--) {
    request_value = *request++;
    request_ir = (request_value & DAP_TRANSFER_APnDP) ? JTAG_APACC : JTAG_DPACC;
    if (request_value & DAP_TRANSFER_RnW) {
      // Read register
      if (post_read) {
        // Read was posted before
        retry = DAP_Data.transfer.retry_count;
        if ((ir == request_ir) && ((request_value & DAP_TRANSFER_MATCH_VALUE) == 0U)) {
          // Read previous data and post next read
          do {
            response_value = JTAG_Transfer(request_value, &data);
          } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
        } else {
          // Select JTAG chain
          if (ir != JTAG_DPACC) {
            ir = JTAG_DPACC;
            JTAG_IR(ir);
          }
          // Read previous data
          do {
            response_value = JTAG_Transfer(DP_RDBUFF | DAP_TRANSFER_RnW, &data);
          } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
          post_read = 0U;
        }
        if (response_value != DAP_TRANSFER_OK) { break; }
        // Store previous data
        *response++ = (uint8_t) data;
        *response++ = (uint8_t)(data >>  8);
        *response++ = (uint8_t)(data >> 16);
        *response++ = (uint8_t)(data >> 24);
      }
      if (request_value & DAP_TRANSFER_MATCH_VALUE) {
        // Read with value match
        match_value = (*(request+0) <<  0) |
                      (*(request+1) <<  8) |
                      (*(request+2) << 16) |
                      (*(request+3) << 24);
        request += 4;
        match_retry  = DAP_Data.transfer.match_retry;
        // Select JTAG chain
        if (ir != request_ir) {
          ir = request_ir;
          JTAG_IR(ir);
        }
        // Post DP/AP read
        retry = DAP_Data.transfer.retry_count;
        do {
          response_value = JTAG_Transfer(request_value, NULL);
        } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
        if (response_value != DAP_TRANSFER_OK) { break; }
        do {
          // Read register until its value matches or retry counter expires
          retry = DAP_Data.transfer.retry_count;
          do {
            response_value = JTAG_Transfer(request_value, &data);
          } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
          if (response_value != DAP_TRANSFER_OK) { break; }
        } while (((data & DAP_Data.transfer.match_mask) != match_value) && match_retry-- && !DAP_TransferAbort);
        if ((data & DAP_Data.transfer.match_mask) != match_value) {
          response_value |= DAP_TRANSFER_MISMATCH;
        }
        if (response_value != DAP_TRANSFER_OK) { break; }
      } else {
        // Normal read
        if (post_read == 0U) {
          // Select JTAG chain
          if (ir != request_ir) {
            ir = request_ir;
            JTAG_IR(ir);
          }
          // Post DP/AP read
          retry = DAP_Data.transfer.retry_count;
          do {
            response_value = JTAG_Transfer(request_value, NULL);
          } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
          if (response_value != DAP_TRANSFER_OK) { break; }
          post_read = 1U;
        }
      }
    } else {
      // Write register
      if (post_read) {
        // Select JTAG chain
        if (ir != JTAG_DPACC) {
          ir = JTAG_DPACC;
          JTAG_IR(ir);
        }
        // Read previous data
        retry = DAP_Data.transfer.retry_count;
        do {
          response_value = JTAG_Transfer(DP_RDBUFF | DAP_TRANSFER_RnW, &data);
        } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
        if (response_value != DAP_TRANSFER_OK) { break; }
        // Store previous data
        *response++ = (uint8_t) data;
        *response++ = (uint8_t)(data >>  8);
        *response++ = (uint8_t)(data >> 16);
        *response++ = (uint8_t)(data >> 24);
        post_read = 0U;
      }
      // Load data
      data = (*(request+0) <<  0) |
             (*(request+1) <<  8) |
             (*(request+2) << 16) |
             (*(request+3) << 24);
      request += 4;
      if (request_value & DAP_TRANSFER_MATCH_MASK) {
        // Write match mask
        DAP_Data.transfer.match_mask = data;
        response_value = DAP_TRANSFER_OK;
      } else {
        // Select JTAG chain
        if (ir != request_ir) {
          ir = request_ir;
          JTAG_IR(ir);
        }
        // Write DP/AP register
        retry = DAP_Data.transfer.retry_count;
        do {
          response_value = JTAG_Transfer(request_value, &data);
        } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
        if (response_value != DAP_TRANSFER_OK) { break; }
      }
    }
    response_count++;
    if (DAP_TransferAbort) { break; }
  }

  for (; request_count; request_count--) {
    // Process canceled requests
    request_value = *request++;
    if (request_value & DAP_TRANSFER_RnW) {
      // Read register
      if (request_value & DAP_TRANSFER_MATCH_VALUE) {
        // Read with value match
        request += 4;
      }
    } else {
      // Write register
      request += 4;
    }
  }

  if (response_value == DAP_TRANSFER_OK) {
    // Select JTAG chain
    if (ir != JTAG_DPACC) {
      ir = JTAG_DPACC;
      JTAG_IR(ir);
    }
    if (post_read) {
      // Read previous data
      retry = DAP_Data.transfer.retry_count;
      do {
        response_value = JTAG_Transfer(DP_RDBUFF | DAP_TRANSFER_RnW, &data);
      } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
      if (response_value != DAP_TRANSFER_OK) { goto end; }
      // Store previous data
      *response++ = (uint8_t) data;
      *response++ = (uint8_t)(data >>  8);
      *response++ = (uint8_t)(data >> 16);
      *response++ = (uint8_t)(data >> 24);
    } else {
      // Check last write
      retry = DAP_Data.transfer.retry_count;
      do {
        response_value = JTAG_Transfer(DP_RDBUFF | DAP_TRANSFER_RnW, NULL);
      } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
    }
  }

end:
  *(response_head+0) = (uint8_t)response_count;
  *(response_head+1) = (uint8_t)response_value;

  return (((request - request_head) << 16) | (response - response_head));
}
#endif


// Process Dummy Transfer command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_Dummy_Transfer(const uint8_t *request, uint8_t *response) {
  const
  uint8_t  *request_head;
  uint32_t  request_count;
  uint32_t  request_value;

  request_head  =  request;

  request++;            // Ignore DAP index

  request_count = *request++;

  for (; request_count; request_count--) {
    // Process dummy requests
    request_value = *request++;
    if (request_value & DAP_TRANSFER_RnW) {
      // Read register
      if (request_value & DAP_TRANSFER_MATCH_VALUE) {
        // Read with value match
        request += 4;
      }
    } else {
      // Write register
      request += 4;
    }
  }

  *(response+0) = 0U;   // Response count
  *(response+1) = 0U;   // Response value

  return (((request - request_head) << 16) | 2U);
}


// Process Transfer command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_Transfer(const uint8_t *request, uint8_t *response) {
  uint32_t num;

  switch (DAP_Data.debug_port) {
#if (DAP_SWD != 0)
    case DAP_PORT_SWD:
      num = DAP_SWD_Transfer(request, response);
      break;
#endif
#if (DAP_JTAG != 0)
    case DAP_PORT_JTAG:
      num = DAP_JTAG_Transfer(request, response);
      break;
#endif
    default:
      num = DAP_Dummy_Transfer(request, response);
      break;
  }

  return (num);
}


// Process SWD Transfer Block command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if (DAP_SWD != 0)
static uint32_t DAP_SWD_TransferBlock(const uint8_t *request, uint8_t *response) {
  uint32_t  request_count;
  uint32_t  request_value;
  uint32_t  response_count;
  uint32_t  response_value;
  uint8_t  *response_head;
  uint32_t  retry;
  uint32_t  data;

  response_count = 0U;
  response_value = 0U;
  response_head  = response;
  response      += 3;

  DAP_TransferAbort = 0U;

  request++;            // Ignore DAP index

  request_count = *request | (*(request+1) << 8);
  request += 2;
  if (request_count == 0U) { goto end; }

  request_value = *request++;
  if (request_value & DAP_TRANSFER_RnW) {
    // Read register block
    if (request_value & DAP_TRANSFER_APnDP) {
      // Post AP read
      retry = DAP_Data.transfer.retry_count;
      do {
        response_value = SWD_Transfer(request_value, NULL);
      } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
      if (response_value != DAP_TRANSFER_OK) { goto end; }
    }
    while (request_count--) {
      // Read DP/AP register
      if ((request_count == 0U) && (request_value & DAP_TRANSFER_APnDP)) {
        // Last AP read
        request_value = DP_RDBUFF | DAP_TRANSFER_RnW;
      }
      retry = DAP_Data.transfer.retry_count;
      do {
        response_value = SWD_Transfer(request_value, &data);
      } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
      if (response_value != DAP_TRANSFER_OK) { goto end; }
      // Store data
      *response++ = (uint8_t) data;
      *response++ = (uint8_t)(data >>  8);
      *response++ = (uint8_t)(data >> 16);
      *response++ = (uint8_t)(data >> 24);
      response_count++;
    }
  } else {
    // Write register block
    while (request_count--) {
      // Load data
      data = (*(request+0) <<  0) |
             (*(request+1) <<  8) |
             (*(request+2) << 16) |
             (*(request+3) << 24);
      request += 4;
      // Write DP/AP register
      retry = DAP_Data.transfer.retry_count;
      do {
        response_value = SWD_Transfer(request_value, &data);
      } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
      if (response_value != DAP_TRANSFER_OK) { goto end; }
      response_count++;
    }
    // Check last write
    retry = DAP_Data.transfer.retry_count;
    do {
      response_value = SWD_Transfer(DP_RDBUFF | DAP_TRANSFER_RnW, NULL);
    } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
  }

end:
  *(response_head+0) = (uint8_t)(response_count >> 0);
  *(response_head+1) = (uint8_t)(response_count >> 8);
  *(response_head+2) = (uint8_t) response_value;

  return (response - response_head);
}
#endif


// Process JTAG Transfer Block command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if (DAP_JTAG != 0)
static uint32_t DAP_JTAG_TransferBlock(const uint8_t *request, uint8_t *response) {
  uint32_t  request_count;
  uint32_t  request_value;
  uint32_t  response_count;
  uint32_t  response_value;
  uint8_t  *response_head;
  uint32_t  retry;
  uint32_t  data;
  uint32_t  ir;

  response_count = 0U;
  response_value = 0U;
  response_head  = response;
  response      += 3;

  DAP_TransferAbort = 0U;

  // Device index (JTAP TAP)
  DAP_Data.jtag_dev.index = *request++;
  if (DAP_Data.jtag_dev.index >= DAP_Data.jtag_dev.count) { goto end; }

  request_count = *request | (*(request+1) << 8);
  request += 2;
  if (request_count == 0U) { goto end; }

  request_value = *request++;

  // Select JTAG chain
  ir = (request_value & DAP_TRANSFER_APnDP) ? JTAG_APACC : JTAG_DPACC;
  JTAG_IR(ir);

  if (request_value & DAP_TRANSFER_RnW) {
    // Post read
    retry = DAP_Data.transfer.retry_count;
    do {
      response_value = JTAG_Transfer(request_value, NULL);
    } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
    if (response_value != DAP_TRANSFER_OK) { goto end; }
    // Read register block
    while (request_count--) {
      // Read DP/AP register
      if (request_count == 0U) {
        // Last read
        if (ir != JTAG_DPACC) {
          JTAG_IR(JTAG_DPACC);
        }
        request_value = DP_RDBUFF | DAP_TRANSFER_RnW;
      }
      retry = DAP_Data.transfer.retry_count;
      do {
        response_value = JTAG_Transfer(request_value, &data);
      } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
      if (response_value != DAP_TRANSFER_OK) { goto end; }
      // Store data
      *response++ = (uint8_t) data;
      *response++ = (uint8_t)(data >>  8);
      *response++ = (uint8_t)(data >> 16);
      *response++ = (uint8_t)(data >> 24);
      response_count++;
    }
  } else {
    // Write register block
    while (request_count--) {
      // Load data
      data = (*(request+0) <<  0) |
             (*(request+1) <<  8) |
             (*(request+2) << 16) |
             (*(request+3) << 24);
      request += 4;
      // Write DP/AP register
      retry = DAP_Data.transfer.retry_count;
      do {
        response_value = JTAG_Transfer(request_value, &data);
      } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
      if (response_value != DAP_TRANSFER_OK) { goto end; }
      response_count++;
    }
    // Check last write
    if (ir != JTAG_DPACC) {
      JTAG_IR(JTAG_DPACC);
    }
    retry = DAP_Data.transfer.retry_count;
    do {
      response_value = JTAG_Transfer(DP_RDBUFF | DAP_TRANSFER_RnW, NULL);
    } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
  }

end:
  *(response_head+0) = (uint8_t)(response_count >> 0);
  *(response_head+1) = (uint8_t)(response_count >> 8);
  *(response_head+2) = (uint8_t) response_value;

  return (response - response_head);
}
#endif


// Process Transfer Block command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_TransferBlock(const uint8_t *request, uint8_t *response) {
  uint32_t num;

  switch (DAP_Data.debug_port) {
#if (DAP_SWD != 0)
    case DAP_PORT_SWD:
      num = DAP_SWD_TransferBlock (request, response);
      break;
#endif
#if (DAP_JTAG != 0)
    case DAP_PORT_JTAG:
      num = DAP_JTAG_TransferBlock(request, response);
      break;
#endif
    default:
      *(response+0) = 0U;       // Response count [7:0]
      *(response+1) = 0U;       // Response count[15:8]
      *(response+2) = 0U;       // Response value
      num = 3U;
  }

  if (*(request+3) & DAP_TRANSFER_RnW) {
    // Read register block
    num |=  4U << 16;
  } else {
    // Write register block
    num |= (4U + ((*(request+1) | (*(request+2) << 8)) * 4)) << 16;
  }

  return (num);
}


// Process SWD Write ABORT command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if (DAP_SWD != 0)
static uint32_t DAP_SWD_WriteAbort(const uint8_t *request, uint8_t *response) {
  uint32_t data;

  // Load data (Ignore DAP index)
  data = (*(request+1) <<  0) |
         (*(request+2) <<  8) |
         (*(request+3) << 16) |
         (*(request+4) << 24);

  // Write Abort register
  SWD_Transfer(DP_ABORT, &data);

  *response = DAP_OK;
  return (1U);
}
#endif


// Process JTAG Write ABORT command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if (DAP_JTAG != 0)
static uint32_t DAP_JTAG_WriteAbort(const uint8_t *request, uint8_t *response) {
  uint32_t data;

  // Device index (JTAP TAP)
  DAP_Data.jtag_dev.index = *request;
  if (DAP_Data.jtag_dev.index >= DAP_Data.jtag_dev.count) {
    *response = DAP_ERROR;
    return (1U); 
  }

  // Select JTAG chain
  JTAG_IR(JTAG_ABORT);

  // Load data
  data = (*(request+1) <<  0) |
         (*(request+2) <<  8) |
         (*(request+3) << 16) |
         (*(request+4) << 24);

  // Write Abort register
  JTAG_WriteAbort(data);

  *response = DAP_OK;
  return (1U); 
}
#endif


// Process Write ABORT command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
static uint32_t DAP_WriteAbort(const uint8_t *request, uint8_t *response) {
  uint32_t num;

  switch (DAP_Data.debug_port) {
#if (DAP_SWD != 0)
    case DAP_PORT_SWD:
      num = DAP_SWD_WriteAbort (request, response);
      break;
#endif
#if (DAP_JTAG != 0)
    case DAP_PORT_JTAG:
      num = DAP_JTAG_WriteAbort(request, response);
      break;
#endif
    default:
      *response = DAP_ERROR;
      num = 1U;
  }
  return ((5U << 16) | num);
}


// Process DAP Vendor command request and prepare response
// Default function (can be overridden)
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
__weak uint32_t DAP_ProcessVendorCommand(const uint8_t *request, uint8_t *response) {
  *response = ID_DAP_Invalid;
  return ((1U << 16) | 1U);
}


// Process DAP command request and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
uint32_t DAP_ProcessCommand(const uint8_t *request, uint8_t *response) {
  uint32_t num;

  if ((*request >= ID_DAP_Vendor0) && (*request <= ID_DAP_Vendor31)) {
    return DAP_ProcessVendorCommand(request, response);
  }

  *response++ = *request;

  switch (*request++) {
    case ID_DAP_Info:
      num = DAP_Info(*request, response+1);
      *response = (uint8_t)num;
      return ((2U << 16) + 2U + num);

    case ID_DAP_HostStatus:
      num = DAP_HostStatus(request, response);
      break;

    case ID_DAP_Connect:
      num = DAP_Connect(request, response);
      break;
    case ID_DAP_Disconnect:
      num = DAP_Disconnect(response);
      break;

    case ID_DAP_Delay:
      num = DAP_Delay(request, response);
      break;

    case ID_DAP_ResetTarget:
      num = DAP_ResetTarget(response);
      break;

    case ID_DAP_SWJ_Pins:
      num = DAP_SWJ_Pins(request, response);
      break;
    case ID_DAP_SWJ_Clock:
      num = DAP_SWJ_Clock(request, response);
      break;
    case ID_DAP_SWJ_Sequence:
      num = DAP_SWJ_Sequence(request, response);
      break;

    case ID_DAP_SWD_Configure:
      num = DAP_SWD_Configure(request, response);
      break;

    case ID_DAP_JTAG_Sequence:
      num = DAP_JTAG_Sequence(request, response);
      break;
    case ID_DAP_JTAG_Configure:
      num = DAP_JTAG_Configure(request, response);
      break;
    case ID_DAP_JTAG_IDCODE:
      num = DAP_JTAG_IDCode(request, response);
      break;

    case ID_DAP_TransferConfigure:
      num = DAP_TransferConfigure(request, response);
      break;
    case ID_DAP_Transfer:
      num = DAP_Transfer(request, response);
      break;
    case ID_DAP_TransferBlock:
      num = DAP_TransferBlock(request, response);
      break;

    case ID_DAP_WriteABORT:
      num = DAP_WriteAbort(request, response);
      break;

#if ((SWO_UART != 0) || (SWO_MANCHESTER != 0))
    case ID_DAP_SWO_Transport:
      num = SWO_Transport(request, response);
      break;
    case ID_DAP_SWO_Mode:
      num = SWO_Mode(request, response);
      break;
    case ID_DAP_SWO_Baudrate:
      num = SWO_Baudrate(request, response);
      break;
    case ID_DAP_SWO_Control:
      num = SWO_Control(request, response);
      break;
    case ID_DAP_SWO_Status:
      num = SWO_Status(response);
      break;
    case ID_DAP_SWO_Data:
      num = SWO_Data(request, response);
      break;
#endif

    default:
      *(response-1) = ID_DAP_Invalid;
      return ((1U << 16) | 1U);
  }

  return ((1U << 16) + 1U + num);
}


// Execute DAP command (process request and prepare response)
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
uint32_t DAP_ExecuteCommand(const uint8_t *request, uint8_t *response) {
  uint32_t cnt, num, n;

  if (*request == ID_DAP_ExecuteCommands) {
    *response++ = *request++;
    cnt = *request++;
    *response++ = (uint8_t)cnt;
    num = (2U << 16) | 2U;
    while (cnt--) {
      n = DAP_ProcessCommand(request, response);
      num += n;
      request  += (uint16_t)(n >> 16);
      response += (uint16_t) n;  
    }
    return (num);
  }

  return DAP_ProcessCommand(request, response);
}


// Setup DAP
void DAP_Setup(void) {

  // Default settings
  DAP_Data.debug_port  = 0U;
  DAP_Data.fast_clock  = 0U;
  DAP_Data.clock_delay = CLOCK_DELAY(DAP_DEFAULT_SWJ_CLOCK);
  DAP_Data.transfer.idle_cycles = 0U;
  DAP_Data.transfer.retry_count = 100U;
  DAP_Data.transfer.match_retry = 0U;
  DAP_Data.transfer.match_mask  = 0x00000000U;
#if (DAP_SWD != 0)
  DAP_Data.swd_conf.turnaround  = 1U;
  DAP_Data.swd_conf.data_phase  = 0U;
#endif
#if (DAP_JTAG != 0)
  DAP_Data.jtag_dev.count = 0U;
#endif

  DAP_SETUP();  // Device specific setup
}
