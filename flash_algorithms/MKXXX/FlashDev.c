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

#ifdef MKP64
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKXX 64kB Prog Flash",     // Device Name
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x00010000,                 // Device Size (64kB)
   512,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000800, 0x000000,         // Sector Size  2kB (128 Sectors)
   SECTOR_END
};
#endif

#ifdef MKP64_50MHZ
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,               // Driver Version, do not modify!
   "MKXX 50Mhz 64kB Prog Flash",// Device Name
   ONCHIP,                       // Device Type
   0x00000000,                   // Device Start Address
   0x00010000,                   // Device Size (64kB)
   256,                          // Programming Page Size
   0,                            // Reserved, must be 0
   0xFF,                         // Initial Content of Erased Memory
   1000,                         // Program Page Timeout 1000 mSec
   3000,                         // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000400, 0x000000,           // Sector Size  1kB (64 Sectors)
   SECTOR_END
};
#endif

#ifdef MKP64_48MHZ
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,               // Driver Version, do not modify!
   "MKXX 48Mhz 64kB Prog Flash",// Device Name
   ONCHIP,                       // Device Type
   0x00000000,                   // Device Start Address
   0x00010000,                   // Device Size (64kB)
   256,                          // Programming Page Size
   0,                            // Reserved, must be 0
   0xFF,                         // Initial Content of Erased Memory
   1000,                         // Program Page Timeout 1000 mSec
   3000,                         // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000400, 0x000000,           // Sector Size  1kB (64 Sectors)
   SECTOR_END
};
#endif

#ifdef MKP128
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKXX 128kB Prog Flash",    // Device Name
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x00020000,                 // Device Size (128kB)
   512,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000800, 0x000000,         // Sector Size  2kB (128 Sectors)
   SECTOR_END
};
#endif

#ifdef MKP128_50MHZ
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,               // Driver Version, do not modify!
   "MKXX 50Mhz 128kB Prog Flash",// Device Name
   ONCHIP,                       // Device Type
   0x00000000,                   // Device Start Address
   0x00020000,                   // Device Size (128kB)
   256,                          // Programming Page Size
   0,                            // Reserved, must be 0
   0xFF,                         // Initial Content of Erased Memory
   1000,                         // Program Page Timeout 1000 mSec
   3000,                         // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000400, 0x000000,           // Sector Size  1kB (128 Sectors)
   SECTOR_END
};
#endif

#ifdef MKP128_48MHZ
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,               // Driver Version, do not modify!
   "MKXX 48Mhz 128kB Prog Flash",// Device Name
   ONCHIP,                       // Device Type
   0x00000000,                   // Device Start Address
   0x00020000,                   // Device Size (128kB)
   256,                          // Programming Page Size
   0,                            // Reserved, must be 0
   0xFF,                         // Initial Content of Erased Memory
   1000,                         // Program Page Timeout 1000 mSec
   3000,                         // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000400, 0x000000,           // Sector Size  1kB (128 Sectors)
   SECTOR_END
};
#endif


#ifdef MKP32_50MHZ
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,               // Driver Version, do not modify!
   "MKXX 50Mhz 32kB Prog Flash", // Device Name
   ONCHIP,                       // Device Type
   0x00000000,                   // Device Start Address
   0x00008000,                   // Device Size (32kB)
   256,                          // Programming Page Size
   0,                            // Reserved, must be 0
   0xFF,                         // Initial Content of Erased Memory
   1000,                         // Program Page Timeout 1000 mSec
   3000,                         // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000400, 0x000000,           // Sector Size  1kB (32 Sectors)
   SECTOR_END
};
#endif

#ifdef MKP32_48MHZ
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,               // Driver Version, do not modify!
   "MKXX 48Mhz 32kB Prog Flash", // Device Name
   ONCHIP,                       // Device Type
   0x00000000,                   // Device Start Address
   0x00008000,                   // Device Size (32kB)
   256,                          // Programming Page Size
   0,                            // Reserved, must be 0
   0xFF,                         // Initial Content of Erased Memory
   1000,                         // Program Page Timeout 1000 mSec
   3000,                         // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000400, 0x000000,           // Sector Size  1kB (32 Sectors)
   SECTOR_END
};
#endif

#ifdef MKP16_48MHZ
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,               // Driver Version, do not modify!
   "MKXX 48Mhz 16kB Prog Flash", // Device Name
   ONCHIP,                       // Device Type
   0x00000000,                   // Device Start Address
   0x00004000,                   // Device Size (16kB)
   128,                          // Programming Page Size
   0,                            // Reserved, must be 0
   0xFF,                         // Initial Content of Erased Memory
   1000,                         // Program Page Timeout 1000 mSec
   3000,                         // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000400, 0x000000,           // Sector Size  1kB (16 Sectors)
   SECTOR_END
};
#endif

