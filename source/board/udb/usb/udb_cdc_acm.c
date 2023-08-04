/**
 * @file    udb_cdc_acm.c
 * @brief   implementation of USB CDC ACM middleware
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2020 Arm Limited, All Rights Reserved
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

#include "cmsis_os2.h"
#include "rl_usb.h"
#include "daplink.h"
#include DAPLINK_MAIN_HEADER
#include "uart.h"
#ifdef DRAG_N_DROP_SUPPORT
#include "flash_intf.h"
#endif
#include "target_family.h"
#include "usb_for_lib.h"
#include "util.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "nluif_udb-daplink.h"
#include "udb_log.h"
#include "udb_uart_b.h"

static const char error_msg[] = "\r\n<OVERFLOW>\r\n";

int32_t USBD_CDC_ACM_PortInitialize(usbd_cdc_num_t cdc_num)
{
    if (cdc_num == USB_CDC_ACM_NUM_1_USB2UART)
    {
        uart_initialize();
        main_cdc_send_event();
    }
    else if (cdc_num == USB_CDC_ACM_NUM_2_USER)
    {
        udb_uart_b_initialize();
        main_cdc_send_event();
    }
    else if (cdc_num == USB_CDC_ACM_NUM_3_USER)
    {
        // Disable buffering of stdout, otherwise prints without newline won't
        // appear until stdout is flushed or sth else is printed
        setbuf(stdout, NULL);
        main_cdc_send_event();
    }
    return 1;
}

int32_t USBD_CDC_ACM_PortUninitialize(usbd_cdc_num_t cdc_num)
{
    if (cdc_num == USB_CDC_ACM_NUM_1_USB2UART)
    {
        uart_uninitialize();
    }
    else if (cdc_num == USB_CDC_ACM_NUM_2_USER)
    {
        udb_uart_b_uninitialize();
    }
    return 1;
}

int32_t USBD_CDC_ACM_PortReset(usbd_cdc_num_t cdc_num)
{
    if (cdc_num == USB_CDC_ACM_NUM_1_USB2UART)
    {
        uart_reset();
    }
    else if (cdc_num == USB_CDC_ACM_NUM_2_USER)
    {
        udb_uart_b_reset();
    }
    return 1;
}

int32_t USBD_CDC_ACM_PortSetLineCoding(usbd_cdc_num_t cdc_num, CDC_LINE_CODING *line_coding)
{
    int ret = 1;
    UART_Configuration UART_Config;
    UART_Config.Baudrate    = line_coding->dwDTERate;
    UART_Config.DataBits    = (UART_DataBits) line_coding->bDataBits;
    UART_Config.Parity      = (UART_Parity)   line_coding->bParityType;
    UART_Config.StopBits    = (UART_StopBits) line_coding->bCharFormat;
    UART_Config.FlowControl = UART_FLOW_CONTROL_NONE;

    if (cdc_num == USB_CDC_ACM_NUM_1_USB2UART)
    {
        ret = uart_set_configuration(&UART_Config);
    }
    else if (cdc_num == USB_CDC_ACM_NUM_2_USER)
    {
        ret = udb_uart_b_set_configuration(&UART_Config);
    }
    return ret;
}

int32_t USBD_CDC_ACM_PortGetLineCoding(usbd_cdc_num_t cdc_num, CDC_LINE_CODING *line_coding)
{
    UART_Configuration UART_Config = {0};

    if (cdc_num == USB_CDC_ACM_NUM_1_USB2UART)
    {
        uart_get_configuration(&UART_Config);
    }
    else if (cdc_num == USB_CDC_ACM_NUM_2_USER)
    {
        udb_uart_b_get_configuration(&UART_Config);
    }

    line_coding->dwDTERate   = UART_Config.Baudrate;
    line_coding->bDataBits   = UART_Config.DataBits;
    line_coding->bParityType = UART_Config.Parity;
    line_coding->bCharFormat = UART_Config.StopBits;

    return (1);
}

int32_t USBD_CDC_ACM_SendBreak(usbd_cdc_num_t cdc_num, uint16_t dur)
{
    // Disable reset of target when a cdc break is received on CDC EP 1 for UDB.
    // This is used in MBED test automation, but causes hiccups for our use
    // (i.e. closing the uart on a mac) https://github.com/ARMmbed/DAPLink/issues/738
    return (1);
}

int32_t USBD_CDC_ACM_PortSetControlLineState(usbd_cdc_num_t cdc_num, uint16_t ctrl_bmp)
{
    if (cdc_num == USB_CDC_ACM_NUM_1_USB2UART)
    {
        uart_set_control_line_state(ctrl_bmp);
    }
    else if (cdc_num == USB_CDC_ACM_NUM_2_USER)
    {
        udb_uart_b_set_control_line_state(ctrl_bmp);
    }
    else if (cdc_num == USB_CDC_ACM_NUM_3_USER)
    {
        // bit 0 of ctrl_bmp is DTR state. If the terminal implements
        // flow control, it will set DTR to 1 when cdc is connected
        // and set DTR to 0 when disconnected
        if (ctrl_bmp & 1)
        {
            udb_log_set_cdc_ready(true);
        }
        else
        {
            udb_log_set_cdc_ready(false);
        }
    }
    return (1);
}

void cdc_process_event(void)
{
    uint8_t data[64];
    int32_t len_data = 0;

    /* CDC 1 */
    len_data = USBD_CDC_ACM_DataFree(USB_CDC_ACM_NUM_1_USB2UART);

    if (len_data > sizeof(data)) {
        len_data = sizeof(data);
    }

    if (len_data) {
        len_data = uart_read_data(data, len_data);
    }

    if (len_data) {
        if (USBD_CDC_ACM_DataSend(USB_CDC_ACM_NUM_1_USB2UART, data , len_data)) {
            main_blink_cdc_led(MAIN_LED_FLASH);
        }
    }

    len_data = uart_write_free();

    if (len_data > sizeof(data)) {
        len_data = sizeof(data);
    }

    if (len_data) {
        len_data = USBD_CDC_ACM_DataRead(USB_CDC_ACM_NUM_1_USB2UART, data, len_data);
    }

    if (len_data) {
        if (uart_write_data(data, len_data)) {
            main_blink_cdc_led(MAIN_LED_FLASH);
        }
    }

    /* CDC 2 */
    len_data = USBD_CDC_ACM_DataFree(USB_CDC_ACM_NUM_2_USER);

    if (len_data > sizeof(data)) {
        len_data = sizeof(data);
    }

    if (len_data) {
        len_data = udb_uart_b_read_data(data, len_data);
    }

    if (len_data) {
        if (USBD_CDC_ACM_DataSend(USB_CDC_ACM_NUM_2_USER, data , len_data)) {
            main_blink_cdc_led(MAIN_LED_FLASH);
        }
    }

    len_data = udb_uart_b_write_free();

    if (len_data > sizeof(data)) {
        len_data = sizeof(data);
    }

    if (len_data) {
        len_data = USBD_CDC_ACM_DataRead(USB_CDC_ACM_NUM_2_USER, data, len_data);
    }

    if (len_data) {
        if (udb_uart_b_write_data(data, len_data)) {
            main_blink_cdc_led(MAIN_LED_FLASH);
        }
    }

    /* CDC 3 */
    len_data = USBD_CDC_ACM_DataRead(USB_CDC_ACM_NUM_3_USER, data, sizeof(data));

    if (len_data)
    {
        for (uint8_t i = 0; i < len_data; i++)
        {
            if (uif_handle_input_char(data[i]))
            {
                udb_log_set_cdc_ready(true);
                uif_run_cmd();
                uif_prompt();
            }
        }
    }

    // Always process events
    main_cdc_send_event();
}

