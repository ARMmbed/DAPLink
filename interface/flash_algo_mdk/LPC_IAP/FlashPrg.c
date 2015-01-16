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

// Memory Mapping Control
#if defined(LPC11xx_32) || defined(LPC8xx_4) || defined(LPC11U68_256)
#define MEMMAP   (*((volatile unsigned long *) 0x40048000))
#else
#define MEMMAP   (*((volatile unsigned char *) 0x400FC040))
#endif

#ifdef MBED

#if defined(LPC11xx_32) || defined(LPC8xx_4) || defined(LPC11U68_256)
#define MAINCLKSEL (*((volatile unsigned long *) 0x40048070))
#define MAINCLKUEN (*((volatile unsigned long *) 0x40048074))
#define MAINCLKDIV (*((volatile unsigned long *) 0x40048078))

#elif defined(LPC1549_256)

#define SYSMEMREMAP    (*((volatile unsigned long *) 0x40074000))
#define MAINCLKSELA    (*((volatile unsigned long *) 0x40074080))
#define MAINCLKSELB    (*((volatile unsigned long *) 0x40074084))
#define SYSAHBCLKDIV   (*((volatile unsigned long *) 0x400740C0))
#define SYSAHBCLKCTRL0 (*((volatile unsigned long *) 0x400740C4))

#elif defined(LPC4337_1024)

#define BASE_M4_CLK (*((volatile unsigned long *) 0x4005006C))
#define PLL1_CTRL   (*((volatile unsigned long *) 0x40050044))

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
#ifdef LPC1549_256
#define END_SECTOR     63
#endif
#ifdef LPC11U68_256
#define END_SECTOR     28
#endif
#ifdef LPC4337_1024
#define END_SECTOR     14  /* 15 sectors per bank */
#define FLASH_BANK_A    0
#define FLASH_BANK_B    1
#define FLASH_BANK(_adr)  (((_adr) >= 0x80000) ? FLASH_BANK_B : FLASH_BANK_A)
#define FLASH_ADDR(_adr)  (((_adr) >= 0x80000) ? ((_adr) | 0x1B000000) : ((_adr) | 0x1A000000))
#endif

/* Code Read Protection (CRP) */
#define CRP_ADDRESS (0x000002FC)
#define NO_ISP        (0x4E697370)
#define CRP1        (0x12345678)
#define CRP2        (0x87654321)
#define CRP3        (0x43218765)
#define IS_CRP_VALUE(v) ((v==NO_ISP) || (v==CRP1) || (v==CRP2) || (v==CRP3))

#if defined(LPC11xx_32) || defined(LPC8xx_4) || defined(LPC1549_256) || defined(LPC11U68_256)
#define NO_CRP      (0)
#define _CCLK (12000)
#elif defined(LPC4337_1024)
#define NO_CRP      (0)
#define _CCLK (96000)
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
#if defined(LPC1549_256)
  #define IAP_Call ((IAP_Entry) 0x03000205)
#elif defined(LPC4337_1024)
  #define IAP_Call ((IAP_Entry) (*(volatile unsigned int *)(0x10400100)))
#else
#define IAP_Call ((IAP_Entry) 0x1FFF1FF1)
#endif


/*
 * Get Sector Number

 *    Parameter:      adr:  Sector Address
 *    Return Value:   Sector Number
 */