#ifdef MKP8_48MHZ
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,               // Driver Version, do not modify!
   "MKXX 48Mhz 8kB Prog Flash",  // Device Name
   ONCHIP,                       // Device Type
   0x00000000,                   // Device Start Address
   0x00002000,                   // Device Size (8kB)
   32,                           // Programming Page Size
   0,                            // Reserved, must be 0
   0xFF,                         // Initial Content of Erased Memory
   1000,                         // Program Page Timeout 1000 mSec
   3000,                         // Erase Sector Timeout 3000 mSec
// Specify Size and Address of Sectors
   0x000400, 0x000000,           // Sector Size  1kB (8 Sectors)
   SECTOR_END
};
#endif

#ifdef MKP256
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKXX 256kB Prog Flash",    // Device Name
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x00040000,                 // Device Size (256kB)
   512,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000800, 0x000000,         // Sector Size  2kB (128 Sectors)
   SECTOR_END
};
#endif

#ifdef MKP256_50MHZ
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,               // Driver Version, do not modify!
   "MKXX 50Mhz 256kB Prog Flash",// Device Name
   ONCHIP,                       // Device Type
   0x00000000,                   // Device Start Address
   0x00040000,                   // Device Size (256kB)
   256,                          // Programming Page Size
   0,                            // Reserved, must be 0
   0xFF,                         // Initial Content of Erased Memory
   1000,                         // Program Page Timeout 1000 mSec
   3000,                         // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000400, 0x000000,         // Sector Size  1kB (256 Sectors)
   SECTOR_END
};
#endif

#ifdef MKP512
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKxxN 512kB Prog Flash",   // Device Name
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x00080000,                 // Device Size (512kB)
   512,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000800, 0x000000,         // Sector Size  2kB (256 Sectors)
   SECTOR_END
};
#endif

#ifdef MKP512X
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKxxX 512kB Prog Flash",   // Device Name
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x00080000,                 // Device Size (512kB)
   512,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x001000, 0x000000,         // Sector Size  4kB (128 Sectors)
   SECTOR_END
};
#endif

#ifdef MKP512_50MHZ
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,               // Driver Version, do not modify!
   "MKXX 50Mhz 512kB Prog Flash",// Device Name
   ONCHIP,                       // Device Type
   0x00000000,                   // Device Start Address
   0x00080000,                   // Device Size (512kB)
   256,                          // Programming Page Size
   0,                            // Reserved, must be 0
   0xFF,                         // Initial Content of Erased Memory
   1000,                         // Program Page Timeout 1000 mSec
   3000,                         // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000400, 0x000000,           // Sector Size  1kB (512 Sectors)
   SECTOR_END
};
#endif

#ifdef MKP1024
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKxxN 1024KB Prog Flash",  // Device Name
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x00100000,                 // Device Size (512kB)
   512,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x001000, 0x000000,         // Sector Size  4kB (256 Sectors)
   SECTOR_END
};
#endif

#ifdef MKD32
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKXX 32kB Data Flash",     // Device Name
   ONCHIP,                     // Device Type
   0x10000000,                 // Device Start Address
   0x00008000,                 // Device Size (32kB)
   512,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec
// Specify Size and Address of Sectors
   0x000800, 0x000000,         // Sector Size  2kB (16 Sectors)
   SECTOR_END
};
#endif

#ifdef MKD32_50MHZ
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,              // Driver Version, do not modify!
   "MKXX 50Mhz 32kB Data Flash",// Device Name
   ONCHIP,                      // Device Type
   0x10000000,                  // Device Start Address
   0x00008000,                  // Device Size (32kB)
   256,                         // Programming Page Size
   0,                           // Reserved, must be 0
   0xFF,                        // Initial Content of Erased Memory
   1000,                        // Program Page Timeout 1000 mSec
   3000,                        // Erase Sector Timeout 3000 mSec
// Specify Size and Address of Sectors
   0x000400, 0x000000,          // Sector Size  1kB (32 Sectors)
   SECTOR_END
};
#endif

#ifdef MKD32_72MHZ
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,              // Driver Version, do not modify!
   "MKXX 72Mhz 32kB Data Flash",// Device Name
   ONCHIP,                      // Device Type
   0x10000000,                  // Device Start Address
   0x00008000,                  // Device Size (32kB)
   256,                         // Programming Page Size
   0,                           // Reserved, must be 0
   0xFF,                        // Initial Content of Erased Memory
   1000,                        // Program Page Timeout 1000 mSec
   3000,                        // Erase Sector Timeout 3000 mSec
