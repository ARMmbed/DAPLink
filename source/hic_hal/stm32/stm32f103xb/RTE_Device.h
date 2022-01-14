/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2016 Arm Limited (or its affiliates). All
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
 * $Date:        09. September 2016
 * $Revision:    V1.1.2
 *
 * Project:      RTE Device Configuration for STMicroelectronics STM32F1xx
 *
 * -------------------------------------------------------------------------- */

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H


#define GPIO_PORT(num) \
 ((num == 0) ? GPIOA : \
  (num == 1) ? GPIOB : \
  (num == 2) ? GPIOC : \
  (num == 3) ? GPIOD : \
  (num == 4) ? GPIOE : \
  (num == 5) ? GPIOF : \
  (num == 6) ? GPIOG : \
  NULL)


// <h> Clock Configuration
//   <o> High-speed Internal Clock <1-999999999>
#define RTE_HSI                         8000000
//   <o> High-speed External Clock <1-999999999>
#define RTE_HSE                         25000000
//   <o> System Clock <1-999999999>
#define RTE_SYSCLK                      72000000
//   <o> HCLK Clock   <1-999999999>
#define RTE_HCLK                        72000000
//   <o> APB1 Clock   <1-999999999>
#define RTE_PCLK1                       36000000
//   <o> APB2 Clock   <1-999999999>
#define RTE_PCLK2                       72000000
//   <o> ADC Clock    <1-999999999>
#define RTE_ADCCLK                      36000000
//       USB Clock
#define RTE_USBCLK                      48000000
// </h>


// <e> USART1 (Universal synchronous asynchronous receiver transmitter)
// <i> Configuration settings for Driver_USART1 in component ::CMSIS Driver:USART
#define RTE_USART1                       0

//   <o> USART1_TX Pin <0=>Not Used <1=>PA9
#define RTE_USART1_TX_PORT_ID_DEF       0
#if    (RTE_USART1_TX_PORT_ID_DEF == 0)
#define RTE_USART1_TX_DEF               0
#elif  (RTE_USART1_TX_PORT_ID_DEF == 1)
#define RTE_USART1_TX_DEF               1
#define RTE_USART1_TX_PORT_DEF          GPIOA
#define RTE_USART1_TX_BIT_DEF           9
#else
#error "Invalid USART1_TX Pin Configuration!"
#endif

//   <o> USART1_RX Pin <0=>Not Used <1=>PA10
#define RTE_USART1_RX_PORT_ID_DEF       0
#if    (RTE_USART1_RX_PORT_ID_DEF == 0)
#define RTE_USART1_RX_DEF               0
#elif  (RTE_USART1_RX_PORT_ID_DEF == 1)
#define RTE_USART1_RX_DEF               1
#define RTE_USART1_RX_PORT_DEF          GPIOA
#define RTE_USART1_RX_BIT_DEF           10
#else
#error "Invalid USART1_RX Pin Configuration!"
#endif

//   <o> USART1_CK Pin <0=>Not Used <1=>PA8
#define RTE_USART1_CK_PORT_ID_DEF       0
#if    (RTE_USART1_CK_PORT_ID_DEF == 0)
#define RTE_USART1_CK                   0
#elif  (RTE_USART1_CK_PORT_ID_DEF == 1)
#define RTE_USART1_CK                   1
#define RTE_USART1_CK_PORT_DEF          GPIOA
#define RTE_USART1_CK_BIT_DEF           8
#else
#error "Invalid USART1_CK Pin Configuration!"
#endif

//   <o> USART1_CTS Pin <0=>Not Used <1=>PA11
#define RTE_USART1_CTS_PORT_ID_DEF      0
#if    (RTE_USART1_CTS_PORT_ID_DEF == 0)
#define RTE_USART1_CTS                  0
#elif  (RTE_USART1_CTS_PORT_ID_DEF == 1)
#define RTE_USART1_CTS                  1
#define RTE_USART1_CTS_PORT_DEF         GPIOA
#define RTE_USART1_CTS_BIT_DEF          11
#else
#error "Invalid USART1_CTS Pin Configuration!"
#endif

//   <o> USART1_RTS Pin <0=>Not Used <1=>PA12
#define RTE_USART1_RTS_PORT_ID_DEF      0
#if    (RTE_USART1_RTS_PORT_ID_DEF == 0)
#define RTE_USART1_RTS                  0
#elif  (RTE_USART1_RTS_PORT_ID_DEF == 1)
#define RTE_USART1_RTS                  1
#define RTE_USART1_RTS_PORT_DEF         GPIOA
#define RTE_USART1_RTS_BIT_DEF          12
#else
#error "Invalid USART1_RTS Pin Configuration!"
#endif

//   <e> USART1 Pin Remap
//   <i> Enable USART1 Pin Remapping
#define RTE_USART1_REMAP_FULL           0

