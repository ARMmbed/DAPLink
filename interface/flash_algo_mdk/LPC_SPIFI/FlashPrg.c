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

#include "spifi_rom_api.h"

#define CGU_BASE_SPIFI0_CLK     (*(volatile unsigned long *)0x40050070)

#define SCU_SFSP3_3             (*(volatile unsigned long *)0x4008618C)
#define SCU_SFSP3_4             (*(volatile unsigned long *)0x40086190)
#define SCU_SFSP3_5             (*(volatile unsigned long *)0x40086194)
#define SCU_SFSP3_6             (*(volatile unsigned long *)0x40086198)
#define SCU_SFSP3_7             (*(volatile unsigned long *)0x4008619C)
#define SCU_SFSP3_8             (*(volatile unsigned long *)0x400861A0)


SPIFIobj   obj;
SPIFIopers opers;

unsigned long base_adr;

/*  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

#ifdef USE_SPIFI_LIB
#else
  SPIFI_RTNS * pSpifi;
  pSpifi = (SPIFI_RTNS *)(SPIFI_ROM_PTR);
  /* Call functions via spifi rom table */
  #define spifi_init pSpifi->spifi_init
#endif

  opers.dest    = NULL;
  opers.length  = 0;
  opers.scratch = NULL;
  opers.protect = -1;                    // save and restore protection
  opers.options = S_CALLER_ERASE;         // Keil code handles erasing

  base_adr = adr;

  /* init SPIFI clock and pins */
  CGU_BASE_SPIFI0_CLK = 1<<24 | 1<<11; /* IRC 12 MHz is good enough for us */

  /* set up SPIFI I/O (undocumented bit 7 set as 1, Aug 2 2011) */
  SCU_SFSP3_3 = 0xF3; /* high drive for SCLK */
  /* IO pins */
  SCU_SFSP3_4 =
  SCU_SFSP3_5 =
  SCU_SFSP3_6 =
  SCU_SFSP3_7 = 0xD3;
  SCU_SFSP3_8 = 0x13; /* CS doesn't need feedback */

  if (spifi_init(&obj, 3, S_RCVCLK | S_FULLCLK, 12)) {
    return 1;
  }

  return 0;                                  // Finished without Errors
}

/*  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
int UnInit (unsigned long fnc) {

  return 0;                                  // Finished without Errors
}

/*  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed */
int EraseChip (void) {
  int32_t rc;

#ifdef USE_SPIFI_LIB
  /* Use spifi function names directly */
#else
  SPIFI_RTNS * pSpifi;
  pSpifi = (SPIFI_RTNS *)(SPIFI_ROM_PTR);
  /* Call functions via spifi rom table */
  #define spifi_erase pSpifi->spifi_erase
#endif

  opers.dest    = 0;
  opers.length  = obj.devSize;
  opers.scratch = NULL;
  opers.options = S_VERIFY_ERASE;

  rc = spifi_erase(&obj, &opers);
  if (rc != 0) 
    return (1);

  return (0);
}

/*  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */
int EraseSector (unsigned long adr) {
  int32_t rc;
#ifdef USE_SPIFI_LIB
  /* Use spifi function names directly */
#else
  SPIFI_RTNS * pSpifi;
  pSpifi = (SPIFI_RTNS *)(SPIFI_ROM_PTR);
  /* Call functions via spifi rom table */
  #define spifi_erase pSpifi->spifi_erase
#endif

  opers.dest    = (char *)(adr - base_adr);
  opers.length  = 0x010000UL;
  opers.scratch = NULL;
  opers.options = S_VERIFY_ERASE;

  rc = spifi_erase(&obj, &opers);
  if (rc != 0) 
    return (1);

  return (0);
}

/*  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  int32_t rc;
#ifdef USE_SPIFI_LIB
  /* Use spifi function names directly */
#else
  SPIFI_RTNS * pSpifi;
  pSpifi = (SPIFI_RTNS *)(SPIFI_ROM_PTR);
  /* Call functions via spifi rom table */
   #define spifi_program pSpifi->spifi_program
#endif

  opers.dest    = (char *)(adr - base_adr);
  opers.length  = sz;
  opers.scratch = NULL;
  opers.protect = 0;
  opers.options = S_CALLER_ERASE;

  rc = spifi_program(&obj, (char *)buf, &opers);
  if (rc != 0) 
    return (1);

  return (0);
}
