/**
 * @file    usbd_core_cdc.h
 * @brief   USB Device Core CDC header
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

#ifndef __USBD_CORE_CDC_H__
#define __USBD_CORE_CDC_H__


/*--------------------------- Core overridable class specific functions ------*/

extern BOOL USBD_EndPoint0_Setup_CDC_ReqToIF(void);
extern BOOL USBD_EndPoint0_Out_CDC_ReqToIF(void);


#endif  /* __USBD_CORE_CDC_H__ */
