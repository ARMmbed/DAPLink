/**
 * @file    usbd_cdc_acm.c
 * @brief   Communication Device Class driver
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
 */

#include <string.h>

#include "rl_usb.h"
#include "usb_for_lib.h"

#ifndef CDC_ACM_DEFAULT_BAUDRATE
#define CDC_ACM_DEFAULT_BAUDRATE 9600
#endif

/* Module instance variables                                                    */

/** \ingroup  USBD_CDC_ACM_instance_variables
    \defgroup USBD_CDC_ACM_INSTANCE_VAR  Instance Variables (INSTANCE_VAR)
    \brief      Instance variables used in USBD CDC ACM module
 */
typedef struct
{
    int32_t data_send_access;              /*!< Flag active while send data (in the send intermediate buffer) is being accessed */
    int32_t data_send_active;              /*!< Flag active while data is being sent */
    int32_t data_send_zlp;                 /*!< Flag active when ZLP needs to be sent */
    int32_t data_to_send_wr;               /*!< Number of bytes written to the send intermediate buffer */
    int32_t data_to_send_rd;               /*!< Number of bytes read from the send intermediate buffer */
    uint8_t *ptr_data_to_send;             /*!< Pointer to the send intermediate buffer to the data to be sent */
    uint8_t *ptr_data_sent;                /*!< Pointer to the send intermediate buffer to the data already sent */
    int32_t data_read_access;              /*!< Flag active while read data (in the receive intermediate buffer) is being accessed */
    int32_t data_receive_int_access;       /*!< Flag active while read data (in the receive intermediate buffer) is being accessed from the IRQ function*/
    int32_t data_received_pending_pckts;   /*!< Number of packets received but not handled (pending) */
    int32_t data_no_space_for_receive;     /*!< Flag active while there is no more space for reception */
    uint8_t *ptr_data_received;            /*!< Pointer to the receive intermediate buffer to the received unread data */
    uint8_t *ptr_data_read;                /*!< Pointer to the receive intermediate buffer to the received read data */
    uint16_t control_line_state;           /*!< Control line state settings bitmap (0. bit - DTR state, 1. bit - RTS state) */
    CDC_LINE_CODING line_coding;           /*!< Communication settings */
} cdc_context_t;

static cdc_context_t cdc_contexts[USB_CDC_ACM_EP_COUNT];

/* end of group USBD_CDC_ACM_INSTANCE_VAR */


/* Functions that should be provided by user to use standard Virtual COM port
   functionality                                                              */
__WEAK int32_t USBD_CDC_ACM_PortInitialize(usbd_cdc_num_t cdc_num)
{
    return (0);
}
__WEAK int32_t USBD_CDC_ACM_PortUninitialize(usbd_cdc_num_t cdc_num)
{
    return (0);
}
__WEAK int32_t USBD_CDC_ACM_PortReset(usbd_cdc_num_t cdc_num)
{
    return (0);
}
__WEAK int32_t USBD_CDC_ACM_PortSetLineCoding(usbd_cdc_num_t cdc_num, CDC_LINE_CODING *line_coding)
{
    return (0);
}
__WEAK int32_t USBD_CDC_ACM_PortGetLineCoding(usbd_cdc_num_t cdc_num, CDC_LINE_CODING *line_coding)
{
    return (0);
}
__WEAK int32_t USBD_CDC_ACM_PortSetControlLineState(usbd_cdc_num_t cdc_num, uint16_t ctrl_bmp)
{
    return (0);
}

/* Functions that can be used by user to use standard Virtual COM port
   functionality                                                              */
int32_t USBD_CDC_ACM_DataSend(usbd_cdc_num_t cdc_num, const uint8_t *buf, int32_t len);
int32_t USBD_CDC_ACM_PutChar(usbd_cdc_num_t cdc_num, const uint8_t  ch);
int32_t USBD_CDC_ACM_DataRead(usbd_cdc_num_t cdc_num, uint8_t *buf, int32_t len);
int32_t USBD_CDC_ACM_GetChar(usbd_cdc_num_t cdc_num);
__WEAK int32_t USBD_CDC_ACM_DataReceived(usbd_cdc_num_t cdc_num, int32_t len)
{
    return (0);
}
int32_t USBD_CDC_ACM_DataAvailable(usbd_cdc_num_t cdc_num);
int32_t USBD_CDC_ACM_Notify(usbd_cdc_num_t cdc_num, uint16_t stat);

/* Functions handling CDC ACM requests (can be overridden to provide custom
   handling of CDC ACM requests)                                              */
__WEAK int32_t USBD_CDC_ACM_SendEncapsulatedCommand(usbd_cdc_num_t cdc_num)
{
    return (0);
}
__WEAK int32_t USBD_CDC_ACM_GetEncapsulatedResponse(usbd_cdc_num_t cdc_num)
{
    return (0);
}
__WEAK int32_t USBD_CDC_ACM_SetCommFeature(usbd_cdc_num_t cdc_num, uint16_t feat)
{
    return (0);
}
__WEAK int32_t USBD_CDC_ACM_GetCommFeature(usbd_cdc_num_t cdc_num, uint16_t feat)
{
    return (0);
}
__WEAK int32_t USBD_CDC_ACM_ClearCommFeature(usbd_cdc_num_t cdc_num, uint16_t feat)
{
    return (0);
}
__WEAK int32_t USBD_CDC_ACM_SendBreak(usbd_cdc_num_t cdc_num, uint16_t dur)
{
    return (0);
}


/* Local function prototypes                                                  */
static void USBD_CDC_ACM_EP_BULKOUT_HandleData(usbd_cdc_num_t cdc_num);
static void USBD_CDC_ACM_EP_BULKIN_HandleData(usbd_cdc_num_t cdc_num);


/*----------------- USB CDC ACM class handling functions ---------------------*/

