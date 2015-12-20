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
#include "compiler.h"
#include "cortex_m.h"

// Setting/flash should only be changed in the
// bootloader
#define FLASH_WRITE_ALLOWED  defined(DAPLINK_BL)

#if FLASH_WRITE_ALLOWED
    #include "FlashPrg.h"
#endif

// 'kvld' in hex - key valid
#define CFG_KEY             0x6b766c64
#define SECTOR_BUFFER_SIZE  16

// WARNING - THIS STRUCTURE RESIDES IN NON-VOLATILE STORAGE!
// Be careful with changes:
// -Only add new members to end end of this structure
// -Do not change the order of members in this structure
// -Structure must remain packed so no padding bytes are added
typedef struct __attribute__ ((__packed__)) cfg_setting {
    uint32_t key;               // Magic key to indicate a valid record
    uint16_t size;              // Size of cfg_setting_t

    // Configurable values
    uint8_t auto_rst;

    // Add new members here

} cfg_setting_t;

// Make sure FORMAT in generate_config.py is updated if size changes
COMPILER_ASSERT(sizeof(cfg_setting_t) == 7);

// Sector buffer must be as big or bigger than settings
COMPILER_ASSERT(sizeof(cfg_setting_t) < SECTOR_BUFFER_SIZE);
// Sector buffer must be a multiple of 4 bytes at least.
// ProgramPage for some interfaces, like the k20dx, require that 
// the data is a multiple of 4 bytes, otherwise programming will
// fail.  Assert 8 byte alignement just to be safe.
COMPILER_ASSERT(SECTOR_BUFFER_SIZE % 8 == 0);

// WARNING - THIS STRUCTURE RESIDES IN RAM STORAGE!
// Be careful with changes:
// -Only add new members to end end of this structure
// -Do not change the order of members in this structure
// -Structure must remain packed so no padding bytes are added
typedef struct __attribute__ ((__packed__)) cfg_ram {
    uint32_t key;               // Magic key to indicate a valid record
    uint16_t size;              // Offset of the last member from the start

    // Configurable values
    uint8_t hold_in_bl;

    // Add new members here

} cfg_ram_t;

// Configuration ROM
static volatile const cfg_setting_t config_rom __attribute__((section("cfgrom"), zero_init));
// Ram copy of ROM config
static cfg_setting_t config_rom_copy;
// Buffer for data to flash
#if FLASH_WRITE_ALLOWED
    static uint32_t write_buffer[SECTOR_BUFFER_SIZE/4];
#endif

// Configuration RAM
static volatile cfg_ram_t config_ram __attribute__((section("cfgram"), zero_init));
// Ram copy of RAM config
static cfg_ram_t config_ram_copy;

// Configuration defaults in flash
static const cfg_setting_t config_default =
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
    if (config_rom.size < sizeof(config_rom)) {
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
    cortex_int_state_t state;

    addr = (uint32_t)&config_rom;

    state = cortex_int_get_and_disable();
    status = EraseSector(addr);
    cortex_int_restore(state);
    if(status != 0) {
        return;
    }

    memset(write_buffer, 0xFF, sizeof(write_buffer));
    memcpy(write_buffer, new_cfg, sizeof(cfg_setting_t));
    state = cortex_int_get_and_disable();
    status = ProgramPage(addr, sizeof(write_buffer), write_buffer);
    cortex_int_restore(state);
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

    /* Initialize ROM */

    // Fill in the ram copy with the defaults
    memcpy(&config_rom_copy, &config_default, sizeof(config_rom_copy));

    // Read settings from flash if the key is valid
    if (CFG_KEY == config_rom.key) {
        uint32_t size = MIN(config_rom.size, sizeof(config_rom));
        memcpy(&config_rom_copy, (void*)&config_rom, size);
    }

    // Fill in special values
    config_rom_copy.key = CFG_KEY;
    config_rom_copy.size = sizeof(config_rom);

    // Write settings back to flash if they are out of date
    // Note - program_cfg only programs data in bootloader mode
    if (config_needs_update()) {
        // Program with defaults if none are set
        program_cfg(&config_rom_copy);
    }

    /* Initialize RAM */

    // Initialize RAM copy
    memset(&config_ram_copy, 0, sizeof(config_ram_copy));

    // Read settings from RAM if the key is valid
    if (CFG_KEY == config_ram.key) {
        uint32_t size = MIN(config_ram.size, sizeof(config_ram));
        memcpy(&config_ram_copy, (void*)&config_ram, size);
    }

    // Initialize RAM
    memset((void*)&config_ram, 0, sizeof(config_ram));
    config_ram.key = CFG_KEY;
    config_ram.size = sizeof(config_ram);
}

void config_set_auto_rst(bool on)
{
#if FLASH_WRITE_ALLOWED
    config_rom_copy.auto_rst = on;
    program_cfg(&config_rom_copy);
#endif
}

bool config_get_auto_rst()
{
    return config_rom_copy.auto_rst;
}

void config_ram_set_hold_in_bl(bool hold)
{
    config_ram.hold_in_bl = hold;
}

bool config_ram_get_hold_in_bl()
{
    return config_ram.hold_in_bl;
}

bool config_ram_get_initial_hold_in_bl()
{
    return config_ram_copy.hold_in_bl;
}
