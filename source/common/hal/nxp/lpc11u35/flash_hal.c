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
#include "flash_hal.h"        // FlashOS Structures

#define SET_VALID_CODE 1       // Set Valid User Code Signature
#define END_SECTOR     15
/* Code Read Protection (CRP) */
#define CRP_ADDRESS (0x000002FC)
#define NO_ISP        (0x4E697370)
#define CRP1        (0x12345678)
#define CRP2        (0x87654321)
#define CRP3        (0x43218765)
#define IS_CRP_VALUE(v) ((v==NO_ISP) || (v==CRP1) || (v==CRP2) || (v==CRP3))
#define NO_CRP      (1)     /* Forbid programming if CRP is enabled. */

uint32_t _CCLK;           // CCLK in kHz

struct sIAP {             // IAP Structure
  uint32_t cmd;           // Command
  uint32_t par[4];        // Parameters
  uint32_t stat;          // Status
  uint32_t res[2];        // Result
} IAP;

/* IAP Call */
typedef void (*IAP_Entry) (uint32_t *cmd, uint32_t *stat);
#define IAP_Call ((IAP_Entry) 0x1FFF1FF1)

static uint32_t GetSecNum (uint32_t adr) {
  uint32_t n;

  n = adr >> 12;                               //  4kB Sector
  if (n >= 0x10) {
    n = 0x0E + (n >> 3);                       // 32kB Sector
  }

  return (n);                                  // Sector Number
}

int flash_hal_init (uint32_t clk) {
  _CCLK = clk/1000;
  return (0);
}

int flash_hal_uninit (uint32_t fnc) {
  return (0);
}

int flash_hal_erase_chip (void) {
  IAP.cmd    = 50;                             // Prepare Sector for Erase
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = END_SECTOR;                     // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  IAP.cmd    = 52;                             // Erase Sector
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = END_SECTOR;                     // End Sector
  IAP.par[2] = _CCLK;                          // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  return (0);                                  // Finished without Errors
}

int flash_hal_erase_sector (uint32_t adr) {
  uint32_t n = GetSecNum(adr);

  IAP.cmd    = 50;                             // Prepare Sector for Erase
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat)
      return (1);                    // Command Failed

  IAP.cmd    = 52;                             // Erase Sector
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP.par[2] = _CCLK;                          // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat)
      return (1);                    // Command Failed

  return (0);                                  // Finished without Errors
}

int flash_hal_program_page (uint32_t adr, uint32_t sz, unsigned char *buf) {
  uint32_t n;

#if NO_CRP != 0
  if (adr == 0) {
      n = *((uint32_t *)(buf + CRP_ADDRESS));
    if (IS_CRP_VALUE(n))
    {
        /* CRP is enabled, exit. */
        return 1;
    }
  }
#endif

#if SET_VALID_CODE != 0                        // Set valid User Code Signature
  if (adr == 0) {                              // Check for Vector Table
    n = *((uint32_t *)(buf + 0x00)) +
        *((uint32_t *)(buf + 0x04)) +
        *((uint32_t *)(buf + 0x08)) +
        *((uint32_t *)(buf + 0x0C)) +
        *((uint32_t *)(buf + 0x10)) +
        *((uint32_t *)(buf + 0x14)) +
        *((uint32_t *)(buf + 0x18));
    *((uint32_t *)(buf + 0x1C)) = 0 - n;  // Signature at Reserved Vector
  }
#endif

  n = GetSecNum(adr);                          // Get Sector Number

  IAP.cmd    = 50;                             // Prepare Sector for Write
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat)
      return (1);                    // Command Failed

  IAP.cmd    = 51;                             // Copy RAM to Flash
  IAP.par[0] = adr;                            // Destination Flash Address
  IAP.par[1] = (uint32_t)buf;             // Source RAM Address
  IAP.par[2] = 1024;                           // Fixed Page Size
  IAP.par[3] = _CCLK;                          // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat)
      return (1);                    // Command Failed

  return (0);                                  // Finished without Errors
}