/** \brief  Initialization of the USB CDC class (ACM)

    The function calls USBD_CDC_ACM_PortInitialize function which
    initializes Virtual COM Port.

    \param [in]         cdc_num  CDC endpoint to modify.
    \return             0        Function failed.
    \return             1        Function succeeded.
 */

__WEAK int32_t USBD_CDC_ACM_Initialize(usbd_cdc_num_t cdc_num)
{
    cdc_context_t *context = &cdc_contexts[cdc_num];
    context->data_send_access            = 0;
    context->data_send_active            = 0;
    context->data_send_zlp               = 0;
    context->data_to_send_wr             = 0;
    context->data_to_send_rd             = 0;
    context->ptr_data_to_send            = USBD_CDC_ACM_SendBuf[cdc_num];
    context->ptr_data_sent               = USBD_CDC_ACM_SendBuf[cdc_num];
    context->data_read_access            = 0;
    context->data_receive_int_access     = 0;
    context->data_received_pending_pckts = 0;
    context->data_no_space_for_receive   = 0;
    context->ptr_data_received           = USBD_CDC_ACM_ReceiveBuf[cdc_num];
    context->ptr_data_read               = USBD_CDC_ACM_ReceiveBuf[cdc_num];
    context->control_line_state          = 0;
    context->line_coding.dwDTERate       = CDC_ACM_DEFAULT_BAUDRATE;
    context->line_coding.bCharFormat     = 0;
    context->line_coding.bParityType     = 0;
    context->line_coding.bDataBits       = 8;
    return (USBD_CDC_ACM_PortInitialize(cdc_num));
}


/** \brief  Uninitialization of the USB CDC class (ACM)

    The function calls USBD_CDC_ACM_PortUninitialize function which
    uninitializes Virtual COM Port.

    \param [in]         cdc_num  CDC endpoint to modify.
    \return             0        Function failed.
    \return             1        Function succeeded.
 */

__WEAK int32_t USBD_CDC_ACM_Uninitialize(usbd_cdc_num_t cdc_num)
{
    return (USBD_CDC_ACM_PortUninitialize(cdc_num));
}


/** \brief  Reset of the USB CDC class (ACM) variables and states

    The function resets class variables and states for all CDC EPs, it calls
    USBD_CDC_ACM_PortReset function which resets Virtual COM Port variables
    and states and calls USBD_CDC_ACM_PortSetLineCoding function with
    default parameters to set default communication settings for the
    Virtual COM Port.

    \return             0        Function failed.
    \return             1        Function succeeded.
 */

__WEAK int32_t USBD_CDC_ACM_Reset(void)
{
    int32_t ret = 1;


    for (usbd_cdc_num_t cdc_num = 0; cdc_num < USB_CDC_ACM_EP_COUNT; cdc_num++)
    {
        cdc_context_t *context = &cdc_contexts[cdc_num];
        context->data_send_access            = 0;
        context->data_send_active            = 0;
        context->data_send_zlp               = 0;
        context->data_to_send_wr             = 0;
        context->data_to_send_rd             = 0;
        context->ptr_data_to_send            = USBD_CDC_ACM_SendBuf[cdc_num];
        context->ptr_data_sent               = USBD_CDC_ACM_SendBuf[cdc_num];
        context->data_read_access            = 0;
        context->data_receive_int_access     = 0;
        context->data_received_pending_pckts = 0;
        context->data_no_space_for_receive   = 0;
        context->ptr_data_received           = USBD_CDC_ACM_ReceiveBuf[cdc_num];
        context->ptr_data_read               = USBD_CDC_ACM_ReceiveBuf[cdc_num];
        context->control_line_state          = 0;

        USBD_CDC_ACM_PortReset(cdc_num);

        context->line_coding.dwDTERate       = CDC_ACM_DEFAULT_BAUDRATE;
        context->line_coding.bCharFormat     = 0;
        context->line_coding.bParityType     = 0;
        context->line_coding.bDataBits       = 8;

        ret = USBD_CDC_ACM_PortSetLineCoding(cdc_num, &(context->line_coding));
        if (ret)
        {
            return ret;
        }
    }
    return (ret);
}


/** \brief  Sets Line Coding for the USB CDC ACM Virtual COM Port

    The function is a callback function that forwards USB CDC ACM request
    to set communication settings to the Virtual COM Port.

    \param [in]         cdc_num  CDC endpoint to modify.
    \return             0        Function failed.
    \return             1        Function succeeded.
 */

__WEAK int32_t USBD_CDC_ACM_SetLineCoding(usbd_cdc_num_t cdc_num)
{
    cdc_context_t *context = &cdc_contexts[cdc_num];
    context->line_coding.dwDTERate   = (USBD_EP0Buf[0] <<  0) |
                              (USBD_EP0Buf[1] <<  8) |
                              (USBD_EP0Buf[2] << 16) |
                              (USBD_EP0Buf[3] << 24) ;
    context->line_coding.bCharFormat =  USBD_EP0Buf[4];
    context->line_coding.bParityType =  USBD_EP0Buf[5];
    context->line_coding.bDataBits   =  USBD_EP0Buf[6];
    return (USBD_CDC_ACM_PortSetLineCoding(cdc_num, &(context->line_coding)));
}


/** \brief  Gets Line Coding from the USB CDC ACM Virtual COM Port

    The function is a callback function that forwards USB CDC ACM request
    to get communication settings from the Virtual COM Port.

    \param [in]         cdc_num  CDC endpoint to read.
    \return             0        Function failed.
    \return             1        Function succeeded.
 */

