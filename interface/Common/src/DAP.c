/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include "DAP_config.h"
#include "DAP.h"
#include "semihost.h"


#define DAP_FW_VER      "1.0"   // Firmware Version


#if (DAP_PACKET_SIZE < 64)
#error "Minimum Packet Size is 64"
#endif
#if (DAP_PACKET_SIZE > 32768)
#error "Maximum Packet Size is 32768"
#endif
#if (DAP_PACKET_COUNT < 1)
#error "Minimum Packet Count is 1"
#endif
#if (DAP_PACKET_COUNT > 255)
#error "Maximum Packet Count is 255"
#endif


// Clock Macros

#define MAX_SWJ_CLOCK(delay_cycles) \
  (CPU_CLOCK/2 / (IO_PORT_WRITE_CYCLES + delay_cycles))

#define CLOCK_DELAY(swj_clock) \
 ((CPU_CLOCK/2 / swj_clock) - IO_PORT_WRITE_CYCLES)


         DAP_Data_t DAP_Data;           // DAP Data
volatile uint8_t    DAP_TransferAbort;  // Trasfer Abort Flag


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
  uint8_t length = 0;

  switch (id) {
    case DAP_ID_VENDOR:
#ifdef DAP_VENDOR
      memcpy(info, DAP_Vendor, sizeof(DAP_Vendor));
      length = sizeof(DAP_Vendor);
#endif
      break;
    case DAP_ID_PRODUCT:
#ifdef DAP_PRODUCT
      memcpy(info, DAP_Product, sizeof(DAP_Product));
      length = sizeof(DAP_Product);
#endif
      break;
    case DAP_ID_SER_NUM:
#ifdef DAP_SER_NUM
      memcpy(info, DAP_SerNum, sizeof(DAP_SerNum));
      length = sizeof(DAP_SerNum);
#endif
      break;
    case DAP_ID_FW_VER:
      memcpy(info, DAP_FW_Ver, sizeof(DAP_FW_Ver));
      length = sizeof(DAP_FW_Ver);
      break;
    case DAP_ID_DEVICE_VENDOR:
#if TARGET_DEVICE_FIXED
      memcpy(info, TargetDeviceVendor, sizeof(TargetDeviceVendor));
      length = sizeof(TargetDeviceVendor);
#endif
      break;
    case DAP_ID_DEVICE_NAME:
#if TARGET_DEVICE_FIXED
      memcpy(info, TargetDeviceName, sizeof(TargetDeviceName));
      length = sizeof(TargetDeviceName);
#endif
      break;
    case DAP_ID_CAPABILITIES:
      info[0] = ((DAP_SWD  != 0) ? (1 << 0) : 0) |
                ((DAP_JTAG != 0) ? (1 << 1) : 0);
      length = 1;
      break;
    case DAP_ID_PACKET_SIZE:
      info[0] = (uint8_t)(DAP_PACKET_SIZE >> 0);
      info[1] = (uint8_t)(DAP_PACKET_SIZE >> 8);
      length = 2;
      break;
    case DAP_ID_PACKET_COUNT:
      info[0] = DAP_PACKET_COUNT;
      length = 1;
      break;
  }

  return (length);
}


// Timer Functions

#if ((DAP_SWD != 0) || (DAP_JTAG != 0))

// Start Timer
static __inline void TIMER_START (uint32_t usec) {
  SysTick->VAL  = 0;
  SysTick->LOAD = usec * CPU_CLOCK/1000000;
  SysTick->CTRL = (1 << SysTick_CTRL_ENABLE_Pos) |
                  (1 << SysTick_CTRL_CLKSOURCE_Pos);
}

// Stop Timer
static __inline void TIMER_STOP (void) {
  SysTick->CTRL = 0;
}

// Check if Timer expired
static __inline uint32_t TIMER_EXPIRED (void) {
  return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) ? 1 : 0);
}

#endif


