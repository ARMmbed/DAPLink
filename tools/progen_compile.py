#!/usr/bin/env python
#
# DAPLink Interface Firmware
# Copyright (c) 2009-2020, ARM Limited, All Rights Reserved
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

from package_release_files import package_release_files
from post_build_script import post_build_script
from project_generator import generate
from pre_build_script import generate_version_file
import argparse
import logging
import shutil
import yaml
import sys
import os

self_path = os.path.abspath(__file__)
tools_dir = os.path.dirname(self_path)
daplink_dir = os.path.dirname(tools_dir)

PROJECTS_YAML = os.path.join(daplink_dir, "projects.yaml")
VERSION_YAML =  os.path.join(daplink_dir, "records", "tools", "version.yaml")

def load_project_list(path):
    project_list = []
    with open(path, 'r') as top_yaml:
        try:
            topdict = yaml.safe_load(top_yaml)
            for dict_key in topdict:
                if dict_key == 'projects':
                    for project in topdict[dict_key]:
                        project_list.append(project)
                    break
        except yaml.YAMLError as ex:
            print("Found yaml parse error", ex)
    return project_list

def get_core_count():
    try:
        import multiprocessing
        return multiprocessing.cpu_count()
    except:
        return 4

project_list = load_project_list(PROJECTS_YAML)
parser = argparse.ArgumentParser(description='project-generator compile support for DAPLink',
                                 epilog="List of supported projects (%d): %s" % (len(project_list), ", ".join(project_list)),
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
parser.add_argument('projects', help='Selectively compile only the firmware specified otherwise all projects',
                    nargs='*', type=str, default=[])
parser.add_argument('--release', dest='release', action='store_true', help='Create a release with the yaml version file')
parser.add_argument('--release-folder', type=str, default='firmware', help='Directory to create and place files in')
parser.add_argument('--supported', dest='supported', action='store_true', help='Generate the images with official identifiers')
parser.add_argument('--test', dest='test', action='store_true', help='Add test binaries to release')
parser.add_argument('--toolchain', '-t', type=str, help='Toolchain (default: make_gcc_arm)')
parser.add_argument('--generator', '-g', type=str, help='CMake Toolchain Generator (default: make)')
parser.add_argument('--clean', dest='clean', action='store_true', help='Rebuild or delete build folder before compile')
parser.add_argument('--ignore-failures', dest='ignore_failures', action='store_true', help='Continue build even in case of failures')
parser.add_argument('--parallel', dest='parallel', action='store_true', help='Build with multiple compilations in parallel')
parser.add_argument('-v', dest='verbosity', action='count', help='Increase verbosity (can be specify multiple times)', default=0)
parser.set_defaults(build=True)
parser.set_defaults(clean=False)
parser.set_defaults(ignore_failures=False)
parser.set_defaults(parallel=False)
parser.set_defaults(supported=False)
parser.set_defaults(release=False)
args = parser.parse_args()


toolchains = ["make_gcc_arm", "make_armcc", "make_armclang", # Make options
              "cmake_gcc_arm", "cmake_armcc", "cmake_armclang", # CMake options
              "gcc_arm", "armcc", "armclang"] # Aliases for the make options
toolchain = args.toolchain if args.toolchain else 'make_gcc_arm'
if toolchain not in toolchains:
    print("Unsupported toolchain '%s' (options: %s)\n" % (toolchain, ", ".join(toolchains)))
    exit(-1)

# armcc does not support Cortex-M33 and lpc55s69 is not ported to armclang
if 'armc' in toolchain:
    project_list = list(filter(lambda p: not p.startswith("lpc55"), project_list))
# musca projects are too large to fit when compiled with gcc. LTO should fix that but it does not work (yet)
if 'gcc' in toolchain and args.release:
    project_list = list(filter(lambda p: "musca" not in p, project_list))
# remove all test projects from list
if not args.test:
    project_list = list(filter(lambda p: not p.endswith("test_if"), project_list))

logging_level = logging.DEBUG if args.verbosity >= 2 else (logging.INFO if args.verbosity >= 1 else logging.WARNING)
logging.basicConfig(format="%(asctime)s %(name)020s %(levelname)s\t%(message)s", level=logging_level)
logger = logging.getLogger('progen_build')

if args.release:
    if len(args.projects) > 0:
        logger.warning("A release can should only be done on all packages")
    version_git_dir = os.path.join(daplink_dir, "source", "daplink")
    error = generate_version_file(version_git_dir)
    if error:
        exit(-1)

# Build the project(s)
cores = get_core_count() if args.parallel else 1
projects = args.projects if len(args.projects) > 0 else project_list
generator = generate.Generator(PROJECTS_YAML)
for p_name in projects:
    p_valid = False
    for project in generator.generate(p_name):
        p_valid = True
        failed = False
        if hasattr(project, 'workspace_name') and (project.workspace_name is not None):
            logger.info("Generating %s for %s in workspace %s", toolchain, project.name, project.workspace_name)
        else:
            logger.info("Generating %s for %s", toolchain, project.name)
        if args.clean:
            if project.clean(toolchain) == -1:
                logger.error("Error cleaning project %s", project.name)
                failed = True
        if not failed and project.generate(toolchain) == -1:
            logger.error("Error generating project %s", project.name)
            failed = True
        if not failed:
            if project.build(toolchain, jobs=cores, verbose=(args.verbosity > 0), generator=args.generator) == -1:
                logger.error("Error building project %s", project.name)
                failed = True
        if failed and not args.ignore_failures:
            exit(-1)
    if not p_valid:
        exit(-1)

# Generate images with boardid / familyid for supported configurations
if args.release or args.supported:
    sys.path.append(os.path.join(daplink_dir, "test"))
    from info import SUPPORTED_CONFIGURATIONS, PROJECT_RELEASE_INFO, TEST_RELEASE_INFO
    id_map = {}
    for board_id, family_id, firmware, bootloader, target in SUPPORTED_CONFIGURATIONS:
        if firmware in id_map:
            id_map[firmware].append((hex(board_id), hex(family_id)))
        else:
            id_map[firmware] = [(hex(board_id), hex(family_id))]
    for project in projects:
        output = "projectfiles/%s/%s/build/%s" % (toolchain, project, project)
        hex = output + ".hex"
        crc = output + "_crc"

        if not os.path.exists(hex):
            # Build failed
            if args.ignore_failures:
                logger.warning("Missing file %s (build failed)" % (hex))
                continue
            else:
                logger.error("Missing file %s (build failed)" % (hex))
                exit(-1)

        # Do a build with board_id and family_id
        if project in id_map:
            for (boardid, familyid) in id_map[project]:
                logger.debug("Building image for %s (%s, %s)" % (project, boardid, familyid))
                post_build_script(hex, crc, boardid, familyid)

# Build release package
if args.release:
    release_version = 0
    with open(VERSION_YAML, 'r') as ver_yaml:
        try:
            verdict = yaml.safe_load(ver_yaml)
            release_version = int(verdict['common']['macros'][0].split('=')[1])
        except yaml.YAMLError as ex:
            print("Found yaml parse error", ex)

    release_dir = args.release_folder + "_%04i" % release_version
    if os.path.exists(release_dir):
        logger.info("Deleting: %s" % release_dir)
        shutil.rmtree(release_dir, ignore_errors=True)
    logger.info("Releasing directory: %s" % release_dir)

    release_info = PROJECT_RELEASE_INFO + (TEST_RELEASE_INFO if args.test else [])
    package_release_files("build", release_dir, release_version, "projectfiles/" + toolchain,
                          release_info, SUPPORTED_CONFIGURATIONS)
