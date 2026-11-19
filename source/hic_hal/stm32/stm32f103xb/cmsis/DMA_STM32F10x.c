/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2020 Arm Limited (or its affiliates). All 
 * rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        27. November 2020
 * $Revision:    V1.3
 *  
 * Project:      DMA Driver for ST STM32F10x
 * -------------------------------------------------------------------- */

/* History:
 *  Version 1.3
 *    Added support for ARM Compiler 6
 *  Version 1.2
 *    DMAx_Channel variable is checked for initialized channels in DMA_ChannelUninitialize function
 *  Version 1.1
 *    DMA2 peripheral added to build when HD, XL, HD_VL or CL device is used
 *  Version 1.0
 *    Initial release
 */

#include "DMA_STM32F10x.h"

#if defined(STM32F10X_HD)    || defined(STM32F10X_XL) || \
    defined(STM32F10X_HD_VL) || defined(STM32F10X_CL)
  /* DMA2 peripheral exists on HD, XL, HD_VL and CL devices */
  #define STM32F10X_ENABLE_DMA2
#endif

/// DMA Variables
static uint8_t DMA1_Channel;             // Used DMA1 Channels
#if defined (STM32F10X_ENABLE_DMA2)
static uint8_t DMA2_Channel;             // Used DMA2 Channels
#endif


/// DMA Functions

