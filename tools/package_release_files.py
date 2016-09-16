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

import os
import sys
import shutil
import argparse

self_path = os.path.abspath(__file__)
tools_dir = os.path.dirname(self_path)
daplink_dir = os.path.dirname(tools_dir)
test_dir = os.path.join(daplink_dir, "test")
sys.path.append(test_dir)

import info


def main():
    parser = argparse.ArgumentParser(description='Package a release for distribution')
    parser.add_argument('source', help='Release directory to grab files from')
    parser.add_argument('dest', help='Directory to create and place files in')
    parser.add_argument('version', type=int, help='Version number of this release')
    args = parser.parse_args()

    proj_dir = args.source
    output_dir = args.dest
    build_number = "%04i" % args.version

    os.mkdir(output_dir)
    for prj_name, legacy, offset, extension in info.PROJECT_RELEASE_INFO:
        legacy_str = "_legacy" if legacy else ""
        source_offset_str = "_0x%04x" % offset if legacy else ""
        source_name = prj_name + "_crc" + legacy_str + source_offset_str + "." + extension
        source_path = os.path.join(proj_dir, prj_name, source_name)
        items = prj_name.split('_')  # "k20dx_frdmk22f_if" -> ("k20dx", "frdmk22f", "if")
        assert items[-1] == "if", "Unexpected string: %s" % items[2]
        host_mcu = items[0]
        base_name = items[1]
        dest_offset_str = "_0x%04x" % offset
        dest_name = build_number + "_" + host_mcu + "_" + base_name + dest_offset_str + "." + extension
        dest_path = os.path.join(output_dir, dest_name)
        shutil.copyfile(source_path, dest_path)

if __name__ == "__main__":
    main()
