/*******************************************************************************
* @file    usbd_PSoC5LP.h
* @brief   This file contains the function prototypes and constants used in
*          the usbd_PSoC5LP.c
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

#ifndef USBD_PSOC5LP_H
#define USBD_PSOC5LP_H                              /**< Symbol preventing repeated inclusion */

#include "cytypes.h"

/***************************************
*          API Constants
***************************************/

#define USBFS_EP0                        (0u)
#define USBFS_EP1                        (1u)
#define USBFS_EP2                        (2u)
#define USBFS_EP3                        (3u)
#define USBFS_EP4                        (4u)
#define USBFS_EP5                        (5u)
#define USBFS_EP6                        (6u)
#define USBFS_EP7                        (7u)
#define USBFS_EP8                        (8u)
#define USBFS_MAX_EP                     (9u)

/* USB Endpoint Directions */
#define USBFS_DIR_IN                     (0x80u)

#define USBFS_MODE_DISABLE               (0x00u)
#define USBFS_MODE_NAK_IN_OUT            (0x01u)
#define USBFS_MODE_STATUS_OUT_ONLY       (0x02u)
#define USBFS_MODE_STALL_IN_OUT          (0x03u)
#define USBFS_MODE_ISO_OUT               (0x05u)
#define USBFS_MODE_STATUS_IN_ONLY        (0x06u)
#define USBFS_MODE_ISO_IN                (0x07u)
#define USBFS_MODE_NAK_OUT               (0x08u)
#define USBFS_MODE_ACK_OUT               (0x09u)
#define USBFS_MODE_ACK_OUT_STATUS_IN     (0x0Bu)
#define USBFS_MODE_NAK_IN                (0x0Cu)
#define USBFS_MODE_ACK_IN                (0x0Du)
#define USBFS_MODE_ACK_IN_STATUS_OUT     (0x0Fu)
#define USBFS_MODE_MASK                  (0x0Fu)
#define USBFS_MODE_STALL_DATA_EP         (0x80u)

#define USBFS_MODE_ACKD                  (0x10u)
#define USBFS_MODE_OUT_RCVD              (0x20u)
#define USBFS_MODE_IN_RCVD               (0x40u)
#define USBFS_MODE_SETUP_RCVD            (0x80u)

#define USBFS_EP_TYPE_ISOC               (0x01u)
#define USBFS_EP_TYPE_MASK               (0x03u)

#define USBFS_NO_EVENT_PENDING           (0u)
#define USBFS_EVENT_PENDING              (1u)
#define USBFS_NO_EVENT_ALLOWED           (2u)

#define USBFS_IN_BUFFER_EMPTY            USBFS_EVENT_PENDING
#define USBFS_OUT_BUFFER_FULL            USBFS_EVENT_PENDING

#define USBFS_ENDPOINT_STATUS_HALT           (0x01u)

#define USBFS_PM_ACT_EN_FSUSB            USBFS_USB__PM_ACT_MSK
#define USBFS_PM_STBY_EN_FSUSB           USBFS_USB__PM_STBY_MSK
#define USBFS_DM_MASK                    USBFS_Dm__0__MASK
#define USBFS_DP_MASK                    USBFS_Dp__0__MASK

