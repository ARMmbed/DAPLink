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

#include "..\FlashOS.H"        // FlashOS Structures

// Memory Mapping Control
#if defined(LPC11xx_32) || defined(LPC8xx_4)
#define MEMMAP   (*((volatile unsigned long *) 0x40048000))
#else
#define MEMMAP   (*((volatile unsigned char *) 0x400FC040))
#endif

#ifdef MBED

#if defined(LPC11xx_32) || defined(LPC8xx_4)
#define MAINCLKSEL (*((volatile unsigned long *) 0x40048070))
#define MAINCLKUEN (*((volatile unsigned long *) 0x40048074))
#define MAINCLKDIV (*((volatile unsigned long *) 0x40048078))
#else

/* Phase Locked Loop (Main PLL) */
#define PLL0CON           (*((volatile unsigned char *) 0x400FC080))
#define PLL0CFG           (*((volatile unsigned long *) 0x400FC084))
#define PLL0STAT          (*((volatile unsigned long *) 0x400FC088))
#define PLL0FEED          (*((volatile unsigned char *) 0x400FC08C))
/* CPU clock configuration */
#define CCLKCFG          (*((volatile unsigned char *) 0x400FC104))
/* USB clock configuration */
#define USBCLKCFG          (*((volatile unsigned char *) 0x400FC108))
/* Clock source select */
#define CLKSRCSEL         (*((volatile unsigned char *) 0x400FC10C))
/* System control and status */
#define SCS              (*((volatile unsigned long *) 0x400FC1A0))

#define PLLCON_PLLE        (1<<0)
#define PLLCON_PLLC        (1<<1)
#define PLLSTAT_LOCK     (1<<26)
#define SCS_OSCEN         (1<<5)
#define SCS_OSCSTAT      (1<<6)
#endif

#else
// Phase Locked Loop (Main PLL)
#define PLL0CON  (*((volatile unsigned char *) 0x400FC080))
#define PLL0CFG  (*((volatile unsigned long *) 0x400FC084))
#define PLL0STAT (*((volatile unsigned long *) 0x400FC088))
#define PLL0FEED (*((volatile unsigned char *) 0x400FC08C))
#endif

#define STACK_SIZE     64      // Stack Size

#define SET_VALID_CODE 1       // Set Valid User Code Signature

#ifdef LPC1700_512
#define END_SECTOR     29
#endif
#ifdef LPC1700_256
#define END_SECTOR     21
#endif
#ifdef LPC1700_128
#define END_SECTOR     17
#endif
#ifdef LPC1700_64
#define END_SECTOR     15
#endif
#ifdef LPC1700_32
#define END_SECTOR     7
#endif
#ifdef LPC11xx_32
#define END_SECTOR     7
#endif
#ifdef LPC8xx_4
#define END_SECTOR     3
#endif

/* Code Read Protection (CRP) */
#define CRP_ADDRESS (0x000002FC)
#define NO_ISP        (0x4E697370)
#define CRP1        (0x12345678)
#define CRP2        (0x87654321)
#define CRP3        (0x43218765)
#define IS_CRP_VALUE(v) ((v==NO_ISP) || (v==CRP1) || (v==CRP2) || (v==CRP3))

#if defined(LPC11xx_32) || defined(LPC8xx_4)
#define NO_CRP      (0)
#define _CCLK (12000)
#else
#define NO_CRP      (1)     /* Forbid programming if CRP is enabled. */
unsigned long _CCLK;           // CCLK in kHz
#endif

struct sIAP {                  // IAP Structure
  unsigned long cmd;           // Command
  unsigned long par[4];        // Parameters
  unsigned long stat;          // Status
  unsigned long res[2];        // Result
} IAP;


/* IAP Call */
typedef void (*IAP_Entry) (unsigned long *cmd, unsigned long *stat);
#define IAP_Call ((IAP_Entry) 0x1FFF1FF1)


/*
 * Get Sector Number

 *    Parameter:      adr:  Sector Address
 *    Return Value:   Sector Number
 */

unsigned long GetSecNum (unsigned long adr) {
  unsigned long n;

#if defined(LPC8xx_4)
  n = adr >> 10;                               //  1kB Sector
#elif defined(LPC11xx_32)
  n = adr >> 12;                               //  4kB Sector
#else
  n = adr >> 12;                               //  4kB Sector
  if (n >= 0x10) {
    n = 0x0E + (n >> 3);                       // 32kB Sector
  }
#endif

  return (n);                                  // Sector Number
}

