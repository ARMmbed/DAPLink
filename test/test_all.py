# CMSIS-DAP Interface Firmware
# Copyright (c) 2009-2013 ARM Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
"""
DAPLink validation and testing tool

optional arguments:
  -h, --help            show this help message and exit
  --user USER           MBED username (required for compile-api)
  --password PASSWORD   MBED password (required for compile-api)
  --project {k20dx_k64f_if,kl26z_nrf51822_if,lpc11u35_lpc1114_if,kl26z_microbit
_if,lpc11u35_lpc812_if,k20dx_k22f_if}
                        Project to test
  --nobuild             Skip build step. Binaries must have been built
                        already.
  --noloadif            Skip load step for interface.
  --noloadbl            Skip load step for bootloader.
  --notestendpt         Dont test the interface USB endpoints.
  --testfirst           If multiple boards of the same type are found only
                        test the first one.
  --verbose {Minimal,Normal,Verbose,All}
                        Verbose output

The purpose of this script is test the daplink projects
and verify as much of it is bug free as reasonable.
1. Build all or some of the projects
2. Load new interface code (bootloader in the future too)
3. Test filesystem and file contents
4. Test each USB endpoint (HID, MSD, CDC)


Example usages
------------------------

Test everything on all projects in the repository:
test_all.py --user <username> --password <password>

Test everything on a single project in the repository:
test_all.py --project <project> --testfirst --user <username>
    --password <password>

Quickly verify that the mass storage and the filesystem
have not regressed after creating a build using:
test_all.py --nobuild --project <project name>

Verify that the USB endpoints are working correctly on
an existing board with firmware already loaded:
test_all.py --nobuild --noloadif --user <username> --password <password>
"""
from __future__ import absolute_import
from __future__ import print_function

import os
import re
import time
import argparse
from hid_test import test_hid
from serial_test import test_serial
from msd_test import test_mass_storage
from daplink_board import get_all_attached_daplink_boards
from project_generator.generate import Generator
from test_info import TestInfo

TEST_REPO = 'https://developer.mbed.org/users/c1728p9/code/daplink-validation/'


def test_endpoints(board, parent_test):
    """Run tests to validate DAPLINK fimrware"""
    test_info = parent_test.create_subtest('test_endpoints')
    board.build_target_firmware(test_info)
    test_hid(board, parent_test)
    test_serial(board, test_info)
    test_mass_storage(board, test_info)


