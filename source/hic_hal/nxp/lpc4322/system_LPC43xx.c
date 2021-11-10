/* -----------------------------------------------------------------------------
 * Copyright (c) 2013 - 2020 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * $Date:        20. Januar 2020
 * $Revision:    V1.0.4
 *
 * Project:      NXP LPC43xx System initialization
 * -------------------------------------------------------------------------- */

#include "LPC43xx.h"

/*----------------------------------------------------------------------------
  This file configures the clocks as follows:
 -----------------------------------------------------------------------------
 Clock Unit  |  Output clock  |  Source clock  |          Note
 -----------------------------------------------------------------------------
   PLL0USB   |    480 MHz     |      XTAL      | External crystal @ 12 MHz
 -----------------------------------------------------------------------------
    PLL1     |    120 MHz     |      XTAL      | External crystal @ 12 MHz
 -----------------------------------------------------------------------------
    CPU      |    120 MHz     |      PLL1      | CPU Clock ==  BASE_M4_CLK
 -----------------------------------------------------------------------------
   IDIV A    |     60 MHz     |      PLL1      | To the USB1 peripheral
 -----------------------------------------------------------------------------
   IDIV B    |     25 MHz     |   ENET_TX_CLK  | ENET_TX_CLK @ 50MHz
 -----------------------------------------------------------------------------
   IDIV C    |     12 MHz     |      IRC       | Internal oscillator @ 12 MHz
 -----------------------------------------------------------------------------
   IDIV D    |     12 MHz     |      IRC       | Internal oscillator @ 12 MHz
 -----------------------------------------------------------------------------
   IDIV E    |    5.2 MHz     |      PLL1      | To the LCD controller
 -----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
  Clock source selection definitions (do not change)
 *----------------------------------------------------------------------------*/
#define CLK_SRC_32KHZ       0x00
#define CLK_SRC_IRC         0x01
#define CLK_SRC_ENET_RX     0x02
#define CLK_SRC_ENET_TX     0x03
#define CLK_SRC_GP_CLKIN    0x04
#define CLK_SRC_XTAL        0x06
#define CLK_SRC_PLL0U       0x07
#define CLK_SRC_PLL0A       0x08
#define CLK_SRC_PLL1        0x09
#define CLK_SRC_IDIVA       0x0C
#define CLK_SRC_IDIVB       0x0D
#define CLK_SRC_IDIVC       0x0E
#define CLK_SRC_IDIVD       0x0F
#define CLK_SRC_IDIVE       0x10


/*----------------------------------------------------------------------------
  Define external input frequency values
 *----------------------------------------------------------------------------*/
#define CLK_32KHZ            32768UL    /* 32 kHz oscillator frequency        */
#define CLK_IRC           12000000UL    /* Internal oscillator frequency      */
#define CLK_ENET_RX       50000000UL    /* Ethernet Rx frequency              */
#define CLK_ENET_TX       50000000UL    /* Ethernet Tx frequency              */
#define CLK_GP_CLKIN      12000000UL    /* General purpose clock input freq.  */
#define CLK_XTAL          12000000UL    /* Crystal oscilator frequency        */


/*----------------------------------------------------------------------------
  Define clock sources
 *----------------------------------------------------------------------------*/
#define PLL1_CLK_SEL      CLK_SRC_XTAL    /* PLL1 input clock: XTAL           */
#define PLL0USB_CLK_SEL   CLK_SRC_XTAL    /* PLL0USB input clock: XTAL        */
#define IDIVA_CLK_SEL     CLK_SRC_PLL1    /* IDIVA input clock: PLL1          */
#define IDIVB_CLK_SEL     CLK_SRC_ENET_TX /* IDIVB input clock: ENET TX       */
#define IDIVC_CLK_SEL     CLK_SRC_IRC     /* IDIVC input clock: IRC           */
#define IDIVD_CLK_SEL     CLK_SRC_IRC     /* IDIVD input clock: IRC           */
#define IDIVE_CLK_SEL     CLK_SRC_PLL1    /* IDIVD input clock: PLL1          */


/*----------------------------------------------------------------------------
  Configure integer divider values
 *----------------------------------------------------------------------------*/
#define IDIVA_IDIV        1             /* Divide input clock by 2            */
#define IDIVB_IDIV        1             /* Divide input clock by 2            */
#define IDIVC_IDIV        0             /* Divide input clock by 1            */
#define IDIVD_IDIV        0             /* Divide input clock by 1            */
#define IDIVE_IDIV       22             /* Divide input clock by 23           */