__WEAK int32_t USBD_CDC_ACM_GetLineCoding(usbd_cdc_num_t cdc_num)
{
    cdc_context_t *context = &cdc_contexts[cdc_num];
    if (USBD_CDC_ACM_PortGetLineCoding(cdc_num, &(context->line_coding))) {
        USBD_EP0Buf[0] = (context->line_coding.dwDTERate >>  0) & 0xFF;
        USBD_EP0Buf[1] = (context->line_coding.dwDTERate >>  8) & 0xFF;
        USBD_EP0Buf[2] = (context->line_coding.dwDTERate >> 16) & 0xFF;
        USBD_EP0Buf[3] = (context->line_coding.dwDTERate >> 24) & 0xFF;
        USBD_EP0Buf[4] =  context->line_coding.bCharFormat;
        USBD_EP0Buf[5] =  context->line_coding.bParityType;
        USBD_EP0Buf[6] =  context->line_coding.bDataBits;
        return (1);
    }

    return (0);
}


/** \brief  Sets Control Line State for the USB CDC ACM Virtual COM Port

    The function is a callback function that forwards USB CDC ACM request
    to set desired control line state to the Virtual COM Port.

    \param [in]         cdc_num  CDC endpoint to modify.
    \param [in]         ctrl_bmp Control line settings bitmap (
                          0. bit - DTR state,
                          1. bit - RTS state).
    \return             0        Function failed.
    \return             1        Function succeeded.
 */

__WEAK int32_t USBD_CDC_ACM_SetControlLineState(usbd_cdc_num_t cdc_num, uint16_t ctrl_bmp)
{
    cdc_contexts[cdc_num].control_line_state = ctrl_bmp;
    return (USBD_CDC_ACM_PortSetControlLineState(cdc_num, ctrl_bmp));
}


/*----------------- USB CDC ACM user API functions ---------------------------*/

/** \brief Number of free bytes in the Send buffer for the requested CDC EP.

    \param [in]         cdc_num  CDC endpoint to read.
*/
int32_t USBD_CDC_ACM_DataFree(usbd_cdc_num_t cdc_num)
{
    return ((int32_t)usbd_cdc_acm_sendbuf_sz[cdc_num]) -
            (cdc_contexts[cdc_num].data_to_send_wr - cdc_contexts[cdc_num].data_to_send_rd);
}

/** \brief  Sends data over the USB CDC ACM Virtual COM Port

    The function puts requested data to the send intermediate buffer and
    prepares it for sending over the Virtual COM Port.

    \param [in]         cdc_num  CDC endpoint to send data to.
    \param [in]         buf      Buffer containing data to be sent.
    \param [in]         len      Maximum number of bytes to be sent.
    \return                      Number of bytes accepted to be sent.
 */

int32_t USBD_CDC_ACM_DataSend(usbd_cdc_num_t cdc_num, const uint8_t *buf, int32_t len)
{
    int32_t  len_data, len_available, len_before_wrap;
    uint8_t *buf_loc;
    cdc_context_t *context = &cdc_contexts[cdc_num];
    buf_loc       = (uint8_t *)buf;       /* Pointer to buf                     */
    len_data      = context->data_to_send_wr - context->data_to_send_rd;  /* Num of data in buffer*/
    len_available = ((int32_t)usbd_cdc_acm_sendbuf_sz[cdc_num]) - len_data;  /* Num of
                                           bytes of space available           */

    if (len_available <= 0) {             /* If no space for data to send       */
        return (0);
    }

    if (len > len_available)              /* If more data requested for sending
                                           then available space               */
    {
        len = len_available;    /* Correct to maximum available       */
    }

    len_before_wrap = 0;                  /* Circular buffer size before wrap   */

    if ((context->ptr_data_to_send >= context->ptr_data_sent) && /* If wrap is possible to happen */
            ((context->ptr_data_to_send + len) >= (USBD_CDC_ACM_SendBuf[cdc_num] + usbd_cdc_acm_sendbuf_sz[cdc_num]))) {
        /* If data wraps around end of buffer */
        len_before_wrap   = USBD_CDC_ACM_SendBuf[cdc_num] + usbd_cdc_acm_sendbuf_sz[cdc_num] - context->ptr_data_to_send;
        memcpy(context->ptr_data_to_send, buf_loc, len_before_wrap); /* Copy data till end */
        buf_loc          += len_before_wrap;            /* Increment buf pointer  */
        len              -= len_before_wrap;            /* Decrement bytes to send*/
        context->ptr_data_to_send  = USBD_CDC_ACM_SendBuf[cdc_num];      /* Wrap send buffer
                                                                            pointer to beginning of
                                                                            the send buffer        */
    }

    if (len) {                            /* If there are bytes to send         */
        memcpy(context->ptr_data_to_send, buf_loc, len);   /* Copy data to send buffer     */
        context->ptr_data_to_send += len;            /* Correct position of write pointer  */
    }

    len += len_before_wrap;               /* Total number of bytes prepared for
                                           send                               */
    context->data_to_send_wr += len;               /* Bytes prepared to send counter     */
    return (len);                         /* Number of bytes accepted for send  */
}


/** \brief  Sends a single character over the USB CDC ACM Virtual COM Port

    The function puts requested data character to the send intermediate buffer
    and prepares it for sending over the Virtual COM Port.

    \param [in]         cdc_num  CDC endpoint to send data to.
    \param [in]         ch       Character to be sent.
    \return             -1       Function failed.
    \return                      Character accepted to be sent.
 */

int32_t USBD_CDC_ACM_PutChar(usbd_cdc_num_t cdc_num, const uint8_t ch)
{
    if ((USBD_CDC_ACM_DataSend(cdc_num, &ch, 1)) == 1) {
        return ((uint32_t) ch);
    }

    return (-1);
}


/** \brief  Reads data received over the USB CDC ACM Virtual COM Port

    The function reads data from the receive intermediate buffer that was
    received over the Virtual COM Port.

    \param [in]         cdc_num  CDC endpoint to read data from.
    \param [in]         buf      Buffer to where data will be read.
    \param [in]         len      Maximum number of bytes to be read.
    \return                      Number of bytes actually read.
 */

