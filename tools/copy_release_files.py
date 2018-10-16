#
# DAPLink Interface Firmware
# Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

"""
Copy release files from the uvision build directories into a release package

This script is intended to be called the script creating a daplink build.
"""
from __future__ import absolute_import
from __future__ import print_function

import os
import shutil
import argparse

COPY_PATTERN_LIST = [
    "%s_crc.bin",
    "%s_crc.hex",
    "%s_crc.txt",
    ]
OPTIONAL_COPY_PATTERN_LIST = [
    "%s.axf",
    "%s.elf",
    "%s_crc_legacy_0x8000.bin",
    "%s_crc_legacy_0x5000.bin",
    "%s_crc_legacy.txt",
    "%s_crc_legacy.txt",
    "%s_crc.c",
    "%s.build_log.htm",
    "%s.map",
    "%s.htm",
    "%s_map.html",
]

TOOL_DIR = { 
    'uvision' : { 'proj_dir': os.path.join('projectfiles', 'uvision') , 'rel_dir' : 'uvision_release', 'build_dir' : 'build' },
    'mbedcli' : { 'proj_dir': 'BUILD' , 'rel_dir' : 'mbedcli_release', 'build_dir' : 'ARM-CUSTOM_PROFILE' }        
}


def main():
    """Copy imporant files for the current release"""
    parser = argparse.ArgumentParser(description='Copy imporant files for the current release')
    parser.add_argument('--project-tool', type=str, default='uvision', choices=['uvision', 'mbedcli'], help='Choose from uvision and mbedcli')
    args = parser.parse_args()

    self_path = os.path.abspath(__file__)
    tools_dir = os.path.dirname(self_path)
    daplink_dir = os.path.dirname(tools_dir)

    if os.path.basename(tools_dir) != "tools":
        print("Error - this script must be run from the tools directory")
        exit(-1)

    proj_dir = os.path.join(daplink_dir, TOOL_DIR[args.project_tool]['proj_dir'])
    rel_dir = os.path.join(daplink_dir, TOOL_DIR[args.project_tool]['rel_dir'])
    build_dir = TOOL_DIR[args.project_tool]['build_dir']
    # Make sure uvision dir is present
    if not os.path.isdir(proj_dir):
        print("Error - uvision directory '%s' missing" % proj_dir)
        exit(-1)

    # Clean release dir is present
    if os.path.isdir(rel_dir):
        shutil.rmtree(rel_dir)

    os.mkdir(rel_dir)
    
    project_list = os.listdir(proj_dir)
    for project in project_list:
        src_dir = os.path.join(proj_dir, project, build_dir)
        dest_dir = os.path.join(rel_dir, project.lower())

        #only copy a built project
        if not os.path.exists(src_dir):
            continue

        # File must not have been copied already
        if os.path.exists(dest_dir):
            print("Error - package dir '%s' alread exists" % dest_dir)
            exit(-1)
        os.mkdir(dest_dir)

        for file_pattern in COPY_PATTERN_LIST:
            file_name = file_pattern % project.lower()
            file_source = os.path.join(src_dir, file_name)
            file_dest = os.path.join(dest_dir, file_name)
            shutil.copy(file_source, file_dest)
        for file_pattern in OPTIONAL_COPY_PATTERN_LIST:
            file_name = file_pattern % project.lower()
            file_source = os.path.join(src_dir, file_name)
            if os.path.isfile(file_source):
                file_dest = os.path.join(dest_dir, file_name)
                shutil.copy(file_source, file_dest)

main()
