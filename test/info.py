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

# Add new HICs here
HIC_STRING_TO_ID = {
    'k20dx': 0x97969900,
    'kl26z': 0x97969901,
    'lpc11u35': 0x97969902,
    'sam3u2c': 0x97969903,
}

<<<<<<< HEAD
# Add new firmware with a hard coded board ID here
FIRMWARE_NAME_TO_BOARD_ID = {
    'k20dx_frdmk22f_if': 0x0231,
    'k20dx_frdmk64f_if': 0x0240,
    'kl26z_microbit_if': 0x9900,
    'kl26z_nrf51822_if': 0x9900,
    'lpc11u35_lpc812xpresso_if': 0x1050,
    'lpc11u35_ssci1114_if': 0x1114,
    'lpc11u35_efm32gg_stk_if': 0x2015,
    'sam3u2c_nrf51dk_if': 0x1100,
    'k20dx_frdmk20dx_if': 0x0230,
    'k20dx_frdmkw24f_if': 0x0280,
    'k20dx_frdmkl02z_if': 0x0250,
    'k20dx_frdmkl05z_if': 0x0210,
    'k20dx_frdmkl25z_if': 0x0200,
    'k20dx_frdmkl26z_if': 0x0260,
    'k20dx_frdmkl46z_if': 0x0220,
    'sam3u2c_nrf51mkit_if': 0x1070,
    'sam3u2c_nrf51dongle_if': 0x1120,
    'lpc11u35_archble_if': 0x9009,
    'lpc11u35_archpro_if': 0x9004,
    'lpc11u35_archmax_if': 0x9011,
    'lpc11u35_hrm1017_if': 0x1017,
    'lpc11u35_sscity_if': 0x1019,
    'lpc11u35_ssci824_if': 0x1018,
    'lpc11u35_lpc824xpresso_if': 0x0824,
    'k20dx_rbl_if': 0x1090,
    'k20dx_rblnano_if': 0x1095,
    'lpc11u35_tiny_if': 0x9012,
    'lpc11u35_c027_if': 0x1234,
    'lpc11u35_arm_watch_stm32f411_if': 0x5050,
    'lpc11u35_arm_watch_efm32_if': 0x5051,
    'lpc11u35_arm_watch_nrf51_if': 0x5052,
    'lpc11u35_archlink_if': 0x9013,
    'lpc11u35_6LoWPAN_BorderRouterHAT_if': 0x7402
    'lpc11u35_6LoWPAN_BorderRouterUSB_if': 0x7403
    'lpc11u35_6LoWPAN_BorderRouterPOE_if': 0x7404
}
=======
>>>>>>> 4864068bf12868869d83ae48ccbbffba1602d8c3

