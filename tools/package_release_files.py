#!/usr/bin/env python
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
import yaml
import subprocess
import zipfile
from collections import OrderedDict

class literal(str):
    pass

def literal_presenter(dumper, data):
    return dumper.represent_scalar('tag:yaml.org,2002:str', data, style='|')

yaml.add_representer(literal, literal_presenter)

def ordered_dict_presenter(dumper, data):
    return dumper.represent_dict(data.items())
yaml.add_representer(OrderedDict, ordered_dict_presenter)

instructions = {
    'default': OrderedDict(
        windows=literal("\n".join([
            '1. Download the firmware file.',
            '2. While holding down the boards reset button, connect the boards USB debug port to the computer. It should enumerate and mount as `BOOTLOADER` or `MAINTENANCE`. For boards that enumerate as `BOOTLOADER` [see our blog to determine if an update for the DAPLink bootloader is available.](https://os.mbed.com/blog/entry/DAPLink-bootloader-update/)',
            '3. Drag-and-drop the firmware file onto the mounted drive.',
            '4. Wait for the file copy operation to complete.',
            '5. Power cycle the board. It will now enumerate and mount as `DAPLINK` or the name of the board.'
        ])),
        osx=literal("\n".join([
            '1. Download the firmware file.',
            '2. While holding down the boards reset button, connect the boards USB debug port to the computer. It should enumerate as `BOOTLOADER` or `MAINTENANCE`. For boards that enumerate as `BOOTLOADER` [see our blog to determine if an update for the DAPLink bootloader is available.](https://os.mbed.com/blog/entry/DAPLink-bootloader-update/)',
            '3. In a terminal execute',
            '   - `sudo mount -u -w -o sync /Volumes/MAINTENANCE ; cp -X <path to firmware file> /Volumes/MAINTENANCE/`',
            '   - Note: If your drive does not mount as `MAINTENANCE` make sure to change this to match the name of the mounted disk attached to your system.',
            '4. Wait for the file copy operation to complete.',
            '5. Power cycle the board. It will now enumerate and mount as `DAPLINK` or the name of the board.'
        ])),
        linux=literal("\n".join([
            '1. Download the firmware file.',
            '2. While holding down the boards reset button, connect the boards USB debug port to the computer. It should enumerate as `BOOTLOADER` or `MAINTENANCE`. For boards that enumerate as `BOOTLOADER` [see our blog to determine if an update for the DAPLink bootloader is available.](https://os.mbed.com/blog/entry/DAPLink-bootloader-update/)',
            '3. In a terminal execute',
            '   - `cp <path to firmware file> <MAINTENANCE> && sync`',
            '   - Note: make sure to change `MAINTENANCE` to the name of the mount point of the drive on your system.',
            '4. Power cycle the board. It will now enumerate and mount as `DAPLINK` or the name of the board.'
        ]))
    ),
    'lpc11u35': {
        'windows': literal("\n".join([
            '1. Download the firmware file.',
            '2. While holding down the boards reset button, connect the boards USB debug port to the computer. It should enumerate and mount as `CRP DISABLD`',
            '3. Delete the file named `firmware.bin`, then drag and drop or copy the new bin file',
            '4. Wait for the file copy operation to complete.',
            '5. Power cycle the board. It will now enumerate and mount as `DAPLINK` or the name of the board.'
        ])),
        'osx': literal("\n".join([
            '1. Download the firmware file.',
            '2. While holding down the boards reset button, connect the boards USB debug port to the computer. It should enumerate and mount as `CRP DISABLD`',
            '3. Delete the file named `firmware.bin`',
            '4. In a terminal execute',
            '   - `sudo mount -u -w -o sync /Volumes/CRP\ DISABLD ; rm /Volumes/CRP\ DISABLD/firmware.bin && cp -X <path to firmware file> /Volumes/CRP\ DISABLD/`',
            '5. Wait for the file copy operation to complete.',
            '6. Power cycle the board. It will now enumerate and mount as `DAPLINK` or the name of the board.'
        ])),
        'linux': literal("\n".join([
            '1. Download the firmware file.',
            '2. While holding down the boards reset button, connect the boards USB debug port to the computer. It should enumerate and mount as `CRP DISABLD`',
            '3. Delete the file named `firmware.bin`',
            '4. In a terminal execute',
            '   - `cp <path to firmware file> <CRP DISABLD> && sync`',
            '   - Note: make sure to change `CRP DISABLD` to the name of the mount point on your system.',
            '5. Power cycle the board. It will now enumerate and mount as `DAPLINK` or the name of the board.'
        ]))
    }
}


def make_bin_zip(dir, name):
    working_dir = os.getcwd()
    os.chdir(dir)
    with zipfile.ZipFile(name, mode='w') as zipf:
        for file in os.listdir("./"):
            if file.endswith(".bin") or file.endswith(".hex"):
                zipf.write(os.path.join("./", file))
    #go back
    os.chdir(working_dir)

