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

#define U8  unsigned char
#define U16 unsigned short
#define U32 unsigned long
#define U64 unsigned long long

#define I8  signed char
#define I16 signed short
#define I32 signed long

#define FLASH_MODE_READ  (0)
#define FLASH_MODE_WRITE (1)
#define FLASH_MODE_ERASE (2)

/*********************************************************************
*
*       Register definitions
*/
#define FLASH_REG_BASE_ADDR (0x4001E000)
#define INFO_REGS_BASE_ADDR (0x10000000)
#define WDT_REGS_BASE_ADDR  (0x40010000)

#define FLASH_REG_READY       *((volatile U32*)(FLASH_REG_BASE_ADDR + 0x400))
#define FLASH_REG_CONFIG      *((volatile U32*)(FLASH_REG_BASE_ADDR + 0x504))
#define FLASH_REG_ERASEPAGE   *((volatile U32*)(FLASH_REG_BASE_ADDR + 0x508))
#define FLASH_REG_ERASEALL    *((volatile U32*)(FLASH_REG_BASE_ADDR + 0x50C))
#define FLASH_REG_ERASEUICR   *((volatile U32*)(FLASH_REG_BASE_ADDR + 0x514))
#define INFO_REG_CODEPAGESIZE *((volatile U32*)(INFO_REGS_BASE_ADDR + 0x10))
#define INFO_REG_CODESIZE     *((volatile U32*)(INFO_REGS_BASE_ADDR + 0x14))
#define INFO_REG_CODEBASE     *((volatile U32*)(INFO_REGS_BASE_ADDR + 0x18))
#define INFO_REG_CONFIGID     *((volatile U32*)(INFO_REGS_BASE_ADDR + 0x5C))
#define INFO_REG_DEVICEID     *((volatile U32*)(INFO_REGS_BASE_ADDR + 0x60))

#define WDT_REG_INTENSET      *((volatile U32*)(WDT_REGS_BASE_ADDR + 0x304))
#define WDT_REG_INTENCLR      *((volatile U32*)(WDT_REGS_BASE_ADDR + 0x308))
#define WDT_REG_REQSTATUS     *((volatile U32*)(WDT_REGS_BASE_ADDR + 0x404))
#define WDT_REG_CRV           *((volatile U32*)(WDT_REGS_BASE_ADDR + 0x504))
#define WDT_REG_RREN          *((volatile U32*)(WDT_REGS_BASE_ADDR + 0x508))
#define WDT_REG_CONFIG        *((volatile U32*)(WDT_REGS_BASE_ADDR + 0x50C))
#define WDT_REG_RR0           *((volatile U32*)(WDT_REGS_BASE_ADDR + 0x600))  // 8 registers, each 4 bytes in size

/*
 *  Feed watchdog, if running
 */
static void _FeedWDT(void) {
  U32 Status;
  volatile U32* pReloadReg;
  
  Status = WDT_REG_REQSTATUS & 0xFF;
  if (Status && WDT_REG_CRV) {    // Watchdog reload value configured and any reload registers are enabled and not fed yet?
    pReloadReg = (volatile U32*)(WDT_REGS_BASE_ADDR + 0x600);
    //
    // We need to write a reload request to each reload request register which is enabled and which has not been written yet
    //
    do {
      if (Status & 1) {
        *pReloadReg = 0x6E524635;
      }
      pReloadReg++;
      Status >>= 1;
    } while(Status);
  }
}

/*
 *  Erase a single flash sector
 */
static void _EraseSector(U32 Addr) {
  U32 Status;
  //
  // Make sure that flash controller is in erase mode
  //
  FLASH_REG_CONFIG = FLASH_MODE_ERASE;
  //
  // Check if sector is the UICR or CODE region
  //
  if (Addr >= 0x10001000) {
    FLASH_REG_ERASEUICR = 1;
  } else {
    FLASH_REG_ERASEPAGE = Addr;
  }
  //
  // Wait for operation to complete
  //
  do {
    _FeedWDT();
    Status = FLASH_REG_READY;
    if (Status & 1) {        // Flash controller ready?
      break;
    }
  } while(1);
  //
  // Bring back flash controller into read mode
  //
  FLASH_REG_CONFIG = FLASH_MODE_READ;
}

/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {
	//
	// No special init necessary
	//
  return (0);
}

/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {
	//
	// No special uninit necessary
	//
  return (0);
}

/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */
int EraseChip (void) {
  U32 Status;
  //
  // Make sure that flash controller is in erase mode
  //
  FLASH_REG_CONFIG = FLASH_MODE_ERASE;
  //
	// Use erase chip command, since it is faster
	//
	//
	// Chip erase, erases CODE and UICR Regions
	//
	FLASH_REG_ERASEALL = 1;
	//
	// Wait for operation to complete
	//
	do {
		Status = FLASH_REG_READY;
		if (Status & 1) {        // Flash controller ready?
			break;
		}
		_FeedWDT();
	} while(1);
  //
  // Bring back flash controller into read mode
  //
  FLASH_REG_CONFIG = FLASH_MODE_READ;   
  return (0);                                    // Finished without Errors
}

/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */
int EraseSector (unsigned long adr) {
  _EraseSector(adr);
  return (0);
}

/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  volatile U32* pDest;
  volatile U32* pSrc;
  U32 NumWords;
  U32 Status;
	
  pDest = (volatile U32*)adr;
  pSrc = (volatile U32*)buf;    // Always 32-bit aligned. Made sure by CMSIS-DAP firmware
	//
	// adr is always aligned to "Programming Page Size" specified in table in FlashDev.c
  // sz is always a multiple of "Programming Page Size"
	//
  NumWords = sz >> 2;	
  //
  // Make sure that flash controller is in write mode
  //
  FLASH_REG_CONFIG = FLASH_MODE_WRITE;
  //
  // Program word by word
  //
  do {
		*pDest++ = *pSrc++;
    //
    // Wait for operation to complete
    //
    do {
      Status = FLASH_REG_READY;
      if (Status & 1) {        // Flash controller ready?
        break;
      }
      _FeedWDT();
    } while(1);
  } while(--NumWords);
  //
  // Bring back flash controller into read mode
  //
  FLASH_REG_CONFIG = FLASH_MODE_READ;
  return (0);                                  // Finished without Errors
}