int32_t USBD_CDC_ACM_DataRead(usbd_cdc_num_t cdc_num, uint8_t *buf, int32_t len)
{
    int32_t len_data;
    cdc_context_t *context = &cdc_contexts[cdc_num];

    if (context->ptr_data_received > context->ptr_data_read) { /*If there is already received data   */
        len_data = context->ptr_data_received - context->ptr_data_read; /* Available bytes of data  */

        if (len > len_data) {               /* If more requested then available   */
            len = len_data;    /* correct to return maximum available*/
        }

        memcpy(buf, context->ptr_data_read, len);    /* Copy received data to provided buf */
        context->ptr_data_read      += len;          /* Correct position of read pointer   */
    } else {
        len = 0;                            /* No data received                   */
    }

    return (len);                         /* Number of bytes actually read      */
}


/** \brief  Reads one character of data received over the USB CDC ACM Virtual COM Port

    The function reads data character from the receive intermediate buffer that
    was received over the Virtual COM Port.

    \param [in]         cdc_num  CDC endpoint to read data from.
    \return             -1       No character available.
    \return                      Received character.
 */

int32_t USBD_CDC_ACM_GetChar(usbd_cdc_num_t cdc_num)
{
    uint8_t ch;

    if ((USBD_CDC_ACM_DataRead(cdc_num, &ch, 1)) == 1) {
        return ((int32_t) ch);
    }

    return (-1);
}


/** \brief  Retrieves number of bytes received over the USB CDC ACM Virtual COM Port

    The function retrieves number of bytes available in the intermediate buffer
    that were received over the Virtual COM Port.

    \param [in]         cdc_num  CDC endpoint to check.
    \return                      Number of bytes available for read.
 */

int32_t USBD_CDC_ACM_DataAvailable(usbd_cdc_num_t cdc_num)
{
    cdc_context_t *context = &cdc_contexts[cdc_num];
    return (context->ptr_data_received - context->ptr_data_read);
}


/** \brief  Sends a notification of Virtual COM Port statuses and line states

    The function sends error and line status of the Virtual COM Port over the
    Interrupt endpoint. (SerialState notification is defined in usbcdc11.pdf, 6.3.5.)

    \param [in]         cdc_num  CDC endpoint to send notification for.
    \param [in]         stat     Error and line statuses (
                                   6. bit - bOverRun,
                                   5. bit - bParity,
                                   4. bit - bFraming,
                                   3. bit - bRingSignal,
                                   2. bit - bBreak,
                                   1. bit - bTxCarrier (DSR line state),
                                   0. bit - bRxCarrier (DCD line status)).
    \return             0        Function failed.
    \return             1        Function succeeded.
 */

int32_t USBD_CDC_ACM_Notify(usbd_cdc_num_t cdc_num, uint16_t stat)
{
    if (USBD_Configuration) {
        USBD_CDC_ACM_NotifyBuf[cdc_num][0] = 0xA1;   /* bmRequestType                      */
        USBD_CDC_ACM_NotifyBuf[cdc_num][1] = CDC_NOTIFICATION_SERIAL_STATE;/* bNotification
                                          (SERIAL_STATE)                      */
        USBD_CDC_ACM_NotifyBuf[cdc_num][2] = 0x00;   /* wValue                             */
        USBD_CDC_ACM_NotifyBuf[cdc_num][3] = 0x00;
        USBD_CDC_ACM_NotifyBuf[cdc_num][4] = 0x00;   /* wIndex (Interface 0)               */
        USBD_CDC_ACM_NotifyBuf[cdc_num][5] = 0x00;
        USBD_CDC_ACM_NotifyBuf[cdc_num][6] = 0x02;   /* wLength                            */
        USBD_CDC_ACM_NotifyBuf[cdc_num][7] = 0x00;
        USBD_CDC_ACM_NotifyBuf[cdc_num][8] = stat >> 0; /* UART State Bitmap                  */
        USBD_CDC_ACM_NotifyBuf[cdc_num][9] = stat >> 8;
        /* Write notification to be sent      */
        USBD_WriteEP(usbd_cdc_acm_ep_intin[cdc_num] | 0x80, USBD_CDC_ACM_NotifyBuf[cdc_num], 10);
        return (1);
    }

    return (0);
}


/*----------------- USB CDC ACM communication event handlers -----------------*/

/** \brief  Handle Reset Events

    The function handles Reset events.
 */

void USBD_CDC_ACM_Reset_Event(void)
{
    USBD_CDC_ACM_Reset();
}


/** \brief  Handle SOF Events

    The function handles Start Of Frame events. It checks if there is pending
    data on any of the CDC Bulk Out endpoints and handles it
    (USBD_CDC_ACM_EP_BULKOUT_HandleData) if there is enough space in the
    intermediate receive buffer and it calls received function callback
    (USBD_CDC_ACM_DataReceived) it also activates data send over the Bulk In
    endpoint if there is data to be sent (USBD_CDC_ACM_EP_BULKIN_HandleData).
 */

