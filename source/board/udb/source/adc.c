#include "adc.h"
#include "stm32h743xx.h"
#include "stm32h7xx_hal.h"
#include "cmsis_os2.h"
#include "rtx_os.h"
#include "util.h"

#define ADC_CONVERSION_TIMEOUT_MS (10)
#define ADC_BASE ADC3

static ADC_HandleTypeDef s_adc_handle;
static ADC_ChannelConfTypeDef s_chan_conf;

static osMutexId_t s_adc_mutex_id;
static uint32_t s_adc_mutex_cb[WORDS(sizeof(osRtxMutex_t))];
static const osMutexAttr_t k_adc_mutex_attr =
{
    .name = "adc_mutex",
    .attr_bits = osMutexRobust,
    .cb_mem = s_adc_mutex_cb,
    .cb_size = sizeof(s_adc_mutex_cb)
};

void adc_init(void)
{
    s_adc_handle.Instance = ADC_BASE;
    if (HAL_ADC_DeInit(&s_adc_handle) != HAL_OK)
    {
        util_assert(false);
    }
    s_adc_handle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
    s_adc_handle.Init.Resolution = ADC_RESOLUTION_16B;
    s_adc_handle.Init.ScanConvMode = DISABLE;
    s_adc_handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    s_adc_handle.Init.LowPowerAutoWait = DISABLE;
    s_adc_handle.Init.ContinuousConvMode = DISABLE;
    s_adc_handle.Init.NbrOfConversion = 1;
    s_adc_handle.Init.DiscontinuousConvMode = DISABLE;
    s_adc_handle.Init.NbrOfDiscConversion = 1;
    s_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    s_adc_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    s_adc_handle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    s_adc_handle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    s_adc_handle.Init.OversamplingMode = DISABLE;

    if (HAL_ADC_Init(&s_adc_handle) != HAL_OK)
    {
        util_assert(false);
    }

    s_adc_mutex_id = osMutexNew(&k_adc_mutex_attr);
    if (s_adc_mutex_id == NULL)
    {
        util_assert(false);
    }
}

void adc_init_pins(void)
{
}

uint32_t adc_read_channel(uint32_t channelGroup, uint32_t channelNumber, uint32_t channelMux)
{
    uint16_t adc_value;
    osStatus_t  status;

    status = osMutexAcquire(s_adc_mutex_id, osWaitForever);
    if (status != osOK)
    {
        util_assert(false);
    }

    s_chan_conf.Channel = channelNumber;
    s_chan_conf.Rank = ADC_REGULAR_RANK_1;
    s_chan_conf.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;
    s_chan_conf.SingleDiff = ADC_SINGLE_ENDED;
    s_chan_conf.OffsetNumber = ADC_OFFSET_NONE;
    s_chan_conf.Offset = 0;

    if (HAL_ADC_ConfigChannel(&s_adc_handle, &s_chan_conf) != HAL_OK)
    {
        util_assert(false);
    }

    if (HAL_ADCEx_Calibration_Start(&s_adc_handle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
    {
        util_assert(false);
    }

    if (HAL_ADC_Start(&s_adc_handle) != HAL_OK)
    {
        util_assert(false);
    }

    if (HAL_ADC_PollForConversion(&s_adc_handle, ADC_CONVERSION_TIMEOUT_MS) != HAL_OK)
    {
        util_assert(false);
    }
    else
    {
        adc_value = HAL_ADC_GetValue(&s_adc_handle);
    }

    if (HAL_ADC_Stop(&s_adc_handle) != HAL_OK)
    {
        util_assert(false);
    }

    status = osMutexRelease(s_adc_mutex_id);
    if (status != osOK)
    {
        util_assert(false);
    }

    return adc_value;
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    UNUSED(hadc);
    __HAL_RCC_ADC3_CLK_ENABLE();
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
    UNUSED(hadc);
    __HAL_RCC_ADC3_CLK_DISABLE();
}
