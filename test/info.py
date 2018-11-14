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
    ("k20dx_hexiwear_if",                           True,       0x8000,     "bin"       ),
    ("k20dx_rapid_iot_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmk66f_if",                           True,       0x8000,     "bin"       ),
    ("k20dx_frdmk28f_if",                           True,       0x8000,     "bin"       ),
    ("k20dx_frdmk82f_if",                           True,       0x8000,     "bin"       ),
    ("kl26z_microbit_if",                           False,      0x8000,     "hex"       ),
    ("lpc11u35_lpc812xpresso_if",                   False,      0x0000,     "bin"       ),
    ("lpc11u35_lpc824xpresso_if",                   False,      0x0000,     "bin"       ),
    ("lpc4322_lpc54114xpresso_if",                  False,      0x10000,    "bin"       ),
    ("lpc4322_lpc54608xpresso_if",                  False,      0x10000,    "bin"       ),
    ("lpc11u35_ssci1114_if",                        False,      0x0000,     "bin"       ),
    ("sam3u2c_mkit_dk_dongle_nrf5x_if",             True,       0x5000,     "bin"       ),
    ("k20dx_frdmk20dx_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkw24d_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkw41z_if",                          True,       0x8000,     "bin"       ),
    ("k20dx_frdmkl02z_if",                          True,       0x8000,     "bin"       ),
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
    ("k20dx_mimxrt1050_evk_qspi_if",                True,       0x8000,     "bin"       ),
    ("lpc11u35_archble_if",                         False,      0x0000,     "bin"       ),
    ("lpc11u35_archpro_if",                         False,      0x0000,     "bin"       ),
    ("lpc11u35_archmax_if",                         False,      0x0000,     "bin"       ),
    ("lpc11u35_hrm1017_if",                         False,      0x0000,     "bin"       ),
    ("lpc11u35_sscity_if",                          False,      0x0000,     "bin"       ),
    ("lpc11u35_ssci824_if",                         False,      0x0000,     "bin"       ),
    ("lpc11u35_ro359b_if",                          False,      0x0000,     "bin"       ),
    ("k20dx_rbl_if",                                True,       0x5000,     "bin"       ),
    ("k20dx_rblnano_if",                            True,       0x5000,     "bin"       ),
    ("lpc11u35_archlink_if",                        False,      0x0000,     "bin"       ),
    ("lpc11u35_tiny_if",                            False,      0x0000,     "bin"       ),
    ("lpc11u35_c027_if",                            False,      0x0000,     "bin"       ),
    ("sam3u2c_ncs36510rf_if",                       False,      0x8000,     "bin"       ),
    ("kl26z_nina_b1_if",                            False,      0x8000,     "bin"       ),
    ("lpc11u35_lpc4088dm_if",                       False,      0x0000,     "bin"       ),
    ("lpc11u35_lpc4088qsb_if",                      False,      0x0000,     "bin"       ),
    ('lpc11u35_ssci_chibi_if',                      False,      0x0000,     "bin"       ),
    ('lpc11u35_ff1705_l151_if',                     False,      0x0000,     "bin"       ),
    ('lpc11u35_ff_lpc546xx_if',                     False,      0x0000,     "bin"       ),
    ("k20dx_xdot_l151_if",                          False,      0x8000,     "bin"       ),
    ('lpc11u35_6lowpan_borderrouterhat_if',         None,       0x0000,     "bin"       ),
    ('lpc11u35_6lowpan_borderrouterusb_if',         None,       0x0000,     "bin"       ),
    ('lpc11u35_6lowpan_borderrouterethernet_if',    None,       0x0000,     "bin"       ),
    ('lpc11u35_gr_peach_if',                        None,       0x0000,     "bin"       ),
    ('lpc11u35_gr_lychee_if',                       None,       0x0000,     "bin"       ),
    ('lpc11u35_nz32_sc151_if',                      False,      0x0000,     "bin"       ),
    ('lpc11u35_wizwiki_w7500_if',                   False,      0x0000,     "bin"       ),
    ('lpc11u35_wizwiki_w7500_eco_if',               False,      0x0000,     "bin"       ),
    ('lpc11u35_wizwiki_w7500p_if',                  False,      0x0000,     "bin"       ),
    ("sam3u2c_ublox_evk_nina_b1_if",                True,       0x5000,     "bin"       ),
    ("lpc11u35_vbluno51_if",                        False,      0x0000,     "bin"       ),
    ('lpc11u35_mtconnect04s_if',                    False,      0x0000,     "bin"       ),
    ('lpc11u35_mbed_cloud_connect_if',              False,      0x0000,     "bin"       ),
    ("lpc11u35_rtl8195am_if",                       False,      0x0000,     "bin"       ),
    ("lpc11u35_96b_nitrogen_if",                    False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_mxchip_emw3166_if',              False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_nina_b112_if',                   False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_murata_abz_078_if',              False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_rak811_if',                      False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_wise1510_if',                    False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_wise1530_if',                    False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_wise1570_if',                    False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_laird_bl652_if',                 False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_usi_wm_bn_bm_22_if',             False,      0x0000,     "bin"       ),
    ('lpc11u35_dipdap_sdt52832b_if',                False,      0x0000,     "bin"       ),
    ('lpc11u35_dipdap_sdt32429b_if',                False,      0x0000,     "bin"       ),
    ('lpc11u35_dipdap_sdt32439b_if',                False,      0x0000,     "bin"       ),
    ('lpc11u35_dipdap_sdt64b_if',                   False,      0x0000,     "bin"       ),
    ('lpc11u35_dipdap_sdt51822b_if',                False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_ublox_odin_w2_if',               False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_mts_dragonfly_if',               False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_mts_xdot_if',                    False,      0x0000,     "bin"       ),
    ('lpc11u35_dipdap_cc3220sf_if',                 False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_laird_bl654_if',                 False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_murata_bl241_if',                False,      0x0000,     "bin"       ),
    ('lpc11u35_mtb_stm_s2lp_if',                    False,      0x0000,     "bin"       ),
}

# All supported configurations
SUPPORTED_CONFIGURATIONS = [
    #   Board ID    Firmware                                    Bootloader          Target
    (   0x200,      'k20dx_frdmkl25z_if',                       'k20dx_bl',         'KL25Z'                                 ),
    (   0x203,      'k20dx_twrkl28z72m_if',                     'k20dx_bl',         None                                    ),
    (   0x204,      'k20dx_frdmkl02z_if',                       'k20dx_bl',         None                                    ),
    (   0x205,      'k20dx_frdmkl28z_if',                       'k20dx_bl',         None                                    ),
    (   0x206,      'k20dx_twrke18f_if',                        'k20dx_bl',         None                                    ),
    (   0x210,      'k20dx_frdmkl05z_if',                       'k20dx_bl',         'FRDM-KL05Z'                            ),
    (   0x213,      'k20dx_frdmke15z_if',                       'k20dx_bl',         None                                    ),
    (   0x216,      'k20dx_hvpke18f_if',                        'k20dx_bl',         None                                    ),
    (   0x217,      'k20dx_frdmk82f_if',                        'k20dx_bl',         'FRDM-K82F'                             ),
    (   0x220,      'k20dx_frdmkl46z_if',                       'k20dx_bl',         'FRDM-KL46Z'                            ),
    (   0x224,      'k20dx_frdmk28f_if',                        'k20dx_bl',         None                                    ), # TODO - set target to 'FRDM-K28F' when mbed-os supports this
    (   0x226,      'k20dx_mimxrt1020_evk_qspi_if',             'k20dx_bl',         None                                    ), # TODO - set target to 'MIMX6RT1050' when mbed-os supports this
    (   0x227,      'k20dx_mimxrt1050_evk_hyper_if',            'k20dx_bl',         None                                    ), # TODO - set target to 'MIMX6RT1050' when mbed-os supports this
    (   0x227,      'k20dx_mimxrt1050_evk_qspi_if',             'k20dx_bl',         None                                    ), # TODO - set target to 'MIMX6RT1050' when mbed-os supports this
    (   0x230,      'k20dx_frdmk20dx_if',                       'k20dx_bl',         'FRDM-K20D50M'                          ),
    (   0x231,      'k20dx_frdmk22f_if',                        'k20dx_bl',         'FRDM-K22F'                             ),
    (   0x240,      'k20dx_frdmk64f_if',                        'k20dx_bl',         'FRDM-K64F'                             ),
    (   0x214,      'k20dx_hexiwear_if',                        'k20dx_bl',         'HEXIWEAR'                              ),
    (   0x228,      'k20dx_rapid_iot_if',                       'k20dx_bl',         None                                    ), # TODO - set target to 'Rapid-IoT' when mbed-os supports this
    (   0x260,      'k20dx_frdmkl26z_if',                       'k20dx_bl',         None                                    ),
    (   0x261,      'k20dx_frdmkl27z_if',                       'k20dx_bl',         'FRDM-KL27Z'                            ),
    (   0x262,      'k20dx_frdmkl43z_if',                       'k20dx_bl',         'FRDM-KL43Z'                            ),
    (   0x250,      'k20dx_frdmkw24d_if',                       'k20dx_bl',         'FRDM-KW24D512'                         ),
    (   0x201,      'k20dx_frdmkw41z_if',                       'k20dx_bl',         'FRDM-KW41Z'                            ),
    (   0x218,      'k20dx_frdmkl82z_if',                       'k20dx_bl',         'FRDM-KL82Z'                            ),
    (   0x311,      'k20dx_frdmk66f_if',                        'k20dx_bl',         'FRDM-K66F'                             ),
    (   0x350,      'k20dx_xdot_l151_if',                       'k20dx_bl',         None                                    ), # TODO - set target to 'MTS-xDot-L151CC' when mbed-os supports this
    (   0x0450,     'lpc11u35_mtb_ublox_odin_w2_if',            None,               None                                    ),
    (   0x0451,     'lpc11u35_mtb_mxchip_emw3166_if',           None,               None                                    ), # TODO - set target when mbed-os supports this
    (   0x0453,     'lpc11u35_mtb_mts_xdot_if',                 None,               None                                    ),
    (   0x0454,     'lpc11u35_mtb_mts_dragonfly_if',            None,               None                                    ),
    (   0x0455,     'lpc11u35_mtb_nina_b112_if',                None,               None                                    ), # TODO - set target when mbed-os supports this
    (   0x0456,     'lpc11u35_mtb_murata_abz_078_if',           None,               None                                    ), # TODO - set target when mbed-os supports this
    (   0x0457,     'lpc11u35_mtb_rak811_if',                   None,               None                                    ), # TODO - set target when mbed-os supports this
    (   0x458,      'lpc11u35_mtb_wise1510_if',                 None,               None                                    ), # TODO - set target when mbed-os supports this
    (   0x459,      'lpc11u35_mtb_wise1530_if',                 None,               None                                    ), # TODO - set target when mbed-os supports this
    (   0x460,      'lpc11u35_mtb_wise1570_if',                 None,               None                                    ), # TODO - set target when mbed-os supports this
    (   0x0461,     'lpc11u35_mtb_laird_bl652_if',              None,               None                                    ), # TODO - set target when mbed-os supports this
    (   0x0462,     'lpc11u35_mtb_usi_wm_bn_bm_22_if',          None,               None                                    ), # TODO - set target when mbed-os supports this
    (   0x0465,     'lpc11u35_mtb_laird_bl654_if',              None,               None                                    ), # TODO - set target when mbed-os supports this
    (   0x0466,     'lpc11u35_mtb_murata_bl241_if',             None,               None                                    ), # TODO - set target when mbed-os supports this
    (   0x0467,     'lpc11u35_mtb_stm_s2lp_if',                 None,               None                                    ), # TODO - set target when mbed-os supports this
    (   0x8080,     'lpc11u35_ff1705_l151_if',                  None,               None                                    ), # TODO - set target to 'L-TEK-FF1705' when mbed-os supports this
    (   0x8081,     'lpc11u35_ff_lpc546xx_if',                  None,               None                                    ), # TODO - set target to 'L-TEK-FF-LPC546XX' when mbed-os supports this
    (   0x700,      'stm32f103xb_stm32f103rb_if',               'stm32f103xb_bl',   'ST-Nucleo-F103RB'                      ),
    (   0x720,      'stm32f103xb_stm32f401re_if',               'stm32f103xb_bl',   'ST-Nucleo-F401RE'                      ),
    (   0x720,      'stm32f103xb_stm32f072rb_if',               'stm32f103xb_bl',   'ST-Nucleo-F072RB'                      ),
    (   0x735,      'stm32f103xb_stm32f334r8_if',               'stm32f103xb_bl',   'ST-Nucleo-F334R8'                      ),
    (   0x740,      'stm32f103xb_stm32f411re_if',               'stm32f103xb_bl',   'ST-Nucleo-F411RE'                      ),
    (   0x765,      'stm32f103xb_stm32l476rg_if',               'stm32f103xb_bl',   'ST-Nucleo-L476RG'                      ),
    (   0x796,      'stm32f103xb_stm32f429zi_if',               'stm32f103xb_bl',   'ST-Nucleo-F429ZI'                      ),
    (   0x816,      'stm32f103xb_stm32f746zg_if',               'stm32f103xb_bl',   'ST-Nucleo-F746ZG'                      ),
    (   0x824,      'lpc11u35_lpc824xpresso_if',                None,               'LPCXpresso824-MAX'                     ),
    (   0x835,      'stm32f103xb_stm32f207zg_if',               'stm32f103xb_bl',   'ST-Nucleo-F207ZG'                      ),
    (   0x1017,     'lpc11u35_hrm1017_if',                      None,               'mbed-HRM1017'                          ),
    (   0x1018,     'lpc11u35_ssci824_if',                      None,               'Switch-Science-mbed-LPC824'            ),
    (   0x1019,     'lpc11u35_sscity_if',                       None,               'Switch-Science-mbed-TY51822r3'         ),
    (   0x1022,     'lpc11u35_ro359b_if',                       None,               None                                    ),
    (   0x1050,     'lpc11u35_lpc812xpresso_if',                None,               'NXP-LPC800-MAX'                        ),
    (   0x1054,     'lpc4322_lpc54114xpresso_if',               'lpc4322_bl',       'LPCXpresso54114'                       ),
    (   0x1056,     'lpc4322_lpc54608xpresso_if',               'lpc4322_bl',       'LPCXpresso54608'                       ),
    (   0x1060,     'lpc11u35_lpc4088qsb_if',                   None,               'EA-LPC4088'                            ),
    (   0x1062,     'lpc11u35_lpc4088dm_if',                    None,               'EA-LPC4088-Display-Module'             ),
    (   0x1070,     'sam3u2c_mkit_dk_dongle_nrf5x_if',          'sam3u2c_bl',       'Nordic-nRF51822'                       ),
    (   0x1090,     'k20dx_rbl_if',                             'k20dx_bl',         'RedBearLab-nRF51822'                   ),
    (   0x1095,     'k20dx_rblnano_if',                         'k20dx_bl',         'RedBearLab-BLE-Nano'                   ),
    (   0x1100,     'sam3u2c_mkit_dk_dongle_nrf5x_if',          'sam3u2c_bl',       'Nordic-nRF51-DK'                       ),
    (   0x1101,     'sam3u2c_mkit_dk_dongle_nrf5x_if',          'sam3u2c_bl',       'Nordic-nRF52-DK'                       ),
    (   0x1102,     'sam3u2c_mkit_dk_dongle_nrf5x_if',          'sam3u2c_bl',       'Nordic-nRF52840-DK'                    ),
    (   0x1021,     'lpc11u35_ssci_chibi_if',                   None,               None                                    ), # TODO - set target to 'SSCI-MBIT' when mbed-os supports this
    (   0x1114,     'lpc11u35_ssci1114_if',                     None,               'LPC1114FN28'                           ),
    (   0x1120,     'sam3u2c_mkit_dk_dongle_nrf5x_if',          'sam3u2c_bl',       'Nordic-nRF51-Dongle'                   ),
    (   0x1234,     'lpc11u35_c027_if',                         None,               'u-blox-C027'                           ),
    (   0x1200,     'sam3u2c_ncs36510rf_if',                   'sam3u2c_bl',        None                                    ),# TODO - Set to 'ncs36510' when non-zero flash addresses are supported
    (   0x1236,     'stm32f103xb_ublox_evk_odin_w2_if',        'stm32f103xb_bl',    'ublox-EVK-ODIN-W2'                     ),
    (   0x1238,     'kl26z_nina_b1_if',                        'kl26z_bl',          'u-blox-NINA-B1'                        ),
    (   0x2410,     'lpc11u35_mbed_cloud_connect_if',           None,               None                                    ),
    (   0x4600,     'lpc11u35_rtl8195am_if',                    None,               'REALTEK-RTL8195AM'                     ),
    (   0x5050,     'lpc11u35_arm_watch_stm32f411_if',          None,               None                                    ),
    (   0x5051,     'lpc11u35_arm_watch_efm32_if',              None,               None                                    ),
    (   0x5052,     'lpc11u35_arm_watch_nrf51_if',              None,               None                                    ),
    (   0x7010,     'lpc11u35_blueninja_if',                    None,               None                                    ), # TODO - set target to 'BlueNinja-CDP-TZ01B' when mbed-os supports this
    (   0x9004,     'lpc11u35_archpro_if',                      None,               'Seeeduino-Arch-Pro'                    ),
    (   0x9009,     'lpc11u35_archble_if',                      None,               'Seeed-Arch-BLE'                        ),
    (   0x9011,     'lpc11u35_archmax_if',                      None,               'Seeed-Arch-Max'                        ),
    (   0x9012,     'lpc11u35_tiny_if',                         None,               'Seeed-Tiny-BLE'                        ),
    (   0x9013,     'lpc11u35_archlink_if',                     None,               'Seeed-Arch-Link'                       ),
    (   0x9014,     'lpc11u35_96b_nitrogen_if',                 None,               None                                    ), # TODO - set target to 'Seeed-96Boards-Nitrogen' when mbed-os supports this
    (   0x9900,     'kl26z_microbit_if',                        'kl26z_bl',         'Microbit'                              ),
    (   0x7402,     'lpc11u35_6lowpan_borderrouterhat_if',      None,               'mbed-6LoWPAN-Border-Router-HAT'        ),
    (   0x7403,     'lpc11u35_6lowpan_borderrouterusb_if',      None,               'mbed-6LoWPAN-Border-Router-USB'        ),
    (   0x7404,     'lpc11u35_6lowpan_borderrouterethernet_if', None,               'mbed-6LoWPAN-Border-Router-ETHERNET'   ),
    (   0xC000,     'lpc11u35_cocorico_if',                     None,               'CoCo-ri-Co'                            ),
    (   0x5500,     'lpc11u35_gr_peach_if',                     None,               None                                    ), # TODO - Set to 'Renesas-GR-PEACH' once this target builds
    (   0x5501,     'lpc11u35_gr_lychee_if',                    None,               None                                    ), # TODO - Set to 'Renesas-GR-LYCHEE' once this target builds
    (   0x6660,     'lpc11u35_nz32_sc151_if',                   None,               None                                    ), # TODO - set target to 'NZ32-SC151' when mbed-os supports this
    (   0x2201,     'lpc11u35_wizwiki_w7500_if',                None,               'WIZwiki-W7500'                         ),
    (   0x2202,     'lpc11u35_wizwiki_w7500_eco_if',            None,               'WIZwiki-W7500ECO'                      ),
    (   0x2203,     'lpc11u35_wizwiki_w7500p_if',               None,               'WIZwiki-W7500P'                        ),
    (   0x1237,     'sam3u2c_ublox_evk_nina_b1_if',            'sam3u2c_bl',        'U-BLOX-EVK-NINA-B1'                    ),
    (   0xC006,     'lpc11u35_vbluno51_if',                     None,               'VBLUNO51'                              ),
    (   0xC005,     'lpc11u35_mtconnect04s_if',                 None,               'MtConnect04S'                          ),
    (   0x3104,     'lpc11u35_dipdap_sdt52832b_if',             None,               None                                    ),
    (   0x3108,     'lpc11u35_dipdap_sdt32429b_if',             None,               None                                    ),
    (   0x3110,     'lpc11u35_dipdap_sdt32439b_if',             None,               None                                    ),
    (   0x3105,     'lpc11u35_dipdap_sdt64b_if',                None,               None                                    ),
    (   0x3103,     'lpc11u35_dipdap_sdt51822b_if',             None,               None                                    ),
    (   0x3300,     'lpc11u35_dipdap_cc3220sf_if',              None,               'CC3220SF'                              ),
]

# Add new HICs here
HIC_STRING_TO_ID = {
    'k20dx': 0x97969900,
    'kl26z': 0x97969901,
    'lpc11u35': 0x97969902,
    'sam3u2c': 0x97969903,
    'lpc4322': 0x97969905,
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
    0x0261,  # FRDM-KL27Z
    0x0262,  # FRDM-KL43Z
    0x0218,  # FRDM-KL82Z
    0x1022,  # RO359B (K24F)
    0x0201,  # KW41Z
])