void USBD_CDC_ACM_SOF_Event(void)
{
    for (usbd_cdc_num_t cdc_num = 0; cdc_num < USB_CDC_ACM_EP_COUNT; cdc_num++)
    {
        if (!USBD_Configuration) {
            // Don't process events until CDC is
            // configured and the endpoints enabled
            return;
        }

        cdc_context_t *context = &cdc_contexts[cdc_num];

        if ((!context->data_read_access)         &&    /* If not read active                 */
            (context->ptr_data_received == context->ptr_data_read) &&    /* If received and read
                                                                            pointers point to same
                                                                            the location             */
            (context->ptr_data_received != USBD_CDC_ACM_ReceiveBuf[cdc_num])) {
                                                     /* and if receive
                                                        pointer does not already
                                                        point to the start of
                                                        the receive buffer       */
            context->data_read_access = 1;               /* Block access to read data          */
            context->ptr_data_received = USBD_CDC_ACM_ReceiveBuf[cdc_num];  /* Correct received pointer
                                                        to point to the start of
                                                        the receive buffer       */
            context->ptr_data_read     = USBD_CDC_ACM_ReceiveBuf[cdc_num];  /* Correct read pointer to
                                                        point to the start of the
                                                        receive buffer           */
            context->data_no_space_for_receive  = 0;    /* There is space for
                                                           reception available      */
            context->data_read_access = 0;               /* Allow access to read data          */
        }

        if (context->data_received_pending_pckts &&      /* If packets are pending             */
            (!context->data_read_access)         &&      /* and if not read active             */
            (!context->data_no_space_for_receive)) {     /* and if there is space to receive   */
            context->data_read_access = 1;               /* Disable access to read data        */
            USBD_CDC_ACM_EP_BULKOUT_HandleData(cdc_num); /* Handle received data               */
            context->data_read_access = 0;               /* Enable access to read data         */

            if (context->ptr_data_received != context->ptr_data_read) {
                USBD_CDC_ACM_DataReceived(cdc_num, context->ptr_data_received - context->ptr_data_read);
            }  /* Call received callback                  */
        }

        if ((!context->data_send_access)         &&    /* If send data is not being accessed */
            (!context->data_send_active)         &&    /* and send is not active             */
            (context->data_to_send_wr - context->data_to_send_rd) /* and if there is data to be sent    */
        //&& ((context->control_line_state & 3) == 3)    /* and if DTR and RTS is 1            */
        ) {
            context->data_send_access = 1;               /* Block access to send data          */
            context->data_send_active = 1;               /* Start data sending                 */
            USBD_CDC_ACM_EP_BULKIN_HandleData(cdc_num);  /* Handle data to send                */
            context->data_send_access = 0;               /* Allow access to send data          */
        }
    }
}

/** \brief  Handle Interrupt In Endpoint Events for CDC EP 1.

    The function handles Interrupt In endpoint events.

    \param [in]         event    Type of event (USBD_EVT_IN - input event).
 */

void USBD_CDC_ACM_1_EP_INTIN_Event(uint32_t event)
{
    /* Notification will be loadad aynchronously and sent automatically upon
       Interrupt IN token reception                                             */
}

#if (USB_CDC_ACM_EP_COUNT > 1)
/** \brief  Handle Interrupt In Endpoint Events for CDC EP 2.

    The function handles Interrupt In endpoint events.

    \param [in]         event    Type of event (USBD_EVT_IN - input event).
 */

void USBD_CDC_ACM_2_EP_INTIN_Event(uint32_t event)
{
    /* Notification will be loadad aynchronously and sent automatically upon
       Interrupt IN token reception                                             */
}

#if (USB_CDC_ACM_EP_COUNT > 2)
/** \brief  Handle Interrupt In Endpoint Events for CDC EP 3.

    The function handles Interrupt In endpoint events.

    \param [in]         event    Type of event (USBD_EVT_IN - input event).
 */

void USBD_CDC_ACM_3_EP_INTIN_Event(uint32_t event)
{
    /* Notification will be loadad aynchronously and sent automatically upon
       Interrupt IN token reception                                             */
}
#endif
#endif

/** \brief  Handle Bulk Out Endpoint Received Data

    The function handles data received on the Bulk Out endpoint. It reads the
    received data to the receive intermediate buffer if there is enough space
    available.

    \param [in]         cdc_num  CDC endpoint to handle data for.
 */

static void USBD_CDC_ACM_EP_BULKOUT_HandleData(usbd_cdc_num_t cdc_num)
{
    uint32_t len_free_to_recv;
    int32_t len_received;
    cdc_context_t *context = &cdc_contexts[cdc_num];

    if ((usbd_cdc_acm_receivebuf_sz[cdc_num] - (context->ptr_data_received - USBD_CDC_ACM_ReceiveBuf[cdc_num])) >= usbd_cdc_acm_maxpacketsize1[cdc_num][USBD_HighSpeed]) {
        /* If there is space for 1 max packet */
        /* Read received packet to receive buf*/
        len_free_to_recv = usbd_cdc_acm_receivebuf_sz[cdc_num] - (context->ptr_data_received - USBD_CDC_ACM_ReceiveBuf[cdc_num]);
        len_received       = USBD_ReadEP(usbd_cdc_acm_ep_bulkout[cdc_num], context->ptr_data_received, len_free_to_recv);
        context->ptr_data_received += len_received;     /* Correct pointer to received data   */

        if (context->data_received_pending_pckts &&     /* If packet was pending              */
                !context->data_receive_int_access) {    /* and not interrupt access           */
            context->data_received_pending_pckts--;     /* Decrement pending packets number   */
        }
    } else {
        context->data_no_space_for_receive = 1;      /* There is no space in receive buffer
                                                        for the newly received data          */

        if (context->data_receive_int_access) {
            /* If this access is from interrupt function                                     */
            context->data_received_pending_pckts++;    /* then this is new unhandled packet  */
        }
    }
}


/** \brief  Handle Bulk In Endpoint Data to Send

    The function handles data to be sent on the Bulk In endpoint. It transmits
    pending data to be sent that is already in the send intermediate buffer,
    and it also sends Zero Length Packet if last packet sent was not a short
    packet.

    \param [in]         cdc_num  CDC endpoint to handle data for.
 */

