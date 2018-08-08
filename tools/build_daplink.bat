::
:: DAPLink Interface Firmware
:: Copyright (c) 2009-2018, ARM Limited, All Rights Reserved
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
:: git and python are expected to be in PATH. Project will fail to build otherwise
::
:: Script assumes working directory is workspace root. Force it.
:: Build DAPLink firmware using build_daplink batch file.
::
:: ./tools/build_daplink.bat <project_name_1> <project_name_2> <project_name_3>
::
:: You can give any number of project name or if you want to bulid all then run below command
:: ./tools/build_daplink.bat

:: Script assumes working directory is workspace root. Force it.
cd %~dp0..\
@echo off

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
set uv4exe=%UV4%

@REM Flag for checking the input.txt exits or not.
Set /A flag=0

@REM use project projects if not specified
@If [%~1]==[] (
    @REM Generate all projects
    @echo Project name is not given...Generating all the projects..
    @echo progen generate -f projects.yaml -t uvision
    progen generate -f projects.yaml -t uvision
) Else (
    @REM use custom project if specified
    If EXIST projectfiles (
        DIR projectfiles\*.uvproj /s /b > tools/input.txt
        for %%x in (%*) do @for /F "tokens=*" %%A in (tools/input.txt) do (
                If ["%%~x.uvproj"] NEQ %%A (
                    progen generate -f projects.yaml -p %%~x -t uvision
                )
        )
        Set /A flag=1
        echo.
        @echo Projects are already generated.
    ) Else (
        for %%x in (%*) do (
            @REM Generate the project which is given in the command line argument
            echo.
            @echo progen generate -f projects.yaml -p %%~x uvision
            progen generate -f projects.yaml -p %%~x -t uvision
        )
    )
)

If %flag%==0 (
    DIR projectfiles\*.uvproj /s /b > tools/input.txt
    @echo Project Building...
    @set buff=Building...
) Else (
    @echo Project Re-Building...
    @set buff=Re-Building...
)
for /F "tokens=*" %%A in (tools/input.txt) do (
    @echo %%A %buff%
    %uv4exe% -b %%A -o buildOutput.txt
)
@echo All the projects successfully builded.!
@echo Exiting...!
@exit /B 0

:error_nomdk
@echo Error: Keil MDK not installed or not found. If you installed it to a
@echo non-default location, you need to set environment variable UV4 to
@echo the path of the executable
@exit /B 1