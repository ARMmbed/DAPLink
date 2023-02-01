/* -----------------------------------------------------------------------------
 * Copyright (c) 2013 Arm Limited (or its affiliates). All
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
 * $Date:        27. August 2013
 * $Revision:    V1.01
 *
 * Project:      DMA Driver definitions for ST STM32F10x
 * -------------------------------------------------------------------- */

#ifndef __DMA_STM32F10X_H
#define __DMA_STM32F10X_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f10x.h"


#if defined (__CC_ARM)
#define __FORCE_INLINE  static __forceinline
#else
#define __FORCE_INLINE  __STATIC_INLINE
#endif


#define _DMAx_CHANNELy(x, y)              DMA##x##_Channel##y
#define  DMAx_CHANNELy(x, y)             _DMAx_CHANNELy(x, y)

#define _DMAx_CHANNELy_EVENT(x, y)        DMA##x##_Channel##y##_Event
#define  DMAx_CHANNELy_EVENT(x, y)       _DMAx_CHANNELy_EVENT(x, y)


// DMA channel Interrupt Flags
#define DMA_CHANNEL_GLOBAL_INTERRUPT     (1UL<<0)
#define DMA_CHANNEL_TRANSFER_COMPLETE    (1UL<<1)
#define DMA_CHANNEL_HALF_TRANSFER        (1UL<<2)
#define DMA_CHANNEL_TRANSFER_ERROR       (1UL<<3)
#define DMA_CHANNEL_FLAGS                (DMA_CHANNEL_TRANSFER_ERROR    | \
                                          DMA_CHANNEL_HALF_TRANSFER     | \
                                          DMA_CHANNEL_TRANSFER_COMPLETE | \
                                          DMA_CHANNEL_GLOBAL_INTERRUPT)

// DMA channel Configuration Register definitions
#define DMA_TRANSFER_ERROR_INTERRUPT    DMA_CCR1_TEIE
#define DMA_HALF_TRANSFER_INTERRUPT     DMA_CCR1_HTIE
#define DMA_TRANSFER_COMPLETE_INTERRUPT DMA_CCR1_TCIE
#define DMA_PERIPHERAL_TO_MEMORY        0
#define DMA_READ_MEMORY                 DMA_CCR1_DIR
#define DMA_MEMORY_TO_MEMORY            DMA_CCR1_MEM2MEM
#define DMA_CIRCULAR_MODE               DMA_CCR1_CIRC
#define DMA_PERIPHERAL_INCREMENT        DMA_CCR1_PINC
#define DMA_MEMORY_INCREMENT            DMA_CCR1_MINC
#define DMA_PERIPHERAL_DATA_8BIT        0
#define DMA_PERIPHERAL_DATA_16BIT       DMA_CCR1_PSIZE_0
#define DMA_PERIPHERAL_DATA_32BIT       DMA_CCR1_PSIZE_1
#define DMA_MEMORY_DATA_8BIT            0
#define DMA_MEMORY_DATA_16BIT           DMA_CCR1_MSIZE_0
#define DMA_MEMORY_DATA_32BIT           DMA_CCR1_MSIZE_1
#define DMA_PRIORITY_POS                12
#define DMA_PRIORITY_MASK               DMA_CCR1_PL
#define DMA_CHANNEL_EN                  DMA_CCR1_EN


// DMA Information definitions
typedef struct _DMA_INFO {
  DMA_Channel_TypeDef *ptr_channel;
  uint8_t              dma;
  uint8_t              channel;
  uint8_t              priority;
} DMA_INFO;


/// DMA Functions

/**
  \fn          void DMA_ChannelInitialize (uint32_t dma, uint32_t channel)
  \brief       Initialize DMA Channel
  \param[in]   dma     DMA number
  \param[in]   channel DMA channel number
*/
extern void DMA_ChannelInitialize (uint32_t dma, uint32_t channel);

/**
  \fn          void DMA_ChannelUninitialize (uint32_t dma, uint32_t channel)
  \brief       Uninitialize DMA channel
  \param[in]   dma     DMA number
  \param[in]   channel DMA channel number
*/
extern void DMA_ChannelUninitialize (uint32_t dma, uint32_t channel);

/**
  \fn          void DMA_ChannelConfigure (DMA_Channel_TypeDef *DMA_Channel,
                                         uint32_t            ccr,
                                         uint32_t            cpar,
                                         uint32_t            cmar,
                                         uint32_t            cndtr);
  \brief       Configure DMA channel for next transfer
  \param[in]   DMA_Channel specifies pointer to DMA channel peripheral
  \param[in]   ccr    Configuration register value
  \param[in]   cpar   Peripheral address register value
  \param[in]   cmar   Memory address register value
  \param[in]   cndtr  Number of data transfer register value
*/
__FORCE_INLINE void DMA_ChannelConfigure (DMA_Channel_TypeDef *DMA_Channel,
                                         uint32_t              ccr,
                                         uint32_t              cpar,
                                         uint32_t              cmar,
                                         uint32_t              cndtr) {
  DMA_Channel->CCR   = ccr;
  DMA_Channel->CPAR  = cpar;
  DMA_Channel->CMAR  = cmar;
  DMA_Channel->CNDTR = cndtr;
}


/**
  \fn          void DMA_ChannelEnable (DMA_Channel_TypeDef *DMA_Channel)
  \brief       Enable channel and/or start memory to memory transfer
  \param[in]   DMA_Channel  Pointer to DMA channel peripheral
*/
__FORCE_INLINE void DMA_ChannelEnable (DMA_Channel_TypeDef *DMA_Channel) {
  DMA_Channel->CCR |=  DMA_CHANNEL_EN;
}

/**
  \fn          void DMA_ChannelDisable (DMA_Channel_TypeDef *DMA_Channel)
  \brief       Disable channel and/or stop memory to memory transfer
  \param[in]   DMA_Channel  Pointer to DMA channel peripheral
*/
__FORCE_INLINE void DMA_ChannelDisable (DMA_Channel_TypeDef *DMA_Channel) {
  DMA_Channel->CCR &= ~DMA_CHANNEL_EN;
}

/**
  \fn          bool DMA_ChannelStatus (DMA_Channel_TypeDef *DMA_Channel)
  \brief       Check if channel is enabled or disabled
  \param[in]   DMA_Channel  Pointer to DMA channel peripheral
  \return      Channel Status
               - \b true Enabled
               - \b false Disabled
*/
__FORCE_INLINE bool DMA_ChannelStatus (DMA_Channel_TypeDef *DMA_Channel) {
  return (DMA_Channel->CCR & DMA_CHANNEL_EN) ? true : false;
}

/**
  \fn          uint32_t DMA_ChannelTransferItemCount (DMA_Channel_TypeDef *DMA_Channel)
  \brief       Get number of data items to transfer
  \param[in]   DMA_channel  Pointer to DMA channel peripheral
  \return      Number of data items to transfer
*/
__FORCE_INLINE uint32_t DMA_ChannelTransferItemCount (DMA_Channel_TypeDef *DMA_Channel) {
  return DMA_Channel->CNDTR;
}


#endif /* __DMA_STM32F10X_H */