/*----------------------------------------------------------------------------
  Define CPU clock input
 *----------------------------------------------------------------------------*/
#define CPU_CLK_SEL       CLK_SRC_PLL1  /* Default CPU clock source is PLL1   */


/*----------------------------------------------------------------------------
  Configure external memory controller options
 *----------------------------------------------------------------------------*/
#define USE_EXT_STAT_MEM_CS0 1          /* Use ext. static  memory with CS0   */
#define USE_EXT_DYN_MEM_CS0  1          /* Use ext. dynamic memory with CS0   */


/*----------------------------------------------------------------------------
 * Configure PLL1
 *----------------------------------------------------------------------------
 * Integer mode:
 *    - PLL1_DIRECT = 0 (Post divider enabled)
 *    - PLL1_FBSEL  = 1 (Feedback divider runs from PLL output)
 *    - Output frequency:
 *                        FCLKOUT = (FCLKIN / N) * M
 *                        FCCO    = FCLKOUT * 2 * P
 *
 * Non-integer:
 *    - PLL1_DIRECT = 0 (Post divider enabled)
 *    - PLL1_FBSEL  = 0 (Feedback divider runs from CCO clock)
 *    - Output frequency:
 *                        FCLKOUT = (FCLKIN / N) * M / (2 * P)
 *                        FCCO    = FCLKOUT * 2 * P
 *
 * Direct mode:
 *    - PLL1_DIRECT = 1         (Post divider disabled)
 *    - PLL1_FBSEL  = dont care (Feedback divider runs from CCO clock)
 *    - Output frequency:
 *                        FCLKOUT = (FCLKIN / N) * M
 *                        FCCO    = FCLKOUT
 *
 *----------------------------------------------------------------------------
 * PLL1 requirements:
 * | Frequency |  Minimum  |  Maximum  |               Note                   |
 * |  FCLKIN   |    1MHz   |   25MHz   |   Clock source is external crystal   |
 * |  FCLKIN   |    1MHz   |   50MHz   |                                      |
 * |   FCCO    |  156MHz   |  320MHz   |                                      |
 * |  FCLKOUT  | 9.75MHz   |  320MHz   |                                      |
 *----------------------------------------------------------------------------
 * Configuration examples:
 * | Fclkout |  Fcco  |  N  |  M  |  P  | DIRECT | FBSEL | BYPASS |
 * |  36MHz | 288MHz |  1  |  24 |  4  |   0    |   0   |    0   |
 * |  72MHz | 288MHz |  1  |  24 |  2  |   0    |   0   |    0   |
 * | 100MHz | 200MHz |  3  |  50 |  1  |   0    |   0   |    0   |
 * | 120MHz | 240MHz |  1  |  20 |  1  |   0    |   0   |    0   |
 * | 160MHz | 160MHz |  3  |  40 |  x  |   1    |   0   |    0   |
 * | 180MHz | 180MHz |  1  |  15 |  x  |   1    |   0   |    0   |
 * | 204MHz | 204MHz |  1  |  17 |  x  |   1    |   0   |    0   |
 *----------------------------------------------------------------------------
 * Relations beetwen PLL dividers and definitions:
 * N = PLL1_NSEL + 1,     M = PLL1_MSEL + 1,     P = 2 ^ PLL1_PSEL
 *----------------------------------------------------------------------------*/

/* PLL1 output clock: 120MHz, Fcco: 240MHz, N = 1, M = 20, P = 1              */
#define PLL1_NSEL   0           /* Range [0 -   3]: Pre-divider ratio N       */
#define PLL1_MSEL  19           /* Range [0 - 255]: Feedback-divider ratio M  */
#define PLL1_PSEL   0           /* Range [0 -   3]: Post-divider ratio P      */

#define PLL1_BYPASS 0           /* 0: Use PLL, 1: PLL is bypassed             */
#define PLL1_DIRECT 0           /* 0: Use PSEL, 1: Don't use PSEL             */
#define PLL1_FBSEL  0           /* 0: FCCO is used as PLL feedback            */
                                /* 1: FCLKOUT is used as PLL feedback         */

