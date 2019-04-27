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
    .family_id = myFamilyID,
    .default_reset_type = kHardwareReset,
    .target_before_init_debug = target_before_init_debug,
    .target_unlock_sequence = target_unlock_sequence,
    .target_set_state = target_set_state,
    .security_bits_set = security_bits_set,
};
```

The target family api is located in `source/target/target_family.h` and target_reset file can customize the function apis according to the family specification. Family id is a combination of vendor id and an incrementing id. There are predefined family id stubs that can be used for generic reset types; kStub_HWReset_FamilyID, kStub_SWVectReset_FamilyID and kStub_SWSysReset_FamilyID.

```c
/**
 * @file    target_family.h
 * @brief   
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2018-2019, ARM Limited, All Rights Reserved
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

#ifndef TARGET_FAMILY_H
#define TARGET_FAMILY_H

#include <stdint.h>
#include <string.h>
#include "target_reset.h"

#define VENDOR_TO_FAMILY(x, y) (x<<8 | y)

typedef enum _reset_type { 
    kHardwareReset=1, 
    kSoftwareReset, 
} reset_type_t; 

enum _vendor_ids {
    kStub_VendorID = 0,
    kNXP_VendorID = 11,
    kTI_VendorID = 16,
    kNordic_VendorID = 54,
    kToshiba_VendorID = 92,
    kRenesas_VendorID = 117,
    kWiznet_VendorID = 122,
    kRealtek_VendorID = 124,
};

typedef enum _family_id {
    kStub_HWReset_FamilyID = VENDOR_TO_FAMILY(kStub_VendorID, 1),
    kStub_SWVectReset_FamilyID = VENDOR_TO_FAMILY(kStub_VendorID, 2),
    kStub_SWSysReset_FamilyID = VENDOR_TO_FAMILY(kStub_VendorID, 3),
    kNXP_KinetisK_FamilyID = VENDOR_TO_FAMILY(kNXP_VendorID, 1),
    kNXP_KinetisL_FamilyID = VENDOR_TO_FAMILY(kNXP_VendorID, 2),
    kNXP_Mimxrt_FamilyID = VENDOR_TO_FAMILY(kNXP_VendorID, 3),
    kNXP_RapidIot_FamilyID = VENDOR_TO_FAMILY(kNXP_VendorID, 4),
    kNXP_KinetisK32W_FamilyID = VENDOR_TO_FAMILY(kNXP_VendorID, 5),
    kNordic_Nrf51_FamilyID = VENDOR_TO_FAMILY(kNordic_VendorID, 1),
    kNordic_Nrf52_FamilyID = VENDOR_TO_FAMILY(kNordic_VendorID, 2),
    kRealtek_Rtl8195am_FamilyID = VENDOR_TO_FAMILY(kRealtek_VendorID, 1),
    kTI_Cc3220sf_FamilyID = VENDOR_TO_FAMILY(kTI_VendorID, 1),
    kToshiba_Tz_FamilyID = VENDOR_TO_FAMILY(kToshiba_VendorID, 1),
    kWiznet_W7500_FamilyID = VENDOR_TO_FAMILY(kWiznet_VendorID, 1),
    kRenesas_FamilyID = VENDOR_TO_FAMILY(kRenesas_VendorID, 1),
} family_id_t;
 
typedef struct target_family_descriptor { 
    uint16_t family_id;                         /*!< Use to select or identify target family from defined target family or custom ones */ 
    reset_type_t default_reset_type;            /*!< Target family can select predefined reset from  kHardwareReset and kSoftwareReset */ 
    uint32_t soft_reset_type;                   /*!< Families can override software reset type to VECTRESET or SYSRESETREQ */
    void (*target_before_init_debug)(void);     /*!< Target dependant function before debug initialization */
    void (*prerun_target_config)(void);         /*!< Target specific initialization */
    uint8_t (*target_unlock_sequence)(void);    /*!< Unlock targets that can enter lock state */
    uint8_t (*security_bits_set)(uint32_t addr, uint8_t *data, uint32_t size);  /*!< Check security bits in the programmable flash region */
    uint8_t (*target_set_state)(TARGET_RESET_STATE state);                      /*!< Families can customize target debug states in target_reset.h */
    void (*swd_set_target_reset)(uint8_t asserted);                             /*!< Families can customize how to send reset to the target */
    uint8_t (*validate_bin_nvic)(const uint8_t *buf);                           /*!< Validate a bin file to be flash by drag and drop */
    uint8_t (*validate_hexfile)(const uint8_t *buf);                            /*!< Validate a hex file to be flash by drag and drop */
} target_family_descriptor_t;

extern const target_family_descriptor_t *g_target_family;

#ifdef __cplusplus
extern "C" {
#endif

void init_family(void);
uint8_t target_family_valid(void);
uint8_t target_set_state(TARGET_RESET_STATE state);
void swd_set_target_reset(uint8_t asserted);

#ifdef __cplusplus
}
#endif