//     <o> USART1_TX Pin <0=>Not Used <1=>PB6
#define RTE_USART1_TX_PORT_ID_FULL      0
#if    (RTE_USART1_TX_PORT_ID_FULL == 0)
#define RTE_USART1_TX_FULL              0
#elif  (RTE_USART1_TX_PORT_ID_FULL == 1)
#define RTE_USART1_TX_FULL              1
#define RTE_USART1_TX_PORT_FULL         GPIOB
#define RTE_USART1_TX_BIT_FULL          6
#else
#error "Invalid USART1_TX Pin Configuration!"
#endif

//     <o> USART1_RX Pin <0=>Not Used <1=>PB7
#define RTE_USART1_RX_PORT_ID_FULL      0
#if    (RTE_USART1_RX_PORT_ID_FULL == 0)
#define RTE_USART1_RX_FULL              0
#elif  (RTE_USART1_RX_PORT_ID_FULL == 1)
#define RTE_USART1_RX_FULL              1
#define RTE_USART1_RX_PORT_FULL         GPIOB
#define RTE_USART1_RX_BIT_FULL          7
#else
#error "Invalid USART1_RX Pin Configuration!"
#endif
//   </e>

#if    (RTE_USART1_REMAP_FULL)
#define RTE_USART1_AF_REMAP              AFIO_USART1_REMAP
#define RTE_USART1_TX                    RTE_USART1_TX_FULL
#define RTE_USART1_TX_PORT               RTE_USART1_TX_PORT_FULL
#define RTE_USART1_TX_BIT                RTE_USART1_TX_BIT_FULL
#define RTE_USART1_RX                    RTE_USART1_RX_FULL
#define RTE_USART1_RX_PORT               RTE_USART1_RX_PORT_FULL
#define RTE_USART1_RX_BIT                RTE_USART1_RX_BIT_FULL
#define RTE_USART1_CK_PORT               RTE_USART1_CK_PORT_DEF
#define RTE_USART1_CK_BIT                RTE_USART1_CK_BIT_DEF
#define RTE_USART1_CTS_PORT              RTE_USART1_CTS_PORT_DEF
#define RTE_USART1_CTS_BIT               RTE_USART1_CTS_BIT_DEF
#define RTE_USART1_RTS_PORT              RTE_USART1_RTS_PORT_DEF
#define RTE_USART1_RTS_BIT               RTE_USART1_RTS_BIT_DEF
#else
#define RTE_USART1_AF_REMAP              AFIO_USART1_NO_REMAP
#define RTE_USART1_TX                    RTE_USART1_TX_DEF
#define RTE_USART1_TX_PORT               RTE_USART1_TX_PORT_DEF
#define RTE_USART1_TX_BIT                RTE_USART1_TX_BIT_DEF
#define RTE_USART1_RX                    RTE_USART1_RX_DEF
#define RTE_USART1_RX_PORT               RTE_USART1_RX_PORT_DEF
#define RTE_USART1_RX_BIT                RTE_USART1_RX_BIT_DEF
#define RTE_USART1_CK_PORT               RTE_USART1_CK_PORT_DEF
#define RTE_USART1_CK_BIT                RTE_USART1_CK_BIT_DEF
#define RTE_USART1_CTS_PORT              RTE_USART1_CTS_PORT_DEF
#define RTE_USART1_CTS_BIT               RTE_USART1_CTS_BIT_DEF
#define RTE_USART1_RTS_PORT              RTE_USART1_RTS_PORT_DEF
#define RTE_USART1_RTS_BIT               RTE_USART1_RTS_BIT_DEF
#endif

//   <e> DMA Rx
//     <o1> Number <1=>1
//     <i>  Selects DMA Number (only DMA1 can be used)
//     <o2> Channel <5=>5
//     <i>  Selects DMA Channel (only Channel 5 can be used)
//     <o3> Priority <0=>Low <1=>Medium <2=>High <3=>Very high
//     <i>  Set DMA Channel priority
//   </e>
#define RTE_USART1_RX_DMA               0
#define RTE_USART1_RX_DMA_NUMBER        1
#define RTE_USART1_RX_DMA_CHANNEL       5
#define RTE_USART1_RX_DMA_PRIORITY      0
//   <e> DMA Tx
//     <o1> Number <1=>1
//     <i>  Selects DMA Number (only DMA1 can be used)
//     <o2> Channel <4=>4
//     <i>  Selects DMA Channel (only Channel 4 can be used)
//     <o3> Priority <0=>Low <1=>Medium <2=>High <3=>Very high
//     <i>  Set DMA Channel priority
//   </e>
#define RTE_USART1_TX_DMA               0
#define RTE_USART1_TX_DMA_NUMBER        1
#define RTE_USART1_TX_DMA_CHANNEL       4
#define RTE_USART1_TX_DMA_PRIORITY      0
// </e>


