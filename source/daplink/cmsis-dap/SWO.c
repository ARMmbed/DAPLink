/*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
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
 * ----------------------------------------------------------------------
 *
 * $Date:        20. May 2015
 * $Revision:    V1.10
 *
 * Project:      CMSIS-DAP Source
 * Title:        SWO.c CMSIS-DAP SWO I/O
 *
 *---------------------------------------------------------------------------*/

#include "DAP_config.h"
#include "DAP.h"
#if (SWO_UART != 0)
#include "Driver_USART.h"
#endif


#if (SWO_UART != 0)

#ifndef  SWO_USART_PORT
#define  SWO_USART_PORT 0           /* USART Port Number */
#endif

// USART Driver
#define _USART_Driver_(n)  Driver_USART##n
#define  USART_Driver_(n) _USART_Driver_(n)
extern ARM_DRIVER_USART    USART_Driver_(SWO_USART_PORT);
#define pUSART           (&USART_Driver_(SWO_USART_PORT))

static uint8_t  USART_Ready;

#endif  /* (SWO_UART != 0) */


#if ((SWO_UART != 0) || (SWO_MANCHESTER != 0))


// Trace State
static uint8_t  TraceTransport =  0U;       /* Trace Transport */
static uint8_t  TraceMode      =  0U;       /* Trace Mode */
static uint8_t  TraceStatus    =  0U;       /* Trace Status without Errors */
static uint8_t  TraceError[2]  = {0U, 0U};  /* Trace Error flags (banked) */
static uint8_t  TraceError_n   =  0U;       /* Active Trace Error bank */

// Trace Buffer
static uint8_t  TraceBuf[SWO_BUFFER_SIZE];  /* Trace Buffer (must be 2^n) */
static volatile uint32_t TraceIn      = 0U; /* Incoming Trace Index */
static volatile uint32_t TraceOut     = 0U; /* Outgoing Trace Index */
static volatile uint32_t TracePending = 0U; /* Pending Trace Count */

// Trace Helper functions
static void     ClearTrace     (void);
static uint32_t GetTraceSpace  (void);
static uint32_t GetTraceCount  (void);
static uint8_t  GetTraceStatus (void);
static void     SetTraceError  (uint8_t flag);


#if (SWO_UART != 0)

// USART Driver Callback function
//   event: event mask
static void USART_Callback (uint32_t event) {
  uint32_t count;

  if (event &  ARM_USART_EVENT_RECEIVE_COMPLETE) {
    TracePending = 0U;
    TraceIn += pUSART->GetRxCount();
    count = GetTraceSpace();
    if (count != 0U) {
      pUSART->Receive(&TraceBuf[TraceIn & (SWO_BUFFER_SIZE-1U)], count);
    } else {
      TraceStatus = DAP_SWO_CAPTURE_ACTIVE | DAP_SWO_CAPTURE_PAUSED;
    }
  }
  if (event &  ARM_USART_EVENT_RX_OVERFLOW) {
    SetTraceError(DAP_SWO_BUFFER_OVERRUN);
  }
  if (event & (ARM_USART_EVENT_RX_BREAK         |
               ARM_USART_EVENT_RX_FRAMING_ERROR |
               ARM_USART_EVENT_RX_PARITY_ERROR)) {
    SetTraceError(DAP_SWO_STREAM_ERROR);
  }
}

// Enable or disable UART SWO Mode
//   enable: enable flag
//   return: 1 - Success, 0 - Error
__weak uint32_t UART_SWO_Mode (uint32_t enable) {
  int32_t status;

  USART_Ready = 0U;

  if (enable) {
    status = pUSART->Initialize(USART_Callback);
    if (status != ARM_DRIVER_OK) { return (0U); }
    status = pUSART->PowerControl(ARM_POWER_FULL);
    if (status != ARM_DRIVER_OK) {
      pUSART->Uninitialize();
      return (0U);
    }
  } else {
    pUSART->Control(ARM_USART_ABORT_RECEIVE, 0U);
    pUSART->PowerControl(ARM_POWER_OFF);
    pUSART->Uninitialize();
  }
  return (1U);
}

