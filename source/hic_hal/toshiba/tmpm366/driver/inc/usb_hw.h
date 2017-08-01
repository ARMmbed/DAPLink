/**
 ****************************************************************************
 * @file    usbd_hw.h
 * @brief   This file provides all the functions prototypes for USBD hardware driver.
 * @version V2.0.2.3
 * @date    2014/05/20
 *
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
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
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_HW_H
#define __USB_HW_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "TMPM366.h"
#include "tx03_common.h"

#if defined(__USB_HW_H)
/** @addtogroup TX03_USBD_Driver
  * @{
  */

/** @addtogroup USBD
  * @{
  */

/** @addtogroup USBD_Exported_types
  * @{
  */


/**
  * @brief USBD Interrupt status union definition
  */
    typedef union {
        uint32_t All;
        struct {
            uint32_t Setup:1;   /*!< <0> RO             */
            uint32_t Status_NAK:1;      /*!< <1> RO     */
            uint32_t Status:1;  /*!< <2> RO             */
            uint32_t Rx_Zero:1; /*!< <3> RO             */
            uint32_t SOF:1;     /*!< <4> RO             */
            uint32_t EP0:1;     /*!< <5> RO             */
            uint32_t EP:1;      /*!< <6> RO             */
            uint32_t NAK:1;     /*!< <7> RO             */
            uint32_t Suspend_Resume:1;  /*!< <8> RW1C   */
            uint32_t USB_Reset:1;       /*!< <9> RW1C   */
            uint32_t USB_Reset_End:1;   /*!< <10> RW1C  */
            uint32_t Reserved1:6;       /*!< <16:11>    */
            uint32_t MW_Set_Add:1;      /*!< <17> RW1C  1: Master Write transfer address request */
            uint32_t MW_End_Add:1;      /*!< <18> RW1C  1: Master Write transfer finished */
            uint32_t MW_TimeOut:1;      /*!< <19> RW1C  */
            uint32_t MW_AHBErr:1;       /*!< <20> RW1C  */
            uint32_t MR_End_Add:1;      /*!< <21> RW1C  1: Master Read transfer finished */
            uint32_t MR_EP_DSet:1;      /*!< <22> RW1C  1: FIFO of EP for UDC2 Tx is writable */
            uint32_t MR_AHBErr:1;       /*!< <23> RW1C  */
            uint32_t UDC2_Reg_Read:1;   /*!< <24> RW1C  */
            uint32_t DMAC_Reg_Read:1;   /*!< <25> RW1C  */
            uint32_t Reserved2:3;       /*!< <28:26>    */
            uint32_t MW_ReadError:1;    /*!< <29> RW1C  1: Endpoint read error occurred in Master Write */
            uint32_t Reserved3:2;       /*!< <31:30>    */
        } Bit;
    } USBD_INTStatus;

/**
  * @brief USBD Interrupt Source (UDC2 and UDC2AB) Enum
  */
    typedef enum {
        USBD_INT_SETUP = 0U,
        USBD_INT_STATUS_NAK = 1U,
        USBD_INT_STATUS = 2U,
        USBD_INT_RX_ZERO = 3U,
        USBD_INT_SOF = 4U,
        USBD_INT_EP0 = 5U,
        USBD_INT_EP = 6U,
        USBD_INT_NAK = 7U,
        USBD_INT_SUSPEND_RESUME = 8U,
        USBD_INT_USB_RESET = 9U,
        USBD_INT_USB_RESET_END = 10U,
        USBD_INT_MW_SET_ADD = 17U,
        USBD_INT_MW_END_ADD = 18U,
        USBD_INT_MW_TIMEOUT = 19U,
        USBD_INT_MW_AHBERR = 20U,
        USBD_INT_MR_END_ADD = 21U,
        USBD_INT_MR_EP_DSET = 22U,
        USBD_INT_MR_AHBERR = 23U,
        USBD_INT_UDC2_REG_READ = 24U,
        USBD_INT_DMAC_REG_READ = 25U,
        USBD_INT_MW_READERROR = 29U
    } USBD_INTSrc;
#define IS_USBD_UDC2AB_INT(param)      ((((param) >= USBD_INT_SUSPEND_RESUME) && \
                                         ((param) <= USBD_INT_USB_RESET_END))   || \
                                        (((param) >= USBD_INT_MW_SET_ADD)     && \
                                         ((param) <= USBD_INT_DMAC_REG_READ))   || \
                                        ((param) == USBD_INT_MW_READERROR))


