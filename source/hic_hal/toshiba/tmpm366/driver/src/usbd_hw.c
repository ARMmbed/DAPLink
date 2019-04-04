/**
 ****************************************************************************
 * @file    usbd_hw.c
 * @brief   This file provides all the functions code for USBD hardware driver.
 * @version V2.0.2.4
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


/* Includes ------------------------------------------------------------------*/
#include "usb_hw.h"

#if defined(__USB_HW_H)
/** @addtogroup TX03_Periph_Driver
  * @{
  */

/** @defgroup USBD
  * @brief USBD Hardware driver modules
  * @{
  */

/** @defgroup USBD_Private_Defines
  * @{
  */


#define USBD_INTSTATUS_MASK     (uint32_t)0x23FE07FF
#define USBD_INTMASK_UDC2AB     (uint32_t)0x23FE0700

/* MSTSET: bit5 and bit1 is "write only", must be masked in read */
#define USBD_DMACCONFIG_MASK_READ     (uint32_t)0x0155
#define USBD_DMACCONFIG_MASK_WRITE    (uint32_t)0x0177

#define USBD_DMACSTATUS_MASK    (uint32_t)0x001F
#define USBD_POWERCTRL_MASK     (uint32_t)0x00FF
#define UDC2_EP0STATUS_MASK     (uint32_t)0xBE00
#define UDC2_EPxSTATUS_MASK     (uint32_t)0xFF8F


/** @} */
/* End of group USBD_Private_Defines */


/** @defgroup USBD_Private_FunctionPrototypes
  * @{
  */

/** @} */
/* End of group USBD_Private_FunctionPrototypes */

/** @defgroup USBD_Private_Functions
  * @{
  */


/** @} */
/* End of group USBD_Private_Functions */


/** @defgroup USBD_HW_Exported_FunctionPrototypes
  * @{
  */

/**
  * @brief  Get all the interrupt status of USBD.
  * @param  None
  * @retval The state of each interrupt source.
  */
USBD_INTStatus USBD_GetINTStatus(void)
{
    USBD_INTStatus tmp = { 0U };
    tmp.All = TSB_UDFS->INTSTS & USBD_INTSTATUS_MASK;
    return tmp;
}


/**
  * @brief  Set mask of the specified interrupt of USBD.
  * @param  IntSrc: Specify the interrupt source.
  *   This parameter can be one of the following values:
  *      USBD_INT_SETUP
  *      USBD_INT_STATUS_NAK
  *      USBD_INT_STATUS
  *      USBD_INT_RX_ZERO
  *      USBD_INT_SOF
  *      USBD_INT_EP0
  *      USBD_INT_EP
  *      USBD_INT_NAK
  *      USBD_INT_SUSPEND_RESUME
  *      USBD_INT_USB_RESET
  *      USBD_INT_USB_RESET_END
  *      USBD_INT_MW_SET_ADD
  *      USBD_INT_MW_END_ADD
  *      USBD_INT_MW_TIMEOUT
  *      USBD_INT_MW_AHBERR
  *      USBD_INT_MR_END_ADD
  *      USBD_INT_MR_EP_DSET
  *      USBD_INT_MR_AHBERR
  *      USBD_INT_UDC2_REG_READ
  *      USBD_INT_DMAC_REG_READ
  *      USBD_INT_MW_READERROR
  * @param NewState: Set the interrupt source mask state.
  *   This parameter can be one of the following values:
  *      ENABLE
  *      DISABLE
  * @retval None
  */
