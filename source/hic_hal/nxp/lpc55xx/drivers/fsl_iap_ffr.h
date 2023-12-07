/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __FSL_IAP_FFR_H_
#define __FSL_IAP_FFR_H_

#include "fsl_iap.h"

/*!
 * @addtogroup flash_ifr_driver
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @name Flash IFR version
 * @{
 */
/*! @brief Flash IFR driver version for SDK*/
#define FSL_FLASH_IFR_DRIVER_VERSION (MAKE_VERSION(2, 1, 0)) /*!< Version 2.1.0. */
/*@}*/

/*! @brief Alignment(down) utility. */
#if !defined(ALIGN_DOWN)
#define ALIGN_DOWN(x, a) ((x) & (uint32_t)(-((int32_t)(a))))
#endif

/*! @brief Alignment(up) utility. */
#if !defined(ALIGN_UP)
#define ALIGN_UP(x, a) (-((int32_t)((uint32_t)(-((int32_t)(x))) & (uint32_t)(-((int32_t)(a))))))
#endif

#define FLASH_FFR_MAX_PAGE_SIZE    (512u)
#define FLASH_FFR_HASH_DIGEST_SIZE (32u)
#define FLASH_FFR_IV_CODE_SIZE     (52u)

/*! @brief flash ffr page offset. */
enum _flash_ffr_page_offset
{
    kFfrPageOffset_CFPA         = 0, /*!< Customer In-Field programmed area*/
    kFfrPageOffset_CFPA_Scratch = 0, /*!< CFPA Scratch page */
    kFfrPageOffset_CFPA_Cfg     = 1, /*!< CFPA Configuration area (Ping page)*/
    kFfrPageOffset_CFPA_CfgPong = 2, /*!< Same as CFPA page (Pong page)*/

    kFfrPageOffset_CMPA     = 3, /*!< Customer Manufacturing programmed area*/
    kFfrPageOffset_CMPA_Cfg = 3, /*!< CMPA Configuration area (Part of CMPA)*/
    kFfrPageOffset_CMPA_Key = 4, /*!< Key Store area (Part of CMPA)*/

    kFfrPageOffset_NMPA        = 7,  /*!< NXP Manufacturing programmed area*/
    kFfrPageOffset_NMPA_Romcp  = 7,  /*!< ROM patch area (Part of NMPA)*/
    kFfrPageOffset_NMPA_Repair = 9,  /*!< Repair area (Part of NMPA)*/
    kFfrPageOffset_NMPA_Cfg    = 15, /*!< NMPA configuration area (Part of NMPA)*/
    kFfrPageOffset_NMPA_End    = 16, /*!< Reserved (Part of NMPA)*/
};

/*! @brief flash ffr page number. */
enum _flash_ffr_page_num
{
    kFfrPageNum_CFPA = 3,  /*!< Customer In-Field programmed area*/
    kFfrPageNum_CMPA = 4,  /*!< Customer Manufacturing programmed area*/
    kFfrPageNum_NMPA = 10, /*!< NXP Manufacturing programmed area*/

    kFfrPageNum_CMPA_Cfg   = 1,
    kFfrPageNum_CMPA_Key   = 3,
    kFfrPageNum_NMPA_Romcp = 2,

    kFfrPageNum_SpecArea = kFfrPageNum_CFPA + kFfrPageNum_CMPA,
    kFfrPageNum_Total    = (kFfrPageNum_CFPA + kFfrPageNum_CMPA + kFfrPageNum_NMPA),
};

enum _flash_ffr_block_size
{
    kFfrBlockSize_Key            = 52u,
    kFfrBlockSize_ActivationCode = 1192u,
};

typedef enum _cfpa_cfg_cmpa_prog_process
{
    kFfrCmpaProgProcess_Pre  = 0x0u,
    kFfrCmpaProgProcess_Post = 0xFFFFFFFFu,
} cmpa_prog_process_t;

typedef struct _cfpa_cfg_iv_code
{
    uint32_t keycodeHeader;
    uint8_t reserved[FLASH_FFR_IV_CODE_SIZE];
} cfpa_cfg_iv_code_t;