/*----------------------------------------------------------------------------
 * Configure Flash Accelerator
 *----------------------------------------------------------------------------
 * Flash acces time:
 * |  CPU clock   | FLASHTIM |
 * | up to  21MHz |    0     |
 * | up to  43MHz |    1     |
 * | up to  64MHz |    2     |
 * | up to  86MHz |    3     |
 * | up to 107MHz |    4     |
 * | up to 129MHz |    5     |
 * | up to 150MHz |    6     |
 * | up to 172MHz |    7     |
 * | up to 193MHz |    8     |
 * | up to 204MHz |    9     |
 *----------------------------------------------------------------------------*/
#define FLASHCFG_FLASHTIM   5


/*----------------------------------------------------------------------------
 * Configure PLL0USB
 *----------------------------------------------------------------------------
 *
 *   Normal operating mode without post-divider and without pre-divider
 *    - PLL0USB_DIRECTI = 1
 *    - PLL0USB_DIRECTO = 1
 *    - PLL0USB_BYPASS  = 0
 *    - Output frequency:
 *                        FOUT = FIN * 2 * M
 *                        FCCO = FOUT
 *
 *   Normal operating mode with post-divider and without pre-divider
 *    - PLL0USB_DIRECTI = 1
 *    - PLL0USB_DIRECTO = 0
 *    - PLL0USB_BYPASS  = 0
 *    - Output frequency:
 *                        FOUT = FIN * (M / P)
 *                        FCCO = FOUT * 2 * P
 *
 *   Normal operating mode without post-divider and with pre-divider
 *    - PLL0USB_DIRECTI = 0
 *    - PLL0USB_DIRECTO = 1
 *    - PLL0USB_BYPASS  = 0
 *    - Output frequency:
 *                        FOUT = FIN * 2 * M / N
 *                        FCCO = FOUT
 *
 *   Normal operating mode with post-divider and with pre-divider
 *    - PLL0USB_DIRECTI = 0
 *    - PLL0USB_DIRECTO = 0
 *    - PLL0USB_BYPASS  = 0
 *    - Output frequency:
 *                        FOUT = FIN * M / (P * N)
 *                        FCCO = FOUT * 2 * P
 *----------------------------------------------------------------------------
 * PLL0 requirements:
 * | Frequency |  Minimum  |  Maximum  |               Note                   |
 * |  FCLKIN   |   14kHz   |   25MHz   |   Clock source is external crystal   |
 * |  FCLKIN   |   14kHz   |  150MHz   |                                      |
 * |   FCCO    |  275MHz   |  550MHz   |                                      |
 * |  FCLKOUT  |  4.3MHz   |  550MHz   |                                      |
 *----------------------------------------------------------------------------
 * Configuration examples:
 * | Fclkout |  Fcco  |  N  |  M  |  P  | DIRECTI | DIRECTO | BYPASS |
 * | 120MHz | 480MHz |  x  |  20 |  2  |    1    |    0    |    0   |
 * | 480MHz | 480MHz |  1  |  20 |  1  |    1    |    1    |    0   |
 *----------------------------------------------------------------------------*/

/* PLL0USB output clock: 480MHz, Fcco: 480MHz, N = 1, M = 20, P = 1           */
#define PLL0USB_N       1       /* Range [1 -  256]: Pre-divider              */
#define PLL0USB_M      20       /* Range [1 - 2^15]: Feedback-divider         */
#define PLL0USB_P       1       /* Range [1 -   32]: Post-divider             */

#define PLL0USB_DIRECTI 1       /* 0: Use N_DIV, 1: Don't use N_DIV           */
#define PLL0USB_DIRECTO 1       /* 0: Use P_DIV, 1: Don't use P_DIV           */
#define PLL0USB_BYPASS  0       /* 0: Use PLL, 1: PLL is bypassed             */


/*----------------------------------------------------------------------------
  End of configuration
 *----------------------------------------------------------------------------*/

