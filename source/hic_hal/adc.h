/**
 * @file adc.h
 * @brief
 */

#ifndef ADC_H
#define ADC_H

#include "IO_Config.h"

#ifdef __cplusplus
extern "C" {
#endif

void adc_init(void);
void adc_init_pins(void);
uint32_t adc_read_channel(uint32_t channelGroup, uint32_t channelNumber, uint32_t channelMux);

#ifdef __cplusplus
}
#endif

#endif
