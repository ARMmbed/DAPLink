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
progen generate -t uvision -b
python tools/copy_release_files.py
