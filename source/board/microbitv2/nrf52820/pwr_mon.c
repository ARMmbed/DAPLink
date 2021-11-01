/**
 * @file    pwr_mon.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2021 Micro:bit Educational Foundation
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "nrf.h"

#include "IO_Config.h"

#include "pwr_mon_adc.h"
#include "pwr_mon.h"

// Define to enable debugging
//#define PWR_MON_DEBUG 1

#ifdef PWR_MON_DEBUG

#define MSC_DEBUG 1
#include "daplink_debug.h"

uint32_t bat_lo = 0xFFFFFFFF;
uint32_t bat_hi = 0;

uint32_t vin_lo = 0xFFFFFFFF;
uint32_t vin_hi = 0;

uint32_t nousb_vin = 0;
uint32_t nousb_bat = 0;

uint32_t low_vin = 0;
uint32_t low_bat = 0;

uint32_t dbg_bat[50];
uint32_t dbg_cc[50];
uint32_t dbg_count;

uint32_t dbg_bat_nousb[50];
uint32_t dbg_cc_nousb[50];
uint32_t dbg_count_nousb;
#endif // PWR_MON_DEBUG


static void power_pins_init();
uint32_t pwr_mon_get_vbat_mv_imp();
static uint32_t pwr_mon_comp_mv(uint32_t psel, uint32_t vref, uint32_t *comps_count);


// Volatge monitor pins
// RUN_VBAT_SENSE   Enable Battery Voltage monitor
// VBAT_SENSE       Battery Voltage monitor
// VBUS_ABSENT      High when USB power is not connected (WAKE_ON_EDGE)

#define NRF52833_PIN_RUN_VBAT_SENSE         NRF_GPIO_PIN_MAP(0, 31)
#define NRF52833_PIN_VBAT_SENSE             NRF_GPIO_PIN_MAP(0, 30)
#define NRF52833_PIN_VBAT_SENSE_COMP        6 //COMP_PSEL_PSEL_AnalogInput6
#define NRF52833_PIN_VIN_COMP               0 //Not available
#define NRF52833_PIN_VBUS_ABSENT            NRF_GPIO_PIN_MAP(0, 11)

#define NRF52820_PIN_RUN_VBAT_SENSE         NRF_GPIO_PIN_MAP(0, 2)
#define NRF52820_PIN_VBAT_SENSE             NRF_GPIO_PIN_MAP(0, 3)
#define NRF52820_PIN_VBAT_SENSE_COMP        COMP_PSEL_PSEL_AnalogInput1
#define NRF52820_PIN_VIN_COMP               COMP_PSEL_PSEL_VddhDiv5
#define NRF52820_PIN_VBUS_ABSENT            NRF_GPIO_PIN_MAP(0, 7)

#ifdef NRF528XX_DYNAMIC_PIN

uint32_t pin_run_vbat_sense;
uint32_t pin_vbat_sense;
uint32_t pin_vbat_sense_comp;
uint32_t pin_vin_comp;
uint32_t pin_vbus_absent;

COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52833(NRF52833_PIN_RUN_VBAT_SENSE));
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52833(NRF52833_PIN_VBAT_SENSE));
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52833(NRF52833_PIN_VBUS_ABSENT));

COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52820(NRF52820_PIN_RUN_VBAT_SENSE));
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52820(NRF52820_PIN_VBAT_SENSE));
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52820(NRF52820_PIN_VBUS_ABSENT));
#else
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52820(NRF52820_PIN_RUN_VBAT_SENSE));
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52820(NRF52820_PIN_VBAT_SENSE));
COMPILER_ASSERT(GPIO_CHECK_PRESENT_NRF52820(NRF52820_PIN_VBUS_ABSENT));
#endif

#ifdef NRF528XX_DYNAMIC_PIN
#define PIN_RUN_VBAT_SENSE      pin_run_vbat_sense
#define PIN_VBAT_SENSE          pin_vbat_sense
#define PIN_VBAT_SENSE_COMP     pin_vbat_sense_comp
#define PIN_VIN_COMP            pin_vin_comp
#define PIN_VBUS_ABSENT         pin_vbus_absent
#else
#define PIN_RUN_VBAT_SENSE      NRF52820_PIN_RUN_VBAT_SENSE
#define PIN_VBAT_SENSE          NRF52820_PIN_VBAT_SENSE
#define PIN_VBAT_SENSE_COMP     NRF52820_PIN_VBAT_SENSE_COMP
#define PIN_VIN_COMP            NRF52820_PIN_VIN_COMP
#define PIN_VBUS_ABSENT         NRF52820_PIN_VBUS_ABSENT
#endif

static void power_pins_init()
{
#ifdef NRF528XX_DYNAMIC_PIN
    if (NRF_FICR->INFO.PART == 0x52833) {
        // nRF52833
        pin_run_vbat_sense  = NRF52833_PIN_RUN_VBAT_SENSE;
        pin_vbat_sense      = NRF52833_PIN_VBAT_SENSE;
        pin_vbat_sense_comp = NRF52833_PIN_VBAT_SENSE_COMP;
        pin_vin_comp        = NRF52833_PIN_VIN_COMP;
        pin_vbus_absent     = NRF52833_PIN_VBUS_ABSENT;
    } else {
        // nRF52820
        pin_run_vbat_sense  = NRF52820_PIN_RUN_VBAT_SENSE;
        pin_vbat_sense      = NRF52820_PIN_VBAT_SENSE;
        pin_vbat_sense_comp = NRF52820_PIN_VBAT_SENSE_COMP;
        pin_vin_comp        = NRF52820_PIN_VIN_COMP;
        pin_vbus_absent     = NRF52820_PIN_VBUS_ABSENT;
    }
#endif
}


void pwr_mon_init(void)
{
    power_pins_init();

    // Configure VMON_BAT and RUN_VBAT_SENSE
    gpio_cfg_output(GPIO_REG(PIN_RUN_VBAT_SENSE), GPIO_IDX(PIN_RUN_VBAT_SENSE)); 
    gpio_cfg_input(GPIO_REG(PIN_VBAT_SENSE), GPIO_IDX(PIN_VBAT_SENSE), NRF_GPIO_PIN_NOPULL);
    gpio_cfg_input(GPIO_REG(PIN_VBUS_ABSENT), GPIO_IDX(PIN_VBUS_ABSENT), NRF_GPIO_PIN_NOPULL);
}


power_source_t pwr_mon_get_power_source(void)
{
    uint32_t        bat_min_voltage = 1550;
    uint32_t        bat_voltage_mv  = 0;
    uint32_t        vin_voltage_mv  = 0;
    power_source_t  power_source    = PWR_SOURCE_NONE;

    // Read VBUS_ABSENT (WAKE_ON_EDGE) pin for detecting if board is USB powered
    bool usb_on = gpio_read(GPIO_REG(PIN_VBUS_ABSENT), GPIO_IDX(PIN_VBUS_ABSENT)) ? false : true;
    
    // Read battery voltage
    // Allow ~3ms for the 100nF capacitors to charge to about 3*RC.
    // The voltage should reach bat_min_voltage within 3ms if a battery is present
    bat_voltage_mv = pwr_mon_get_vbat_mv_imp( 3000, bat_min_voltage);
    
    // Read Vin voltage
    //vin_voltage_mv = pwr_mon_get_vin_mv();

    // Get power source based on battery and USB
    if (usb_on) {
        power_source = bat_voltage_mv < bat_min_voltage ? PWR_USB_ONLY : PWR_USB_AND_BATT;
    } else if (bat_voltage_mv >= bat_min_voltage) {
        power_source = PWR_BATT_ONLY;
        // TODO? If battery voltage is greater than Vin, it means the battery is used
        // With nRF52820, bat values after 3ms are much less than vin. The max is reached after 11-13ms
        // bat values are greater with battery + USB or EC than battery alone 
        // if (bat_voltage_mv + 250 > vin_voltage_mv) {
        //     power_source = PWR_BATT_ONLY;
        // }
    }

#ifdef PWR_MON_DEBUG
    if (gpio_read(GPIO_REG(PIN_VBUS_ABSENT), GPIO_IDX(PIN_VBUS_ABSENT))) {
        nousb_vin = vin_voltage_mv;
        nousb_bat = bat_voltage_mv;
        if (bat_voltage_mv < bat_min_voltage) {
            low_vin = vin_voltage_mv;
            low_bat = bat_voltage_mv;
        }
    }
    debug_msg("PIN_VBUS_ABSENT %d usb_on %d\n", (int) PIN_VBUS_ABSENT, usb_on ? 1 : 0);
    debug_msg("vin %d bat %d nousb vin %d bat %d low vin %d bat %d\n", (int) vin_voltage_mv, (int) bat_voltage_mv, (int) nousb_vin, (int) nousb_bat, (int) low_vin, (int) low_bat);
#endif // PWR_MON_DEBUG

    return power_source;
}

uint32_t pwr_mon_get_vin_mv(void)
{
    uint32_t vin;
    
    if (NRF_FICR->INFO.PART == 0x52833) {
        // nRF52833
#ifdef PWR_MON_DEBUG
        debug_msg("pwr_mon_adc_vin\n");
#endif // PWR_MON_DEBUG
        vin = pwr_mon_adc_vin();
    } else {
#ifdef PWR_MON_DEBUG
        debug_msg("pwr_mon_comp_mv(%d)\n", (int) PIN_VIN_COMP);
#endif // PWR_MON_DEBUG
        vin = pwr_mon_comp_mv(PIN_VIN_COMP, COMP_REFSEL_REFSEL_Int1V2, NULL);
    }

#ifdef PWR_MON_DEBUG
    if ( gpio_read(GPIO_REG(PIN_VBUS_ABSENT), GPIO_IDX(PIN_VBUS_ABSENT))) {
        if ( vin_lo > vin) vin_lo = vin;
        if ( vin_hi < vin) vin_hi = vin;
    }
    debug_msg("vin %d low vin %d bat %d\n", (int) vin, (int) low_vin, (int) low_bat);
#endif // PWR_MON_DEBUG

    return vin;
}

uint32_t pwr_mon_get_vbat_mv(void)
{
    // CODAL needs a reply within 5ms
    uint32_t bat = pwr_mon_get_vbat_mv_imp( 3000, 3300);

#ifdef PWR_MON_DEBUG
    debug_msg("bat %d\n", (int) bat);
#endif // PWR_MON_DEBUG
    return bat;
}


/**
 * Measures VBAT_SENSE voltage
 * @param max_us Approximate maximum time to wait in microseconds
 * @param max_mv Stop when detected voltage is at least this value. Zero for no maximum.
 * @return Voltage in millivolts.
 */
