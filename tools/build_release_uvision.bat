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
rmdir /q /s uvision_release
mkdir uvision_release
git rev-parse --verify HEAD                     > uvision_release\git_info.txt
git diff --no-ext-diff --quiet --exit-code      >> uvision_release\git_info.txt
echo Uncommitted Changes: %errorlevel%          >> uvision_release\git_info.txt
virtualenv env
call env\Scripts\activate
REM use project requirements if not specified
if [%1]==[] pip install -r requirements.txt
REM use custom requirements if specified
if not [%1]==[] pip install -r %1
pip freeze                                      > uvision_release\build_requirements.txt
REM build but continue if there are errors
progen generate -t uvision -b
SET LEVEL=%ERRORLEVEL%
python tools/copy_release_files.py
if %errorlevel% neq 0 exit %errorlevel%
exit /B %level%
