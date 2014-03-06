/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
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
 */
 
#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

// Each board should have a unique ID and secret. For information 
//  about obtaining a secret contact support@mbed.org
#if   defined (TARGET_MK20D5)
#define BOARD_ID        "ffff"
#define BOARD_SECRET    "00000000"
// these need to move into a target specific file if cant be had from the linker
//TODO: find symbols from the Linker to do this
#define FLASH_START	    0x410
#define FLASH_END	    0x1FFFFFFF
#define RAM_START       0x1FFFE000
#define RAM_END	        0x20002000
#define SECTOR_SIZE     1024
#define FLASH_SIZE_KB   (128-5)

#elif defined (TARGET_LPC11U35)
#define BOARD_ID        "fffe"
#define BOARD_SECRET    "00000000"
#endif

#if !defined(BOARD_SECRET) || !defined(BOARD_ID)
// TODO: how to check a string value using the preprocessor...
#error contact support@mbed.org to obtain a secret and board ID
#define BOARD_ID        "0000"
#define BOARD_SECRET    "xxxxxxxx"

#endif

typedef struct {
    const uint8_t  id[5];
    const uint8_t  secret[9];
} BOARD;

extern BOARD board;

#endif
