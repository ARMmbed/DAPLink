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

# Name of all projects ready for public release
# and info on the file to be distributed
PROJECT_RELEASE_INFO = {
    # Project Name                                  Legacy      Offset      Extension
    ("k20dx_frdmk22f_if",                           True,       0x8000,     "bin"       ),
    ("k20dx_frdmk64f_if",                           True,       0x5000,     "bin"       ),
    ("kl26z_microbit_if",                           False,      0x8000,     "hex"       ),
    ("lpc11u35_lpc812xpresso_if",                   False,      0x0000,     "bin"       ),
    ("lpc11u35_lpc824xpresso_if",                   False,      0x0000,     "bin"       ),
    ("lpc11u35_ssci1114_if",                        False,      0x0000,     "bin"       ),
    ("sam3u2c_mkit_dk_dongle_nrf5x_if",             True,       0x5000,     "bin"       ),
    ("k20dx_frdmk20dx_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkw24f_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl02z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl05z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl25z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl26z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl46z_if",                          True,       0x8000,     "bin"       ),
    ("lpc11u35_archble_if",                         False,      0x0000,     "bin"       ),
    ("lpc11u35_archpro_if",                         False,      0x0000,     "bin"       ),
    #("lpc11u35_archmax_if",                        False,      0x0000,     "bin"       ),  # Unsupported currently
    ("lpc11u35_hrm1017_if",                         False,      0x0000,     "bin"       ),
    ("lpc11u35_sscity_if",                          False,      0x0000,     "bin"       ),
    ("lpc11u35_ssci824_if",                         False,      0x0000,     "bin"       ),
    ("k20dx_rbl_if",                                True,       0x5000,     "bin"       ),
    ("k20dx_rblnano_if",                            True,       0x5000,     "bin"       ),
    ("lpc11u35_archlink_if",                        False,      0x0000,     "bin"       ),
    ("lpc11u35_tiny_if",                            False,      0x0000,     "bin"       ),
    #("lpc11u35_c027_if",                           False,      0x0000,     "bin"       ),  # Unsupported currently
    ("lpc11u35_lpc4088dm_if",                       False,      0x0000,     "bin"       ),
    ("lpc11u35_lpc4088qsb_if",                      False,      0x0000,     "bin"       ),
    ('lpc11u35_ssci_chibi_if',                      False,      0x0000,     "bin"       ),
    ("k20dx_xdot_l151_if",                          False,      0x8000,     "bin"       ),
    ('lpc11u35_6lowpan_borderrouterhat_if',         None,       0x0000,     "bin"       ),
    ('lpc11u35_6lowpan_borderrouterusb_if',         None,       0x0000,     "bin"       ),
    ('lpc11u35_6lowpan_borderrouterethernet_if',    None,       0x0000,     "bin"       ),
}