# Determine interface or bootloader - also check if name is valid
# Handle building project when requested
# Handle testing project when requested
class ProjectTester(object):

    _if_exp = re.compile("^([a-z0-9]+)_([a-z0-9_]+)_if$")
    _bl_exp = re.compile("^([a-z0-9]+)_bl$")
    _tool = 'uvision'
    _name_to_board_id = {
        'k20dx_k22f_if': 0x0231,
        'k20dx_k64f_if': 0x0240,
        'kl26z_microbit_if': 0x9900,
        'kl26z_nrf51822_if': 0x9900,
        'lpc11u35_lpc812_if': 0x1050,
        'lpc11u35_lpc1114_if': 0x1114,
        'lpc11u35_efm32gg_stk_if': 0x2015,
    }

    def __init__(self, yaml_prj, path='.'):
        self.prj = yaml_prj
        self._path = path
        self.name = yaml_prj.name
        if_match = self._if_exp.match(self.name)
        if if_match is not None:
            self._bl_name = if_match.group(1) + '_bl'
            self._is_bl = False
            self._board_id = self._name_to_board_id[self.name]
        bl_match = self._bl_exp.match(self.name)
        if bl_match is not None:
            self._is_bl = True
        if if_match is None and bl_match is None:
            raise Exception("Invalid project name %s" % self.name)
        self._built = False
        self._boards = None
        self._bl = None
        self._test_info = TestInfo(self.get_name())
        build_path = os.path.normpath(path + os.sep + 'projectfiles' + os.sep +
                                      self._tool + os.sep + self.name +
                                      os.sep + 'build')
        self._hex_file = os.path.normpath(build_path + os.sep +
                                          self.name + '.hex')
        self._bin_file = os.path.normpath(build_path + os.sep +
                                          self.name + '.bin')

        # By default test all configurations and boards
        self._only_test_first = False
        self._load_if = True
        self._load_bl = True
        self._test_if_bl = True
        self._test_ep = True

    def is_bl(self):
        return self._is_bl

    def get_name(self):
        return self.name

    def get_built(self):
        """
        Return true if the project has been built in the current session
        """
        return self._built

    def get_bl_name(self):
        """
        Get the name of the bootloader

        This function should only be called if the project
        is not a bootloader
        """
        assert not self.is_bl()
        return self._bl_name

    def set_bl(self, bl_prj):
        """
        Set the boot loader for this interface.

        Note - this function should only be called on
        an interface project.
        """
        assert isinstance(bl_prj, ProjectTester)
        assert not self._is_bl
        self._bl = bl_prj

    def get_binary(self):
        """
        Get the binary file associated with this project

        Returns None if the bin file has not been created yet.
        """
        return self._bin_file if os.path.isfile(self._bin_file) else None

    def get_hex(self):
        """
        Get the hex file associated with this project

        Returns None if the hex file has not been created yet.
        """
        return self._hex_file if os.path.isfile(self._hex_file) else None

    def get_board_id(self):
        """
        Get the board ID for this project

        Board ID is only valid if the target is not a bootloader
        """
        return self._board_id

    def set_test_boards(self, boards):
        assert type(boards) is list
        self._boards = boards

    def get_test_boards(self):
        return self._boards

    def get_test_info(self):
        return self._test_info

    def build(self, clean=True):
        self._test_info.info("Building %s" % self.name)
        start = time.time()
        #TODO - build bootloader if it isn't built yet
        #TODO - print info on bootloader
        ret = self.prj.generate(self._tool, False)
        self._test_info.info('Export return value %s' % ret)
        if ret != 0:
            raise Exception('Export return value %s' % ret)
        ret = self.prj.build(self._tool)
        self._test_info.info('Build return value %s' % ret)
        if ret != 0:
            raise Exception('Build return value %s' % ret)
        files = self.prj.get_generated_project_files(self._tool)
        export_path = files['path']
        base_file = os.path.normpath(export_path + os.sep + 'build' +
                                     os.sep + self.name)
        built_hex_file = base_file + '.hex'
        built_bin_file = base_file + '.bin'
        assert (os.path.abspath(self._hex_file) ==
                os.path.abspath(built_hex_file))
        assert (os.path.abspath(self._bin_file) ==
                os.path.abspath(built_bin_file))
        self._hex_file = built_hex_file
        self._bin_file = built_bin_file
        stop = time.time()
        self._test_info.info('Build took %s seconds' % (stop - start))
        self._built = True

    def test_set_first_board_only(self, first):
        assert type(first) is bool
        self._only_test_first = first

    def test_set_load_if(self, load):
        assert type(load) is bool
        self._load_if = load

    def test_set_load_bl(self, load):
        assert type(load) is bool
        self._load_bl = load

    def test_set_test_if_bl(self, run_test):
        assert type(run_test) is bool
        self._test_if_bl = run_test

    def test_set_test_ep(self, run_test):
        assert type(run_test) is bool
        self._test_ep = run_test

    def test(self):
        boards_to_test = self._boards
        if self._only_test_first:
            boards_to_test = self._boards[0:1]
        for board in boards_to_test:
            # Load interface & bootloader
            if self._load_if:
                print("Loading interface")
                board.load_interface(self.get_hex(), self._test_info)
                #TODO - check CRC

            if self._load_bl:
                pass
                #TODO - load bootloader
                #TODO - check CRC

            if self._test_if_bl:

                # Test bootloader

                # Test interface
                board.test_fs(self._test_info)
                board.test_fs_contents(self._test_info)

            # Test endpoint
            if self._test_ep:
                test_endpoints(board, self._test_info)

        return not self._test_info.get_failed()


