/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2015-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include "config_settings.h"
#include "target_flash.h"
#include "target_config.h"

#define MIN(a,b) ((a) < (b) ? a : b)

// Setting/flash should only be changed in the
// bootloader
#define FLASH_WRITE_ALLOWED  defined(DAPLINK_BL)

#if FLASH_WRITE_ALLOWED
    #include "FlashPrg.h"
#endif

// Configuration ROM
static volatile const cfg_setting_t config_rom __attribute__((section("cfgrom"), zero_init));
// Ram copy of config
static cfg_setting_t config_ram;

// These are the defaults if no default is 
// supplied.  To overwrite set target_device.cfg to the
// desired defaults
static const cfg_setting_t default_config = 
{
    .auto_rst = 0,
};

// Check if the configuration in flash needs to be updated
static bool config_needs_update()
{
    // Update if the key is invalid
    if (CFG_KEY != config_rom.key) {
        return true;
    }

    // Update if the config key is valid but
    // has a smaller size.
    if (config_rom.offset_of_end < CFG_SIZE_CURRENT) {
        return true;
    }
    
    // The config is valid and has the right
    // size so it does not need to be updated
    return false;
}

// Reprogram the new settings if flash writing is allowed
static void program_cfg(cfg_setting_t* new_cfg)
{
#if FLASH_WRITE_ALLOWED
    uint32_t status;
    uint32_t addr;
    
    addr = (uint32_t)&config_rom;
    
    __disable_irq();
    status = EraseSector(addr);
    __enable_irq();
    if(status != 0) {
        return;
    }
    
    __disable_irq();
    status = ProgramPage(addr, sizeof(cfg_setting_t), (uint32_t*)new_cfg);
    __enable_irq();
    if (0 != status) {
        return;
    }
#endif
}

void config_init()
{
#if FLASH_WRITE_ALLOWED
    Init(0,0,0);
#endif

    // Fill in the ram copy with the defaults
    memcpy(&config_ram, &default_config, sizeof(config_ram));

    // Read settings from flash if the key is valid
    if (CFG_KEY == config_rom.key) {
        uint32_t size = MIN(config_rom.offset_of_end, CFG_SIZE_CURRENT);
        memcpy(&config_ram, (void*)&config_rom, size);
    }

    // Fill in special values
    config_ram.key = CFG_KEY;
    config_ram.offset_of_end = CFG_SIZE_CURRENT;
    config_ram.end = 0;

    // Write settings back to flash if they are out of date
    // Note - program_cfg only programs data in bootloader mode
    if (config_needs_update()) {
        // Program with defaults if none are set
        program_cfg(&config_ram);
    }
}

void config_set_auto_rst(bool on) 
{
    config_ram.auto_rst = on;
    program_cfg(&config_ram);
}

bool config_get_auto_rst()
{
    return config_ram.auto_rst;
}