// <e> USART2 (Universal synchronous asynchronous receiver transmitter)
// <i> Configuration settings for Driver_USART2 in component ::CMSIS Driver:USART
#define RTE_USART2                      0

//   <o> USART2_TX Pin <0=>Not Used <1=>PA2
#define RTE_USART2_TX_PORT_ID_DEF       0
#if    (RTE_USART2_TX_PORT_ID_DEF == 0)
#define RTE_USART2_TX_DEF               0
#elif  (RTE_USART2_TX_PORT_ID_DEF == 1)
#define RTE_USART2_TX_DEF               1
#define RTE_USART2_TX_PORT_DEF          GPIOA
#define RTE_USART2_TX_BIT_DEF           2
#else
#error "Invalid USART2_TX Pin Configuration!"
#endif

//   <o> USART2_RX Pin <0=>Not Used <1=>PA3
#define RTE_USART2_RX_PORT_ID_DEF       0
#if    (RTE_USART2_RX_PORT_ID_DEF == 0)
#define RTE_USART2_RX_DEF               0
#elif  (RTE_USART2_RX_PORT_ID_DEF == 1)
#define RTE_USART2_RX_DEF               1
#define RTE_USART2_RX_PORT_DEF          GPIOA
#define RTE_USART2_RX_BIT_DEF           3
#else
#error "Invalid USART2_RX Pin Configuration!"
#endif

//   <o> USART2_CK Pin <0=>Not Used <1=>PA4
#define RTE_USART2_CK_PORT_ID_DEF       0
#if    (RTE_USART2_CK_PORT_ID_DEF == 0)
#define RTE_USART2_CK_DEF               0
#elif  (RTE_USART2_CK_PORT_ID_DEF == 1)
#define RTE_USART2_CK_DEF               1
#define RTE_USART2_CK_PORT_DEF          GPIOA
#define RTE_USART2_CK_BIT_DEF           4
#else
#error "Invalid USART2_CK Pin Configuration!"
#endif

//   <o> USART2_CTS Pin <0=>Not Used <1=>PA0
#define RTE_USART2_CTS_PORT_ID_DEF      0
#if    (RTE_USART2_CTS_PORT_ID_DEF == 0)
#define RTE_USART2_CTS_DEF              0
#elif  (RTE_USART2_CTS_PORT_ID_DEF == 1)
#define RTE_USART2_CTS_DEF              1
#define RTE_USART2_CTS_PORT_DEF         GPIOA
#define RTE_USART2_CTS_BIT_DEF          0
#else
#error "Invalid USART2_CTS Pin Configuration!"
#endif

//   <o> USART2_RTS Pin <0=>Not Used <1=>PA1
#define RTE_USART2_RTS_PORT_ID_DEF      0
#if    (RTE_USART2_RTS_PORT_ID_DEF == 0)
#define RTE_USART2_RTS_DEF              0
#elif  (RTE_USART2_RTS_PORT_ID_DEF == 1)
#define RTE_USART2_RTS_DEF              1
#define RTE_USART2_RTS_PORT_DEF         GPIOA
#define RTE_USART2_RTS_BIT_DEF          1
#else
#error "Invalid USART2_RTS Pin Configuration!"
#endif

//   <e> USART2 Pin Remap
//   <i> Enable USART2 Pin Remapping
#define RTE_USART2_REMAP_FULL           0

//     <o> USART2_TX Pin <0=>Not Used <1=>PD5
#define RTE_USART2_TX_PORT_ID_FULL      0
#if    (RTE_USART2_TX_PORT_ID_FULL == 0)
#define RTE_USART2_TX_FULL              0
#elif  (RTE_USART2_TX_PORT_ID_FULL == 1)
#define RTE_USART2_TX_FULL              1
#define RTE_USART2_TX_PORT_FULL         GPIOD
#define RTE_USART2_TX_BIT_FULL          5
#else
#error "Invalid USART2_TX Pin Configuration!"
#endif

//     <o> USART2_RX Pin <0=>Not Used <1=>PD6
#define RTE_USART2_RX_PORT_ID_FULL      0
#if    (RTE_USART2_RX_PORT_ID_FULL == 0)
#define RTE_USART2_RX_FULL              0
#elif  (RTE_USART2_RX_PORT_ID_FULL == 1)
#define RTE_USART2_RX_FULL              1
#define RTE_USART2_RX_PORT_FULL         GPIOD
#define RTE_USART2_RX_BIT_FULL          6
#else
#error "Invalid USART2_RX Pin Configuration!"
#endif

