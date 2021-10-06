/**
 * @file    vfs_user.c
 * @brief   Implementation of vfs_user.h
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
 * Copyright (2019-2021) Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation.
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

#include "string.h"
#include "vfs_manager.h"
#include "util.h"
#include "psoc5lp.h"
#include "Bootloadable.h"
#include "vfs_user.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

//! @brief Constants for KitProg3 specific magic action or config files.
typedef enum {
    kBootloaderModeActionFile,  //!< Switch to KitProg Bootloader.
    kKP3ModeActionFile,         //!< Switch to KitProg3.
} cy_magic_file_t;

//! @brief Mapping from filename string to magic file enum.
typedef struct {
    const char *name;      //!< Name of the magic file, must be in 8.3 format.
    cy_magic_file_t which; //!< Enum for the file.
} cy_magic_file_info_t;

//! @brief Hook for magic files.
//!
//! This hook is intended to simplify checking for magic files. In addition to allowing support for
//! new magic files, you can also change the behaviour of or disable standard magic files.
//!
//! @param filename Name of the file that was created.
//! @param[out] do_remount Whether the caller should remount the MSD volume. Only applies if true
//!     is returned. The default is true, so if the hook does not modify this parameter and returns
//!     true, a remount is performed.
//! @retval true The hook handled the specified file. A remount will be performed if requested,
//!     but otherwise no other standard behaviour is applied.
//! @retval false The hook did not handle the file; continue with canonical behaviour.
bool vfs_user_magic_file_hook(const vfs_filename_t filename, bool *do_remount)
{
    (void) do_remount;
    // Table of magic files and their names.
    static const cy_magic_file_info_t s_cy_magic_file_info[] = {
        { "START_CBACT", kBootloaderModeActionFile  },
        { "START_KPACT", kKP3ModeActionFile         },
    };
    int32_t which_magic_file = -1;
    bool res = false;
    // Compare the new file's name to our table of magic filenames.
    for (int32_t i = 0; i < (int32_t)ARRAY_SIZE(s_cy_magic_file_info); ++i)
    {
        if (0 == memcmp((const char *)filename, s_cy_magic_file_info[i].name, sizeof(vfs_filename_t)))
        {
            which_magic_file = s_cy_magic_file_info[i].which;
        }
    }

    // Check if we matched a magic filename and handle it.
    if (which_magic_file >= 0)
    {
        switch (which_magic_file)
        {
        case (int32_t)kBootloaderModeActionFile:
            // Switch to KitProg Bootloader.
            Bootloadable_Load();
            break;
        case (int32_t)kKP3ModeActionFile:
            // Switch to KitProg3.
            SetKitProgActiveApp(KP3_MODE_BULK);
            break;
        default:
            util_assert(false);
            break;
        }

        res = true;
    }
    else
    {
        res = false;
    }
    return res;
}