SUPPORTED_CONFIGURATIONS = [
    #   Board ID    Firmware                                    Bootloader          Target
    (   0x200,      'k20dx_frdmkl25z_if',                       'k20dx_bl',         'KL25Z'                         ),
    (   0x210,      'k20dx_frdmkl05z_if',                       'k20dx_bl',         'FRDM-KL05Z'                    ),
    (   0x220,      'k20dx_frdmkl46z_if',                       'k20dx_bl',         'FRDM-KL46Z'                    ),
    (   0x230,      'k20dx_frdmk20dx_if',                       'k20dx_bl',         'FRDM-K20D50M'                  ),
    (   0x231,      'k20dx_frdmk22f_if',                        'k20dx_bl',         'FRDM-K22F'                     ),
    (   0x240,      'k20dx_frdmk64f_if',                        'k20dx_bl',         'FRDM-K64F'                     ),
    (   0x250,      'k20dx_frdmkl02z_if',                       'k20dx_bl',         None                            ),
    (   0x260,      'k20dx_frdmkl26z_if',                       'k20dx_bl',         None                            ),
    (   0x280,      'k20dx_frdmkw24f_if',                       'k20dx_bl',         None                            ),
    (   0x824,      'lpc11u35_lpc824xpresso_if',                None,               'LPCXpresso824-MAX'             ),
    (   0x1017,     'lpc11u35_hrm1017_if',                      None,               'mbed-HRM1017'                  ),
    (   0x1018,     'lpc11u35_ssci824_if',                      None,               'Switch-Science-mbed-LPC824'    ),
    (   0x1019,     'lpc11u35_sscity_if',                       None,               'Switch-Science-mbed-TY51822r3' ),
    (   0x1050,     'lpc11u35_lpc812xpresso_if',                None,               'NXP-LPC800-MAX'                ),
    (   0x1060,     'lpc11u35_lpc4088qsb_if',                   None,               'EA-LPC4088'                    ),
    (   0x1062,     'lpc11u35_lpc4088dm_if',                    None,               'EA-LPC4088-Display-Module'     ),
    (   0x1070,     'sam3u2c_nrf51mkit_if',                     'sam3u2c_bl',       'Nordic-nRF51822'               ),
    (   0x1090,     'k20dx_rbl_if',                             'k20dx_bl',         'RedBearLab-nRF51822'           ),
    (   0x1095,     'k20dx_rblnano_if',                         'k20dx_bl',         'RedBearLab-BLE-Nano'           ),
    (   0x1100,     'sam3u2c_nrf51dk_if',                       'sam3u2c_bl',       'Nordic-nRF51-DK'               ),
    (   0x1114,     'lpc11u35_ssci1114_if',                     None,               'LPC1114FN28'                   ),
    (   0x1120,     'sam3u2c_nrf51dongle_if',                   'sam3u2c_bl',       'Nordic-nRF51-Dongle'           ),
    (   0x1234,     'lpc11u35_c027_if',                         None,               'u-blox-C027'                   ),
    (   0x2015,     'lpc11u35_efm32gg_stk_if',                  None,               None                            ),
    (   0x5050,     'lpc11u35_arm_watch_stm32f411_if',          None,               None                            ),
    (   0x5051,     'lpc11u35_arm_watch_efm32_if',              None,               None                            ),
    (   0x5052,     'lpc11u35_arm_watch_nrf51_if',              None,               None                            ),
    (   0x9004,     'lpc11u35_archpro_if',                      None,               'Seeeduino-Arch-Pro'            ),
    (   0x9009,     'lpc11u35_archble_if',                      None,               'Seeed-Arch-BLE'                ),
    (   0x9011,     'lpc11u35_archmax_if',                      None,               'Seeed-Arch-Max'                ),
    (   0x9012,     'lpc11u35_tiny_if',                         None,               'Seeed-Tiny-BLE'                ),
    (   0x9013,     'lpc11u35_archlink_if',                     None,               'Seeed-Arch-Link'               ),
    (   0x9900,     'kl26z_microbit_if',                        'kl26z_bl',         'Microbit'                      ),
    (   0x7403,     'lpc11u35_6LoWPAN_BorderRouterUSB_if',      None,               None                            ),
    (   0x7404,     'lpc11u35_6LoWPAN_BorderRouterPOE_if',      None,               None                            ),
    (   0x7402,     'lpc11u35_6LoWPAN_BorderRouterHAT_if',      None,               None                            ),
    (   0x7401,     'lpc11u35_swdap_k64f_if',                   None,               None                            ),
    (   0x7401,     'lpc11u35_swdap_lpc1768_if',                None,               None                            ),
    (   0x7401,     'lpc11u35_swdap_nrf51822_if',               None,               None                            ),
    
]

BOARD_ID_TO_BUILD_TARGET = {config[0]: config[3] for config in
                            SUPPORTED_CONFIGURATIONS}
FIRMWARE_SET = set((config[1] for config in SUPPORTED_CONFIGURATIONS))
TARGET_SET = set((target[3] for target in SUPPORTED_CONFIGURATIONS if
                  target[3] is not None))

BOARD_ID_LOCKED_WHEN_ERASED = set([
    0x0231,  # K22F
    0x0240,  # K64F
    0x0230,  # K20DX
    0x0200,  # KL25Z
    0x0220,  # KL46Z
    0x0210,  # KL05Z
])

TARGET_WITH_COMPILE_API_LIST = [config[3] for config in
                                SUPPORTED_CONFIGURATIONS if
                                config[3] is not None]

#Hack until these targets have an image with a valid vector table
TARGET_WITH_BAD_VECTOR_TABLE_LIST = [
    'Microbit',
    'Nordic-nRF51-DK',
    'Nordic-nRF51822',
    'Nordic-nRF51-Dongle',
    'Seeed-Arch-BLE',
    'mbed-HRM1017',
    'Switch-Science-mbed-TY51822r3',
    'RedBearLab-nRF51822',
    'RedBearLab-BLE-Nano',
    'Seeed-Tiny-BLE',
    'Seeed-Arch-Link',
]
