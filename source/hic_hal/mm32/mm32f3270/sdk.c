/**
 * @file    sdk.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2017-2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mm32_device.h"
#include "DAP_config.h"
#include "gpio.h"
#include "daplink.h"
#include "util.h"
#include "cortex_m.h"

uint32_t time_count;


/**
    * @brief  Switch the PLL source from HSI to HSE bypass, and select the PLL as SYSCLK
  *         source.
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE bypass)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void sdk_init()
{
    // RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    // RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    // SystemCoreClockUpdate();
    // HAL_Init();

    // /* Select HSI as system clock source to allow modification of the PLL configuration */
    // RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    // RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    // if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
    //     /* Initialization Error */
    //     util_assert(0);
    // }

    // /* Enable HSE bypass Oscillator, select it as PLL source and finally activate the PLL */
    // RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    // RCC_OscInitStruct.HSEState = RCC_CR_HSEON;
    // RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    // RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    // RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    // RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    // if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    //     /* Initialization Error */
    //     util_assert(0);
    // }

    // /* Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
    // RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    // RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    // RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    // RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    // RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    // if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    //     /* Initialization Error */
    //     util_assert(0);
    // }
}