BOARD_ID_SUPPORTING_PAGE_ERASE = set([
    0x0214,  # HEXIWEAR
    0x0226,  # MIMXRT1020 EVK
    0x0227,  # MIMXRT1050 EVK
    0x0240,  # K64F
    0x0311,  # K66F
    0x0450,  # mtb_ublox_odin_w2
    0x1101,  # Nordic-nRF52-DK
    0x1102,  # Nordic-nRF52840-DK
    0x3103,  # dipdap_sdt51822b
    0x3104,  # dipdap_sdt52832b
    0x3105,  # dipdap_sdt64b
    0x3108,  # dipdap_sdt32429b
    0x3110,  # dipdap_sdt32439b
    0x3300,  # dipdap_cc3220sf
    0x5500,  # GR-PEACH
    0x5501,  # GR-LYCHEE
])

#Hack until these targets have an image with a valid vector table
TARGET_WITH_BAD_VECTOR_TABLE_LIST = [
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
    'VNG-VBLUno51',
    'REALTEK-RTL8195AM']

BOARD_ID_TO_BUILD_TARGET = {config[0]: config[3] for config in
                            SUPPORTED_CONFIGURATIONS}
FIRMWARE_SET = set((config[1] for config in SUPPORTED_CONFIGURATIONS))
TARGET_SET = set((target[3] for target in SUPPORTED_CONFIGURATIONS if
                  target[3] is not None))

TARGET_WITH_COMPILE_API_LIST = [config[3] for config in
                                SUPPORTED_CONFIGURATIONS if
                                config[3] is not None]