// Configure UART SWO Baudrate
//   baudrate: requested baudrate
//   return:   actual baudrate or 0 when not configured
__weak uint32_t UART_SWO_Baudrate (uint32_t baudrate) {
  int32_t  status;
  uint32_t count;

  if (baudrate > SWO_UART_MAX_BAUDRATE) {
    baudrate = SWO_UART_MAX_BAUDRATE;
  }

  if (TraceStatus & DAP_SWO_CAPTURE_ACTIVE) {
    pUSART->Control(ARM_USART_CONTROL_RX, 0U);
    if (pUSART->GetStatus().rx_busy) {
      TracePending = 0U;
      TraceIn += pUSART->GetRxCount();
      pUSART->Control(ARM_USART_ABORT_RECEIVE, 0U);
    }
  }

  status = pUSART->Control(ARM_USART_MODE_ASYNCHRONOUS |
                           ARM_USART_DATA_BITS_8       |
                           ARM_USART_PARITY_NONE       |
                           ARM_USART_STOP_BITS_1,
                           baudrate);

  if (status == ARM_DRIVER_OK) {
    USART_Ready = 1U;
  } else {
    USART_Ready = 0U;
    baudrate = 0U;
  }

  if ((TraceStatus & DAP_SWO_CAPTURE_ACTIVE) && USART_Ready) {
    pUSART->Control(ARM_USART_CONTROL_RX, 1U);
    count = GetTraceSpace();
    if (count != 0U) {
      pUSART->Receive(&TraceBuf[TraceIn & (SWO_BUFFER_SIZE-1U)], count);
    } else {
      TraceStatus = DAP_SWO_CAPTURE_ACTIVE | DAP_SWO_CAPTURE_PAUSED;
    }
  }

  return (baudrate);
}

// Control UART SWO Capture
//   active: active flag
//   return: 1 - Success, 0 - Error
__weak uint32_t UART_SWO_Control (uint32_t active) {
  int32_t status;

  if (active) {
    if (!USART_Ready) { return (0U); }
    status = pUSART->Control(ARM_USART_CONTROL_RX, 1U);
    if (status != ARM_DRIVER_OK) { return (0U); }
    status = pUSART->Receive(TraceBuf, SWO_BUFFER_SIZE);
    if (status != ARM_DRIVER_OK) { return (0U); }
  } else {
    pUSART->Control(ARM_USART_CONTROL_RX, 0U);
    if (pUSART->GetStatus().rx_busy) {
      TracePending = 0U;
      TraceIn += pUSART->GetRxCount();
      pUSART->Control(ARM_USART_ABORT_RECEIVE, 0U);
    }
  }
  return (1U);
}

// Start UART SWO Capture
//   buf:   pointer to buffer for capturing
//   count: number of bytes to capture
__weak void UART_SWO_Capture (uint8_t *buf, uint32_t count) {
  pUSART->Receive(buf, count);
}

// Update UART SWO Trace Info
__weak void UART_SWO_Update (void) {
  TracePending = pUSART->GetRxCount();
}

#endif  /* (SWO_UART != 0) */


#if (SWO_MANCHESTER != 0)

// Enable or disable Manchester SWO Mode
//   enable: enable flag
//   return: 1 - Success, 0 - Error
__weak uint32_t Manchester_SWO_Mode (uint32_t enable) {
  return (0U);
}

// Configure Manchester SWO Baudrate
//   baudrate: requested baudrate
//   return:   actual baudrate or 0 when not configured
__weak uint32_t Manchester_SWO_Baudrate (uint32_t baudrate) {
  return (0U);
}

// Control Manchester SWO Capture
//   active: active flag
//   return: 1 - Success, 0 - Error
__weak uint32_t Manchester_SWO_Control (uint32_t active) {
  return (0U);
}

// Start Manchester SWO Capture
//   buf:   pointer to buffer for capturing
//   count: number of bytes to capture
__weak void Manchester_SWO_Capture (uint8_t *buf, uint32_t count) {
}

// Update Manchester SWO Trace Info
__weak void Manchester_SWO_Update (void) {
}

#endif  /* (SWO_MANCHESTER != 0) */


// Clear Trace Errors and Data
static void ClearTrace (void) {
  TraceError[0] = 0U;
  TraceError[1] = 0U;
  TraceError_n  = 0U;
  TraceIn       = 0U;
  TraceOut      = 0U;
  TracePending  = 0U;
}