typedef struct _cfpa_cfg_info
{
    uint32_t header;                          /*!< [0x000-0x003] */
    uint32_t version;                         /*!< [0x004-0x007 */
    uint32_t secureFwVersion;                 /*!< [0x008-0x00b */
    uint32_t nsFwVersion;                     /*!< [0x00c-0x00f] */
    uint32_t imageKeyRevoke;                  /*!< [0x010-0x013] */
    uint8_t reserved0[4];                     /*!< [0x014-0x017] */
    uint32_t rotkhRevoke;                     /*!< [0x018-0x01b] */
    uint32_t vendorUsage;                     /*!< [0x01c-0x01f] */
    uint32_t dcfgNsPin;                       /*!< [0x020-0x013] */
    uint32_t dcfgNsDflt;                      /*!< [0x024-0x017] */
    uint32_t enableFaMode;                    /*!< [0x028-0x02b] */
    uint8_t reserved1[4];                     /*!< [0x02c-0x02f] */
    cfpa_cfg_iv_code_t ivCodePrinceRegion[3]; /*!< [0x030-0x0d7] */
    uint8_t reserved2[264];                   /*!< [0x0d8-0x1df] */
    uint8_t sha256[32];                       /*!< [0x1e0-0x1ff] */
} cfpa_cfg_info_t;

#define FFR_BOOTCFG_BOOTSPEED_MASK  (0x18U)
#define FFR_BOOTCFG_BOOTSPEED_SHIFT (7U)
#define FFR_BOOTCFG_BOOTSPEED_48MHZ (0x0U)
#define FFR_BOOTCFG_BOOTSPEED_96MHZ (0x1U)

#define FFR_USBID_VENDORID_MASK   (0xFFFFU)
#define FFR_USBID_VENDORID_SHIFT  (0U)
#define FFR_USBID_PRODUCTID_MASK  (0xFFFF0000U)
#define FFR_USBID_PRODUCTID_SHIFT (16U)

typedef struct _cmpa_cfg_info
{
    uint32_t bootCfg;     /*!< [0x000-0x003] */
    uint32_t spiFlashCfg; /*!< [0x004-0x007] */
    struct
    {
        uint16_t vid;
        uint16_t pid;
    } usbId;                 /*!< [0x008-0x00b] */
    uint32_t sdioCfg;        /*!< [0x00c-0x00f] */
    uint32_t dcfgPin;        /*!< [0x010-0x013] */
    uint32_t dcfgDflt;       /*!< [0x014-0x017] */
    uint32_t dapVendorUsage; /*!< [0x018-0x01b] */
    uint32_t secureBootCfg;  /*!< [0x01c-0x01f] */
    uint32_t princeBaseAddr; /*!< [0x020-0x023] */
    uint32_t princeSr[3];    /*!< [0x024-0x02f] */
    uint8_t reserved0[32];   /*!< [0x030-0x04f] */
    uint32_t rotkh[8];       /*!< [0x050-0x06f] */
    uint8_t reserved1[368];  /*!< [0x070-0x1df] */
    uint8_t sha256[32];      /*!< [0x1e0-0x1ff] */
} cmpa_cfg_info_t;

typedef struct _cmpa_key_store_header
{
    uint32_t header;
    uint8_t reserved[4];
} cmpa_key_store_header_t;

#define FFR_SYSTEM_SPEED_CODE_MASK             (0x3U)
#define FFR_SYSTEM_SPEED_CODE_SHIFT            (0U)
#define FFR_SYSTEM_SPEED_CODE_FRO12MHZ_12MHZ   (0x0U)
#define FFR_SYSTEM_SPEED_CODE_FROHF96MHZ_24MHZ (0x1U)
#define FFR_SYSTEM_SPEED_CODE_FROHF96MHZ_48MHZ (0x2U)
#define FFR_SYSTEM_SPEED_CODE_FROHF96MHZ_96MHZ (0x3U)

#define FFR_PERIPHERALCFG_PERI_MASK    (0x7FFFFFFFU)
#define FFR_PERIPHERALCFG_PERI_SHIFT   (0U)
#define FFR_PERIPHERALCFG_COREEN_MASK  (0x10000000U)
#define FFR_PERIPHERALCFG_COREEN_SHIFT (31U)