void USBD_SetINTMask(USBD_INTSrc IntSrc, FunctionalState NewState)
{
    uint32_t tmp = 0U;

    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (IntSrc <= USBD_INT_NAK) {
        USBD_ReadUDC2Reg(UDC2_INT, &tmp);
        tmp &= 0x0000FF00UL;
        if (NewState == ENABLE) {
            /* Clear bit to enable UDC2 interrupt */
            tmp &= (uint32_t) (~(0x01UL << ((uint32_t) IntSrc + 8U)));
        } else {
            /* Set bit to disable UDC2 interrupt */
            tmp |= 0x01UL << ((uint32_t) IntSrc + 8U);
        }
        USBD_WriteUDC2Reg(UDC2_INT, tmp);

    } else {
        assert_param(IS_USBD_UDC2AB_INT(IntSrc));

        tmp = TSB_UDFS->INTENB & USBD_INTMASK_UDC2AB;
        if (NewState == ENABLE) {
            /* Set bit to enable UDC2AB interrupt */
            tmp |= 0x01UL << (uint32_t) IntSrc;
        } else {
            /* Clear bit to disable UDC2AB interrupt  */
            tmp &= (uint32_t) (~(0x01UL << (uint32_t) IntSrc));
        }
        TSB_UDFS->INTENB = tmp;
    }
}


/**
  * @brief  Clear the specified interrupt flag of USBD.
  * @param  IntSrc: Specify the interrupt source.
  *   This parameter can be one of the following values:
  *      USBD_INT_SETUP
  *      USBD_INT_STATUS_NAK
  *      USBD_INT_STATUS
  *      USBD_INT_RX_ZERO
  *      USBD_INT_SOF
  *      USBD_INT_EP0
  *      USBD_INT_EP
  *      USBD_INT_NAK
  *      USBD_INT_SUSPEND_RESUME
  *      USBD_INT_USB_RESET
  *      USBD_INT_USB_RESET_END
  *      USBD_INT_MW_SET_ADD
  *      USBD_INT_MW_END_ADD
  *      USBD_INT_MW_TIMEOUT
  *      USBD_INT_MW_AHBERR
  *      USBD_INT_MR_END_ADD
  *      USBD_INT_MR_EP_DSET
  *      USBD_INT_MR_AHBERR
  *      USBD_INT_UDC2_REG_READ
  *      USBD_INT_DMAC_REG_READ
  *      USBD_INT_MW_READERROR
  * @retval None
  */
void USBD_ClearINT(USBD_INTSrc IntSrc)
{
    uint32_t tmp = 0U;
    if (IntSrc <= USBD_INT_NAK) {
        USBD_ReadUDC2Reg(UDC2_INT, &tmp);
        tmp &= 0x0000FF00UL;
        /* Set bit to clear UDC2 Interrupt */
        tmp |= 0x01UL << (uint32_t) IntSrc;
        USBD_WriteUDC2Reg(UDC2_INT, tmp);
    } else {
        assert_param(IS_USBD_UDC2AB_INT(IntSrc));
        /* Set bit to clear the specified UDC2AB Interrupt. Only clear one Interrupt */
        tmp |= 0x01UL << (uint32_t) IntSrc;

        TSB_UDFS->INTSTS = tmp;
    }
}


/**
  * @brief  Get the configuration of DMA Controller in USBD.
  * @param  None
  * @retval The configuration of DMA Controller in USBD.
  */
USBD_DMACConfig USBD_GetDMACConfig(void)
{
    USBD_DMACConfig tmp = { 0U };
    tmp.All = TSB_UDFS->MSTSET & USBD_DMACCONFIG_MASK_READ;
    return tmp;
}


/**
  * @brief  Configure the DMA Controller of USBD.
  * @param  Config: Contain the configuration for DMA Controller of USBD.
  * @retval None
  */
void USBD_ConfigDMAC(USBD_DMACConfig Config)
{
    Config.All &= USBD_DMACCONFIG_MASK_WRITE;
    TSB_UDFS->MSTSET = Config.All;
}


/**
  * @brief  Get the status of DMA Controller Master Operation.
  * @param  None
  * @retval The status of DMAC Master Operation.
  */
USBD_DMACStatus USBD_GetDMACStatus(void)
{
    USBD_DMACStatus tmp = { 0U };
    tmp.All = TSB_UDFS->MSTSTS & USBD_DMACSTATUS_MASK;

    return tmp;
}


