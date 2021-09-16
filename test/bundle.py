#
# DAPLink Interface Firmware
# Copyright (c) 2009-2021, Arm Limited, All Rights Reserved
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

import os
from firmware import DAPLinkFirmware, ReleaseFirmware


def load_bundle_from_release(directory):
    """ Return a bundle representing the given build"""
    return ReleaseFirmwareBundle(directory)


def load_bundle_from_project(tool='uvision'):
    """
    Return a bundle for the given tool

    Note - This does not build the project.  It only returns the
    firmware that has already been built.
    """
    progen_toolchains = ['make_gcc_arm', 'make_armclang', 'make_armcc',
                         'cmake_gcc_arm', 'cmake_armclang', 'cmake_armcc']
    assert (tool in progen_toolchains or tool == 'uvision' or tool == 'mbedcli'), 'Input tool %s is not supported' % (tool)

    self_path = os.path.abspath(__file__)
    test_dir = os.path.dirname(self_path)
    daplink_dir = os.path.dirname(test_dir)
    assert os.path.basename(test_dir) == 'test', 'The script "%s" must be ' \
        'located in the "test" directory of daplink to work correctly.'

    if tool in progen_toolchains or tool == 'uvision':
        project_dir = os.path.join(daplink_dir, 'projectfiles', tool)
        build_folder = 'build'
    elif tool == 'mbedcli':
        project_dir = os.path.join(daplink_dir, 'BUILD')
        build_folder = 'ARM-CUSTOM_PROFILE'

    return ProjectFirmwareBundle(project_dir, build_folder)


class FirmwareBundle(object):

    def get_firmware_list(self):
        """Return the firmware objects associated with this bundle"""
        raise NotImplementedError()

    @property
    def build_sha(self):
        """The GIT SHA this build was created at as a string or None"""
        raise NotImplementedError()

    @property
    def build_local_mods(self):
        """True if this was a clean build, False otherwise"""
        raise NotImplementedError()


class ReleaseFirmwareBundle(FirmwareBundle):
    """Class to abstract access a formal build as a bundle"""

    def __init__(self, directory):
        bundle_contents = os.listdir(directory)
        firmware_list = []
        for name in bundle_contents:
            path = directory + os.sep + name
            if os.path.isdir(path):
                daplink_firmware = DAPLinkFirmware(name, self, path)
                if daplink_firmware.valid:
                    firmware_list.append(daplink_firmware)
            elif os.path.isfile(path):
                if ('.hex' in name):
                    daplink_firmware = ReleaseFirmware(name, self, path)
                    if daplink_firmware.valid:
                        firmware_list.append(daplink_firmware)
            else:
                assert False
        self._firmware_list = firmware_list

    def get_firmware_list(self):
        return self._firmware_list

    @property
    def build_sha(self):
        raise NotImplementedError()

    @property
    def build_local_mods(self):
        raise NotImplementedError()


class ProjectFirmwareBundle(FirmwareBundle):
    """Class to abstract access to daplink's build directory as a bundle"""

    def __init__(self, project_dir, build_folder):

        if not os.path.exists(project_dir):
            print("Error: DAPLink project folder %s missing" % project_dir)
            exit(-1)

        project_dir_list = os.listdir(project_dir)
        if not project_dir_list:
            print("Error: DAPLink projects not build yet at %s" % project_dir)
            exit(-1)

        firmware_list = []
        for name in project_dir_list:
            build_dir = os.path.join(project_dir, name, build_folder)
            if os.path.isdir(build_dir):
                daplink_firmware = DAPLinkFirmware(name.lower(), self, build_dir)
                if daplink_firmware.valid:
                    firmware_list.append(daplink_firmware)
        self._firmware_list = firmware_list

    def get_firmware_list(self):
        return self._firmware_list

    @property
    def build_sha(self):
        raise NotImplementedError()

    @property
    def build_local_mods(self):
        raise NotImplementedError()