//     <o> USART2_CK Pin <0=>Not Used <1=>PD7
#define RTE_USART2_CK_PORT_ID_FULL      0
#if    (RTE_USART2_CK_PORT_ID_FULL == 0)
#define RTE_USART2_CK_FULL              0
#elif  (RTE_USART2_CK_PORT_ID_FULL == 1)
#define RTE_USART2_CK_FULL              1
#define RTE_USART2_CK_PORT_FULL         GPIOD
#define RTE_USART2_CK_BIT_FULL          7
#else
#error "Invalid USART2_CK Pin Configuration!"
#endif

//     <o> USART2_CTS Pin <0=>Not Used <1=>PD3
#define RTE_USART2_CTS_PORT_ID_FULL     0
#if    (RTE_USART2_CTS_PORT_ID_FULL == 0)
#define RTE_USART2_CTS_FULL             0
#elif  (RTE_USART2_CTS_PORT_ID_FULL == 1)
#define RTE_USART2_CTS_FULL             1
#define RTE_USART2_CTS_PORT_FULL        GPIOD
#define RTE_USART2_CTS_BIT_FULL         3
#else
#error "Invalid USART2_CTS Pin Configuration!"
#endif

//     <o> USART2_RTS Pin <0=>Not Used <1=>PD4
#define RTE_USART2_RTS_PORT_ID_FULL     0
#if    (RTE_USART2_RTS_PORT_ID_FULL == 0)
#define RTE_USART2_RTS_FULL             0
#elif  (RTE_USART2_RTS_PORT_ID_FULL == 1)
#define RTE_USART2_RTS_FULL             1
#define RTE_USART2_RTS_PORT_FULL        GPIOD
#define RTE_USART2_RTS_BIT_FULL         4
#else
#error "Invalid USART2_RTS Pin Configuration!"
#endif
//   </e>

#if    (RTE_USART2_REMAP_FULL)
#define RTE_USART2_AF_REMAP              AFIO_USART2_REMAP
#define RTE_USART2_TX                    RTE_USART2_TX_FULL
#define RTE_USART2_TX_PORT               RTE_USART2_TX_PORT_FULL
#define RTE_USART2_TX_BIT                RTE_USART2_TX_BIT_FULL
#define RTE_USART2_RX                    RTE_USART2_RX_FULL
#define RTE_USART2_RX_PORT               RTE_USART2_RX_PORT_FULL
#define RTE_USART2_RX_BIT                RTE_USART2_RX_BIT_FULL
#define RTE_USART2_CK                    RTE_USART2_CK_FULL
#define RTE_USART2_CK_PORT               RTE_USART2_CK_PORT_FULL
#define RTE_USART2_CK_BIT                RTE_USART2_CK_BIT_FULL
#define RTE_USART2_CTS                   RTE_USART2_CTS_FULL
#define RTE_USART2_CTS_PORT              RTE_USART2_CTS_PORT_FULL
#define RTE_USART2_CTS_BIT               RTE_USART2_CTS_BIT_FULL
#define RTE_USART2_RTS                   RTE_USART2_RTS_FULL
#define RTE_USART2_RTS_PORT              RTE_USART2_RTS_PORT_FULL
#define RTE_USART2_RTS_BIT               RTE_USART2_RTS_BIT_FULL
#else
#define RTE_USART2_AF_REMAP              AFIO_USART2_NO_REMAP
#define RTE_USART2_TX                    RTE_USART2_TX_DEF
#define RTE_USART2_TX_PORT               RTE_USART2_TX_PORT_DEF
#define RTE_USART2_TX_BIT                RTE_USART2_TX_BIT_DEF
#define RTE_USART2_RX                    RTE_USART2_RX_DEF
#define RTE_USART2_RX_PORT               RTE_USART2_RX_PORT_DEF
#define RTE_USART2_RX_BIT                RTE_USART2_RX_BIT_DEF
#define RTE_USART2_CK                    RTE_USART2_CK_DEF
#define RTE_USART2_CK_PORT               RTE_USART2_CK_PORT_DEF
#define RTE_USART2_CK_BIT                RTE_USART2_CK_BIT_DEF
#define RTE_USART2_CTS                   RTE_USART2_CTS_DEF
#define RTE_USART2_CTS_PORT              RTE_USART2_CTS_PORT_DEF
#define RTE_USART2_CTS_BIT               RTE_USART2_CTS_BIT_DEF
#define RTE_USART2_RTS                   RTE_USART2_RTS_DEF
#define RTE_USART2_RTS_PORT              RTE_USART2_RTS_PORT_DEF
#define RTE_USART2_RTS_BIT               RTE_USART2_RTS_BIT_DEF
#endif