unsigned long GetSecNum (unsigned long adr) {
  unsigned long n;

#if defined(LPC8xx_4)
  n = adr >> 10;                               //  1kB Sector
#elif defined(LPC11xx_32) || defined(LPC1549_256)
  n = adr >> 12;                               //  4kB Sector
#elif defined(LPC11U68_256)
  n = adr >> 12;                               //  4kB Sector
  if (n >= 0x18) {
    n = 0x15 + (n >> 3);                       // 32kB Sector
  }
#elif defined(LPC4337_1024)
  if (adr >= 0x80000) {
    adr -= 0x80000;                            // Treat Flash Bank B as A
  }
  if (adr < 0x10000) {
    n = adr >> 13;                             //  8kB Sector
  } else {
    n = (adr >> 16) + 7;                       // 64kB Sector
  }
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

#if defined(LPC11xx_32) || defined(LPC8xx_4) || defined(LPC11U68_256)

  MAINCLKSEL = 0;                              // Select Internal RC Oscillator
  MAINCLKUEN = 1;                              // Update Main Clock Source
  MAINCLKUEN = 0;                              // Toggle Update Register
  MAINCLKUEN = 1;
  while (!(MAINCLKUEN & 1));                   // Wait until updated
  MAINCLKDIV = 1;                              // Set Main Clock divider to 1

  MEMMAP     = 0x02;                           // User Flash Mode

#elif defined(LPC1549_256)

  MAINCLKSELA    = 0;                          // Select Internal RC Oscillator
  MAINCLKSELB    = 0;                          // Select Internal RC Oscillator
  SYSAHBCLKDIV   = 1;                          // Set System Clock divider to 1
  SYSAHBCLKCTRL0 = 0x19b | (1<<9) | (1<<14) | (1<<15);   // Enable clocks, default+MUX+GPIO0+GPIO1
  SYSMEMREMAP    = 0x02;                       // User Flash Mode

#elif defined(LPC4337_1024)

  volatile unsigned int delay = 250;
  unsigned int reg;

  // Set BASE_M4_CLK to use IRC as clock in, and to use autoblock
  reg = BASE_M4_CLK;
  reg &= ~((0x1F << 24) | (1 << 11) | (1 << 0));
  reg |= (1 << 24) | (1 << 11);
  BASE_M4_CLK = reg;

  // Setup PLL to 96MHz using IRC (12MHz) * 8
  reg = PLL1_CTRL;
  reg &= ~0x1fff33c2;
  reg |=  0x01070040;
  PLL1_CTRL = reg & ~(1 << 0);

  /* Wait for 50uSec */
  while(delay--) {}

  IAP.cmd    = 49;                             // Initialize
  IAP.par[0] = 0;                              // 
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command

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
  IAP.par[1] = END_SECTOR;                     // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  IAP.cmd    = 52;                             // Erase Sector
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = END_SECTOR;                     // End Sector
  IAP.par[2] = _CCLK;                          // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

#elif defined(LPC4337_1024)

  IAP.cmd    = 50;                             // Prepare Sector for Erase
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = END_SECTOR;                     // End Sector
  IAP.par[2] = FLASH_BANK_A;                   // Flash Bank
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (0xea0000 | IAP.stat);  // Command Failed

  IAP.cmd    = 52;                             // Erase Sector
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = END_SECTOR;                     // End Sector
  IAP.par[2] = _CCLK;                          // CCLK in kHz
  IAP.par[3] = FLASH_BANK_A;                   // Flash Bank
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (0xea1000 | IAP.stat);  // Command Failed

  IAP.cmd    = 50;                             // Prepare Sector for Erase
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = END_SECTOR;                     // End Sector
  IAP.par[2] = FLASH_BANK_B;                   // Flash Bank
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (0xea2000 | IAP.stat);  // Command Failed

  IAP.cmd    = 52;                             // Erase Sector
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = END_SECTOR;                     // End Sector
  IAP.par[2] = _CCLK;                          // CCLK in kHz
  IAP.par[3] = FLASH_BANK_B;                   // Flash Bank
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (0xea3000 | IAP.stat);  // Command Failed

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

#if defined(LPC4337_1024)

  IAP.cmd    = 50;                             // Prepare Sector for Erase
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP.par[2] = FLASH_BANK(adr);                // Flash Bank
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (0xea4000 | IAP.stat);  // Command Failed

  IAP.cmd    = 52;                             // Erase Sector
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP.par[2] = _CCLK;                          // CCLK in kHz
  IAP.par[3] = FLASH_BANK(adr);                // Flash Bank
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (0xea5000 | IAP.stat);  // Command Failed

#else

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

#endif

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

#if defined(LPC4337_1024)

  IAP.cmd    = 50;                             // Prepare Sector for Write
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP.par[2] = FLASH_BANK(adr);                // Flash Bank
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (0xea6000 | IAP.stat);  // Command Failed

  IAP.cmd    = 51;                             // Copy RAM to Flash
  IAP.par[0] = FLASH_ADDR(adr);                // Destination Flash Address
  IAP.par[1] = (unsigned long)buf;             // Source RAM Address
  IAP.par[2] = 512;                            // Fixed Page Size
  IAP.par[3] = _CCLK;                          // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (0xea7000 | IAP.stat);  // Command Failed

#else

  IAP.cmd    = 50;                             // Prepare Sector for Write
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  IAP.cmd    = 51;                             // Copy RAM to Flash
  IAP.par[0] = adr;                            // Destination Flash Address
  IAP.par[1] = (unsigned long)buf;             // Source RAM Address
#if defined(LPC11xx_32) || defined(LPC8xx_4) || defined(LPC1549_256) || defined(LPC11U68_256)
  IAP.par[2] = 256;                            // Fixed Page Size
#else
  IAP.par[2] = 512;                            // Fixed Page Size
#endif
  IAP.par[3] = _CCLK;                          // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

#endif

return (0);                                  // Finished without Errors
}
