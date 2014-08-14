"""
CMSIS-DAP Interface Firmware
Copyright (c) 2009-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
from optparse import OptionParser

INTERFACES = ['LPC11U35',
              'K20DX128',
              'LPC4322']

TARGETS = ['LPC812',
           'LPC11U68',
           'LPC1549',
           'LPC1768',
           'KL25Z',
           'K64F',
           'K22F',
           'K24F256']


def options_parser():
    parser = OptionParser()

    parser.add_option("-i", "--interface", default=INTERFACES[0],
                  help="mbed interface (%s)" % ', '.join(INTERFACES),
                  metavar="INTERFACE")

    parser.add_option("-t", "--target", default=TARGETS[0],
                  help="mbed  (%s)" % ', '.join(TARGETS),
                  metavar="TARGET")

    return parser


def get_options():
    (options, args) = options_parser().parse_args()
    return options