//   <e> DMA Rx
//     <o1> Number <1=>1
//     <i>  Selects DMA Number (only DMA1 can be used)
//     <o2> Channel <6=>6
//     <i>  Selects DMA Channel (only Channel 6 can be used)
//     <o3> Priority <0=>Low <1=>Medium <2=>High <3=>Very high
//     <i>  Set DMA Channel priority
//   </e>
#define RTE_USART2_RX_DMA               0
#define RTE_USART2_RX_DMA_NUMBER        1
#define RTE_USART2_RX_DMA_CHANNEL       6
#define RTE_USART2_RX_DMA_PRIORITY      0

//   <e> DMA Tx
//     <o1> Number <1=>1
//     <i>  Selects DMA Number (only DMA1 can be used)
//     <o2> Channel <7=>7
//     <i>  Selects DMA Channel (only Channel 7 can be used)
//     <o3> Priority <0=>Low <1=>Medium <2=>High <3=>Very high
//     <i>  Set DMA Channel priority
//   </e>
#define RTE_USART2_TX_DMA               0
#define RTE_USART2_TX_DMA_NUMBER        1
#define RTE_USART2_TX_DMA_CHANNEL       7
#define RTE_USART2_TX_DMA_PRIORITY      0

// </e>


// <e> USART3 (Universal synchronous asynchronous receiver transmitter)
// <i> Configuration settings for Driver_USART3 in component ::CMSIS Driver:USART
#define RTE_USART3                      0

//   <o> USART3_TX Pin <0=>Not Used <1=>PB10
#define RTE_USART3_TX_PORT_ID_DEF       0
#if    (RTE_USART3_TX_PORT_ID_DEF == 0)
#define RTE_USART3_TX_DEF               0
#elif  (RTE_USART3_TX_PORT_ID_DEF == 1)
#define RTE_USART3_TX_DEF               1
#define RTE_USART3_TX_PORT_DEF          GPIOB
#define RTE_USART3_TX_BIT_DEF           10
#else
#error "Invalid USART3_TX Pin Configuration!"
#endif

//   <o> USART3_RX Pin <0=>Not Used <1=>PB11
#define RTE_USART3_RX_PORT_ID_DEF       0
#if    (RTE_USART3_RX_PORT_ID_DEF == 0)
#define RTE_USART3_RX_DEF               0
#elif  (RTE_USART3_RX_PORT_ID_DEF == 1)
#define RTE_USART3_RX_DEF               1
#define RTE_USART3_RX_PORT_DEF          GPIOB
#define RTE_USART3_RX_BIT_DEF           11
#else
#error "Invalid USART3_RX Pin Configuration!"
#endif

//   <o> USART3_CK Pin <0=>Not Used <1=>PB12
#define RTE_USART3_CK_PORT_ID_DEF       0
#if    (RTE_USART3_CK_PORT_ID_DEF == 0)
#define RTE_USART3_CK_DEF               0
#elif  (RTE_USART3_CK_PORT_ID_DEF == 1)
#define RTE_USART3_CK_DEF               1
#define RTE_USART3_CK_PORT_DEF          GPIOB
#define RTE_USART3_CK_BIT_DEF           12
#else
#error "Invalid USART3_CK Pin Configuration!"
#endif

//   <o> USART3_CTS Pin <0=>Not Used <1=>PB13
#define RTE_USART3_CTS_PORT_ID_DEF      0
#if    (RTE_USART3_CTS_PORT_ID_DEF == 0)
#define RTE_USART3_CTS_DEF              0
#elif  (RTE_USART3_CTS_PORT_ID_DEF == 1)
#define RTE_USART3_CTS_DEF              1
#define RTE_USART3_CTS_PORT_DEF         GPIOB
#define RTE_USART3_CTS_BIT_DEF          13
#else
#error "Invalid USART3_CTS Pin Configuration!"
#endif

//   <o> USART3_RTS Pin <0=>Not Used <1=>PB14
#define RTE_USART3_RTS_PORT_ID_DEF      0
#if    (RTE_USART3_RTS_PORT_ID_DEF == 0)
#define RTE_USART3_RTS_DEF              0
#elif  (RTE_USART3_RTS_PORT_ID_DEF == 1)
#define RTE_USART3_RTS_DEF              1
#define RTE_USART3_RTS_PORT_DEF         GPIOB
#define RTE_USART3_RTS_BIT_DEF          14
#else
#error "Invalid USART3_RTS Pin Configuration!"
#endif

//   <e> USART3 Partial Pin Remap
//   <i> Enable USART3 Partial Pin Remapping
#define RTE_USART3_REMAP_PARTIAL        0

//     <o> USART3_TX Pin <0=>Not Used <1=>PC10
#define RTE_USART3_TX_PORT_ID_PARTIAL   0
#if    (RTE_USART3_TX_PORT_ID_PARTIAL == 0)
#define RTE_USART3_TX_PARTIAL           0
#elif  (RTE_USART3_TX_PORT_ID_PARTIAL == 1)
#define RTE_USART3_TX_PARTIAL           1
#define RTE_USART3_TX_PORT_PARTIAL      GPIOC
#define RTE_USART3_TX_BIT_PARTIAL       10
#else
#error "Invalid USART3_TX Pin Configuration!"
#endif

