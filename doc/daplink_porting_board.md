# Adding New Board

Board is composed of a Hardware Interface Circuit (HIC) and Target MCU. In order to add a new Board, you need to create its description in `projects.yaml` file. The yaml descriptions are used to create modules that can be inherited.

You can add new Board to the build system by adding the product name and all required modules to `yaml` file, i.e. if the HIC is OpenSDA based on the NXP MK20D50 MCU that would be:

	k20dx_myboardname_if:
	    - *module_if
	    - *module_hic_k20dx
	    - records/board/myboardname.yaml

Next create a new file in the `records/board` directory called `myboardname.yaml`. This file defines the target MCU and allows  board parameters to be configured. The target MCU in this example exists and is a Nordic nRF51822 (16k RAM variant):

	common:
	    sources:
	        board:
	            - source/board/myboardname.c
	        target:
	            - source/target/nordic/nrf51822/target_16.c
	            - source/target/nordic/target_reset.c

This assumes Target support is already present in the codebase (as explained in [Adding New Target section](daplink_porting_target.md). Now `source/board/myboardname.c` file needs to be created with a `BOARD ID`. In order to obtain uqnique `BOARD ID` (each board needs that) please contact support@mbed.org.

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

Executing (from within a [Python Virtual Environment](daplink_sdk_setup.md)):

	progen generate -t uvision
	
will create all project files to build the DAPLink for your new Board and Target. Have fun!

If you want to read more about the `yaml` format see the [project_generator][project_generator] documentation.

**Note:** We are currently using Keil uVision MDK 5 to build DAPLink firmware because of high quality of the produced code and small footprint of the binary.

[project_generator]: https://github.com/project-generator/project_generator/wiki/Getting_started "Project Generator"