/* PLL0 Setting Check */
#if (PLL0USB_BYPASS == 0)
 #if (PLL0USB_CLK_SEL == CLK_SRC_XTAL)
  #define PLL0USB_CLKIN CLK_XTAL
 #else
  #define PLL0USB_CLKIN CLK_IRC
 #endif

 #if   ((PLL0USB_DIRECTI == 1) && (PLL0USB_DIRECTO == 1)) /* Mode 1a          */
  #define PLL0USB_FOUT (PLL0USB_CLKIN * 2 * PLL0USB_M)
  #define PLL0USB_FCCO (PLL0USB_FOUT)
 #elif ((PLL0USB_DIRECTI == 1) && (PLL0USB_DIRECTO == 0)) /* Mode 1b          */
  #define PLL0USB_FOUT (PLL0USB_CLKIN * PLL0USB_M / PLL0USB_P)
  #define PLL0USB_FCCO (PLL0USB_FOUT * 2 * PLL0USB_P)
 #elif ((PLL0USB_DIRECTI == 0) && (PLL0USB_DIRECTO == 1)) /* Mode 1c          */
  #define PLL0USB_FOUT (PLL0USB_CLKIN * 2 * PLL0USB_M / PLL0USB_N)
  #define PLL0USB_FCCO (PLL0USB_FOUT)
 #else                                                    /* Mode 1d          */
  #define PLL0USB_FOUT (PLL0USB_CLKIN * PLL0USB_M / (PLL0USB_P * PLL0USB_N))
  #define PLL0USB_FCCO (PLL0USB_FOUT * 2 * PLL0USB_P)
 #endif

 #if (PLL0USB_FCCO < 275000000UL || PLL0USB_FCCO > 550000000UL)
  #error "PLL0USB Fcco frequency out of range! (275MHz >= Fcco <= 550MHz)"
 #endif
 #if (PLL0USB_FOUT < 4300000UL || PLL0USB_FOUT > 550000000UL)
  #error "PLL0USB output frequency out of range! (4.3MHz >= Fclkout <= 550MHz)"
 #endif
#endif

/* PLL1 Setting Check */
#if (PLL1_BYPASS == 0)
 #if (PLL1_CLK_SEL == CLK_SRC_XTAL)
  #define PLL1_CLKIN CLK_XTAL
 #else
  #define PLL1_CLKIN CLK_IRC
 #endif

 #if   (PLL1_DIRECT == 1)               /* Direct Mode                        */
  #define PLL1_FCCO ((PLL1_MSEL + 1) * (PLL1_CLKIN / (PLL1_NSEL + 1)))
  #define PLL1_FOUT ((PLL1_MSEL + 1) * (PLL1_CLKIN / (PLL1_NSEL + 1)))
 #elif (PLL1_FBSEL  == 1)               /* Integer Mode                       */
  #define PLL1_FCCO ((2 * (1 << PLL1_PSEL)) * (PLL1_MSEL + 1) * (PLL1_CLKIN / (PLL1_NSEL + 1)))
  #define PLL1_FOUT ((PLL1_MSEL + 1) * (PLL1_CLKIN / (PLL1_NSEL + 1)))
 #else                                  /* Noninteger Mode                    */
  #define PLL1_FCCO ((PLL1_MSEL + 1) * (PLL1_CLKIN / (PLL1_NSEL + 1)))
  #define PLL1_FOUT (PLL1_FCCO / (2 * (1 << PLL1_PSEL)))
 #endif
 #if (PLL1_FCCO < 156000000UL || PLL1_FCCO > 320000000UL)
  #error "PLL1 Fcco frequency out of range! (156MHz >= Fcco <= 320MHz)"
 #endif
 #if (PLL1_FOUT < 9750000UL || PLL1_FOUT > 204000000UL)
  #error "PLL1 output frequency out of range! (9.75MHz >= Fclkout <= 204MHz)"
 #endif
#endif

/*----------------------------------------------------------------------------
  Function prototypes
 *----------------------------------------------------------------------------*/
uint32_t MeasureFreq (uint32_t clk_sel);
uint32_t GetClockFreq (uint32_t clk_src);

/*----------------------------------------------------------------------------
  System Core Clock variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = 120000000U; /* System Clock Frequency (Core Clock) */


/******************************************************************************
 * SetClock
 ******************************************************************************/
