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

from __future__ import absolute_import
from enum import Enum
import os
import info
import re


class Firmware(object):
    """Class wrapping a firmware build"""

    class TYPE(Enum):
        BOOTLOADER = 1
        INTERFACE = 2

    def __str__(self):
        """A string that describes this firmware"""
        raise NotImplementedError()

    @property
    def name(self):
        """Name of this project"""
        raise NotImplementedError()

    @property
    def hic_id(self):
        """HIC ID for the type of board this firmware can run on"""
        raise NotImplementedError()

    @property
    def type(self):
        """Build type - either interface or bootloader"""
        raise NotImplementedError()

    @property
    def bin_path(self):
        """Path to the binary vesion of this firmware or None"""
        raise NotImplementedError()

    @property
    def hex_path(self):
        """Path to the hex vesion of this firmware or None"""
        raise NotImplementedError()

    @property
    def elf_path(self):
        """Path to the hex vesion of this firmware or None"""
        raise NotImplementedError()


class DAPLinkFirmware(Firmware):
    """Class to abstract access to a daplink firmware image"""

    _IF_RE = re.compile("^([a-z0-9]+)([_a-z0-9]*)_if$")
    _BL_RE = re.compile("^([a-z0-9]+)([_a-z0-9]*)_bl$")

    def __init__(self, name, bundle, directory):
        self._name = name
        self._bundle = bundle
        self._directory = directory
        self._valid = False

        # Set type
        self._type = None
        string_hic = None
        match = self._IF_RE.match(name)
        if match:
            string_hic = match.group(1)
            self._type = self.TYPE.INTERFACE
        match = self._BL_RE.match(name)
        if match:
            string_hic = match.group(1)
            self._type = self.TYPE.BOOTLOADER
        if self._type is None:
            assert False, 'Bad project name "%s"' % name

        # Set HIC
        assert string_hic in info.HIC_STRING_TO_ID, 'Unknown HIC "%s" must ' \
            'be added to HIC_STRING_TO_ID in info.py' % string_hic
        self._hic_id = info.HIC_STRING_TO_ID[string_hic]

        # Check firmware name and type
        assert self._type in self.TYPE, "Invalid type %s" % self._type
        if self._type is self.TYPE.INTERFACE:
            if name not in info.FIRMWARE_SET:
                print('Warning: board "%s" no entry in SUPPORTED_CONFIGURATIONS in info.py' % name)

        # Set file paths
        self._bin_path = self._directory + os.sep + '%s_crc.bin' % name
        self._hex_path = self._directory + os.sep + '%s_crc.hex' % name
        self._bin_path = os.path.abspath(self._bin_path)
        self._hex_path = os.path.abspath(self._hex_path)
        if not os.path.isfile(self._bin_path):
            return  # Failure
        if not os.path.isfile(self._hex_path):
            return  # Failure

        self._valid = True

    def __str__(self):
        return "Name=%s" % (self.name)

    @property
    def valid(self):
        """Set to True if the firmware is valid"""
        return self._valid

    @property
    def name(self):
        return self._name

    @property
    def hic_id(self):
        return self._hic_id

    @property
    def type(self):
        return self._type

    @property
    def bin_path(self):
        return self._bin_path

    @property
    def hex_path(self):
        return self._hex_path

    @property
    def elf_path(self):
        return self._elf_path


class ReleaseFirmware(Firmware):
    """Class to abstract access to a daplink firmware image"""

    _IF_RE = re.compile("^([a-z0-9]+)([_a-z0-9]*)_if$")
    _BL_RE = re.compile("^([a-z0-9]+)([_a-z0-9]*)_bl$")

    def __init__(self, file, bundle, path):
        name = re.sub(r'^[0-9]+_(.*?)[_]+0x[0-9a-f]+[.][a-z]+$', r'\1', file, flags=re.IGNORECASE) + "_if"
        # print(name)

        self._name = name
        self._bundle = bundle
        self._directory = os.path.dirname(path)
        self._valid = False
        # Set type
        self._type = None
        string_hic = None
        match = self._IF_RE.match(name)
        if match:
            string_hic = match.group(1)
            self._type = self.TYPE.INTERFACE
        if self._type is None:
            assert False, 'Bad project name "%s"' % name

        # Set HIC
        assert string_hic in info.HIC_STRING_TO_ID, 'Unknown HIC "%s" must ' \
            'be added to HIC_STRING_TO_ID in info.py' % string_hic
        self._hic_id = info.HIC_STRING_TO_ID[string_hic]

        # Check firmware name and type
        assert self._type in self.TYPE, "Invalid type %s" % self._type
        if self._type is self.TYPE.INTERFACE:
            if name not in info.FIRMWARE_SET:
                print('Warning: board "%s" no entry in SUPPORTED_CONFIGURATIONS in info.py' % name)

        # Set file paths
        self._hex_path = os.path.abspath(path)
        if not os.path.isfile(self._hex_path):
            return  # Failure
        bin_path = re.sub(r'[.]hex', r'.bin', self._hex_path)
        self._bin_path = bin_path if os.path.isfile(bin_path) else None
        self._valid = True

    def __str__(self):
        return "Name=%s" % (self.name)

    @property
    def valid(self):
        """Set to True if the firmware is valid"""
        return self._valid

    @property
    def name(self):
        return self._name

    @property
    def hic_id(self):
        return self._hic_id

    @property
    def type(self):
        return self._type

    @property
    def bin_path(self):
        return self._bin_path

    @property
    def hex_path(self):
        return self._hex_path

    @property
    def elf_path(self):
        return self._elf_path
