/**
 * @file power_monitor.h
 * @brief
 */

#ifndef PWR_MON_H
#define PWR_MON_H

#include "IO_Config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PWR_SOURCE_NONE = 0,
    PWR_USB_ONLY,
    PWR_BATT_ONLY,
    PWR_USB_AND_BATT
} power_source_t;

void pwr_mon_init(void);
power_source_t pwr_mon_get_power_source(void);
uint32_t pwr_mon_get_vin_mv(void);
uint32_t pwr_mon_get_vbat_mv(void);


#ifdef __cplusplus
}
#endif

#endif