static void SetClock (void) {
  uint32_t x, i;
  uint32_t selp, seli;

  /* Set flash accelerator configuration for bank A and B to reset value      */
  LPC_CREG->FLASHCFGA |= (0xF << 12);
  LPC_CREG->FLASHCFGB |= (0xF << 12);

  /* Set flash wait states to maximum                                         */
  LPC_EMC->STATICWAITRD0  = 0x1F;

  /* Switch BASE_M4_CLOCK to IRC                                              */
  LPC_CGU->BASE_M4_CLK = (0x01        << 11) |  /* Autoblock En               */
                         (CLK_SRC_IRC << 24) ;  /* Set clock source           */

  /* Configure input to crystal oscilator                                     */
  LPC_CGU->XTAL_OSC_CTRL = (0 << 0) |   /* Enable oscillator-pad              */
                           (0 << 1) |   /* Operation with crystal connected   */
                           (0 << 2) ;   /* Low-frequency mode                 */

  /* Wait ~250us @ 12MHz */
  for (i = 1500; i; i--);

#ifdef USE_SPIFI
/* configure SPIFI clk to IRC via IDIVA (later IDIVA is configured to PLL1/3) */
  LPC_CGU->IDIVA_CTRL     = (0              <<  0) |  /* Disable Power-down   */
                            (0              <<  2) |  /* IDIV                 */
                            (1              << 11) |  /* Autoblock En         */
                            (CLK_SRC_IRC    << 24) ;  /* Clock source         */

  LPC_CGU->BASE_SPIFI_CLK = (0              <<  0) |  /* Disable Power-down   */
                            (0              <<  2) |  /* IDIV                 */
                            (1              << 11) |  /* Autoblock En         */
                            (CLK_SRC_IDIVA  << 24) ;  /* Clock source         */
#endif

/*----------------------------------------------------------------------------
  PLL1 Setup
 *----------------------------------------------------------------------------*/
  /* Power down PLL                                                           */
  LPC_CGU->PLL1_CTRL |= 1;

#if ((PLL1_FOUT > 110000000UL) && (CPU_CLK_SEL == CLK_SRC_PLL1))
  /* To run at full speed, CPU must first run at an intermediate speed        */
  LPC_CGU->PLL1_CTRL = (0            << 0) | /* PLL1 Enabled                  */
                       (PLL1_BYPASS  << 1) | /* CCO out sent to post-dividers */
                       (PLL1_FBSEL   << 6) | /* PLL output used as feedback   */
                       (0            << 7) | /* Direct on/off                 */
                       (PLL1_PSEL    << 8) | /* PSEL                          */
                       (0            << 11)| /* Autoblock Disabled            */
                       (PLL1_NSEL    << 12)| /* NSEL                          */
                       (PLL1_MSEL    << 16)| /* MSEL                          */
                       (PLL1_CLK_SEL << 24); /* Clock source                  */
  /* Wait for lock                                                            */
  while (!(LPC_CGU->PLL1_STAT & 1));

  /* CPU base clock is in the mid frequency range before final clock set      */
  LPC_CGU->BASE_M4_CLK     = (0x01 << 11) |  /* Autoblock En                  */
                             (0x09 << 24) ;  /* Clock source: PLL1            */

  /* Max. BASE_M4_CLK frequency here is 102MHz, wait at least 20us */
  for (i = 1050; i; i--);                    /* Wait minimum 2100 cycles      */
#endif
  /* Configure PLL1                                                           */
  LPC_CGU->PLL1_CTRL = (0            << 0) | /* PLL1 Enabled                  */
                       (PLL1_BYPASS  << 1) | /* CCO out sent to post-dividers */
                       (PLL1_FBSEL   << 6) | /* PLL output used as feedback   */
                       (PLL1_DIRECT  << 7) | /* Direct on/off                 */
                       (PLL1_PSEL    << 8) | /* PSEL                          */
                       (1            << 11)| /* Autoblock En                  */
                       (PLL1_NSEL    << 12)| /* NSEL                          */
                       (PLL1_MSEL    << 16)| /* MSEL                          */
                       (PLL1_CLK_SEL << 24); /* Clock source                  */

  /* Wait for lock                                                            */
  while (!(LPC_CGU->PLL1_STAT & 1));

  /* Set CPU base clock source                                                */
  LPC_CGU->BASE_M4_CLK = (0x01        << 11) |  /* Autoblock En               */
                         (CPU_CLK_SEL << 24) ;  /* Set clock source           */

  /* Set flash accelerator configuration for internal flash bank A and B      */
  LPC_CREG->FLASHCFGA = (LPC_CREG->FLASHCFGA & (~0x0000F000U)) | (FLASHCFG_FLASHTIM << 12);
  LPC_CREG->FLASHCFGB = (LPC_CREG->FLASHCFGB & (~0x0000F000U)) | (FLASHCFG_FLASHTIM << 12);

/*----------------------------------------------------------------------------
  PLL0USB Setup
 *----------------------------------------------------------------------------*/

  /* Power down PLL0USB                                                       */
  LPC_CGU->PLL0USB_CTRL  |= 1;

  /* M divider                                                                */
  x = 0x00004000;
  switch (PLL0USB_M) {
    case 0:  x = 0xFFFFFFFF;
      break;
    case 1:  x = 0x00018003;
      break;
    case 2:  x = 0x00010003;
      break;
    default:
      for (i = PLL0USB_M; i <= 0x8000; i++) {
        x = (((x ^ (x >> 1)) & 1) << 14) | ((x >> 1) & 0x3FFF);
      }
  }

  if (PLL0USB_M < 60) selp = (PLL0USB_M >> 1) + 1;
  else        selp = 31;

  if      (PLL0USB_M > 16384) seli = 1;
  else if (PLL0USB_M >  8192) seli = 2;
  else if (PLL0USB_M >  2048) seli = 4;
  else if (PLL0USB_M >=  501) seli = 8;
  else if (PLL0USB_M >=   60) seli = 4 * (1024 / (PLL0USB_M + 9));
  else                        seli = (PLL0USB_M & 0x3C) + 4;
  LPC_CGU->PLL0USB_MDIV   =  (selp   << 17) |
                             (seli   << 22) |
                             (x      <<  0);

  /* N divider                                                                */
  x = 0x80;
  switch (PLL0USB_N) {
    case 0:  x = 0xFFFFFFFF;
      break;
    case 1:  x = 0x00000302;
      break;
    case 2:  x = 0x00000202;
      break;
    default:
      for (i = PLL0USB_N; i <= 0x0100; i++) {
        x =(((x ^ (x >> 2) ^ (x >> 3) ^ (x >> 4)) & 1) << 7) | ((x >> 1) & 0x7F);
      }
  }
  LPC_CGU->PLL0USB_NP_DIV = (x << 12);

  /* P divider                                                                */
  x = 0x10;
  switch (PLL0USB_P) {
    case 0:  x = 0xFFFFFFFF;
      break;
    case 1:  x = 0x00000062;
      break;
    case 2:  x = 0x00000042;
      break;
    default:
      for (i = PLL0USB_P; i <= 0x200; i++) {
        x = (((x ^ (x >> 2)) & 1) << 4) | ((x >> 1) &0x0F);
      }
  }
  LPC_CGU->PLL0USB_NP_DIV |= x;

  LPC_CGU->PLL0USB_CTRL  = (PLL0USB_CLK_SEL   << 24) | /* Clock source sel    */
                           (1                 << 11) | /* Autoblock En        */
                           (1                 << 4 ) | /* PLL0USB clock en    */
                           (PLL0USB_DIRECTO   << 3 ) | /* Direct output       */
                           (PLL0USB_DIRECTI   << 2 ) | /* Direct input        */
                           (PLL0USB_BYPASS    << 1 ) | /* PLL bypass          */
                           (0                 << 0 ) ; /* PLL0USB Enabled     */
  while (!(LPC_CGU->PLL0USB_STAT & 1));


/*----------------------------------------------------------------------------
  Integer divider Setup
 *----------------------------------------------------------------------------*/

  /* Configure integer dividers                                               */
  LPC_CGU->IDIVA_CTRL = (0              <<  0) |  /* Disable Power-down       */
                        (IDIVA_IDIV     <<  2) |  /* IDIV                     */
                        (1              << 11) |  /* Autoblock En             */
                        (IDIVA_CLK_SEL  << 24) ;  /* Clock source             */

  LPC_CGU->IDIVB_CTRL = (0              <<  0) |  /* Disable Power-down       */
                        (IDIVB_IDIV     <<  2) |  /* IDIV                     */
                        (1              << 11) |  /* Autoblock En             */
                        (IDIVB_CLK_SEL  << 24) ;  /* Clock source             */

  LPC_CGU->IDIVC_CTRL = (0              <<  0) |  /* Disable Power-down       */
                        (IDIVC_IDIV     <<  2) |  /* IDIV                     */
                        (1              << 11) |  /* Autoblock En             */
                        (IDIVC_CLK_SEL  << 24) ;  /* Clock source             */

  LPC_CGU->IDIVD_CTRL = (0              <<  0) |  /* Disable Power-down       */
                        (IDIVD_IDIV     <<  2) |  /* IDIV                     */
                        (1              << 11) |  /* Autoblock En             */
                        (IDIVD_CLK_SEL  << 24) ;  /* Clock source             */

  LPC_CGU->IDIVE_CTRL = (0              <<  0) |  /* Disable Power-down       */
                        (IDIVE_IDIV     <<  2) |  /* IDIV                     */
                        (1              << 11) |  /* Autoblock En             */
                        (IDIVE_CLK_SEL  << 24) ;  /* Clock source             */
}