// Get Trace Space
//   return: number of contiguous free bytes in trace buffer
static uint32_t GetTraceSpace (void) {
  uint32_t index;
  uint32_t limit;
  uint32_t count;

  index = TraceIn & (SWO_BUFFER_SIZE-1U);
  limit = SWO_BUFFER_SIZE - index;
  count = SWO_BUFFER_SIZE - (TraceIn - TraceOut);
  if (count > limit) {
    count = limit;
  }

  return (count);
}

// Get Trace Count
//   return: number of available data bytes in trace buffer
static uint32_t GetTraceCount (void) {
  uint32_t count;

  if (TraceStatus == DAP_SWO_CAPTURE_ACTIVE) {
    count = (TraceIn - TraceOut) + TracePending;
    if (TracePending == 0U) {
      count = TraceIn - TraceOut;
    }
  } else {
    count = TraceIn - TraceOut;
  }

  return (count);
}

// Get Trace Status (clear Error flags)
//   return: Trace Status (Active flag and Error flags)
static uint8_t GetTraceStatus (void) {
  uint8_t  status;
  uint32_t n;

  n = TraceError_n;
  TraceError_n ^= 1U;
  status = TraceStatus | TraceError[n];
  TraceError[n] = 0U;

  return (status);
}

// Set Trace Error flag(s)
//   flag:  error flag(s) to set
static void SetTraceError (uint8_t flag) {
  TraceError[TraceError_n] |= flag;
}


// Process SWO Transport command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
uint32_t SWO_Transport (const uint8_t *request, uint8_t *response) {
  uint8_t  transport;
  uint32_t result;

  if (!(TraceStatus & DAP_SWO_CAPTURE_ACTIVE)) {
    transport = *request;
    switch (transport) {
      case 0:
      case 1:
        TraceTransport = transport;
        result = 1U;
        break;
      default:
        result = 0U;
        break;
    }
  } else {
    result = 0U;
  }

  if (result != 0U) {
    *response = DAP_OK;
  } else {
    *response = DAP_ERROR;
  }

  return ((1U << 16) | 1U);
}


// Process SWO Mode command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
uint32_t SWO_Mode (const uint8_t *request, uint8_t *response) {
  uint8_t  mode;
  uint32_t result;

  mode = *request;

  switch (TraceMode) {
#if (SWO_UART != 0)
    case DAP_SWO_UART:
      UART_SWO_Mode(0U);
      break;
#endif
#if (SWO_MANCHESTER != 0)
    case DAP_SWO_MANCHESTER:
      Manchester_SWO_Mode(0U);
      break;
#endif
    default:
      break;
  }
  switch (mode) {
    case DAP_SWO_OFF:
      result = 1U;
      break;
#if (SWO_UART != 0)
    case DAP_SWO_UART:
      result = UART_SWO_Mode(1U);
      break;
#endif
#if (SWO_MANCHESTER != 0)
    case DAP_SWO_MANCHESTER:
      result = Manchester_SWO_Mode(1U);
      break;
#endif
    default:
      result = 0U;
      break;
  }
  if (result != 0U) {
    TraceMode = mode;
  } else {
    TraceMode = DAP_SWO_OFF;
  }
  TraceStatus = 0U;
  ClearTrace();

  if (result != 0U) {
    *response = DAP_OK;
  } else {
    *response = DAP_ERROR;
  }

  return ((1U << 16) | 1U);
}


// Process SWO Baudrate command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
uint32_t SWO_Baudrate (const uint8_t *request, uint8_t *response) {
  uint32_t baudrate;

  baudrate = (*(request+0) <<  0) |
             (*(request+1) <<  8) |
             (*(request+2) << 16) |
             (*(request+3) << 24);

  switch (TraceMode) {
#if (SWO_UART != 0)
    case DAP_SWO_UART:
      baudrate = UART_SWO_Baudrate(baudrate);
      break;
#endif
#if (SWO_MANCHESTER != 0)
    case DAP_SWO_MANCHESTER:
      baudrate = Manchester_SWO_Baudrate(baudrate);
      break;
#endif
    default:
      baudrate = 0U;
      break;
  }

  if (baudrate == 0U) {
    TraceStatus = 0U;
  }

  *response++ = (uint8_t)(baudrate >>  0);
  *response++ = (uint8_t)(baudrate >>  8);
  *response++ = (uint8_t)(baudrate >> 16);
  *response   = (uint8_t)(baudrate >> 24);

  return ((4U << 16) | 4U);
}


