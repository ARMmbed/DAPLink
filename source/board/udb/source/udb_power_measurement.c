#include "udb_power_measurement.h"

#include <stm32h7xx_hal.h>
#include "pac193x.h"
#include "udb_errno.h"
#include "util.h"

extern pwr_measurement_dev_t g_pwr_measurement_dev;

static pac193x_cfg_t s_udb_power_measurement_cfg;

#define UDB_POWER_MEASUREMENT_MAINBOARD_USB_RESISTOR_VALUE    (15)
#define UDB_POWER_MEASUREMENT_ADAPTER_USB_RESISTOR_VALUE      (15)
#define UDB_POWER_MEASUREMENT_DENOMINATOR                     PAC193X_UNIPOLAR_DENOMINATOR

static const uint32_t k_udb_power_measurement_full_scale_current_uamp[] =
{
    [UDB_POWER_MEASUREMENT_TARGET_MAINBOARD_USB]   = PAC193X_FULL_SCALE_RANGE_MV * 1000000 / UDB_POWER_MEASUREMENT_MAINBOARD_USB_RESISTOR_VALUE,
    [UDB_POWER_MEASUREMENT_TARGET_ADAPTER_USB]     = PAC193X_FULL_SCALE_RANGE_MV * 1000000 / UDB_POWER_MEASUREMENT_ADAPTER_USB_RESISTOR_VALUE,
    // For DUT0 and DUT1, we can not measure the current for lack of sense resistors.
};

static const char* k_udb_power_measurement_target_name[] =
{
    [UDB_POWER_MEASUREMENT_TARGET_MAINBOARD_USB]  = "Mainboard USB",
    [UDB_POWER_MEASUREMENT_TARGET_ADAPTER_USB]    = "Adapter USB",
    [UDB_POWER_MEASUREMENT_TARGET_DUT0]           = "DUT0",
    [UDB_POWER_MEASUREMENT_TARGET_DUT1]           = "DUT1",
};

int udb_power_measurement_init(void)
{
    s_udb_power_measurement_cfg.ctrl_cfg.val      = PAC193X_CTRL_POR_VALUE;
    s_udb_power_measurement_cfg.chan_dis_cfg.val  = PAC1934_CHANNEL_DIS_POR_VALUE;
    s_udb_power_measurement_cfg.neg_pwr_cfg.val   = PAC193X_NEG_PWR_POR_VALUE;
    s_udb_power_measurement_cfg.slow_cfg.val      = PAC193X_SLOW_POR_VALUE;

    return pac193x_init(&s_udb_power_measurement_cfg);
}

int udb_power_measurement_measure(void)
{
    int ret = pac193x_send_command(PAC193X_COMMAND_REFRESH);
    if (ret == UDB_SUCCESS)
    {
        HAL_Delay(PAC193X_REFRESH_STABLIZATION_TIME_MS);
    }

    return ret;
}

int udb_power_measurement_read_voltage_mV(udb_power_measurement_target_t target, uint16_t *voltage_mV)
{
    util_assert(target < UDB_POWER_MEASUREMENT_TARGET_COUNT);

    int ret;
    uint8_t vbus_reg[PAC193X_VBUSN_REG_SIZE];

    ret = pac193x_read_reg(g_pwr_measurement_dev.voltage_reg[target], PAC193X_VBUSN_REG_SIZE, vbus_reg);
    if (ret == UDB_SUCCESS)
    {

        uint32_t vbus = 0;
        for (uint8_t i = 0; i < PAC193X_VBUSN_REG_SIZE; ++i)
        {
            vbus = (vbus << 8) + vbus_reg[i];
        }

        // can not divide the denominator first, both FSV and vbus are smaller than the denominator
        *voltage_mV = (uint16_t)(PAC193X_FULL_SCALE_VOLTAGE_MV * vbus / UDB_POWER_MEASUREMENT_DENOMINATOR);
    }

    return ret;
}

int udb_power_measurement_read_current_uA(udb_power_measurement_target_t target, uint32_t *current_uA)
{
    util_assert(target < UDB_POWER_MEASUREMENT_TARGET_COUNT);

    int ret;
    uint8_t vsense_reg[PAC193X_VSENSEN_REG_SIZE];

    ret = pac193x_read_reg(g_pwr_measurement_dev.current_reg[target], PAC193X_VSENSEN_REG_SIZE, vsense_reg);

    if (ret == UDB_SUCCESS)
    {
        uint32_t vsense = 0;
        for (uint8_t i = 0; i < PAC193X_VSENSEN_REG_SIZE; ++i)
        {
            vsense = (vsense << 8) + vsense_reg[i];
        }

        // divide the denominator first to avoid overflow in the computation
        *current_uA = (k_udb_power_measurement_full_scale_current_uamp[target] / UDB_POWER_MEASUREMENT_DENOMINATOR) * vsense;
    }

    return ret;
}

const char* udb_power_measurement_get_target_name(udb_power_measurement_target_t target)
{
    util_assert(target < UDB_POWER_MEASUREMENT_TARGET_COUNT);

    return k_udb_power_measurement_target_name[target];
}

bool udb_power_measurement_can_measure_voltage(udb_power_measurement_target_t target)
{
    util_assert(target < UDB_POWER_MEASUREMENT_TARGET_COUNT);

    return (g_pwr_measurement_dev.voltage_reg[target] != PAC193X_INVALID_REG);
}

bool udb_power_measurement_can_measure_current(udb_power_measurement_target_t target)
{
    util_assert(target < UDB_POWER_MEASUREMENT_TARGET_COUNT);

    return (g_pwr_measurement_dev.current_reg[target] != PAC193X_INVALID_REG);
}

bool udb_power_measurement_can_measure_target(udb_power_measurement_target_t target)
{
    return (udb_power_measurement_can_measure_voltage(target) || udb_power_measurement_can_measure_current(target));
}