/*----------------------------------------------------------------------------
  Approximate delay function (must be used after SystemCoreClockUpdate() call)
 *----------------------------------------------------------------------------*/
#define CPU_NANOSEC(x) (((uint64_t)(x) * SystemCoreClock)/1000000000)

static void WaitUs (uint32_t us) {
  uint32_t cyc = us * CPU_NANOSEC(1000)/4;
  while(cyc--);
}


/*----------------------------------------------------------------------------
  Measure frequency using frequency monitor
 *----------------------------------------------------------------------------*/
uint32_t MeasureFreq (uint32_t clk_sel) {
  uint32_t fcnt, rcnt, fout;

  /* Set register values */
  LPC_CGU->FREQ_MON &= ~(1U << 23);               /* Stop frequency counters  */
  LPC_CGU->FREQ_MON  = (clk_sel << 24) | 511;     /* RCNT == 511              */
  LPC_CGU->FREQ_MON |= (1 << 23);                 /* Start RCNT and FCNT      */
  while (LPC_CGU->FREQ_MON & (1 << 23)) {
    fcnt = (LPC_CGU->FREQ_MON >> 9) & 0x3FFF;
    rcnt = (LPC_CGU->FREQ_MON     ) & 0x01FF;
    if (fcnt == 0 && rcnt == 0) {
      return (0);                                 /* No input clock present   */
    }
  }
  fcnt = (LPC_CGU->FREQ_MON >> 9) & 0x3FFF;
  fout = fcnt * (12000000U/511U);                 /* FCNT * (IRC_CLK / RCNT)  */

  return (fout);
}


