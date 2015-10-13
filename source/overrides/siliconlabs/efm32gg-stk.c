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

#include "version.h"
#include "config_settings.h"

// URL_NAME and DRIVE_NAME must be 11 characters excluding
// the null terminated character
const char daplink_url_name[11] =   "MBED    HTM";
const char daplink_drive_name[11] = "EFM32 DAP  ";
const char * const daplink_target_url = "https://mbed.org/device/?code=@A";
