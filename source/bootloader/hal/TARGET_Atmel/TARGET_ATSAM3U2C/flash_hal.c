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

#define KEY_VALUE             (0x5A)
#define FCMD_WP               (0x1)           // "Write page" command
#define FCMD_EWP              (0x3)           // "Erase page and write page" command
#define FCMD_CLB              (0x9)           // "Clear lock bit" command
#define FCMD_GLB              (0xA)           // "Get lock bit" command
#define FCMD_SGPB             (0xB)           // "Set General Purpose Bit" command
#define FCMD_CGPB             (0xC)           // "Clear General Purpose Bit" command
#define FCMD_GGPB             (0xD)           // "Get General Purpose Bit" command
#define FCMD_EA               (0x5)           // "Erase all" command
#define FMR_FWS               (0xF << 8)

#define WDT_CR                   *(volatile uint32_t*)(0x400E1250)
#define WDT_MR                   *(volatile uint32_t*)(0x400E1254)
#define WDT_SR                   *(volatile uint32_t*)(0x400E1258)

#define EEFC0_BASEADDR        (0x400E0800)
#define EEFC1_BASEADDR        (0x400E0A00)
#define FMR_FAM               (1 << 24)
#define GPNVMB_BMS            (1)

typedef struct {
  volatile uint32_t MC_FMR;
  volatile uint32_t MC_FCR;
  volatile uint32_t MC_FSR;
  volatile uint32_t MC_FRR;
} SFR_TABLE;


static void _FeedWDT(void) {
  //
	// Feed watchdog if enabled
	//
  if ((WDT_MR & (1 << 15)) == 0) {      // Is watchdog enabled ?
    WDT_CR = 0xA5000001;                // Feed it!
  }
}

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

static void _WritePage(uint32_t Addr, volatile uint32_t* pSrc, int PerformErase) {
  SFR_TABLE* pSFRs;
	volatile uint32_t* pDest;
	uint32_t NumItemsInPageLeft;
  uint32_t iPage;
	uint32_t Status;
  //
	// Return value 0 == O.K.
	// Return value 1 == Error
	// Application assumes that this function always erases 4 KB
	// Application always calls this function with 4 KB aligned addresses
	//
  Addr |= 0x80000;   // Convert to physical flash address
  pSFRs = (SFR_TABLE*)(EEFC0_BASEADDR);
  pDest = (volatile uint32_t*)Addr;
	//
	// Fill page buffer use blank data in case of erase is requested
	//
	iPage = ((Addr - 0x80000) >> 8);
	NumItemsInPageLeft = (1 << 8) >> 2;
  if (PerformErase) {
		do {
			*pDest++ = 0xFFFFFFFF;
		} while (--NumItemsInPageLeft);
	} else {
		do {
			*pDest++ = *pSrc++;
		} while (--NumItemsInPageLeft);
	}
	//
	// Trigger flash operation
	//
	pSFRs->MC_FCR = (KEY_VALUE << 24) | (FCMD_EWP << 0) | (iPage << 8);  // Erase page and write with blank data
	do {
		_FeedWDT();
		Status = pSFRs->MC_FSR;
	} while ((Status & 1) == 0);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

int flash_hal_init (uint32_t clk) {
  //
	// No special init required
	//
  return (0);
}

int flash_hal_uninit (uint32_t fnc) {
  //
	// No special uninit required
	//
  return (0);
}

int flash_hal_erase_chip (void) {
  uint32_t Addr;
  //
	// Return value 0 == O.K.
	// Return value 1 == Error
  // Erase complete chip by erasing sector-by-sector
	Addr = START_APP_ADDRESS;
  do {
    _WritePage(Addr, (volatile uint32_t*)0, 1);
    Addr += (1 << 8);
	} while (Addr < END_FLASH);
  return (0);  // O.K.
}

int flash_hal_erase_sector (uint32_t adr) {
  uint32_t NumPagesLeft;
  //
	// Return value 0 == O.K.
	// Return value 1 == Error
	// Application assumes that this function always erases 4 KB
	// Application always calls this function with 4 KB aligned addresses
	//
	NumPagesLeft = 0x1000 >> 8;                                         // SAM3U has 256 byte pages, CMSIS-DAP BTL/FW assumes 4 KB sectors
  do {
    _WritePage(adr, (volatile uint32_t*)0, 1);
    adr += (1 << 8);
	} while (--NumPagesLeft);
  return (0);  // O.K.
}

int flash_hal_program_page (uint32_t adr, uint32_t sz, unsigned char *buf) {
  uint32_t NumPagesLeft;
  //
	// Return value 0 == O.K.
	// Return value 1 == Error
  // Seems to program page-wise
	// 1 page seems to be 1 KB
	// App always calls this function with 1 KB aligned start-adresses
  // Always called with multiple of 1 page to program
  //
  NumPagesLeft = sz >> 8;    // SAM3U has 256 byte pages, CMSIS-DAP BTL/FW assumes 1 KB pages
  do {
    _WritePage(adr, (volatile uint32_t*)buf, 0);
    adr += (1 << 8);
		buf += (1 << 8);
	} while (--NumPagesLeft);
  return (0);                                  // Finished without Errors
}