/**
  \fn          void DMA_ChannelInitialize (uint32_t dma, uint32_t channel)
  \brief       Initialize DMA Channel
  \param[in]   dma     DMA number
  \param[in]   channel DMA channel number
*/
void DMA_ChannelInitialize (uint32_t dma, uint32_t channel) {
  
  switch (dma) {
    case 1:
      RCC->AHBENR |=  RCC_AHBENR_DMA1EN;
      switch (channel) {
        case 1:
          DMA1_Channel1->CCR = 0;
          DMA1->IFCR = DMA_IFCR_CGIF1  | DMA_IFCR_CTEIF1 |
                       DMA_IFCR_CHTIF1 | DMA_IFCR_CTCIF1;
          NVIC_ClearPendingIRQ(DMA1_Channel1_IRQn);
          NVIC_EnableIRQ(DMA1_Channel1_IRQn);
          break;
        case 2:
          DMA1_Channel2->CCR = 0;
          DMA1->IFCR = DMA_IFCR_CGIF2  | DMA_IFCR_CTEIF2 |
                       DMA_IFCR_CHTIF2 | DMA_IFCR_CTCIF2;
          NVIC_ClearPendingIRQ(DMA1_Channel2_IRQn);
          NVIC_EnableIRQ(DMA1_Channel2_IRQn);
          break;
        case 3:
          DMA1_Channel3->CCR = 0;
          DMA1->IFCR = DMA_IFCR_CGIF3  | DMA_IFCR_CTEIF3 |
                       DMA_IFCR_CHTIF3 | DMA_IFCR_CTCIF3;
          NVIC_ClearPendingIRQ(DMA1_Channel3_IRQn);
          NVIC_EnableIRQ(DMA1_Channel3_IRQn);
          break;
        case 4:
          DMA1_Channel4->CCR = 0;
          DMA1->IFCR = DMA_IFCR_CGIF4  | DMA_IFCR_CTEIF4 |
                       DMA_IFCR_CHTIF4 | DMA_IFCR_CTCIF4;
          NVIC_ClearPendingIRQ(DMA1_Channel4_IRQn);
          NVIC_EnableIRQ(DMA1_Channel4_IRQn);
          break;
        case 5:
          DMA1_Channel5->CCR = 0;
          DMA1->IFCR = DMA_IFCR_CGIF5  | DMA_IFCR_CTEIF5 |
                       DMA_IFCR_CHTIF5 | DMA_IFCR_CTCIF5;
          NVIC_ClearPendingIRQ(DMA1_Channel5_IRQn);
          NVIC_EnableIRQ(DMA1_Channel5_IRQn);
          break;
        case 6:
          DMA1_Channel6->CCR = 0;
          DMA1->IFCR = DMA_IFCR_CGIF6  | DMA_IFCR_CTEIF6 |
                       DMA_IFCR_CHTIF6 | DMA_IFCR_CTCIF6;
          NVIC_ClearPendingIRQ(DMA1_Channel6_IRQn);
          NVIC_EnableIRQ(DMA1_Channel6_IRQn);
          break;
        case 7:
          DMA1_Channel7->CCR = 0;
          DMA1->IFCR = DMA_IFCR_CGIF7  | DMA_IFCR_CTEIF7 |
                       DMA_IFCR_CHTIF7 | DMA_IFCR_CTCIF7;
          NVIC_ClearPendingIRQ(DMA1_Channel7_IRQn);
          NVIC_EnableIRQ(DMA1_Channel7_IRQn);
          break;
      }
      DMA1_Channel |= 1 << channel;
      break;
    
    #if defined (STM32F10X_ENABLE_DMA2)
    case 2:
      RCC->AHBENR |= RCC_AHBENR_DMA2EN;
      switch (channel) {
        case 1:
          DMA2_Channel1->CCR = 0;
          DMA2->IFCR = DMA_IFCR_CGIF1  | DMA_IFCR_CTEIF1 |
                       DMA_IFCR_CHTIF1 | DMA_IFCR_CTCIF1;
          NVIC_ClearPendingIRQ(DMA2_Channel1_IRQn);
          NVIC_EnableIRQ(DMA2_Channel1_IRQn);
          break;
        case 2:
          DMA2_Channel2->CCR = 0;
          DMA2->IFCR = DMA_IFCR_CGIF2  | DMA_IFCR_CTEIF2 |
                       DMA_IFCR_CHTIF2 | DMA_IFCR_CTCIF2;
          NVIC_ClearPendingIRQ(DMA2_Channel2_IRQn);
          NVIC_EnableIRQ(DMA2_Channel2_IRQn);
          break;
        case 3:
          DMA2_Channel3->CCR = 0;
          DMA2->IFCR = DMA_IFCR_CGIF3  | DMA_IFCR_CTEIF3 |
                       DMA_IFCR_CHTIF3 | DMA_IFCR_CTCIF3;
          NVIC_ClearPendingIRQ(DMA2_Channel3_IRQn);
          NVIC_EnableIRQ(DMA2_Channel3_IRQn);
          break;
        case 4:
          DMA2_Channel4->CCR = 0;
          DMA2->IFCR = DMA_IFCR_CGIF4  | DMA_IFCR_CTEIF4 |
                       DMA_IFCR_CHTIF4 | DMA_IFCR_CTCIF4;

          #if defined (STM32F10X_CL)
            NVIC_ClearPendingIRQ(DMA2_Channel4_IRQn);
            NVIC_EnableIRQ(DMA2_Channel4_IRQn);
          #else
            NVIC_ClearPendingIRQ(DMA2_Channel4_5_IRQn);
            NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
          #endif
          break;
        case 5:
          DMA2_Channel5->CCR = 0;
          DMA2->IFCR = DMA_IFCR_CGIF5  | DMA_IFCR_CTEIF5 |
                       DMA_IFCR_CHTIF5 | DMA_IFCR_CTCIF5;

          #if defined (STM32F10X_CL)
            NVIC_ClearPendingIRQ(DMA2_Channel5_IRQn);
            NVIC_EnableIRQ(DMA2_Channel5_IRQn);
          #elif defined (STM32F10X_HD_VL)
            if (AFIO->MAPR2 & AFIO_MAPR2_MISC_REMAP) {
              /* DMA2 Channel 5 mapped at position 60 */
              NVIC_ClearPendingIRQ(DMA2_Channel5_IRQn);
              NVIC_EnableIRQ(DMA2_Channel5_IRQn);
            }
            else {
              NVIC_ClearPendingIRQ(DMA2_Channel4_5_IRQn);
              NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
            }
          #else
            NVIC_ClearPendingIRQ(DMA2_Channel4_5_IRQn);
            NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
          #endif
          break;
      }
      DMA2_Channel |= 1 << channel;
      break;
    #endif
  }
}