typedef struct _nmpa_cfg_info
{
    uint16_t fro32kCfg;   /*!< [0x000-0x001] */
    uint8_t reserved0[6]; /*!< [0x002-0x007] */
    uint8_t sysCfg;       /*!< [0x008-0x008] */
    uint8_t reserved1[7]; /*!< [0x009-0x00f] */
    struct
    {
        uint32_t data;
        uint32_t reserved[3];
    } GpoInitData[3];              /*!< [0x010-0x03f] */
    uint32_t GpoDataChecksum[4];   /*!< [0x040-0x04f] */
    uint32_t finalTestBatchId[4];  /*!< [0x050-0x05f] */
    uint32_t deviceType;           /*!< [0x060-0x063] */
    uint32_t finalTestProgVersion; /*!< [0x064-0x067] */
    uint32_t finalTestDate;        /*!< [0x068-0x06b] */
    uint32_t finalTestTime;        /*!< [0x06c-0x06f] */
    uint32_t uuid[4];              /*!< [0x070-0x07f] */
    uint8_t reserved2[32];         /*!< [0x080-0x09f] */
    uint32_t peripheralCfg;        /*!< [0x0a0-0x0a3] */
    uint32_t ramSizeCfg;           /*!< [0x0a4-0x0a7] */
    uint32_t flashSizeCfg;         /*!< [0x0a8-0x0ab] */
    uint8_t reserved3[36];         /*!< [0x0ac-0x0cf] */
    uint8_t fro1mCfg;              /*!< [0x0d0-0x0d0] */
    uint8_t reserved4[15];         /*!< [0x0d1-0x0df] */
    uint32_t dcdc[4];              /*!< [0x0e0-0x0ef] */
    uint32_t bod;                  /*!< [0x0f0-0x0f3] */
    uint8_t reserved5[12];         /*!< [0x0f4-0x0ff] */
    uint8_t calcHashReserved[192]; /*!< [0x100-0x1bf] */
    uint8_t sha256[32];            /*!< [0x1c0-0x1df] */
    uint32_t ecidBackup[4];        /*!< [0x1e0-0x1ef] */
    uint32_t pageChecksum[4];      /*!< [0x1f0-0x1ff] */
} nmpa_cfg_info_t;

typedef struct _ffr_key_store
{
    uint8_t reserved[3][FLASH_FFR_MAX_PAGE_SIZE];
} ffr_key_store_t;

typedef enum _ffr_key_type
{
    kFFR_KeyTypeSbkek         = 0x00U,
    kFFR_KeyTypeUser          = 0x01U,
    kFFR_KeyTypeUds           = 0x02U,
    kFFR_KeyTypePrinceRegion0 = 0x03U,
    kFFR_KeyTypePrinceRegion1 = 0x04U,
    kFFR_KeyTypePrinceRegion2 = 0x05U,
} ffr_key_type_t;

