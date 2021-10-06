/****************************************************************************//**
* \file Bootloadable.c
* \version 1.60
*
* \brief
*   Provides an API for the Bootloadable application. The API includes a
*   single function for starting the Bootloader.
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


#ifndef CY_BOOTLOADABLE_Bootloadable_H
#define CY_BOOTLOADABLE_Bootloadable_H

#include "cydevice_trm.h"
#include "CyFlash.h"


/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Bootloadable_v1_60 requires cy_boot v3.0 or later
#endif /* !defined (CY_PSOC5LP) */


#ifndef CYDEV_FLASH_BASE
    #define CYDEV_FLASH_BASE                            CYDEV_FLS_BASE
    #define CYDEV_FLASH_SIZE                            CYDEV_FLS_SIZE
#endif /* CYDEV_FLASH_BASE */

#if(CY_PSOC3)
    #define Bootloadable_GET_CODE_DATA(idx)         (*((uint8  CYCODE *) (idx)))
#else
    #define Bootloadable_GET_CODE_DATA(idx)         (*((uint8  *)(CYDEV_FLASH_BASE + (idx))))
#endif /* (CY_PSOC3) */


/*******************************************************************************
* This variable is used by the Bootloader/Bootloadable components to schedule which
* application will be started after a software reset.
*******************************************************************************/
#if (CY_PSOC4)
    #if defined(__ARMCC_VERSION)
        __attribute__ ((section(".bootloaderruntype"), zero_init))
    #elif defined (__GNUC__)
        __attribute__ ((section(".bootloaderruntype")))
   #elif defined (__ICCARM__)
        #pragma location=".bootloaderruntype"
    #endif  /* defined(__ARMCC_VERSION) */
    extern volatile uint32 cyBtldrRunType;
#endif  /* (CY_PSOC4) */


/*******************************************************************************
* Gets the reason for a device reset
*******************************************************************************/
#if(CY_PSOC4)
    #define Bootloadable_RES_CAUSE_RESET_SOFT   (0x10u)
    #define Bootloadable_GET_RUN_TYPE           \
                        (((CY_GET_REG32(CYREG_RES_CAUSE) & Bootloadable_RES_CAUSE_RESET_SOFT) > 0u) \
                            ? (cyBtldrRunType) \
                            : 0u)
#else
    #define Bootloadable_GET_RUN_TYPE           (CY_GET_REG8(CYREG_RESET_SR0) & \
                                                    (Bootloadable_START_BTLDR | Bootloadable_START_APP))
#endif  /* (CY_PSOC4) */


/*******************************************************************************
* Schedule the Bootloader/Bootloadable to be run after a software reset.
*******************************************************************************/
#if(CY_PSOC4)
    #define Bootloadable_SET_RUN_TYPE(x)        (cyBtldrRunType = (x))
#else
    #define Bootloadable_SET_RUN_TYPE(x)        CY_SET_REG8(CYREG_RESET_SR0, (x))
#endif  /* (CY_PSOC4) */



/***************************************
*     Function Prototypes
***************************************/
extern void Bootloadable_Load(void) ;


/*******************************************************************************
* The following code is OBSOLETE and must not be used starting from version 1.10.
*******************************************************************************/
#define CYBTDLR_SET_RUN_TYPE(x)     Bootloadable_SET_RUN_TYPE(x)

/*******************************************************************************
* Bootloadable's declarations for in-app bootloading.
*******************************************************************************/
#define Bootloadable_MD_BTLDB_ACTIVE_0          (0x00u)

#if (CYDEV_PROJ_TYPE == CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER)
    #define Bootloadable_MAX_NUM_OF_BTLDB       (0x02u)
    #define Bootloadable_MD_BTLDB_ACTIVE_1      (0x01u)
    #define Bootloadable_MD_BTLDB_ACTIVE_NONE   (0x02u)
    #define Bootloadable_MD_SIZEOF              (64u)
    #define Bootloadable_MD_BASE_ADDR(appId)    (CYDEV_FLASH_BASE + (CYDEV_FLASH_SIZE - ((uint32)(appId) * CYDEV_FLS_ROW_SIZE) - \
                                                                        Bootloadable_MD_SIZEOF))
    #define Bootloadable_MD_BTLDB_ACTIVE_OFFSET(appId) (Bootloadable_MD_BASE_ADDR(appId) + 16u)

