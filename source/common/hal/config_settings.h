/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
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
#ifndef CONFIG_SETTINGS_H
#define CONFIG_SETTINGS_H

#include<stdbool.h>
#include "target_config.h"

// 'kvld' in hex - key valid
#define CFG_KEY             0x6b766c64
#define CFG_VALID(dev)      (NULL != (dev).cfg && CfG_KEY == (dev).cfg->key)
// Size of the structure not including the member 'end'.  This define is used
// instead of sizeof() so the exact size can be obtained, rather than
// the 4 byte aligned size that sizeof() returns.
#define CFG_SIZE_CURRENT    (offsetof(cfg_setting_t, end))

// WARNING - THIS STRUCTURE RESIDES IN NON-VOLATILE STORAGE! 
// Be careful with changes:
// -Only add new members to end end of this structure
// -Do not change the order of members in this structure
typedef struct cfg_setting {
    uint32_t key;               // Magic key to indicate a valid record
    uint16_t offset_of_end;     // Offset of the last member from the start

    // Configurable values
    uint8_t auto_rst;
    
    // Add new members here
    
    uint8_t end; // Must be last member
} cfg_setting_t;

extern const cfg_setting_t config_default;

void config_init(void);
void config_set_auto_rst(bool on);
bool config_get_auto_rst(void);

#endif
