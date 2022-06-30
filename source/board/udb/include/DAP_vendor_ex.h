#ifndef DAP_VENDOR_EX_H
#define DAP_VENDOR_EX_H

typedef enum
{
    ID_DAP_VendorEx32_I2C_READ = ID_DAP_VendorExFirst,
    ID_DAP_VendorEx33_I2C_WRITE,
    ID_DAP_VendorEx34_DUT_PIN_GROUP_WRITE,
    ID_DAP_VendorEx35_DUT_PWR_CTRL,
    ID_DAP_VendorEx36_INTERFACE_VERSION_DETAILS,
    ID_DAP_VendorEx37_HOLD_IN_BL,
    ID_DAP_VendorEx38_RESET_DAPLINK,
    ID_DAP_VendorEx39_READ_UDC_ADAPTER_TYPE_ADC,
    ID_DAP_VendorEx40_MEASURE_POWER,
    ID_DAP_VendorEx41_BOOTLOADER_VERSION_DETAILS,
    ID_DAP_VendorEx42_CHANGE_SWD_DUT,

    // Add new commands before the last command
    ID_DAP_VendorEx126_LAST = ID_DAP_VendorExLast,
} DAP_vendor_ex_cmd_t;

#endif // DAP_VENDOR_EX_H
