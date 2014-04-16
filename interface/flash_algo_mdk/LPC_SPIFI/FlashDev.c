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

#include "../FlashOS.H"        // FlashOS Structures
#include "FlashDev.h"

struct FlashDevice const FlashDevice = {
    FLASH_DRV_VERS,         // Driver Version, do not modify!
    FLASH_DEV_NAME,         // Device Name
    EXTSPI,                 // Device Type
    FLASH_START,            // Device Start Address
    FLASH_DEV_SIZE,         // Device Size
    256,                    // Programming Page Size
    0,                      // Reserved, must be 0
    0xFF,                   // Initial Content of Erased Memory
    500,                    // Program Page Timeout 500 mSec
    5000,                   // Erase Sector Timeout 5000 mSec

    // Specify size and address of sectors for each region
    FLASH_SECTOR_SIZE, 0x000000, // sector size, address
    SECTOR_END
};
