/* ****************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _MXC_UART_REGS_H_
#define _MXC_UART_REGS_H_

/* **** Includes **** */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

///@cond
/*
    If types are not defined elsewhere (CMSIS) define them here
*/
#ifndef __IO
#define __IO volatile
#endif
#ifndef __I
#define __I  volatile const
#endif
#ifndef __O
#define __O  volatile
#endif
#ifndef __R
#define __R  volatile const
#endif
///@endcond

/**
 * @defgroup    uart_registers UART Registers
 * @brief       Registers, Bit Masks and Bit Positions
 * @ingroup     uart_comm
 * @{
 */

/**
 * Structure type for the UART peripheral registers allowing direct 32-bit access to each register.
 */
typedef struct {
    __IO uint32_t ctrl;                                 /**< <tt>\b 0x0000:</tt> UART_CTRL Register - UART Control Register.                    */
    __IO uint32_t baud;                                 /**< <tt>\b 0x0004:</tt> UART_BAUD Register - UART Baud Control Register.               */
    __IO uint32_t tx_fifo_ctrl;                         /**< <tt>\b 0x0008:</tt> UART_TX_FIFO_CTRL Register - UART TX FIFO Control Register.    */
    __IO uint32_t rx_fifo_ctrl;                         /**< <tt>\b 0x000C:</tt> UART_RX_FIFO_CTRL Register - UART RX FIFO Control Register.    */
    __IO uint32_t md_ctrl;                              /**< <tt>\b 0x0010:</tt> UART_MD_CTRL Register - UART Multidrop Control Register.       */
    __IO uint32_t intfl;                                /**< <tt>\b 0x0014:</tt> UART_INTFL Register - UART Interrupt Flags.                    */
    __IO uint32_t inten;                                /**< <tt>\b 0x0018:</tt> UART_INTEN Register - UART Interrupt Enable/Disable Control.   */
#if (MXC_UART_REV > 0)
    __R  uint32_t idle;                                 /**< <tt>\b 0x001C:</tt> UART_IDLE Register - UART Idle Status                          */
#endif
} mxc_uart_regs_t;
/**@} end of group uart_registers */

/**
 * @defgroup uart_fifos UART TX and RX FIFOs
 * @brief TX and RX FIFO access for reads and writes using 8-bit, 16-bit and 32-bit data types.
 * @ingroup uart_registers
 * @{
 */
/**
 * Structure type for accessing the UART Transmit and Receive FIFOs.
 */
typedef struct {
    union {
        __IO uint8_t  tx;                               /**< TX FIFO write point for data to transmit.                                          */
        __IO uint8_t  tx_8[2048];                       /**< 8-bit access to TX FIFO.                                                           */
        __IO uint16_t tx_16[1024];                      /**< 16-bit access to TX FIFO.                                                          */
        __IO uint32_t tx_32[512];                       /**< 32-bit access to TX FIFO.                                                          */
    };
    union {
        __IO uint8_t  rx;                               /**< RX FIFO read point for received data.                                              */
        __IO uint8_t  rx_8[2048];                       /**< 8-bit access to RX FIFO.                                                           */
        __IO uint16_t rx_16[1024];                      /**< 16-bit access to RX FIFO.                                                          */
        __IO uint32_t rx_32[512];                       /**< 32-bit access to RX FIFO.                                                          */
    };
} mxc_uart_fifo_regs_t;
/**@} end of group uart_fifos */

/*
   Register offsets for module UART.
*/

#define MXC_R_UART_OFFS_CTRL                                ((uint32_t)0x00000000UL)
#define MXC_R_UART_OFFS_BAUD                                ((uint32_t)0x00000004UL)
#define MXC_R_UART_OFFS_TX_FIFO_CTRL                        ((uint32_t)0x00000008UL)
#define MXC_R_UART_OFFS_RX_FIFO_CTRL                        ((uint32_t)0x0000000CUL)
#define MXC_R_UART_OFFS_MD_CTRL                             ((uint32_t)0x00000010UL)
#define MXC_R_UART_OFFS_INTFL                               ((uint32_t)0x00000014UL)
#define MXC_R_UART_OFFS_INTEN                               ((uint32_t)0x00000018UL)
#define MXC_R_UART_FIFO_OFFS_TX                             ((uint32_t)0x00000000UL)
#define MXC_R_UART_FIFO_OFFS_RX                             ((uint32_t)0x00000800UL)


/*
   Field positions and masks for module UART.
*/

