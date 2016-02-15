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

# Add new HDKs here
HDK_STRING_TO_ID = {
    'k20dx': 0x646c0000,
    'kl26z': 0x646c0001,
    'lpc11u35': 0x646c0002,
    'sam3u2c': 0x646c0003,
}

# Add new firmware with a hard coded board ID here
FIRMWARE_NAME_TO_BOARD_ID = {
    'k20dx_k22f_if': 0x0231,
    'k20dx_k64f_if': 0x0240,
    'kl26z_microbit_if': 0x9900,
    'kl26z_nrf51822_if': 0x9900,
    'lpc11u35_lpc812_if': 0x1050,
    'lpc11u35_lpc1114_if': 0x1114,
    'lpc11u35_efm32gg_stk_if': 0x2015,
    'sam3u2c_nrf51822_if': 0x1100,
}

TARGET_NAME_TO_BOARD_ID = {
    'FRDM-K22F': 0x0231,
    'NXP-LPC800-MAX': 0x1050,
    'FRDM-K64F': 0x0240,
    'Microbit': 0x9900,
    'Nordic-nRF51-DK': 0x1100,
}

BOARD_ID_TO_BUILD_TARGET = {v: k for k, v in TARGET_NAME_TO_BOARD_ID.items()}
