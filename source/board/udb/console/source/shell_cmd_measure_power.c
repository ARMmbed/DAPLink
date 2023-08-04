#include <stdio.h>
#include "shell_cmd_measure_power.h"
#include "udb_power_measurement.h"
#include "udb_errno.h"
#include "udb_version.h"

void cmd_measure_power(int argc, char *argv[])
{
    uint16_t voltage_mV;
    uint32_t current_uA;

    int ret = udb_power_measurement_measure();

    if (ret == UDB_SUCCESS)
    {
        for (udb_power_measurement_target_t target_type = 0; target_type < UDB_POWER_MEASUREMENT_TARGET_COUNT; ++target_type)
        {
            if (udb_power_measurement_can_measure_target(target_type) == false)
            {
                continue;
            }

            printf("Target: %s\n", udb_power_measurement_get_target_name(target_type));

            if (udb_power_measurement_can_measure_voltage(target_type) == true)
            {
                ret = udb_power_measurement_read_voltage_mV(target_type, &voltage_mV);
                if (ret == UDB_SUCCESS)
                {
                    printf("\tvoltage: %u mV\n", voltage_mV);
                }
                else
                {
                    printf("\tERROR: can not read voltage\n");
                }
            }
            else
            {
                printf("\tDoesn't support reading %s voltage\n", udb_power_measurement_get_target_name(target_type));
            }

            if (udb_power_measurement_can_measure_current(target_type) == true)
            {
                ret = udb_power_measurement_read_current_uA(target_type, &current_uA);

                if (ret == UDB_SUCCESS)
                {
                    printf("\tcurrent: %lu uA\n", current_uA);
                }
                else
                {
                    printf("\tERROR: can not read current\n");
                }
            }
            else
            {
                printf("\tDoesn't support reading %s current\n", udb_power_measurement_get_target_name(target_type));
            }
        }
    }
    else
    {
        printf("ERROR: can not measure UDB power\n");
    }
}
