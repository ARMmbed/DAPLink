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

#include "stm32h7xx.h"
#include "cortex_m.h"
#include "util.h"

TIM_HandleTypeDef timer;
uint32_t time_count;

static uint32_t tim2_clk_div(uint32_t apb1clkdiv);

/**
  * @brief  The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 240MHz
  *            HCLK(Hz)                       = 120MHz
  *            AHB Prescaler                  = 2
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 64MHz
  *            HSI PREDIV1                    = 1
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
void sdk_init()
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    HAL_Init();

    /** Supply configuration update enable
    */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    /** Configure the main internal regulator output voltage
    */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 30;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        util_assert(false);
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                    |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                                    |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        util_assert(false);
    }
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_USART2
                                                |RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_USART6
                                                |RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_SPI5
                                                |RCC_PERIPHCLK_SPI4|RCC_PERIPHCLK_SPI2
                                                |RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_I2C3
                                                |RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_I2C1
                                                |RCC_PERIPHCLK_USB|RCC_PERIPHCLK_QSPI;
    PeriphClkInitStruct.PLL2.PLL2M = 4;
    PeriphClkInitStruct.PLL2.PLL2N = 10;
    PeriphClkInitStruct.PLL2.PLL2P = 4;
    PeriphClkInitStruct.PLL2.PLL2Q = 2;
    PeriphClkInitStruct.PLL2.PLL2R = 2;
    PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
    PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
    PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;
    PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
    PeriphClkInitStruct.Spi45ClockSelection = RCC_SPI45CLKSOURCE_HSI;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_HSI;
    PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_HSI;
    PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_HSI;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        util_assert(false);
    }

    /** Enable USB Voltage detector
    */
    HAL_PWREx_EnableUSBVoltageDetector();

    SystemCoreClockUpdate();
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    HAL_StatusTypeDef ret;
    RCC_ClkInitTypeDef clk_init;
    uint32_t unused;
    uint32_t prescaler;
    uint32_t source_clock;

    HAL_RCC_GetClockConfig(&clk_init, &unused);

    /* Compute the prescaler value to have TIMx counter clock equal to 4000 Hz */
    source_clock = HAL_RCC_GetHCLKFreq() / tim2_clk_div(clk_init.APB1CLKDivider);
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
    switch (apb1clkdiv)
    {
        case RCC_APB1_DIV2:
            return 1;
        case RCC_APB1_DIV4:
            return 2;
        case RCC_APB1_DIV8:
            return 4;
        case RCC_APB1_DIV16:
            return 8;
        default:
            return 1;
    }
}

/* MSP stands for MCU support package and comes from STCube. The MSP layer is responsible
 * for initializing low level hardware like GPIOs and clocks for some peripherals.
 * This function is called by the HAL, but we could also make our drivers handle this functionality
 * with more refactoring.
 */

void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
}

/**
* @brief PCD MSP Initialization
* This function configures the hardware resources used in this example
* @param hpcd: PCD handle pointer
* @retval None
*/
void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if(hpcd->Instance==USB_OTG_FS)
    {
    /** Initializes the peripherals clock
    */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
        PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
        util_assert(false);
        }
    /** Enable USB Voltage detector
    */
        HAL_PWREx_EnableUSBVoltageDetector();

        /**USB_OTG_FS GPIO Configuration
        PA12     ------> USB_OTG_FS_DP
        PA11     ------> USB_OTG_FS_DM
        */
        GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    }
    else if(hpcd->Instance==USB_OTG_HS)
    {
    /** Initializes the peripherals clock
    */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
        PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
        util_assert(false);
        }
    /** Enable USB Voltage detector
    */
        HAL_PWREx_EnableUSBVoltageDetector();

        /**USB_OTG_HS GPIO Configuration
        PB5     ------> USB_OTG_HS_ULPI_D7
        PI11     ------> USB_OTG_HS_ULPI_DIR
        PC0     ------> USB_OTG_HS_ULPI_STP
        PC3     ------> USB_OTG_HS_ULPI_NXT
        PB10     ------> USB_OTG_HS_ULPI_D3
        PB11     ------> USB_OTG_HS_ULPI_D4
        PA5     ------> USB_OTG_HS_ULPI_CK
        PB1     ------> USB_OTG_HS_ULPI_D2
        PB12     ------> USB_OTG_HS_ULPI_D5
        PA3     ------> USB_OTG_HS_ULPI_D0
        PB0     ------> USB_OTG_HS_ULPI_D1
        PB13     ------> USB_OTG_HS_ULPI_D6
        */
        GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_1
                            |GPIO_PIN_12|GPIO_PIN_0|GPIO_PIN_13;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_HS;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_HS;
        HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_HS;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_HS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
        __HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE();
    }
}

/**
* @brief PCD MSP De-Initialization
* This function frees the hardware resources used in this example
* @param hpcd: PCD handle pointer
* @retval None
*/
void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd)
{
    if(hpcd->Instance==USB_OTG_FS)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USB_OTG_FS_CLK_DISABLE();

        /**USB_OTG_FS GPIO Configuration
        PA12     ------> USB_OTG_FS_DP
        PA11     ------> USB_OTG_FS_DM
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12|GPIO_PIN_11);
    }
    else if(hpcd->Instance==USB_OTG_HS)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USB_OTG_HS_CLK_DISABLE();
        __HAL_RCC_USB_OTG_HS_ULPI_CLK_DISABLE();

        /**USB_OTG_HS GPIO Configuration
        PB5     ------> USB_OTG_HS_ULPI_D7
        PI11     ------> USB_OTG_HS_ULPI_DIR
        PC0     ------> USB_OTG_HS_ULPI_STP
        PC3     ------> USB_OTG_HS_ULPI_NXT
        PB10     ------> USB_OTG_HS_ULPI_D3
        PB11     ------> USB_OTG_HS_ULPI_D4
        PA5     ------> USB_OTG_HS_ULPI_CK
        PB1     ------> USB_OTG_HS_ULPI_D2
        PB12     ------> USB_OTG_HS_ULPI_D5
        PA3     ------> USB_OTG_HS_ULPI_D0
        PB0     ------> USB_OTG_HS_ULPI_D1
        PB13     ------> USB_OTG_HS_ULPI_D6
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_1
                            |GPIO_PIN_12|GPIO_PIN_0|GPIO_PIN_13);

        HAL_GPIO_DeInit(GPIOI, GPIO_PIN_11);

        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_3);

        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_3);
    }
}
