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
from __future__ import absolute_import

# Add new HICs here
HIC_STRING_TO_ID = {
    'k20dx': 0x97969900,
    'kl26z': 0x97969901,
    'lpc11u35': 0x97969902,
    'sam3u2c': 0x97969903,
}

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
    'k20dx_frdmk24f_if': 0x0280,
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
}

TARGET_NAME_TO_BOARD_ID = {
    'FRDM-K22F': 0x0231,
    'NXP-LPC800-MAX': 0x1050,
    'FRDM-K64F': 0x0240,
    'Microbit': 0x9900,
    'Nordic-nRF51-DK': 0x1100,
    'FRDM-K20D50M': 0x0230,
    #k20dx_frdmk24f_if
    #k20dx_frdmkl02z_if
    'FRDM-KL05Z': 0x0210,
    'KL25Z': 0x0200,
    #k20dx_frdmkl26z_if
    'FRDM-KL46Z': 0x0220,
    'Nordic-nRF51822': 0x1070,
    'Nordic-nRF51-Dongle': 0x1120,
    'Seeed-Arch-BLE': 0x9009,
    'Seeeduino-Arch-Pro': 0x9004,
    'Seeed-Arch-Max': 0x9011,
    'mbed-HRM1017': 0x1017,
    'Switch-Science-mbed-TY51822r3': 0x1019,
    'Switch-Science-mbed-LPC824': 0x1018,
    'LPC1114FN28': 0x1114,
    'LPCXpresso824-MAX': 0x0824,
    'RedBearLab-nRF51822': 0x1090,
    'RedBearLab-BLE-Nano': 0x1095,
    'Seeed-Tiny-BLE': 0x9012,
    'u-blox-C027': 0x1234,
    #lpc11u35_arm_watch_stm32f411_if
    #lpc11u35_arm_watch_efm32_if
    #lpc11u35_arm_watch_nrf51_if
    'Seeed-Arch-Link': 0x9013,
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

BOARD_ID_TO_BUILD_TARGET = {v: k for k, v in TARGET_NAME_TO_BOARD_ID.items()}
