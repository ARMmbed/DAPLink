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

#include "microbitv2_pins.h"
#include "pwr_mon_adc.h"
#include "pwr_mon.h"


uint32_t pwr_mon_get_vbat_mv_imp(uint32_t max_us, uint32_t max_mv);
static uint32_t pwr_mon_comp_mv(uint32_t psel, uint32_t vref, uint32_t *comps_count);


void pwr_mon_init(void)
{
    microbitv2_pins_init();

    // Configure VMON_BAT and RUN_VBAT_SENSE
    gpio_cfg_output(GPIO_REG(PIN_RUN_VBAT_SENSE), GPIO_IDX(PIN_RUN_VBAT_SENSE)); 
    gpio_cfg_input(GPIO_REG(PIN_VBAT_SENSE), GPIO_IDX(PIN_VBAT_SENSE), NRF_GPIO_PIN_NOPULL);
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

    return power_source;
}

uint32_t pwr_mon_get_vin_mv(void)
{
    uint32_t vin;
    
    if (NRF_FICR->INFO.PART == 0x52833) {
        // nRF52833
        vin = pwr_mon_adc_vin();
    } else {
        vin = pwr_mon_comp_mv(PIN_VIN_COMP, COMP_REFSEL_REFSEL_Int1V2, NULL);
    }

    return vin;
}

uint32_t pwr_mon_get_vbat_mv(void)
{
    // CODAL needs a reply within 5ms
    uint32_t bat = pwr_mon_get_vbat_mv_imp(3000, 3300);
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

    // Enable voltage divider to take measurement
    gpio_write(GPIO_REG(PIN_RUN_VBAT_SENSE), GPIO_IDX(PIN_RUN_VBAT_SENSE), 1);

    while (true) {
        bat = pwr_mon_comp_mv(PIN_VBAT_SENSE_COMP, COMP_REFSEL_REFSEL_Int1V2, &comps_count);
        // Compensate for voltage divider
        bat = 4017 * bat / 1000;

        if ( comps_count >= max_comps) {
            break;
        }

        if ( max_mv > 0 && bat >= max_mv) {
            break;
        }
    }

    // Disable voltage divider
    gpio_write(GPIO_REG(PIN_RUN_VBAT_SENSE), GPIO_IDX(PIN_RUN_VBAT_SENSE), 0);

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
