#
# DAPLink Interface Firmware
# Copyright (c) 2009-2018, ARM Limited, All Rights Reserved
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

import argparse
import os
import shutil
import sys

from subprocess import check_output, CalledProcessError
from pre_build_script import generate_version_file
from generate_mbedcli_files import generate_mbedcli_files
from post_compute_crc import post_compute_crc
from package_release_files import package_release_files

def mbedcli_compile(daplink_dir, build_folder, project, toolchain, clean):
    generate_mbedcli_files(os.path.join(daplink_dir, "projects.yaml"), project)
    project_dir=os.path.join(daplink_dir, build_folder, project.upper())
    if clean is True and os.path.exists(project_dir):
        print("Deleting %s" % project_dir)
        shutil.rmtree(project_dir, ignore_errors=True)
    try:
        check_output("mbed compile -m %s -t %s --profile custom_profile.json" % (project, toolchain), shell=True)
        cli_name_out = os.path.basename(daplink_dir)
        build_dir = os.path.join(project_dir, toolchain)
        for file in os.listdir(build_dir):
            if file.startswith(cli_name_out):
                os.rename(os.path.join(build_dir, file), os.path.join(build_dir, file.replace(cli_name_out, project, 1)))
        cli_hex_output = os.path.join(daplink_dir, build_folder, project.upper(), toolchain, project + ".hex")
        crc_file_output = os.path.join(daplink_dir, build_folder, project.upper(), toolchain, project + "_crc")
        post_compute_crc(cli_hex_output, crc_file_output)
    except CalledProcessError:
        print("Error - mbed compile error")
        exit(1)

def main():
    """python -u tools/mbedcli_compile.py --project kl26z_microbit_if kl26z_bl --clean"""
    parser = argparse.ArgumentParser(description='mbedcli compile support for DAPLink')
    parser.add_argument('--project', help='Selectively compile only the firmware specified',
                        nargs='+', type=str, default=[], required=False)
    parser.add_argument('--release_version', type=int, help='Create a release with this version')
    parser.add_argument('--build_folder', type=str, default='BUILD', help='Release directory to grab files from')
    parser.add_argument('--release_folder', type=str, default='firmware', help='Directory to create and place files in')
    parser.add_argument('--toolchain', type=str, default='ARM', help='Toolchain directory if present')
    parser.add_argument('--clean', dest='clean', action='store_true')
    parser.set_defaults(clean=False)
    args = parser.parse_args()
    self_path = os.path.abspath(__file__)
    tools_dir = os.path.dirname(self_path)
    daplink_dir = os.path.dirname(tools_dir)
    test_dir = os.path.join(daplink_dir, "test")
    sys.path.append(test_dir)
    #get all projects
    import info
    if os.path.basename(tools_dir) != "tools":
        print("Error - this script must be run from the tools directory")
        exit(-1)
    version_git_dir = os.path.join(daplink_dir, "source", "daplink")
    generate_version_file(version_git_dir)
    if not args.project == []: 

        for project in args.project:
            mbedcli_compile(daplink_dir, args.build_folder, project, args.toolchain, args.clean)
    else:
        print("compiling all firmware")
        bootloader_list = []
        #compile all the bootloader
        for board_id, fimware, bootloader, target in info.SUPPORTED_CONFIGURATIONS:
            if bootloader is not None and bootloader not in bootloader_list:
                bootloader_list.append(bootloader)
                mbedcli_compile(daplink_dir, args.build_folder, bootloader, args.toolchain, True)

        #compile all projects in the release info
        for prj_name, legacy, offset, extension in info.PROJECT_RELEASE_INFO:
            print("Compiling %s" % prj_name)
            mbedcli_compile(daplink_dir, args.build_folder, prj_name, args.toolchain, True)

        if args.release_version is not None:
            release_dir = args.release_folder + "%04i" % args.release_version 
            if os.path.exists(release_dir):
                print("Deleting %s" % release_dir)
                shutil.rmtree(release_dir, ignore_errors=True)
            package_release_files(args.build_folder, release_dir, args.release_version, args.toolchain)

main()
