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

#ifndef VIRTUAL_FS_USER_H
#define VIRTUAL_FS_USER_H

#include <stdint.h>
#include <stdbool.h>

#include "virtual_fs.h"
#include "error.h"

#ifdef __cplusplus
  extern "C" {
#endif

extern const vfs_filename_t daplink_mode_file_name;
extern const vfs_filename_t daplink_drive_name;
extern const vfs_filename_t daplink_url_name;
extern const char * const daplink_target_url;


/* Callable from anywhere */

// Enable or disable the virtual filesystem
void vfs_user_enable(bool enabled);

// Remount the virtual filesystem
void vfs_user_remount(void);


/* Callable only from the thread running virtual_fs_user.h */

// Initialize the VFS user code
// Must be called after USB has been initialized (usbd_init())
// Notes: Must only be called from the thread runnning USB
void vfs_user_init(bool enabled);

// Run the vfs_user state machine
// Notes: Must only be called from the thread runnning USB
void vfs_user_periodic(uint32_t elapsed_ms);


#ifdef __cplusplus
}
#endif

#endif