/**
  * @brief  Read data from UDC2 registers which must be read through ¡°UDC2 Read Request Register¡± of UDC2AB
  * @param  Addr: The address of registers in UDC2 module.
  *    This parameter can be one of the following values:
  *      UDC2_ADDR
  *      UDC2_FRAME
  *      UDC2_COMMAND
  *      UDC2_BREQ_BMREQTYPE
  *      UDC2_WVALUE
  *      UDC2_WINDEX
  *      UDC2_WLENGTH
  *      UDC2_INT
  *      UDC2_INTEP
  *      UDC2_INTEP_MASK
  *      UDC2_INTRX0
  *      UDC2_EPxMAXPACKETSIZE( x = 0 to 7 )
  *      UDC2_EPxSTATUS( x = 0 to 7 )
  *      UDC2_EPxDATASIZE( x = 0 to 7 )
  *      UDC2_EPxFIFO( x = 0 to 7 )
  *      UDC2_INTNAK
  *      UDC2_INTNAK_MASK
  * @param  Data:  The pointer point to where the value of register will be stored.
  * @retval None
  */
void USBD_ReadUDC2Reg(uint32_t Addr, uint32_t * Data)
{
    volatile uint32_t tmp = 0U;

    assert_param(IS_UDC2REG_READ(Addr));
    assert_param(IS_POINTER_NOT_NULL(Data));

    /* Only need high 8 bits(<9:2>) of 10bits offset address */
    tmp = (Addr & (uint32_t) 0x03FCU);

    /* Set bit31 to issue a read request */
    tmp |= (uint32_t) (0x1U << 31U);
    TSB_UDFS->UDC2RDREQ = tmp;

    do {
        tmp = TSB_UDFS->UDC2RDREQ;
    } while (tmp & ((0x1U << 31U)));    /* Wait till bit31 turn to 0 */

    /* Get result and only keep low 16bits */
    *Data = TSB_UDFS->UDC2RDVL & (uint32_t) (0xFFFFU);
}


/**
  * @brief  Write data to UDC2 registers.
  * @param  Addr: The address of registers in UDC2 module.
  *   This parameter can be one of the following values:
  *      UDC2_ADDR
  *      UDC2_FRAME
  *      UDC2_COMMAND
  *      UDC2_INT
  *      UDC2_INTEP
  *      UDC2_INTEP_MASK
  *      UDC2_INTRX0
  *      UDC2_EP0MAXPACKETSIZE
  *      UDC2_EP0FIFO
  *      UDC2_EPxMAXPACKETSIZE (x = 1 to 7)
  *      UDC2_EPxSTATUS (x = 1 to 7)
  *      UDC2_EPxFIFO (x = 1 to 7)
  *      UDC2_INTNAK
  *      UDC2_INTNAK_MASK
  * @param  Data: The data which will be written to UDC2 register.
  * @retval None
  */
void USBD_WriteUDC2Reg(uint32_t Addr, const uint32_t Data)
{
    uint32_t *tmp_addr = 0U;

    assert_param(IS_UDC2REG_WRITE(Addr));

    tmp_addr = (uint32_t *) Addr;
    *tmp_addr = Data & (uint32_t) (0xFFFFU);    /* only write low 16 bits */
}


/**
  * @brief  Get the addresses of DMAC master operation.
  * @param  MasterMode: Specify DMAC master addresses type.
  *    This parameter can be one of the following values:
  *      USBD_MASTER_WRITE
  *      USBD_MASTER_READ
  * @retval The addresses of DMAC Master Operation.
  */
