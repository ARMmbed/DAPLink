#! /usr/bin/env python

# CMSIS-DAP Interface Firmware
# Copyright (c) 2009-2014 ARM Limited
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

from __future__ import print_function
import sys
import os
import argparse
import subprocess

import settings

BOOTLOADER_PROJECTS = [
        {
            'target' : 'k20dx128',
            'path' : r'bootloader\mdk\k20dx128\k20dx128_bootloader.uvproj',
            'targets' : [
                            'k20dx128_bootloader'
                        ]
        },
        {
            'target' : 'lpc11u35',
            'path' : r'bootloader\mdk\lpc11u35\lpc11u35_bootloader.uvproj',
            'targets' : [
                            'lpc11u35_bootloader'
                        ]
        }
    ]

INTERFACE_PROJECTS = [
        {
            'target' : 'k20dx128',
            'path' : r'interface\mdk\k20dx128\k20dx128_interface.uvproj',
            'targets' : [
                            'k20dx128_kl25z_if',
                            'k20dx128_kl25z_openSDA_bootloader_if',
                            'k20dx128_kl25z_mbed_bootloader_if',
                            'k20dx128_twr_kl25z_if',
                            'k20dx128_twr_kl25z_openSDA_bootloader_if',
                            'k20dx128_twr_kl25z_mbed_bootloader_if',
                            'k20dx128_k20dx_if',
                            'k20dx128_k20dx_openSDA_bootloader_if',
                            'k20dx128_k20dx_mbed_bootloader_if',
                            'k20dx128_kl05z_if',
                            'k20dx128_kl05z_openSDA_bootloader_if',
                            'k20dx128_kl05z_mbed_bootloader_if',
                            'k20dx128_kl46z_if',
                            'k20dx128_kl46z_openSDA_bootloader_if',
                            'k20dx128_kl46z_mbed_bootloader_if',
                            'k20dx128_k64f_if',
                            'k20dx128_k64f_openSDA_bootloader_if',
                            'k20dx128_k64f_mbed_bootloader_if',
                            'k20dx128_kl02z_if',
                            'k20dx128_kl02z_openSDA_bootloader_if',
                            'k20dx128_kl02z_mbed_bootloader_if',
                            'k20dx128_kl26z_if',
                            'k20dx128_kl26z_openSDA_bootloader_if',
                            'k20dx128_kl26z_mbed_bootloader_if',
                        ],
        },
        {
            'target' : 'lpc11u35',
            'path' : r'interface\mdk\lpc11u35\lpc11u35_interface.uvproj',
            'targets' : [
                            'lpc11u35_lpc812_if',
                            'lpc11u35_lpc812_mbed_bootloader',
                            'lpc11u35_lpc1768_if',
                            'lpc11u35_lpc1768_mbed_bootloader',
                            'lpc11u35_ublox_lpc1768_if',
                            'lpc11u35_lpc1114_if',
                            'lpc11u35_lpc1114_mbed_bootloader',
                            #'lpc11u35_lpc810_if' # fails build
                         ],
        },
        {
            'target' : 'lpc4322',
            'path' : r'interface\mdk\lpc4322\lpc4322_interface.uvproj',
            'targets' : [
                            'lpc4322_lpc1549_if',
                            'lpc4322_lpc1549_dbg_sram',
                            'lpc4322_lpc11U68_if',
                            'lpc4322_lpc11U68_dbg_sram',
                            'lpc4322_lpc4337_if',
                         ],
        }
    ]