/*----------------------------------------------------------------------------
  Get PLL1 (divider and multiplier) parameters
 *----------------------------------------------------------------------------*/
static __inline uint32_t GetPLL1Param (void) {
  uint32_t ctrl;
  uint32_t p;
  uint32_t div, mul;

  ctrl = LPC_CGU->PLL1_CTRL;
  div = ((ctrl >> 12) & 0x03) + 1;
  mul = ((ctrl >> 16) & 0xFF) + 1;
  p = 1 << ((ctrl >>  8) & 0x03);

  if (ctrl & (1 << 1)) {
    /* Bypass = 1, PLL1 input clock sent to post-dividers */
    if (ctrl & (1 << 7)) {
      div *= (2*p);
    }
  }
  else {
    /* Direct and integer mode */
    if (((ctrl & (1 << 7)) == 0) && ((ctrl & (1 << 6)) == 0)) {
      /* Non-integer mode */
      div *= (2*p);
    }
  }
  return ((div << 8) | (mul));
}


/*----------------------------------------------------------------------------
  Get input clock source for specified clock generation block
 *----------------------------------------------------------------------------*/
static int32_t GetClkSel (uint32_t clk_src) {
  uint32_t reg;
  int32_t clk_sel = -1;

  switch (clk_src) {
    case CLK_SRC_IRC:
    case CLK_SRC_ENET_RX:
    case CLK_SRC_ENET_TX:
    case CLK_SRC_GP_CLKIN:
      return ( (int32_t) clk_src);

    case CLK_SRC_32KHZ:
      return ((LPC_CREG->CREG0 & 0x0A) != 0x02) ? (-1) : (CLK_SRC_32KHZ);
    case CLK_SRC_XTAL:
     return  (LPC_CGU->XTAL_OSC_CTRL & 1)       ? (-1) : (CLK_SRC_XTAL);

    case CLK_SRC_PLL0U: reg = LPC_CGU->PLL0USB_CTRL;    break;
    case CLK_SRC_PLL0A: reg = LPC_CGU->PLL0AUDIO_CTRL;  break;
    case CLK_SRC_PLL1:  reg = (LPC_CGU->PLL1_STAT & 1) ? (LPC_CGU->PLL1_CTRL) : (0); break;

    case CLK_SRC_IDIVA: reg = LPC_CGU->IDIVA_CTRL;      break;
    case CLK_SRC_IDIVB: reg = LPC_CGU->IDIVB_CTRL;      break;
    case CLK_SRC_IDIVC: reg = LPC_CGU->IDIVC_CTRL;      break;
    case CLK_SRC_IDIVD: reg = LPC_CGU->IDIVD_CTRL;      break;
    case CLK_SRC_IDIVE: reg = LPC_CGU->IDIVE_CTRL;      break;

    default:
      return (clk_sel);
  }
  if (!(reg & 1)) {
    clk_sel = (reg >> 24) & 0x1F;
  }
  return (clk_sel);
}


