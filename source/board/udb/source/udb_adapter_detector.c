/*
 *  Description:
 *    Detect UDC adapter type:
 *      0: UDB-13-Flex
 *      1: Joker-Flex
 *      2: UDB-6-Flex
 *      3: UDB-17-Flex
 *      4: UDB-12-Flex
 *      5: Unknown
 *      6: None
 */
#include "udb_adapter_detector.h"
#include "stm32h743xx.h"
#include "stm32h7xx_hal.h"
#include "adc.h"

// Use PF4 (AF: ADC3_INP9)
#define ADAPTER_DETECTOR_ADC_CHANNEL ADC_CHANNEL_9

// The value is selected based on the average of the desired ADC value
// of adapter type i and adapter type i+1
static const uint16_t k_adapter_type_adc_map[] =
{
    [ADAPTER_UDB_13_FLEX]   = 54102,
    [ADAPTER_JOKER_FLEX]    = 50487,
    [ADAPTER_UDB_6_FLEX]    = 46871,
    [ADAPTER_UDB_17_FLEX]   = 43470,
    [ADAPTER_UDB_12_FLEX]   = 40147,
    [ADAPTER_TYPE_5]        = 36988,
    [ADAPTER_TYPE_6]        = 33697,
    [ADAPTER_TYPE_7]        = 30231,
    [ADAPTER_TYPE_8]        = 26850,
    [ADAPTER_TYPE_9]        = 23526,
    [ADAPTER_TYPE_10]       = 20285,
    [ADAPTER_TYPE_11]       = 17164,
    [ADAPTER_TYPE_12]       = 14227,
    [ADAPTER_TYPE_13]       = 11040,
    [ADAPTER_TYPE_14]       = 7567,
    [ADAPTER_TYPE_15]       = 5904,
    // 0xff is an arbitrarily chosen low value between 0 and adapter type 15
    [ADAPTER_UNKNOWN]       = 0xff,
    [ADAPTER_NONE]          = 0,
};

static const char* k_adapter_type_name[] =
{
    [ADAPTER_UDB_13_FLEX]   = "UDB_13_FLEX",
    [ADAPTER_JOKER_FLEX]    = "JOKER_FLEX",
    [ADAPTER_UDB_6_FLEX]    = "UDB_6_FLEX",
    [ADAPTER_UDB_17_FLEX]   = "UDB_17_FLEX",
    [ADAPTER_UDB_12_FLEX]   = "UDB_12_FLEX",
    [ADAPTER_TYPE_5]        = "TYPE_5",
    [ADAPTER_TYPE_6]        = "TYPE_6",
    [ADAPTER_TYPE_7]        = "TYPE_7",
    [ADAPTER_TYPE_8]        = "TYPE_8",
    [ADAPTER_TYPE_9]        = "TYPE_9",
    [ADAPTER_TYPE_10]       = "TYPE_10",
    [ADAPTER_TYPE_11]       = "TYPE_11",
    [ADAPTER_TYPE_12]       = "TYPE_12",
    [ADAPTER_TYPE_13]       = "TYPE_13",
    [ADAPTER_TYPE_14]       = "TYPE_14",
    [ADAPTER_TYPE_15]       = "TYPE_15",
    [ADAPTER_UNKNOWN]       = "UNKNOWN",
    [ADAPTER_NONE]          = "NONE",
};

static udb_adapter_type_t s_adapter_type_adc;

void udb_adapter_detector_update_adapter_type_adc(void)
{
    uint32_t adc = adc_read_channel(0, ADAPTER_DETECTOR_ADC_CHANNEL, 0);

    uint8_t adapter_type_adc_map_size = sizeof(k_adapter_type_adc_map) / sizeof(uint16_t);
    for (int8_t adapter_type = 0; adapter_type < adapter_type_adc_map_size; ++adapter_type)
    {
        if (adc > k_adapter_type_adc_map[adapter_type])
        {
            s_adapter_type_adc = adapter_type;
            break;
        }
    }
}

udb_adapter_type_t udb_adapter_detector_get_adapter_type_adc(void)
{
    return s_adapter_type_adc;
}

const char* udb_adapter_detector_get_adapter_type_name(udb_adapter_type_t type)
{
    return k_adapter_type_name[type];
}