#define USBFS_BUS_RESET_PRIOR    USBFS_bus_reset__INTC_PRIOR_NUM
#define USBFS_BUS_RESET_VECT_NUM USBFS_bus_reset__INTC_NUMBER
#define USBFS_EP_0_PRIOR         USBFS_ep_0__INTC_PRIOR_NUM
#define USBFS_EP_0_VECT_NUM      USBFS_ep_0__INTC_NUMBER
#define USBFS_SOF_PRIOR          USBFS_sof_int__INTC_PRIOR_NUM
#define USBFS_SOF_VECT_NUM       USBFS_sof_int__INTC_NUMBER
#define USBFS_EP_1_PRIOR         USBFS_ep_1__INTC_PRIOR_NUM
#define USBFS_EP_1_VECT_NUM      USBFS_ep_1__INTC_NUMBER
#define USBFS_EP_2_PRIOR         USBFS_ep_2__INTC_PRIOR_NUM
#define USBFS_EP_2_VECT_NUM      USBFS_ep_2__INTC_NUMBER
#define USBFS_EP_3_PRIOR         USBFS_ep_3__INTC_PRIOR_NUM
#define USBFS_EP_3_VECT_NUM      USBFS_ep_3__INTC_NUMBER
#define USBFS_EP_4_PRIOR         USBFS_ep_4__INTC_PRIOR_NUM
#define USBFS_EP_4_VECT_NUM      USBFS_ep_4__INTC_NUMBER
#define USBFS_EP_5_PRIOR         USBFS_ep_5__INTC_PRIOR_NUM
#define USBFS_EP_5_VECT_NUM      USBFS_ep_5__INTC_NUMBER
#define USBFS_EP_6_PRIOR         USBFS_ep_6__INTC_PRIOR_NUM
#define USBFS_EP_6_VECT_NUM      USBFS_ep_6__INTC_NUMBER
#define USBFS_EP_7_PRIOR         USBFS_ep_7__INTC_PRIOR_NUM
#define USBFS_EP_7_VECT_NUM      USBFS_ep_7__INTC_NUMBER
#define USBFS_EP_8_PRIOR         USBFS_ep_8__INTC_PRIOR_NUM
#define USBFS_EP_8_VECT_NUM      USBFS_ep_8__INTC_NUMBER
#define USBFS_DP_INTC_PRIORITY       USBFS_dp_int__INTC_PRIOR_NUM
#define USBFS_DP_INTC_VECT_NUM       USBFS_dp_int__INTC_NUMBER

/* USBFS_INTR_SIE */
#define USBFS_INTR_SIE_EP0_INTR_POS          (2u) /* [2] Interrupt for EP0       */
#define USBFS_INTR_SIE_EP0_INTR              ((uint32) 0x01u << USBFS_INTR_SIE_EP0_INTR_POS)

/* USBFS_SIE_INT */
#define USBFS_SIE_INT_EP1_INTR_POS          (0u) /* [0] Interrupt for USB EP1 */
#define USBFS_SIE_INT_EP2_INTR_POS          (1u) /* [1] Interrupt for USB EP2 */
#define USBFS_SIE_INT_EP3_INTR_POS          (2u) /* [2] Interrupt for USB EP3 */
#define USBFS_SIE_INT_EP4_INTR_POS          (3u) /* [3] Interrupt for USB EP4 */
#define USBFS_SIE_INT_EP5_INTR_POS          (4u) /* [4] Interrupt for USB EP5 */
#define USBFS_SIE_INT_EP6_INTR_POS          (5u) /* [5] Interrupt for USB EP6 */
#define USBFS_SIE_INT_EP7_INTR_POS          (6u) /* [6] Interrupt for USB EP7 */
#define USBFS_SIE_INT_EP8_INTR_POS          (7u) /* [7] Interrupt for USB EP8 */
#define USBFS_SIE_INT_EP1_INTR              ((uint8) ((uint8) 0x01u << USBFS_SIE_INT_EP1_INTR_POS))
#define USBFS_SIE_INT_EP2_INTR              ((uint8) ((uint8) 0x01u << USBFS_SIE_INT_EP2_INTR_POS))
#define USBFS_SIE_INT_EP3_INTR              ((uint8) ((uint8) 0x01u << USBFS_SIE_INT_EP3_INTR_POS))
#define USBFS_SIE_INT_EP4_INTR              ((uint8) ((uint8) 0x01u << USBFS_SIE_INT_EP4_INTR_POS))
#define USBFS_SIE_INT_EP5_INTR              ((uint8) ((uint8) 0x01u << USBFS_SIE_INT_EP5_INTR_POS))
#define USBFS_SIE_INT_EP6_INTR              ((uint8) ((uint8) 0x01u << USBFS_SIE_INT_EP6_INTR_POS))
#define USBFS_SIE_INT_EP7_INTR              ((uint8) ((uint8) 0x01u << USBFS_SIE_INT_EP7_INTR_POS))
#define USBFS_SIE_INT_EP8_INTR              ((uint8) ((uint8) 0x01u << USBFS_SIE_INT_EP8_INTR_POS))

/* USBFS_USBIO_CR0 */

#define USBFS_USBIO_CR0_TEN              (0x80u)
#define USBFS_USBIO_CR0_TSE0             (0x40u)
#define USBFS_USBIO_CR0_TD               (0x20u)
#define USBFS_USBIO_CR0_RD               (0x01u)

