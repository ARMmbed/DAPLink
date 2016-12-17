# Adding A New Board
A board is composed of a Hardware Interface Circuit and target MCU. To create a new board build a description needs to be added to `projects.yaml`. The yaml descriptions are used to create modules that can be inherited. Create a new board build by adding the product name and the required modules. eg: if the HIC is OpenSDA based on the NXP MK20D50 MCU

```yaml
    k20dx_myboardname_if:
        - *module_if
        - *module_hic_k20dx
        - records/board/myboardname.yaml
```

Next create a new file in the `records/board` directory called myboardname.yaml. This file defines the target MCU and allows overrideable board parameters to be configured. The target MCU in this example exists and is a Nordic nRF51822 (16k RAM variant)

```yaml
common:
    sources:
        board:
            - source/board/myboardname.c
        target:
            - source/target/nordic/nrf51822/target_16.c
            - source/target/nordic/target_reset.c
```

This assumes there is already target support present in the codebase. If adding a new target additional steps below will be needed. If the target support exists `source/board/myboardname.c` needs creation with a BOARD ID. To get a BOARD ID email support@mbed.org
```c
/**
 * @file    myboardname.c
 * @brief   board ID for myboard
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

const char *board_id = "0000";
```

Now running `progen generate -t uvision` will create project files including the new board that can be developed and debugged. For more information about the yaml format [see the project_generator documentation.](https://github.com/project-generator/project_generator/wiki/Getting_started)

### Adding Board to Automated Tests
Update `test/info.py` so the new board has at least one configuration in SUPPORTED_CONFIGURATIONS
```python
SUPPORTED_CONFIGURATIONS = [
    #   Board ID    Firmware                            Bootloader          Target
    ...
    (   0x240,      'k20dx_frdmk64f_if',                'k20dx_bl',         'FRDM-K64F'                     ),
    ...
]
```
Configration Fields
* Board ID - The ID assigned to the board type.
* Firmware - The name of the firmware as shown in projects.yaml.
* Bootloader - The name of the bootloader firmware as shown in projects.yaml. Only required on HICs with a DAPLink bootloader.
* Target - Name of the target for this board.
    * If this is an mbed official board then the target should match the name in the mbed platform page URL. For example the K64F is located at https://developer.mbed.org/platforms/FRDM-K64F/ so it would have the target name `FRDM-K64F`.    Using this naming convention allows the automated tests to use the RESTful Compile API. the automated tests will build the    target UART application on-the-fly in the cloud using the RESTful Compile API, download it to the PC, then download the    resulting image to the target. 
    * If it is not, you will need to build the UART application yourself and supply it to `test\run_tests.py` via --targetdir. In this case, the target is the application image filename sans extension.

You may need to update one or more other dictionaries. See comments in the code for guidance.

See [Automated Tests](AUTOMATED_TESTS.md) for more information related to automated testing.
