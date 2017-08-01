/**
 *******************************************************************************
 * @file    system_TMPM366.c
 * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer Source File for the
 *          TOSHIBA 'TMPM366' Device Series
 * @version V2.0.2.5
 * @date    2014/05/20
 *
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
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
 *******************************************************************************
 */

#include "TMPM366.h"

/*-------- <<< Start of configuration section >>> ----------------------------*/

/* Watchdog Timer (WD) Configuration */
#define WD_SETUP    1
#define WDMOD_Val   (0x00000000UL)
#define WDCR_Val    (0x000000B1UL)

/* Clock Generator (CG) Configuration */
#define CLOCK_SETUP      1
#define SYSCR_Val        (0x00010000UL)
#define OSCCR_Val        (0x00060134UL) /* OSCCR<OSCSEL> = 1, OSCCR<XEN2> = 0, OSCCR<XEN1> = 1, OSCCR<PLLON> = 1, <EHOSCSEL> = 1 */

#define PLLSEL_Ready     (0x0000703CUL)
#define PLLSEL_Val       (0x0000703DUL)


/*-------- <<< End of configuration section >>> ------------------------------*/

/*-------- DEFINES -----------------------------------------------------------*/
/* Define clocks */
#define EXTALH (12000000UL)      /* External high-speed oscillator freq */
#define XTALH  (10000000UL)       /* Internal high-speed oscillator freq */

/* Configure Warm-up time */
#define HZ_1M                   1000000UL
#define WU_TIME_EXT             5000UL          /* warm-up time for EXT is 5ms   */
#define WU_TIME_PLL             100UL           /* warm-up time for PLL is 100us */
#define OSCCR_WUODR_MASK        (0x000FFFFFUL)
#define OSCCR_WUODR_EXT         ((WU_TIME_EXT * EXTALH / HZ_1M / 16U) << 20U) /* OSCCR<WUPODR11:0> = warm-up time(us) * EXTALH(MHz) / 16 */
#define OSCCR_WUODR_PLL         ((WU_TIME_PLL * EXTALH / HZ_1M / 16U) << 20U)

/* Determine core clock frequency according to settings */
/* System clock is high-speed clock*/
#if (OSCCR_Val & (1U<<17))
  #define CORE_TALH (EXTALH)
#else
  #define CORE_TALH (XTALH)
#endif
#if ((PLLSEL_Val & (1U<<0)) && (OSCCR_Val & (1U<<2))) /* If PLL selected and enabled */
  #if ((PLLSEL_Val & 0x0000FFFE) == 0x0000703CUL)     /* If PLL set as 8 times */
    #if   ((SYSCR_Val & 7U) == 0U)      /* Gear -> fc                         */
      #define __CORE_CLK   (CORE_TALH * 8U / 2U)
    #elif ((SYSCR_Val & 7U) == 4U)      /* Gear -> fc/2                       */
      #define __CORE_CLK   (CORE_TALH * 8U / 2U / 2U)
    #elif ((SYSCR_Val & 7U) == 5U)      /* Gear -> fc/4                       */
      #define __CORE_CLK   (CORE_TALH * 8U / 2U / 4U)
    #elif ((SYSCR_Val & 7U) == 6U)      /* Gear -> fc/8                       */
      #define __CORE_CLK   (CORE_TALH * 8U / 2U / 8U)
    #elif ((SYSCR_Val & 7U) == 7U)      /* Gear -> fc/16                      */
      #define __CORE_CLK   (CORE_TALH * 8U / 2U / 16U)
    #else                               /* Gear -> reserved                   */
      #define __CORE_CLK   (0U)
    #endif
  #elif ((PLLSEL_Val & 0x0000FFFE) == 0x0000702CUL)     /* If PLL set as 6 times */
    #if   ((SYSCR_Val & 7U) == 0U)      /* Gear -> fc                         */
      #define __CORE_CLK   (CORE_TALH * 6U / 2U)
    #elif ((SYSCR_Val & 7U) == 4U)      /* Gear -> fc/2                       */
      #define __CORE_CLK   (CORE_TALH * 6U / 2U / 2U)
    #elif ((SYSCR_Val & 7U) == 5U)      /* Gear -> fc/4                       */
      #define __CORE_CLK   (CORE_TALH * 6U / 2U / 4U)
    #elif ((SYSCR_Val & 7U) == 6U)      /* Gear -> fc/8                       */
      #define __CORE_CLK   (CORE_TALH * 6U / 2U / 8U)
    #elif ((SYSCR_Val & 7U) == 7U)      /* Gear -> fc/16                      */
      #define __CORE_CLK   (CORE_TALH * 6U / 2U / 16U)
    #else                               /* Gear -> reserved                   */
      #define __CORE_CLK   (0U)
    #endif
  #endif
#else
  #if   ((SYSCR_Val & 7U) == 0U)      /* Gear -> fc                         */
    #define __CORE_CLK   (CORE_TALH)
  #elif ((SYSCR_Val & 7U) == 4U)      /* Gear -> fc/2                       */
    #define __CORE_CLK   (CORE_TALH / 2U)
  #elif ((SYSCR_Val & 7U) == 5U)      /* Gear -> fc/4                       */
    #define __CORE_CLK   (CORE_TALH / 4U)
  #elif ((SYSCR_Val & 7U) == 6U)      /* Gear -> fc/8                       */
    #define __CORE_CLK   (CORE_TALH / 8U)
  #else                               /* Gear -> reserved                   */
    #define __CORE_CLK   (0U)
  #endif