/* USBFS_USBIO_CR1 */
#define USBFS_USBIO_CR1_DM0_POS      (0u)
#define USBFS_USBIO_CR1_DP0_POS      (1u)
#define USBFS_USBIO_CR1_USBPUEN_POS  (2u)
#define USBFS_USBIO_CR1_IOMODE_POS   (5u)
#define USBFS_USBIO_CR1_DM0          ((uint8) ((uint8) 0x1u << USBFS_USBIO_CR1_DM0_POS))
#define USBFS_USBIO_CR1_DP0          ((uint8) ((uint8) 0x1u << USBFS_USBIO_CR1_DP0_POS))
#define USBFS_USBIO_CR1_USBPUEN      ((uint8) ((uint8) 0x1u << USBFS_USBIO_CR1_USBPUEN_POS))
#define USBFS_USBIO_CR1_IOMODE       ((uint8) ((uint8) 0x1u << USBFS_USBIO_CR1_IOMODE_POS))

/* USBFS_CR0 */
#define USBFS_CR0_DEVICE_ADDRESS_POS     (0u)
#define USBFS_CR0_ENABLE_POS             (7u)
#define USBFS_CR0_DEVICE_ADDRESS_MASK    ((uint8) ((uint8) 0x7Fu << USBFS_CR0_DEVICE_ADDRESS_POS))
#define USBFS_CR0_ENABLE                 ((uint8) ((uint8) 0x01u << USBFS_CR0_ENABLE_POS))

/* USBFS_CR1 */
#define USBFS_CR1_REG_ENABLE_POS         (0u)
#define USBFS_CR1_ENABLE_LOCK_POS        (1u)
#define USBFS_CR1_BUS_ACTIVITY_POS       (2u)
#define USBFS_CR1_TRIM_OFFSET_MSB_POS    (3u)
#define USBFS_CR1_REG_ENABLE             ((uint8) ((uint8) 0x1u << USBFS_CR1_REG_ENABLE_POS))
#define USBFS_CR1_ENABLE_LOCK            ((uint8) ((uint8) 0x1u << USBFS_CR1_ENABLE_LOCK_POS))
#define USBFS_CR1_BUS_ACTIVITY           ((uint8) ((uint8) 0x1u << USBFS_CR1_BUS_ACTIVITY_POS))
#define USBFS_CR1_TRIM_OFFSET_MSB        ((uint8) ((uint8) 0x1u << USBFS_CR1_TRIM_OFFSET_MSB_POS))

/* USBFS_USB_CLK */
#define USBFS_USB_CLK_CSR_CLK_EN_POS (0u)
#define USBFS_USB_CLK_CSR_CLK_EN     ((uint8) ((uint8) 0x1u << USBFS_USB_CLK_CSR_CLK_EN_POS))
#define USBFS_USB_CLK_ENABLE         (USBFS_USB_CLK_CSR_CLK_EN)

/* USBFS_EPX_CNT */
#define USBFS_EP0_CNT_DATA_TOGGLE        (0x80u)
#define USBFS_EPX_CNT_DATA_TOGGLE        (0x80u)
#define USBFS_EPX_CNT0_MASK              (0x0Fu)
#define USBFS_EPX_CNTX_CRC_COUNT         (0x02u)
#define USBFS_EPX_DATA_BUF_MAX           (512u)

/* USBFS_ARB_EPX_CFG */
#define USBFS_ARB_EPX_CFG_IN_DATA_RDY_POS    (0u)
#define USBFS_ARB_EPX_CFG_DMA_REQ_POS        (1u)
#define USBFS_ARB_EPX_CFG_CRC_BYPASS_POS     (2u)
#define USBFS_ARB_EPX_CFG_RESET_POS          (3u)
#define USBFS_ARB_EPX_CFG_IN_DATA_RDY        ((uint8) ((uint8) 0x1u << USBFS_ARB_EPX_CFG_IN_DATA_RDY_POS))
#define USBFS_ARB_EPX_CFG_DMA_REQ            ((uint8) ((uint8) 0x1u << USBFS_ARB_EPX_CFG_DMA_REQ_POS))
#define USBFS_ARB_EPX_CFG_CRC_BYPASS         ((uint8) ((uint8) 0x1u << USBFS_ARB_EPX_CFG_CRC_BYPASS_POS))
#define USBFS_ARB_EPX_CFG_RESET              ((uint8) ((uint8) 0x1u << USBFS_ARB_EPX_CFG_RESET_POS))

