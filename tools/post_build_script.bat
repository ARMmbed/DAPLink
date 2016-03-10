::
:: DAPLink Interface Firmware
:: Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
:: SPDX-License-Identifier: Apache-2.0
::
:: Licensed under the Apache License, Version 2.0 (the "License"); you may
:: not use this file except in compliance with the License.
:: You may obtain a copy of the License at
::
:: http://www.apache.org/licenses/LICENSE-2.0
::
:: Unless required by applicable law or agreed to in writing, software
:: distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
:: WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
:: See the License for the specific language governing permissions and
:: limitations under the License.
::

set KEIL_ARM=%2
set TOOLS=..\..\..\tools
REM make sure fromelf is part of path
set path=%KEIL_ARM%\ARMCC\bin;%path%
set base_name= %1
fromelf --bin %base_name%.axf -o %base_name%.bin
fromelf --i32 %base_name%.axf -o %base_name%.hex
python %TOOLS%\post_compute_crc.py %base_name%.hex %base_name%_crc
REM Always return success
exit 0