uint32_t pwr_mon_get_vbat_mv_imp( uint32_t max_us, uint32_t max_mv)
{
    uint32_t max_comps = (max_us + 49) / 50; // Each COMP typically takes >50us 
    uint32_t bat;
    uint32_t comps_count = 0;

#ifdef PWR_MON_DEBUG
    dbg_count = 0;
    debug_msg("\n");
    debug_msg("pwr_mon_get_vbat_mv_imp(%d,%d) run %d comp %d\n", (int) max_us, (int) max_mv, (int) PIN_RUN_VBAT_SENSE, (int) PIN_VBAT_SENSE_COMP);
#endif // PWR_MON_DEBUG

    // Enable voltage divider to take measurement
    gpio_write(GPIO_REG(PIN_RUN_VBAT_SENSE), GPIO_IDX(PIN_RUN_VBAT_SENSE), 1);

    while (true) {
        bat = pwr_mon_comp_mv(PIN_VBAT_SENSE_COMP, COMP_REFSEL_REFSEL_Int1V2, &comps_count);
        // Compensate for voltage divider
        bat = 4017 * bat / 1000;

#ifdef PWR_MON_DEBUG
        if ( dbg_count < 50) {
            dbg_bat[dbg_count] = bat;
            dbg_cc[dbg_count] = comps_count;
            dbg_count++;
        }
#endif // PWR_MON_DEBUG

        if ( comps_count >= max_comps) {
            break;
        }

        if ( max_mv > 0 && bat >= max_mv) {
            break;
        }
    }

    // Disable voltage divider
    gpio_write(GPIO_REG(PIN_RUN_VBAT_SENSE), GPIO_IDX(PIN_RUN_VBAT_SENSE), 0);

#ifdef PWR_MON_DEBUG
    if ( gpio_read(GPIO_REG(PIN_VBUS_ABSENT), GPIO_IDX(PIN_VBUS_ABSENT))) {
        for (int i = 0; i < dbg_count; i++) {
            dbg_bat_nousb[i] = dbg_bat[i];
            dbg_cc_nousb[i] = dbg_cc[i];
        }
        dbg_count_nousb = dbg_count;
    }

    if ( gpio_read(GPIO_REG(PIN_VBUS_ABSENT), GPIO_IDX(PIN_VBUS_ABSENT))) {
        if ( bat_lo > bat) bat_lo = bat;
        if ( bat_hi < bat) bat_hi = bat;
    }

    // int num =  dbg_count_nousb >= 50 ? 50 : dbg_count_nousb;
    // for (int i = 0; i < num; i++) {
    //   debug_msg("%d,%d\n", (int) dbg_cc_nousb[dbg_count_nousb - num + i], (int) dbg_bat_nousb[dbg_count_nousb - num + i]);
    // }

    // int numnum =  dbg_count >= 50 ? 50 : dbg_count;
    // for (int i = 0; i < numnum; i++) {
    //   debug_msg("%d,%d\n", (int) dbg_cc[dbg_count - numnum + i], (int) dbg_bat[dbg_count - numnum + i]);
    // }

    //debug_msg("vin lo %d hi %d bat lo %d hi %d\n", (int) vin_lo, (int) vin_hi, (int) bat_lo, (int) bat_hi);
#endif // PWR_MON_DEBUG

    return bat;
}


