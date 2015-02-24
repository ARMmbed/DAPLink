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
import sys
import os
import argparse
from subprocess import check_output, CalledProcessError

VERSION_GIT_FILE_TEMPLATE = """
/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef VERSION_GIT_H
#define VERSION_GIT_H

#define GIT_COMMIT_SHA  \"%s\"
#define GIT_LOCAL_MODS  %d

#endif

"""


GIT_VERSION_FILE_PATH = "../../Common/inc/version_git.h"

def pre_build():
    print "#> Pre-build script start"

    # First thing to do is delete any existing generated files.
    try:
        os.remove(GIT_VERSION_FILE_PATH)
        print "#> Old git version file removed"
    except OSError:
        print "#> No git version file to remove"
        pass

    # Get the git SHA.
    print "#> Getting git SHA"
    try:
        git_sha = check_output("git rev-parse --verify HEAD", shell=True)
        git_sha = git_sha.strip()
    except:
        print "#> ERROR: Failed to get git SHA, do you have git.exe in your PATH environment variable?"
        return 1

    # Check are there any local, uncommitted modifications.
    print "#> Checking for local changes"
    try:
        check_output("git diff --no-ext-diff --quiet --exit-code")
    except CalledProcessError as e:
        git_has_changes = 1
    else:
        git_has_changes = 0


    #Create the version file.
    print "#> Creating new git version file"
    version_file = open(GIT_VERSION_FILE_PATH, 'a')
    version_file.write(VERSION_GIT_FILE_TEMPLATE % (git_sha, git_has_changes))
    version_file.close()

    print "#> Pre-build script completed"

    return 0

if __name__ == "__main__":
    exit(pre_build())
 