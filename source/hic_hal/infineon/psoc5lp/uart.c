/*******************************************************************************

* @file    uart.c
* @brief   PSoC5LP HW uart API
*
********************************************************************************
* Copyright (2019-2021) Cypress Semiconductor Corporation (an Infineon company) 
* or an affiliate of Cypress Semiconductor Corporation.
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
*****************************************************************************/

#include "uart.h"
#include "util.h"
#include "cortex_m.h"
#include "circ_buf.h"
#include "RTL.h"
#include "rl_usb.h"

#include "UART_Bridge.h"
#include "Pin_UART_Tx.h"
#include "Clock_UART.h"
#include "usbd_PSoC5LP.h"

/* UART Source Clock Frequency */
#define SOURCECLK            (64000000u)
#define SOURCECLK_IMO        (24000000u)
/* Dividers for Baud Rates */
#define DIVIDER4000000      (SOURCECLK/(UART_Bridge_OVER_SAMPLE_COUNT * 4000000u))
#define DIVIDER3000000      (SOURCECLK_IMO/(UART_Bridge_OVER_SAMPLE_COUNT * 3000000u))
#define DIVIDER2000000      (SOURCECLK/(UART_Bridge_OVER_SAMPLE_COUNT * 2000000u))
#define DIVIDER1000000      (SOURCECLK/(UART_Bridge_OVER_SAMPLE_COUNT * 1000000u))
#define DIVIDER500000       (SOURCECLK/(UART_Bridge_OVER_SAMPLE_COUNT * 500000u))
#define DIVIDER250000       (SOURCECLK/(UART_Bridge_OVER_SAMPLE_COUNT * 250000u))
#define DIVIDER115200       (SOURCECLK/(UART_Bridge_OVER_SAMPLE_COUNT * 115200u))
#define DIVIDER57600        (SOURCECLK/(UART_Bridge_OVER_SAMPLE_COUNT * 57600u))
#define DIVIDER38400        (SOURCECLK/(UART_Bridge_OVER_SAMPLE_COUNT * 38400u))
#define DIVIDER19200        (SOURCECLK/(UART_Bridge_OVER_SAMPLE_COUNT * 19200u))
#define DIVIDER9600         (SOURCECLK/(UART_Bridge_OVER_SAMPLE_COUNT * 9600u))
#define DIVIDER4800         (SOURCECLK/(UART_Bridge_OVER_SAMPLE_COUNT * 4800u))
#define DIVIDER2400         (SOURCECLK/(UART_Bridge_OVER_SAMPLE_COUNT * 2400u))
#define DIVIDER1200         (SOURCECLK/(UART_Bridge_OVER_SAMPLE_COUNT * 1200u))

/* USB IN EndPoint Packet size */
#define USBINPACKETSIZE     (64u)
#define USBCDCINBULKEP      (USBFS_EP5)

extern UART_Configuration UART_Config;

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    uint32_t wCount;

    uint8_t rxStatus = UART_Bridge_ReadRxStatus();
    /* Check status of UART_RX_STS_SOFT_BUFF_OVER */
    if ((rxStatus & UART_Bridge_RX_STS_SOFT_BUFF_OVER) != 0u)
    {
        UART_Bridge_ClearRxBuffer();
    }

    wCount = UART_Bridge_GetRxBufferSize();
    wCount = (wCount < USBINPACKETSIZE) ? wCount : USBINPACKETSIZE;

    /* Choose the lesser of the evils */
    if (size < wCount)
    {
        wCount = size;
    }

    if(wCount == 0u)
    {
        if ((rxStatus & (UART_Bridge_RX_STS_BREAK | UART_Bridge_RX_STS_PAR_ERROR | UART_Bridge_RX_STS_STOP_ERROR | UART_Bridge_RX_STS_OVERRUN)) != 0u)
        {
            /* will clear HW FIFO if HWError occured */
            for(uint32_t cnt = 0; cnt < UART_Bridge_FIFO_LENGTH; cnt++ )
            {
                (void)UART_Bridge_GetChar();
            }
        }
        else
        {
            if ((rxStatus & UART_Bridge_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* at least one byte can be read */
                wCount = 1u;
            }
        }
    }

    /* Check if Rx has data */
    if (wCount != 0u)
    {
        for (uint32_t bIndex = 0u; bIndex < wCount; bIndex++)
        {
            /* Load from UART RX Buffer to USB */
            data[bIndex] = UART_Bridge_ReadRxData();
        }
    }
    return (int32_t)wCount;
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    /* Send to UART Tx */
    UART_Bridge_PutArray(data, size);
    return (int32_t)size;
}