FLASH_ALGO_PROJECTS = [
        {
            'target' : 'MKXXX',
            'path' : r'interface\flash_algo_mdk\MKXXX\MKXX.uvproj',
            'targets' : [
                            'MK20DX128_Pflash',
                            'MK64FN1M0_Pflash',
                            'MKL02Z32_Pflash',
                            'MKL05Z32_Pflash',
                            'MKL25Z128_Pflash',
                            'MKL26Z128_Pflash',
                            'MKL46Z256_Pflash',
                            'MK22F51212_Pflash'
                        ]
        },
        {
            'target' : 'LPC_IAP',
            'path' : r'interface\flash_algo_mdk\LPC_IAP\LPC_IAP.uvproj',
            'targets' : [
                            'LPC1700_IAP_512',
                            'LPC1700_IAP_256',
                            'LPC1700_IAP_128',
                            'LPC1700_IAP_64',
                            'LPC1700_IAP_32',
                            'LPC1700_IAP_512_MBED_60MHz',
                            #'LPC11xx_IAP_32',
                            'LPC8xx_IAP_4',
                            'LPC1549_IAP_256',
                            'LPC11U68_IAP_256',
                            'LPC4337_IAP_1024'
                        ]
        },
        {
            'target' : 'LPC_SPIFI',
            'path' : r'interface\flash_algo_mdk\LPC_SPIFI\LPC_SPIFI.uvproj',
            'targets' : [
                            'LPC1800_SPIFI_8M_4K',
                            'LPC1800_SPIFI_4M_4K',
                            'LPC1800_SPIFI_4M_64K',
                        ]
        }
    ]


class BuildError(Exception): pass

##
# @brief Class to build uVision projects.
class UV4Project(object):
    # Status codes from building a project.

    ## No warnings or errors.
    SUCCESS = 0
    ## Warnings only.
    WARNINGS = 1
    ## Fatal errors and possibly warnings.
    ERRORS = 2
    ## The request target does not exist.
    INVALID_TARGET = 3
    ## The project file does not exit.
    INVALID_PROJECT = 15

    ##
    # @brief Constructor.
    # @param self
    # @param project Path to the project file.
    def __init__(self, project):
        self.project = project

    ##
    # @brief Build a target of the project.
    #
    # @param self
    # @param target Name of the desired target to build. If not specified, or set to None, the
    #   currently selected target (in the GUI) will be built.
    # @param logFile Path to a file that the build log will be written to. The path is relative
    #   to the project file's directory. The log file will be created if it doesn't exist, and
    #   it will be overwritten if it does already exist.
    #
    # @return The integer status code from the uVision build.
    def build(self, target=None, logFile=None):
        # Build list of arguments to UV4.
        argList = [settings.UV4, '-j0', '-b', self.project]
        if target:
            argList += ['-t', target]
        if logFile:
            argList += ['-o', logFile]

        # Run UV4 command.
        return subprocess.call(argList)

##
# @brief Command line interface to UV4 builder.
class Builder(object):
    def __init__(self):
        self.rootPath = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    def _read_options(self):
        # Build arg parser.
        parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter)

        parser.add_argument("-b", "--bootloader", action="store_true", help="Build bootloader projects.")
        parser.add_argument("-i", "--interface", action="store_true", help="Build interface projects.")
        parser.add_argument("-f", "--flash", action="store_true", help="Build flash algo projects.")
        parser.add_argument("-t", "--target", metavar="TARGET", default=None, help="Specify the target to build.")
        parser.add_argument("-l", "--log", metavar="PATH", help="Specify the log file.")

        return parser.parse_args()

    def run(self):
        # Read command line arguments.
        self.args = self._read_options()

        # Build all projects if no type was specified.
        if (not self.args.bootloader) and (not self.args.interface) and (not self.args.flash):
            self.args.bootloader = True
            self.args.interface = True
            self.args.flash = True

        try:
            if self.args.bootloader:
                self._build_project_list(BOOTLOADER_PROJECTS)
            if self.args.interface:
                self._build_project_list(INTERFACE_PROJECTS)
            if self.args.flash:
                self._build_project_list(FLASH_ALGO_PROJECTS)
        except BuildError:
            return 1
        else:
            return 0

    def _build_project_list(self, projects):
        for targetDict in projects:
            # Skip this target if it shouldn't be built.
            if self.args.target and (not targetDict['target'].lower().startswith(self.args.target.lower())):
                continue

            # Construct project path and name.
            projectPath = os.path.join(self.rootPath, targetDict['path'])
            projectName = os.path.basename(projectPath)

            # Create the project file object.
            project = UV4Project(projectPath)

            # Build all targets listed for this project.
            for targetName in targetDict['targets']:
                print("Building target %s of %s..." % (targetName, projectName))

                status = project.build(targetName, self.args.log)
                print("Status = %d" % status)

                if status != UV4Project.SUCCESS and status != UV4Project.WARNINGS:
                    print("* Error building target %s of %s" % (targetName, projectName))
                    raise BuildError


if __name__ == "__main__":
    exit(Builder().run())

