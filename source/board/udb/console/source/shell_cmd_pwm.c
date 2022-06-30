#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stm32h7xx.h>
#include <util.h>
#include <daplink_debug.h>
#include "shell_cmd_pwm.h"

#define TIM_PRESCALER 256

static TIM_HandleTypeDef timer = {.Instance = TIM1};

static uint32_t tim1_clk_div(uint32_t apb2clkdiv)
{
    switch (apb2clkdiv) {
        case RCC_APB2_DIV1:
            return 1;
        case RCC_APB2_DIV4:
            return 2;
        case RCC_APB2_DIV8:
            return 4;
        default: // RCC_CFGR_PPRE2_DIV1
            util_assert(false);
            return 1;
    }
}

static uint32_t ahb_clk_div(uint32_t ahbclkdiv)
{
    switch (ahbclkdiv) {
        case RCC_HCLK_DIV1:
            return 1;
        case RCC_HCLK_DIV2:
            return 2;
        case RCC_HCLK_DIV4:
            return 4;
        case RCC_HCLK_DIV8:
            return 8;
        case RCC_HCLK_DIV16:
            return 16;
        default:
            util_assert(false);
            return 1;
    }
}

static void pwm_stop(void)
{
    __HAL_RCC_TIM1_CLK_ENABLE();

    if (HAL_TIM_PWM_Stop(&timer, TIM_CHANNEL_1) != HAL_OK)
    {
        util_assert(false);
    }

    if (HAL_TIM_Base_Stop(&timer) != HAL_OK)
    {
        util_assert(false);
    }

    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_9);

    if (HAL_TIM_Base_DeInit(&timer) != HAL_OK) {
        util_assert(false);
    }

    if (HAL_TIM_PWM_DeInit(&timer) != HAL_OK) {
        util_assert(false);
    }

    __HAL_RCC_TIM1_CLK_DISABLE();
}

static void pwm_start(uint16_t freq, uint16_t duty_cycle)
{
    RCC_ClkInitTypeDef clk_init = {0};
    TIM_OC_InitTypeDef pwm_config = {0};
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    uint16_t pulse;
    uint32_t unused;
    uint16_t period;
    uint32_t tim_clock;

    HAL_RCC_GetClockConfig(&clk_init, &unused);
    tim_clock = SystemCoreClock / tim1_clk_div(clk_init.APB2CLKDivider) / ahb_clk_div(clk_init.AHBCLKDivider) / TIM_PRESCALER;

    if ((duty_cycle < 1) || (duty_cycle > 99))
    {
        printf("ERROR: wrong duty cycle value\n");
        return;
    }

    if ((freq < 1) || (freq > (tim_clock / 3)))
    {
        printf("ERROR: wrong freq value\n");
        return;
    }

    period = tim_clock / freq;
    pulse = (period * (100 - duty_cycle) / 100);

    timer.Init.Prescaler         = TIM_PRESCALER - 1;
    timer.Init.ClockDivision     = 0;
    timer.Init.CounterMode       = TIM_COUNTERMODE_UP;
    timer.Init.RepetitionCounter = 0;
    timer.Init.Period            = period - 1;

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    GPIO_InitStruct.Pin         = GPIO_PIN_9;
    GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed       = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate   = GPIO_AF1_TIM1;

    pwm_config.OCMode           = TIM_OCMODE_PWM1;
    pwm_config.Pulse            = pulse;
    pwm_config.OCPolarity       = TIM_OCPOLARITY_HIGH;
    pwm_config.OCNPolarity      = TIM_OCPOLARITY_HIGH;
    pwm_config.OCFastMode       = TIM_OCFAST_DISABLE;
    pwm_config.OCIdleState      = TIM_OCIDLESTATE_RESET;
    pwm_config.OCNIdleState     = TIM_OCIDLESTATE_RESET;

    __HAL_RCC_TIM1_CLK_ENABLE();

    if (HAL_TIM_Base_Init(&timer) != HAL_OK)
    {
        util_assert(false);
    }

    if (HAL_TIM_PWM_Init(&timer) != HAL_OK)
    {
        util_assert(false);
    }

    if (HAL_TIMEx_MasterConfigSynchronization(&timer, &sMasterConfig) != HAL_OK)
    {
        util_assert(false);
    }

    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    if (HAL_TIM_PWM_ConfigChannel(&timer, &pwm_config, TIM_CHANNEL_1) != HAL_OK)
    {
        util_assert(false);
    }

    if (HAL_TIM_Base_Start(&timer) != HAL_OK)
    {
        util_assert(false);
    }

    if (HAL_TIM_PWM_Start(&timer, TIM_CHANNEL_1) != HAL_OK)
    {
        util_assert(false);
    }
}

void cmd_pwm(int argc, char *argv[])
{
    if (strcmp(argv[1], "start") == 0)
    {
        if (argc < 4)
        {
            printf("ERROR: missing args\n");
            return;
        }

        uint16_t freq = strtoul(argv[2], NULL, 10);
        uint16_t duty_cycle = strtoul(argv[3], NULL, 10);

        pwm_start(freq, duty_cycle);
    }
    else if (strcmp(argv[1], "stop") == 0)
    {
        pwm_stop();
    }
    else
    {
        printf("ERROR: unknown cmd\n");
    }

    return;
}