#endif                                /* If PLL not used                    */


/* Clock Variable definitions */
uint32_t SystemCoreClock = __CORE_CLK;/*!< System Clock Frequency (Core Clock)*/


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Update SystemCoreClock according register values.
 */
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
  uint32_t CoreClock = 0U;
  /* Determine clock frequency according to clock register values        */
  /* System clock is high-speed clock */
 if(TSB_CG_OSCCR_OSCSEL){  /* If system clock is External high-speed oscillator freq */
     CoreClock = EXTALH;
 }
 else{                 /* If system clock is Internal high-speed oscillator freq */
     CoreClock = XTALH;
 }
 if (TSB_CG_PLLSEL_PLLSEL && TSB_CG_OSCCR_PLLON) {   /* If PLL enabled      */
   if ((TSB_CG->PLLSEL & 0x0000FFFE) == 0x0000703CUL) {         /* If PLL set as 8 times */
     switch (TSB_CG->SYSCR & 7U) {
       case 0U:                                 /* Gear -> fc          */
         SystemCoreClock = CoreClock * 8U / 2U;
         break;
       case 1U:
       case 2U:
       case 3U:                                 /* Gear -> reserved    */
         SystemCoreClock = 0U;
         break;
       case 4U:                                 /* Gear -> fc/2        */
         SystemCoreClock = CoreClock * 8U / 2U / 2U;
         break;
       case 5U:                                 /* Gear -> fc/4        */
         SystemCoreClock = CoreClock * 8U / 2U / 4U;
         break;
       case 6U:                                 /* Gear -> fc/8        */
         SystemCoreClock = CoreClock * 8U / 2U / 8U;
         break;
       case 7U:                                 /* Gear -> fc/16       */
         SystemCoreClock = CoreClock * 8U / 2U / 16U;
         break;
       default:
         SystemCoreClock = 0U;
         break;
     }
    } else if ((TSB_CG->PLLSEL & 0x0000FFFE) == 0x0000702CUL){     /* If PLL set as 6 times */
     switch (TSB_CG->SYSCR & 7U) {
       case 0U:                                 /* Gear -> fc          */
         SystemCoreClock = CoreClock * 6U / 2U;
         break;
       case 1U:
       case 2U:
       case 3U:                                /* Gear -> reserved   */
         SystemCoreClock = 0U;
         break;
       case 4U:                                 /* Gear -> fc/2      */
         SystemCoreClock = CoreClock * 6U / 2U / 2U;
         break;
       case 5U:                                 /* Gear -> fc/4      */
         SystemCoreClock = CoreClock * 6U / 2U / 4U;
         break;
       case 6U:                                 /* Gear -> fc/8      */
         SystemCoreClock = CoreClock * 6U / 2U / 8U;
         break;
       case 7U:                                 /* Gear -> fc/16     */
         SystemCoreClock = CoreClock * 6U / 2U / 16U;
         break;
       default:
         SystemCoreClock = 0U;
         break;
     }
   }
 } else {   /* If PLL not used                    */
   switch (TSB_CG->SYSCR & 7U) {
     case 0U:                                 /* Gear -> fc          */
       SystemCoreClock = CoreClock;
       break;
     case 1U:
     case 2U:
     case 3U:
     case 7U:                                 /* Gear -> reserved    */
       SystemCoreClock = 0U;
       break;
     case 4U:                                 /* Gear -> fc/2        */
       SystemCoreClock = CoreClock / 2U;
       break;
     case 5U:                                 /* Gear -> fc/4        */
       SystemCoreClock = CoreClock / 4U;
       break;
     case 6U:                                 /* Gear -> fc/8        */
       SystemCoreClock = CoreClock / 8U;
       break;
     default:
       SystemCoreClock = 0U;
       break;
   }
 }
}
/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit(void)
{
#if (WD_SETUP)                  /* Watchdog Setup */
  TSB_WD->MOD = WDMOD_Val;
  if (!TSB_WD_MOD_WDTE) {           /* If watchdog is to be disabled */
    TSB_WD->CR = WDCR_Val;
  }
#endif

#if (CLOCK_SETUP)               /* Clock(external) Setup */
  TSB_CG->SYSCR = SYSCR_Val;
  TSB_CG->OSCCR &= OSCCR_WUODR_MASK;
  TSB_CG->OSCCR |= OSCCR_WUODR_EXT;
  TSB_CG_OSCCR_EHOSCSEL = 1U;
  TSB_CG_OSCCR_XEN1 = 1U;
  TSB_CG_OSCCR_HWUPSEL = 1U;
  TSB_CG_OSCCR_WUEON = 1U;
  TSB_CG->PLLSEL = PLLSEL_Ready;
  while(TSB_CG_OSCCR_WUEF){}        /* Warm-up */
  TSB_CG_OSCCR_OSCSEL = 1U;
  TSB_CG_OSCCR_XEN2 = 0U;

  TSB_CG->OSCCR &= OSCCR_WUODR_MASK;
  TSB_CG->OSCCR |= OSCCR_WUODR_PLL;
  TSB_CG_OSCCR_PLLON = 1U;          /* PLL enabled */
  TSB_CG_OSCCR_WUEON = 1U;
  while(TSB_CG_OSCCR_WUEF){}        /* Warm-up */

  TSB_CG->PLLSEL = PLLSEL_Val;
#endif
}