#define MXC_F_UART_CTRL_UART_EN_POS                         0
#define MXC_F_UART_CTRL_UART_EN                             ((uint32_t)(0x00000001UL << MXC_F_UART_CTRL_UART_EN_POS))
#define MXC_F_UART_CTRL_RX_FIFO_EN_POS                      1
#define MXC_F_UART_CTRL_RX_FIFO_EN                          ((uint32_t)(0x00000001UL << MXC_F_UART_CTRL_RX_FIFO_EN_POS))
#define MXC_F_UART_CTRL_TX_FIFO_EN_POS                      2
#define MXC_F_UART_CTRL_TX_FIFO_EN                          ((uint32_t)(0x00000001UL << MXC_F_UART_CTRL_TX_FIFO_EN_POS))
#define MXC_F_UART_CTRL_DATA_SIZE_POS                       4
#define MXC_F_UART_CTRL_DATA_SIZE                           ((uint32_t)(0x00000003UL << MXC_F_UART_CTRL_DATA_SIZE_POS))
#define MXC_F_UART_CTRL_EXTRA_STOP_POS                      8
#define MXC_F_UART_CTRL_EXTRA_STOP                          ((uint32_t)(0x00000001UL << MXC_F_UART_CTRL_EXTRA_STOP_POS))
#define MXC_F_UART_CTRL_PARITY_POS                          12
#define MXC_F_UART_CTRL_PARITY                              ((uint32_t)(0x00000003UL << MXC_F_UART_CTRL_PARITY_POS))
#define MXC_F_UART_CTRL_CTS_EN_POS                          16
#define MXC_F_UART_CTRL_CTS_EN                              ((uint32_t)(0x00000001UL << MXC_F_UART_CTRL_CTS_EN_POS))
#define MXC_F_UART_CTRL_CTS_POLARITY_POS                    17
#define MXC_F_UART_CTRL_CTS_POLARITY                        ((uint32_t)(0x00000001UL << MXC_F_UART_CTRL_CTS_POLARITY_POS))
#define MXC_F_UART_CTRL_RTS_EN_POS                          18
#define MXC_F_UART_CTRL_RTS_EN                              ((uint32_t)(0x00000001UL << MXC_F_UART_CTRL_RTS_EN_POS))
#define MXC_F_UART_CTRL_RTS_POLARITY_POS                    19
#define MXC_F_UART_CTRL_RTS_POLARITY                        ((uint32_t)(0x00000001UL << MXC_F_UART_CTRL_RTS_POLARITY_POS))
#define MXC_F_UART_CTRL_RTS_LEVEL_POS                       20
#define MXC_F_UART_CTRL_RTS_LEVEL                           ((uint32_t)(0x0000003FUL << MXC_F_UART_CTRL_RTS_LEVEL_POS))

#define MXC_F_UART_BAUD_BAUD_DIVISOR_POS                    0
#define MXC_F_UART_BAUD_BAUD_DIVISOR                        ((uint32_t)(0x000000FFUL << MXC_F_UART_BAUD_BAUD_DIVISOR_POS))
#define MXC_F_UART_BAUD_BAUD_MODE_POS                       8
#define MXC_F_UART_BAUD_BAUD_MODE                           ((uint32_t)(0x00000003UL << MXC_F_UART_BAUD_BAUD_MODE_POS))

#define MXC_F_UART_TX_FIFO_CTRL_FIFO_ENTRY_POS              0
#define MXC_F_UART_TX_FIFO_CTRL_FIFO_ENTRY                  ((uint32_t)(0x0000001FUL << MXC_F_UART_TX_FIFO_CTRL_FIFO_ENTRY_POS))
#define MXC_F_UART_TX_FIFO_CTRL_FIFO_AE_LVL_POS             16
#define MXC_F_UART_TX_FIFO_CTRL_FIFO_AE_LVL                 ((uint32_t)(0x0000001FUL << MXC_F_UART_TX_FIFO_CTRL_FIFO_AE_LVL_POS))

#define MXC_F_UART_RX_FIFO_CTRL_FIFO_ENTRY_POS              0
#define MXC_F_UART_RX_FIFO_CTRL_FIFO_ENTRY                  ((uint32_t)(0x0000001FUL << MXC_F_UART_RX_FIFO_CTRL_FIFO_ENTRY_POS))
#define MXC_F_UART_RX_FIFO_CTRL_FIFO_AF_LVL_POS             16
#define MXC_F_UART_RX_FIFO_CTRL_FIFO_AF_LVL                 ((uint32_t)(0x0000001FUL << MXC_F_UART_RX_FIFO_CTRL_FIFO_AF_LVL_POS))