static void USBD_CDC_ACM_EP_BULKIN_HandleData(usbd_cdc_num_t cdc_num)
{
    int32_t len_to_send, len_sent;
    cdc_context_t *context = &cdc_contexts[cdc_num];

    if (!context->data_send_active) {     /* If sending is not active           */
        return;
    }

    len_to_send = context->data_to_send_wr - context->data_to_send_rd;  /* Num of data to send    */

    /* Check if sending is finished                                             */
    if (!len_to_send    &&                /* If all data was sent               */
            !context->data_send_zlp)  {   /* and ZLP was sent if necessary also */
        context->data_send_active = 0;    /* Sending not active any more        */
        return;
    }

    /* Check if data needs to be sent                                           */
    if (len_to_send) {
        /* If there is data available do be sent                               */
        if ((context->ptr_data_sent >= context->ptr_data_to_send) && /* If data before end of buf avail*/
                ((context->ptr_data_sent + len_to_send) >= (USBD_CDC_ACM_SendBuf[cdc_num] + usbd_cdc_acm_sendbuf_sz[cdc_num]))) {
            /* and if available data wraps around
               the end of the send buffer         */
            /* Correct bytes to send to data
               available untill end of send buf   */
            len_to_send = USBD_CDC_ACM_SendBuf[cdc_num] + usbd_cdc_acm_sendbuf_sz[cdc_num] - context->ptr_data_sent;
        }

        if (len_to_send > usbd_cdc_acm_maxpacketsize1[cdc_num][USBD_HighSpeed]) {
            /* If there is more data to be sent then
               can be sent in a single packet     */
            /* Correct to send maximum pckt size  */
            len_to_send = usbd_cdc_acm_maxpacketsize1[cdc_num][USBD_HighSpeed];
        }
    } else if (context->data_send_zlp) {           /* or if ZLP should be sent           */
        len_to_send = 0;
    }

    context->data_send_zlp = 0;
    /* Send data                          */
    len_sent = USBD_WriteEP(usbd_cdc_acm_ep_bulkin[cdc_num] | 0x80, context->ptr_data_sent, len_to_send);
    context->ptr_data_sent    += len_sent;         /* Correct position of sent pointer   */
    context->data_to_send_rd  += len_sent;         /* Correct num of bytes left to send  */

    if (context->ptr_data_sent == USBD_CDC_ACM_SendBuf[cdc_num] + usbd_cdc_acm_sendbuf_sz[cdc_num])
        /* If pointer to sent data wraps      */
    {
        context->ptr_data_sent = USBD_CDC_ACM_SendBuf[cdc_num];
    } /* Correct it to beginning of send buffer                             */

    if ((context->data_to_send_wr == context->data_to_send_rd) &&   /* If there are no more
                                                                       bytes available to be sent */
            (len_sent == usbd_cdc_acm_maxpacketsize1[cdc_num][USBD_HighSpeed])) {
        /* If last packet size was same as
           maximum packet size                */
        context->data_send_zlp = 1;                  /* ZLP packet should be sent          */
    } else {
        context->data_send_zlp = 0;                  /* No ZLP packet should be sent       */
    }
}


/** \brief  Handle Bulk Out Endpoint Events

    The function handles Bulk Out endpoint events. It calls
    USBD_CDC_ACM_EP_BULKOUT_HandleData function to handle received data
    unless data was being accessed in which case function just acknowledges
    that there is data to be handled later.

    \param [in]         cdc_num  CDC endpoint to handle events for.
    \param [in]         event    Type of event (USBD_EVT_OUT - output event).
 */

static void USBD_CDC_ACM_EP_BULKOUT_Event(usbd_cdc_num_t cdc_num, uint32_t event)
{
    cdc_context_t *context = &cdc_contexts[cdc_num];

    if (context->data_read_access) {
        /* If data is being accessed from read function                      */
        context->data_received_pending_pckts++;      /* 1 more packet received and not
                                                        handled                            */
        return;
    }

    context->data_read_access = 1;                 /* Block access to read data          */
    context->data_receive_int_access = 1;          /* Read access from interrupt function*/
    USBD_CDC_ACM_EP_BULKOUT_HandleData(cdc_num);   /* Handle received data               */
    context->data_receive_int_access = 0;          /* Read access from interrupt func end*/
    context->data_read_access = 0;                 /* Allow access to read data          */

    if (context->ptr_data_received != context->ptr_data_read) {
        USBD_CDC_ACM_DataReceived(cdc_num, context->ptr_data_received - context->ptr_data_read);
    }    /* Call received callback                  */
}

/** \brief  Handle Bulk Out Endpoint Events for CDC EP 1

    This function simply maps to the correct endpoint number enum argument.

    \param [in]         event    Type of event (USBD_EVT_OUT - output event).
 */

void USBD_CDC_ACM_1_EP_BULKOUT_Event(uint32_t event)
{
    USBD_CDC_ACM_EP_BULKOUT_Event(USB_CDC_ACM_NUM_1_USB2UART, event);
}

#if (USB_CDC_ACM_EP_COUNT > 1)
/** \brief  Handle Bulk Out Endpoint Events for CDC EP 2

    This function simply maps to the correct endpoint number enum argument.

    \param [in]         event    Type of event (USBD_EVT_OUT - output event).
 */

void USBD_CDC_ACM_2_EP_BULKOUT_Event(uint32_t event)
{
    USBD_CDC_ACM_EP_BULKOUT_Event(USB_CDC_ACM_NUM_2_USER, event);
}

#if (USB_CDC_ACM_EP_COUNT > 2)
/** \brief  Handle Bulk Out Endpoint Events for CDC EP 3

    This function simply maps to the correct endpoint number enum argument.

    \param [in]         event    Type of event (USBD_EVT_OUT - output event).
 */

void USBD_CDC_ACM_3_EP_BULKOUT_Event(uint32_t event)
{
    USBD_CDC_ACM_EP_BULKOUT_Event(USB_CDC_ACM_NUM_3_USER, event);
}
#endif
#endif

/** \brief  Handle Bulk In Endpoint Events

    The function handles Bulk In endpoint events. It calls
    USBD_CDC_ACM_EP_BULKIN_HandleData function to handle send data
    unless data was being accessed in which case function just returns.

    \param [in]         cdc_num  CDC endpoint to handle events for.
    \param [in]         event    Type of event (USBD_EVT_IN - input event).
 */

static void USBD_CDC_ACM_EP_BULKIN_Event(usbd_cdc_num_t cdc_num, uint32_t event)
{
    cdc_context_t *context = &cdc_contexts[cdc_num];
    if (context->data_send_access                  /* If send data is being accessed     */
// ||((context->control_line_state & 3) != 3)    /* or if DTR or RTS is 0              */
       ) {
        return;
    }

    context->data_send_access = 1;                 /* Block access to send data          */
    USBD_CDC_ACM_EP_BULKIN_HandleData(cdc_num);  /* Handle data to send                */
    context->data_send_access = 0;                 /* Allow access to send data          */
}

