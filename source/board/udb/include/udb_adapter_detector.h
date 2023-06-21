#ifndef _UDB_ADAPTER_DETECTOR_H_INCLUDED_
#define _UDB_ADAPTER_DETECTOR_H_INCLUDED_

#include <stdint.h>

typedef enum
{
    ADAPTER_UDB_13_FLEX     = 0,
    ADAPTER_JOKER_FLEX      = 1,
    ADAPTER_UDB_6_FLEX      = 2,
    ADAPTER_UDB_17_FLEX     = 3,
    ADAPTER_UDB_12_FLEX     = 4,
    ADAPTER_JOKER_FLEX_V2   = 5,
    ADAPTER_TYPE_6          = 6,
    ADAPTER_TYPE_7          = 7,
    ADAPTER_TYPE_8          = 8,
    ADAPTER_TYPE_9          = 9,
    ADAPTER_TYPE_10         = 10,
    ADAPTER_TYPE_11         = 11,
    ADAPTER_TYPE_12         = 12,
    ADAPTER_TYPE_13         = 13,
    ADAPTER_TYPE_14         = 14,
    ADAPTER_TYPE_15         = 15,
    ADAPTER_UNKNOWN         = 16,
    ADAPTER_NONE            = 17
} udb_adapter_type_t;

void udb_adapter_detector_update_adapter_type_adc(void);
udb_adapter_type_t udb_adapter_detector_get_adapter_type_adc(void);
const char* udb_adapter_detector_get_adapter_type_name(udb_adapter_type_t type);

#endif
