/**
 * @file    DAP_config.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2018, ARM Limited, All Rights Reserved
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

#include "DAP_config.h"
#include "IO_Config.h"
#include "stm32f0xx.h"
#include "util.h"

#define __wait_todo() util_assert(0)

void DAP_SETUP(void)
{
    __HAL_RCC_DAP_GPIO_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    HAL_GPIO_WritePin(LED_CONNECTED_PORT, LED_CONNECTED_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = LED_CONNECTED_PIN;
    HAL_GPIO_Init(LED_CONNECTED_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(LED_RUNNING_PORT, LED_RUNNING_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = LED_RUNNING_PIN;
    HAL_GPIO_Init(LED_RUNNING_PORT, &GPIO_InitStruct);
}

#if (DAP_SWD != 0)
void PORT_SWD_SETUP(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
#if (DAP_JTAG != 0)
    __wait_todo();
#endif
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    HAL_GPIO_WritePin(SWCLK_TCK_PORT, SWCLK_TCK_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = SWCLK_TCK_PIN;
    HAL_GPIO_Init(SWCLK_TCK_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(SWDIO_TMS_PORT, SWDIO_TMS_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = SWDIO_TMS_PIN;
    HAL_GPIO_Init(SWDIO_TMS_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(nRESET_PORT, nRESET_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = nRESET_PIN;
    HAL_GPIO_Init(nRESET_PORT, &GPIO_InitStruct);
}
#endif

#if (DAP_JTAG != 0)
void PORT_JTAG_SETUP(void)
{
}
#endif

void PORT_OFF(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
#if (DAP_JTAG != 0)
    __wait_todo();
#endif

#if (DAP_JTAG != 0) || (DAP_SWD != 0)
    HAL_GPIO_WritePin(SWCLK_TCK_PORT, SWCLK_TCK_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = SWCLK_TCK_PIN;
    HAL_GPIO_Init(SWCLK_TCK_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(SWDIO_TMS_PORT, SWDIO_TMS_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = SWDIO_TMS_PIN;
    HAL_GPIO_Init(SWDIO_TMS_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(nRESET_PORT, nRESET_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = nRESET_PIN;
    HAL_GPIO_Init(nRESET_PORT, &GPIO_InitStruct);
#endif
}

uint32_t RESET_TARGET(void)
{
    __wait_todo();
    return 0;
}

void PIN_SWDIO_OUT_ENABLE(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin = SWDIO_TMS_PIN;
    HAL_GPIO_Init(SWDIO_TMS_PORT, &GPIO_InitStruct);
}

void PIN_SWDIO_OUT_DISABLE(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pin = SWDIO_TMS_PIN;
    HAL_GPIO_Init(SWDIO_TMS_PORT, &GPIO_InitStruct);
}