/*
   overwrite the _write() in libnosys to enable logging with printf()
 */
int _write(int file, char *ptr, int len_to_write)
{
    uint32_t write_free;
    uint32_t total_free = USBD_CDC_ACM_DataFree(USB_CDC_ACM_NUM_3_USER);
    uint32_t error_len = strlen(error_msg);

    if (!udb_log_cdc_ready())
    {
        udb_log_push(ptr, len_to_write);
        return len_to_write;
    }

    if (total_free < error_len) {
        // drop everything so that printf() don't keep polling
        return len_to_write;
    }

    write_free = total_free - error_len;

    for (int idx = 0; idx < len_to_write; idx++)
    {
        if (write_free < 2)
        {
            // No more space to write a \r\n in the worst case
            USBD_CDC_ACM_DataSend(USB_CDC_ACM_NUM_3_USER, (uint8_t*)error_msg, error_len);
            break;
        }

        if (ptr[idx] == '\n')
        {
            USBD_CDC_ACM_DataSend(USB_CDC_ACM_NUM_3_USER, (uint8_t*)"\r", 1);
            write_free--;
        }
        USBD_CDC_ACM_DataSend(USB_CDC_ACM_NUM_3_USER, (uint8_t*)(ptr + idx), 1);
        write_free--;
    }

    return len_to_write;
}
