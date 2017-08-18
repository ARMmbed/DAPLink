/**
 * @file    usbd_lib_dfu.h
 * @brief   USB Device DFU Library header
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
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
 */

#ifndef __USBD_LIB_DFU_H__
#define __USBD_LIB_DFU_H__

/*--------------------------- DFU error/status API ---------------------------*/
extern void DFU_SetStatus(U8 status);
extern void DFU_Reset(void);

/*--------------------------- User Hooks -------------------------------------*/

extern BOOL USBD_DFU_StartUpgrade(void);
extern BOOL USBD_DFU_FinishUpgrade(void);
extern BOOL USBD_DFU_WriteBlock(const U8 *buffer, U16 blockSize);
extern BOOL USBD_DFU_HandleAbort(BOOL error);
extern U32 USBD_DFU_GetInitTimeout(void);
extern U32 USBD_DFU_GetDownloadTimeout(uint16_t blockSize);
extern U32 USBD_DFU_GetManifestTimeout(void);

/*--------------------------- USB Requests -----------------------------------*/

extern BOOL USBD_DFU_GetStatus(void);
extern void USBD_DFU_GetStatusAction(void);
extern BOOL USBD_DFU_GetState(void);
extern BOOL USBD_DFU_StartDnload(U16 transferSize);
extern BOOL USBD_DFU_SyncDnload(void);
extern BOOL USBD_DFU_Abort(void);
extern BOOL USBD_DFU_ClrStatus(void);

#endif  /* __USBD_LIB_DFU_H__ */
