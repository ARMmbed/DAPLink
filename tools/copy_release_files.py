"""
Copy release files from the uvision build directories into a release package

This script is intended to be called the script creating a daplink build.
"""
from __future__ import absolute_import
from __future__ import print_function

import os
import shutil

UVISION_PROJ_DIR = os.path.join("projectfiles", "uvision")
RELEASE_PKG_DIR = os.path.join("uvision_release")
COPY_PATTERN_LIST = [
    "%s.axf",
    "%s.bin",
    "%s.build_log.htm",
    "%s.hex",
    "%s.htm",
    "%s.map",
    ]


def main():
    """Copy imporant files for the current release"""
    self_path = os.path.abspath(__file__)
    tools_dir = os.path.dirname(self_path)
    daplink_dir = os.path.dirname(tools_dir)

    if os.path.basename(tools_dir) != "tools":
        print("Error - this script must be run from the tools directory")
        exit(-1)

    uvision_dir = os.path.join(daplink_dir, UVISION_PROJ_DIR)
    release_dir = os.path.join(daplink_dir, RELEASE_PKG_DIR)

    # Make sure uvision dir is present
    if not os.path.isdir(uvision_dir):
        print("Error - uvision directory '%s' missing" % uvision_dir)
        exit(-1)

    # Make sure release dir is present
    if not os.path.isdir(release_dir):
        print("Error - release directory '%s' missing" % release_dir)
        exit(-1)

    project_list = os.listdir(uvision_dir)
    for project in project_list:
        src_dir = os.path.join(uvision_dir, project, "build")
        dest_dir = os.path.join(daplink_dir, RELEASE_PKG_DIR, project)
        # File must not have been copied already
        if os.path.exists(dest_dir):
            print("Error - package dir '%s' alread exists" % dest_dir)
            exit(-1)
        os.mkdir(dest_dir)

        for file_pattern in COPY_PATTERN_LIST:
            file_name = file_pattern % project
            file_source = os.path.join(src_dir, file_name)
            file_dest = os.path.join(dest_dir, file_name)
            shutil.copy(file_source, file_dest)

main()
