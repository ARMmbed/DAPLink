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
import shutil
import argparse

# Name of all projects ready for public release
# and info on the file to be distributed
PROJECT_RELEASE_INFO = {
    # Project Name                          Legacy      Offset      Extension
    ("k20dx_frdmk22f_if",                   True,       0x8000,     "bin"       ),
    ("k20dx_frdmk64f_if",                   True,       0x5000,     "bin"       ),
    ("kl26z_microbit_if",                   False,      0x8000,     "hex"       ),
    ("lpc11u35_lpc812xpresso_if",           False,      0x0000,     "bin"       ),
    ("lpc11u35_lpc824xpresso_if",           False,      0x0000,     "bin"       ),
    ("lpc11u35_ssci1114_if",                False,      0x0000,     "bin"       ),
    ("lpc11u35_efm32gg_stk_if",             False,      0x0000,     "bin"       ),
    ("sam3u2c_mkit_dk_dongle_nrf5x_if",     True,       0x5000,     "bin"       ),
    ("k20dx_frdmk20dx_if",                  True,       0x8000,     "bin"       ),
    ("k20dx_frdmkw24f_if",                  True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl02z_if",                  True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl05z_if",                  True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl25z_if",                  True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl26z_if",                  True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl46z_if",                  True,       0x8000,     "bin"       ),
    ("lpc11u35_archble_if",                 False,      0x0000,     "bin"       ),
    ("lpc11u35_archpro_if",                 False,      0x0000,     "bin"       ),
    #("lpc11u35_archmax_if",                False,      0x0000,     "bin"       ),  # Unsupported currently
    ("lpc11u35_hrm1017_if",                 False,      0x0000,     "bin"       ),
    ("lpc11u35_sscity_if",                  False,      0x0000,     "bin"       ),
    ("lpc11u35_ssci824_if",                 False,      0x0000,     "bin"       ),
    ("k20dx_rbl_if",                        True,       0x5000,     "bin"       ),
    ("k20dx_rblnano_if",                    True,       0x5000,     "bin"       ),
    ("lpc11u35_archlink_if",                False,      0x0000,     "bin"       ),
    ("lpc11u35_tiny_if",                    False,      0x0000,     "bin"       ),
    #("lpc11u35_c027_if",                   False,      0x0000      "bin"       ),  # Unsupported currently
}

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
    for prj_name, legacy, offset, extension in PROJECT_RELEASE_INFO:
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
