/**
 * @file    target.c
 * @brief   Target information for the lpc4088
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

// The LPC4088 QuickStart Board has 512K internal flash and 8M external QSPI flash
target_cfg_t target_device = {
	.sector_size    = KB(4),
	.sector_cnt     = ((MB(8) + KB(512)) / KB(4)),
	.flash_start    = 0,
	.flash_end      = MB(8) + KB(512),
	.ram_start      = 0x10000000,
	.ram_end        = 0x10010000,
	.flash_algo     = (program_target_t *) &flash,
};
