
:: a wrapper for source formatting

@echo off

:: Get the source path
set /p path_to_source=Enter the source path:
:: a method for removing the surrounding quotes when spaces are used
::  in the path of a passed directory
set path_to_source=%path_to_source:"=%
:: and the types of files to format
set cpp=*.cpp
set cc=*.cc
set c=*.c
set h=*.h
set hh=*.hh
set hh=*.hpp

echo.
echo formating files of type:
echo %path_to_source%\%cpp%
echo %path_to_source%\%cc%
echo %path_to_source%\%c%
echo %path_to_source%\%h%
echo %path_to_source%\%hh%
echo.

astyle --options=style.txt "%path_to_source%\%cpp%"
astyle --options=style.txt "%path_to_source%\%cc%"
astyle --options=style.txt "%path_to_source%\%c%"
astyle --options=style.txt "%path_to_source%\%hh%"
astyle --options=style.txt "%path_to_source%\%h%"
astyle --options=style.txt "%path_to_source%\%hpp%"


pause