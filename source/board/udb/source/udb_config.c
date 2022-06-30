#include "udb_version.h"
#include "pac193x.h"
#include "udb_power_measurement.h"

pwr_measurement_dev_t g_pwr_measurement_dev;

static const pwr_measurement_dev_t k_pwr_measurement_dev_before_p3 =
{
    .voltage_reg[UDB_POWER_MEASUREMENT_TARGET_MAINBOARD_USB] = PAC193X_VBUS1_REG,
    .voltage_reg[UDB_POWER_MEASUREMENT_TARGET_ADAPTER_USB] = PAC193X_VBUS2_REG,
    .voltage_reg[UDB_POWER_MEASUREMENT_TARGET_DUT0] = PAC193X_INVALID_REG,
    .voltage_reg[UDB_POWER_MEASUREMENT_TARGET_DUT1] = PAC193X_INVALID_REG,
    .current_reg[UDB_POWER_MEASUREMENT_TARGET_MAINBOARD_USB] = PAC193X_VSENSE1_REG,
    .current_reg[UDB_POWER_MEASUREMENT_TARGET_ADAPTER_USB] = PAC193X_VSENSE2_REG,
    .current_reg[UDB_POWER_MEASUREMENT_TARGET_DUT0] = PAC193X_INVALID_REG,
    .current_reg[UDB_POWER_MEASUREMENT_TARGET_DUT1] = PAC193X_INVALID_REG,
};

static const pwr_measurement_dev_t k_pwr_measurement_dev_p3_and_after =
{
    .voltage_reg[UDB_POWER_MEASUREMENT_TARGET_MAINBOARD_USB] = PAC193X_VBUS3_REG,
    .voltage_reg[UDB_POWER_MEASUREMENT_TARGET_ADAPTER_USB] = PAC193X_VBUS4_REG,
    .voltage_reg[UDB_POWER_MEASUREMENT_TARGET_DUT0] = PAC193X_VBUS1_REG,
    .voltage_reg[UDB_POWER_MEASUREMENT_TARGET_DUT1] = PAC193X_VBUS2_REG,
    .current_reg[UDB_POWER_MEASUREMENT_TARGET_MAINBOARD_USB] = PAC193X_VSENSE3_REG,
    .current_reg[UDB_POWER_MEASUREMENT_TARGET_ADAPTER_USB] = PAC193X_VSENSE4_REG,
    .current_reg[UDB_POWER_MEASUREMENT_TARGET_DUT0] = PAC193X_INVALID_REG,
    .current_reg[UDB_POWER_MEASUREMENT_TARGET_DUT1] = PAC193X_INVALID_REG,
};

void udb_config_init(void)
{
    if (udb_get_hw_version() < HW_VERSION_P3)
    {
        g_pwr_measurement_dev = k_pwr_measurement_dev_before_p3;
    }
    else
    {
        g_pwr_measurement_dev = k_pwr_measurement_dev_p3_and_after;
    }
}