/**
  \fn          void DMA_ChannelUninitialize (uint32_t dma, uint32_t channel)
  \brief       Uninitialize DMA Channel
  \param[in]   dma     DMA number
  \param[in]   channel DMA channel number
*/
void DMA_ChannelUninitialize (uint32_t dma, uint32_t channel) {

  switch (dma) {
    case 1:
      if (DMA1_Channel == 0) {
        // All DMA1 Channels are uninitialized
        return;
      }
      switch (channel) {
        case 1:
          NVIC_DisableIRQ(DMA1_Channel1_IRQn);
          DMA1_Channel1->CCR = 0;
          break;
        case 2:
          NVIC_DisableIRQ(DMA1_Channel2_IRQn);
          DMA1_Channel2->CCR = 0;
          break;
        case 3:
          NVIC_DisableIRQ(DMA1_Channel3_IRQn);
          DMA1_Channel3->CCR = 0;
          break;
        case 4:
          NVIC_DisableIRQ(DMA1_Channel4_IRQn);
          DMA1_Channel4->CCR = 0;
          break;
        case 5:
          NVIC_DisableIRQ(DMA1_Channel5_IRQn);
          DMA1_Channel5->CCR = 0;
          break;
        case 6:
          NVIC_DisableIRQ(DMA1_Channel6_IRQn);
          DMA1_Channel6->CCR = 0;
          break;
        case 7:
          NVIC_DisableIRQ(DMA1_Channel7_IRQn);
          DMA1_Channel7->CCR = 0;
          break;
      }
      DMA1_Channel &= ~(1 << channel);
      if (DMA1_Channel == 0){
        RCC->AHBENR &= ~RCC_AHBENR_DMA1EN;
      }
      break;
    
    #if defined (STM32F10X_ENABLE_DMA2)
    case 2:
      if (DMA2_Channel == 0) {
        // All DMA2 Channels are uninitialized
        return;
      }
      switch (channel) {
        case 1:
          NVIC_DisableIRQ(DMA2_Channel1_IRQn);
          DMA2_Channel1->CCR = 0;
          break;
        case 2:
          NVIC_DisableIRQ(DMA2_Channel2_IRQn);
          DMA2_Channel2->CCR = 0;
          break;
        case 3:
          NVIC_DisableIRQ(DMA2_Channel3_IRQn);
          DMA2_Channel3->CCR = 0;
          break;
        case 4:
          #if defined (STM32F10X_CL)
            NVIC_DisableIRQ(DMA2_Channel4_IRQn);
          #else
            /* If Channel 5 disabled, disable also Channel 4 */
            if (!(DMA2_Channel & (1 << 5))) {
              NVIC_DisableIRQ(DMA2_Channel4_5_IRQn);
            }
          #endif
          DMA2_Channel4->CCR = 0;
          break;
        case 5:
          #if defined (STM32F10X_CL)
            NVIC_DisableIRQ(DMA2_Channel5_IRQn);
          #elif defined (STM32F10X_HD_VL)
            if (AFIO->MAPR2 & AFIO_MAPR2_MISC_REMAP) {
              /* DMA2 Channel 5 mapped at position 60 */
              NVIC_DisableIRQ(DMA2_Channel5_IRQn);
            }
            else {
              /* If Channel 4 disabled, disable also Channel 5 */
              if (!(DMA2_Channel & (1 << 4))) {
                NVIC_DisableIRQ(DMA2_Channel4_5_IRQn);
              }
            }
          #else
            /* If Channel 4 disabled, disable also Channel 5 */
            if (!(DMA2_Channel & (1 << 4))) {
              NVIC_DisableIRQ(DMA2_Channel4_5_IRQn);
            }
          #endif
          DMA2_Channel5->CCR = 0;
          break;
      }
      DMA2_Channel &= ~(1 << channel);
      if (DMA2_Channel == 0){
        RCC->AHBENR &= ~RCC_AHBENR_DMA2EN;
      }
      break;
    #endif
  }
}


/// DMA1 Channel Events
__WEAK void DMA1_Channel1_Event (uint32_t event) {;}
__WEAK void DMA1_Channel2_Event (uint32_t event) {;}
__WEAK void DMA1_Channel3_Event (uint32_t event) {;}
__WEAK void DMA1_Channel4_Event (uint32_t event) {;}
__WEAK void DMA1_Channel5_Event (uint32_t event) {;}
__WEAK void DMA1_Channel6_Event (uint32_t event) {;}
__WEAK void DMA1_Channel7_Event (uint32_t event) {;}


#if defined (STM32F10X_ENABLE_DMA2)
/// DMA2 Channel Events
__WEAK void DMA2_Channel1_Event (uint32_t event) {;}
__WEAK void DMA2_Channel2_Event (uint32_t event) {;}
__WEAK void DMA2_Channel3_Event (uint32_t event) {;}
__WEAK void DMA2_Channel4_Event (uint32_t event) {;}
__WEAK void DMA2_Channel5_Event (uint32_t event) {;}
#endif

/**
  \fn          void DMA1_Channel1_IRQHandler (void)
  \brief       DMA1 Channel0 interrupt handler
*/
void DMA1_Channel1_IRQHandler (void) {
  uint32_t events;

  events = (DMA1->ISR >>  0) & DMA_CHANNEL_FLAGS;
  DMA1->IFCR = events <<  0;
  DMA1_Channel1_Event(events);
}

/**
  \fn          void DMA1_Channel2_IRQHandler (void)
  \brief       DMA1 Channel2 interrupt handler
*/
void DMA1_Channel2_IRQHandler (void) {
  uint32_t events;

  events = (DMA1->ISR >>  4) & DMA_CHANNEL_FLAGS;
  DMA1->IFCR = events <<  4;
  DMA1_Channel2_Event(events);
}