#else
    #define Bootloadable_MAX_NUM_OF_BTLDB       (0x01u)
#endif  /* (CYDEV_PROJ_TYPE == CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER)*/

/* Mask used to indicate starting application */
#define Bootloadable_SCHEDULE_BTLDB             (0x80u)
#define Bootloadable_SCHEDULE_BTLDR             (0x40u)
#define Bootloadable_SCHEDULE_MASK              (0xC0u)
/*******************************************************************************
* API prototypes
*******************************************************************************/
#if (CYDEV_PROJ_TYPE == CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER)
    uint8 Bootloadable_GetActiveApplication(void) CYSMALL \
          ;
    cystatus Bootloadable_SetActiveApplication(uint8 appId) CYSMALL \
             ;
#endif  /* (CYDEV_PROJ_TYPE == CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER)*/

/*******************************************************************************
* The following code is OBSOLETE and must not be used starting from version 1.20
*******************************************************************************/
#define Bootloadable_START_APP                      (0x80u)
#define Bootloadable_START_BTLDR                    (0x40u)
#define Bootloadable_META_DATA_SIZE                 (64u)
#define Bootloadable_META_APP_CHECKSUM_OFFSET       (0u)

#if(CY_PSOC3)

    #define Bootloadable_APP_ADDRESS                    uint16
    #define Bootloadable_GET_CODE_WORD(idx)             (*((uint32 CYCODE *) (idx)))

    /* Offset by 2 from 32 bit start because only 16 bits are needed */
    #define Bootloadable_META_APP_ADDR_OFFSET           (3u)
    #define Bootloadable_META_APP_BL_LAST_ROW_OFFSET    (7u)
    #define Bootloadable_META_APP_BYTE_LEN_OFFSET       (11u)
    #define Bootloadable_META_APP_RUN_TYPE_OFFSET       (15u)

#else

    #define Bootloadable_APP_ADDRESS                    uint32
    #define Bootloadable_GET_CODE_WORD(idx)             (*((uint32 *)(CYDEV_FLASH_BASE + (idx))))

    #define Bootloadable_META_APP_ADDR_OFFSET           (1u)
    #define Bootloadable_META_APP_BL_LAST_ROW_OFFSET    (5u)
    #define Bootloadable_META_APP_BYTE_LEN_OFFSET       (9u)
    #define Bootloadable_META_APP_RUN_TYPE_OFFSET       (13u)

#endif /* (CY_PSOC3) */

#define Bootloadable_META_APP_ACTIVE_OFFSET             (16u)
#define Bootloadable_META_APP_VERIFIED_OFFSET           (17u)

#define Bootloadable_META_APP_BL_BUILD_VER_OFFSET       (18u)
#define Bootloadable_META_APP_ID_OFFSET                 (20u)
#define Bootloadable_META_APP_VER_OFFSET                (22u)
#define Bootloadable_META_APP_CUST_ID_OFFSET            (24u)

#define Bootloadable_SetFlashRunType(runType)           \
                        Bootloadable_SetFlashByte(Bootloadable_MD_APP_RUN_ADDR(0), (runType))

/*******************************************************************************
* The following code is OBSOLETE and must not be used.
*
* If the obsoleted macro definitions are intended for the application, use the
* following scheme, redefine your own versions of these definitions:
*    #ifdef <OBSOLETED_DEFINE>
*        #undef  <OBSOLETED_DEFINE>
*        #define <OBSOLETED_DEFINE>      (<New Value>)
*    #endif
*
* NOTE Redefine obsoleted macro definitions with caution. They might still be
*       used in the application and their modification might lead to unexpected
*       consequences.
*******************************************************************************/
void Bootloadable_SetFlashByte(uint32 address, uint8 runType) ;
#if(CY_PSOC4)
    #define Bootloadable_SOFTWARE_RESET         CySoftwareReset()
#else
    #define Bootloadable_SOFTWARE_RESET         CySoftwareReset()
#endif  /* (CY_PSOC4) */

#if(CY_PSOC4)
    extern uint8 appRunType;
#endif  /* (CY_PSOC4) */


#endif /* CY_BOOTLOADABLE_Bootloadable_H */


/* [] END OF FILE */
