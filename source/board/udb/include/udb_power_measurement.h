/*
 *  Description:
 *     Provide an interface to measure the power on UDB
 */
#ifndef _UDB_POWER_MEASUREMENT_H_INCLUDED_
#define _UDB_POWER_MEASUREMENT_H_INCLUDED_

#include <stdint.h>
#include <stdbool.h>

/*
 * Before P3, only two channels are available.
 *   channel 1: mainboard usb
 *   channel 2: adapter usb
 *
 * P3 and after, four channels are available but DUT0 and DUT1 can only measure voltage.
 *   channel 1: DUT0
 *   channel 2: DUT1
 *   channel 3: mainboard usb
 *   channel 4: adapter usb
 */
typedef enum
{
    UDB_POWER_MEASUREMENT_TARGET_MAINBOARD_USB            = 0,
    UDB_POWER_MEASUREMENT_TARGET_ADAPTER_USB              = 1,
    UDB_POWER_MEASUREMENT_TARGET_DUT0                     = 2,
    UDB_POWER_MEASUREMENT_TARGET_DUT1                     = 3,
    UDB_POWER_MEASUREMENT_TARGET_COUNT                    = 4,
} udb_power_measurement_target_t;

typedef struct
{
    uint8_t voltage_reg[UDB_POWER_MEASUREMENT_TARGET_COUNT];
    uint8_t current_reg[UDB_POWER_MEASUREMENT_TARGET_COUNT];
} pwr_measurement_dev_t;

int udb_power_measurement_init(void);
int udb_power_measurement_measure(void);
int udb_power_measurement_read_voltage_mV(udb_power_measurement_target_t target, uint16_t *voltage_mV);
int udb_power_measurement_read_current_uA(udb_power_measurement_target_t target, uint32_t *current_uA);

const char* udb_power_measurement_get_target_name(udb_power_measurement_target_t target);
bool udb_power_measurement_can_measure_voltage(udb_power_measurement_target_t target);
bool udb_power_measurement_can_measure_current(udb_power_measurement_target_t target);
bool udb_power_measurement_can_measure_target(udb_power_measurement_target_t target);

#endif // _UDB_POWER_MEASUREMENT_H_INCLUDED_