def package_release_files(source, dest, version, toolchain, release_info, supported_configurations):
    proj_dir = source
    output_dir = dest
    build_number = "%04i" % version

    update_yml_entries = {
        'default': OrderedDict(
            website='http://os.mbed.com/platforms',
            fw_version=build_number,
            image_format='.bin',
            instructions=instructions),
        'targets': {}
    }

    if os.path.exists(output_dir):
        shutil.rmtree(output_dir)
        print (output_dir + ' existed and deleted!!')

    os.mkdir(output_dir)

    for prj_name, legacy, offset, extension in release_info:
        legacy_str = "_legacy" if legacy else ""
        source_offset_str = "_0x%04x" % offset if legacy else ""
        source_name = prj_name + "_crc" + legacy_str + source_offset_str + "." + extension
        # The build path hierarchy is different between mbedcli and progen
        if toolchain.startswith("projectfiles"):
            source_dir_part = os.path.join(toolchain, prj_name, proj_dir)
        else:
            source_dir_part = os.path.join(proj_dir, prj_name, toolchain).upper()
        source_path = os.path.join(source_dir_part, source_name)
        if not os.path.isfile(source_path):
            print("Warning %s not added to release (expected file '%s')" % (prj_name, source_path))
            continue
        items = prj_name.split('_')  # "k20dx_frdmk22f_if" -> ("k20dx", "frdmk22f", "if")
        assert items[-1] == "if", "Unexpected string: %s" % items[2]
        host_mcu = items[0]
        base_name = '_'.join(items[1:-1])
        dest_offset_str = "_0x%04x" % offset
        dest_name = build_number + "_" + host_mcu + "_" + base_name + dest_offset_str + "." + extension
        dest_path = os.path.join(output_dir, dest_name)
        shutil.copyfile(source_path, dest_path)
        if extension == 'bin':
            shutil.copyfile(os.path.join(source_dir_part, prj_name + "_crc" + ".hex"),
                            os.path.join(output_dir, build_number + "_" + host_mcu + "_" + base_name + dest_offset_str + ".hex"))
        else:
            shutil.copyfile(os.path.join(source_dir_part, prj_name + "_crc" + legacy_str + source_offset_str + ".bin"),
                            os.path.join(output_dir, build_number + "_" + host_mcu + "_" + base_name + dest_offset_str + ".bin"))

        product_code = 'NOT SUPPORTED'
        for board_id, family_id, fimware, bootloader, target in supported_configurations:
            if fimware == prj_name:
                product_code = board_id
                if target is not None:
                    target_name = target
                else:
                    # Skip bare interface firmwares
                    if len(base_name) == 0:
                        continue
                    target_name = base_name.upper()

                fw_instuction = 'default'
                for fw_name_key in instructions:
                    if fw_name_key in dest_name.lower():
                        fw_instuction = fw_name_key
                        break

                if extension == 'bin':
                    update_yml_entries['targets'][target_name] = OrderedDict(
                            name=target_name,
                            product_code=format(product_code, '04x'),
                            fw_name=host_mcu + "_" + base_name + dest_offset_str,
                            instructions=fw_instuction
                    )

    make_bin_zip(output_dir, build_number + '_release_package_' + subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD']).decode().strip() + '.zip')
    print("Writing %s" % (os.path.join(output_dir, 'default.yml')))
    with open(os.path.join(output_dir, 'default.yml'), "w") as yml_file:
        yml_file.write("---\n")
        yml_file.write(yaml.dump(update_yml_entries['default']))
    print("Writing %s" % (os.path.join(output_dir, 'targets.yml')))
    with open(os.path.join(output_dir, 'targets.yml'), "w") as yml_file:
        yml_file.write("---\n")
        yml_file.write(yaml.dump(update_yml_entries['targets']))


if __name__ == "__main__":
    self_path = os.path.abspath(__file__)
    tools_dir = os.path.dirname(self_path)
    daplink_dir = os.path.dirname(tools_dir)
    test_dir = os.path.join(daplink_dir, "test")
    sys.path.append(test_dir)

    import info

    parser = argparse.ArgumentParser(description='Package a release for distribution')
    parser.add_argument('source', help='Release directory to grab files from')
    parser.add_argument('dest', help='Directory to create and place files in')
    parser.add_argument('version', type=int, help='Version number of this release')
    parser.add_argument('--toolchain', type=str, default='', help='Toolchain directory if present')
    parser.add_argument('--test', dest='test', action='store_true', help='Add test binaries to release')
    args = parser.parse_args()

    release_info = info.PROJECT_RELEASE_INFO + (info.TEST_RELEASE_INFO if args.test else [])
    package_release_files(args.source, args.dest, args.version, args.toolchain,
                          release_info, info.SUPPORTED_CONFIGURATIONS)