#endif
```

A flash algorithm blob is needed to program the target MCUs internal (or external) flash memory. This blob contains position independent functions for erasing, reading and writing to the flash controller. Flash algorithm blobs are created from the [FlashAlgo project.](https://github.com/mbedmicro/FlashAlgo) An example blob is shown below and would be added to `source/family/<mfg>/<targetname>/flash_blob.c`

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
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x4770ba40, 0x4770bac0, 0x4c0cb510, 0xf04068a0, 0x60a00001, 0x301af246, 0xf44f6560, 0x60e07040,
    0x20012100, 0xf83ef000, 0x65612100, 0xf02168a1, 0x60a10101, 0xbf182800, 0xbd102001, 0x400c0000,
    0x4684b510, 0xf44f2300, 0xf8505180, 0x1f092b04, 0x1c52d002, 0xe001d0f9, 0xd0131c50, 0x68a04c0c,
    0x0001f040, 0xf8c460a0, 0x2001c010, 0x200260e0, 0x210060e0, 0xf0002001, 0x4603f815, 0xf02068a0,
    0x60a00001, 0xbf142b00, 0x20002001, 0x0000bd10, 0x400c0000, 0xf6414902, 0x63c83071, 0x47702000,
    0x400c0000, 0x4b14b410, 0xc050f8df, 0x201cf8dc, 0x0f16f012, 0xf8dcd014, 0xf0200008, 0xf8cc0005,
    0xf0120008, 0xbf1e0f02, 0x0001f06f, 0x4770bc10, 0x0f04f012, 0xf04fbf1e, 0xbc1030ff, 0xe0074770,
    0x428a4002, 0xbc10d102, 0x47702000, 0xd1dd1e5b, 0xf06fbc10, 0x47700002, 0x00989680, 0x400c0000,
    0x4df0e92d, 0x1cc84604, 0x0603f020, 0x46904859, 0xf0416881, 0x60810101, 0xd9732e07, 0x0f07f014,
    0xf8d8d011, 0x61041000, 0x60c22201, 0x21086181, 0x210060c1, 0xf7ff4610, 0x2800ffb5, 0x1d24d17e,
    0x0810f108, 0x484b1f36, 0xf0416881, 0x60810104, 0xbf882e07, 0x0a02f06f, 0xf504d966, 0x46015080,
    0x010bf36f, 0x42b11b09, 0x4635bf8e, 0x000bf36f, 0xf0151b05, 0xbf180f04, 0x46471f2d, 0x20012100,
    0xff90f7ff, 0xd13b2800, 0xe0e8f8df, 0x4010f8ce, 0xf8ce2001, 0xf857000c, 0xf8ce0b04, 0xf8570018,
    0xf8ce0b04, 0x20080018, 0x000cf8ce, 0x0208f1a5, 0xd9332a07, 0x0b08f04f, 0x46dc482f, 0x101cf8de,
    0x0f16f011, 0xf8ded012, 0xf0200008, 0xf8ce0005, 0xf0110008, 0xbf180f02, 0x0001f06f, 0xf011d10f,
    0xbf180f04, 0x30fff04f, 0xe007d109, 0x010bea01, 0xd1014561, 0xe0022000, 0xd1df1e40, 0xb1104650,
    0xe00bb9e0, 0xf857e01c, 0xf8ce0b04, 0xf8570018, 0xf8ce0b04, 0x3a080018, 0xd8cb2a07, 0xeb081b76,
    0x442c0885, 0xd8982e07, 0x20012100, 0xff3af7ff, 0x4810b920, 0xf0216881, 0xe0000104, 0x6081e00f,
    0x480cb186, 0x1000f8d8, 0x22016104, 0x618160c2, 0x60c12108, 0x46102100, 0xff24f7ff, 0x2001b110,
    0x8df0e8bd, 0x68814803, 0x0101f021, 0x20006081, 0x8df0e8bd, 0x400c0000, 0x00989680, 0x68814803,
    0x0105f021, 0x20006081, 0x00004770, 0x400c0000, 0x00000000,
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
```c
enum _region_flags {
    kRegionIsDefault = (1 << 0),        //out of bounds regions will use the same flash algo if this is set
    kRegionIsSecure  = (1 << 1)
};


typedef struct region_info {
    uint32_t start;
    uint32_t end;
    uint32_t flags;
    uint8_t alias_index;            /*!<use with flags; will point to a different index if there is an alias region */
    program_target_t *flash_algo;   /*!< A pointer to the flash algorithm structure */
} region_info_t;

/**
 @struct target_cfg_t
 @brief  The firmware configuration struct has unique about the chip its running on.
 */
typedef struct target_cfg {
    uint32_t version;                                       /*!< Target configuration version */
    const sector_info_t* sectors_info;                      /*!< Sector start and length list */
    int sector_info_length;                                 /*!< Sector start and length list total */
    region_info_t flash_regions[MAX_EXTRA_FLASH_REGION];    /*!< Flash regions */
    region_info_t ram_regions[MAX_EXTRA_RAM_REGION];        /*!< RAM regions  */
    uint8_t erase_reset;                                    /*!< Reset after performing an erase */
    const char *rt_board_id;                                /*!< If assigned, this is a flexible board ID */
    uint16_t rt_family_id;                                     /*!< If assigned, this is a flexible board ID */
} target_cfg_t;
```
At this point these target specific files could be added to a board build and developed.

# Supported Target Families
A HIC can target all supported families available and a post build script can modify a board's family id to point to the correct family. See [Porting board guide](PORT_BOARD.md) 
