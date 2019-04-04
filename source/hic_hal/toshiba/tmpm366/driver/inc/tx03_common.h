/**
 *******************************************************************************
 * @file    tx03_common.h
 * @brief   All common macro and definition for TX03 peripheral drivers
 * @version V2.1.1
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
#ifndef __TX03_COMMON_H
#define __TX03_COMMON_H

typedef enum {
    SUCCESS = 0U,
    ERROR = 1U
} Result;

typedef enum {
    BUSY = 0U,
    DONE = 1U
} WorkState;

typedef enum {
    DISABLE = 0U,
    ENABLE = 1U
} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

#define IS_POINTER_NOT_NULL(param) ((void*)(param) != (void*)0)

/* 
 * To report the name of the source file and source line number where the
 * assert_param error has occurred, "DEBUG" must be defined. And detailed
 * definition of assert_failed() is needed to be implemented, which can be
 * done, for example, in the main.c file.
 */
#ifdef DEBUG
void assert_failed(char *file, int32_t line);
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((char *)__FILE__, __LINE__))
#else
#define assert_param(expr)
#endif                          /* DEBUG */

#endif                          /* __TX03_COMMON_H */