def main():
    # Save current directory
    cur_dir = os.getcwd()
    parent_dir = os.path.dirname(cur_dir)
    os.chdir(parent_dir)

    # Wrap every project in a ProjectTester object
    # Tie every bootloader to one or more interface projects
    projects = list(Generator('projects.yaml').generate())
    yaml_dir = os.getcwd()
    all_projects = [ProjectTester(project, yaml_dir) for project in projects]
    if_project_list = [project for project in all_projects
                       if not project.is_bl()]
    bl_project_list = [project for project in all_projects
                       if project.is_bl()]
    bl_name_to_proj = {project.name: project for
                       project in bl_project_list}
    for project in if_project_list:
        bl_name = project.get_bl_name()
        if bl_name in bl_name_to_proj:
            project.set_bl(bl_name_to_proj[bl_name])
    #TODO - make sure all bootloaders are tied to an interface, make sure all
    # objects are accounted for

    # Create list of projects to show user
    prj_names = [prj.get_name() for prj in if_project_list]
    name_to_prj = {prj.get_name(): prj for prj in if_project_list}

    VERB_MINIMAL = 'Minimal'    # Just top level errors
    VERB_NORMAL = 'Normal'      # Top level errors and warnings
    VERB_VERBOSE = 'Verbose'    # All errors and warnings
    VERB_ALL = 'All'            # All errors
    VERB_LEVELS = [VERB_MINIMAL, VERB_NORMAL, VERB_VERBOSE, VERB_ALL]

    description = 'DAPLink validation and testing tool'
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('--user', type=str, default=None,
                        help='MBED username (required for compile-api)')
    parser.add_argument('--password', type=str, default=None,
                        help='MBED password (required for compile-api)')
    parser.add_argument('--project', help='Project to test', action='append',
                        choices=prj_names, default=[], required=False)
    parser.add_argument('--nobuild', help='Skip build step.  Binaries must have been built already.', default=False,
                        action='store_true')
    parser.add_argument('--noloadif', help='Skip load step for interface.',
                        default=False, action='store_true')
    parser.add_argument('--noloadbl', help='Skip load step for bootloader.',
                        default=False, action='store_true')
    parser.add_argument('--notestendpt', help='Dont test the interface USB endpoints.',
                        default=False, action='store_true')
    parser.add_argument('--testfirst', help='If multiple boards of the same type are found only test the first one.',
                        default=False, action='store_true')
    parser.add_argument('--verbose', help='Verbose output',
                        choices=VERB_LEVELS, default=VERB_NORMAL)
    args = parser.parse_args()

    # Validate args
    if not args.notestendpt:
        if args.user is None:
            print("'--user' must be specified when testing USB endpoints")
            exit(-1)
        if args.password is None:
            print("'--password' must be specified when testing USB endpoints")
            exit(-1)

    boards_explicitly_specified = len(args.project) != 0

    # Put together the list of projects to build
    if boards_explicitly_specified:
        projects_to_test = [name_to_prj[name] for name in args.project]
    else:
        projects_to_test = if_project_list

    # Collect attached mbed devices
    all_boards = get_all_attached_daplink_boards()

    # Attach firmware build credentials
    if not args.notestendpt:
        for board in all_boards:
            board.set_build_login(args.user, args.password)

    # Create table mapping each board id to boards
    board_id_to_board_list = {}
    for board in all_boards:
        board_id = board.get_board_id()
        if board_id not in board_id_to_board_list:
            board_id_to_board_list[board_id] = []
        board_id_to_board_list[board_id].append(board)

    # Attach each test board to a project
    for project in projects_to_test:
        board_id = project.get_board_id()
        if board_id in board_id_to_board_list:
            project.set_test_boards(board_id_to_board_list[board_id])

    # Fail if a board for the requested project is not attached
    if boards_explicitly_specified:
        for project in projects_to_test:
            if project.get_test_boards() is None:
                print("No test board(s) for project %s" % project.get_name())
                exit(-1)

    # Build all projects
    if not args.nobuild:
        for project in projects_to_test:
            project.build()

    # Test all projects with boards that are attached
    test_passed = True
    tested_projects = []
    untested_projects = []
    for project in projects_to_test:
        if project.get_test_boards() is not None:
            project.test_set_first_board_only(args.testfirst)
            project.test_set_load_if(not args.noloadif)
            project.test_set_load_bl(not args.noloadbl)
            project.test_set_test_ep(not args.notestendpt)
            test_passed &= project.test()
            tested_projects.append(project)
        else:
            # Cannot test board
            untested_projects.append(project)
    assert (len(tested_projects) + len(untested_projects) ==
            len(projects_to_test))
    if len(tested_projects) == 0:
        print("Test Failed - no connected boards to test")
        exit(-1)
    if boards_explicitly_specified:
        # Error should have been triggered before this
        # point if there were boards that couldn't be tested
        assert len(untested_projects) == 0

    # Print info for boards tested
    for project in tested_projects:
        print('')
        if args.verbose == VERB_MINIMAL:
            project.get_test_info().print_msg(TestInfo.FAILURE, 0)
        elif args.verbose == VERB_NORMAL:
            project.get_test_info().print_msg(TestInfo.WARNING, None)
        elif args.verbose == VERB_VERBOSE:
            project.get_test_info().print_msg(TestInfo.WARNING, None)
        elif args.verbose == VERB_ALL:
            project.get_test_info().print_msg(TestInfo.INFO, None)
        else:
            # This should never happen
            assert False

    # Warn about untested boards
    print('')
    for project in untested_projects:
        print('Warning - project %s is untested' % project.get_name())

    if test_passed:
        print("All boards passed")
        exit(0)
    else:
        print("Test Failed")
        exit(-1)


if __name__ == "__main__":
    main()