// Delay for specified time
//    delay:  delay time in ms
void Delayms(uint32_t delay) {
  delay *= (CPU_CLOCK/1000 + (DELAY_SLOW_CYCLES-1)) / DELAY_SLOW_CYCLES;
  PIN_DELAY_SLOW(delay);
}


// Process Delay command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
static uint32_t DAP_Delay(uint8_t *request, uint8_t *response) {
  uint32_t delay;

  delay  = *(request+0) | (*(request+1) << 8);
  delay *= (CPU_CLOCK/1000000 + (DELAY_SLOW_CYCLES-1)) / DELAY_SLOW_CYCLES;

  PIN_DELAY_SLOW(delay);

  *response = DAP_OK;
  return (1);
}


// Process Host Status command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
static uint32_t DAP_HostStatus(uint8_t *request, uint8_t *response) {

  switch (*request) {
    case DAP_DEBUGGER_CONNECTED:
      LED_CONNECTED_OUT((*(request+1) & 1));
      break;
    case DAP_TARGET_RUNNING:
      LED_RUNNING_OUT((*(request+1) & 1));
      break;
    default:
      *response = DAP_ERROR;
      return (1);
  }

  *response = DAP_OK;
  return (1);
}


// Process Connect command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
static uint32_t DAP_Connect(uint8_t *request, uint8_t *response) {
  uint32_t port;

  if (*request == DAP_PORT_AUTODETECT) {
    port = DAP_DEFAULT_PORT;
  } else {
    port = *request;
  }

  semihost_disable();

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
      *response = DAP_PORT_DISABLED;
      return (1);
  }

  *response = port;
  return (1);
}


// Process Disconnect command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
static uint32_t DAP_Disconnect(uint8_t *response) {

  DAP_Data.debug_port = DAP_PORT_DISABLED;
  PORT_OFF();

  semihost_enable();

  *response = DAP_OK;
  return (1);
}


// Process Reset Target command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
static uint32_t DAP_ResetTarget(uint8_t *response) {

  *(response+1) = RESET_TARGET();
  *(response+0) = DAP_OK;
  return (2);
}