/**
  * @brief USBD DMAC Setting union definition
  */
    typedef union {
        uint32_t All;
        struct {
            uint32_t MW_Enable:1;       /*!< <0> RW1S   */
            uint32_t MW_Abort:1;        /*!< <1> WO     */
            uint32_t MW_Reset:1;        /*!< <2> RW1S   */
            uint32_t Reserved1:1;       /*!< <3> W0C    */
            uint32_t MR_Enable:1;       /*!< <4> RW1S   */
            uint32_t MR_Abort:1;        /*!< <5> WO     */
            uint32_t MR_Reset:1;        /*!< <6> RW1S   */
            uint32_t Reserved2:1;       /*!< <7> W0C    */
            uint32_t M_Burst_Type:1;    /*!< <8> RW     */
            uint32_t Reserved3:23;      /*!< <31:9>W0C  */
        } Bit;
    } USBD_DMACConfig;


/**
  * @brief USBD DMAC Master Status union definition
  */
    typedef union {
        uint32_t All;
        struct {
            uint32_t MW_EP_DSet:1;      /*!< <0> RO, mwepdset   */
            uint32_t MR_EP_DSet:1;      /*!< <1> RO, mrepdset   */
            uint32_t MW_Buf_Empty:1;    /*!< <2> RO, mwbfemp    */
            uint32_t MR_Buf_Empty:1;    /*!< <3> RO, mrbfemp    */
            uint32_t MR_EP_Empty:1;     /*!< <4> RO, mrepempty  */
            uint32_t Reserved:27;       /*!< <31:5>             */
        } Bit;
    } USBD_DMACStatus;

/**
  * @brief USBD Master Read/Write Action Enum
  */
    typedef enum {
        USBD_MASTER_WRITE = 0U,
        USBD_MASTER_READ = 1U
    } USBD_MasterMode;
#define IS_USBD_MASTERMODE(param)   (((param) <= USBD_MASTER_READ))

/**
  * @brief USBD Master Read/Write Address structure
  */
    typedef struct {
        uint32_t StartAddr;
        uint32_t EndAddr;
    } USBD_DMACAddr;

/**
  * @brief USBD Power Detect Control union definition
  */
    typedef union {
        uint32_t All;
        struct {
            uint32_t USB_Reset:1;       /*!< <0> RO  */
            uint32_t PW_Resetb:1;       /*!< <1> RW  */
            uint32_t PW_Detect:1;       /*!< <2> RO  */
            uint32_t PHY_Suspend:1;     /*!< <3> RW  */
            uint32_t Suspend_x:1;       /*!< <4> RO  */
            uint32_t PHY_Resetb:1;      /*!< <5> RW  */
            uint32_t PHY_Remote_Wakeup:1;       /*!< <6> RW1S */
            uint32_t Wakeup_En:1;       /*!< <7> RW      */
            uint32_t Reserved:24;       /*!< <31:8>      */
        } Bit;
    } USBD_PowerCtrl;

/**
  * @brief USBD UDC2 Endpoint 0 status union definition
  */
    typedef union {
        uint32_t All;
        struct {
            uint32_t Reserved1:9;       /* <8:0>   -    */
            uint32_t Status:3;          /* <11:9>  RO   */
            uint32_t Toggle:2;          /* <13:12> RO   */
            uint32_t Reserved2:1;       /* <14>    -    */
            uint32_t EP0_Mask:1;        /* <15>    RO   */
            uint32_t Reserved3:16;      /* <31:16> -    */
        } Bit;
    } USBD_EP0Status;

/**
  * @brief USBD UDC2 Endpoint x (x = 1 to 7) status union definition
  */
    typedef union {
        uint32_t All;
        struct {
            uint32_t Num_MF:2;  /* <1:0>   RW   */
            uint32_t T_Type:2;  /* <3:2>   RW   */
            uint32_t Reserved1:3;       /* <6:4>   -    */
            uint32_t Dir:1;     /* <7>     RW   */
            uint32_t Disable:1; /* <8>     RO   */
            uint32_t Status:3;  /* <11:9>  RO   */
            uint32_t Toggle:2;  /* <13:12> RO   */
            uint32_t Bus_Sel:1; /* <14>    RW   */
            uint32_t Pkt_Mode:1;        /* <15>    RW   */
            uint32_t Reserved2:16;      /* <31:16> -    */
        } Bit;
    } USBD_EPxStatus;

#define USBD_EPxConfig  USBD_EPxStatus

