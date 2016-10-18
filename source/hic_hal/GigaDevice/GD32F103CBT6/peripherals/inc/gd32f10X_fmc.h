/**
  ******************************************************************************
  * @file    gd32f1x0_fmc.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   FMC header file of the firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_FMC_H
#define __GD32F10X_FMC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/** @addtogroup GD32F1x0_Firmware
  * @{
  */

/** @addtogroup FMC
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  FMC State
  */ 
typedef enum
{
    FMC_READY,
    FMC_BSY,
    FMC_WRPERR,
    FMC_PGERR,
    FMC_TIMEOUT_ERR
}FMC_State;

/* Exported constants --------------------------------------------------------*/
  
/** @defgroup FMC_Exported_Constants
  * @{
  */ 

/** @defgroup FMC_Interrupts 
  * @{
  */
   
#define FMC_INT_EOP                          FMC_CMR_ENDIE         /*!< End of programming interrupt source */
#define FMC_INT_ERR                          FMC_CMR_ERIE          /*!< Error interrupt source */
#define FMC_INT_B2_EOP                      ((uint32_t)0x80000400) /*!< Bank2 End of programming interrupt source */
#define FMC_INT_B2_ERR                      ((uint32_t)0x80001000) /*!< Bank2 Error interrupt source */
/**
  * @}
  */

/** @defgroup FMC_Option_Bytes_Write_Protection 
  * @{
  */
  

#define WRP_sector0                     ((uint32_t)0x00000001) /*!< Write protection of sector 0 */
#define WRP_sector1                     ((uint32_t)0x00000002) /*!< Write protection of sector 1 */
#define WRP_sector2                     ((uint32_t)0x00000004) /*!< Write protection of sector 2 */
#define WRP_sector3                     ((uint32_t)0x00000008) /*!< Write protection of sector 3 */
#define WRP_sector4                     ((uint32_t)0x00000010) /*!< Write protection of sector 4 */
#define WRP_sector5                     ((uint32_t)0x00000020) /*!< Write protection of sector 5 */
#define WRP_sector6                     ((uint32_t)0x00000040) /*!< Write protection of sector 6 */
#define WRP_sector7                     ((uint32_t)0x00000080) /*!< Write protection of sector 7 */
#define WRP_sector8                     ((uint32_t)0x00000100) /*!< Write protection of sector 8 */
#define WRP_sector9                     ((uint32_t)0x00000200) /*!< Write protection of sector 9 */
#define WRP_sector10                    ((uint32_t)0x00000400) /*!< Write protection of sector 10 */
#define WRP_sector11                    ((uint32_t)0x00000800) /*!< Write protection of sector 11 */
#define WRP_sector12                    ((uint32_t)0x00001000) /*!< Write protection of sector 12 */
#define WRP_sector13                    ((uint32_t)0x00002000) /*!< Write protection of sector 13 */
#define WRP_sector14                    ((uint32_t)0x00004000) /*!< Write protection of sector 14 */
#define WRP_sector15                    ((uint32_t)0x00008000) /*!< Write protection of sector 15 */
#define WRP_sector16                    ((uint32_t)0x00010000) /*!< Write protection of sector 16 */
#define WRP_sector17                    ((uint32_t)0x00020000) /*!< Write protection of sector 17 */
#define WRP_sector18                    ((uint32_t)0x00040000) /*!< Write protection of sector 18 */
#define WRP_sector19                    ((uint32_t)0x00080000) /*!< Write protection of sector 19 */
#define WRP_sector20                    ((uint32_t)0x00100000) /*!< Write protection of sector 20 */
#define WRP_sector21                    ((uint32_t)0x00200000) /*!< Write protection of sector 21 */
#define WRP_sector22                    ((uint32_t)0x00400000) /*!< Write protection of sector 22 */
#define WRP_sector23                    ((uint32_t)0x00800000) /*!< Write protection of sector 23 */
#define WRP_sector24                    ((uint32_t)0x01000000) /*!< Write protection of sector 24 */
#define WRP_sector25                    ((uint32_t)0x02000000) /*!< Write protection of sector 25 */
#define WRP_sector26                    ((uint32_t)0x04000000) /*!< Write protection of sector 26 */
#define WRP_sector27                    ((uint32_t)0x08000000) /*!< Write protection of sector 27 */
#define WRP_sector28                    ((uint32_t)0x10000000) /*!< Write protection of sector 28 */
#define WRP_sector29                    ((uint32_t)0x20000000) /*!< Write protection of sector 29 */
#define WRP_sector30                    ((uint32_t)0x40000000) /*!< Write protection of sector 30 */
#define WRP_sector31                    ((uint32_t)0x80000000) /*!< Write protection of sector 31 */

#define WRP_Allsectors                  ((uint32_t)0xFFFFFFFF) /*!< Write protection of all Sectors */

/**
  * @}
  */

/** @defgroup FMC_Option_Bytes_Read_Protection 
  * @{
  */ 


/** 
  * @brief  FMC_Read Protection Level  
  */ 
#define RDP_Level_0                     ((uint8_t)0xA5)
#define RDP_Level_1                     ((uint8_t)0xBB)

/**
  * @}
  */ 

/** @defgroup FMC_Option_Bytes_IWatchdog 
  * @{
  */