// Process SWJ Pins command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if ((DAP_SWD != 0) || (DAP_JTAG != 0))
static uint32_t DAP_SWJ_Pins(uint8_t *request, uint8_t *response) {
  uint32_t value;
  uint32_t select;
  uint32_t wait;

  value  =  *(request+0);
  select =  *(request+1);
  wait   = (*(request+2) <<  0) |
           (*(request+3) <<  8) |
           (*(request+4) << 16) |
           (*(request+5) << 24);

  if (select & (1 << DAP_SWJ_SWCLK_TCK)) {
    if (value & (1 << DAP_SWJ_SWCLK_TCK)) {
      PIN_SWCLK_TCK_SET();
    } else {
      PIN_SWCLK_TCK_CLR();
    }
  }
  if (select & (1 << DAP_SWJ_SWDIO_TMS)) {
    if (value & (1 << DAP_SWJ_SWDIO_TMS)) {
      PIN_SWDIO_TMS_SET();
    } else {
      PIN_SWDIO_TMS_CLR();
    }
  }
  if (select & (1 << DAP_SWJ_TDI)) {
    PIN_TDI_OUT(value >> DAP_SWJ_TDI);
  }
  if (select & (1 << DAP_SWJ_nTRST)) {
    PIN_nTRST_OUT(value >> DAP_SWJ_nTRST);
  }
  if (select & (1 << DAP_SWJ_nRESET)) {
    PIN_nRESET_OUT(value >> DAP_SWJ_nRESET);
  }

  if (wait) {
    if (wait > 3000000) wait = 3000000;
    TIMER_START(wait);
    do {
      if (select & (1 << DAP_SWJ_SWCLK_TCK)) {
        if ((value >> DAP_SWJ_SWCLK_TCK) ^ PIN_SWCLK_TCK_IN()) continue;
      }
      if (select & (1 << DAP_SWJ_SWDIO_TMS)) {
        if ((value >> DAP_SWJ_SWDIO_TMS) ^ PIN_SWDIO_TMS_IN()) continue;
      }
      if (select & (1 << DAP_SWJ_TDI)) {
        if ((value >> DAP_SWJ_TDI) ^ PIN_TDI_IN()) continue;
      }
      if (select & (1 << DAP_SWJ_nTRST)) {
        if ((value >> DAP_SWJ_nTRST) ^ PIN_nTRST_IN()) continue;
      }
      if (select & (1 << DAP_SWJ_nRESET)) {
        if ((value >> DAP_SWJ_nRESET) ^ PIN_nRESET_IN()) continue;
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
  return (1);
}
#endif


// Process SWJ Clock command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if ((DAP_SWD != 0) || (DAP_JTAG != 0))
static uint32_t DAP_SWJ_Clock(uint8_t *request, uint8_t *response) {
  uint32_t clock;
  uint32_t delay;

  clock = (*(request+0) <<  0) |
          (*(request+1) <<  8) |
          (*(request+2) << 16) |
          (*(request+3) << 24);

  if (clock == 0) {
    *response = DAP_ERROR;
    return (1);
  }

  if (clock >= MAX_SWJ_CLOCK(DELAY_FAST_CYCLES)) {
    DAP_Data.fast_clock  = 1;
    DAP_Data.clock_delay = 1;
  } else {
    DAP_Data.fast_clock  = 0;

    delay = (CPU_CLOCK/2 + (clock - 1)) / clock;
    if (delay > IO_PORT_WRITE_CYCLES) {
      delay -= IO_PORT_WRITE_CYCLES;
      delay  = (delay + (DELAY_SLOW_CYCLES - 1)) / DELAY_SLOW_CYCLES;
    } else {
      delay  = 1;
    }

    DAP_Data.clock_delay = delay;
  }

  *response = DAP_OK;
  return (1);
}
#endif


// Process SWJ Sequence command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if ((DAP_SWD != 0) || (DAP_JTAG != 0))
static uint32_t DAP_SWJ_Sequence(uint8_t *request, uint8_t *response) {
  uint32_t count;

  count = *request++;
  if (count == 0) count = 256;

  SWJ_Sequence(count, request);

  *response = DAP_OK;
  return (1);
}
#endif


// Process SWD Configure command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if (DAP_SWD != 0)
static uint32_t DAP_SWD_Configure(uint8_t *request, uint8_t *response) {
  uint8_t value;

  value = *request;
  DAP_Data.swd_conf.turnaround  = (value & 0x03) + 1;
  DAP_Data.swd_conf.data_phase  = (value & 0x04) ? 1 : 0;

  *response = DAP_OK;

  return (1);
}
#endif


// Process SWD Abort command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if (DAP_SWD != 0)
static uint32_t DAP_SWD_Abort(uint8_t *request, uint8_t *response) {
  uint32_t data;

  if (DAP_Data.debug_port != DAP_PORT_SWD) {
    *response = DAP_ERROR;
    return (1);
  }

  // Load data (Ignore DAP index)
  data = (*(request+1) <<  0) |
         (*(request+2) <<  8) |
         (*(request+3) << 16) |
         (*(request+4) << 24);

  // Write Abort register
  SWD_Transfer(DP_ABORT, &data);
  *response = DAP_OK;

  return (1);
}
#endif


// Process JTAG Sequence command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if (DAP_JTAG != 0)
static uint32_t DAP_JTAG_Sequence(uint8_t *request, uint8_t *response) {
  uint32_t sequence_info;
  uint32_t sequence_count;
  uint32_t response_count;
  uint32_t count;

  *response++ = DAP_OK;
  response_count = 1;

  sequence_count = *request++;
  while (sequence_count--) {
    sequence_info = *request++;
    JTAG_Sequence(sequence_info, request, response);
    count = sequence_info & JTAG_SEQUENCE_TCK;
    if (count == 0) count = 64;
    count = (count + 7) / 8;
    request += count;
    if (sequence_info & JTAG_SEQUENCE_TDO) {
      response += count;
      response_count += count;
    }
  }

  return (response_count);
}
#endif


// Process JTAG Configure command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if (DAP_JTAG != 0)
static uint32_t DAP_JTAG_Configure(uint8_t *request, uint8_t *response) {
  uint32_t count;
  uint32_t length;
  uint32_t bits;
  uint32_t n;

  count = *request++;
  DAP_Data.jtag_dev.count = count;

  bits = 0;
  for (n = 0; n < count; n++) {
    length = *request++;
    DAP_Data.jtag_dev.ir_length[n] = length;
    DAP_Data.jtag_dev.ir_before[n] = bits;
    bits += length;
  }
  for (n = 0; n < count; n++) {
    bits -= DAP_Data.jtag_dev.ir_length[n];
    DAP_Data.jtag_dev.ir_after[n] = bits;
  }

  *response = DAP_OK;
  return (1);
}
#endif


// Process JTAG IDCODE command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if (DAP_JTAG != 0)
static uint32_t DAP_JTAG_IDCode(uint8_t *request, uint8_t *response) {
  uint32_t data;

  if (DAP_Data.debug_port != DAP_PORT_JTAG) {
err:*response = DAP_ERROR;
    return (1);
  }

  // Device index (JTAP TAP)
  DAP_Data.jtag_dev.index = *request;
  if (DAP_Data.jtag_dev.index >= DAP_Data.jtag_dev.count) goto err;

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

  return (1+4);
}
#endif


// Process JTAG Abort command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if (DAP_JTAG != 0)
static uint32_t DAP_JTAG_Abort(uint8_t *request, uint8_t *response) {
  uint32_t data;

  if (DAP_Data.debug_port != DAP_PORT_JTAG) {
err:*response = DAP_ERROR;
    return (1);
  }

  // Device index (JTAP TAP)
  DAP_Data.jtag_dev.index = *request;
  if (DAP_Data.jtag_dev.index >= DAP_Data.jtag_dev.count) goto err;

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

  return (1);
}
#endif


// Process Transfer Configure command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
static uint32_t DAP_TransferConfigure(uint8_t *request, uint8_t *response) {

  DAP_Data.transfer.idle_cycles = *(request+0);
  DAP_Data.transfer.retry_count = *(request+1) | (*(request+2) << 8);
  DAP_Data.transfer.match_retry = *(request+3) | (*(request+4) << 8);

  *response = DAP_OK;

  return (1);
}


// Process SWD Transfer command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if (DAP_SWD != 0)
static uint32_t DAP_SWD_Transfer(uint8_t *request, uint8_t *response) {
  uint32_t  request_count;
  uint32_t  request_value;
  uint32_t  response_count;
  uint32_t  response_value;
  uint8_t  *response_head;
  uint32_t  post_read;
  uint32_t  check_write;
  uint32_t  match_value;
  uint32_t  match_retry;
  uint32_t  retry;
  uint32_t  data;

  response_count = 0;
  response_value = 0;
  response_head  = response;
  response      += 2;

  DAP_TransferAbort = 0;

  post_read   = 0;
  check_write = 0;

  request++;            // Ignore DAP index

  request_count = *request++;
  while (request_count--) {
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
          post_read = 0;
        }
        if (response_value != DAP_TRANSFER_OK) break;
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
          if (response_value != DAP_TRANSFER_OK) break;
        }
        do {
          // Read register until its value matches or retry counter expires
          retry = DAP_Data.transfer.retry_count;
          do {
            response_value = SWD_Transfer(request_value, &data);
          } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
          if (response_value != DAP_TRANSFER_OK) break;
        } while (((data & DAP_Data.transfer.match_mask) != match_value) && match_retry-- && !DAP_TransferAbort);
        if ((data & DAP_Data.transfer.match_mask) != match_value) {
          response_value |= DAP_TRANSFER_MISMATCH;
        }
        if (response_value != DAP_TRANSFER_OK) break;
      } else {
        // Normal read
        retry = DAP_Data.transfer.retry_count;
        if (request_value & DAP_TRANSFER_APnDP) {
          // Read AP register
          if (post_read == 0) {
            // Post AP read
            do {
              response_value = SWD_Transfer(request_value, NULL);
            } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
            if (response_value != DAP_TRANSFER_OK) break;
            post_read = 1;
          }
        } else {
          // Read DP register
          do {
            response_value = SWD_Transfer(request_value, &data);
          } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
          if (response_value != DAP_TRANSFER_OK) break;
          // Store data
          *response++ = (uint8_t) data;
          *response++ = (uint8_t)(data >>  8);
          *response++ = (uint8_t)(data >> 16);
          *response++ = (uint8_t)(data >> 24);
        }
      }
      check_write = 0;
    } else {
      // Write register
      if (post_read) {
        // Read previous data
        retry = DAP_Data.transfer.retry_count;
        do {
          response_value = SWD_Transfer(DP_RDBUFF | DAP_TRANSFER_RnW, &data);
        } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
        if (response_value != DAP_TRANSFER_OK) break;
        // Store previous data
        *response++ = (uint8_t) data;
        *response++ = (uint8_t)(data >>  8);
        *response++ = (uint8_t)(data >> 16);
        *response++ = (uint8_t)(data >> 24);
        post_read = 0;
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
        if (response_value != DAP_TRANSFER_OK) break;
        check_write = 1;
      }
    }
    response_count++;
    if (DAP_TransferAbort) break;
  }

  if (response_value == DAP_TRANSFER_OK) {
    if (post_read) {
      // Read previous data
      retry = DAP_Data.transfer.retry_count;
      do {
        response_value = SWD_Transfer(DP_RDBUFF | DAP_TRANSFER_RnW, &data);
      } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
      if (response_value != DAP_TRANSFER_OK) goto end;
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

  return (response - response_head);
}
#endif


// Process JTAG Transfer command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if (DAP_JTAG != 0)
static uint32_t DAP_JTAG_Transfer(uint8_t *request, uint8_t *response) {
  uint32_t  request_count;
  uint32_t  request_value;
  uint32_t  request_ir;
  uint32_t  response_count;
  uint32_t  response_value;
  uint8_t  *response_head;
  uint32_t  post_read;
  uint32_t  match_value;
  uint32_t  match_retry;
  uint32_t  retry;
  uint32_t  data;
  uint32_t  ir;

  response_count = 0;
  response_value = 0;
  response_head  = response;
  response      += 2;

  DAP_TransferAbort = 0;

  ir        = 0;
  post_read = 0;

  // Device index (JTAP TAP)
  DAP_Data.jtag_dev.index = *request++;
  if (DAP_Data.jtag_dev.index >= DAP_Data.jtag_dev.count) goto end;

  request_count = *request++;
  while (request_count--) {
    request_value = *request++;
    request_ir = (request_value & DAP_TRANSFER_APnDP) ? JTAG_APACC : JTAG_DPACC;
    if (request_value & DAP_TRANSFER_RnW) {
      // Read register
      if (post_read) {
        // Read was posted before
        retry = DAP_Data.transfer.retry_count;
        if ((ir == request_ir) && ((request_value & DAP_TRANSFER_MATCH_VALUE) == 0)) {
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
          post_read = 0;
        }
        if (response_value != DAP_TRANSFER_OK) break;
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
        if (response_value != DAP_TRANSFER_OK) break;
        do {
          // Read register until its value matches or retry counter expires
          retry = DAP_Data.transfer.retry_count;
          do {
            response_value = JTAG_Transfer(request_value, &data);
          } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
          if (response_value != DAP_TRANSFER_OK) break;
        } while (((data & DAP_Data.transfer.match_mask) != match_value) && match_retry-- && !DAP_TransferAbort);
        if ((data & DAP_Data.transfer.match_mask) != match_value) {
          response_value |= DAP_TRANSFER_MISMATCH;
        }
        if (response_value != DAP_TRANSFER_OK) break;
      } else {
        // Normal read
        if (post_read == 0) {
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
          if (response_value != DAP_TRANSFER_OK) break;
          post_read = 1;
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
        if (response_value != DAP_TRANSFER_OK) break;
        // Store previous data
        *response++ = (uint8_t) data;
        *response++ = (uint8_t)(data >>  8);
        *response++ = (uint8_t)(data >> 16);
        *response++ = (uint8_t)(data >> 24);
        post_read = 0;
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
        if (response_value != DAP_TRANSFER_OK) break;
      }
    }
    response_count++;
    if (DAP_TransferAbort) break;
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
      if (response_value != DAP_TRANSFER_OK) goto end;
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

  return (response - response_head);
}
#endif


// Process SWD Transfer Block command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
#if (DAP_SWD != 0)
static uint32_t DAP_SWD_TransferBlock(uint8_t *request, uint8_t *response) {
  uint32_t  request_count;
  uint32_t  request_value;
  uint32_t  response_count;
  uint32_t  response_value;
  uint8_t  *response_head;
  uint32_t  retry;
  uint32_t  data;

  response_count = 0;
  response_value = 0;
  response_head  = response;
  response      += 3;

  DAP_TransferAbort = 0;

  request++;            // Ignore DAP index

  request_count = *request | (*(request+1) << 8);
  request += 2;
  if (request_count == 0) goto end;

  request_value = *request++;
  if (request_value & DAP_TRANSFER_RnW) {
    // Read register block
    if (request_value & DAP_TRANSFER_APnDP) {
      // Post AP read
      retry = DAP_Data.transfer.retry_count;
      do {
        response_value = SWD_Transfer(request_value, NULL);
      } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
      if (response_value != DAP_TRANSFER_OK) goto end;
    }
    while (request_count--) {
      // Read DP/AP register
      if ((request_count == 0) && (request_value & DAP_TRANSFER_APnDP)) {
        // Last AP read
        request_value = DP_RDBUFF | DAP_TRANSFER_RnW;
      }
      retry = DAP_Data.transfer.retry_count;
      do {
        response_value = SWD_Transfer(request_value, &data);
      } while ((response_value == DAP_TRANSFER_WAIT) && retry-- && !DAP_TransferAbort);
      if (response_value != DAP_TRANSFER_OK) goto end;
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
      if (response_value != DAP_TRANSFER_OK) goto end;
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
static uint32_t DAP_JTAG_TransferBlock(uint8_t *request, uint8_t *response) {
  uint32_t  request_count;
  uint32_t  request_value;
  uint32_t  response_count;
  uint32_t  response_value;
  uint8_t  *response_head;
  uint32_t  retry;
  uint32_t  data;
  uint32_t  ir;

  response_count = 0;
  response_value = 0;
  response_head  = response;
  response      += 3;

  DAP_TransferAbort = 0;

  // Device index (JTAP TAP)
  DAP_Data.jtag_dev.index = *request++;
  if (DAP_Data.jtag_dev.index >= DAP_Data.jtag_dev.count) goto end;

  request_count = *request | (*(request+1) << 8);
  request += 2;
  if (request_count == 0) goto end;

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
    if (response_value != DAP_TRANSFER_OK) goto end;
    // Read register block
    while (request_count--) {
      // Read DP/AP register
      if (request_count == 0) {
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
      if (response_value != DAP_TRANSFER_OK) goto end;
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
      if (response_value != DAP_TRANSFER_OK) goto end;
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


// Process DAP Vendor command and prepare response
// Default function (can be overridden)
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
__weak uint32_t DAP_ProcessVendorCommand(uint8_t *request, uint8_t *response) {
  *response = ID_DAP_Invalid;
  return (1);
}


// Process DAP command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response
uint32_t DAP_ProcessCommand(uint8_t *request, uint8_t *response) {
  uint32_t num;

  if ((*request >= ID_DAP_Vendor0) && (*request <= ID_DAP_Vendor31)) {
    return DAP_ProcessVendorCommand(request, response);
  }

  *response++ = *request;

  switch (*request++) {
    case ID_DAP_Info:
      num = DAP_Info(*request, response+1);
      *response = num;
      return (2 + num);
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

#if ((DAP_SWD != 0) || (DAP_JTAG != 0))
    case ID_DAP_SWJ_Pins:
      num = DAP_SWJ_Pins(request, response);
      break;
    case ID_DAP_SWJ_Clock:
      num = DAP_SWJ_Clock(request, response);
      break;
    case ID_DAP_SWJ_Sequence:
      num = DAP_SWJ_Sequence(request, response);
      break;
#else
    case ID_DAP_SWJ_Pins:
    case ID_DAP_SWJ_Clock:
    case ID_DAP_SWJ_Sequence:
      *response = DAP_ERROR;
      return (2);
#endif

#if (DAP_SWD != 0)
    case ID_DAP_SWD_Configure:
      num = DAP_SWD_Configure(request, response);
      break;
#else
    case ID_DAP_SWD_Configure:
      *response = DAP_ERROR;
      return (2);
#endif

#if (DAP_JTAG != 0)
    case ID_DAP_JTAG_Sequence:
      num = DAP_JTAG_Sequence(request, response);
      break;
    case ID_DAP_JTAG_Configure:
      num = DAP_JTAG_Configure(request, response);
      break;
    case ID_DAP_JTAG_IDCODE:
      num = DAP_JTAG_IDCode(request, response);
      break;
#else
    case ID_DAP_JTAG_Sequence:
    case ID_DAP_JTAG_Configure:
    case ID_DAP_JTAG_IDCODE:
      *response = DAP_ERROR;
      return (2);
#endif

    case ID_DAP_TransferConfigure:
      num = DAP_TransferConfigure(request, response);
      break;

    case ID_DAP_Transfer:
      switch (DAP_Data.debug_port) {
#if (DAP_SWD != 0)
        case DAP_PORT_SWD:
          num = DAP_SWD_Transfer (request, response);
          break;
#endif
#if (DAP_JTAG != 0)
        case DAP_PORT_JTAG:
          num = DAP_JTAG_Transfer(request, response);
          break;
#endif
        default:
          *(response+0) = 0;    // Response count
          *(response+1) = 0;    // Response value
          num = 2;
      }
      break;

    case ID_DAP_TransferBlock:
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
          *(response+0) = 0;    // Response count [7:0]
          *(response+1) = 0;    // Response count[15:8]
          *(response+2) = 0;    // Response value
          num = 3;
      }
      break;

    case ID_DAP_WriteABORT:
      switch (DAP_Data.debug_port) {
#if (DAP_SWD != 0)
        case DAP_PORT_SWD:
          num = DAP_SWD_Abort (request, response);
          break;
#endif
#if (DAP_JTAG != 0)
        case DAP_PORT_JTAG:
          num = DAP_JTAG_Abort(request, response);
          break;
#endif
        default:
          *response = DAP_ERROR;
          return (2);
      }
      break;

    default:
      *(response-1) = ID_DAP_Invalid;
      return (1);
  }

  return (1 + num);
}


// Setup DAP
void DAP_Setup(void) {

  // Default settings (only non-zero values)
//DAP_Data.debug_port  = 0;
//DAP_Data.fast_clock  = 0;
  DAP_Data.clock_delay = CLOCK_DELAY(DAP_DEFAULT_SWJ_CLOCK);
//DAP_Data.transfer.idle_cycles = 0;
  DAP_Data.transfer.retry_count = 100;
//DAP_Data.transfer.match_retry = 0;
//DAP_Data.transfer.match_mask  = 0x000000;
#if (DAP_SWD != 0)
  DAP_Data.swd_conf.turnaround  = 1;
//DAP_Data.swd_conf.data_phase  = 0;
#endif
#if (DAP_JTAG != 0)
//DAP_Data.jtag_dev.count = 0;
#endif

  DAP_SETUP();  // Device specific setup
}