/**
  * @brief USBD Endpoint number Enum
  */
    typedef enum {
        USBD_EP0 = 0U,
        USBD_EP1 = 1U,
        USBD_EP2 = 2U,
        USBD_EP3 = 3U,
        USBD_EP4 = 4U,
        USBD_EP5 = 5U,
        USBD_EP6 = 6U,
        USBD_EP7 = 7U
    } USBD_EPx;
#define IS_USBD_EPxALL(param)      ((param) <= USBD_EP7)
#define IS_USBD_NOT_EP0(param)     (((param) >= USBD_EP1) && ((param) <= USBD_EP7))

/**
  * @brief USBD Endpoint command Enum
  */
    typedef enum {
        USBD_CMD_SETUP_FIN = 1U,
        USBD_CMD_SET_DATA0 = 2U,
        USBD_CMD_EP_RESET = 3U,
        USBD_CMD_EP_STALL = 4U,
        USBD_CMD_EP_INVALID = 5U,
        USBD_CMD_EP_DISABLE = 7U,
        USBD_CMD_EP_ENABLE = 8U,
        USBD_CMD_ALL_EP_INVALID = 9U,
        USBD_CMD_USB_READY = 10U,
        USBD_CMD_SETUP_RECEIVED = 11U,
        USBD_CMD_EP_EOP = 12U,
        USBD_CMD_EP_FIFO_CLEAR = 13U,
        USBD_CMD_EP_TX_0DATA = 14U
    } USBD_EPCMD;

/* valid command for EP0 */
#define IS_USBD_EP0CMD(param)  (((param) == USBD_CMD_SETUP_FIN) || \
                                ((param) == USBD_CMD_EP_RESET)  || \
                                ((param) == USBD_CMD_EP_STALL)  || \
                                ((param) == USBD_CMD_ALL_EP_INVALID) || \
                                ((param) == USBD_CMD_USB_READY) || \
                                ((param) == USBD_CMD_SETUP_RECEIVED) || \
                                ((param) == USBD_CMD_EP_EOP) || \
                                ((param) == USBD_CMD_EP_FIFO_CLEAR) || \
                                ((param) == USBD_CMD_EP_TX_0DATA))

/* valid command for EPx(x = 1 to 7 ) */
#define IS_USBD_EPxCMD(param)  (((param) == USBD_CMD_SET_DATA0) || \
                                ((param) == USBD_CMD_EP_RESET)  || \
                                ((param) == USBD_CMD_EP_STALL)  || \
                                ((param) == USBD_CMD_EP_INVALID)|| \
                                ((param) == USBD_CMD_EP_DISABLE)|| \
                                ((param) == USBD_CMD_EP_ENABLE) || \
                                ((param) == USBD_CMD_ALL_EP_INVALID) || \
                                ((param) == USBD_CMD_EP_EOP) || \
                                ((param) == USBD_CMD_EP_FIFO_CLEAR) || \
                                ((param) == USBD_CMD_EP_TX_0DATA))


