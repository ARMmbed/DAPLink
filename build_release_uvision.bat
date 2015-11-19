mkdir uvision_release
git rev-parse --verify HEAD                     > uvision_release\git_info.txt
git diff --no-ext-diff --quiet --exit-code      >> uvision_release\git_info.txt
echo Uncommitted Changes: %errorlevel%          >> uvision_release\git_info.txt
virtualenv env
call env\Scripts\activate
pip install -r requirements.txt
pip freeze                                      > uvision_release\build_requirements.txt
pgen export -t uvision -b
python tools/copy_release_files.py
