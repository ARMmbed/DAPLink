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

#include "FlashOS.H"        // FlashOS Structures

struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "nRF51822AA 256 KB Flash",  // Device Name
   ONCHIP,                     // Device Type
   0x00000000,                 // Flash start address
   0x00040000,                 // Flash total size (256 KB // + 1 kB)
   4,                          // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec
   3000,                       // Erase Sector Timeout 3000 mSec
// Specify Size and Address of Sectors
  0x000400, 0x000000,         // Sector Size  1 KB (256 Sectors)
//  0x000400, 0x10001000        // Sector Size  1 KB (1 Sector)
  SECTOR_END                  // Marks end of sector table
};