/** \brief  Handle Bulk In Endpoint Events for EP 1

    This function simply maps to the correct endpoint number enum argument.

    \param [in]         event    Type of event (USBD_EVT_IN - input event).
 */

void USBD_CDC_ACM_1_EP_BULKIN_Event(uint32_t event)
{
    USBD_CDC_ACM_EP_BULKIN_Event(USB_CDC_ACM_NUM_1_USB2UART, event);
}

#if (USB_CDC_ACM_EP_COUNT > 1)
/** \brief  Handle Bulk In Endpoint Events for EP 2

    This function simply maps to the correct endpoint number enum argument.

    \param [in]         event    Type of event (USBD_EVT_IN - input event).
 */

void USBD_CDC_ACM_2_EP_BULKIN_Event(uint32_t event)
{
    USBD_CDC_ACM_EP_BULKIN_Event(USB_CDC_ACM_NUM_2_USER, event);
}

#if (USB_CDC_ACM_EP_COUNT > 2)
/** \brief  Handle Bulk In Endpoint Events for EP 3

    This function simply maps to the correct endpoint number enum argument.

    \param [in]         event    Type of event (USBD_EVT_IN - input event).
 */

void USBD_CDC_ACM_3_EP_BULKIN_Event(uint32_t event)
{
    USBD_CDC_ACM_EP_BULKIN_Event(USB_CDC_ACM_NUM_3_USER, event);
}
#endif
#endif

/** \brief  Handle Bulk In/Out Endpoint Events

    The function handles Bulk In/Out endpoint events. It is used for endpoints
    that do In and Out functionality on the same endpoint number. It dispatches
    events to appropriate In or Out event handlers.

    \param [in]         cdc_num  CDC endpoint to handle events for.
    \param [in]         event    Type of event (
                                   USBD_EVT_IN  - input event,
                                   USBD_EVT_OUT - output event).
 */

static void USBD_CDC_ACM_EP_BULK_Event(usbd_cdc_num_t cdc_num, uint32_t event)
{
    if (event & USBD_EVT_OUT) {
        USBD_CDC_ACM_EP_BULKOUT_Event(cdc_num, event);
    }

    if (event & USBD_EVT_IN) {
        USBD_CDC_ACM_EP_BULKIN_Event(cdc_num, event);
    }
}

/** \brief  Handle Bulk In/Out Endpoint Events for CDC EP 1

    This function simply maps to the correct endpoint number enum argument.

    \param [in]         event    Type of event (
                                   USBD_EVT_IN  - input event,
                                   USBD_EVT_OUT - output event).
 */

void USBD_CDC_ACM_1_EP_BULK_Event(uint32_t event)
{
    USBD_CDC_ACM_EP_BULK_Event(USB_CDC_ACM_NUM_1_USB2UART, event);
}

/** \brief  Handle Bulk In/Out Endpoint Events for CDC EP 2

    This function simply maps to the correct endpoint number enum argument.

    \param [in]         event    Type of event (
                                   USBD_EVT_IN  - input event,
                                   USBD_EVT_OUT - output event).
 */

#if (USB_CDC_ACM_EP_COUNT > 1)
void USBD_CDC_ACM_2_EP_BULK_Event(uint32_t event)
{
    USBD_CDC_ACM_EP_BULK_Event(USB_CDC_ACM_NUM_2_USER, event);
}

/** \brief  Handle Bulk In/Out Endpoint Events for CDC EP 3

    This function simply maps to the correct endpoint number enum argument.

    \param [in]         event    Type of event (
                                   USBD_EVT_IN  - input event,
                                   USBD_EVT_OUT - output event).
 */

#if (USB_CDC_ACM_EP_COUNT > 2)
void USBD_CDC_ACM_3_EP_BULK_Event(uint32_t event)
{
    USBD_CDC_ACM_EP_BULK_Event(USB_CDC_ACM_NUM_3_USER, event);
}
#endif
#endif

#ifdef __RTX                            /* RTX tasks for handling events      */

/** \brief  Task Handling Interrupt In Endpoint Events

    The task dispatches Interrupt In events to the Interrupt In handling
    function (USBD_CDC_ACM_EP_INTIN_Event).

    \param [in]         cdc_num  CDC endpoint to handle events for.
 */

static void USBD_RTX_CDC_ACM_EP_INTIN_Event(usbd_cdc_num_t cdc_num)
{
    for (;;) {
        usbd_os_evt_wait_or(0xFFFF, 0xFFFF);
        USBD_CDC_ACM_EP_INTIN_Event(cdc_num, usbd_os_evt_get());
    }
}

/** \brief  Task Handling Interrupt In Endpoint Events for CDC EP 1

    This function simply maps to the correct endpoint number enum argument.
 */

void USBD_RTX_CDC_ACM_1_EP_INTIN_Event(void)
{
    USBD_RTX_CDC_ACM_EP_INTIN_Event(USB_CDC_ACM_NUM_1_USB2UART);
}

#if (USB_CDC_ACM_EP_COUNT > 1)
/** \brief  Task Handling Interrupt In Endpoint Events for CDC EP 2

    This function simply maps to the correct endpoint number enum argument.
 */

void USBD_RTX_CDC_ACM_2_EP_INTIN_Event(void)
{
    USBD_RTX_CDC_ACM_EP_INTIN_Event(USB_CDC_ACM_NUM_2_USER);
}

#if (USB_CDC_ACM_EP_COUNT > 2)
/** \brief  Task Handling Interrupt In Endpoint Events for CDC EP 3

    This function simply maps to the correct endpoint number enum argument.
 */

