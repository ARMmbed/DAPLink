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
#ifndef MBED_HTM_H
#define MBED_HTM

#include "version_git.h"

#if GIT_LOCAL_MODS == 1
    #warning "Building with local modifications."
    #define GIT_LOCAL_MODS_STR "Yes"
#else
    #define GIT_LOCAL_MODS_STR "No"
#endif

const unsigned char WebSide[] = {
"<!-- mbed Microcontroller Website and Authentication Shortcut -->\r\n"
"<!-- Version: " FW_BUILD " Build: " __DATE__ " " __TIME__ " Git Commit SHA: "  GIT_COMMIT_SHA " Git local mods:" GIT_LOCAL_MODS_STR"-->\r\n"
"<html>\r\n"
"<head>\r\n"
"<meta http-equiv=\"refresh\" content=\"0; url=http://mbed.org/device/?code=@A\"/>\r\n"
"<title>mbed Website Shortcut</title>\r\n"
"</head>\r\n"
"<body></body>\r\n"
"</html>\r\n"
"\r\n"};

#endif