USBD_DMACAddr USBD_GetDMACMasterAddr(USBD_MasterMode MasterMode)
{
    USBD_DMACAddr tmp = { 0U, 0U };

    assert_param(IS_USBD_MASTERMODE(MasterMode));

    if (MasterMode == USBD_MASTER_READ) {
        tmp.StartAddr = TSB_UDFS->MRSADR;
        tmp.EndAddr = TSB_UDFS->MREADR;
    } else {
        tmp.StartAddr = TSB_UDFS->MWSADR;
        tmp.EndAddr = TSB_UDFS->MWEADR;
    }

    return tmp;
}

/**
  * @brief  Set the addresses for DMAC master operation.
  * @param  MasterMode: Specify DMAC master addresses type.
  *    This parameter can be one of the following values:
  *      USBD_MASTER_WRITE
  *      USBD_MASTER_READ
  * @param  Addr: the addresses for DMAC master operation.
  * @retval None
  */
void USBD_SetDMACMasterAddr(USBD_MasterMode MasterMode, USBD_DMACAddr Addr)
{
    assert_param(IS_USBD_MASTERMODE(MasterMode));

    if (MasterMode == USBD_MASTER_READ) {
        TSB_UDFS->MRSADR = Addr.StartAddr;
        TSB_UDFS->MREADR = Addr.EndAddr;
    } else {
        TSB_UDFS->MWSADR = Addr.StartAddr;
        TSB_UDFS->MWEADR = Addr.EndAddr;
    }
}


/**
  * @brief  Get the status of USBD power detect control.
  * @param  None
  * @retval The status of USBD power detect control.
  */
USBD_PowerCtrl USBD_GetPowerCtrlStatus(void)
{
    USBD_PowerCtrl tmp = { 0U };
    tmp.All = TSB_UDFS->PWCTL & USBD_POWERCTRL_MASK;

    return tmp;
}


/**
  * @brief  Set the USBD power detect control.
  * @param  PowerCtrl: Contain the setting for USBD power detect control.
  * @retval None
  */
void USBD_SetPowerCtrl(USBD_PowerCtrl PowerCtrl)
{
    TSB_UDFS->PWCTL = PowerCtrl.All & USBD_POWERCTRL_MASK;
}


/**
  * @brief  Send command to endpoints.
  * @param  EPx: Specify the target endpoint (x = 0 to 7).
  *    This parameter can be one of the following values:
  *      USBD_EP0, USBD_EP1, USBD_EP2, USBD_EP3, USBD_EP4,
  *      USBD_EP5, USBD_EP6, USBD_EP7
  * @param Cmd: The command which will be sent to endpoint
  *    This parameter can be one of the following values:
  *     when EPx is USBD_EP0:
  *          USBD_CMD_SETUP_FIN
  *          USBD_CMD_EP_RESET
  *          USBD_CMD_EP_STALL
  *          USBD_CMD_ALL_EP_INVALID
  *          USBD_CMD_USB_READY
  *          USBD_CMD_SETUP_RECEIVED
  *          USBD_CMD_EP_EOP
  *          USBD_CMD_EP_FIFO_CLEAR
  *          USBD_CMD_EP_TX_0DATA
  *     when EPx is USBD_EPy (y = 1 to 7):
  *          USBD_CMD_SET_DATA0
  *          USBD_CMD_EP_RESET
  *          USBD_CMD_EP_STALL
  *          USBD_CMD_EP_INVALID
  *          USBD_CMD_EP_DISABLE
  *          USBD_CMD_EP_ENABLE
  *          USBD_CMD_ALL_EP_INVALID
  *          USBD_CMD_EP_EOP
  *          USBD_CMD_EP_FIFO_CLEAR
  *          USBD_CMD_EP_TX_0DATA
  * @retval None
  */
void USBD_SetEPCMD(USBD_EPx EPx, USBD_EPCMD Cmd)
{
    uint32_t tmp = 0U;

    assert_param(IS_USBD_EPxALL(EPx));

    if (EPx == USBD_EP0) {
        assert_param(IS_USBD_EP0CMD(Cmd));
    } else {
        assert_param(IS_USBD_EPxCMD(Cmd));
    }

    USBD_ReadUDC2Reg(UDC2_COMMAND, &tmp);
    tmp &= (uint32_t) (0x1U << 15U);    /* only keep bit15 */
    tmp |= (uint32_t) EPx << 4U;
    tmp |= (uint32_t) Cmd;

    USBD_WriteUDC2Reg(UDC2_COMMAND, tmp);
}