/* Wait cycles required for USB reference restore: 40us */
#define USBFS_WAIT_VREF_RESTORE          (40u)
/* Wait cycles required for stabilization after register is written : 50ns */
#define USBFS_WAIT_REG_STABILITY_50NS    (0u)
#define USBFS_WAIT_REG_STABILITY_1US     (1u)
/* Wait cycles required after PD_PULLUP_N bit is set in PM_USB_CR0: 2us */
#define USBFS_WAIT_PD_PULLUP_N_ENABLE    (2u)
/* Wait cycles required after CR0 register write: 1 cycle */
#define USBFS_WAIT_CR0_REG_STABILITY     (1u)

#define USBFS_PM_USB_CR0_REF_EN          (0x01u)
#define USBFS_PM_USB_CR0_PD_N            (0x02u)
#define USBFS_PM_USB_CR0_PD_PULLUP_N     (0x04u)

#define USBFS_3V_OPERATION               (0x00u)
#define USBFS_5V_OPERATION               (0x01u)
#define USBFS_DWR_POWER_OPERATION        (0x02u)
/* Renamed global constants */
#define USBFS_DWR_VDDD_OPERATION         (USBFS_DWR_POWER_OPERATION)


/***************************************
*    Data Structures Definition
***************************************/

typedef struct
{
    uint8  attrib;
    uint8  apiEpState;
    uint8  hwEpState;
    uint8  epToggle;
    uint8  addr;
    uint8  epMode;
    uint16 buffOffset;
    uint16 bufferSize;
    uint8  interface;
} USBFS_EP_CTL_BLOCK;

/* Number of SIE endpoint registers group. */
#define USBFS_SIE_EP_REG_SIZE   (USBFS_USB__SIE_EP1_CR0 - \
                                            USBFS_USB__SIE_EP1_CNT0)

/* Size of gap between SIE endpoint registers groups. */
#define USBFS_SIE_GAP_CNT        (((USBFS_USB__SIE_EP2_CNT0 - \
                                             (USBFS_USB__SIE_EP1_CNT0 + \
                                              USBFS_SIE_EP_REG_SIZE)) / sizeof(reg8)) - 1u)

/* Structure to access to SIE registers for endpoint. */
typedef struct
{
    uint8 epCnt0;
    uint8 epCnt1;
    uint8 epCr0;
    uint8 gap[USBFS_SIE_GAP_CNT];
} USBFS_sie_ep_struct;

/* Number of endpoint (takes to account that endpoints numbers are 1-8). */
#define USBFS_NUMBER_EP  (9u)

/* Consoled SIE register groups for endpoints 1-8. */
typedef struct
{
    USBFS_sie_ep_struct sieEp[USBFS_NUMBER_EP];
} USBFS_sie_eps_struct;

/* Number of ARB endpoint registers group. */
#define USBFS_ARB_EP_REG_SIZE    (USBFS_USB__ARB_RW1_DR - \
                                             USBFS_USB__ARB_EP1_CFG)

/* Size of gap between ARB endpoint registers groups. */
#define USBFS_ARB_GAP_CNT        (((USBFS_USB__ARB_EP2_CFG - \
                                             (USBFS_USB__ARB_EP1_CFG + \
                                              USBFS_ARB_EP_REG_SIZE)) / sizeof(reg8)) - 1u)

/* Structure to access to ARB registers for endpoint. */
typedef struct
{
    uint8 epCfg;
    uint8 epIntEn;
    uint8 epSr;
    uint8 reserved;
    uint8 rwWa;
    uint8 rwWaMsb;
    uint8 rwRa;
    uint8 rwRaMsb;
    uint8 rwDr;
    uint8 gap[USBFS_ARB_GAP_CNT];
} USBFS_arb_ep_struct;

/* Consolidate ARB register groups for endpoints 1-8.*/
typedef struct
{
    USBFS_arb_ep_struct arbEp[USBFS_NUMBER_EP];
} USBFS_arb_eps_struct;

/* Number of endpoint 0 data registers. */
#define USBFS_EP0_DR_MAPPED_REG_CNT  (8u)