#ifdef MBED

/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

#if defined(LPC11xx_32) || defined(LPC8xx_4)

  MAINCLKSEL = 0;                              // Select Internal RC Oscillator
  MAINCLKUEN = 1;                              // Update Main Clock Source
  MAINCLKUEN = 0;                              // Toggle Update Register
  MAINCLKUEN = 1;
  while (!(MAINCLKUEN & 1));                   // Wait until updated
  MAINCLKDIV = 1;                              // Set Main Clock divider to 1

  MEMMAP     = 0x02;                           // User Flash Mode

#else

  /* TODO: Could check for valid part ID here */

  /* Setup PLL etc. to give CCLK = 60MHz */

  /* Shut down PLL */
  PLL0CON &= ~PLLCON_PLLC; /* Disconnect */
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  PLL0CON &= ~PLLCON_PLLE; /* Disable */
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;

  /* Start main oscillator */
  SCS |= SCS_OSCEN;
  while ((SCS & SCS_OSCSTAT) == 0);

  /* Select main oscillator */
  CLKSRCSEL = 0x01;

  /* Configure PLL */
  PLL0CFG =     0x00000013;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;

  /* Enable PLL */
  PLL0CON |= PLLCON_PLLE;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  while ((PLL0STAT & PLLSTAT_LOCK) == 0);

  /* Set CPU clock divider */
  CCLKCFG = 0x00000007;

  /* Set USB clock divider (just to set this clock to a reasonable value) */
  USBCLKCFG = 0x00000009;

  /* Connect PLL */
  PLL0CON |= PLLCON_PLLC;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;

  /* Select user flash mode */
  MEMMAP  = 0x01;

  _CCLK = 60000; /* 60MHz */
#endif

  return (0);
}

#else
/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  _CCLK     = 4000;                            // 4MHz Internal RC Oscillator

  PLL0CON  = 0x00;                             // Disable PLL (use Oscillator)
  PLL0FEED = 0xAA;                             // Feed Sequence Part #1
  PLL0FEED = 0x55;                             // Feed Sequence Part #2

  MEMMAP   = 0x01;                             // User Flash Mode

  return (0);
}
#endif

/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {
  return (0);
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {

#if defined(LPC11xx_32) || defined (LPC8xx_4)

  IAP.cmd    = 50;                             // Prepare Sector for Erase
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = 0;                              // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  IAP.cmd    = 52;                             // Erase Sector
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = 0;                              // End Sector
  IAP.par[2] = _CCLK;                          // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

#else
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
#endif

return (0);                                    // Finished without Errors
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {
  unsigned long n;

  n = GetSecNum(adr);                          // Get Sector Number

  IAP.cmd    = 50;                             // Prepare Sector for Erase
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  IAP.cmd    = 52;                             // Erase Sector
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP.par[2] = _CCLK;                          // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  return (0);                                  // Finished without Errors
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  unsigned long n;

#if NO_CRP != 0
  if (adr == 0) {
      n = *((unsigned long *)(buf + CRP_ADDRESS));
    if (IS_CRP_VALUE(n))
    {
        /* CRP is enabled, exit. */
        return 1;
    }
  }
#endif

#if SET_VALID_CODE != 0                        // Set valid User Code Signature
  if (adr == 0) {                              // Check for Vector Table
    n = *((unsigned long *)(buf + 0x00)) +
        *((unsigned long *)(buf + 0x04)) +
        *((unsigned long *)(buf + 0x08)) +
        *((unsigned long *)(buf + 0x0C)) +
        *((unsigned long *)(buf + 0x10)) +
        *((unsigned long *)(buf + 0x14)) +
        *((unsigned long *)(buf + 0x18));
    *((unsigned long *)(buf + 0x1C)) = 0 - n;  // Signature at Reserved Vector
  }
#endif

  n = GetSecNum(adr);                          // Get Sector Number

  IAP.cmd    = 50;                             // Prepare Sector for Write
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  IAP.cmd    = 51;                             // Copy RAM to Flash
  IAP.par[0] = adr;                            // Destination Flash Address
  IAP.par[1] = (unsigned long)buf;             // Source RAM Address
#if defined(LPC11xx_32) || defined(LPC8xx_4)
  IAP.par[2] = 256;                            // Fixed Page Size
#else
  IAP.par[2] = 512;                            // Fixed Page Size
#endif
  IAP.par[3] = _CCLK;                          // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

return (0);                                  // Finished without Errors
}