/**
  * @brief  Get current EP0 status.
  * @param  None
  * @retval Current Endpoint0 status.
  */
USBD_EP0Status USBD_GetEP0Status(void)
{
    USBD_EP0Status tmp = { 0U };
    USBD_ReadUDC2Reg(UDC2_EP0STATUS, &tmp.All);
    tmp.All &= UDC2_EP0STATUS_MASK;
    return tmp;
}

/**
  * @brief  Get current EPx status (x = 1 to 7)
  * @param  EPx: Specify the target endpoint.
  *    This parameter can be one of the following values:
  *      USBD_EP1, USBD_EP2, USBD_EP3, USBD_EP4,
  *      USBD_EP5, USBD_EP6, USBD_EP7
  * @retval The status for the specified EPx.
  */
USBD_EPxStatus USBD_GetEPxStatus(USBD_EPx EPx)
{
    USBD_EPxStatus tmp = { 0U };
    uint32_t tmpAddr = 0U;

    assert_param(IS_USBD_NOT_EP0(EPx));

    tmpAddr = (uint32_t) UDC2_EP1STATUS + (0x10U * ((uint32_t) (EPx) - 1U));
    USBD_ReadUDC2Reg(tmpAddr, &tmp.All);

    tmp.All &= UDC2_EPxSTATUS_MASK;
    return tmp;
}

/**
  * @brief  Configure EPx (x = 1 to 7).
  * @param  EPx: Specify the target endpoint.
  *    This parameter can be one of the following values:
  *      USBD_EP1, USBD_EP2, USBD_EP3, USBD_EP4,
  *      USBD_EP5, USBD_EP6, USBD_EP7
  * @param  Config: Contain the settting information for EPx.
  * @retval None
  */
void USBD_ConfigEPx(USBD_EPx EPx, USBD_EPxConfig Config)
{
    uint32_t tmp = 0U;
    uint32_t tmpAddr = 0U;

    assert_param(IS_USBD_NOT_EP0(EPx));

    tmp = (uint32_t) Config.All & UDC2_EPxSTATUS_MASK;
    tmpAddr = (uint32_t) UDC2_EP1STATUS + (0x10U * ((uint32_t) (EPx) - 1U));
    USBD_WriteUDC2Reg(tmpAddr, tmp);
}

/**
  * @brief  Set mask of the specified EPx interrupt.
  * @param  EPx: Endpoint number.
  * @param  NewState: Set the interrupt source mask state.
  *   This parameter can be one of the following values:
  *      ENABLE
  *      DISABLE
  * @retval None
  */
void USBD_SetINTEPMask(USBD_EPx EPx, FunctionalState NewState)
{
    uint32_t tmp = 0U;

    assert_param(IS_FUNCTIONAL_STATE(NewState));

    USBD_ReadUDC2Reg(UDC2_INTEP_MASK, &tmp);
    /* no need to mask 'tmp' because it has been only kept low 16bits above */
    if (NewState == ENABLE) {
        /* Clear bit to enable UDC2 interrupt */
        tmp &= (uint32_t) (~(0x01UL << (uint32_t) EPx));
    } else {
        /* Set bit to disable UDC2 interrupt */
        tmp |= 0x01UL << (uint32_t) EPx;
    }
    USBD_WriteUDC2Reg(UDC2_INTEP_MASK, tmp);
}


/** @} */
/* End of group USBD_HW_Exported_FunctionPrototypes */

/** @} */
/* End of group USBD */

/** @} */
/* End of group TX03_Periph_Driver */

#endif /*__USB_HW_H */