/*----------------------------------------------------------------------------
  Get clock frequency for specified clock source
 *----------------------------------------------------------------------------*/
uint32_t GetClockFreq (uint32_t clk_src) {
  uint32_t tmp;
  uint32_t mul        =  1;
  uint32_t div        =  1;
  uint32_t main_freq  =  0;
  int32_t  clk_sel    = (int32_t) clk_src;

  do {
    switch (clk_sel) {
      case CLK_SRC_32KHZ:    main_freq = CLK_32KHZ;     break;
      case CLK_SRC_IRC:      main_freq = CLK_IRC;       break;
      case CLK_SRC_ENET_RX:  main_freq = CLK_ENET_RX;   break;
      case CLK_SRC_ENET_TX:  main_freq = CLK_ENET_TX;   break;
      case CLK_SRC_GP_CLKIN: main_freq = CLK_GP_CLKIN;  break;
      case CLK_SRC_XTAL:     main_freq = CLK_XTAL;      break;

      case CLK_SRC_IDIVA: div *= ((LPC_CGU->IDIVA_CTRL >> 2) & 0x03) + 1; break;
      case CLK_SRC_IDIVB: div *= ((LPC_CGU->IDIVB_CTRL >> 2) & 0x0F) + 1; break;
      case CLK_SRC_IDIVC: div *= ((LPC_CGU->IDIVC_CTRL >> 2) & 0x0F) + 1; break;
      case CLK_SRC_IDIVD: div *= ((LPC_CGU->IDIVD_CTRL >> 2) & 0x0F) + 1; break;
      case CLK_SRC_IDIVE: div *= ((LPC_CGU->IDIVE_CTRL >> 2) & 0xFF) + 1; break;

      case CLK_SRC_PLL0U: /* Not implemented */  break;
      case CLK_SRC_PLL0A: /* Not implemented */  break;

      case CLK_SRC_PLL1:
        tmp = GetPLL1Param ();
        mul *= (tmp     ) & 0xFF;       /* PLL input clock multiplier         */
        div *= (tmp >> 8) & 0xFF;       /* PLL input clock divider            */
        break;

      default:
        return (0);                     /* Clock not running or not supported */
    }
    if (main_freq == 0) {
      clk_sel = GetClkSel ( (uint32_t) clk_sel);
    }
  }
  while (main_freq == 0);

  return ((main_freq * mul) / div);
}


/*----------------------------------------------------------------------------
  System Core Clock update
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void) {
  /* Check BASE_M4_CLK connection */
  uint32_t base_src = (LPC_CGU->BASE_M4_CLK >> 24) & 0x1F;

  /* Update core clock frequency */
  SystemCoreClock = GetClockFreq (base_src);
}

#if defined(__ARMCC)
extern uint32_t __Vectors;                         /* see startup_LPC43xx.s   */
#endif

/*----------------------------------------------------------------------------
  Initialize the system
 *----------------------------------------------------------------------------*/
void SystemInit (void) {

  #if (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2) |                 /* set CP10 Full Access */
                   (3UL << 11*2)  );               /* set CP11 Full Access */
  #endif

  /* Stop CM0 core */
  LPC_RGU->RESET_CTRL1 = (1 << 24);

  /* Disable SysTick timer                                                    */
  SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);

  /* Set vector table pointer */
#if defined(__ARMCC)
  SCB->VTOR = ((uint32_t)(&__Vectors)) & 0xFFF00000UL;
#endif

  /* Configure PLL0 and PLL1, connect CPU clock to selected clock source */
  SetClock();

  /* Update SystemCoreClock variable */
  SystemCoreClockUpdate();
}

void SystemReset(void)
{
    /* Ensure all outstanding memory accesses included buffered write are completed before reset */
    __DSB();

    LPC_WWDT->MOD |= (1 << 1);
    LPC_WWDT->MOD |= (1 << 0);
    LPC_WWDT->FEED = 0xAA;
    LPC_WWDT->FEED = 0x55;

    /* Ensure completion of memory access */
    __DSB();

    /* wait until reset */
    while(1);
}