/* rename the register in UDC2 to be parameters for USBD_ReadUDC2Reg() and USBD_WriteUDC2Reg() */
/* TSB_UDFS2_BASE  == 0x40008200UL  */
#define UDC2_ADDR               (uint32_t)(TSB_UDFS2_BASE + 0x00U)
#define UDC2_FRAME              (uint32_t)(TSB_UDFS2_BASE + 0x04U)
#define UDC2_RESERVED0          (uint32_t)(TSB_UDFS2_BASE + 0x08U)
#define UDC2_COMMAND            (uint32_t)(TSB_UDFS2_BASE + 0x0CU)
#define UDC2_BREQ_BMREQTYPE     (uint32_t)(TSB_UDFS2_BASE + 0x10U)      /* bRequest-bmRequesetType */
#define UDC2_WVALUE             (uint32_t)(TSB_UDFS2_BASE + 0x14U)
#define UDC2_WINDEX             (uint32_t)(TSB_UDFS2_BASE + 0x18U)
#define UDC2_WLENGTH            (uint32_t)(TSB_UDFS2_BASE + 0x1CU)
#define UDC2_INT                (uint32_t)(TSB_UDFS2_BASE + 0x20U)
#define UDC2_INTEP              (uint32_t)(TSB_UDFS2_BASE + 0x24U)
#define UDC2_INTEP_MASK         (uint32_t)(TSB_UDFS2_BASE + 0x28U)
#define UDC2_INTRX0             (uint32_t)(TSB_UDFS2_BASE + 0x2CU)
#define UDC2_EP0MAXPACKETSIZE   (uint32_t)(TSB_UDFS2_BASE + 0x30U)
#define UDC2_EP0STATUS          (uint32_t)(TSB_UDFS2_BASE + 0x34U)
#define UDC2_EP0DATASIZE        (uint32_t)(TSB_UDFS2_BASE + 0x38U)
#define UDC2_EP0FIFO            (uint32_t)(TSB_UDFS2_BASE + 0x3CU)
#define UDC2_EP1MAXPACKETSIZE   (uint32_t)(TSB_UDFS2_BASE + 0x40U)
#define UDC2_EP1STATUS          (uint32_t)(TSB_UDFS2_BASE + 0x44U)
#define UDC2_EP1DATASIZE        (uint32_t)(TSB_UDFS2_BASE + 0x48U)
#define UDC2_EP1FIFO            (uint32_t)(TSB_UDFS2_BASE + 0x4CU)
#define UDC2_EP2MAXPACKETSIZE   (uint32_t)(TSB_UDFS2_BASE + 0x50U)
#define UDC2_EP2STATUS          (uint32_t)(TSB_UDFS2_BASE + 0x54U)
#define UDC2_EP2DATASIZE        (uint32_t)(TSB_UDFS2_BASE + 0x58U)
#define UDC2_EP2FIFO            (uint32_t)(TSB_UDFS2_BASE + 0x5CU)
#define UDC2_EP3MAXPACKETSIZE   (uint32_t)(TSB_UDFS2_BASE + 0x60U)
#define UDC2_EP3STATUS          (uint32_t)(TSB_UDFS2_BASE + 0x64U)
#define UDC2_EP3DATASIZE        (uint32_t)(TSB_UDFS2_BASE + 0x68U)
#define UDC2_EP3FIFO            (uint32_t)(TSB_UDFS2_BASE + 0x6CU)
#define UDC2_EP4MAXPACKETSIZE   (uint32_t)(TSB_UDFS2_BASE + 0x70U)
#define UDC2_EP4STATUS          (uint32_t)(TSB_UDFS2_BASE + 0x74U)
#define UDC2_EP4DATASIZE        (uint32_t)(TSB_UDFS2_BASE + 0x78U)
#define UDC2_EP4FIFO            (uint32_t)(TSB_UDFS2_BASE + 0x7CU)
#define UDC2_EP5MAXPACKETSIZE   (uint32_t)(TSB_UDFS2_BASE + 0x80U)
#define UDC2_EP5STATUS          (uint32_t)(TSB_UDFS2_BASE + 0x84U)
#define UDC2_EP5DATASIZE        (uint32_t)(TSB_UDFS2_BASE + 0x88U)
#define UDC2_EP5FIFO            (uint32_t)(TSB_UDFS2_BASE + 0x8CU)
#define UDC2_EP6MAXPACKETSIZE   (uint32_t)(TSB_UDFS2_BASE + 0x90U)
#define UDC2_EP6STATUS          (uint32_t)(TSB_UDFS2_BASE + 0x94U)
#define UDC2_EP6DATASIZE        (uint32_t)(TSB_UDFS2_BASE + 0x98U)
#define UDC2_EP6FIFO            (uint32_t)(TSB_UDFS2_BASE + 0x9CU)
#define UDC2_EP7MAXPACKETSIZE   (uint32_t)(TSB_UDFS2_BASE + 0xA0U)
#define UDC2_EP7STATUS          (uint32_t)(TSB_UDFS2_BASE + 0xA4U)
#define UDC2_EP7DATASIZE        (uint32_t)(TSB_UDFS2_BASE + 0xA8U)
#define UDC2_EP7FIFO            (uint32_t)(TSB_UDFS2_BASE + 0xACU)

#define UDC2_EPxMAXPACKETSIZE(x)    (uint32_t)(UDC2_EP0MAXPACKETSIZE + (0x10U * x))
#define UDC2_EPxSTATUS(x)           (uint32_t)(UDC2_EP0STATUS + (0x10U * x))
#define UDC2_EPxDATASIZE(x)         (uint32_t)(UDC2_EP0DATASIZE + (0x10U * x))
#define UDC2_EPxFIFO(x)             (uint32_t)(UDC2_EP0FIFO + (0x10U * x))

#define UDC2_INTNAK             (uint32_t)(TSB_UDFS2_BASE + 0x130U)
#define UDC2_INTNAK_MASK        (uint32_t)(TSB_UDFS2_BASE + 0x134U)