//     <o> USART3_RX Pin <0=>Not Used <1=>PC11
#define RTE_USART3_RX_PORT_ID_PARTIAL   0
#if    (RTE_USART3_RX_PORT_ID_PARTIAL == 0)
#define RTE_USART3_RX_PARTIAL           0
#elif  (RTE_USART3_RX_PORT_ID_PARTIAL == 1)
#define RTE_USART3_RX_PARTIAL           1
#define RTE_USART3_RX_PORT_PARTIAL      GPIOC
#define RTE_USART3_RX_BIT_PARTIAL       11
#else
#error "Invalid USART3_RX Pin Configuration!"
#endif

//     <o> USART3_CK Pin <0=>Not Used <1=>PC12
#define RTE_USART3_CK_PORT_ID_PARTIAL   0
#if    (RTE_USART3_CK_PORT_ID_PARTIAL == 0)
#define RTE_USART3_CK_PARTIAL           0
#elif  (RTE_USART3_CK_PORT_ID_PARTIAL == 1)
#define RTE_USART3_CK_PARTIAL           1
#define RTE_USART3_CK_PORT_PARTIAL      GPIOC
#define RTE_USART3_CK_BIT_PARTIAL       12
#else
#error "Invalid USART3_CK Pin Configuration!"
#endif
//   </e>

//   <e> USART3 Full Pin Remap
//   <i> Enable USART3 Full Pin Remapping
#define RTE_USART3_REMAP_FULL           0

//     <o> USART3_TX Pin <0=>Not Used <1=>PD8
#define RTE_USART3_TX_PORT_ID_FULL      0
#if    (RTE_USART3_TX_PORT_ID_FULL == 0)
#define RTE_USART3_TX_FULL              0
#elif  (RTE_USART3_TX_PORT_ID_FULL == 1)
#define RTE_USART3_TX_FULL              1
#define RTE_USART3_TX_PORT_FULL         GPIOD
#define RTE_USART3_TX_BIT_FULL          8
#else
#error "Invalid USART3_TX Pin Configuration!"
#endif

//     <o> USART3_RX Pin <0=>Not Used <1=>PD9
#define RTE_USART3_RX_PORT_ID_FULL      0
#if    (RTE_USART3_RX_PORT_ID_FULL == 0)
#define RTE_USART3_RX_FULL              0
#elif  (RTE_USART3_RX_PORT_ID_FULL == 1)
#define RTE_USART3_RX_FULL              1
#define RTE_USART3_RX_PORT_FULL         GPIOD
#define RTE_USART3_RX_BIT_FULL          9
#else
#error "Invalid USART3_RX Pin Configuration!"
#endif

//     <o> USART3_CK Pin <0=>Not Used <1=>PD10
#define RTE_USART3_CK_PORT_ID_FULL      0
#if    (RTE_USART3_CK_PORT_ID_FULL == 0)
#define RTE_USART3_CK_FULL              0
#elif  (RTE_USART3_CK_PORT_ID_FULL == 1)
#define RTE_USART3_CK_FULL              1
#define RTE_USART3_CK_PORT_FULL         GPIOD
#define RTE_USART3_CK_BIT_FULL          10
#else
#error "Invalid USART3_CK Pin Configuration!"
#endif

//     <o> USART3_CTS Pin <0=>Not Used <1=>PD11
#define RTE_USART3_CTS_PORT_ID_FULL     0
#if    (RTE_USART3_CTS_PORT_ID_FULL == 0)
#define RTE_USART3_CTS_FULL             0
#elif  (RTE_USART3_CTS_PORT_ID_FULL == 1)
#define RTE_USART3_CTS_FULL             1
#define RTE_USART3_CTS_PORT_FULL        GPIOD
#define RTE_USART3_CTS_BIT_FULL         11
#else
#error "Invalid USART3_CTS Pin Configuration!"
#endif

//     <o> USART3_RTS Pin <0=>Not Used <1=>PD12
#define RTE_USART3_RTS_PORT_ID_FULL     0
#if    (RTE_USART3_RTS_PORT_ID_FULL == 0)
#define RTE_USART3_RTS_FULL             0
#elif  (RTE_USART3_RTS_PORT_ID_FULL == 1)
#define RTE_USART3_RTS_FULL             1
#define RTE_USART3_RTS_PORT_FULL        GPIOD
#define RTE_USART3_RTS_BIT_FULL         12
#else
#error "Invalid USART3_RTS Pin Configuration!"
#endif
//   </e>

#if ((RTE_USART3_REMAP_PARTIAL == 1) && (RTE_USART3_REMAP_FULL == 1))
#error "Invalid USART3 Pin Remap Configuration!"
#endif

