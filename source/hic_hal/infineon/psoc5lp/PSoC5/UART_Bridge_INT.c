/*******************************************************************************
* File Name: UART_BridgeINT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
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

#include "UART_Bridge.h"



/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (UART_Bridge_RX_INTERRUPT_ENABLED && (UART_Bridge_RX_ENABLED || UART_Bridge_HD_ENABLED))
    /*******************************************************************************
    * Function Name: UART_Bridge_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_Bridge_rxBuffer - RAM buffer pointer for save received data.
    *  UART_Bridge_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  UART_Bridge_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  UART_Bridge_rxBufferOverflow - software overflow flag. Set to one
    *     when UART_Bridge_rxBufferWrite index overtakes
    *     UART_Bridge_rxBufferRead index.
    *  UART_Bridge_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when UART_Bridge_rxBufferWrite is equal to
    *    UART_Bridge_rxBufferRead
    *  UART_Bridge_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  UART_Bridge_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(UART_Bridge_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef UART_Bridge_RXISR_ENTRY_CALLBACK
        UART_Bridge_RXISR_EntryCallback();
    #endif /* UART_Bridge_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START UART_Bridge_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = UART_Bridge_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support
            *  of the user code in UART_Bridge_RXISR_ERROR` section.
            */
            readData = readStatus;

            if((readStatus & (UART_Bridge_RX_STS_BREAK |
                            UART_Bridge_RX_STS_PAR_ERROR |
                            UART_Bridge_RX_STS_STOP_ERROR |
                            UART_Bridge_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                UART_Bridge_errorStatus |= readStatus & ( UART_Bridge_RX_STS_BREAK |
                                                            UART_Bridge_RX_STS_PAR_ERROR |
                                                            UART_Bridge_RX_STS_STOP_ERROR |
                                                            UART_Bridge_RX_STS_OVERRUN);
                /* `#START UART_Bridge_RXISR_ERROR` */

                /* `#END` */

            #ifdef UART_Bridge_RXISR_ERROR_CALLBACK
                UART_Bridge_RXISR_ERROR_Callback();
            #endif /* UART_Bridge_RXISR_ERROR_CALLBACK */
            }

            if((readStatus & UART_Bridge_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = UART_Bridge_RXDATA_REG;
            #if (UART_Bridge_RXHW_ADDRESS_ENABLED)
                if(UART_Bridge_rxAddressMode == (uint8)UART_Bridge__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & UART_Bridge_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & UART_Bridge_RX_STS_ADDR_MATCH) != 0u)
                        {
                            UART_Bridge_rxAddressDetected = 1u;
                        }
                        else
                        {
                            UART_Bridge_rxAddressDetected = 0u;
                        }
                    }
                    if(UART_Bridge_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        UART_Bridge_rxBuffer[UART_Bridge_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    UART_Bridge_rxBuffer[UART_Bridge_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                UART_Bridge_rxBuffer[UART_Bridge_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (UART_Bridge_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(UART_Bridge_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        UART_Bridge_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    UART_Bridge_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(UART_Bridge_rxBufferWrite >= UART_Bridge_RX_BUFFER_SIZE)
                    {
                        UART_Bridge_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(UART_Bridge_rxBufferWrite == UART_Bridge_rxBufferRead)
                    {
                        UART_Bridge_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (UART_Bridge_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            UART_Bridge_RXSTATUS_MASK_REG  &= (uint8)~UART_Bridge_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(UART_Bridge_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (UART_Bridge_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & UART_Bridge_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START UART_Bridge_RXISR_END` */

        /* `#END` */

    #ifdef UART_Bridge_RXISR_EXIT_CALLBACK
        UART_Bridge_RXISR_ExitCallback();
    #endif /* UART_Bridge_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }

#endif /* (UART_Bridge_RX_INTERRUPT_ENABLED && (UART_Bridge_RX_ENABLED || UART_Bridge_HD_ENABLED)) */


#if (UART_Bridge_TX_INTERRUPT_ENABLED && UART_Bridge_TX_ENABLED)
    /*******************************************************************************
    * Function Name: UART_Bridge_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_Bridge_txBuffer - RAM buffer pointer for transmit data from.
    *  UART_Bridge_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  UART_Bridge_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(UART_Bridge_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef UART_Bridge_TXISR_ENTRY_CALLBACK
        UART_Bridge_TXISR_EntryCallback();
    #endif /* UART_Bridge_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START UART_Bridge_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((UART_Bridge_txBufferRead != UART_Bridge_txBufferWrite) &&
             ((UART_Bridge_TXSTATUS_REG & UART_Bridge_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(UART_Bridge_txBufferRead >= UART_Bridge_TX_BUFFER_SIZE)
            {
                UART_Bridge_txBufferRead = 0u;
            }

            UART_Bridge_TXDATA_REG = UART_Bridge_txBuffer[UART_Bridge_txBufferRead];

            /* Set next pointer */
            UART_Bridge_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START UART_Bridge_TXISR_END` */

        /* `#END` */

    #ifdef UART_Bridge_TXISR_EXIT_CALLBACK
        UART_Bridge_TXISR_ExitCallback();
    #endif /* UART_Bridge_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (UART_Bridge_TX_INTERRUPT_ENABLED && UART_Bridge_TX_ENABLED) */


/* [] END OF FILE */