// Specify Size and Address of Sectors
   0x000400, 0x000000,          // Sector Size  1kB (32 Sectors)
   SECTOR_END
};
#endif

#ifdef MKD64_50MHZ
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,              // Driver Version, do not modify!
   "MKXX 50Mhz 64kB Data Flash",// Device Name
   ONCHIP,                      // Device Type
   0x10000000,                  // Device Start Address
   0x00010000,                  // Device Size (64kB)
   256,                         // Programming Page Size
   0,                           // Reserved, must be 0
   0xFF,                        // Initial Content of Erased Memory
   1000,                        // Program Page Timeout 1000 mSec
   3000,                        // Erase Sector Timeout 3000 mSec
// Specify Size and Address of Sectors
   0x000400, 0x000000,          // Sector Size  1kB (64 Sectors)
   SECTOR_END
};
#endif

#ifdef MKD128
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKXX 128kB Data Flash",    // Device Name
   ONCHIP,                     // Device Type
   0x10000000,                 // Device Start Address
   0x00020000,                 // Device Size (128kB)
   512,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000800, 0x000000,         // Sector Size  2kB (64 Sectors)
   SECTOR_END
};
#endif


#ifdef MKD256
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKXX 256kB Data Flash",    // Device Name
   ONCHIP,                     // Device Type
   0x10000000,                 // Device Start Address
   0x00040000,                 // Device Size (256kB)
   512,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000800, 0x000000,         // Sector Size  2kB (128 Sectors)
   SECTOR_END
};
#endif

#ifdef MKD512
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKXX 512kB Data Flash",    // Device Name
   ONCHIP,                     // Device Type
   0x10000000,                 // Device Start Address
   0x00080000,                 // Device Size (512kB)
   512,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x001000, 0x000000,         // Sector Size  4kB (128 Sectors)
   SECTOR_END
};
#endif

#ifdef MKPIFR //Program Flash IFR
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKXX Program Flash IFR",   // Device Name
   ONCHIP,                     // Device Type
   0x0FFFFFC0,                 // Device Start Address
   0x00000040,                 // Device Size (64 Byte)
   64,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000040, 0x000000,         // Sector Size  64 Byte (1 Sector)
   SECTOR_END
};
#endif

#ifdef MKPIFR_50MHZ  //Program Flash IFR
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKXX 50Mhz Program Flash IFR",// Device Name
   ONCHIP,                     // Device Type
   0x0FFFFFC0,                 // Device Start Address
   0x00000040,                 // Device Size (64 Byte)
   64,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec
// Specify Size and Address of Sectors
   0x000040, 0x000000,         // Sector Size  64 Byte (1 Sector)
   SECTOR_END
};
#endif

#ifdef MKPIFR_48MHZ  //Program Flash IFR
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKXX 48Mhz Program Flash IFR",// Device Name
   ONCHIP,                     // Device Type
   0x0FFFFFC0,                 // Device Start Address
   0x00000040,                 // Device Size (64 Byte)
   64,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec
// Specify Size and Address of Sectors
   0x000040, 0x000000,         // Sector Size  64 Byte (1 Sector)
   SECTOR_END
};
#endif

#ifdef MKPIFR_120MHZ  //Program Flash IFR
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKXX 120Mhz Program Flash IFR",// Device Name
   ONCHIP,                     // Device Type
   0x0FFFFFC0,                 // Device Start Address
   0x00000040,                 // Device Size (64 Byte)
   64,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec
// Specify Size and Address of Sectors
   0x000040, 0x000000,         // Sector Size  64 Byte (1 Sector)
   SECTOR_END
};
#endif

#ifdef MKDIFR //Data Flash IFR
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKXX Data Flash IFR",      // Device Name
   ONCHIP,                     // Device Type
   0x13FFFFFC,                 // Device Start Address
   0x00000004,                 // Device Size (4 Byte)
   4,                          // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec
// Specify Size and Address of Sectors
   0x000004, 0x000000,         // Sector Size  4 Byte (1 Sector)
   SECTOR_END
};
#endif

#ifdef MKDIFR_50MHZ  //Data Flash IFR
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKXX 50Mhz Data Flash IFR",// Device Name
   ONCHIP,                     // Device Type
   0x13FFFFFC,                 // Device Start Address
   0x00000004,                 // Device Size (4 Byte)
   4,                          // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec
// Specify Size and Address of Sectors
   0x000004, 0x000000,         // Sector Size  4 Byte (1 Sector)
   SECTOR_END
};
#endif