# All supported configurations
SUPPORTED_CONFIGURATIONS = [
    #   Board ID    Firmware                                    Bootloader          Target
    (   0x200,      'k20dx_frdmkl25z_if',                       'k20dx_bl',         'KL25Z'                                 ),
    (   0x210,      'k20dx_frdmkl05z_if',                       'k20dx_bl',         'FRDM-KL05Z'                            ),
    (   0x220,      'k20dx_frdmkl46z_if',                       'k20dx_bl',         'FRDM-KL46Z'                            ),
    (   0x230,      'k20dx_frdmk20dx_if',                       'k20dx_bl',         'FRDM-K20D50M'                          ),
    (   0x231,      'k20dx_frdmk22f_if',                        'k20dx_bl',         'FRDM-K22F'                             ),
    (   0x240,      'k20dx_frdmk64f_if',                        'k20dx_bl',         'FRDM-K64F'                             ),
    (   0x250,      'k20dx_frdmkl02z_if',                       'k20dx_bl',         None                                    ),
    (   0x260,      'k20dx_frdmkl26z_if',                       'k20dx_bl',         None                                    ),
    (   0x280,      'k20dx_frdmkw24f_if',                       'k20dx_bl',         None                                    ),
    (   0x350,      'k20dx_xdot_l151_if',                       'k20dx_bl',         None                                    ),  # TODO - set target to 'MTS-xDot-L151CC' when mbed-os supports this
    (   0x824,      'lpc11u35_lpc824xpresso_if',                None,               'LPCXpresso824-MAX'                     ),
    (   0x1017,     'lpc11u35_hrm1017_if',                      None,               'mbed-HRM1017'                          ),
    (   0x1018,     'lpc11u35_ssci824_if',                      None,               'Switch-Science-mbed-LPC824'            ),
    (   0x1019,     'lpc11u35_sscity_if',                       None,               'Switch-Science-mbed-TY51822r3'         ),
    (   0x1050,     'lpc11u35_lpc812xpresso_if',                None,               'NXP-LPC800-MAX'                        ),
    (   0x1060,     'lpc11u35_lpc4088qsb_if',                   None,               'EA-LPC4088'                            ),
    (   0x1062,     'lpc11u35_lpc4088dm_if',                    None,               'EA-LPC4088-Display-Module'             ),
    (   0x1070,     'sam3u2c_mkit_dk_dongle_nrf5x_if',          'sam3u2c_bl',       'Nordic-nRF51822'                       ),
    (   0x1090,     'k20dx_rbl_if',                             'k20dx_bl',         'RedBearLab-nRF51822'                   ),
    (   0x1095,     'k20dx_rblnano_if',                         'k20dx_bl',         'RedBearLab-BLE-Nano'                   ),
    (   0x1100,     'sam3u2c_mkit_dk_dongle_nrf5x_if',          'sam3u2c_bl',       'Nordic-nRF51-DK'                       ),
    (   0x1101,     'sam3u2c_mkit_dk_dongle_nrf5x_if',          'sam3u2c_bl',       'Nordic-nRF52-DK'                       ),
    (   0x1021,     'lpc11u35_ssci_chibi_if',                   None,               None                                    ), # TODO - set target to 'SSCI-MBIT' when mbed-os supports this
    (   0x1114,     'lpc11u35_ssci1114_if',                     None,               'LPC1114FN28'                           ),
    (   0x1120,     'sam3u2c_mkit_dk_dongle_nrf5x_if',          'sam3u2c_bl',       'Nordic-nRF51-Dongle'                   ),
    (   0x1234,     'lpc11u35_c027_if',                         None,               'u-blox-C027'                           ),
    (   0x5050,     'lpc11u35_arm_watch_stm32f411_if',          None,               None                                    ),
    (   0x5051,     'lpc11u35_arm_watch_efm32_if',              None,               None                                    ),
    (   0x5052,     'lpc11u35_arm_watch_nrf51_if',              None,               None                                    ),
    (   0x9004,     'lpc11u35_archpro_if',                      None,               'Seeeduino-Arch-Pro'                    ),
    (   0x9009,     'lpc11u35_archble_if',                      None,               'Seeed-Arch-BLE'                        ),
    (   0x9011,     'lpc11u35_archmax_if',                      None,               'Seeed-Arch-Max'                        ),
    (   0x9012,     'lpc11u35_tiny_if',                         None,               'Seeed-Tiny-BLE'                        ),
    (   0x9013,     'lpc11u35_archlink_if',                     None,               'Seeed-Arch-Link'                       ),
    (   0x9900,     'kl26z_microbit_if',                        'kl26z_bl',         'Microbit'                              ),
    (   0x7402,     'lpc11u35_6lowpan_borderrouterhat_if',      None,               'mbed-6LoWPAN-Border-Router-HAT'        ),
    (   0x7403,     'lpc11u35_6lowpan_borderrouterusb_if',      None,               'mbed-6LoWPAN-Border-Router-USB'        ),
    (   0x7404,     'lpc11u35_6lowpan_borderrouterethernet_if', None,               'mbed-6LoWPAN-Border-Router-ETHERNET'   ),
]

# Add new HICs here
HIC_STRING_TO_ID = {
    'k20dx': 0x97969900,
    'kl26z': 0x97969901,
    'lpc11u35': 0x97969902,
    'sam3u2c': 0x97969903,
}

BOARD_ID_LOCKED_WHEN_ERASED = set([
    0x0231,  # K22F
    0x0240,  # K64F
    0x0230,  # K20DX
    0x0200,  # KL25Z
    0x0220,  # KL46Z
    0x0210,  # KL05Z
])

#Hack until these targets have an image with a valid vector table
TARGET_WITH_BAD_VECTOR_TABLE_LIST = [
    'Microbit',
    'Nordic-nRF51-DK',
    'Nordic-nRF52-DK',
    'Nordic-nRF51822',
    'Nordic-nRF51-Dongle',
    'Seeed-Arch-BLE',
    'mbed-HRM1017',
    'Switch-Science-mbed-TY51822r3',
    'RedBearLab-nRF51822',
    'RedBearLab-BLE-Nano',
    'Seeed-Tiny-BLE',
    'Seeed-Arch-Link',
    'SSCI-MBIT',
]

BOARD_ID_TO_BUILD_TARGET = {config[0]: config[3] for config in
                            SUPPORTED_CONFIGURATIONS}
FIRMWARE_SET = set((config[1] for config in SUPPORTED_CONFIGURATIONS))
TARGET_SET = set((target[3] for target in SUPPORTED_CONFIGURATIONS if
                  target[3] is not None))

TARGET_WITH_COMPILE_API_LIST = [config[3] for config in
                                SUPPORTED_CONFIGURATIONS if
                                config[3] is not None]