void USBD_RTX_CDC_ACM_3_EP_INTIN_Event(void)
{
    USBD_RTX_CDC_ACM_EP_INTIN_Event(USB_CDC_ACM_NUM_3_USER);
}
#endif
#endif
/** \brief  Task Handling Bulk In Endpoint Events

    The task dispatches Bulk In events to the Bulk In handling
    function (USBD_CDC_ACM_EP_BULKIN_Event).

    \param [in]         cdc_num  CDC endpoint to handle events for.
 */

static void USBD_RTX_CDC_ACM_EP_BULKIN_Event(usbd_cdc_num_t cdc_num)
{
    for (;;) {
        usbd_os_evt_wait_or(0xFFFF, 0xFFFF);

        if (usbd_os_evt_get() & USBD_EVT_IN) {
            USBD_CDC_ACM_EP_BULKIN_Event(cdc_num, 0);
        }
    }
}

/** \brief  Task Handling Bulk In Endpoint Events for CDC EP 1

    This function simply maps to the correct endpoint number enum argument.
 */

void USBD_RTX_CDC_ACM_1_EP_BULKIN_Event(void)
{
    USBD_RTX_CDC_ACM_EP_BULKIN_Event(USB_CDC_ACM_NUM_1_USB2UART);
}

#if (USB_CDC_ACM_EP_COUNT > 1)
/** \brief  Task Handling Bulk In Endpoint Events for CDC EP 2

    This function simply maps to the correct endpoint number enum argument.
 */

void USBD_RTX_CDC_ACM_2_EP_BULKIN_Event(void)
{
    USBD_RTX_CDC_ACM_EP_BULKIN_Event(USB_CDC_ACM_NUM_2_USER);
}

#if (USB_CDC_ACM_EP_COUNT > 2)
/** \brief  Task Handling Bulk In Endpoint Events for CDC EP 3

    This function simply maps to the correct endpoint number enum argument.
 */

void USBD_RTX_CDC_ACM_3_EP_BULKIN_Event(void)
{
    USBD_RTX_CDC_ACM_EP_BULKIN_Event(USB_CDC_ACM_NUM_3_USER);
}
#endif
#endif

/** \brief  Task Handling Bulk Out Endpoint Events

    The task dispatches Bulk Out events to the Bulk Out handling
    function (USBD_CDC_ACM_EP_BULKOUT_Event).

    \param [in]         cdc_num  CDC endpoint to handle events for.
 */

static void USBD_RTX_CDC_ACM_EP_BULKOUT_Event(usbd_cdc_num_t cdc_num)
{
    for (;;) {
        usbd_os_evt_wait_or(0xFFFF, 0xFFFF);

        if (usbd_os_evt_get() & USBD_EVT_OUT) {
            USBD_CDC_ACM_EP_BULKOUT_Event(cdc_num, 0);
        }
    }
}

/** \brief  Task Handling Bulk Out Endpoint Events for CDC EP 1

    This function simply maps to the correct endpoint number enum argument.
 */

void USBD_RTX_CDC_ACM_1_EP_BULKOUT_Event(void)
{
    USBD_RTX_CDC_ACM_EP_BULKOUT_Event(USB_CDC_ACM_NUM_1_USB2UART);
}

#if (USB_CDC_ACM_EP_COUNT > 1)
/** \brief  Task Handling Bulk Out Endpoint Events for CDC EP 2

    This function simply maps to the correct endpoint number enum argument.
 */

void USBD_RTX_CDC_ACM_2_EP_BULKOUT_Event(void)
{
    USBD_RTX_CDC_ACM_EP_BULKOUT_Event(USB_CDC_ACM_NUM_2_USER);
}

#if (USB_CDC_ACM_EP_COUNT > 2)
/** \brief  Task Handling Bulk Out Endpoint Events for CDC EP 3

    This function simply maps to the correct endpoint number enum argument.
 */

void USBD_RTX_CDC_ACM_3_EP_BULKOUT_Event(void)
{
    USBD_RTX_CDC_ACM_EP_BULKOUT_Event(USB_CDC_ACM_NUM_3_USER);
}
#endif
#endif

/** \brief  Task Handling Bulk In/Out Endpoint Events

    The task dispatches Bulk In/Out events to the Bulk In/Out handling
    function (USBD_CDC_ACM_EP_BULK_Event).

    \param [in]         cdc_num  CDC endpoint to handle events for.
 */

static void USBD_RTX_CDC_ACM_EP_BULK_Event(usbd_cdc_num_t cdc_num)
{
    for (;;) {
        usbd_os_evt_wait_or(0xFFFF, 0xFFFF);
        USBD_CDC_ACM_EP_BULK_Event(cdc_num, usbd_os_evt_get());
    }
}

/** \brief  Task Handling Bulk In/Out Endpoint Events for CDC EP 1

    This function simply maps to the correct endpoint number enum argument.
 */

void USBD_RTX_CDC_ACM_1_EP_BULK_Event(void)
{
    USBD_RTX_CDC_ACM_EP_BULK_Event(USB_CDC_ACM_NUM_1_USB2UART);
}

#if (USB_CDC_ACM_EP_COUNT > 1)
/** \brief  Task Handling Bulk In/Out Endpoint Events for CDC EP 2

    This function simply maps to the correct endpoint number enum argument.
 */

void USBD_RTX_CDC_ACM_2_EP_BULK_Event(void)
{
    USBD_RTX_CDC_ACM_EP_BULK_Event(USB_CDC_ACM_NUM_2_USER);
}

#if (USB_CDC_ACM_EP_COUNT > 2)
/** \brief  Task Handling Bulk In/Out Endpoint Events for CDC EP 3

    This function simply maps to the correct endpoint number enum argument.
 */

void USBD_RTX_CDC_ACM_3_EP_BULK_Event(void)
{
    USBD_RTX_CDC_ACM_EP_BULK_Event(USB_CDC_ACM_NUM_3_USER);
}
#endif
#endif

#endif

