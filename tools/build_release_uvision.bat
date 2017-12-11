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

@rem Script assumes working directory is workspace root. Force it.
cd %~dp0..\

@rem See if we can find uVision. This logic is consistent with progen
@if [%UV4%]==[] (
	@echo UV4 variable is not set, trying to autodetect..
	if EXIST c:\keil\uv4\uv4.exe (
		set UV4=c:\keil\uv4\uv4.exe
	) else if EXIST c:\keil_v5\uv4\uv4.exe (
		set UV4=c:\keil_v5\uv4\uv4.exe
	) else goto error_nomdk
)
@echo USING UV4=%UV4%

rmdir /q /s uvision_release
@if %errorlevel% neq 0 exit /B %errorlevel%

mkdir uvision_release
@if %errorlevel% neq 0 exit /B %errorlevel%

@git --version 2> nul
@if %errorlevel% neq 0 echo Error: git must be in PATH && exit /B %errorlevel%

git rev-parse --verify HEAD                     > uvision_release\git_info.txt
git diff --no-ext-diff --quiet --exit-code      >> uvision_release\git_info.txt

echo Uncommitted Changes: %errorlevel%          >> uvision_release\git_info.txt

virtualenv env
call env\Scripts\activate

@REM use project requirements if not specified
if [%1]==[] pip install -r requirements.txt
@REM use custom requirements if specified
if not [%1]==[] pip install -r %1
pip freeze                                      > uvision_release\build_requirements.txt

@REM build bootloader images first (In the future this should be done with a pattern like *_bl)
progen generate -t uvision -b -p kl26z_bl
progen generate -t uvision -b -p k20dx_bl
progen generate -t uvision -b -p sam3u2c_bl
@REM build but continue if there are errors
progen generate -t uvision -b
@set level=%errorlevel%
python tools/copy_release_files.py
@if %errorlevel% neq 0 exit /B %errorlevel%

@exit /B %level%

:error_nomdk
@echo Error: Keil MDK not installed or not found. If you installed it to a 
@echo non-default location, you need to set environment variable UV4 to 
@echo the path of the executable
@exit /B 1
