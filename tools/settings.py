"""
CMSIS-DAP Interface Firmware
Copyright (c) 2009-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
FROMELF = 'fromelf'
UV4 = r'C:\Keil\UV4\UV4.exe'

try:
    # Allow to overwrite the default settings without the need to edit the
    # settings file stored in the repository
    from private_settings import *
except ImportError:
    print "[WARNING] Using default settings. Define your settings in the file private_settings.py"