/* Structure to access data registers for EP0. */
typedef struct
{
    uint8 epData[USBFS_EP0_DR_MAPPED_REG_CNT];
} USBFS_ep0_data_struct;

/***************************************
*              Registers
***************************************/

#define USBFS_SIE_EP_INT_SR_REG  (*(reg8 *) USBFS_USB__SIE_EP_INT_SR)

#define USBFS_SIE_EP_BASE        (*(volatile USBFS_sie_eps_struct *) \
                                            (USBFS_USB__SIE_EP1_CNT0 - sizeof(USBFS_sie_ep_struct)))
#define USBFS_ARB_EP_BASE        (*(volatile USBFS_arb_eps_struct *) \
                                            (USBFS_USB__ARB_EP1_CFG - sizeof(USBFS_arb_ep_struct)))
#define USBFS_EP0_DR_BASE        (*(volatile USBFS_ep0_data_struct *) USBFS_USB__EP0_DR0)

#define USBFS_EP0_CR_REG         (*(reg8 *) USBFS_USB__EP0_CR)
#define USBFS_EP0_CNT_REG        (*(reg8 *) USBFS_USB__EP0_CNT)
#define USBFS_CR0_REG            (*(reg8 *) USBFS_USB__CR0)
#define USBFS_CR1_REG            (*(reg8 *) USBFS_USB__CR1)
#define USBFS_USBIO_CR0_REG      (*(reg8 *) USBFS_USB__USBIO_CR0)
#define USBFS_USBIO_CR1_REG      (*(reg8 *) USBFS_USB__USBIO_CR1)

/* USBFS_PM_ACT/STBY_CFG */
#define USBFS_PM_ACT_CFG_REG     (*(reg8 *) USBFS_USB__PM_ACT_CFG)
#define USBFS_PM_STBY_CFG_REG    (*(reg8 *) USBFS_USB__PM_STBY_CFG)
#define USBFS_PM_USB_CR0_REG     (*(reg8 *) CYREG_PM_USB_CR0)

#define USBFS_USB_CLK_EN_REG     (*(reg8 *) USBFS_USB__USB_CLK_EN)

#define USBFS_DM_INP_DIS_REG     (*(reg8 *) USBFS_Dm__INP_DIS)
#define USBFS_DP_INP_DIS_REG     (*(reg8 *) USBFS_Dp__INP_DIS)

#define USBFS_ARB_RW1_WA_PTR     ( (reg8 *) USBFS_USB__ARB_RW1_WA)
#define USBFS_ARB_RW1_WA_MSB_PTR ( (reg8 *) USBFS_USB__ARB_RW1_WA_MSB)

#define USBFS_SIE_EP_INT_EN_REG  (*(reg8 *) USBFS_USB__SIE_EP_INT_EN)

/***************************************
*    Initialization Register Settings
***************************************/

/* Clear device address and enable USB IP respond to USB traffic. */
#define USBFS_DEFUALT_CR0    (USBFS_CR0_ENABLE)

#define USBFS_ARB_EPX_CFG_DEFAULT    (USBFS_ARB_EPX_CFG_RESET | \
                                                 USBFS_ARB_EPX_CFG_CRC_BYPASS)

/***************************************
*       Macros Definitions
***************************************/

#define USBFS_ClearSieInterruptSource(intMask) \
    do{ /* Does nothing. */ }while(false)
#define USBFS_ClearSieEpInterruptSource(intMask) \
    do{ \
        USBFS_SIE_EP_INT_SR_REG = (uint8) (intMask); \
    }while(false)
#define USBFS_GET_EP_TYPE(epNumber)  (USBFS_EP[epNumber].attrib & USBFS_EP_TYPE_MASK)
#define USBFS_GET_ACTIVE_IN_EP_CR0_MODE(epType)  (((epType) == USBFS_EP_TYPE_ISOC) ? \
                                                                (USBFS_MODE_ISO_IN) : (USBFS_MODE_ACK_IN))
#define USBFS_GET_ACTIVE_OUT_EP_CR0_MODE(epType) (((epType) == USBFS_EP_TYPE_ISOC) ? \
                                                                (USBFS_MODE_ISO_OUT) : (USBFS_MODE_ACK_OUT))

void USBD_Reset_Event(void);

#endif
