/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _FSL_IAP_SKBOOT_AUTHENTICATE_H_
#define _FSL_IAP_SKBOOT_AUTHENTICATE_H_

#include "fsl_common.h"

/*!
 * @addtogroup skboot_authenticate
 * @{
 */

/*******************************************************************************
 * Definitions
 *******************************************************************************/

typedef enum _skboot_status
{
    kStatus_SKBOOT_Success                            = 0x5ac3c35au,
    kStatus_SKBOOT_Fail                               = 0xc35ac35au,
    kStatus_SKBOOT_InvalidArgument                    = 0xc35a5ac3u,
    kStatus_SKBOOT_KeyStoreMarkerInvalid              = 0xc3c35a5au,
    kStatus_SKBOOT_HashcryptFinishedWithStatusSuccess = 0xc15a5ac3,
    kStatus_SKBOOT_HashcryptFinishedWithStatusFail    = 0xc15a5acb,
} skboot_status_t;

typedef enum _secure_bool
{
    kSECURE_TRUE                       = 0xc33cc33cU,
    kSECURE_FALSE                      = 0x5aa55aa5U,
    kSECURE_CALLPROTECT_SECURITY_FLAGS = 0xc33c5aa5U,
    kSECURE_CALLPROTECT_IS_APP_READY   = 0x5aa5c33cU,
    kSECURE_TRACKER_VERIFIED           = 0x55aacc33U,
} secure_bool_t;

/*******************************************************************************
 * Externs
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Authenticate entry function with ARENA allocator init
 *
 * This is called by ROM boot or by ROM API g_skbootAuthenticateInterface
 */
skboot_status_t skboot_authenticate(const uint8_t *imageStartAddr, secure_bool_t *isSignVerified);

/*!
 * @brief Interface for image authentication API
 */
void HASH_IRQHandler(void);

#if defined(__cplusplus)
}
#endif

/*!
 *@}
 */

#endif /* _FSL_IAP_SKBOOT_AUTHENTICATE_H_ */