#if    (RTE_USART3_REMAP_FULL)
#define RTE_USART3_AF_REMAP              AFIO_USART3_REMAP_FULL
#define RTE_USART3_TX                    RTE_USART3_TX_FULL
#define RTE_USART3_TX_PORT               RTE_USART3_TX_PORT_FULL
#define RTE_USART3_TX_BIT                RTE_USART3_TX_BIT_FULL
#define RTE_USART3_RX                    RTE_USART3_RX_FULL
#define RTE_USART3_RX_PORT               RTE_USART3_RX_PORT_FULL
#define RTE_USART3_RX_BIT                RTE_USART3_RX_BIT_FULL
#define RTE_USART3_CK                    RTE_USART3_CK_FULL
#define RTE_USART3_CK_PORT               RTE_USART3_CK_PORT_FULL
#define RTE_USART3_CK_BIT                RTE_USART3_CK_BIT_FULL
#define RTE_USART3_CTS                   RTE_USART3_CTS_FULL
#define RTE_USART3_CTS_PORT              RTE_USART3_CTS_PORT_FULL
#define RTE_USART3_CTS_BIT               RTE_USART3_CTS_BIT_FULL
#define RTE_USART3_RTS                   RTE_USART3_RTS_FULL
#define RTE_USART3_RTS_PORT              RTE_USART3_RTS_PORT_FULL
#define RTE_USART3_RTS_BIT               RTE_USART3_RTS_BIT_FULL
#elif  (RTE_USART3_REMAP_PARTIAL)
#define RTE_USART3_AF_REMAP              AFIO_USART3_REMAP_PARTIAL
#define RTE_USART3_TX                    RTE_USART3_TX_PARTIAL
#define RTE_USART3_TX_PORT               RTE_USART3_TX_PORT_PARTIAL
#define RTE_USART3_TX_BIT                RTE_USART3_TX_BIT_PARTIAL
#define RTE_USART3_RX                    RTE_USART3_RX_PARTIAL
#define RTE_USART3_RX_PORT               RTE_USART3_RX_PORT_PARTIAL
#define RTE_USART3_RX_BIT                RTE_USART3_RX_BIT_PARTIAL
#define RTE_USART3_CK                    RTE_USART3_CK_PARTIAL
#define RTE_USART3_CK_PORT               RTE_USART3_CK_PORT_PARTIAL
#define RTE_USART3_CK_BIT                RTE_USART3_CK_BIT_PARTIAL
#define RTE_USART3_CTS                   RTE_USART3_CTS_DEF
#define RTE_USART3_CTS_PORT              RTE_USART3_CTS_PORT_DEF
#define RTE_USART3_CTS_BIT               RTE_USART3_CTS_BIT_DEF
#define RTE_USART3_RTS                   RTE_USART3_RTS_DEF
#define RTE_USART3_RTS_PORT              RTE_USART3_RTS_PORT_DEF
#define RTE_USART3_RTS_BIT               RTE_USART3_RTS_BIT_DEF
#else
#define RTE_USART3_AF_REMAP              AFIO_USART3_NO_REMAP
#define RTE_USART3_TX                    RTE_USART3_TX_DEF
#define RTE_USART3_TX_PORT               RTE_USART3_TX_PORT_DEF
#define RTE_USART3_TX_BIT                RTE_USART3_TX_BIT_DEF
#define RTE_USART3_RX                    RTE_USART3_RX_DEF
#define RTE_USART3_RX_PORT               RTE_USART3_RX_PORT_DEF
#define RTE_USART3_RX_BIT                RTE_USART3_RX_BIT_DEF
#define RTE_USART3_CK                    RTE_USART3_CK_DEF
#define RTE_USART3_CK_PORT               RTE_USART3_CK_PORT_DEF
#define RTE_USART3_CK_BIT                RTE_USART3_CK_BIT_DEF
#define RTE_USART3_CTS                   RTE_USART3_CTS_DEF
#define RTE_USART3_CTS_PORT              RTE_USART3_CTS_PORT_DEF
#define RTE_USART3_CTS_BIT               RTE_USART3_CTS_BIT_DEF
#define RTE_USART3_RTS                   RTE_USART3_RTS_DEF
#define RTE_USART3_RTS_PORT              RTE_USART3_RTS_PORT_DEF
#define RTE_USART3_RTS_BIT               RTE_USART3_RTS_BIT_DEF
#endif

//   <e> DMA Rx
//     <o1> Number <1=>1
//     <i>  Selects DMA Number (only DMA1 can be used)
//     <o2> Channel <3=>3
//     <i>  Selects DMA Channel (only Channel 3 can be used)
//     <o3> Priority <0=>Low <1=>Medium <2=>High <3=>Very high
//     <i>  Sets DMA Channel priority
//   </e>
#define RTE_USART3_RX_DMA               0
#define RTE_USART3_RX_DMA_NUMBER        1
#define RTE_USART3_RX_DMA_CHANNEL       3
#define RTE_USART3_RX_DMA_PRIORITY      0

