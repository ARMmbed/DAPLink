/**
 * @file    adc.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * Copyright 2020 NXP
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

#include "adc.h"
#include "fsl_adc16.h"

#define ADC_BASE    ADC0

void adc_init(void)
{
    adc16_config_t adc16ConfigStruct;
    adc16ConfigStruct.referenceVoltageSource     = kADC16_ReferenceVoltageSourceVref;
    adc16ConfigStruct.clockSource                = kADC16_ClockSourceAsynchronousClock;
    adc16ConfigStruct.enableAsynchronousClock    = true;
    adc16ConfigStruct.clockDivider               = kADC16_ClockDivider1;
    adc16ConfigStruct.resolution                 = kADC16_ResolutionSE12Bit;
    adc16ConfigStruct.longSampleMode             = kADC16_LongSampleCycle24;
    adc16ConfigStruct.enableHighSpeed            = false;
    adc16ConfigStruct.enableLowPower             = false;
    adc16ConfigStruct.enableContinuousConversion = false;
    //ADC16_GetDefaultConfig(&adc16ConfigStruct);
    ADC16_Init(ADC_BASE, &adc16ConfigStruct);
    ADC16_SetHardwareAverage(ADC_BASE, kADC16_HardwareAverageCount8);
    ADC16_DoAutoCalibration(ADC_BASE);
}

void adc_init_pins()
{

}

uint32_t adc_read_channel(uint32_t channelGroup, uint32_t channelNumber, uint32_t channelMux)
{
    adc16_channel_config_t adc16ChannelConfigStruct;
    adc16ChannelConfigStruct.channelNumber = channelNumber;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
    adc16ChannelConfigStruct.enableDifferentialConversion = false;

    ADC16_SetChannelMuxMode(ADC_BASE, (adc16_channel_mux_mode_t)channelMux);
    ADC16_SetChannelConfig(ADC_BASE, channelGroup, &adc16ChannelConfigStruct);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(ADC_BASE, channelGroup)));
    return ADC16_GetChannelConversionValue(ADC_BASE, channelGroup);
}