#define OB_IWDG_SW                      ((uint8_t)0x01)  /*!< Software IWDG selected */
#define OB_IWDG_HW                      ((uint8_t)0x00)  /*!< Hardware IWDG selected */

/**
  * @}
  */

/** @defgroup FMC_Option_Bytes_nRST_DEEPSLEEP 
  * @{
  */
#define OB_DEEPSLEEP_NoRST              ((uint8_t)0x02) /*!< No reset generated when entering in DEEPSLEEP */
#define OB_DEEPSLEEP_RST                ((uint8_t)0x00) /*!< Reset generated when entering in DEEPSLEEP */

/**
  * @}
  */

/** @defgroup FMC_Option_Bytes_nRST_STDBY 
  * @{
  */
#define OB_STDBY_NoRST                  ((uint8_t)0x04) /*!< No reset generated when entering in STANDBY */
#define OB_STDBY_RST                    ((uint8_t)0x00) /*!< Reset generated when entering in STANDBY */

/**
  * @}
  */

/** @defgroup FMC_Option_Bytes_BOOT
  * @{
  */
#define OB_BOOT_B1                  ((uint8_t)0x08)      /*!< BOOT from Bank1 */
#define OB_BOOT_B2                  ((uint8_t)0x00)      /*!< BOOT from Bank2 */
#define OB_USER_BFB2                 ((uint16_t)0x0008)   /*!< Configure BOOT from Bank1 */
/**
  * @}
  */



/** @defgroup FMC_Flags 
  * @{
  */ 

#define FMC_FLAG_BSY                    FMC_CSR_BUSY             /*!< FMC Busy flag */
#define FMC_FLAG_PERR                   FMC_CSR_PGEF             /*!< FMC Programming error flag */
#define FMC_FLAG_WERR                   FMC_CSR_WPEF             /*!< FMC Write protected error flag */
#define FMC_FLAG_EOP                    FMC_CSR_ENDF             /*!< FMC End of Programming flag */
#define FMC_FLAG_OPTERR                 ((uint32_t)0x00000001)   /*!< FMC Option Byte error flag */
#define FMC_FLAG_B2_BSY                 ((uint32_t)0x80000001)   /*!< FMC Busy flag */
#define FMC_FLAG_B2_PERR                ((uint32_t)0x80000004)   /*!< FMC Programming error flag */
#define FMC_FLAG_B2_WERR                ((uint32_t)0x80000010)   /*!< FMC Write protected error flag */
#define FMC_FLAG_B2_EOP                 ((uint32_t)0x80000020)   /*!< FMC End of Programming flag */
/**
  * @}
  */ 

/** @defgroup FMC_Timeout_definition 
  * @{
  */ 
#define FMC_TIMEOUT_COUNT               ((uint32_t)0x000F0000)
/**
  * @}
  */ 
/* FMC BANK address */
#define FMC_B1_END_ADDRESS    ((uint32_t)0x807FFFF)

/**
  * @}
  */ 

/** @defgroup FMC_Exported_Functions
  * @{
  */

/** 
  * @brief  FMC memory functions.
  */
/* FMC Main Memory Programming functions *****************************************/
void FMC_Unlock(void);
void FMC_UnlockB1(void);
void FMC_UnlockB2(void);
void FMC_Lock(void);
void FMC_LockB1(void);
void FMC_LockB2(void);
FMC_State FMC_ErasePage(uint32_t Page_Address);
FMC_State FMC_MassErase(void);
FMC_State FMC_MassB1Erase(void);
FMC_State FMC_MassB2Erase(void);
FMC_State FMC_ProgramWord(uint32_t Address, uint32_t Data);

/* FMC Option Bytes Programming functions *****************************************/
void FMC_OB_Unlock(void);
void FMC_OB_Lock(void);
void FMC_OB_Reset(void);
FMC_State FMC_OB_Erase(void);
FMC_State FMC_OB_EnableWRP(uint32_t OB_WRP);
FMC_State FMC_ReadOutProtection(TypeState NewValue);
FMC_State FMC_OB_RDPConfig(uint8_t OB_RDP);
FMC_State FMC_OB_UserConfig(uint8_t OB_IWDG, uint8_t OB_DEEPSLEEP, uint8_t OB_STDBY);
FMC_State FMC_OB_BOOTConfig(uint8_t OB_BOOT);
FMC_State FMC_OB_WriteUser(uint8_t OB_USER);
FMC_State FMC_ProgramOptionByteData(uint32_t Address, uint8_t Data);
uint8_t FMC_OB_GetUser(void);
uint32_t FMC_OB_GetWRP(void);
TypeState FMC_OB_GetRDP(void);

/* FMC Interrupts and flags management functions **********************************/
void FMC_INTConfig(uint32_t FMC_INT, TypeState NewValue);
TypeState FMC_GetBitState(uint32_t FMC_FLAG);
void FMC_ClearBitState(uint32_t FMC_FLAG);
FMC_State FMC_GetState(void);
FMC_State FMC_GetB1State(void);
FMC_State FMC_GetB2State(void);
FMC_State FMC_WaitReady(uint32_t uCount);
FMC_State FMC_B1_WaitReady(uint32_t uCount);
FMC_State FMC_B2_WaitReady(uint32_t uCount);

#ifdef __cplusplus
}
#endif

#endif /* __GD32F1X0_FMC_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT GIGADEVICE *****END OF FILE****/
