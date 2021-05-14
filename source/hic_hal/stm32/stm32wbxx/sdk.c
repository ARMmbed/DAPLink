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

#include "stm32wbxx.h"
#include "DAP_config.h"
#include "gpio.h"
#include "daplink.h"
#include "util.h"
#include "cortex_m.h"
//#include "stm32wbxx_hal_tim.h" //Ashley

TIM_HandleTypeDef timer;
uint32_t time_count;

static uint32_t tim2_clk_div(uint32_t apb1clkdiv);

/**
    * @brief  Switch the PLL source from HSI to HSE, and select the PLL as SYSCLK
  *         source.
  *         The system Clock is configured as follow :
  *            System Clock source            = HSE via PLL
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
// void sdk_init()
// {
	// RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	// RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	// RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	
	// SystemCoreClockUpdate();
	// HAL_Init();

	// /** Initializes the CPU, AHB and APB busses clocks 
	// */
	// RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	// RCC_OscInitStruct.HSEState = RCC_HSE_ON;
// //	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	// RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	// RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	// RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	// RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1; //Ashley
	
	// HAL_RCC_OscConfig(&RCC_OscInitStruct);
	
	// /** Initializes the CPU, AHB and APB busses clocks 
	// */
	// RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
								  // |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	// RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	// RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	// RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	// RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	// HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
	
	// PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	// PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
	
	// HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
	
// }

void sdk_init(void)  //Ashley copied from STM32CubeIDE, fn SystemClock_Config()
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 32;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
 
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_I2C3|RCC_PERIPHCLK_USB
                              |RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.PLLSAI1.PLLN = 24;
  PeriphClkInitStruct.PLLSAI1.PLLP = RCC_PLLP_DIV2;
  PeriphClkInitStruct.PLLSAI1.PLLQ = RCC_PLLQ_DIV2;
  PeriphClkInitStruct.PLLSAI1.PLLR = RCC_PLLR_DIV2;
  PeriphClkInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_USBCLK|RCC_PLLSAI1_ADCCLK;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSI;
  PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE1;

  HAL_RCCEx_EnableMSIPLLMode();
} //Ashley

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    HAL_StatusTypeDef ret;
    RCC_ClkInitTypeDef clk_init;
    uint32_t unused;
    uint32_t prescaler;
    uint32_t source_clock;

    HAL_RCC_GetClockConfig(&clk_init, &unused);

    /* Compute the prescaler value to have TIMx counter clock equal to 4000 Hz */
    source_clock = SystemCoreClock / tim2_clk_div(clk_init.APB1CLKDivider);
    prescaler = (uint32_t)(source_clock / 4000) - 1;

    /* Set TIMx instance */
    timer.Instance = TIM2;

    timer.Init.Period            = 0xFFFF;
    timer.Init.Prescaler         = prescaler;
    timer.Init.ClockDivision     = 0;
    timer.Init.CounterMode       = TIM_COUNTERMODE_UP;
    timer.Init.RepetitionCounter = 0;

    __HAL_RCC_TIM2_CLK_ENABLE();

    ret = HAL_TIM_Base_DeInit(&timer);
    if (ret != HAL_OK) {
        return ret;
    }

    time_count = 0;
    ret = HAL_TIM_Base_Init(&timer);
    if (ret != HAL_OK) {
        return ret;
    }

    ret = HAL_TIM_Base_Start(&timer);
    if (ret != HAL_OK) {
        return ret;
    }

    return HAL_OK;
}


void HAL_IncTick(void)
{
    // Do nothing
}

uint32_t HAL_GetTick(void)
{
    cortex_int_state_t state;
    state = cortex_int_get_and_disable();
    const uint32_t ticks = __HAL_TIM_GET_COUNTER(&timer) / 4;
    time_count += (ticks - time_count) & 0x3FFF;
    cortex_int_restore(state);
    return time_count;
}

void HAL_SuspendTick(void)
{
    HAL_TIM_Base_Start(&timer);
}

void HAL_ResumeTick(void)
{
    HAL_TIM_Base_Stop(&timer);
}

static uint32_t tim2_clk_div(uint32_t apb1clkdiv)
{
    switch (apb1clkdiv) {
        case LL_RCC_APB1_DIV_2:
            return 1;
        case (LL_RCC_APB1_DIV_4):
            return 2;
        case(LL_RCC_APB1_DIV_8):
            return 4;
        case (LL_RCC_APB1_DIV_16):
            return 8;
        default:
            return 1;
    }
}