#define MXC_F_UART_MD_CTRL_SLAVE_ADDR_POS                   0
#define MXC_F_UART_MD_CTRL_SLAVE_ADDR                       ((uint32_t)(0x000000FFUL << MXC_F_UART_MD_CTRL_SLAVE_ADDR_POS))
#define MXC_F_UART_MD_CTRL_SLAVE_ADDR_MSK_POS               8
#define MXC_F_UART_MD_CTRL_SLAVE_ADDR_MSK                   ((uint32_t)(0x000000FFUL << MXC_F_UART_MD_CTRL_SLAVE_ADDR_MSK_POS))
#define MXC_F_UART_MD_CTRL_MD_MSTR_POS                      16
#define MXC_F_UART_MD_CTRL_MD_MSTR                          ((uint32_t)(0x00000001UL << MXC_F_UART_MD_CTRL_MD_MSTR_POS))
#define MXC_F_UART_MD_CTRL_TX_ADDR_MARK_POS                 17
#define MXC_F_UART_MD_CTRL_TX_ADDR_MARK                     ((uint32_t)(0x00000001UL << MXC_F_UART_MD_CTRL_TX_ADDR_MARK_POS))

/**
 * @defgroup UART_INTFL_Register UART_INTFL
 * @ingroup uart_registers
 * @{
 */
#define MXC_F_UART_INTFL_TX_DONE_POS                        0
#define MXC_F_UART_INTFL_TX_DONE                            ((uint32_t)(0x00000001UL << MXC_F_UART_INTFL_TX_DONE_POS))
#define MXC_F_UART_INTFL_TX_UNSTALLED_POS                   1
#define MXC_F_UART_INTFL_TX_UNSTALLED                       ((uint32_t)(0x00000001UL << MXC_F_UART_INTFL_TX_UNSTALLED_POS))
#define MXC_F_UART_INTFL_TX_FIFO_AE_POS                     2
#define MXC_F_UART_INTFL_TX_FIFO_AE                         ((uint32_t)(0x00000001UL << MXC_F_UART_INTFL_TX_FIFO_AE_POS))
#define MXC_F_UART_INTFL_RX_FIFO_NOT_EMPTY_POS              3
#define MXC_F_UART_INTFL_RX_FIFO_NOT_EMPTY                  ((uint32_t)(0x00000001UL << MXC_F_UART_INTFL_RX_FIFO_NOT_EMPTY_POS))
#define MXC_F_UART_INTFL_RX_STALLED_POS                     4
#define MXC_F_UART_INTFL_RX_STALLED                         ((uint32_t)(0x00000001UL << MXC_F_UART_INTFL_RX_STALLED_POS))
#define MXC_F_UART_INTFL_RX_FIFO_AF_POS                     5
#define MXC_F_UART_INTFL_RX_FIFO_AF                         ((uint32_t)(0x00000001UL << MXC_F_UART_INTFL_RX_FIFO_AF_POS))
#define MXC_F_UART_INTFL_RX_FIFO_OVERFLOW_POS               6
#define MXC_F_UART_INTFL_RX_FIFO_OVERFLOW                   ((uint32_t)(0x00000001UL << MXC_F_UART_INTFL_RX_FIFO_OVERFLOW_POS))
#define MXC_F_UART_INTFL_RX_FRAMING_ERR_POS                 7
#define MXC_F_UART_INTFL_RX_FRAMING_ERR                     ((uint32_t)(0x00000001UL << MXC_F_UART_INTFL_RX_FRAMING_ERR_POS))
#define MXC_F_UART_INTFL_RX_PARITY_ERR_POS                  8
#define MXC_F_UART_INTFL_RX_PARITY_ERR                      ((uint32_t)(0x00000001UL << MXC_F_UART_INTFL_RX_PARITY_ERR_POS))
/**@} end of group UART_INTFL_Register */

#define MXC_F_UART_INTEN_TX_DONE_POS                        0
#define MXC_F_UART_INTEN_TX_DONE                            ((uint32_t)(0x00000001UL << MXC_F_UART_INTEN_TX_DONE_POS))
#define MXC_F_UART_INTEN_TX_UNSTALLED_POS                   1
#define MXC_F_UART_INTEN_TX_UNSTALLED                       ((uint32_t)(0x00000001UL << MXC_F_UART_INTEN_TX_UNSTALLED_POS))
#define MXC_F_UART_INTEN_TX_FIFO_AE_POS                     2
#define MXC_F_UART_INTEN_TX_FIFO_AE                         ((uint32_t)(0x00000001UL << MXC_F_UART_INTEN_TX_FIFO_AE_POS))
#define MXC_F_UART_INTEN_RX_FIFO_NOT_EMPTY_POS              3
#define MXC_F_UART_INTEN_RX_FIFO_NOT_EMPTY                  ((uint32_t)(0x00000001UL << MXC_F_UART_INTEN_RX_FIFO_NOT_EMPTY_POS))
#define MXC_F_UART_INTEN_RX_STALLED_POS                     4
#define MXC_F_UART_INTEN_RX_STALLED                         ((uint32_t)(0x00000001UL << MXC_F_UART_INTEN_RX_STALLED_POS))
#define MXC_F_UART_INTEN_RX_FIFO_AF_POS                     5
#define MXC_F_UART_INTEN_RX_FIFO_AF                         ((uint32_t)(0x00000001UL << MXC_F_UART_INTEN_RX_FIFO_AF_POS))
#define MXC_F_UART_INTEN_RX_FIFO_OVERFLOW_POS               6
#define MXC_F_UART_INTEN_RX_FIFO_OVERFLOW                   ((uint32_t)(0x00000001UL << MXC_F_UART_INTEN_RX_FIFO_OVERFLOW_POS))
#define MXC_F_UART_INTEN_RX_FRAMING_ERR_POS                 7
#define MXC_F_UART_INTEN_RX_FRAMING_ERR                     ((uint32_t)(0x00000001UL << MXC_F_UART_INTEN_RX_FRAMING_ERR_POS))
#define MXC_F_UART_INTEN_RX_PARITY_ERR_POS                  8
#define MXC_F_UART_INTEN_RX_PARITY_ERR                      ((uint32_t)(0x00000001UL << MXC_F_UART_INTEN_RX_PARITY_ERR_POS))