#define IS_UDC2REG_READ(param)  ((((((param) & 0x3FCU) >> 2U) >= 0x80U) && \
                                  ((((param) & 0x3FCU) >> 2U) <= 0xABU) && \
                                  ((((param) & 0x3FCU) >> 2U) != 0x82U)) || \
                                 ((((param) & 0x3FCU) >> 2U) == 0xCCU)  || \
                                 ((((param) & 0x3FCU) >> 2U) == 0xCDU))

#define IS_UDC2REG_WRITE(param)  (((param) == UDC2_ADDR) || \
                                  ((param) == UDC2_FRAME) || \
                                  ((param) == UDC2_COMMAND) || \
                                  ((param) == UDC2_INT) || \
                                  ((param) == UDC2_INTEP) || \
                                  ((param) == UDC2_INTEP_MASK) || \
                                  ((param) == UDC2_INTRX0) || \
                                  ((param) == UDC2_EP0MAXPACKETSIZE) || \
                                  ((param) == UDC2_EP0FIFO) || \
                                  ((param) == UDC2_EP1MAXPACKETSIZE) || \
                                  ((param) == UDC2_EP1STATUS) || \
                                  ((param) == UDC2_EP1FIFO) || \
                                  ((param) == UDC2_EP2MAXPACKETSIZE) || \
                                  ((param) == UDC2_EP2STATUS) || \
                                  ((param) == UDC2_EP2FIFO) || \
                                  ((param) == UDC2_EP3MAXPACKETSIZE) || \
                                  ((param) == UDC2_EP3STATUS) || \
                                  ((param) == UDC2_EP3FIFO) || \
                                  ((param) == UDC2_EP4MAXPACKETSIZE) || \
                                  ((param) == UDC2_EP4STATUS) || \
                                  ((param) == UDC2_EP4FIFO) || \
                                  ((param) == UDC2_EP5MAXPACKETSIZE) || \
                                  ((param) == UDC2_EP5STATUS) || \
                                  ((param) == UDC2_EP5FIFO) || \
                                  ((param) == UDC2_EP6MAXPACKETSIZE) || \
                                  ((param) == UDC2_EP6STATUS) || \
                                  ((param) == UDC2_EP6FIFO) || \
                                  ((param) == UDC2_EP7MAXPACKETSIZE) || \
                                  ((param) == UDC2_EP7STATUS) || \
                                  ((param) == UDC2_EP7FIFO) || \
                                  ((param) == UDC2_INTNAK) || \
                                  ((param) == UDC2_INTNAK_MASK))


/** @} */
/* End of group USBD_Exported_types */

/** @defgroup USBD_HW_Exported_FunctionPrototypes
  * @{
  */


    USBD_INTStatus USBD_GetINTStatus(void);
    void USBD_SetINTMask(USBD_INTSrc IntSrc, FunctionalState NewState);
    void USBD_ClearINT(USBD_INTSrc IntSrc);
    USBD_DMACConfig USBD_GetDMACConfig(void);
    void USBD_ConfigDMAC(USBD_DMACConfig Config);
    USBD_DMACStatus USBD_GetDMACStatus(void);
    void USBD_ReadUDC2Reg(uint32_t Addr, uint32_t * Data);
    void USBD_WriteUDC2Reg(uint32_t Addr, const uint32_t Data);
    USBD_DMACAddr USBD_GetDMACMasterAddr(USBD_MasterMode MasterMode);
    void USBD_SetDMACMasterAddr(USBD_MasterMode MasterMode, USBD_DMACAddr Addr);
    USBD_PowerCtrl USBD_GetPowerCtrlStatus(void);
    void USBD_SetPowerCtrl(USBD_PowerCtrl PowerCtrl);
    void USBD_SetEPCMD(USBD_EPx EPx, USBD_EPCMD Cmd);
    USBD_EP0Status USBD_GetEP0Status(void);
    USBD_EPxStatus USBD_GetEPxStatus(USBD_EPx EPx);
    void USBD_ConfigEPx(USBD_EPx EPx, USBD_EPxConfig Config);
    void USBD_SetINTEPMask(USBD_EPx EPx, FunctionalState NewState);
    void USBD_Handler(void);

/** @} */
/* End of group USBD_HW_Exported_FunctionPrototypes */

/** @} */
/* End of group USBD */

/** @} */
/* End of group TX03_Periph_Driver */

#endif

#ifdef __cplusplus
}
#endif
#endif /*__USB_HW_H */
