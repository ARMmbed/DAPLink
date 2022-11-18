#
# DAPLink Interface Firmware
# Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
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
PROJECT_RELEASE_INFO = [
    # Project Name                                  Legacy      Offset      Extension
    ("k20dx_frdmk22f_if",                           True,       0x8000,     "bin"       ),
    ("k20dx_frdmk64f_if",                           True,       0x5000,     "bin"       ),
    ("k20dx_hexiwear_if",                           True,       0x8000,     "bin"       ),
    ("k20dx_rapid_iot_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmk66f_if",                           True,       0x8000,     "bin"       ),
    ("k20dx_frdmk28f_if",                           True,       0x8000,     "bin"       ),
    ("k20dx_frdmk82f_if",                           True,       0x8000,     "bin"       ),
    ("kl26z_artemis_dk_if",                         False,      0x8000,     "hex"       ),
    ("kl26z_microbit_if",                           False,      0x8000,     "hex"       ),
    ("kl27z_microbit_if",                           False,      0x8000,     "hex"       ),
    ("nrf52820_microbit_if",                        False,      0x8000,     "hex"       ),
    ("lpc11u35_lpc812xpresso_if",                   False,      0x0000,     "bin"       ),
    ("lpc11u35_lpc824xpresso_if",                   False,      0x0000,     "bin"       ),
    ("lpc4322_lpc54114xpresso_if",                  False,      0x10000,    "bin"       ),
    ("lpc4322_lpc54608xpresso_if",                  False,      0x10000,    "bin"       ),
    ("lpc4322_lpc55s69xpresso_if",                  False,      0x10000,    "bin"       ),
    ("lpc11u35_ssci1114_if",                        False,      0x0000,     "bin"       ),
    ("sam3u2c_mkit_dk_dongle_nrf5x_if",             True,       0x5000,     "bin"       ),
    ("sam3u2c_nrf5x_dk_ext_if",                     True,       0x5000,     "bin"       ),
    ("k20dx_frdmk20dx_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkw24d_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkw41z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl02z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl03z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl05z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl25z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl26z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl27z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl28z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_twrkl28z72m_if",                        True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl43z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl46z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl82z_if",                          True,       0x5000,     "bin"       ),
    ("k20dx_frdmke15z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_twrke18f_if",                           True,       0x8000,     "bin"       ),
    ("k20dx_hvpke18f_if",                           True,       0x8000,     "bin"       ),
    ("k20dx_mimxrt1020_evk_qspi_if",                True,       0x8000,     "bin"       ),
    ("k20dx_mimxrt1050_evk_hyper_if",               True,       0x8000,     "bin"       ),
    ("lpc4322_mimxrt1060_evk_qspi_if",              False,      0x10000,    "bin"       ),
    ("lpc4322_mimxrt1170_evk_qspi_if",              False,      0x10000,    "bin"       ),
    ('k20dx_ep_agora_if',                           True,      	0x0000,     "bin"       ),
    ('k20dx_ep_kairos_if',                          True,       0x0000,     "bin"       ),
    ("k26f_frdmk32w042_if",                         False,      0x8000,     "bin"       ),
    ("lpc11u35_archble_if",                         False,      0x0000,     "bin"       ),
    ("lpc11u35_archpro_if",                         False,      0x0000,     "bin"       ),
    ("lpc11u35_archmax_if",                         False,      0x0000,     "bin"       ),
    ("lpc11u35_hrm1017_if",                         False,      0x0000,     "bin"       ),
    ("lpc11u35_sscity_if",                          False,      0x0000,     "bin"       ),
    ("lpc11u35_ssci824_if",                         False,      0x0000,     "bin"       ),
    ("lpc11u35_archlink_if",                        False,      0x0000,     "bin"       ),
    ("lpc11u35_tiny_if",                            False,      0x0000,     "bin"       ),
    ("lpc11u35_c027_if",                            False,      0x0000,     "bin"       ),
    ("lpc11u35_lpc4088dm_if",                       False,      0x0000,     "bin"       ),
    ("lpc11u35_lpc4088qsb_if",                      False,      0x0000,     "bin"       ),
    ('lpc11u35_ssci_chibi_if',                      False,      0x0000,     "bin"       ),
    ('lpc11u35_ff1705_l151_if',                     False,      0x0000,     "bin"       ),
    ('lpc11u35_ff_lpc546xx_if',                     False,      0x0000,     "bin"       ),
    ('lpc11u35_mini_iot_lpc54018_if',               False,      0x0000,     "bin"       ),
    ("k20dx_xdot_l151_if",                          False,      0x8000,     "bin"       ),
    ('lpc11u35_gr_peach_if',                        None,       0x0000,     "bin"       ),
    ('lpc11u35_gr_lychee_if',                       None,       0x0000,     "bin"       ),
    ('lpc11u35_nz32_sc151_if',                      False,      0x0000,     "bin"       ),
    ("sam3u2c_ublox_evk_nina_b1_if",                True,       0x5000,     "bin"       ),
    ("lpc11u35_rtl8195am_if",                       False,      0x0000,     "bin"       ),
    ("lpc11u35_96b_nitrogen_if",                    False,      0x0000,     "bin"       ),
    ('lpc11u35_musca_a_if',                         False,      0x0000,     "bin"       ),
    ('lpc11u35_musca_b_if',                         False,      0x0000,     "bin"       ),
    ('lpc11u35_musca_b_eflash_if',                  False,      0x0000,     "bin"       ),
    ('max32620_max32625mbed_if',                    False,      0x0000,     "bin"       ),
    ('max32625_max32620fthr_if',                    False,      0x0000,     "bin"       ),
    ('max32625_max32630fthr_if',                    False,      0x0000,     "bin"       ),
    ('max32625_max32660evsys_if',                   False,      0x0000,     "bin"       ),
    ('max32625_max32666fthr_if',                    False,      0x0000,     "bin"       ),
    ('max32625_max78000fthr_if',                    False,      0x0000,     "bin"       ),
    ('max32625_max32670evkit_if',                   False,      0x0000,     "bin"       ),
    ('max32625_max32650fthr_if',                    False,      0x0000,     "bin"       ),
    ('kl26z_if',                                    False,      0x0000,     "bin"       ),
    ('k20dx_if',                                    False,      0x0000,     "bin"       ),
    ('k26f_if',                                     False,      0x0000,     "bin"       ),
    ('lpc11u35_if',                                 False,      0x0000,     "bin"       ),
    ('lpc4322_if',                                  False,      0x0000,     "bin"       ),
    ('lpc55s69_if',                                 False,      0x10000,    "bin"       ),
    ('lpc55s69_mculink_if',                         False,      0x10000,    "bin"       ),
    ('max32620_if',                                 False,      0x0000,     "bin"       ),
    ('max32625_if',                                 False,      0x0000,     "bin"       ),
    ('nrf52820_if',                                 False,      0x0000,     "bin"       ),
    ('sam3u2c_if',                                  False,      0x0000,     "bin"       ),
    ('stm32f103xb_if',                              False,      0x0000,     "bin"       ),
    ('lpc11u35_pitaya_link_if',                     False,      0x0000,     "bin"       ),
]

TEST_RELEASE_INFO = [
    ('lpc55s69_nrf52840dk_test_if',                 False,      0x10000,    "bin"       ),
    ('stm32f103xb_stm32f207zg_if',                  False,      0x0000,     "bin"       ),
]


# Add new HICs here
VENDOR_ID = {
    'Stub': 0,
    'NXP': 11,
    'TI': 16,
    'Nordic': 54,
    'Toshiba': 92,
    'Renesas' : 117,
    'Wiznet': 122,
    'Realtek': 124,
    'Ambiq' : 126,
}

def VENDOR_TO_FAMILY(x, y) : return (VENDOR_ID[x] <<8) | y

# All supported configurations
SUPPORTED_CONFIGURATIONS = [
    #   Board ID    Family ID                           Firmware                                    Bootloader          Target
    (   0x0200,     VENDOR_TO_FAMILY('NXP', 2),         'k20dx_frdmkl25z_if',                       'k20dx_bl',         'KL25Z'                                 ),
    (   0x0201,     VENDOR_TO_FAMILY('NXP', 2),         'k20dx_frdmkw41z_if',                       'k20dx_bl',         'FRDM-KW41Z'                            ),
    (   0x0203,     VENDOR_TO_FAMILY('NXP', 2),         'k20dx_twrkl28z72m_if',                     'k20dx_bl',         None                                    ),
    (   0x0204,     VENDOR_TO_FAMILY('NXP', 2),         'k20dx_frdmkl02z_if',                       'k20dx_bl',         None                                    ),
    (   0x0205,     VENDOR_TO_FAMILY('NXP', 2),         'k20dx_frdmkl28z_if',                       'k20dx_bl',         None                                    ),
    (   0x0206,     VENDOR_TO_FAMILY('NXP', 1),         'k20dx_twrke18f_if',                        'k20dx_bl',         None                                    ),
    (   0x0207,     VENDOR_TO_FAMILY('NXP', 2),         'k20dx_frdmkl03z_if',                       'k20dx_bl',         'FRDM-KL03Z'                            ),
    (   0x0210,     VENDOR_TO_FAMILY('NXP', 2),         'k20dx_frdmkl05z_if',                       'k20dx_bl',         'FRDM-KL05Z'                            ),
    (   0x0213,     VENDOR_TO_FAMILY('NXP', 2),         'k20dx_frdmke15z_if',                       'k20dx_bl',         None                                    ),
    (   0x0214,     VENDOR_TO_FAMILY('NXP', 4),         'k20dx_hexiwear_if',                        'k20dx_bl',         'HEXIWEAR'                              ),
    (   0x0216,     VENDOR_TO_FAMILY('NXP', 1),         'k20dx_hvpke18f_if',                        'k20dx_bl',         None                                    ),
    (   0x0217,     VENDOR_TO_FAMILY('NXP', 1),         'k20dx_frdmk82f_if',                        'k20dx_bl',         'FRDM-K82F'                             ),
    (   0x0218,     VENDOR_TO_FAMILY('NXP', 2),         'k20dx_frdmkl82z_if',                       'k20dx_bl',         'FRDM-KL82Z'                            ),
    (   0x0220,     VENDOR_TO_FAMILY('NXP', 2),         'k20dx_frdmkl46z_if',                       'k20dx_bl',         'FRDM-KL46Z'                            ),
    (   0x0224,     VENDOR_TO_FAMILY('NXP', 1),         'k20dx_frdmk28f_if',                        'k20dx_bl',         None                                    ), # TODO - set target to 'FRDM-K28F' when mbed-os supports this
    (   0x0225,     VENDOR_TO_FAMILY('NXP', 1),         'k26f_frdmk32w042_if',                      'k26f_bl',          None                                    ), # TODO - set target to 'FRDM-K32W042' when mbed-os supports this
    (   0x0226,     VENDOR_TO_FAMILY('NXP', 3),         'k20dx_mimxrt1020_evk_qspi_if',             'k20dx_bl',         'MIMXRT1020'                            ),
    (   0x0227,     VENDOR_TO_FAMILY('NXP', 3),         'k20dx_mimxrt1050_evk_hyper_if',            'k20dx_bl',         'MIMXRT1050'                            ),
    (   0x0228,     VENDOR_TO_FAMILY('NXP', 4),         'k20dx_rapid_iot_if',                       'k20dx_bl',         None                                    ), # TODO - set target to 'Rapid-IoT' when mbed-os supports this
    (   0x0229,     VENDOR_TO_FAMILY('NXP', 3),         'lpc4322_mimxrt1060_evk_qspi_if',           'lpc4322_bl',       'MIMXRT1060'                            ),
    (   0x0230,     VENDOR_TO_FAMILY('NXP', 1),         'k20dx_frdmk20dx_if',                       'k20dx_bl',         'FRDM-K20D50M'                          ),
    (   0x0231,     VENDOR_TO_FAMILY('NXP', 1),         'k20dx_frdmk22f_if',                        'k20dx_bl',         'FRDM-K22F'                             ),
    (   0x0236,     VENDOR_TO_FAMILY('NXP', 0),         'lpc4322_lpc55s69xpresso_if',               'lpc4322_bl',       'LPCXpresso55S69'                       ),
    (   0x0240,     VENDOR_TO_FAMILY('NXP', 1),         'k20dx_frdmk64f_if',                        'k20dx_bl',         'FRDM-K64F'                             ),
    (   0x0244,     VENDOR_TO_FAMILY('NXP', 3),         'lpc4322_mimxrt1170_evk_qspi_if',           'lpc4322_bl',       'MIMXRT1170'                            ),
    (   0x0250,     VENDOR_TO_FAMILY('NXP', 1),         'k20dx_frdmkw24d_if',                       'k20dx_bl',         'FRDM-KW24D512'                         ),
    (   0x0260,     VENDOR_TO_FAMILY('NXP', 2),         'k20dx_frdmkl26z_if',                       'k20dx_bl',         None                                    ),
    (   0x0261,     VENDOR_TO_FAMILY('NXP', 2),         'k20dx_frdmkl27z_if',                       'k20dx_bl',         'FRDM-KL27Z'                            ),
    (   0x0262,     VENDOR_TO_FAMILY('NXP', 2),         'k20dx_frdmkl43z_if',                       'k20dx_bl',         'FRDM-KL43Z'                            ),
    (   0x0311,     VENDOR_TO_FAMILY('NXP', 1),         'k20dx_frdmk66f_if',                        'k20dx_bl',         'FRDM-K66F'                             ),
    (   0x0350,     VENDOR_TO_FAMILY('Stub', 1),        'k20dx_xdot_l151_if',                       'k20dx_bl',         None                                    ), # TODO - set target to 'MTS-xDot-L151CC' when mbed-os supports this
    (   0x0360,     VENDOR_TO_FAMILY('NXP', 1),         'lpc4322_hani_iot_if',                      'lpc4322_bl',       'HANI-IOT'                              ),
    (   0x0409,     VENDOR_TO_FAMILY('Stub', 1),        'max32625_max32630fthr_if',                 'max32625_bl',      'MAX32630'                              ),
    (   0x0415,     VENDOR_TO_FAMILY('Stub', 1),        'max32620_max32625mbed_if',                 'max32620_bl',      'MAX32625'                              ),
    (   0x0418,     VENDOR_TO_FAMILY('Stub', 1),        'max32625_max32620fthr_if',                 'max32625_bl',      'MAX32620'                              ),
    (   0x0421,     VENDOR_TO_FAMILY('Stub', 1),        'max32625_max32660evsys_if',                'max32625_bl',      'MAX32660'                              ),
    (   0x0422,     VENDOR_TO_FAMILY('Stub', 1),        'max32625_max32666fthr_if',                 'max32625_bl',      'MAX32666'                              ),
    (   0x0423,     VENDOR_TO_FAMILY('Stub', 1),        'max32625_max78000fthr_if',                 'max32625_bl',      'MAX78000'                              ),
    (   0x0424,     VENDOR_TO_FAMILY('Stub', 1),        'max32625_max32670evkit_if',                'max32625_bl',      'MAX32670'                              ),
    (   0x0425,     VENDOR_TO_FAMILY('Stub', 1),        'max32625_max32650fthr_if',                 'max32625_bl',      'MAX32650'                              ),
    (   0x0700,     VENDOR_TO_FAMILY('Stub', 1),        'stm32f103xb_stm32f103rb_if',               'stm32f103xb_bl',   'ST-Nucleo-F103RB'                      ),
    (   0x0720,     VENDOR_TO_FAMILY('Stub', 1),        'stm32f103xb_stm32f401re_if',               'stm32f103xb_bl',   'ST-Nucleo-F401RE'                      ),
    (   0x0720,     VENDOR_TO_FAMILY('Stub', 1),        'stm32f103xb_stm32f072rb_if',               'stm32f103xb_bl',   'ST-Nucleo-F072RB'                      ),
    (   0x0735,     VENDOR_TO_FAMILY('Stub', 1),        'stm32f103xb_stm32f334r8_if',               'stm32f103xb_bl',   'ST-Nucleo-F334R8'                      ),
    (   0x0740,     VENDOR_TO_FAMILY('Stub', 1),        'stm32f103xb_stm32f411re_if',               'stm32f103xb_bl',   'ST-Nucleo-F411RE'                      ),
    (   0x0765,     VENDOR_TO_FAMILY('Stub', 1),        'stm32f103xb_stm32l476rg_if',               'stm32f103xb_bl',   'ST-Nucleo-L476RG'                      ),
    (   0x0796,     VENDOR_TO_FAMILY('Stub', 1),        'stm32f103xb_stm32f429zi_if',               'stm32f103xb_bl',   'ST-Nucleo-F429ZI'                      ),
    (   0x0816,     VENDOR_TO_FAMILY('Stub', 1),        'stm32f103xb_stm32f746zg_if',               'stm32f103xb_bl',   'ST-Nucleo-F746ZG'                      ),
    (   0x0824,     VENDOR_TO_FAMILY('Stub', 1),        'lpc11u35_lpc824xpresso_if',                None,               'LPCXpresso824-MAX'                     ),
    (   0x0835,     VENDOR_TO_FAMILY('Stub', 1),        'stm32f103xb_stm32f207zg_if',               'stm32f103xb_bl',   'ST-Nucleo-F207ZG'                      ),
    (   0x1017,     VENDOR_TO_FAMILY('Nordic', 1),      'lpc11u35_hrm1017_if',                      None,               'mbed-HRM1017'                          ),
    (   0x1018,     VENDOR_TO_FAMILY('Stub', 1),        'lpc11u35_ssci824_if',                      None,               'Switch-Science-mbed-LPC824'            ),
    (   0x1019,     VENDOR_TO_FAMILY('Nordic', 1),      'lpc11u35_sscity_if',                       None,               'Switch-Science-mbed-TY51822r3'         ),
    (   0x1021,     VENDOR_TO_FAMILY('Nordic', 1),      'lpc11u35_ssci_chibi_if',                   None,               None                                    ), # TODO - set target to 'SSCI-MBIT' when mbed-os supports this
    (   0x1028,     VENDOR_TO_FAMILY('Nordic', 2),      'lpc11u35_pitaya_link_if',                  None,               None                                    ),
    (   0x1050,     VENDOR_TO_FAMILY('Stub', 1),        'lpc11u35_lpc812xpresso_if',                None,               'NXP-LPC800-MAX'                        ),
    (   0x1054,     VENDOR_TO_FAMILY('Stub', 1),        'lpc4322_lpc54114xpresso_if',               'lpc4322_bl',       'LPCXpresso54114'                       ),
    (   0x1056,     VENDOR_TO_FAMILY('Stub', 1),        'lpc4322_lpc54608xpresso_if',               'lpc4322_bl',       'LPCXpresso54608'                       ),
    (   0x1060,     VENDOR_TO_FAMILY('NXP', 0),         'lpc11u35_lpc4088qsb_if',                   None,               'EA-LPC4088'                            ),
    (   0x1062,     VENDOR_TO_FAMILY('NXP', 0),         'lpc11u35_lpc4088dm_if',                    None,               'EA-LPC4088-Display-Module'             ),
    (   0x1070,     VENDOR_TO_FAMILY('Nordic', 1),      'sam3u2c_mkit_dk_dongle_nrf5x_if',          'sam3u2c_bl',       'Nordic-nRF51822'                       ),
    (   0x1100,     VENDOR_TO_FAMILY('Nordic', 1),      'sam3u2c_mkit_dk_dongle_nrf5x_if',          'sam3u2c_bl',       'Nordic-nRF51-DK'                       ),
    (   0x1101,     VENDOR_TO_FAMILY('Nordic', 2),      'sam3u2c_mkit_dk_dongle_nrf5x_if',          'sam3u2c_bl',       'Nordic-nRF52-DK'                       ),
    (   0x1102,     VENDOR_TO_FAMILY('Nordic', 2),      'sam3u2c_mkit_dk_dongle_nrf5x_if',          'sam3u2c_bl',       'Nordic-nRF52840-DK'                    ),
    (   0x1114,     VENDOR_TO_FAMILY('Stub', 1),        'lpc11u35_ssci1114_if',                     None,               'LPC1114FN28'                           ),
    (   0x1120,     VENDOR_TO_FAMILY('Nordic', 1),      'sam3u2c_mkit_dk_dongle_nrf5x_if',          'sam3u2c_bl',       'Nordic-nRF51-Dongle'                   ),
    (   0x1234,     VENDOR_TO_FAMILY('Stub', 1),        'lpc11u35_c027_if',                         None,               'u-blox-C027'                           ),
    (   0x1236,     VENDOR_TO_FAMILY('Stub', 1),        'stm32f103xb_ublox_evk_odin_w2_if',         'stm32f103xb_bl',   'ublox-EVK-ODIN-W2'                     ),
    (   0x1237,     VENDOR_TO_FAMILY('Nordic', 2),      'sam3u2c_ublox_evk_nina_b1_if',             'sam3u2c_bl',       'U-BLOX-EVK-NINA-B1'                    ),
    (   0x1304,     VENDOR_TO_FAMILY('Stub', 3),        'm48ssidae_numaker_pfm_m487km_if',          'm48ssidae_bl',     None                                    ),
    (   0x1309,     VENDOR_TO_FAMILY('Stub', 3),        'm48ssidae_numaker_m252kg_if',              'm48ssidae_bl',     None                                    ),
    (   0x1310,     VENDOR_TO_FAMILY('Stub', 3),        'm48ssidae_numaker_iot_m263a_if',           'm48ssidae_bl',     None                                    ),
    (   0x1312,     VENDOR_TO_FAMILY('Stub', 3),        'm48ssidae_numaker_m2354_if',               'm48ssidae_bl',     None                                    ),
    (   0x1313,     VENDOR_TO_FAMILY('Stub', 3),        'm48ssidae_numaker_iot_m467_if',            'm48ssidae_bl',     None                                    ),
    (   0x2600,     VENDOR_TO_FAMILY('Nordic', 2),      'k20dx_ep_agora_if',                        None,               None                                    ),
    (   0x2601,     VENDOR_TO_FAMILY('NXP', 0),         'k20dx_ep_kairos_if',                       None,               None                                    ),
    (   0x4600,     VENDOR_TO_FAMILY('Realtek', 1),     'lpc11u35_rtl8195am_if',                    None,               'REALTEK-RTL8195AM'                     ),
    (   0x5006,     VENDOR_TO_FAMILY('Stub', 1),        'lpc11u35_musca_a_if',                      None,               'ARM_MUSCA_A1'                          ),
    (   0x5007,     VENDOR_TO_FAMILY('Stub', 1),        'lpc11u35_musca_b_if',                      None,               'ARM_MUSCA_B1'                          ),
    (   0x5007,     VENDOR_TO_FAMILY('Stub', 1),        'lpc11u35_musca_b_eflash_if',               None,               'ARM_MUSCA_B1'                          ),
    (   0x5500,     VENDOR_TO_FAMILY('Renesas', 1),     'lpc11u35_gr_peach_if',                     None,               None                                    ), # TODO - Set to 'Renesas-GR-PEACH' once this target builds
    (   0x5501,     VENDOR_TO_FAMILY('Renesas', 1),     'lpc11u35_gr_lychee_if',                    None,               None                                    ), # TODO - Set to 'Renesas-GR-LYCHEE' once this target builds
    (   0x6660,     VENDOR_TO_FAMILY('Stub', 1),        'lpc11u35_nz32_sc151_if',                   None,               None                                    ), # TODO - set target to 'NZ32-SC151' when mbed-os supports this
    (   0x7010,     VENDOR_TO_FAMILY('Toshiba', 1),     'lpc11u35_blueninja_if',                    None,               None                                    ), # TODO - set target to 'BlueNinja-CDP-TZ01B' when mbed-os supports this
    (   0x8080,     VENDOR_TO_FAMILY('Stub', 1),        'lpc11u35_ff1705_l151_if',                  None,               None                                    ), # TODO - set target to 'L-TEK-FF1705' when mbed-os supports this
    (   0x8081,     VENDOR_TO_FAMILY('Stub', 1),        'lpc11u35_ff_lpc546xx_if',                  None,               None                                    ), # TODO - set target to 'L-TEK-FF-LPC546XX' when mbed-os supports this
    (   0x9004,     VENDOR_TO_FAMILY('Stub', 1),        'lpc11u35_archpro_if',                      None,               'Seeeduino-Arch-Pro'                    ),
    (   0x9009,     VENDOR_TO_FAMILY('Nordic', 1),      'lpc11u35_archble_if',                      None,               'Seeed-Arch-BLE'                        ),
    (   0x9011,     VENDOR_TO_FAMILY('Stub', 1),        'lpc11u35_archmax_if',                      None,               'Seeed-Arch-Max'                        ),
    (   0x9012,     VENDOR_TO_FAMILY('Nordic', 1),      'lpc11u35_tiny_if',                         None,               'Seeed-Tiny-BLE'                        ),
    (   0x9013,     VENDOR_TO_FAMILY('Nordic', 1),      'lpc11u35_archlink_if',                     None,               'Seeed-Arch-Link'                       ),
    (   0x9016,     VENDOR_TO_FAMILY('Nordic', 2),      'lpc11u35_96b_nitrogen_if',                 None,               None                                    ), # TODO - set target to 'Seeed-96Boards-Nitrogen' when mbed-os supports this
    (   0x9900,     VENDOR_TO_FAMILY('Nordic', 1),      'kl26z_microbit_if',                        'kl26z_bl',         'Microbit'                              ),
    (   0x9901,     VENDOR_TO_FAMILY('Nordic', 1),      'kl26z_microbit_if',                        'kl26z_bl',         'Microbit'                              ),
    (   0x9903,     VENDOR_TO_FAMILY('Nordic', 2),      'kl27z_microbit_if',                        'kl27z_bl',         'Microbitv2'                            ),
    (   0x9904,     VENDOR_TO_FAMILY('Nordic', 2),      'kl27z_microbit_if',                        'kl27z_bl',         'Microbitv2'                            ),
    (   0x9905,     VENDOR_TO_FAMILY('Nordic', 2),      'nrf52820_microbit_if',                     'nrf52820_bl',      'Microbitv2'                            ),
    (   0x9906,     VENDOR_TO_FAMILY('Nordic', 2),      'nrf52820_microbit_if',                     'nrf52820_bl',      'Microbitv2'                            ),
    (   0xA127,     VENDOR_TO_FAMILY('Ambiq', 1),       'kl26z_artemis_dk_if',                      'kl26z_bl',         'ARTMBED'                               ),
    (   0x0000,     VENDOR_TO_FAMILY('Stub', 1),        'kl26z_if',                                 None,               None                                    ),
    (   0x0000,     VENDOR_TO_FAMILY('Stub', 1),        'k20dx_if',                                 None,               None                                    ),
    (   0x0000,     VENDOR_TO_FAMILY('Stub', 1),        'k26f_if',                                  None,               None                                    ),
    (   0x0000,     VENDOR_TO_FAMILY('Stub', 1),        'lpc11u35_if',                              None,               None                                    ),
    (   0x0000,     VENDOR_TO_FAMILY('Stub', 1),        'lpc4322_if',                               None,               None                                    ),
    (   0x0000,     VENDOR_TO_FAMILY('Stub', 1),        'lpc55s69_if',                              None,               None                                    ),
    (   0x0000,     VENDOR_TO_FAMILY('Stub', 1),        'max32620_if',                              None,               None                                    ),
    (   0x0000,     VENDOR_TO_FAMILY('Stub', 1),        'max32625_if',                              None,               None                                    ),
    (   0x0000,     VENDOR_TO_FAMILY('Stub', 1),        'nrf52820_if',                              None,               None                                    ),
    (   0x0000,     VENDOR_TO_FAMILY('Stub', 1),        'sam3u2c_if',                               None,               None                                    ),
    (   0x0000,     VENDOR_TO_FAMILY('Stub', 1),        'stm32f103xb_if',                           None,               None                                    ),

    # Test projects
    (   0x1102,     VENDOR_TO_FAMILY('Nordic', 2),      'lpc55s69_nrf52840dk_test_if',              'lpc55s69_bl',      'Nordic-nRF52840-DK'                    ),
]

# Add new HICs here
HIC_STRING_TO_ID = {
    'k20dx': 0x97969900,
    'k26f': 0x97969909,
    'kl26z': 0x97969901,
    'kl27z': 0x9796990B,
    'lpc11u35': 0x97969902,
    'lpc4322': 0x97969905,
    'lpc55s69': 0x4C504355,
    'm48ssidae': 0x97969921,
    'max32620': 0x97969904,
    'max32625': 0x97969906,
    'nrf52820': 0x6E052820,
    'sam3u2c': 0x97969903,
    'stm32f103xb': 0x97969908,
}

BOARD_ID_LOCKED_WHEN_ERASED = set([
    0x0231,  # K22F
    0x0224,  # K28F
    0x0240,  # K64F
    0x0214,  # HEXIWEAR
    0x0228,  # RAPID-IOT
    0x0311,  # K66F
    0x0230,  # K20DX
    0x0200,  # KL25Z
    0x0205,  # FRDM-KL28Z
    0x0203,  # TWR-KL28Z72M
    0x0206,  # TWR-KE18F
    0x0220,  # KL46Z
    0x0210,  # KL05Z
    0x0213,  # FRDM-KE15Z
    0x0216,  # HVP-KE18F
    0x0217,  # FRDM-K82F
    0x0225,  # FRDM-K32W042
    0x0261,  # FRDM-KL27Z
    0x0262,  # FRDM-KL43Z
    0x0218,  # FRDM-KL82Z
    0x0201,  # KW41Z
])

BOARD_ID_SUPPORTING_PAGE_ERASE = set([
    0x0214,  # HEXIWEAR
    0x0225,  # FRDM-K32W042
    0x0226,  # MIMXRT1020 EVK
    0x0227,  # MIMXRT1050 EVK
    0x0228,  # k20dx_rapid_iot_if
    0x0235,  # lpc11u35_mini_iot_lpc54018_if
    0x0236,  # LPCXpresso55S69
    0x0240,  # K64F
    0x0311,  # K66F
    0x0409,  # MAX32630FTHR
    0x0415,  # MAX32625MBED
    0x0418,  # MAX32620FTHR
    0x0421,  # MAX32660EVSYS
    0x0422,  # MAX32666FTHR
    0x0423,  # MAX78000FTHR
    0x0424,  # MAX32670EVKIT
    0x0425,  # MAX32650FTHR
    0x0460,  # WISE-1570
    0x1101,  # Nordic-nRF52-DK
    0x1102,  # Nordic-nRF52840-DK
    0x1304,  # NuMaker-PFM-M487KM
    0x1309,  # NuMaker-M252KG
    0x1310,  # NuMaker-IoT-M263A
    0x1312,  # NuMaker-M2354
    0x1313,  # NuMaker-IoT-M467
    0x3103,  # dipdap_sdt51822b
    0x3104,  # dipdap_sdt52832b
    0x3105,  # dipdap_sdt64b
    0x3108,  # dipdap_sdt32429b
    0x3110,  # dipdap_sdt32439b
    0x4600,  # REALTEK-RTL8195AM
    0x5500,  # GR-PEACH
    0x5501,  # GR-LYCHEE
])

#Hack until these targets have an image with a valid vector table
TARGET_WITH_BAD_VECTOR_TABLE_LIST = [
    'Artemis_DK',
    'Microbit',
    'Nordic-nRF51-DK',
    'Nordic-nRF52-DK',
    'Nordic-nRF52840-DK',
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
    'BlueNinja',
    'U-BLOX-EVK-NINA-B1',
    'REALTEK-RTL8195AM']

BOARD_ID_TO_BUILD_TARGET = {config[0]: config[4] for config in
                            SUPPORTED_CONFIGURATIONS}
FIRMWARE_SET = set((config[2] for config in SUPPORTED_CONFIGURATIONS))
TARGET_SET = set((target[4] for target in SUPPORTED_CONFIGURATIONS if
                  target[4] is not None))

TARGET_WITH_COMPILE_API_LIST = [config[4] for config in
                                SUPPORTED_CONFIGURATIONS if
                                config[4] is not None]
