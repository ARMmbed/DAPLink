# Testing

We are working on an Automated Test Suite, written in [Python][python], that will perform all of the necessary steps to verify proper functionality of the DAPLink and related hardware.
Test suite performs intense and detailed experimentation by putting a dedicated agent firmware (so called "Target Firmware" or "UART Application") onto tested Target.
Yes, test suite can work both on software and hardware behaviors simulating various real life situations like (corrupted) firmware upgrade, user interactions, flashing, debug, serial port, misconfigurations, various operating system versions, etc.
We have even created a dedicated laboratory environment that performs continous stress testing for corner situations with a low probabily of occurance to detect and remove even smallest glitches!

Test logic discovers connected DAPLink boards and targets only those boards. It can be run on the current development environment or a release package. Test results are written to the current directory and printed to the console.

In order to perform testing, you should get familiar with current setup in `test/info.py` (see [Adding new Target/Board for Automated Test Suite](daplink_testing_newboard.md) chapter for details) and help for `test/run_test.py` (using [Python Virtual Environment](daplink_sdk_setup.md)):
```
$ python test/run_test.py --help
usage: run_test.py [-h] [--targetdir TARGETDIR] [--user USER]
                   [--password PASSWORD] [--firmwaredir FIRMWAREDIR]
                   [--firmware {k20dx_frdmkl02z_if,lpc11u35_sscity_if,lpc11u35_vbluno51_if,k20dx_frdmkl27z_if,lpc11u35_wizwiki_w7500_eco_if,kl26z_nina_b1_if,k20dx_frdmk66f_if,k20dx_frdmk20dx_if,lpc11u35_ssci1114_if,lpc11u35_arm_watch_nrf51_if,lpc11u35_nz32_sc151_if,k20dx_xdot_l151_if,k20dx_frdmke15z_if,sam3u2c_ncs36510rf_if,lpc11u35_arm_watch_stm32f411_if,lpc11u35_lpc4088dm_if,lpc11u35_gr_peach_if,k20dx_frdmkl26z_if,k20dx_frdmk28f_if,k20dx_frdmkl46z_if,lpc11u35_lpc824xpresso_if,k20dx_frdmk82f_if,lpc11u35_wizwiki_w7500p_if,kl26z_microbit_if,lpc4322_lpc54114xpresso_if,lpc11u35_hrm1017_if,lpc11u35_lpc4088qsb_if,lpc11u35_6lowpan_borderrouterhat_if,lpc11u35_cocorico_if,lpc11u35_6lowpan_borderrouterusb_if,lpc11u35_archmax_if,lpc11u35_wizwiki_w7500_if,lpc11u35_archpro_if,lpc11u35_ssci824_if,lpc4322_lpc54608xpresso_if,k20dx_frdmkl25z_if,lpc11u35_c027_if,lpc11u35_arm_watch_efm32_if,k20dx_frdmkl05z_if,k20dx_frdmkl43z_if,lpc11u35_blueninja_if,k20dx_rblnano_if,k20dx_rbl_if,lpc11u35_ssci_chibi_if,k20dx_frdmkl82z_if,lpc11u35_tiny_if,lpc11u35_archlink_if,lpc11u35_lpc812xpresso_if,k20dx_frdmk22f_if,lpc11u35_6lowpan_borderrouterethernet_if,k20dx_frdmkw24d_if,k20dx_frdmkl28z_if,k20dx_twrke18f_if,k20dx_twrkl28z72m_if,sam3u2c_mkit_dk_dongle_nrf5x_if,k20dx_hvpke18f_if,sam3u2c_ublox_evk_nina_b1_if,lpc11u35_archble_if,k20dx_frdmk64f_if}]
                   [--logdir LOGDIR] [--noloadif] [--notestendpt] [--loadbl]
                   [--testdl] [--testfirst]
                   [--verbose {Minimal,Normal,Verbose,All}] [--dryrun]
                   [--force]

DAPLink validation and testing tool

optional arguments:
  -h, --help            show this help message and exit
  --targetdir TARGETDIR
                        Directory with pre-built target test images.
  --user USER           MBED username (required for compile-api)
  --password PASSWORD   MBED password (required for compile-api)
  --firmwaredir FIRMWAREDIR
                        Directory with firmware images to test
  --firmware {k20dx_frdmkl02z_if,lpc11u35_sscity_if,lpc11u35_vbluno51_if,k20dx_frdmkl27z_if,lpc11u35_wizwiki_w7500_eco_if,kl26z_nina_b1_if,k20dx_frdmk66f_if,k20dx_frdmk20dx_if,lpc11u35_ssci1114_if,lpc11u35_arm_watch_nrf51_if,lpc11u35_nz32_sc151_if,k20dx_xdot_l151_if,k20dx_frdmke15z_if,sam3u2c_ncs36510rf_if,lpc11u35_arm_watch_stm32f411_if,lpc11u35_lpc4088dm_if,lpc11u35_gr_peach_if,k20dx_frdmkl26z_if,k20dx_frdmk28f_if,k20dx_frdmkl46z_if,lpc11u35_lpc824xpresso_if,k20dx_frdmk82f_if,lpc11u35_wizwiki_w7500p_if,kl26z_microbit_if,lpc4322_lpc54114xpresso_if,lpc11u35_hrm1017_if,lpc11u35_lpc4088qsb_if,lpc11u35_6lowpan_borderrouterhat_if,lpc11u35_cocorico_if,lpc11u35_6lowpan_borderrouterusb_if,lpc11u35_archmax_if,lpc11u35_wizwiki_w7500_if,lpc11u35_archpro_if,lpc11u35_ssci824_if,lpc4322_lpc54608xpresso_if,k20dx_frdmkl25z_if,lpc11u35_c027_if,lpc11u35_arm_watch_efm32_if,k20dx_frdmkl05z_if,k20dx_frdmkl43z_if,lpc11u35_blueninja_if,k20dx_rblnano_if,k20dx_rbl_if,lpc11u35_ssci_chibi_if,k20dx_frdmkl82z_if,lpc11u35_tiny_if,lpc11u35_archlink_if,lpc11u35_lpc812xpresso_if,k20dx_frdmk22f_if,lpc11u35_6lowpan_borderrouterethernet_if,k20dx_frdmkw24d_if,k20dx_frdmkl28z_if,k20dx_twrke18f_if,k20dx_twrkl28z72m_if,sam3u2c_mkit_dk_dongle_nrf5x_if,k20dx_hvpke18f_if,sam3u2c_ublox_evk_nina_b1_if,lpc11u35_archble_if,k20dx_frdmk64f_if}
                        Firmware to test
  --logdir LOGDIR       Directory to log test results to
  --noloadif            Skip load step for interface.
  --notestendpt         Dont test the interface USB endpoints.
  --loadbl              Load bootloader before test.
  --testdl              Run DAPLink specific tests. The DAPLink test tests
                        bootloader updates so usewith caution
  --testfirst           If multiple boards of the same type are found only
                        test the first one.
  --verbose {Minimal,Normal,Verbose,All}
                        Verbose output
  --dryrun              Print info on configurations but dont actually run
                        tests.
  --force               Try to run tests even if there are problems. Delete
                        logs from previous run.
```

Tests are grouped into two categories:
* Tests that validate DAPLink flashing the Target and HIC. This involves automated variations of copying the firmware image file using UMS endpoint (the one that most users use for drag-and-drop). These are not triggered by default, but yout can turn them on with `--testdl` paremeter.
* Tests that validate the UMS (drag-n-drop flashing), CDC (Virtual COM Port), and HID (CMSIS-DAP) endpoints. These are triggered by default, but you can disable them with `--notestendpt` parameter.

**Note:** You really want to take a look at [UMS Commands](daplink_endpoint_ums_commands.md) that will help you a lot with automated testing!

[python]: https://www.python.org/downloads/ "Python is platform independent free and open programming language"
