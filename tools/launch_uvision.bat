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
:: Launches uVision with the python environment needed to build DAPLink
::
:: git and python are expected to be in PATH. Project will fail to build otherwise
:: 

setlocal

@rem Script assumes working directory is workspace root. Force it.
cd %~dp0..\

@rem See if we can find uVision. This logic is consistent with progen
@set uv4exe=c:\Keil_v5\UV4\UV4.exe
@if exist %uv4exe% goto label1
	@if [%UV4%]==[] goto error_nomdk
set uv4exe=%UV4%
:label1

@set env_exists=0
@if exist env set env_exists=1
@if [%env_exists%]==[0] echo Creating python virtual environment && virtualenv env
call env\Scripts\activate

@echo Doing pip install
@REM use project requirements if not specified
if [%1]==[] pip install -r requirements.txt
@REM use custom requirements if specified
if not [%1]==[] pip install -r %1

start %uv4exe%
exit /B 0

:error_nomdk
@echo Error: Keil MDK not installed or not found. If you installed it to a 
@echo non-default location, you need to set environment variable UV4 to 
@echo the path of the executable
@exit /B 1