/**
 * Measures a voltage using COMP peripheral
 * @param psel NRF_COMP PSEL value
 * @param vref NRF_COMP internal REFSEL value
 * @param comps_count Pointer to integer to be incremented for each COMP sample. May be NULL.
 * @return Voltage in millivolts.
 */
static uint32_t pwr_mon_comp_mv(uint32_t psel, uint32_t vref, uint32_t *comps_count)
{
    uint32_t comps = comps_count ? *comps_count : 0;
    uint32_t mv = 0;
    int th0 = 0;
    int th1 = 63;
    int thmax = 0;
    int th;

    NRF_COMP->TASKS_STOP    = 1;
    NRF_COMP->ENABLE        = COMP_ENABLE_ENABLE_Disabled << COMP_ENABLE_ENABLE_Pos;
    NRF_COMP->SHORTS        = 0;
    NRF_COMP->INTENCLR      = NRF_COMP->INTENSET;
    NRF_COMP->HYST          = COMP_HYST_HYST_NoHyst << COMP_HYST_HYST_Pos;
    NRF_COMP->REFSEL        = (vref << COMP_REFSEL_REFSEL_Pos);
    NRF_COMP->PSEL          = psel << COMP_PSEL_PSEL_Pos;
    NRF_COMP->MODE          = (COMP_MODE_MAIN_SE << COMP_MODE_MAIN_Pos) | (COMP_MODE_SP_Normal << COMP_MODE_SP_Pos); // TODO COMP_MODE_SP_Low?

    while ( th1 - th0 > 0) {
        th = (th0 + th1 + 1) / 2;
        NRF_COMP->TH            = ((uint32_t) th << COMP_TH_THUP_Pos) | ((uint32_t) th << COMP_TH_THDOWN_Pos);
        NRF_COMP->ENABLE        = COMP_ENABLE_ENABLE_Enabled << COMP_ENABLE_ENABLE_Pos;
        NRF_COMP->EVENTS_READY  = 0;

        NRF_COMP->TASKS_START = 1;
        while( !NRF_COMP->EVENTS_READY) /*wait*/;
        NRF_COMP->TASKS_SAMPLE = 1;
        uint32_t above = NRF_COMP->RESULT;

        NRF_COMP->TASKS_STOP = 1;
        NRF_COMP->ENABLE    = COMP_ENABLE_ENABLE_Disabled << COMP_ENABLE_ENABLE_Pos;

        if (above) {
            th0 = th;
            thmax = th;
        } else {
            th1 = th - 1;
        }

        comps++;
    }

    if (comps_count) {
        *comps_count = comps;
    }

    switch (vref) {
        case COMP_REFSEL_REFSEL_Int1V2: mv = 1200; break;
        case COMP_REFSEL_REFSEL_Int1V8: mv = 1800; break;
        case COMP_REFSEL_REFSEL_Int2V4: mv = 2400; break;
    }

    mv = mv * thmax / 64;

#ifdef COMP_PSEL_PSEL_VddhDiv5
    if (psel == COMP_PSEL_PSEL_VddhDiv5) {
        return 5 * mv;
    }
#endif
    return mv;  
}