#if (MXC_UART_REV > 0)
#define MXC_F_UART_IDLE_TX_RX_IDLE_POS                      0
#define MXC_F_UART_IDLE_TX_RX_IDLE                          ((uint32_t)(0x00000001UL << MXC_F_UART_IDLE_TX_RX_IDLE_POS))
#define MXC_F_UART_IDLE_TX_IDLE_POS                         1
#define MXC_F_UART_IDLE_TX_IDLE                             ((uint32_t)(0x00000001UL << MXC_F_UART_IDLE_TX_IDLE_POS))
#define MXC_F_UART_IDLE_RX_IDLE_POS                         2
#define MXC_F_UART_IDLE_RX_IDLE                             ((uint32_t)(0x00000001UL << MXC_F_UART_IDLE_RX_IDLE_POS))
#endif

/*
   Field values and shifted values for module UART.
*/

#define MXC_V_UART_CTRL_DATA_SIZE_5_BITS                                        ((uint32_t)(0x00000000UL))
#define MXC_V_UART_CTRL_DATA_SIZE_6_BITS                                        ((uint32_t)(0x00000001UL))
#define MXC_V_UART_CTRL_DATA_SIZE_7_BITS                                        ((uint32_t)(0x00000002UL))
#define MXC_V_UART_CTRL_DATA_SIZE_8_BITS                                        ((uint32_t)(0x00000003UL))

#define MXC_S_UART_CTRL_DATA_SIZE_5_BITS                                        ((uint32_t)(MXC_V_UART_CTRL_DATA_SIZE_5_BITS   << MXC_F_UART_CTRL_DATA_SIZE_POS))
#define MXC_S_UART_CTRL_DATA_SIZE_6_BITS                                        ((uint32_t)(MXC_V_UART_CTRL_DATA_SIZE_6_BITS   << MXC_F_UART_CTRL_DATA_SIZE_POS))
#define MXC_S_UART_CTRL_DATA_SIZE_7_BITS                                        ((uint32_t)(MXC_V_UART_CTRL_DATA_SIZE_7_BITS   << MXC_F_UART_CTRL_DATA_SIZE_POS))
#define MXC_S_UART_CTRL_DATA_SIZE_8_BITS                                        ((uint32_t)(MXC_V_UART_CTRL_DATA_SIZE_8_BITS   << MXC_F_UART_CTRL_DATA_SIZE_POS))

#define MXC_V_UART_CTRL_PARITY_DISABLE                                          ((uint32_t)(0x00000000UL))
#define MXC_V_UART_CTRL_PARITY_ODD                                              ((uint32_t)(0x00000001UL))
#define MXC_V_UART_CTRL_PARITY_EVEN                                             ((uint32_t)(0x00000002UL))
#define MXC_V_UART_CTRL_PARITY_MARK                                             ((uint32_t)(0x00000003UL))

#define MXC_S_UART_CTRL_PARITY_DISABLE                                          ((uint32_t)(MXC_V_UART_CTRL_PARITY_DISABLE  << MXC_F_UART_CTRL_PARITY_POS))
#define MXC_S_UART_CTRL_PARITY_ODD                                              ((uint32_t)(MXC_V_UART_CTRL_PARITY_ODD      << MXC_F_UART_CTRL_PARITY_POS))
#define MXC_S_UART_CTRL_PARITY_EVEN                                             ((uint32_t)(MXC_V_UART_CTRL_PARITY_EVEN     << MXC_F_UART_CTRL_PARITY_POS))
#define MXC_S_UART_CTRL_PARITY_MARK                                             ((uint32_t)(MXC_V_UART_CTRL_PARITY_MARK     << MXC_F_UART_CTRL_PARITY_POS))

#ifdef __cplusplus
}
#endif

#endif   /* _MXC_UART_REGS_H_ */
