set TOOLS=..\..\..\tools
set DIR=..\..\..\source\common\hal
REM  Create default version_git.h.  This is so the project 
REM  builds if python is not installed
copy %DIR%\version_git_tmpl.txt %DIR%\version_git.h
REM  Run python script to delete default version_git.h and create the real one
python %TOOLS%\pre_build_script.py
REM Always return success
exit 0
