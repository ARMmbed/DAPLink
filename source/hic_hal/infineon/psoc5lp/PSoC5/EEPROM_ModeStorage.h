/*******************************************************************************
* File Name: EEPROM_ModeStorage.h
* Version 3.0
*
*  Description:
*   Provides the function definitions for the EEPROM APIs.
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

#if !defined(CY_EEPROM_EEPROM_ModeStorage_H)
#define CY_EEPROM_EEPROM_ModeStorage_H

#include "cydevice_trm.h"
#include "CyFlash.h"

#if !defined(CY_PSOC5LP)
    #error Component EEPROM_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void EEPROM_ModeStorage_Enable(void) ;
void EEPROM_ModeStorage_Start(void) ;
void EEPROM_ModeStorage_Stop (void) ;
cystatus EEPROM_ModeStorage_WriteByte(uint8 dataByte, uint16 address) \
                                            ;
uint8 EEPROM_ModeStorage_ReadByte(uint16 address) ;
uint8 EEPROM_ModeStorage_UpdateTemperature(void) ;
cystatus EEPROM_ModeStorage_EraseSector(uint8 sectorNumber) ;
cystatus EEPROM_ModeStorage_Write(const uint8 * rowData, uint8 rowNumber) ;
cystatus EEPROM_ModeStorage_StartWrite(const uint8 * rowData, uint8 rowNumber) \
                                                ;
cystatus EEPROM_ModeStorage_StartErase(uint8 sectorNumber) ;
cystatus EEPROM_ModeStorage_Query(void) ;
cystatus EEPROM_ModeStorage_ByteWritePos(uint8 dataByte, uint8 rowNumber, uint8 byteNumber) \
                                                ;


/****************************************
*           API Constants
****************************************/

#define EEPROM_ModeStorage_EEPROM_SIZE            CYDEV_EE_SIZE
#define EEPROM_ModeStorage_SPC_BYTE_WRITE_SIZE    (0x01u)

#define EEPROM_ModeStorage_SECTORS_NUMBER         (CYDEV_EE_SIZE / CYDEV_EEPROM_SECTOR_SIZE)

#define EEPROM_ModeStorage_AHB_REQ_SHIFT          (0x00u)
#define EEPROM_ModeStorage_AHB_REQ                ((uint8)(0x01u << EEPROM_ModeStorage_AHB_REQ_SHIFT))
#define EEPROM_ModeStorage_AHB_ACK_SHIFT          (0x01u)
#define EEPROM_ModeStorage_AHB_ACK_MASK           ((uint8)(0x01u << EEPROM_ModeStorage_AHB_ACK_SHIFT))


/***************************************
* Registers
***************************************/
#define EEPROM_ModeStorage_SPC_EE_SCR_REG                 (*(reg8 *) CYREG_SPC_EE_SCR)
#define EEPROM_ModeStorage_SPC_EE_SCR_PTR                 ( (reg8 *) CYREG_SPC_EE_SCR)



/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/
#define EEPROM_ModeStorage_ByteWrite                  EEPROM_ModeStorage_ByteWritePos
#define EEPROM_ModeStorage_QueryWrite                 EEPROM_ModeStorage_Query

#endif /* CY_EEPROM_EEPROM_ModeStorage_H */

/* [] END OF FILE */
