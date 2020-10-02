/**
 * @file power_monitor.c
 * @brief
 */

#include "pwr_mon.h"
#include "adc.h"
#include "fsl_adc16.h"
#include "fsl_pmc.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

#define ADC_VBG_CHANNEL     27U
#define ADC_VBG_MUX         (kADC16_ChannelMuxA)
#define ADC_VREFH_CHANNEL   29U
#define ADC_VREFH_MUX       (kADC16_ChannelMuxA)

#define ADC_MV_TO_ADC(x)    ((x) * 0xFFF / 3300)  // Convert mV to ADC value (12bit and 3.3V reference)
#define BATT_MIN_VOLTAGE    1500

void pwr_mon_bandgap_init(void);
uint32_t pwr_mon_read_vbg(uint32_t channelGroup);
uint32_t pwr_mon_adc_to_mv(uint32_t raw_adc);

void pwr_mon_init(void)
{
    gpio_pin_config_t pin_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    
    adc_init();
    
    // Configure VMON_BAT and RUN_VBAT_SENSE
    GPIO_PinInit(PIN_RUN_VBAT_SENSE_GPIO, PIN_RUN_VBAT_SENSE_BIT, &pin_config);
    PORT_SetPinMux(PIN_RUN_VBAT_SENSE_PORT, PIN_RUN_VBAT_SENSE_BIT, kPORT_MuxAsGpio);
    PORT_SetPinMux(PIN_VMON_BAT_PORT, PIN_VMON_BAT_BIT, PIN_VMON_BAT_ALT_MODE);
}

power_source_t pwr_mon_get_power_source(void) {
    power_source_t power_source = PWR_SOURCE_NONE;
    uint32_t bat_voltage_mv = 0;
    // Detect if device is battery powered
    GPIO_PinWrite(PIN_RUN_VBAT_SENSE_GPIO, PIN_RUN_VBAT_SENSE_BIT, 1);
    // Add a ~3ms delay to allow the 100nF capacitors to charge to about 3*RC.
    // 3 clock cycles per loop at -O2 ARMCC optimization
    for (uint32_t count = 48000; count > 0UL; count--); 
    uint32_t bat_adc = adc_read_channel(0, PIN_VMON_BAT_ADC_CH, PIN_VMON_BAT_ADC_MUX);
    GPIO_PinWrite(PIN_RUN_VBAT_SENSE_GPIO, PIN_RUN_VBAT_SENSE_BIT, 0);
    
    bool usb_on = (((PIN_WAKE_ON_EDGE_GPIO->PDIR) >> PIN_WAKE_ON_EDGE_BIT) & 0x01U) ? false : true;
    
    // Compensate for voltage divider with ratio of 11
    bat_adc = bat_adc * 11;
    
    bat_voltage_mv = pwr_mon_adc_to_mv(bat_adc);
    
    if (usb_on == true && bat_voltage_mv < (BATT_MIN_VOLTAGE)) {
        power_source = PWR_USB_ONLY;
    } else if (usb_on == true && bat_voltage_mv >= (BATT_MIN_VOLTAGE)) {
        power_source = PWR_USB_AND_BATT;
    } else if (usb_on == false && bat_voltage_mv >= (BATT_MIN_VOLTAGE)) {
        power_source = PWR_BATT_ONLY;
    } else if (usb_on == false && bat_voltage_mv < (BATT_MIN_VOLTAGE)) {
        power_source = PWR_SOURCE_NONE;
    }

    return power_source;
}

uint32_t pwr_mon_vcc_mv(void) {
    // ADC read the Vref (same as Vcc) and the Vbg 1V internal reference
    uint32_t vref_high = adc_read_channel(0, ADC_VREFH_CHANNEL, ADC_VREFH_MUX);
    uint32_t ref_volt = pwr_mon_read_vbg(0);

    // Reference voltage is 1V, scale to millivolts
    return vref_high * 1000 / ref_volt;
}

void pwr_mon_bandgap_init(void) {
    pmc_bandgap_buffer_config_t pmcBandgapConfig;
    pmcBandgapConfig.enable = true;
    pmcBandgapConfig.enableInLowPowerMode = false;
    PMC_ConfigureBandgapBuffer(PMC, &pmcBandgapConfig);
}

uint32_t pwr_mon_read_vbg(uint32_t channelGroup) {
    // Ensure the Vbg is enabled from first use only
    static bool bandgap_enabled = false;
    if (!bandgap_enabled) {
        pwr_mon_bandgap_init();
        bandgap_enabled = true;
    }
    return adc_read_channel(channelGroup, ADC_VBG_CHANNEL, ADC_VBG_MUX);
}

uint32_t pwr_mon_adc_to_mv(uint32_t raw_adc)
{
    // ADC read Vbg, a 1V internal reference
    uint32_t ref_volt_read = pwr_mon_read_vbg(0);

    // Scale ADC read out to millivolts
    return (raw_adc * 1000) / ref_volt_read;
}