//   <e> DMA Tx
//     <o1> Number <1=>1
//     <i>  Selects DMA Number (only DMA1 can be used)
//     <o2> Channel <2=>2
//     <i>  Selects DMA Channel (only Channel 2 can be used)
//     <o3> Priority <0=>Low <1=>Medium <2=>High <3=>Very high
//     <i>  Sets DMA Channel priority
//   </e>
#define RTE_USART3_TX_DMA               0
#define RTE_USART3_TX_DMA_NUMBER        1
#define RTE_USART3_TX_DMA_CHANNEL       2
#define RTE_USART3_TX_DMA_PRIORITY      0

// </e>


// <e> UART4 (Universal asynchronous receiver transmitter)
// <i> Configuration settings for Driver_USART4 in component ::CMSIS Driver:USART
#define RTE_UART4                       0
#define RTE_UART4_AF_REMAP              AFIO_UNAVAILABLE_REMAP

//   <o> UART4_TX Pin <0=>Not Used <1=>PC10
#define RTE_UART4_TX_ID                 0
#if    (RTE_UART4_TX_ID == 0)
#define RTE_UART4_TX                    0
#elif  (RTE_UART4_TX_ID == 1)
#define RTE_UART4_TX                    1
#define RTE_UART4_TX_PORT               GPIOC
#define RTE_UART4_TX_BIT                10
#else
#error "Invalid UART4_TX Pin Configuration!"
#endif

//   <o> UART4_RX Pin <0=>Not Used <1=>PC11
#define RTE_UART4_RX_ID                 0
#if    (RTE_UART4_RX_ID == 0)
#define RTE_UART4_RX                    0
#elif  (RTE_UART4_RX_ID == 1)
#define RTE_UART4_RX                    1
#define RTE_UART4_RX_PORT               GPIOC
#define RTE_UART4_RX_BIT                11
#else
#error "Invalid UART4_RX Pin Configuration!"
#endif


//   <e> DMA Rx
//     <o1> Number <2=>2
//     <i>  Selects DMA Number (only DMA2 can be used)
//     <o2> Channel <3=>3
//     <i>  Selects DMA Channel (only Channel 3 can be used)
//     <o3> Priority <0=>Low <1=>Medium <2=>High <3=>Very high
//     <i>  Sets DMA Channel priority
//   </e>
#define RTE_UART4_RX_DMA                0
#define RTE_UART4_RX_DMA_NUMBER         2
#define RTE_UART4_RX_DMA_CHANNEL        3
#define RTE_UART4_RX_DMA_PRIORITY       0

//   <e> DMA Tx
//     <o1> Number <2=>2
//     <i>  Selects DMA Number (only DMA2 can be used)
//     <o2> Channel <5=>5
//     <i>  Selects DMA Channel (only Channel 5 can be used)
//     <o3> Priority <0=>Low <1=>Medium <2=>High <3=>Very high
//     <i>  Sets DMA Channel priority
//   </e>
#define RTE_UART4_TX_DMA                0
#define RTE_UART4_TX_DMA_NUMBER         2
#define RTE_UART4_TX_DMA_CHANNEL        5
#define RTE_UART4_TX_DMA_PRIORITY       0

// </e>


// <e> UART5 (Universal asynchronous receiver transmitter)
// <i> Configuration settings for Driver_USART5 in component ::CMSIS Driver:USART
#define RTE_UART5                       0
#define RTE_UART5_AF_REMAP              AFIO_UNAVAILABLE_REMAP

//   <o> UART5_TX Pin <0=>Not Used <1=>PC12
#define RTE_UART5_TX_ID                 0
#if    (RTE_UART5_TX_ID == 0)
#define RTE_UART5_TX                    0
#elif  (RTE_UART5_TX_ID == 1)
#define RTE_UART5_TX                    1
#define RTE_UART5_TX_PORT               GPIOC
#define RTE_UART5_TX_BIT                12
#else
#error "Invalid UART5_TX Pin Configuration!"
#endif

//   <o> UART5_RX Pin <0=>Not Used <1=>PD2
#define RTE_UART5_RX_ID                 0
#if    (RTE_UART5_RX_ID == 0)
#define RTE_UART5_RX                    0
#elif  (RTE_UART5_RX_ID == 1)
#define RTE_UART5_RX                    1
#define RTE_UART5_RX_PORT               GPIOD
#define RTE_UART5_RX_BIT                2
#else
#error "Invalid UART5_RX Pin Configuration!"
#endif
// </e>

#endif  /* __RTE_DEVICE_H */