int32_t uart_initialize(void)
{
    uint16_t wDivider = DIVIDER115200;
    UART_Bridge_Stop();

    /* Select clock source (PLL) */
    Clock_UART_Stop();
    Clock_UART_SetSourceRegister((uint8_t)CYCLK_SRC_SEL_PLL);
    Clock_UART_Start();

    /* Set new Clock Frequency divider */
    Clock_UART_SetDivider(wDivider - 1u);

    UART_Bridge_Start();

    /* Initialize CDC Interface for USB-UART Bridge */
    Pin_UART_Tx_SetDriveMode(Pin_UART_Tx_DM_STRONG);

    UART_Config.Baudrate    = 115200u;
    UART_Config.DataBits    = UART_DATA_BITS_8;
    UART_Config.Parity      = UART_PARITY_NONE;
    UART_Config.StopBits    = UART_STOP_BITS_1;
    UART_Config.FlowControl = UART_FLOW_CONTROL_NONE;

    return 0;
}

int32_t uart_reset(void)
{
    return 0;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    static uint32_t prevBaudrate = 115200u;
    /* only the data rate can be changed */
    if ( prevBaudrate != config->Baudrate )
    {
        uint32_t dDTERate = config->Baudrate;

        /* Check for Baud Rate Upper Limit */
        if (dDTERate > 4000000u)
        {
            dDTERate = 4000000u;
        }

        /* Check for Baud Rate Lower Limit */
        if (dDTERate < 1200u)
        {
            dDTERate = 1200u;
        }

        uint16_t wDivider;

        /* Sets the required Clock divider for UART */
        switch (dDTERate)
        {
        case 4000000u:
            wDivider = DIVIDER4000000;
            break;
        case 3000000u:
            wDivider = DIVIDER3000000;
            break;
        case 2000000u:
            wDivider = DIVIDER2000000;
            break;
        case 1000000u:
            wDivider = DIVIDER1000000;
            break;
        case 921600u:
            wDivider = DIVIDER1000000;
            break;
        case 460800u:
            wDivider = DIVIDER500000;
            break;
        case 230400u:
            wDivider = DIVIDER250000;
            break;
        case 115200u:
            wDivider = DIVIDER115200;
            break;
        case 57600u:
            wDivider = DIVIDER57600;
            break;
        case 38400u:
            wDivider = DIVIDER38400;
            break;
        case 19200u:
            wDivider = DIVIDER19200;
            break;
        case 9600u:
            wDivider = DIVIDER9600;
            break;
        case 4800u:
            wDivider = DIVIDER4800;
            break;
        case 2400u:
            wDivider = DIVIDER2400;
            break;
        case 1200u:
            wDivider = DIVIDER1200;
            break;
        default:
            wDivider = DIVIDER115200;
            break;
        }

        if ((Clock_UART_GetDividerRegister() + 1u) != wDivider)
        {
            /* Stop UART for new Clock */
            UART_Bridge_Stop();
            /* Select clock source (PLL) */
            if (wDivider != DIVIDER3000000)
            {
                if (Clock_UART_GetSourceRegister() != (uint8_t)CYCLK_SRC_SEL_PLL)
                {
                    Clock_UART_Stop();
                    Clock_UART_SetSourceRegister((uint8_t)CYCLK_SRC_SEL_PLL);
                    Clock_UART_Start();
                }
            }
            else /* For 3 000 000 bps select a different clock source (IMO), it shall improve
             * baud rate accuracy */
            {
                if (Clock_UART_GetSourceRegister() != (uint8_t)CYCLK_SRC_SEL_IMO)
                {
                    Clock_UART_Stop();
                    Clock_UART_SetSourceRegister((uint8_t)CYCLK_SRC_SEL_IMO);
                    Clock_UART_Start();
                }
            }

            /* Set new Clock Frequency divider */
            Clock_UART_SetDivider(wDivider - 1u);

            /* Restart UART */

            UART_Bridge_ClearRxBuffer();
            UART_Bridge_ClearTxBuffer();

            UART_Bridge_Start();

            Pin_UART_Tx_SetDriveMode(Pin_UART_Tx_DM_STRONG);
        }
        prevBaudrate = dDTERate;
    }
    return 0;
}

int32_t uart_uninitialize(void)
{
    return 0;
}

int32_t uart_write_free(void)
{
    uint32_t size_to_write = UART_Bridge_TX_BUFFER_SIZE - (uint32_t)UART_Bridge_GetTxBufferSize();
    return (int32_t)size_to_write;
}