// Process SWO Control command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
uint32_t SWO_Control (const uint8_t *request, uint8_t *response) {
  uint8_t  active;
  uint32_t result;

  active = *request & DAP_SWO_CAPTURE_ACTIVE;

  if (active != (TraceStatus & DAP_SWO_CAPTURE_ACTIVE)) {
    if (active) {
      ClearTrace();
    }
    switch (TraceMode) {
#if (SWO_UART != 0)
      case DAP_SWO_UART:
        result = UART_SWO_Control(active);
        break;
#endif
#if (SWO_MANCHESTER != 0)
      case DAP_SWO_MANCHESTER:
        result = Manchester_SWO_Control(active);
        break;
#endif
      default:
        result = 0U;
        break;
    }
    if (result != 0U) {
      TraceStatus = active;
    }
  } else {
    result = 1U;
  }

  if (result != 0U) {
    *response = DAP_OK;
  } else {
    *response = DAP_ERROR;
  }

  return ((1U << 16) | 1U);
}


// Process SWO Status command and prepare response
//   response: pointer to response data
//   return:   number of bytes in response
uint32_t SWO_Status (uint8_t *response) {
  uint8_t  status;
  uint32_t count;

  if (TraceStatus == DAP_SWO_CAPTURE_ACTIVE) {
    switch (TraceMode) {
#if (SWO_UART != 0)
      case DAP_SWO_UART:
        UART_SWO_Update();
        break;
#endif
#if (SWO_MANCHESTER != 0)
      case DAP_SWO_MANCHESTER:
        Manchester_SWO_Update();
        break;
#endif
      default:
        break;
    }
  }

  status = GetTraceStatus();
  count  = GetTraceCount();

  *response++ = status;
  *response++ = (uint8_t)(count >>  0);
  *response++ = (uint8_t)(count >>  8);
  *response++ = (uint8_t)(count >> 16);
  *response   = (uint8_t)(count >> 24);

  return (5U);
}


// Process SWO Data command and prepare response
//   request:  pointer to request data
//   response: pointer to response data
//   return:   number of bytes in response (lower 16 bits)
//             number of bytes in request (upper 16 bits)
uint32_t SWO_Data (const uint8_t *request, uint8_t *response) {
  uint8_t  status;
  uint32_t count;
  uint32_t n;

  if (TraceStatus == DAP_SWO_CAPTURE_ACTIVE) {
    switch (TraceMode) {
#if (SWO_UART != 0)
      case DAP_SWO_UART:
        UART_SWO_Update();
        break;
#endif
#if (SWO_MANCHESTER != 0)
      case DAP_SWO_MANCHESTER:
        Manchester_SWO_Update();
        break;
#endif
      default:
        break;
    }
  }

  status = GetTraceStatus();
  count  = GetTraceCount();

  if (TraceTransport == 1U) {
    n = (*(request+0) << 0) |
        (*(request+1) << 8);
  } else {
    n = 0U;
  }
  if (count > n) {
    count = n;
  }

  *response++ = status;
  *response++ = (uint8_t)(count >> 0);
  *response++ = (uint8_t)(count >> 8);

  for (n = count; n; n--) {
    *response++ = TraceBuf[TraceOut++ & (SWO_BUFFER_SIZE-1U)];
  }

  if (TraceStatus == (DAP_SWO_CAPTURE_ACTIVE | DAP_SWO_CAPTURE_PAUSED)) {
    n = GetTraceSpace();
    if (n != 0U) {
      switch (TraceMode) {
#if (SWO_UART != 0)
        case DAP_SWO_UART:
          UART_SWO_Capture(&TraceBuf[TraceIn & (SWO_BUFFER_SIZE-1U)], n);
          TraceStatus = DAP_SWO_CAPTURE_ACTIVE;
          break;
#endif
#if (SWO_MANCHESTER != 0)
        case DAP_SWO_MANCHESTER:
          Manchester_SWO_Capture(&TraceBuf[TraceIn & (SWO_BUFFER_SIZE-1U)], n);
          TraceStatus = DAP_SWO_CAPTURE_ACTIVE;
          break;
#endif
        default:
          break;
      }
    }
  }

  return ((2U << 16) | (3U + count));
}


#endif  /* ((SWO_UART != 0) || (SWO_MANCHESTER != 0)) */
