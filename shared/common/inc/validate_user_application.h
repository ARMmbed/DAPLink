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

#ifndef VALIDATE_USER_APPLICATION_H
#define VALIDATE_USER_APPLICATION_H

#if   defined(TARGET_MK20D5)
//TODO: find symbols from the Linker to do this
#define FLASH_VALID_LO	0x410
#define FLASH_VALID_HI	0x1FFFFFFF
#define RAM_VALID_LO	0x1FFFE000
#define RAM_VALID_HI	0x20002000

#elif defined(TARGET_LPC11U35)

#else
#error TARGET_ not defined
    
#endif

int validate_user_application(int app_address);

#endif
