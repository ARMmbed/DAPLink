# Adding A New Board
A board is composed of a Hardware Interface Circuit and target MCU. To create a new board build a description needs to be added to `projects.yaml`. The yaml descriptions are used to create modules that can be inherited. Create a new board build by adding the product name and the required modules. eg: if the HIC is OpenSDA based on the NXP MK20D50 MCU

```yaml
    k20dx_myboardname_if:
        - *module_if
        - *module_hic_k20dx
        - records/board/myboardname.yaml
```

Next create a new file in the `records/board` directory called myboardname.yaml. This file defines the target family MCU and allows overrideable board parameters to be configured. The target family MCU in this example exists and is a Nordic nRF51822 (16k RAM variant). More than one target/family can be added to the project and the family can be retargeted by family ID. See `Post-build Board ID and Family ID` section.

```yaml
common:
    sources:
        board:
            - source/board/myboardname.c
        family:
            - source/family/nordic/nrf51822/target_nrf51.c
            - source/family/nordic/target_reset_nrf51.c
```

This assumes there is already target support present in the codebase. If adding a new target family is needed, additional steps in [Porting target family guide](PORT_TARGET_FAMILY.md) will be needed. If the target support exists `source/board/myboardname.c` needs creation with a BOARD ID. If you're developing a custom or non-official Mbed platform, then can use any BOARD ID and the `mbedls` [mocking feature](https://github.com/ARMmbed/mbed-os-tools/blob/master/packages/mbed-ls/README.md#mocking-renaming-platforms).
```c
/**
 * @file   myboardname.c
 * @brief   board_info api for my board
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2021, Arm Limited, All Rights Reserved
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

#include "target_family.h"
#include "target_board.h"

extern target_cfg_t target_device_nrf51822_16;

const board_info_t g_board_info = {
    .board_id = "0x240",
    .family_id = kStub_HWReset_FamilyID,
    .flags = kEnablePageErase|kEnableUnderResetConnect,
    .target_cfg = &target_device_nrf51822_16,
};
```
The complete fields of board_info api with description is in `source/target/target_board.h`.
```c
// Flags for board_info
enum {
    kEnablePageErase = 1,               /*!< Enable page programming and sector erase for drag and drop */
    kEnableUnderResetConnect = 1<<1,    /*!< Enable under reset connection when enabling debug mode */
};

typedef struct __attribute__((__packed__)) board_info {
    uint16_t infoVersion;               /*!< Version number of the board */
    uint16_t family_id;                 /*!< Use to select target family from defined target family ids */
    char board_id[5];                   /*!< 4-char board ID plus null terminator */
    uint8_t _padding[3];
    uint32_t flags;                     /*!< Combination of kEnablePageErase and kEnableUnderResetConnect */
    target_cfg_t *target_cfg;           /*!< Specific chip configuration for the target and enables MSD when non-NULL */

    // fields used by MSD
    vfs_filename_t daplink_url_name;    /*!< Customize the URL file name */
    vfs_filename_t daplink_drive_name;  /*!< Customize the MSD DAPLink drive name */
    char daplink_target_url[64];        /*!< Customize the target url in DETAILS.TXT */

    // some specific board initilization
    void (*prerun_board_config)(void);                      /*!< Specific board debug/ID related initialization */
    void (*swd_set_target_reset)(uint8_t asserted);         /*!< Boards can customize how to send reset to the target precedence over target family */
    uint8_t (*target_set_state)(TARGET_RESET_STATE state);  /*!< Boards can customize target debug states in target_reset.h precedence over target family */
    uint32_t soft_reset_type;                               /*!< Boards can override software reset type to VECTRESET or SYSRESETREQ */
} board_info_t;
```
Now running `progen generate -t uvision` will create project files including the new board that can be developed and debugged. Complete development guide is in [Developers Guide](DEVELOPERS-GUIDE.md). For more information about the yaml format [see the project_generator documentation.](https://github.com/project-generator/project_generator/wiki/Getting_started)

### Adding Board to Automated Tests
Update `test/info.py` so the new board has at least one configuration in SUPPORTED_CONFIGURATIONS
```python
SUPPORTED_CONFIGURATIONS = [
    #   Board ID    Family ID                           Firmware                                    Bootloader          Target
    ...
    (   0x0240,     VENDOR_TO_FAMILY('NXP', 1),         'k20dx_frdmk64f_if',                        'k20dx_bl',         'FRDM-K64F'                             ),
    ...
]
```
Configration Fields
* Board ID - The ID assigned to the board type.
* Family ID - Use to select or identify target family from defined target family or custom ones. Note that common or stub families are supported which supports hw_reset, sw_vectreset or sw_sysreset. More details in [Porting target family guide](PORT_TARGET_FAMILY.md).
* Firmware - The name of the firmware as shown in projects.yaml.
* Bootloader - The name of the bootloader firmware as shown in projects.yaml. Only required on HICs with a DAPLink bootloader.
* Target - Name of the target for this board.
    * If this is an mbed official board then the target should match the name in the mbed platform page URL. For example the K64F is located at https://developer.mbed.org/platforms/FRDM-K64F/ so it would have the target name `FRDM-K64F`. Using this naming convention allows the automated tests to use the RESTful Compile API. the automated tests will build the target UART application on-the-fly in the cloud using the RESTful Compile API, download it to the PC, then download the resulting image to the target.
    * If it is not, you will need to build the UART application yourself and supply it to `test/run_tests.py` via --targetdir. In this case, the target is the application image filename sans extension.

You may need to update one or more other dictionaries. See comments in the code for guidance.

See [Automated Tests](AUTOMATED_TESTS.md) for more information related to automated testing.

# Post-build Board ID and Family ID
Board ID and Family ID can be overwritten by `tools/post_build_script.py`. This is helpful for supporting existing families and adding board IDs to the database without the need to compile DAPLink. Board ID can uniquely identify the target board while family ID can select which target family to support.
```
usage: post_build_script.py [-h] [--board-id BOARD_ID] [--family-id FAMILY_ID]
                            [--bin-offset BIN_OFFSET]
                            input output

post Build tool for Board ID, Family ID and CRC generation

positional arguments:
  input                 Hex or bin file to read from.
  output                Output base file name to write crc, board_id and
                        family_id.

optional arguments:
  -h, --help            show this help message and exit
  --board-id BOARD_ID   board id to for the target in hex
  --family-id FAMILY_ID
                        family id to for the target in hex
  --bin-offset BIN_OFFSET
                        binary offset in hex
```
Note however that run time board id and family id can be present in the code and DAPLink will use this if it has non-zero value. From `source/hic_hal_target_config.h`
```
/**
 @struct target_cfg_t
 @brief  The firmware configuration struct has unique about the chip its running on.
 */
 ...
typedef struct target_cfg {
    ...
    const char *rt_board_id;                                /*!< If assigned, this is a flexible board ID */
    uint16_t rt_family_id;                                     /*!< If assigned, this is a flexible board ID */
} target_cfg_t;
...
```