typedef enum _ffr_bank_type
{
    kFFR_BankTypeBank0_NMPA = 0x00U,
    kFFR_BankTypeBank1_CMPA = 0x01U,
    kFFR_BankTypeBank2_CFPA = 0x02U
} ffr_bank_type_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name FFR APIs
 * @{
 */

/*!
 * @brief Initializes the global FFR properties structure members.
 *
 * @param config A pointer to the storage for the driver runtime state.
 *
 * @retval #kStatus_FLASH_Success API was executed successfully.
 * @retval #kStatus_FLASH_InvalidArgument An invalid argument is provided.
 */
status_t FFR_Init(flash_config_t *config);

/*!
 * @brief Enable firewall for all flash banks.
 *
 * CFPA, CMPA, and NMPA flash areas region will be locked, After this function executed;
 * Unless the board is reset again.
 *
 * @param config A pointer to the storage for the driver runtime state.
 *
 * @retval #kStatus_FLASH_Success An invalid argument is provided.
 * @retval #kStatus_FLASH_InvalidArgument An invalid argument is provided.
 */
status_t FFR_Lock_All(flash_config_t *config);

/*!
 * @brief APIs to access CFPA pages
 *
 * This routine will erase CFPA and program the CFPA page with passed data.
 *
 * @param config A pointer to the storage for the driver runtime state.
 * @param page_data A pointer to the source buffer of data that is to be programmed
 *        into the CFPA.
 * @param valid_len The length, given in bytes, to be programmed.
 *
 * @retval #kStatus_FLASH_Success The desire page-data were programed successfully into CFPA.
 * @retval #kStatus_FLASH_InvalidArgument An invalid argument is provided.
 * @retval kStatus_FTFx_AddressError Address is out of range.
 * @retval #kStatus_FLASH_FfrBankIsLocked The CFPA was locked.
 * @retval #kStatus_FLASH_OutOfDateCfpaPage It is not newest CFPA page.
 */
status_t FFR_InfieldPageWrite(flash_config_t *config, uint8_t *page_data, uint32_t valid_len);

/*!
 * @brief APIs to access CFPA pages
 *
 * Generic read function, used by customer to read data stored in 'Customer In-field Page'.
 *
 * @param config A pointer to the storage for the driver runtime state.
 * @param pData A pointer to the dest buffer of data that is to be read from 'Customer In-field Page'.
 * @param offset An offset from the 'Customer In-field Page' start address.
 * @param len The length, given in bytes, to be read.
 *
 * @retval #kStatus_FLASH_Success Get data from 'Customer In-field Page'.
 * @retval #kStatus_FLASH_InvalidArgument An invalid argument is provided.
 * @retval kStatus_FTFx_AddressError Address is out of range.
 * @retval #kStatus_FLASH_CommandFailure access error.
 */
status_t FFR_GetCustomerInfieldData(flash_config_t *config, uint8_t *pData, uint32_t offset, uint32_t len);

/*!
 * @brief APIs to access CMPA pages
 *
 * This routine will erase "customer factory page" and program the page with passed data.
 * If 'seal_part' parameter is TRUE then the routine will compute SHA256 hash of
 * the page contents and then programs the pages.
 * 1.During development customer code uses this API with 'seal_part' set to FALSE.
 * 2.During manufacturing this parameter should be set to TRUE to seal the part
 * from further modifications
 * 3.This routine checks if the page is sealed or not. A page is said to be sealed if
 * the SHA256 value in the page has non-zero value. On boot ROM locks the firewall for
 * the region if hash is programmed anyways. So, write/erase commands will fail eventually.
 *
 * @param config A pointer to the storage for the driver runtime state.
 * @param page_data A pointer to the source buffer of data that is to be programmed
 *        into the "customer factory page".
 * @param seal_part Set fasle for During development customer code.
 *
 * @retval #kStatus_FLASH_Success The desire page-data were programed successfully into CMPA.
 * @retval #kStatus_FLASH_InvalidArgument Parameter is not aligned with the specified baseline.
 * @retval kStatus_FTFx_AddressError Address is out of range.
 * @retval #kStatus_FLASH_CommandFailure access error.
 */
status_t FFR_CustFactoryPageWrite(flash_config_t *config, uint8_t *page_data, bool seal_part);

/*!
 * @brief APIs to access CMPA page
 *
 * Read data stored in 'Customer Factory CFG Page'.
 *
 * @param config A pointer to the storage for the driver runtime state.
 * @param pData A pointer to the dest buffer of data that is to be read
 *            from the Customer Factory CFG Page.
 * @param offset Address offset relative to the CMPA area.
 * @param len The length, given in bytes to be read.
 *
 * @retval #kStatus_FLASH_Success Get data from 'Customer Factory CFG Page'.
 * @retval #kStatus_FLASH_InvalidArgument Parameter is not aligned with the specified baseline.
 * @retval kStatus_FTFx_AddressError Address is out of range.
 * @retval #kStatus_FLASH_CommandFailure access error.
 */
status_t FFR_GetCustomerData(flash_config_t *config, uint8_t *pData, uint32_t offset, uint32_t len);

/*!
 * @brief APIs to access CMPA page
 *
 * 1.SW should use this API routine to get the UUID of the chip.
 * 2.Calling routine should pass a pointer to buffer which can hold 128-bit value.
 */
status_t FFR_GetUUID(flash_config_t *config, uint8_t *uuid);

/*!
 * @brief This routine writes the 3 pages allocated for Key store data,
 *
 * 1.Used during manufacturing. Should write pages when 'customer factory page' is not in sealed state.
 * 2.Optional routines to set individual data members (activation code, key codes etc) to construct
 * the key store structure in RAM before committing it to IFR/FFR.
 *
 * @param config A pointer to the storage for the driver runtime state.
 * @param pKeyStore A Pointer to the 3 pages allocated for Key store data.
 *        that will be written to 'customer factory page'.
 *
 * @retval #kStatus_FLASH_Success The key were programed successfully into FFR.
 * @retval #kStatus_FLASH_InvalidArgument Parameter is not aligned with the specified baseline.
 * @retval kStatus_FTFx_AddressError Address is out of range.
 * @retval #kStatus_FLASH_CommandFailure access error.
 */
status_t FFR_KeystoreWrite(flash_config_t *config, ffr_key_store_t *pKeyStore);

/*!
 * @brief Get/Read Key store code routines
 *
 * 1. Calling code should pass buffer pointer which can hold activation code 1192 bytes.
 * 2. Check if flash aperture is small or regular and read the data appropriately.
 */
status_t FFR_KeystoreGetAC(flash_config_t *config, uint8_t *pActivationCode);

/*!
 * @brief Get/Read Key store code routines
 *
 * 1. Calling code should pass buffer pointer which can hold key code 52 bytes.
 * 2. Check if flash aperture is small or regular and read the data appropriately.
 * 3. keyIndex specifies which key code is read.
 */
status_t FFR_KeystoreGetKC(flash_config_t *config, uint8_t *pKeyCode, ffr_key_type_t keyIndex);

/*@}*/

#ifdef __cplusplus
}
#endif

/*@}*/

#endif /*! __FSL_FLASH_FFR_H_ */
