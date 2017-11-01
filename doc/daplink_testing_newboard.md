# Adding New Target/Board to Automated Test Suite

DAPLink source code comes with test suite that automates most of the actions performed on/by DAPLink in order to verify its proper functionality in various environments (i.e. different operating systems, targets, boards, configurations, etc).
Test suite performs intense and detailed experimentation by putting a dedicated agent firmware (so called "Target Firmware" or "UART Application") onto tested Target.
Test suite is written in Python.
**You should really add newly created Target/Board to the Automated Test Suite.**

In order for your Target/Board to be tested, you need to extend `SUPPORTED_CONFIGURATIONS` dictionary from `test/info.py` file:

```python
SUPPORTED_CONFIGURATIONS = [
    #   Board ID    Firmware                            Bootloader          Target
    ...
    (   0x240,      'k20dx_frdmk64f_if',                'k20dx_bl',         'FRDM-K64F'                     ),
    ...
]
```

where:
* `Board ID` is the unique ID assigned to the Board.
* `Firmware` is the name of the firmware as declared in `projects.yaml`.
* `Bootloader` is the name of the bootloader image as declared in `projects.yaml`. This field is only required if given HIC supports custom bootloader (not all HIC can have it).
* `Target` is the Target name related to that particular Board, but keep in mind that:
    * if this is an mbed official Board then the Target should match the name in the mbed platform page URL. For example the `K64F` is located at [https://developer.mbed.org/platforms/FRDM-K64F/][mbed_fdrm_k64f] therefore its name would be `FRDM-K64F`. This nomenclature allows automated tests to leverage RESTful API of ARM the mbed Online Compiler. The automated testing framework will build "UART Application" firmware for that Target on-the-fly using the mbed Online Compiler, download it to the PC, then flash the firmware into Target in order to perform various tests (i.e. filesystem, flashing, serial console, misconfigurations, etc).
    * if this is not an official Target/Board, you need to build the "UART Application" yourself and supply it to `test\run_tests.py` via `--targetdir` parameter. In this case, the target is the application image filename without extension.

You may need to update one or more dictionaries (see comments in the testing source code for guidance).

[mbed_fdrm_k64f]: https://developer.mbed.org/platforms/FRDM-K64F/ "ARM mbed board FDRM-K64F"