/**
  \fn          void DMA1_Channel3_IRQHandler (void)
  \brief       DMA1 Channel3 interrupt handler
*/
void DMA1_Channel3_IRQHandler (void) {
  uint32_t events;

  events = (DMA1->ISR >> 8) & DMA_CHANNEL_FLAGS;
  DMA1->IFCR = events << 8;
  DMA1_Channel3_Event(events);
}

/**
  \fn          void DMA1_Channel4_IRQHandler (void)
  \brief       DMA1 Channel4 interrupt handler
*/
void DMA1_Channel4_IRQHandler (void) {
  uint32_t events;

  events = (DMA1->ISR >> 12) & DMA_CHANNEL_FLAGS;
  DMA1->IFCR = events << 12;
  DMA1_Channel4_Event(events);
}

/**
  \fn          void DMA1_Channel5_IRQHandler (void)
  \brief       DMA1 Channel5 interrupt handler
*/
void DMA1_Channel5_IRQHandler (void) {
  uint32_t events;

  events = (DMA1->ISR >>  16) & DMA_CHANNEL_FLAGS;
  DMA1->IFCR = events <<  16;
  DMA1_Channel5_Event(events);
}

/**
  \fn          void DMA1_Channel6_IRQHandler (void)
  \brief       DMA1 Channel6 interrupt handler
*/
void DMA1_Channel6_IRQHandler (void) {
  uint32_t events;

  events = (DMA1->ISR >>  20) & DMA_CHANNEL_FLAGS;
  DMA1->IFCR = events <<  20;
  DMA1_Channel6_Event(events);
}

/**
  \fn          void DMA1_Channel7_IRQHandler (void)
  \brief       DMA1 Channel7 interrupt handler
*/
void DMA1_Channel7_IRQHandler (void) {
  uint32_t events;

  events = (DMA1->ISR >> 24) & DMA_CHANNEL_FLAGS;
  DMA1->IFCR = events << 24;
  DMA1_Channel7_Event(events);
}


#if defined (STM32F10X_ENABLE_DMA2)
/**
  \fn          void DMA2_Channel1_IRQHandler (void)
  \brief       DMA2 Channel1 interrupt handler
*/
void DMA2_Channel1_IRQHandler (void) {
  uint32_t events;

  events = (DMA2->ISR >>  0) & DMA_CHANNEL_FLAGS;
  DMA2->IFCR = events <<  0;
  DMA2_Channel1_Event(events);
}

/**
  \fn          void DMA2_Channel2_IRQHandler (void)
  \brief       DMA2 Channel2 interrupt handler
*/
void DMA2_Channel2_IRQHandler (void) {
  uint32_t events;

  events = (DMA2->ISR >>  4) & DMA_CHANNEL_FLAGS;
  DMA2->IFCR = events <<  4;
  DMA2_Channel2_Event(events);
}

/**
  \fn          void DMA2_Channel3_IRQHandler (void)
  \brief       DMA2 Channel3 interrupt handler
*/
void DMA2_Channel3_IRQHandler (void) {
  uint32_t events;

  events = (DMA2->ISR >> 8) & DMA_CHANNEL_FLAGS;
  DMA2->IFCR = events << 8;
  DMA2_Channel3_Event(events);
}

#if !defined(STM32F10X_CL)
/**
  \fn          void DMA2_Channel4_IRQHandler (void)
  \brief       DMA2 Channel4 interrupt handler
*/
void DMA2_Channel4_IRQHandler (void) {
  uint32_t events;

  events = (DMA2->ISR >> 12) & DMA_CHANNEL_FLAGS;
  DMA2->IFCR = events << 12;
  DMA2_Channel4_Event(events);
}
#endif

#if defined(STM32F10X_HD_VL) || defined (STM32F10X_CL)
/**
  \fn          void DMA2_Channel5_IRQHandler (void)
  \brief       DMA2 Channel5 interrupt handler
*/
void DMA2_Channel5_IRQHandler (void) {
  uint32_t events;

  events = (DMA2->ISR >>  16) & DMA_CHANNEL_FLAGS;
  DMA2->IFCR = events <<  16;
  DMA2_Channel5_Event(events);
}
#endif

/**
  \fn          void DMA2_Channel4_5_IRQHandler (void)
  \brief       DMA2 Channel 4 and 5 interrupt handler
*/
void DMA2_Channel4_5_IRQHandler (void) {
  uint32_t events;
  
  events = (DMA2->ISR >> 12) & DMA_CHANNEL_FLAGS;
  if (events) {
    /* Channel 4 events */
    DMA2->IFCR = events << 12;
    DMA2_Channel4_Event(events);
  }
  
  events = (DMA2->ISR >> 16) & DMA_CHANNEL_FLAGS;
  if (events) {
    /* Channel 5 events */
    DMA2->IFCR = events << 16;
    DMA2_Channel5_Event(events);
  }
}
#endif
