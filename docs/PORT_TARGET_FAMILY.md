# Adding A New Target Family
Adding new target family support requires creating a flash algo blob and the implementation for target family api. Target support is added to the `source/family/<mfg>/<target>` directory. At minimum, 3 files are needed. The first is `source/target/<mfg>/target_reset.c`

```c
/**
 * @file    target_reset.c
 * @brief   Target reset for the new target
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

#include "target_reset.h"
#include "swd_host.h"
#include "target_family.h"

static void target_before_init_debug(void)
{
    // any target specific sequences needed before attaching
    //	to the DAP across JTAG or SWD
    return;
}

static uint8_t target_unlock_sequence(void)
{
    // if the device can secure the flash and there is a way to
    //	erase all it should be implemented here.
    return 1;
}

static uint8_t target_set_state(TARGET_RESET_STATE state)
{
    // if a custom state machine is needed to set the TARGET_RESET_STATE state
    return 1;
}

static uint8_t security_bits_set(uint32_t addr, uint8_t *data, uint32_t size)
{
    // if there are security bits in the programmable flash region
    //	a check should be performed. This method is used when programming
    //	by drag-n-drop and should refuse to program an image requesting
    //	to set the device security. This can be performed with the debug channel
    //	if needed.
    return 0;
}

const target_family_descriptor_t g_target_family = {
    .version = kTargetFamilyVersion,
    .family_id = myFamilyID,
    .default_reset_type = kHardwareReset,
    .target_before_init_debug = target_before_init_debug,
    .target_unlock_sequence = target_unlock_sequence,
    .target_set_state = target_set_state,
    .security_bits_set = security_bits_set,
};
```

The target family api is located in `source/target/target_family.h` and target_reset file can customize the function apis according to the family specification. Family id is a combination of vendor id and an incrementing id. There are predefined family id stubs that can be used for generic reset types; `kStub_HWReset_FamilyID`, `kStub_SWVectReset_FamilyID`, and `kStub_SWSysReset_FamilyID`.

A flash algorithm blob is needed to program the target MCUs internal (or external) flash memory. This blob contains position independent functions for erasing, reading and writing to the flash controller. Flash algorithm blobs are created from the [FlashAlgo project.](https://github.com/pyocd/FlashAlgo). An example blob is shown below and would be added to `source/family/<mfg>/<targetname>/flash_blob.c`

```c
/**
 * @file    flash_blob.c
 * @brief   Flash algorithm for the new target MCU
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

#include "flash_blob.h"

static const uint32_t targetname_blob[] = {
    0xE00ABE00, /* ... */, 0x00000000,
};

/**
* List of start and size for each size of flash sector
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address start + size.
*/
static const sector_info_t sectors_info[] = {
    {0x00000000, 0x00001000},
 };

static const program_target_t flash = {
    0x200000B5, // Init
    0x2000029D, // UnInit
    0x20000029, // EraseChip
    0x20000061, // EraseSector
    0x20000121, // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000000 + 0x00000001,
        0x20000000 + 0x00000020 + 0x00000290,
        0x20000800
    },

    0x20000000 + 0x00000A00,   // mem buffer location
    0x20000000,                // location to write prog_blob in target RAM
    sizeof(targetname_blob),   // prog_blob size
    targetname_blob,           // address of prog_blob
    0x00000200                 // program_buffer_size, largest size that can be written in a single call to program page
};

```

The last required file is the target MCU description file `source/family/<mfg>/<targetname>/target.c` This file contains information about the size of ROM, RAM and sector operations needed to be performed on the target MCU while programming an image across the drag-n-drop channel.

```c
/**
 * @file    target.c
 * @brief   Target information for the target MCU
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

#include "target_config.h"

// The file flash_blob.c must only be included in target.c
#include "flash_blob.c"

target_cfg_t target_device = {
    .version                        = kTargetConfigVersion,
    .sectors_info                   = sectors_info,
    .sector_info_length             = (sizeof(sectors_info))/(sizeof(sector_info_t)),
    .flash_regions[0].start         = 0x00000000,
    .flash_regions[0].end           = 0x00200000,
    .flash_regions[0].flags         = kRegionIsDefault,
    .flash_regions[0].flash_algo    = (program_target_t *) &flash,
    .ram_regions[0].start           = 0x1fff0000,
    .ram_regions[0].end             = 0x20030000,
};
```

Complete target configuration api is located in `source/target/target_config.h`

At this point these target specific files could be added to a board build and developed.

# Supported Target Families
A HIC can target all supported families available and a post build script can modify a board's family id to point to the correct family. See [Porting board guide](PORT_BOARD.md)
