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

#if defined(LPC1800_SPIFI)
struct FlashDevice const FlashDevice = {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
#if defined(LPC1800_8192)
   "LPC18xx/43xx 4MB SPIFI",   // Device Name 
#else
   "LPC18xx/43xx 8MB SPIFI",   // Device Name 
#endif
   EXTSPI,                     // Device Type
   0x14000000,                 // Device Start Address
#if defined(LPC1800_8192)
   0x00800000,                 // Device Size is 8MB
#else
   0x00400000,                 // Device Size is 4MB
#endif
   256,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   500,                        // Program Page Timeout 100 mSec
   5000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x010000, 0x000000,         // sectors are 64 KB
   SECTOR_END
};
#endif // LPC1800_SPIFI
